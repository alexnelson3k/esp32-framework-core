#ifndef EVENT_SUBSCRIBER_H
#define EVENT_SUBSCRIBER_H

#include "SystemEvent.h"

class EventSubscriber {
public:
    virtual ~EventSubscriber() = default;

    virtual void onEvent(const SystemEvent& event) = 0;
};

#endif