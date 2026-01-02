#pragma once
#include "../events/event_queue.h"
#include "scheduler.h"

class Context;
class State;

class Simulator {
private:
    SimTime current_time = 0;

    EventQueue& queue;
    EventScheduler scheduler;

    const Context& context;
    State& state;

public:
    Simulator(
        EventQueue& q,
        const Context& ctx,
        State& st
    );

    void run();
    SimTime now() const;
};
