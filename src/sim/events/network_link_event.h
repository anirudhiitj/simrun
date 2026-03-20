#pragma once

#include "events.h"
#include <cstdint>

struct Request;

class RequestArrivalAtLinkEvent : public Event {
public:
    RequestArrivalAtLinkEvent(
        SimTime time,
        uint32_t link_id,
        uint32_t dst_component_id,
        Request* req
    );

    void execute(SimulationContext& ctx, EventScheduler& scheduler) override;

private:
    uint32_t link_id;
    uint32_t dst_component_id;
    Request* request;
};
