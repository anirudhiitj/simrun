#pragma once
#include <cstdint>
#include <queue>
#include <nlohmann/json.hpp>

#include "../core/base_entity.h"

struct Request;

class NetworkLink : public BaseEntity {
public:
    /* ---------- topology ---------- */
    uint32_t from;
    uint32_t to;

    /* ---------- configuration ---------- */
    double   base_median_latency  = 0.0;
    double   base_variance_latency = 0.0;
    double   bandwidth_mbps       = 0.0;
    uint32_t packet_size_bytes    = 0;
    uint32_t queue_capacity       = 0;

    /* ---------- runtime state ---------- */
    bool     is_down   = false;
    uint32_t in_flight  = 0;
    std::queue<Request*> queue;

    NetworkLink(
        uint32_t id,
        uint32_t from_id,
        uint32_t to_id,
        const nlohmann::json& config
    )
        : BaseEntity(id),
          from(from_id),
          to(to_id)
    {
        base_median_latency  = config.value("base_median_latency", 0.0);
        base_variance_latency = config.value("base_variance_latency", 0.0);
        bandwidth_mbps       = config.value("base_bandwidth_mbps", 0.0);
        packet_size_bytes    = config.value("base_packet_size_bytes", 0u);
        queue_capacity       = config.value("queue_capacity", 0u);
    }
};
