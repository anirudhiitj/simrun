#pragma once
#include "sim_types.h"
#include "scheduler.h"

class EventQueue;
class SimulationContext;

class Simulator {
private:
    SimTime current_time = 0;

    EventQueue& queue;
    EventScheduler scheduler;

    SimulationContext& ctx;

public:
    Simulator(
        EventQueue& q,
        SimulationContext& ctx
    );

    void run();
    SimTime now() const;
};
