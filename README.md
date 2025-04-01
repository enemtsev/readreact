# ReadReact - Zephyr RTOS GPIO Management with ZBus

[![CI](https://github.com/enemtsev/readreact/actions/workflows/zephyr-ci.yml/badge.svg)](https://github.com/enemtsev/readreact/actions/workflows/zephyr-ci.yml)

A Zephyr RTOS application demonstrating GPIO input/output handling with ZBus message passing between components. The project implements a decoupled, event-driven architecture that follows best practices for embedded software development.

## Overview

ReadReact is a decoupled, event-driven system that leverages Zephyr's ZBus messaging framework to create a maintainable and testable architecture for embedded GPIO applications. It implements a clean separation between input detection (ReadClass) and output actuation (ReactClass).

## Architecture

The system follows a publisher-subscriber pattern with the following key components:

```
┌─────────────┐    ZBus Message    ┌─────────────┐
│  ReadClass  │───────────────────▶│ ReactClass  │
└─────────────┘    (GPIO State)    └─────────────┘
      │                                   │
      ▼                                   ▼
┌─────────────┐                   ┌─────────────┐
│  GPIO Input │                   │ GPIO Output │
│  (Button)   │                   │    (LED)    │
└─────────────┘                   └─────────────┘
```

### Component Responsibilities

#### ReadClass
- Initializes and configures GPIO input pin with pull-up
- Sets up interrupt handling for state changes
- Implements debouncing (50ms) to filter out noise and bounce
- Publishes reliable state changes to ZBus channel
- Uses k_work_delayable to process changes outside ISR context

#### ReactClass
- Subscribes to GPIO state changes through ZBus
- Controls LED output based on received state:
  - HIGH state: Blinks LED 3 times with 100ms intervals
  - LOW state: Turns LED on for 500ms, then off
- Uses k_work_delayable for timing-based output patterns
- Avoids blocking the observer callback

#### ZBusManager
- Centralizes ZBus channel management
- Handles publisher and subscriber registration
- Routes messages between components
- Abstracts Zephyr ZBus API for cleaner component code

#### ReactLED
- Provides hardware abstraction for LED control
- Simplifies testing through mockable interface
- Encapsulates GPIO output configuration

## Features

- **Event-Driven Architecture**: Clean separation of concerns through ZBus messaging
- **Hardware Abstraction**: Abstract interfaces allow for easy mocking and testing
- **Interrupt-Based Input**: Efficient GPIO monitoring with interrupt callbacks
- **Debounce Logic**: Reliable input with software debouncing
- **Non-Blocking Design**: Work queues prevent blocking in interrupt handlers
- **Comprehensive Documentation**: Doxygen-style documentation throughout codebase
- **Extensive Testing**: Unit tests using GTest and GMock frameworks
- **CI/CD Integration**: Automated testing with GitHub Actions

## Prerequisites

- Zephyr SDK (v3.4+ recommended)
- West tool (Zephyr's meta-tool)
- Python 3.8+
- C++20 capable compiler
- Development board or `native_sim` for host simulation

## Building and Running

### Setup Zephyr Environment

```bash
# Initialize Zephyr workspace
west init ~/zephyrproject
cd ~/zephyrproject
west update

# Install required Python packages
pip install -r zephyr/scripts/requirements.txt
```

### Build for Native Simulation

```bash
# Clone the repository
git clone https://github.com/enemtsev/readreact.git
cd readreact

# Build for native_sim (host machine)
west build -b native_sim_64

# Run the application
west build -t run
```

### Build for Hardware

```bash
# Build for your development board (replace 'your_board' with your board name)
west build -b your_board

# Flash to device
west flash
```

## Hardware Configuration

For physical hardware, create or update a device tree overlay for your board in `boards/<board_name>.overlay`:

```dts
/ {
    resources {
        compatible = "test-gpio-basic-api";
        out-gpios = <&gpio0 0 0>; /* LED pin */
        in-gpios = <&gpio0 1 0>;  /* Button pin */
    };
};

&gpio0 {
    ngpios = <2>;
};
```

Then update the `readreact/gpio_device.h` file to use the real hardware definitions instead of emulation.

## Testing

The project includes a comprehensive test suite using GTest and GMock frameworks:

```bash
# Build with testing enabled
west build -b native_sim_64 -- -DTEST_BUILD=ON

# Run tests
west build -t run
```

Test coverage includes:
- ZBusManager message routing
- ReactClass LED pattern generation
- ReadClass debounce and state detection
- Mock hardware interfaces

## Project Structure

```
readreact/
├── CMakeLists.txt           # Main build configuration
├── include/
│   └── readreact/           # Public headers
│       ├── gpio_device.h    # GPIO configuration defines
│       ├── readclass.h      # Input handling class
│       ├── reactclass.h     # Output control class
│       ├── reactled.h       # LED interface
│       └── zbusmanager.h    # ZBus management
├── src/
│   ├── CMakeLists.txt       # Source build configuration
│   ├── main.cpp             # Application entry point
│   ├── readclass.cpp        # Input handling implementation
│   ├── reactclass.cpp       # Output control implementation
│   ├── reactled.cpp         # LED implementation
│   └── zbusmanager.cpp      # ZBus implementation
├── tests/
│   ├── CMakeLists.txt       # Test build configuration
│   ├── mocks/               # Mock implementations
│   │   └── reactled_mock.cpp# LED mock for testing
│   ├── test_main.cpp        # Test entry point
│   ├── test_readclass.cpp   # ReadClass tests
│   ├── test_reactclass.cpp  # ReactClass tests
│   └── test_zbusmanager.cpp # ZBus manager tests
└── boards/
    └── native_sim_64.overlay# Emulation configuration
```

## License

MIT License - see LICENSE for details