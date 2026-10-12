#include "EventBus.h"

EventBus::EventBus() {}

void EventBus::subscribe(EventSubscriber* subscriber) {
    if (!subscriber) return;

    subscribers.push_back(subscriber);
}

void EventBus::unsubscribe(EventSubscriber* subscriber) {
    for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
        if (*it == subscriber) {
            subscribers.erase(it);
            break;
        }
    }
}

void EventBus::publish(const SystemEvent& event) {
    eventQueue.push(event);
}

void EventBus::process() {
    while (!eventQueue.empty()) {
        SystemEvent event = eventQueue.front();
        eventQueue.pop();

        dispatch(event);
    }
}

void EventBus::dispatch(const SystemEvent& event) {
    for (EventSubscriber* subscriber : subscribers) {
        subscriber->onEvent(event);
    }
}

size_t EventBus::getQueueSize() const {
    return eventQueue.size();
}