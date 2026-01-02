// NOTE:
// DES model: time advances only via scheduling of future events.
// Simulation time does NOT advance based on execution or wall-clock time.
// Latency and failure oracles are consulted inside event execution logic to decide timestamps of future events
// to decide the timestamps of those future events.
// The simulator itself remains policy-free and simply jumps to e->time.

#include "simulator.h"
#include "../events/event.h"

Simulator::Simulator(
    EventQueue& q,
    const Context& ctx,
    State& st
)
    : queue(q),
      scheduler(q),
      context(ctx),
      state(st) {}

void Simulator::run() {
    while (!queue.empty()) {
        Event* e = queue.pop();

        //SimTime just jumps to the timestamp of current event about to occur
        current_time = e->time;

        // Simulator is dumb and just calls the execute function
        e->execute(context, state, scheduler);

        delete e;  // later: pool allocator
    }
}

SimTime Simulator::now() const {
    return current_time;
}

