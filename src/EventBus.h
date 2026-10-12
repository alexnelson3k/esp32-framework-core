#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <Arduino.h>
#include <vector>
#include <queue>
#include "SystemEvent.h"
#include "EventSubscriber.h"

class EventBus {
public:
    EventBus();

    void subscribe(EventSubscriber* subscriber);
    void unsubscribe(EventSubscriber* subscriber);

    void publish(const SystemEvent& event);

    void process();

    size_t getQueueSize() const;

private:
    std::vector<EventSubscriber*> subscribers;
    std::queue<SystemEvent> eventQueue;

    void dispatch(const SystemEvent& event);
};

#endif