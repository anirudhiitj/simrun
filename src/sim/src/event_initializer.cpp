#include "event_initializer.h"
#include "../core/scheduler.h"
#include "../factory/factory.h"
#include "../entities/request.h"

void EventInitializer::seedInitialEvents(
    const IR& ir,
    EventScheduler& scheduler,
    EntityFactory& factory
) {
    for (const auto& e : ir.initial_events) {
        // TODO: build proper Request objects from IR event payload
        // and create appropriate events via factory
        (void)e;
        (void)scheduler;
        (void)factory;
    }
}
