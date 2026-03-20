#include "database.h"
#include <nlohmann/json.hpp>

Database::Database(uint32_t id, const nlohmann::json& params)
    : BaseEntity(id)
{
    /* ---------- Seek latency config ---------- */
    seek_model = params.value("seek_model", std::string("lognormal"));
    base_median_seek_ms =
        params.value("base_median_seek_ms", 4.0);
    base_variance_seek_ms =
        params.value("base_variance_seek_ms", 0.5);

    /* ---------- Token bucket config ---------- */
    max_iops =
        params.value("max_iops", 3000u);
    bucket_capacity =
        params.value("bucket_capacity", 6000u);

    /* ---------- Concurrency / queue config ---------- */
    max_concurrency =
        params.value("max_concurrency", 1000u);
    queue_capacity =
        params.value("queue_capacity", 3000u);

    /* ---------- Runtime state ---------- */
    active_requests = params.value("active", 0u);
    tokens = params.value("initial_tokens", bucket_capacity);
    last_token_update_ts = 0.0;

    rejected_requests = 0;
}
