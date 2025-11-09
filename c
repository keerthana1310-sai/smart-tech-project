// ESP32 Soil-moisture -> play melody on passive buzzer
// Wire: Soil sensor OUT -> GPIO34 (ADC1), Buzzer + -> GPIO25, GND common

const int soilPin = 34;     // ADC1 channel, input only pin
const int buzzerPin = 25;   // PWM-capable pin

// LEDC (ESP32 PWM) channel and freq
const int pwmChannel = 0;
const int pwmFreq = 2000;
const int pwmResolution = 8; // 8-bit resolution

// Calibration: adjust these to your sensor/soil
int dryThreshold = 2500;   // lower analog value = wetter; adjust after testing
int wetThreshold = 1500;   // sample numbers — calibrate for your setup

// Melody (note frequencies in Hz). A simple 5-note chime.
const int melodyNotes[] = { 523, 659, 784, 659, 523 }; // C5, E5, G5, E5, C5
const int melodyLen = sizeof(melodyNotes) / sizeof(melodyNotes[0]);
const int noteDur = 250; // ms per note

unsigned long lastAlert = 0;
const unsigned long alertInterval = 1000UL * 60UL * 30UL; // 30 minutes between alerts

void playNote(int freq, int duration_ms) {
  if (freq <= 0) {
    ledcWriteTone(pwmChannel, 0);
    delay(duration_ms);
    return;
  }
  ledcWriteTone(pwmChannel, freq);
  delay(duration_ms);
  ledcWriteTone(pwmChannel, 0);
  delay(30);
}

void playMelody() {
  for (int i = 0; i < melodyLen; ++i) {
    playNote(melodyNotes[i], noteDur);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Plant helper starting...");

  // Setup PWM for buzzer
  ledcAttachPin(buzzerPin, pwmChannel);
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  
  pinMode(soilPin, INPUT);
  // Give a startup tone
  playNote(880, 100); // small beep
}

void loop() {
  // Read ADC (ESP32 ADC is 12-bit but readRaw returns 0-4095 using analogRead)
  int raw = analogRead(soilPin);
  // Optional: you can average several reads for stability
  // calibrate: print raw value when dry and when wet to choose threshold
  Serial.print("Soil raw: ");
  Serial.println(raw);

  // Determine dryness
  // If raw value bigger => dryer (depends on sensor). Confirm by testing and swap sign if needed.
  if (raw > dryThreshold) {
    unsigned long now = millis();
    if (now - lastAlert > alertInterval) {
      Serial.println("Soil is dry — playing melody");
      playMelody();
      lastAlert = now;
    } else {
      Serial.println("Soil is dry but recently alerted. Skipping beep.");
    }
  } else {
    // wet enough
    // Optional: a happy chime when it becomes wet again
    Serial.println("Soil ok.");
  }

  delay(5000); // poll interval (5s); increase to save power
}