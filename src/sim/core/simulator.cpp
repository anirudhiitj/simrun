#include "simulator.h"
#include "event_queue.h"
#include "../events/events.h"
#include "../src/simulation_context.h"

Simulator::Simulator(
    EventQueue& q,
    SimulationContext& ctx
)
    : queue(q),
      scheduler(q),
      ctx(ctx) {}

void Simulator::run() {
    while (!queue.empty()) {
        auto event = queue.pop();

        current_time = event->timestamp;

        event->execute(ctx, scheduler);
    }
}

SimTime Simulator::now() const {
    return current_time;
}
