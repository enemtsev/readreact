# ReadReact - Zephyr RTOS GPIO Management with ZBus

[![CI](https://github.com/yourusername/readreact/actions/workflows/zephyr-ci.yml/badge.svg)](https://github.com/yourusername/readreact/actions/workflows/zephyr-ci.yml)

A Zephyr RTOS application demonstrating GPIO input/output handling with ZBus message passing between components.

## Features

- **ReadClass**: Monitors GPIO input with debouncing and publishes state changes via ZBus
- **ReactClass**: Listens for state changes and controls GPIO output (LED) accordingly
- **Hardware Abstraction**: Decoupled architecture for easier testing
- **Comprehensive Tests**: Unit tests using GTest and GMock
- **CI Integration**: GitHub Actions workflow for automated builds and tests

## Prerequisites

- Zephyr SDK (v3.4+ recommended)
- West tool
- Python 3.8+
- Development board or `native_posix_64` for host simulation

## Hardware Configuration

For real hardware, update the device tree overlay (`boards/<your_board>.overlay`):

/ {
aliases {
input-gpio = &gpio0;
output-gpio = &gpio1;
};
};

## Getting Started

1. Set up the Zephyr environment:

west init ~/zephyrproject
cd ~/zephyrproject
west update
pip install -r zephyr/scripts/requirements.txt


2. Clone this repository:

git clone https://github.com/yourusername/readreact.git
cd readreact

3. Build for native_posix_64:

west build -b native_posix_64

4. Run the application:

west build -t run

## Project Structure

readreact/
├── CMakeLists.txt # Build configuration
├── prj.conf # Zephyr configuration
├── boards/
│ └── native_posix_64.overlay # Device tree overlay
├── src/
│ └── main.cpp # Main application
├── include/
│ └── readreact/
│ └── gpio_observer.hpp # ZBus interface
├── tests/
│ ├── mocks/ # Hardware mocks
│ ├── test_readclass.cpp # ReadClass tests
│ └── test_reactclass.cpp # ReactClass tests
└── .github/workflows/
└── zephyr-ci.yml # CI pipeline

## Testing

To run the unit tests:

west build -b native_posix_64 -- -DBUILD_TESTING=ON
west build -t run

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