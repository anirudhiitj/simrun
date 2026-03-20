//event_queue.cpp defines different priority/calender/ladder (for now just pq) queues
//at runtime EventQueue reference may point to any of them (depending on configuration)
#include "event_queue.h"
#include "../events/events.h"
#include <queue>
#include <vector>
#include <memory>

struct EventCompare {
    bool operator()(
        const std::unique_ptr<Event>& a,
        const std::unique_ptr<Event>& b
    ) const {
        return a->timestamp > b->timestamp;
    }
};

class PriorityEventQueue : public EventQueue {
private:
    std::priority_queue<
        std::unique_ptr<Event>,
        std::vector<std::unique_ptr<Event>>,
        EventCompare
    > pq;

public:
    void push(std::unique_ptr<Event> e) override {
        pq.push(std::move(e));
    }

    std::unique_ptr<Event> pop() override {
        auto e = std::move(const_cast<std::unique_ptr<Event>&>(pq.top()));
        pq.pop();
        return e;
    }

    bool empty() const override {
        return pq.empty();
    }
};

std::unique_ptr<EventQueue> createPriorityEventQueue() {
    return std::make_unique<PriorityEventQueue>();
}

