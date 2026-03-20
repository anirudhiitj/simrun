#pragma once
#include "../core/sim_types.h"
#include "event_types.h"

class SimulationContext;
class EventScheduler;

class Event {
public:
    EventType type  = EventType::UNKNOWN;
    SimTime timestamp = 0;
    uint64_t seed     = 0;

    virtual ~Event() = default;

    SimTime time() const { return timestamp; }

    virtual void execute(SimulationContext& ctx, EventScheduler& scheduler) = 0;
};
