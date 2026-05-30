<!-- Banner -->
<p align="center">
  <img src="https://github.com/user-attachments/assets/5970bd8c-3437-49b5-960c-e0d5693929f2" alt="DGUS-Async Banner" width="100%">
</p>

<h1 align="center">DGUS-Async</h1>

<p align="center">
  Async Arduino library for DWIN / DGUS HMI displays
</p>

<p align="center">
  <img src="https://img.shields.io/badge/platform-Arduino%20Compatible-blue" alt="Platform: Arduino Compatible">
  <img src="https://img.shields.io/badge/license-MIT-green" alt="License: MIT">
  <img src="https://img.shields.io/badge/status-active-success" alt="Status: Active">
</p>

---

## Overview

**DGUS-Async** is a non-blocking Arduino library for **DWIN / DGUS serial HMI displays**, designed for Arduino-compatible boards that need responsive and clean display communication without relying on `delay()`-based workflows.

It provides an asynchronous, queue-based approach for sending commands, reading values, handling page changes, and building more scalable UI-driven embedded applications across a wide range of boards supported by the Arduino ecosystem.

---

## Features

- Non-blocking communication architecture
- Designed for Arduino-compatible platforms
- Queue-based command handling
- Read and write DGUS variables
- Page tracking support
- Animation helper support
- Clean API for responsive embedded UI workflows

---

## Why DGUS-Async?

Many DGUS examples found online are based on blocking logic and frequent use of `delay()`, which can quickly become difficult to maintain in real embedded projects.

**DGUS-Async** is built for modern embedded applications where the display should work smoothly alongside sensors, communication stacks, control logic, and other time-sensitive tasks.

---

## Supported Hardware

This library is intended for **Arduino-compatible boards** that support serial communication, including:

- **ESP32**
- **Arduino boards**
- Other boards supported by the Arduino ecosystem

> Any board capable of running Arduino-based code and communicating with a DGUS display over serial can potentially use this library.

---

## Installation

### Manual Installation
1. Download this repository as a ZIP file
2. Open Arduino IDE
3. Go to:  
   `Sketch > Include Library > Add .ZIP Library...`
4. Select the downloaded ZIP file

### Git Clone
Clone the repository into your Arduino libraries folder:
```bash
git clone https://github.com/masihch/DGUS-Async-Arduino.git
```
---

## Documentation (Wiki)

Full documentation is available in the GitHub Wiki, including wiring guides, API reference, and step-by-step tutorials:

- **Wiki Home:**  
  https://github.com/masihch/DGUS-Async-Arduino/wiki

### Getting Started
- Installation & wiring:  
  https://github.com/masihch/DGUS-Async-Arduino/wiki/Getting-Started

### API Reference
- Core API overview + small code snippets for each function:  
  https://github.com/masihch/DGUS-Async-Arduino/wiki/API-Reference

### Examples Guide
- Explanation of each example and how to run it:  
  https://github.com/masihch/DGUS-Async-Arduino/wiki/Examples

> Tip: If you are new to DGUS / DWIN displays, start with **Getting Started**.

---

## Support / Contact

If you need help with this library or working with **DWIN / DGUS** displays:

- **Documentation (Wiki):**  
  https://github.com/masihch/DGUS-Async-Arduino/wiki

- **GitHub Issues (recommended for bugs & feature requests):**  
  https://github.com/masihch/DGUS-Async-Arduino/issues

- **WhatsApp (direct help):**  
  https://wa.me/+989925786469

