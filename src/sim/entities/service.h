#pragma once

#include <cstdint>
#include <queue>
#include <string>
#include <nlohmann/json.hpp>

#include "../core/base_entity.h"
#include "../core/sim_types.h"

struct Request;

class Service : public BaseEntity {
public:
    explicit Service(uint32_t id, const nlohmann::json& params);

    /* ---------- latency config ---------- */
    std::string latency_dist;
    double base_median_latency  = 0.0;
    double base_variance_latency = 0.0;

    /* ---------- capacity ---------- */
    uint32_t max_concurrency = 0;
    uint32_t queue_capacity  = 0;

    /* ---------- runtime state ---------- */
    uint32_t active_requests = 0;
    std::queue<Request*> wait_queue;

    /* ---------- stats ---------- */
    uint64_t rejected_requests = 0;

    /* ---------- helpers ---------- */
    bool queue_full() const  { return static_cast<uint32_t>(wait_queue.size()) >= queue_capacity; }
    bool queue_empty() const { return wait_queue.empty(); }
    void enqueue_request(Request* r) { wait_queue.push(r); }
    Request* dequeue_request();
    SimTime sample_latency(uint64_t& seed) const;
};
