#pragma once
#include "ir_types.h"

class EventScheduler;
class EntityFactory;

class EventInitializer {
public:
    static void seedInitialEvents(
        const IR& ir,
        EventScheduler& scheduler,
        EntityFactory& factory
    );
};
