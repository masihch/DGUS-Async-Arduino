/*
  DGUS_Async - Example 01: Basic Write (ESP32)

  Video:
  url:

  Description:
  Sends a value to VP 0x1000 every 1 seconds using non-blocking timing.
  The value counts from 1 to 9 and then starts again.

  Wiring:
  HMI TX -> ESP32 RX2 (GPIO13)
  HMI RX -> ESP32 TX2 (GPIO12)
  GND    -> GND

  Notes:
  - This example is for ESP32 only.
  - UART2 is used for communication with the DGUS display.
  - DGUS_Async uses a non-blocking system, so avoid using delay().
  - Always call hmi.update() continuously inside loop().
*/

#include <Arduino.h>
#include <DGUS_Async.h>

static constexpr uint32_t HMI_BAUD = 115200;

// ESP32 UART2 pins
static constexpr int HMI_RX_PIN = 13;   // ESP32 RX, connect to HMI TX
static constexpr int HMI_TX_PIN = 12;   // ESP32 TX, connect to HMI RX

// DGUS VP address
static constexpr uint16_t VP_ICON = 0x1000;

// Send interval
static constexpr unsigned long INTERVAL_MS = 1000;

// Use UART2 on ESP32
HardwareSerial HMISerial(2);
DGUS_Async hmi(HMISerial);

unsigned long lastMs = 0;
uint16_t value = 1;

void setup() {
  // Start UART2 with selected pins and baud rate
  HMISerial.begin(HMI_BAUD, SERIAL_8N1, HMI_RX_PIN, HMI_TX_PIN);

  // Initialize DGUS communication
  hmi.begin(HMI_BAUD);
}

void loop() {
  // Keep async communication running
  hmi.update();

  const unsigned long now = millis();

  // Send a new value every 1 seconds
  if (now - lastMs >= INTERVAL_MS) {
    lastMs = now;

    hmi.write(VP_ICON, value);

    value++;
    if (value > 9) {
      value = 1;
    }
  }
}
