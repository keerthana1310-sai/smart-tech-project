const int soilPin = 34;
const int buzzerPin = 25;
const int sampleCount = 8;
int dryThreshold = 2800;
unsigned long alertInterval = 30UL * 60UL * 1000UL;
unsigned long lastAlert = 0;

void playToneSW(int freq, int duration_ms) {
  if (freq <= 0) { delay(duration_ms); return; }
  long period_us = 1000000L / freq;
  long half = period_us / 2;
  unsigned long iterations = (unsigned long)duration_ms * 1000UL / period_us;
  for (unsigned long i = 0; i < iterations; ++i) {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(half);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(half);
  }
}

void playMelody() {
  int notes[] = {523, 659, 784, 659, 523};
  int len = sizeof(notes) / sizeof(notes[0]);
  for (int i = 0; i < len; ++i) {
    playToneSW(notes[i], 200);
    delay(40);
  }
}

int readSoil() {
  long s = 0;
  for (int i = 0; i < sampleCount; ++i) {
    s += analogRead(soilPin);
    delay(20);
  }
  return (int)(s / sampleCount);
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  delay(500);
  Serial.println("Plant-Me-Gently starting...");
}

void loop() {
  int raw = readSoil();
  Serial.print("Soil raw: ");
  Serial.println(raw);
  if (raw > dryThreshold) {
    unsigned long now = millis();
    if (now - lastAlert > alertInterval) {
      Serial.println("Soil dry -> alert");
      playMelody();
      lastAlert = now;
    } else {
      Serial.println("Soil dry but recently alerted");
    }
  } else {
    Serial.println("Soil OK");
  }
  delay(5000);
}
