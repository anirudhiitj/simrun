#include <iostream>
#include <chrono>
#include <nlohmann/json.hpp>

#include "src/ir_parser.h"
#include "src/event_initializer.h"
#include "src/simulation_context.h"
#include "factory/factory.h"
#include "core/scheduler.h"
#include "core/simulator.h"
#include "core/event_queue.h"

int main(int argc, char** argv) {

    if (argc < 2) {
        nlohmann::json err;
        err["status"] = "error";
        err["phase"]  = "init";
        err["message"] = "Usage: simrun <ir_file.json>";
        std::cout << err.dump() << std::endl;
        return 1;
    }

    auto wall_start = std::chrono::steady_clock::now();

    try {
        /* ---------- Parse IR ---------- */
        IR ir = IRParser::parseFromFile(argv[1]);

        /* ---------- Global Context ---------- */
        SimulationContext ctx(
            ir.header.seed,
            ir.header.time_unit
        );

        /* ---------- Factory ---------- */
        EntityFactory entityFactory(ctx);

        /* ---------- Build Static World ---------- */
        entityFactory.createComponents(ir.components);
        entityFactory.createLinks(ir.links);

        entityFactory.applyComponentContext(ir.components_context);
        entityFactory.applyLinkContext(ir.links_context);

        entityFactory.registerRequestTypes(ir.request_types);

        /* ---------- Event Queue & Scheduler ---------- */
        auto queue = createPriorityEventQueue();
        EventScheduler scheduler(*queue);

        /* ---------- Seed Initial Events ---------- */
        EventInitializer::seedInitialEvents(
            ir,
            scheduler,
            entityFactory
        );

        /* ---------- Run Simulation ---------- */
        Simulator sim(*queue, ctx);
        sim.run();

        auto wall_end = std::chrono::steady_clock::now();
        auto wall_ms  = std::chrono::duration_cast<std::chrono::milliseconds>(
                            wall_end - wall_start).count();

        /* ---------- Build Result JSON ---------- */
        nlohmann::json result;
        result["status"]   = "completed";
        result["phase"]    = "simulation";
        result["wall_time_ms"] = wall_ms;
        result["sim_time_final"] = sim.now();
        result["seed"] = ir.header.seed;

        result["world"]["components_count"] = ctx.components.size();
        result["world"]["links_count"]      = ctx.links.size();
        result["world"]["request_types_count"] = ctx.request_types.size();
        result["world"]["initial_events_count"] = ir.initial_events.size();

        /* stdout is the IPC channel — only JSON goes here */
        std::cout << result.dump() << std::endl;

    } catch (const std::exception& e) {
        auto wall_end = std::chrono::steady_clock::now();
        auto wall_ms  = std::chrono::duration_cast<std::chrono::milliseconds>(
                            wall_end - wall_start).count();

        nlohmann::json err;
        err["status"]  = "error";
        err["phase"]   = "simulation";
        err["message"] = e.what();
        err["wall_time_ms"] = wall_ms;
        std::cout << err.dump() << std::endl;
        return 1;
    }

    return 0;
}