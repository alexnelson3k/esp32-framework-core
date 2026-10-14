#include "EventBus.h"
#include "FrameworkLog.h"

EventBus::EventBus() {}

void EventBus::subscribe(EventSubscriber* subscriber) {
    if (!subscriber) return;

    subscribers.push_back(subscriber);
    FW_LOG_DEBUG("Events", "Subscriber registered; count %u", static_cast<unsigned>(subscribers.size()));
}

void EventBus::unsubscribe(EventSubscriber* subscriber) {
    for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
        if (*it == subscriber) {
            subscribers.erase(it);
            FW_LOG_DEBUG("Events", "Subscriber removed; count %u", static_cast<unsigned>(subscribers.size()));
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
