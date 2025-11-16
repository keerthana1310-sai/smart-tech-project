# Plant-Me-Gently — README

## Project overview
Plant-Me-Gently is a small IoT project using an ESP32 that detects soil moisture and plays a pleasant sound when the plant needs watering.

## Hardware
- ESP32 dev board
- Soil moisture sensor (capacitive recommended)
- Passive buzzer
- Jumper wires
- Breadboard
- TFT/OLED display
- Transistor + 1k resistor for amplified buzzer

## Wiring

Use 3.3V for sensor VCC when connecting to ESP32 ADC. Keep all grounds common.

Simple wiring (single buzzer + capacitive sensor):

Soil sensor VCC → 3.3V

Soil sensor GND → GND

Soil sensor AOUT → GPIO34 (ADC1_6) or any ADC pin

Buzzer + → GPIO25 (or any GPIO capable output)

Buzzer − → GND

ESP32 GND → common GND (soil sensor, buzzer, any other modules)

If using DHT11 (air sensor):

DHT VCC → 3.3V

DHT GND → GND

DHT DATA → GPIO4 (with 10 kΩ pull-up to VCC)

If using transistor to drive buzzers from 5V (recommended for louder sound):

GPIO → 1 kΩ resistor → transistor base (2N2222)

Transistor emitter → GND

Transistor collector → buzzer negative

Buzzer positive → 5V

Add a small flyback diode if using inductive buzzer (not necessary for piezo).

Software / Code

The project uses Arduino-style code for the ESP32.

Main features in the provided sketch

Reads soil moisture via analogRead() from the chosen ADC pin.

Compares raw reading to a calibrated dry threshold.

Plays a pleasant melody on a passive buzzer when the reading indicates “dry.”

Rate limits alerts with a cooldown interval so it doesn’t keep chirping all the time.

Serial debug prints for calibration.

Key variables to tune

soilPin — ADC pin (example: 34)

buzzerPin — output pin for buzzer (example: 25)

dryThreshold — raw ADC threshold for “dry” (calibrate per sensor/soil)

alertInterval — milliseconds between alerts (default 30 minutes in sample)

How to upload

Install ESP32 board support in Arduino IDE (Boards Manager URL: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json).

Select ESP32 Dev Module board and the correct /dev/ttyUSB* port.

Paste the sketch into Arduino IDE, edit pins/thresholds as needed, upload.

Calibration

Upload the debugging sketch that prints analogRead(soilPin) continuously.

With the sensor in air (dry) note raw_dry.

With the sensor in well-watered soil note raw_wet.

Choose dryThreshold somewhere between raw_wet and raw_dry. If you want fewer false alerts, bias toward the dry side. Example:

dryThreshold = (raw_dry + raw_wet) / 2;


Re-test with the sensor placed in the pot and water to verify alerts after the soil has actually dried.

Tips:

For resistive probes, avoid powering them continuously; power briefly while reading to reduce corrosion.

Capacitive sensors are more stable for long-term deployment.

Troubleshooting

No serial output

Confirm correct port selected and ESP32 is running.

Use Serial.begin(115200); and open Serial Monitor at 115200 baud.

analogRead() values unstable

Average several samples (take N readings and compute average).

Ensure sensor VCC is stable 3.3V.

Add a small capacitor (e.g., 0.1 μF) between sensor VCC and GND if noisy.

Buzzer not sounding or too quiet

Verify buzzer type is passive (needs PWM/frequency). Active buzzers only beep on power.

Use transistor + 5V supply for louder sound (common ground required).

Check wiring: buzzer − must go to GND.

ESP32 fails to upload

Ensure board drivers/modules (cp210x/ch341) are present in dmesg.

Add user to dialout group or set appropriate udev rules.

Try manual bootloader mode: hold BOOT, press EN, release EN, release BOOT while uploading.

Example improvements / extensions

Add DHT22 for ambient temperature/humidity and make the alert behavior adaptive (e.g., suppress alerts when humidity is high).

Add OLED/TFT UI showing real-time moisture %, thresholds, last watered time, battery level.

Add OTA updates (ArduinoOTA) so future firmware updates can be wireless.

Add a relay + small pump for automated watering with safety checks.

Log sensor data to SD card or send to Firebase/ThingSpeak / MQTT for remote monitoring.

Use low-power deep sleep and wake on a timer to run on battery longer.

Power considerations

USB provides stable power for prototyping.

For long-term battery use, use Li-ion (18650) + TP4056 charger + DC-DC boost if 5V is needed.

If using multiple buzzers or a speaker, do not drive them all from the ESP32’s 3.3V regulator — use transistor drivers and separate 5V supply with common ground.

Files included

plant_me_gently.ino — main sketch (soil read, buzzer melody, threshold logic)

calibrate_readings.ino — small sketch for printing raw ADC values for calibration (optional)

wiring_diagram.png — simple wiring diagram (optional, draw as needed)

License & credits

You’re free to use and modify this project for personal, educational, or club use. If you publish or present it, please credit the creator and mention any libraries used (Arduino, ESP32 core, optional display/touch libraries). No warranty — use at your own risk.

Final notes

This project is intentionally simple so you can iterate quickly: prototype with the resistive probe if you must, then upgrade to a capacitive sensor for reliability. Once the basic alert + buzzer works, you’ll have a great foundation to add UI, remote alerts, or automatic watering.
