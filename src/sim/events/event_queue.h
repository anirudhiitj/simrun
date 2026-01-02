#pragma once
#include "event.h"

class EventQueue {
public:
    virtual ~EventQueue() = default;

    virtual void push(Event* e) = 0;
    virtual Event* pop() = 0;
    virtual bool empty() const = 0;
};
