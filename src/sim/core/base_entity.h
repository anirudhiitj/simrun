#pragma once
#include <cstdint>

class BaseEntity {
public:
    explicit BaseEntity(uint32_t id_) : entity_id(id_) {}
    virtual ~BaseEntity() = default;

    uint32_t id() const { return entity_id; }

private:
    uint32_t entity_id;
};
