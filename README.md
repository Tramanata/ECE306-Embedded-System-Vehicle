# ECE306-Embedded-System-Vehicle

**Course:** ECE 306 – Introduction to Embedded Systems  
**Institution:** North Carolina State University  
**Language:** C (Assembly for low-level initialization)  
**IDE:** IAR Embedded Workbench  
**Microcontroller:** Texas Instruments MSP430 FRAM Experimenters Board

---

## Project Overview

This project involved the design and programming of an autonomous RC car over the course of a semester. Each project phase incrementally added hardware and software capabilities to the vehicle — from basic motor control all the way to autonomous line-following with wireless remote control.

The car was built on top of a custom control board mounted to the MSP430 FRAM board. A power display board housed voltage regulators and the LCD. A FET (MOSFET) board handled H-bridge motor control, allowing independent forward and reverse operation of both drive wheels.

---

## Hardware

| Component | Description |
|---|---|
| TI MSP430 FRAM Board | Main microcontroller platform |
| Control Board (PCB) | Custom PCB with power management, sensors, and motor driver interface |
| FET Board (H-Bridge) | MOSFET-based motor driver for independent wheel control |
| LCD Display | Character display for status output |
| IR Emitter/Detector | Line detection sensors |
| Wi-Fi Module (STMicro SPWF01SA) | IoT module for wireless communication |
| Battery Pack | Onboard power supply for independent operation |

---

## Project Phases

The course was structured as a sequence of nine incremental projects:

### Project 1 – Hardware Setup
Installed the control board onto the FRAM Experimenters Board and configured the battery-powered system with a power switch for independent operation.

### Project 2 – LCD Integration
Installed and initialized the LCD on the control board. Validated display functionality using provided hardware test code.

### Project 3 – Forward-Only Motor Control
Implemented the forward portion of H-bridge wheel control using GPIO outputs. Verified that the car could navigate predetermined shapes within a 36" × 36" area.

### Project 4 – Full H-Bridge Control
Extended motor control to include forward and reverse. Verified with timed forward and reverse travel sequences.

### Project 5 – Line Detection Sensors
Added the IR emitter/detector circuit for detecting a black electrical tape line on the floor.

### Project 6 – Line Navigation
Replaced timed movement with closed-loop control. The car could detect and navigate around a circular black line on the ground.

### Project 7 – Serial Communication
Implemented serial input/output routines. Developed software for UART-based communication with external devices.

### Project 8 – Wi-Fi Module Integration
Integrated the STMicro SPWF01SA Wi-Fi module to enable bidirectional wireless communication. The car could receive commands from a web interface and respond accordingly.

### Project 9 – Autonomous Line Following
Final integration. The car used sensor feedback and wireless connectivity to follow a black tape line after navigating an obstacle course via Wi-Fi.

---

## Software Features

- **ADC Conversions** – Analog-to-digital conversion for sensor readings
- **DAC Output** – Digital-to-analog conversion for signal generation
- **PWM Motor Control** – Pulse-width modulation for variable wheel speed
- **Timers & Interrupts** – Hardware timers for precise control loops
- **LCD Driver** – Custom routines for character display
- **UART Serial I/O** – Bidirectional serial communication
- **Wi-Fi Command Parsing** – Wireless command interpretation over IoT module
- **Autonomous Navigation** – State-machine-based line following logic

---

## Development Notes

All code was written in C using IAR Embedded Workbench as the IDE. Assembly was used where necessary for low-level hardware initialization. The codebase was developed and tested incrementally across the semester, with each project building directly on the previous one.

---

## Getting Started

### Requirements
- IAR Embedded Workbench for MSP430
- TI MSP430 FRAM Experimenters Board + control board hardware
- STMicro SPWF01SA Wi-Fi module (for Projects 8–9)

### Setup
1. Clone this repository
2. Open the project in IAR Embedded Workbench
3. Connect the MSP430 board via USB
4. Build and flash the firmware to the board

---

## License

This project was developed for academic purposes at NC State University. Code is shared for educational reference.
