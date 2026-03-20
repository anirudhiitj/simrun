#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

#include "../core/base_entity.h"
#include "../entities/network_link.h"

class SimulationContext {
public:
    SimulationContext(uint64_t seed_, const std::string& time_unit_)
        : seed(seed_), time_unit(time_unit_) {}

    /* ---------- Global State ---------- */
    uint64_t seed;
    std::string time_unit;

    /* ---------- World ---------- */
    std::unordered_map<uint32_t, std::unique_ptr<BaseEntity>> components;
    std::unordered_map<uint32_t, std::unique_ptr<NetworkLink>> links;

    /* ---------- Requests ---------- */
    std::unordered_map<std::string, nlohmann::json> request_types;
};
