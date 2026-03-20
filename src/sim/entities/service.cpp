#include "service.h"
#include "../core/rng.h"

Service::Service(uint32_t id, const nlohmann::json& params)
    : BaseEntity(id)
{
    /* -------- Latency params -------- */
    latency_dist          = params.value("dist_latency", std::string("lognormal"));
    base_median_latency   = params.value("base_median_latency", 30.0);
    base_variance_latency = params.value("base_variance_latency", 0.8);

    /* -------- Capacity params -------- */
    max_concurrency = params.value("max_concurrency", 100u);
    queue_capacity  = params.value("queue_capacity", 300u);

    /* -------- Initial state -------- */
    active_requests = 0;
}

Request* Service::dequeue_request() {
    Request* r = wait_queue.front();
    wait_queue.pop();
    return r;
}

SimTime Service::sample_latency(uint64_t& seed) const {
    double latency = 0.0;
    if (latency_dist == "lognormal") {
        latency = lognormal_dist(base_median_latency, base_variance_latency, seed);
    } else {
        latency = normal_dist(base_median_latency, base_variance_latency, seed);
    }
    if (latency < 0.0) latency = 0.0;
    return static_cast<SimTime>(latency);
}
