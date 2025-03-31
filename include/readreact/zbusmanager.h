#pragma once

#include <vector>
#include <functional>
#include <zephyr/zbus/zbus.h>

// Data structure for ZBus message
struct ZBusMessage {
    bool state; // True for HIGH, False for LOW

    // Equality operator for ZBusMessage
    bool operator==(const ZBusMessage& other) const {
        return state == other.state;
    }

    // Inequality operator for ZBusMessage
    bool operator!=(const ZBusMessage& other) const {
        return !(*this == other);
    }
};

// Base class for publishers
class BasePublisher {
protected:
    std::function<void(const ZBusMessage&)> publish_callback;
    
public:
    virtual ~BasePublisher() = default;

    void set_publish_callback(std::function<void(const ZBusMessage&)> callback) {
        publish_callback = callback;
    }

    void publish(const ZBusMessage& message) {
        if (publish_callback) {
            publish_callback(message);
        }
    }
};

// Base class for subscribers
class BaseSubscriber {
public:
    virtual ~BaseSubscriber() = default;
    virtual void handle_message(const ZBusMessage& message) = 0;
};

// ZBusManager to handle channel registration and message dispatch
class ZBusManager {
public:
     ZBusManager() = default;
    ~ZBusManager();

    void register_publisher(BasePublisher* publisher);
    void register_subscriber(BaseSubscriber* subscriber);
    void publish(const ZBusMessage& message);

    void set_channel(const struct zbus_channel *chan);
    
    static void channel_callback(const struct zbus_channel *chan);

private:
    const struct zbus_channel *channel_{nullptr};
    std::vector<BaseSubscriber*> subscribers_;
    std::vector<BasePublisher*> publishers_;
};