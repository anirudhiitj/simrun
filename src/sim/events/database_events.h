#pragma once

#include <cstdint>
#include "events.h"
#include "event_types.h"

class Database;
class EventScheduler;
struct Request;

/* ================= Utility functions ================= */

void db_update_tokens(Database* db, double now);

double db_sample_seek_latency(
    Database* db,
    uint64_t& seed
);

void db_reject_request(
    Database* db,
    Request* req,
    double now
);

void db_try_dispatch(
    Database* db,
    SimulationContext& ctx,
    EventScheduler& scheduler,
    double now,
    uint64_t& seed
);

/* ================= Events ================= */

struct DBRequestArrivalEvent : public Event {
    Database* db;
    Request* request;

    DBRequestArrivalEvent(
        double ts,
        uint64_t seed,
        Database* db,
        Request* req
    );

    void execute(SimulationContext& ctx, EventScheduler& scheduler) override;
};

struct DBRequestSendEvent : public Event {
    Database* db;
    Request* request;

    DBRequestSendEvent(
        double ts,
        uint64_t seed,
        Database* db,
        Request* req
    );

    void execute(SimulationContext& ctx, EventScheduler& scheduler) override;
};
