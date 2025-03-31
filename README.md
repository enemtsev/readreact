# ReadReact - Zephyr RTOS GPIO Management with ZBus

[![CI](https://github.com/enemtsev/readreact/actions/workflows/zephyr-ci.yml/badge.svg)](https://github.com/enemtsev/readreact/actions/workflows/zephyr-ci.yml)

A Zephyr RTOS application demonstrating GPIO input/output handling with ZBus message passing between components.

## Overview

ReadReact is a decoupled, event-driven system that leverages Zephyr's ZBus messaging framework to create a maintainable and testable architecture for embedded GPIO applications. It implements a clean separation between input detection (ReadClass) and output actuation (ReactClass).

## Features

- **ReadClass**: Monitors GPIO input with debouncing and publishes state changes via ZBus
- **ReactClass**: Listens for state changes and controls GPIO output (LED) accordingly
- **ZBusManager**: Handles messaging between components using Zephyr's ZBus framework
- **Hardware Abstraction**: Decoupled architecture through mock interfaces for easier testing
- **Comprehensive Tests**: Unit tests using GTest and GMock
- **CI Integration**: GitHub Actions workflow for automated builds and tests

## Prerequisites

- Zephyr SDK (v3.4+ recommended)
- West tool
- Python 3.8+
- C++20 capable compiler
- Development board or `native_sim` for host simulation

## Hardware Configuration

For real hardware, update the device tree overlay (`boards/<your_board>.overlay`):

```dts
/ {
    resources {
        compatible = "test-gpio-basic-api";
        out-gpios = <&gpio0 0 0>; /* Pin 0 */
        in-gpios = <&gpio0 1 0>; /* Pin 1 */
    };
};

&gpio0 {
    ngpios = <2>;
};
```

## Getting Started

1. Set up the Zephyr environment:

```bash
west init ~/zephyrproject
cd ~/zephyrproject
west update
pip install -r zephyr/scripts/requirements.txt
```

2. Clone this repository:

```bash
git clone https://github.com/enemtsev/readreact.git
cd readreact
```

3. Build for your target board (or native_sim):

```bash
west build -b native_sim
```

4. Run the application:

```bash
west build -t run
# OR for a physical board
west flash
```

## Project Structure

```
readreact/
├── CMakeLists.txt           # Build configuration
├── prj.conf                 # Zephyr configuration
├── boards/                  # Board-specific configurations
│   └── native_sim.overlay   # Device tree overlay
├── src/
│   ├── main.cpp             # Main application
│   ├── readclass.cpp        # GPIO input handling
│   ├── reactclass.cpp       # GPIO output control
│   ├── reactled.cpp         # LED abstraction
│   └── zbusmanager.cpp      # ZBus messaging implementation
├── include/
│   └── readreact/           # Header files
│       ├── gpio_observer.hpp # ZBus interface definitions
│       ├── readclass.h       # Input handling class
│       ├── reactclass.h      # Output control class
│       ├── reactled.h        # LED interface
│       └── zbusmanager.h     # ZBus management
├── tests/
│   ├── CMakeLists.txt       # Test build configuration
│   ├── mocks/               # Hardware mocks
│   │   ├── reactled_mock.h  # LED control mock
│   │   └── zbus_mocks.h     # ZBus mocks
│   ├── test_main.cpp        # Test entry point
│   ├── test_readclass.cpp   # ReadClass tests
│   ├── test_reactclass.cpp  # ReactClass tests
│   └── test_zbusmanager.cpp # ZBus manager tests
└── .github/workflows/
    └── zephyr-ci.yml        # CI pipeline
```

## Class Architecture

### ZBusManager

Central messaging system that:
- Manages publisher/subscriber registration
- Handles message routing between components
- Interfaces with Zephyr's ZBus framework

### ReadClass

Input monitoring component that:
- Configures GPIO pins as inputs with pull-up
- Implements debounce logic (50ms default)
- Publishes state changes via ZBusManager

### ReactClass

Output control component that:
- Subscribes to state change messages
- Controls LED behavior based on state
- Implements timing logic for different patterns

### ReactLED

Hardware abstraction layer that:
- Provides a clean interface for LED control
- Enables mocking for testing

## Testing

To run the unit tests:

```bash
west build -b native_sim -- -DBUILD_TESTING=ON
west build -t run
```

The test suite includes:
- ZBusManager tests for message routing
- ReactClass tests for LED pattern generation
- ReadClass tests for debounce and state detection

## Behavior

### Input (ReadClass)
- Monitors GPIO pin configured as input with pull-up
- Implements 50ms debounce logic
- Publishes state changes via ZBus
  - `true` when pin goes HIGH
  - `false` when pin goes LOW

### Output (ReactClass)
- Listens for state changes on ZBus
- For HIGH state:
  - Blinks connected LED 3 times (100ms interval)
- For LOW state:
  - Turns LED on for 500ms

## License

MIT License - see LICENSE for details.