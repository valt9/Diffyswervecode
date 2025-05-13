#include <CytronMotorDriver.h>

// Updated motor driver setup for Modules 1 & 2
CytronMD motor1A(PWM_DIR, 6, 7);   // Module 1 Motor A: PWM 6, DIR 7
CytronMD motor1B(PWM_DIR, 8, 9);   // Module 1 Motor B: PWM 8, DIR 9
CytronMD motor2A(PWM_DIR, 10, 12); // Module 2 Motor A: PWM 10, DIR 11
CytronMD motor2B(PWM_DIR, 11, 13); // Module 2 Motor B: PWM 12, DIR 13

// Encoder pin assignments (unconflicted)
const int ENC1_A_PIN = 2;  // Interrupt-capable
const int ENC1_B_PIN = 4;
const int ENC2_A_PIN = 3;  // Interrupt-capable
const int ENC2_B_PIN = 5;

// Encoder counts
volatile long encoder1A_count = 0;
volatile long encoder1B_count = 0;
volatile long encoder2A_count = 0;
volatile long encoder2B_count = 0;

// Interrupt service routines
void encoder1A_ISR() { encoder1A_count++; }
void encoder1B_ISR() { encoder1B_count++; }
void encoder2A_ISR() { encoder2A_count++; }
void encoder2B_ISR() { encoder2B_count++; }

void setup() {
  Serial.begin(115200);

  pinMode(ENC1_A_PIN, INPUT);
  pinMode(ENC1_B_PIN, INPUT);
  pinMode(ENC2_A_PIN, INPUT);
  pinMode(ENC2_B_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENC1_A_PIN), encoder1A_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC1_B_PIN), encoder1B_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC2_A_PIN), encoder2A_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC2_B_PIN), encoder2B_ISR, RISING);
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("M1:")) {
      int m1a = command.substring(3, command.indexOf(',')).toInt();
      int m1b = command.substring(command.indexOf(',') + 1, command.indexOf(';')).toInt();
      motor1A.setSpeed(m1a);
      motor1B.setSpeed(m1b);
    }

    if (command.indexOf("M2:") != -1) {
      int m2_start = command.indexOf("M2:") + 3;
      int m2a = command.substring(m2_start, command.indexOf(',', m2_start)).toInt();
      int m2b = command.substring(command.indexOf(',', m2_start) + 1).toInt();
      motor2A.setSpeed(m2a);
      motor2B.setSpeed(m2b);
    }
  }
}
