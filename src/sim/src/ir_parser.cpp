#include "ir_parser.h"
#include <fstream>
#include <string>

IR IRParser::parseFromFile(const std::string& path) {
    std::ifstream f(path);
    json j;
    f >> j;

    IR ir;

    /* ---------- Header ---------- */
    ir.header.ir_version = j["header"]["ir_version"];
    ir.header.engine_version = j["header"]["engine_version"];
    ir.header.seed = j["header"]["seed"];
    ir.header.time_unit = j["header"]["time_unit"];

    /* ---------- Components ---------- */
    for (auto& c : j["context"]["components"]) {
        ComponentType ctype = ComponentType::UNKNOWN;
        std::string type_str = c.value("type", "");
        if (type_str == "api/service" || type_str == "service" || type_str == "api") {
            ctype = ComponentType::SERVICE;
        } else if (type_str == "database") {
            ctype = ComponentType::DATABASE;
        } else if (type_str == "cache") {
            ctype = ComponentType::CACHE;
        }

        ir.components.push_back({
            c["id"],
            ctype,
            c["config"]
        });
    }

    /* ---------- Links ---------- */
    for (auto& l : j["context"]["links"]) {
        ir.links.push_back({
            l["id"],
            l["from"],
            l["to"],
            l["config"]
        });
    }

    /* ---------- Initial State ---------- */
    if (j.contains("initial_state")) {
        if (j["initial_state"].contains("components")) {
            for (auto& c : j["initial_state"]["components"]) {
                bool is_active = true;
                if (c.contains("active")) {
                    if (c["active"].is_boolean()) {
                        is_active = c["active"].get<bool>();
                    } else {
                        is_active = c["active"].get<int>() != 0;
                    }
                }
                uint32_t queue_size = 0;
                if (c.contains("queue")) {
                    if (c["queue"].is_number()) {
                        queue_size = c["queue"].get<uint32_t>();
                    } else if (c["queue"].is_array()) {
                        queue_size = static_cast<uint32_t>(c["queue"].size());
                    }
                }
                ir.components_context.push_back({
                    c.value("id", 0u),
                    is_active,
                    queue_size
                });
            }
        }

        if (j["initial_state"].contains("links")) {
            for (auto& l : j["initial_state"]["links"]) {
                ir.links_context.push_back({
                    l.value("id", 0u),
                    l.value("current_bandwidth_mbps", 0.0),
                    l.value("from", 0u),
                    l.value("to", 0u)
                });
            }
        }
    }

    /* ---------- Request Types ---------- */
    if (j.contains("request_types")) {
        for (auto& r : j["request_types"]) {
            ir.request_types.push_back({
                r["id"],
                r["payload"],
                r["routes"]
            });
        }
    }

    /* ---------- Initial Events ---------- */
    if (j.contains("initial_events")) {
        for (auto& e : j["initial_events"]) {
            EventIRType event_type = EventIRType::UNKNOWN;
            std::string type_str = e.value("type", "");
            if (type_str == "ArrivalEvent" || type_str == "REQUEST_ARRIVAL") {
                event_type = EventIRType::REQUEST_ARRIVAL;
            } else if (type_str == "TIMER" || type_str == "TimerEvent") {
                event_type = EventIRType::TIMER;
            }

            ir.initial_events.push_back({
                e["id"],
                e["time"],
                event_type,
                e["payload"]
            });
        }
    }

    return ir;
}
