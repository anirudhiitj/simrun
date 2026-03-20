#include "cache.h"

Cache::Cache(uint32_t id, const nlohmann::json& params)
    : BaseEntity(id)
{
    /* -------- Cache config -------- */
    base_cache_hit_probability =
        params.value("base_cache_hit_probability", 0.7);

    base_cache_hit_latency =
        params.value("base_cache_hit_latency", 0.3);

    base_cache_miss_latency =
        params.value("base_cache_miss_latency", 0.1);

    /* -------- Initial runtime state -------- */
    hits = 0;
    misses = 0;
}
