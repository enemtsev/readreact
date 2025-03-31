#pragma once

#include "readreact/zbusmanager.h"

// Mock Subscriber class to test message handling
class MockSubscriber : public BaseSubscriber {
public:
    MOCK_METHOD(void, handle_message, (const ZBusMessage &message), (override));
};
