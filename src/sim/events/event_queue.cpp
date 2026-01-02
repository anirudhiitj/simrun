#include "event_queue.h"
#include <queue>
#include <vector>

namespace {
// TO DO : TIE BREAKER ALGORITHM IN EventCompare
struct EventCompare {
    bool operator()(
        const std::unique_ptr<Event>& a,
        const std::unique_ptr<Event>& b
    ) const {
        return a->time > b->time;
    }
};


class PriorityEventQueue : public EventQueue {
private:
    std::priority_queue<std::unique_ptr<Event>,std::vector<std::unique_ptr<Event>>,EventCompare> pq;

public:
    void push(Event* e) override {
        pq.push(e);
    }

    Event* pop() override {
        Event* e = pq.top();
        pq.pop();
        return e;
    }

    bool empty() const override {
        return pq.empty();
    }
};

} 
