<!-- Banner -->
<p align="center">
  <img src="https://github.com/user-attachments/assets/5970bd8c-3437-49b5-960c-e0d5693929f2" alt="DGUS-Async Banner" width="100%">
</p>

<h1 align="center">DGUS-Async</h1>

<p align="center">
  Async Arduino library for DWIN / DGUS HMI displays on ESP32
</p>

<p align="center">
  <img src="https://img.shields.io/badge/platform-ESP32-blue" alt="Platform: ESP32">
  <img src="https://img.shields.io/badge/license-MIT-green" alt="License: MIT">
  <img src="https://img.shields.io/badge/status-active-success" alt="Status: Active">
</p>

---

## Overview

**DGUS-Async** is a non-blocking Arduino library for **DWIN / DGUS serial HMI displays**, designed for **ESP32** projects that need responsive and clean display communication without relying on `delay()`-based workflows.

It provides an asynchronous, queue-based approach for sending commands, reading values, handling page changes, and building more scalable UI-driven embedded applications.

---

## Features

- Non-blocking communication architecture
- Designed for ESP32 hardware UART
- Queue-based command handling
- Read and write DGUS variables
- Page tracking support
- Animation helper support
- Clean API for responsive embedded UI workflows

---

## Why DGUS-Async?

Many DGUS examples found online are based on blocking logic and frequent use of `delay()`, which can quickly become difficult to maintain in real embedded projects.

**DGUS-Async** is built for modern ESP32 applications where the display should work smoothly alongside sensors, networking, automation logic, and other time-sensitive tasks.

---

## Supported Hardware

Currently tested and focused on:

- **ESP32**
- **DWIN / DGUS serial displays**

> This library is currently optimized for ESP32-based projects using hardware serial communication.

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
git clone https://github.com/YOUR_USERNAME/DGUS-Async-Arduino.git
