#include "database_events.h"

#include "../entities/database.h"
#include "../entities/request.h"
#include "../core/scheduler.h"
#include "../core/rng.h"
#include "../src/simulation_context.h"

#include <algorithm>
#include <memory>

/* ================= Utilities ================= */

void db_update_tokens(Database* db, double now) {
    if (now <= db->last_token_update_ts)
        return;

    double delta = now - db->last_token_update_ts;
    uint32_t new_tokens =
        static_cast<uint32_t>(delta * db->max_iops);

    if (new_tokens == 0)
        return;

    db->tokens = std::min(
        db->bucket_capacity,
        db->tokens + new_tokens
    );

    db->last_token_update_ts +=
        static_cast<double>(new_tokens) / db->max_iops;
}

double db_sample_seek_latency(
    Database* db,
    uint64_t& seed
) {
    double latency = 0.0;

    if (db->seek_model == "normal") {
        latency = normal_dist(
            db->base_median_seek_ms,
            db->base_variance_seek_ms,
            seed
        );
    } else {
        latency = lognormal_dist(
            db->base_median_seek_ms,
            db->base_variance_seek_ms,
            seed
        );
    }

    if (latency < 0.0)
        latency = 0.0;

    seed = next_seed(seed);
    return latency;
}

void db_reject_request(
    Database* db,
    Request* req,
    double now
) {
    db->rejected_requests++;
    req->status = RequestStatus::REJECTED;
    req->finish_time = static_cast<SimTime>(now);
}

void db_try_dispatch(
    Database* db,
    SimulationContext& ctx,
    EventScheduler& scheduler,
    double now,
    uint64_t& seed
) {
    if (db->queue.empty())
        return;

    Request* req = db->queue.front();
    uint32_t required_tokens = req->is_write ? 2 : 1;

    if (db->active_requests >= db->max_concurrency ||
        db->tokens < required_tokens)
        return;

    db->queue.pop();
    db->tokens -= required_tokens;
    db->active_requests++;

    double latency = db_sample_seek_latency(db, seed);

    auto ev = std::make_unique<DBRequestSendEvent>(
        now + latency, seed, db, req
    );
    scheduler.schedule(std::move(ev));
}

/* ================= Constructors ================= */

DBRequestArrivalEvent::DBRequestArrivalEvent(
    double ts,
    uint64_t seed_,
    Database* db_,
    Request* req_
) {
    type = EventType::DB_REQUEST_ARRIVAL;
    timestamp = static_cast<SimTime>(ts);
    seed = seed_;
    db = db_;
    request = req_;
}

DBRequestSendEvent::DBRequestSendEvent(
    double ts,
    uint64_t seed_,
    Database* db_,
    Request* req_
) {
    type = EventType::DB_REQUEST_SEND;
    timestamp = static_cast<SimTime>(ts);
    seed = seed_;
    db = db_;
    request = req_;
}

/* ================= Event execution ================= */

void DBRequestArrivalEvent::execute(SimulationContext& ctx, EventScheduler& scheduler) {
    db_update_tokens(db, static_cast<double>(timestamp));

    uint32_t required_tokens = request->is_write ? 2 : 1;

    if (db->active_requests < db->max_concurrency &&
        db->tokens >= required_tokens) {

        db->tokens -= required_tokens;
        db->active_requests++;

        double latency =
            db_sample_seek_latency(db, seed);

        auto ev = std::make_unique<DBRequestSendEvent>(
            static_cast<double>(timestamp) + latency,
            seed,
            db,
            request
        );
        scheduler.schedule(std::move(ev));
        return;
    }

    if (static_cast<uint32_t>(db->queue.size()) < db->queue_capacity) {
        db->queue.push(request);
        return;
    }

    db_reject_request(db, request, static_cast<double>(timestamp));
}

void DBRequestSendEvent::execute(SimulationContext& ctx, EventScheduler& scheduler) {
    db->active_requests--;

    db_update_tokens(db, static_cast<double>(timestamp));

    db_try_dispatch(
        db,
        ctx,
        scheduler,
        static_cast<double>(timestamp),
        seed
    );

    request->db_finish_time = timestamp;
    request->seed = seed;

    Event* next = request->create_next_event(timestamp, seed);
    if (next) {
        scheduler.schedule(std::unique_ptr<Event>(next));
    }
}
