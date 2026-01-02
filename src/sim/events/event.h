#pragma once
#include <cstdint>

class Context;
class State;
class EventScheduler;

using SimTime = uint64_t;

class Event {
public:
    SimTime time;

    explicit Event(SimTime t) : time(t) {}
    virtual ~Event() = default;

    virtual void execute(
        const Context& context,
        State& state,
        EventScheduler& scheduler
    ) = 0;
};
