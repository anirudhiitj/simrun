#include "network_link_event.h"
#include "../entities/network_link.h"
#include "../entities/request.h"
#include "../src/simulation_context.h"
#include "../core/scheduler.h"
#include <memory>

RequestArrivalAtLinkEvent::RequestArrivalAtLinkEvent(
    SimTime t,
    uint32_t link_id_,
    uint32_t dst_id,
    Request* req
) : link_id(link_id_), dst_component_id(dst_id), request(req)
{
    type = EventType::NETWORK_LINK_ARRIVAL;
    timestamp = t;
}

void RequestArrivalAtLinkEvent::execute(SimulationContext& ctx, EventScheduler& scheduler)
{
    auto it = ctx.links.find(link_id);
    if (it == ctx.links.end()) return;
    NetworkLink& link = *it->second;

    /* ---------- queue handling ---------- */
    if (static_cast<uint32_t>(link.queue.size()) >= link.queue_capacity) {
        request->mark_dropped();
        return;
    }

    link.queue.push(request);

    /* ---------- constant propagation latency ---------- */
    double propagation_latency = link.base_median_latency;

    /* ---------- serialization delay (bandwidth-limited) ---------- */
    double serialization_delay_ms = 0.0;
    if (link.bandwidth_mbps > 0.0) {
        serialization_delay_ms =
            (link.packet_size_bytes * 8.0) /
            (link.bandwidth_mbps * 1e6) * 1000.0;
    }

    SimTime total_delay = static_cast<SimTime>(propagation_latency + serialization_delay_ms);

    /* ---------- dequeue (single-packet abstraction) ---------- */
    link.queue.pop();

    /* ---------- schedule arrival at destination component ---------- */
    // TODO: create appropriate arrival event based on destination component type
    (void)dst_component_id;
    (void)total_delay;
}
