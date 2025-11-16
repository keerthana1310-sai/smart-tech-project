const int soilPin = 34;
const int sampleCount = 12;
unsigned long intervalMs = 1500;

int readSoilAverage() {
  long s = 0;
  int mini = 4096;
  int maxi = 0;
  for (int i = 0; i < sampleCount; ++i) {
    int v = analogRead(soilPin);
    s += v;
    if (v < mini) mini = v;
    if (v > maxi) maxi = v;
    delay(20);
  }
  int avg = (int)(s / sampleCount);
  Serial.print("avg:");
  Serial.print(avg);
  Serial.print(" min:");
  Serial.print(mini);
  Serial.print(" max:");
  Serial.println(maxi);
  return avg;
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("Calibration: place sensor in positions and note values");
}

void loop() {
  readSoilAverage();
  delay(intervalMs);
}
