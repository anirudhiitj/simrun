// events/api_events.cpp
#include "api_events.h"
#include "../entities/service.h"
#include "../src/simulation_context.h"
#include "../core/scheduler.h"
#include "../core/rng.h"
#include <memory>

/* ---------------- RequestArrivalEvent ---------------- */

RequestArrivalEvent::RequestArrivalEvent(
    SimTime t,
    Service* svc
) : service(svc)
{
    type = EventType::REQUEST_ARRIVAL;
    timestamp = t;
}

void RequestArrivalEvent::execute(SimulationContext& ctx, EventScheduler& scheduler) {
    // Case 1: Can process immediately
    if (service->active_requests < service->max_concurrency) {
        service->active_requests++;

        scheduler.schedule(
            std::make_unique<RequestProcessingEvent>(timestamp, service)
        );
        return;
    }

    // Case 2: Queue has space
    if (!service->queue_full()) {
        service->enqueue_request(nullptr); // TODO: attach Request object
        return;
    }

    // Case 3: Reject request
    service->rejected_requests++;
}

/* ---------------- RequestProcessingEvent ---------------- */

RequestProcessingEvent::RequestProcessingEvent(
    SimTime t,
    Service* svc
) : service_(svc)
{
    type = EventType::REQUEST_PROCESSING;
    timestamp = t;
}

void RequestProcessingEvent::execute(SimulationContext& ctx, EventScheduler& scheduler) {
    // 1. Sample processing time
    SimTime processing_time = service_->sample_latency(seed);

    // TODO: schedule next hop (network event) when routing is implemented

    // 2. Update concurrency
    service_->active_requests--;

    // 3. Pull from queue if exists
    if (!service_->queue_empty()) {
        service_->dequeue_request();
        service_->active_requests++;

        scheduler.schedule(
            std::make_unique<RequestProcessingEvent>(timestamp, service_)
        );
    }
}

