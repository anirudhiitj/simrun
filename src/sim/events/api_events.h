// events/api_events.h
#pragma once

#include "events.h"
#include <memory>

class Service;

/*
 * Event 1: RequestArrival
 * - Checks concurrency limit
 * - If possible → schedule RequestProcessing
 * - Else enqueue
 * - If queue full → reject
 */
class RequestArrivalEvent : public Event {
public:
    RequestArrivalEvent(SimTime t, Service* service);

    void execute(SimulationContext& ctx, EventScheduler& scheduler) override;

private:
    Service* service;
};


/*
 * Event 2: RequestProcessing
 * - Calculates processing time
 * - Updates concurrency
 * - Advances simulation by scheduling completion/network event
 */
class RequestProcessingEvent : public Event {
public:
    RequestProcessingEvent(SimTime t, Service* service);

    void execute(SimulationContext& ctx, EventScheduler& scheduler) override;

private:
    Service* service_;
};
