#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

#include "../src/simulation_context.h"
#include "../src/ir_types.h"
#include "../events/event_types.h"

// Entities
#include "../entities/service.h"
#include "../entities/database.h"
#include "../entities/cache.h"
#include "../entities/network_link.h"

// Events
#include "../events/database_events.h"

class Event;
struct Request;

class EntityFactory {
public:
    explicit EntityFactory(SimulationContext& ctx);

    void createComponents(const std::vector<IRComponent>& components);
    void createLinks(const std::vector<IRLink>& links);

    void applyComponentContext(
        const std::vector<IRComponentContext>& context
    );

    void applyLinkContext(
        const std::vector<IRLinkContext>& context
    );

    void registerRequestTypes(
        const std::vector<IRRequestType>& request_types
    );

    /* -------- Event creation -------- */
    std::unique_ptr<Event> createEvent(
        EventType type,
        double timestamp,
        uint64_t seed,
        Request* req
    );

private:
    SimulationContext& ctx;

    /* Component helpers */
    void createService(const IRComponent& c);
    void createDatabase(const IRComponent& c);
    void createCache(const IRComponent& c);

    /* Event helpers */
    std::unique_ptr<Event> createDBRequestArrival(
        double ts, uint64_t seed, Request* req
    );

    std::unique_ptr<Event> createDBRequestSend(
        double ts, uint64_t seed, Request* req
    );
};
