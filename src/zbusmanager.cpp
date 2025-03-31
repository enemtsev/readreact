#include "readreact/zbusmanager.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(zbus_manager, LOG_LEVEL_INF);

ZBUS_LISTENER_DEFINE(
    zbusmanager_observer,
    ZBusManager::channel_callback
);

void ZBusManager::channel_callback(const struct zbus_channel *chan) {
    auto *manager = *static_cast<ZBusManager**>(chan->user_data);
        if (!manager) return;

    const ZBusMessage *message = static_cast<const ZBusMessage*>(zbus_chan_const_msg(chan));
    for (auto *subscriber : manager->subscribers_) {
        subscriber->handle_message(*message);
    }
}

ZBusManager::~ZBusManager() {
    zbus_chan_rm_obs(channel_, &zbusmanager_observer, K_NO_WAIT);
}

void ZBusManager::register_publisher(BasePublisher* publisher) {
    publishers_.push_back(publisher);
    publisher->set_publish_callback([this](const ZBusMessage& message) {
        this->publish(message);
    });
}

void ZBusManager::register_subscriber(BaseSubscriber* subscriber) {
    subscribers_.push_back(subscriber);
}

void ZBusManager::publish(const ZBusMessage& message) {
    zbus_chan_pub(channel_, &message, K_NO_WAIT);
}

void ZBusManager::set_channel(const struct zbus_channel *chan) {
    channel_ = chan;
    zbus_chan_add_obs(channel_, &zbusmanager_observer, K_NO_WAIT);
}