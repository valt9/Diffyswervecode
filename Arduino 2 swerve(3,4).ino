#include <CytronMotorDriver.h>

// Updated motor driver setup for Modules 3 & 4
CytronMD motor3A(PWM_DIR, 6, 7);   // Module 3 Motor A: PWM 6, DIR 7
CytronMD motor3B(PWM_DIR, 8, 9);   // Module 3 Motor B: PWM 8, DIR 9
CytronMD motor4A(PWM_DIR, 10, 12); // Module 4 Motor A: PWM 10, DIR 11
CytronMD motor4B(PWM_DIR, 11, 13); // Module 4 Motor B: PWM 12, DIR 13

// Encoder pin assignments (unconflicted)
const int ENC3_A_PIN = 2;  // Interrupt-capable
const int ENC3_B_PIN = 4;
const int ENC4_A_PIN = 3;  // Interrupt-capable
const int ENC4_B_PIN = 5;

// Encoder counts
volatile long encoder3A_count = 0;
volatile long encoder3B_count = 0;
volatile long encoder4A_count = 0;
volatile long encoder4B_count = 0;

// Interrupt service routines
void encoder3A_ISR() { encoder3A_count++; }
void encoder3B_ISR() { encoder3B_count++; }
void encoder4A_ISR() { encoder4A_count++; }
void encoder4B_ISR() { encoder4B_count++; }

void setup() {
  Serial.begin(115200);

  pinMode(ENC3_A_PIN, INPUT);
  pinMode(ENC3_B_PIN, INPUT);
  pinMode(ENC4_A_PIN, INPUT);
  pinMode(ENC4_B_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENC3_A_PIN), encoder3A_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC3_B_PIN), encoder3B_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC4_A_PIN), encoder4A_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC4_B_PIN), encoder4B_ISR, RISING);
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("M3:")) {
      int m3a = command.substring(3, command.indexOf(',')).toInt();
      int m3b = command.substring(command.indexOf(',') + 1, command.indexOf(';')).toInt();
      motor3A.setSpeed(m3a);
      motor3B.setSpeed(m3b);
    }

    if (command.indexOf("M4:") != -1) {
      int m4_start = command.indexOf("M4:") + 3;
      int m4a = command.substring(m4_start, command.indexOf(',', m4_start)).toInt();
      int m4b = command.substring(command.indexOf(',', m4_start) + 1).toInt();
      motor4A.setSpeed(m4a);
      motor4B.setSpeed(m4b);
    }
  }
}
