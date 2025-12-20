# Smart LED Blinker (STM32)

A bare-metal STM32F4 project demonstrating external interrupts, timing, and
simple state handling without using STM32CubeMX.

## Features
- Button-controlled LED using EXTI interrupt
- Software debounce using SysTick timing
- Short press: toggle LED
- Long press (2 seconds): change blink pattern
- Double click: enter/exit low-power (sleep) mode
- NVIC interrupt priority configuration

## Target Hardware
- STM32F4 series microcontroller
- User button connected to EXTI line
- On-board or external LED

## Project Structure
```text
Core/
├── Inc/
│   ├── app/        # Application layer headers
│   ├── drivers/    # Peripheral driver headers
│   └── system/     # CMSIS and system headers
│
├── Src/
│   ├── app/        # Application source files
│   ├── drivers/    # Peripheral driver implementations
│   └── system/     # System and runtime code
│       └── startup/  # Startup assembly and linker-related files
│
config/              # Project configuration headers
docs/                # Documentation
```

## Build Environment
- STM32CubeIDE
- Bare-metal (no HAL, no CubeMX)
- ARM GCC toolchain

## How It Works
- EXTI interrupt handles button press events
- SysTick provides millisecond timing for debounce and press duration
- A simple state machine controls LED behavior
- Sleep mode is entered using WFI instruction

## License
MIT License