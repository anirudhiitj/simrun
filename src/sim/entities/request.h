#pragma once
#include <cstdint>
#include "../core/sim_types.h"

enum class RequestStatus {
    PENDING,
    PROCESSING,
    COMPLETED,
    REJECTED,
    DROPPED,
    TIMED_OUT
};

class Event;

struct Request {
    uint32_t id        = 0;
    uint32_t target_id = 0;

    bool is_write = false;
    RequestStatus status = RequestStatus::PENDING;

    SimTime start_time     = 0;
    SimTime finish_time    = 0;
    SimTime db_finish_time = 0;

    uint64_t seed = 0;

    void mark_dropped() { status = RequestStatus::DROPPED; }

    /* Override point: create the next event in the request's route.
       Returns nullptr when no more hops remain. */
    Event* create_next_event(SimTime now, uint64_t s) {
        (void)now; (void)s;
        return nullptr; // TODO: implement routing
    }
};
