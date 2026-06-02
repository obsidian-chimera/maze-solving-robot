```cpp
// Arduino Mega encoder counts/rev test
// Serial Monitor: 9600 baud
//
// Wiring:
// M1 Front Left:  A = D2,  B = D22
// M2 Rear Left:   A = D18, B = D24
// M3 Front Right: A = D19, B = D26
// M4 Rear Right:  A = D20, B = D28
//
// Direction convention validated by Role 4:
// On channel A RISING, B LOW increments count; B HIGH decrements count.
//
// Commands:
// z = reset all counts
// 1 = reset M1 count
// 2 = reset M2 count
// 3 = reset M3 count
// 4 = reset M4 count

const int ENCODER_COUNT = 4;

const int ENCODER_A_PINS[ENCODER_COUNT] = {2, 18, 19, 20};
const int ENCODER_B_PINS[ENCODER_COUNT] = {22, 24, 26, 28};

volatile long encoderCounts[ENCODER_COUNT] = {0, 0, 0, 0};

unsigned long lastPrintTime = 0;

void updateEncoder(int index) {
  if (digitalRead(ENCODER_B_PINS[index]) == LOW) {
    encoderCounts[index]++;
  } else {
    encoderCounts[index]--;
  }
}

void updateEncoder1() {
  updateEncoder(0);
}

void updateEncoder2() {
  updateEncoder(1);
}

void updateEncoder3() {
  updateEncoder(2);
}

void updateEncoder4() {
  updateEncoder(3);
}

long readEncoderCount(int index) {
  noInterrupts();
  long count = encoderCounts[index];
  interrupts();
  return count;
}

void resetEncoderCount(int index) {
  noInterrupts();
  encoderCounts[index] = 0;
  interrupts();
}

void resetAllEncoderCounts() {
  noInterrupts();
  for (int i = 0; i < ENCODER_COUNT; i++) {
    encoderCounts[i] = 0;
  }
  interrupts();
}

void printCounts() {
  Serial.print("M1_FL=");
  Serial.print(readEncoderCount(0));

  Serial.print(" | M2_RL=");
  Serial.print(readEncoderCount(1));

  Serial.print(" | M3_FR=");
  Serial.print(readEncoderCount(2));

  Serial.print(" | M4_RR=");
  Serial.println(readEncoderCount(3));
}

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < ENCODER_COUNT; i++) {
    pinMode(ENCODER_A_PINS[i], INPUT);
    pinMode(ENCODER_B_PINS[i], INPUT_PULLUP);
  }

  resetAllEncoderCounts();

  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PINS[0]), updateEncoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PINS[1]), updateEncoder2, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PINS[2]), updateEncoder3, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PINS[3]), updateEncoder4, RISING);

  Serial.println("Mega encoder counts/rev test started.");
  Serial.println("Rotate one wheel exactly one full revolution.");
  Serial.println("Use abs(count) as counts per revolution.");
  Serial.println("Commands: z reset all, 1/2/3/4 reset one motor.");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'z' || command == 'Z') {
      resetAllEncoderCounts();
      Serial.println("All counts reset.");
    } else if (command >= '1' && command <= '4') {
      int index = command - '1';
      resetEncoderCount(index);
      Serial.print("M");
      Serial.print(index + 1);
      Serial.println(" count reset.");
    }
  }

  if (millis() - lastPrintTime >= 250) {
    printCounts();
    lastPrintTime = millis();
  }
}
```
