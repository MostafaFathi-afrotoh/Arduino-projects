# 🛠️ SystemUnite4: Hardware Specifications & Circuit Architecture

This document details the hardware components, electronic pinouts, sensor interfaces, relay load capacity, and power management architecture implemented in the **SystemUnite4** microbial incubation master controller.

---

## 📑 Table of Contents
1. [Microcontroller Specs](#1-microcontroller-specs)
2. [Pinout & Signal Mapping Table](#2-pinout--signal-mapping-table)
3. [Sensors & Signal Conditioning](#3-sensors--signal-conditioning)
4. [Actuators & Power Relays](#4-actuators--power-relays)
5. [User Interface & Wireless Modules](#5-user-interface--wireless-modules)
6. [Power Supply & Safety Isolation](#6-power-supply--safety-isolation)

---

## 1. Microcontroller Specs

The primary processing unit selected for this system is the **Arduino Mega 2560 R3** (or compatible ESP32 / Industrial MCU extension board).

* **Architecture:** ATmega2560 (8-bit AVR)
* **Operating Voltage:** $5\text{V DC}$
* **Input Voltage (Recommended):** $7\text{V} - 12\text{V DC}$
* **Digital I/O Pins:** 54 (15 offer PWM output)
* **Hardware UART Ports:** 4 Serial Interfaces (utilized for USB telemetry and Bluetooth connectivity)
* **Flash Memory:** 256 KB (8 KB used by bootloader)

---

## 2. Pinout & Signal Mapping Table

Below is the complete hardware interconnect mapping for the system:

| Pin Number | Signal Name | Hardware Component | Electrical Type | Operational Logic |
| :---: | :--- | :--- | :--- | :--- |
| **Pin 9** | `ONE_WIRE_BUS` | Dual DS18B20 Temp Sensors | Digital Input/Output | OneWire Bus Protocol |
| **Pin 24** | `startLed` | System Active Indicator | Digital Output ($5\text{V}$) | **HIGH** = System Running (Blue LED) |
| **Pin 25** | `stopeLed` | Standby / Stop Indicator | Digital Output ($5\text{V}$) | **HIGH** = Emergency/Stop (Red LED) |
| **Pin 26** | `airGeneratorLed` | Aeration Active Indicator | Digital Output ($5\text{V}$) | **HIGH** = Air Pump Active |
| **Pin 27** | `heaterLed1` | Upper Heater Status | Digital Output ($5\text{V}$) | **HIGH** = Upper Zone Heating (Green LED) |
| **Pin 28** | `heaterLed2` | Bottom Heater Status | Digital Output ($5\text{V}$) | **HIGH** = Lower Zone Heating (Green LED) |
| **Pin 42** | `airGenerator1` | Relay Channel 1 (Air Pump 1) | Digital Output ($5\text{V}$) | Active HIGH / Low-level Trigger |
| **Pin 43** | `airGenerator2` | Relay Channel 2 (Air Pump 2) | Digital Output ($5\text{V}$) | Active HIGH / Low-level Trigger |
| **Pin 44** | `upperHeater` | Upper Heating Element Relay | Digital Output ($5\text{V}$) | Closed-Loop Active HIGH Control |
| **Pin 45** | `bottomHeater` | Lower Heating Element Relay | Digital Output ($5\text{V}$) | Closed-Loop Active HIGH Control |
| **SDA / SCL** | I2C Bus | $16\times 2$ LCD Screen (PCF8574) | Serial Communication | $100\text{ kHz}$ I2C Bus Address `0x27` |
| **TX1 / RX1** | `Serial1` | HC-05 / HC-06 Bluetooth | Serial Communication | $9600\text{ Baud}$ Wireless Telemetry |

---

## 3. Sensors & Signal Conditioning

### Dual DS18B20 Digital Temperature Probe Assembly
* **Transduction Principle:** Direct digital temperature-to-digital conversion via internal bandgap reference and ADC.
* **Accuracy:** $\pm 0.5^\circ\text{C}$ over the range of $-10^\circ\text{C}$ to $+85^\circ\text{C}$.
* **Data Bus Wiring:** Shared 1-Wire bus architecture connected to **Digital Pin 9**.
* **Pull-Up Resistor Requirements:** A $4.7\text{ k}\Omega$ pull-up resistor is bridged between $V_{CC}$ ($+5\text{V}$) and the Data Line to stabilize signal transitions and prevent floating state errors.

---

## 4. Actuators & Power Relays

### Multi-Channel Relay Array & Heating Systems
The system interfaces with AC/DC power loads via optocoupler-isolated relay drivers to protect the microcontroller from inductive switching spikes.

* **Upper Heating Element (`upperHeater` - Pin 44):** Controlled dynamically via upper-zone thermal feedback ($T_1$).
* **Bottom Heating Element (`bottomHeater` - Pin 45):** Controlled dynamically via lower-zone thermal feedback ($T_2$).
* **Dual Aeration Generators (`airGenerator1`/`airGenerator2` - Pins 42/43):** Direct solenoid valve / air diaphragm pump switching for oxygenation during cultivation cycles.
* **Isolation Protection:** Opto-isolated inputs (`PC817` or similar) maintain galvanically separated ground lines between high-voltage loads and low-voltage logic lines.

---

## 5. User Interface & Wireless Modules

### 1. I2C Liquid Crystal Display ($16\times 2$)
* **Interface:** I2C Serial Backpack (`PCF8574` chip).
* **Display Metrics:** Real-time dual temperature telemetry ($T_1$, $T_2$), target setpoint ($T_{desired}$), countdown timer (minutes/seconds), and initialization system sequence.

### 2. Bluetooth Wireless Telemetry (`Serial1`)
* **Module Type:** HC-05 / HC-06 Serial Pass-Through Module.
* **Baud Rate:** $9600\text{ bps}$, 8 data bits, 1 stop bit, no parity.
* **Logic Level Shifting:** Voltage divider ($1\text{ k}\Omega / 2\text{ k}\Omega$) applied between Arduino MCU $\text{TX1}$ ($5\text{V}$) and Bluetooth Module $\text{RX}$ ($3.3\text{V}$) for input level protection.

---

## 6. Power Supply & Safety Isolation

* **Logic Supply Voltage:** Regulated $+5\text{V DC}$ ($1\text{A}$) supply for Arduino Mega, LCD backlight, status LEDs, and sensors.
* **Relay Coil Supply:** External $+5\text{V} / +12\text{V DC}$ supply to avoid drawing excessive current from the Arduino internal voltage regulator (`AMS1117-5.0`).
* **Grounding (GND):** Unified ground plane across logic and sensor lines, kept isolated from AC mains ground to eliminate ground loops.
