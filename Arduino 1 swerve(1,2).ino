// Motor A of module 1 and 2
int M1_A1 = 3;  // PWM pin for motor 1A
int M1_A2 = 4;  // Direction pin for motor 1A
int M2_A1 = 5;  // PWM pin for motor 2A
int M2_A2 = 6;  // Direction pin for motor 2A

// Encoder pins (interrupt pins)
const int encoder1_A = 2;  // Encoder A for motor 1
const int encoder1_B = 7;  // Encoder B for motor 1
volatile int encoder1_count = 0;  // Encoder count for motor 1

const int encoder2_A = 8;  // Encoder A for motor 2
const int encoder2_B = 9;  // Encoder B for motor 2
volatile int encoder2_count = 0;  // Encoder count for motor 2

void setup() {
  // Set motor pins as output
  pinMode(M1_A1, OUTPUT);
  pinMode(M1_A2, OUTPUT);
  pinMode(M2_A1, OUTPUT);
  pinMode(M2_A2, OUTPUT);

  // Set encoder pins as input
  pinMode(encoder1_A, INPUT);
  pinMode(encoder1_B, INPUT);
  pinMode(encoder2_A, INPUT);
  pinMode(encoder2_B, INPUT);
  
  // Attach interrupts to encoder pins
  attachInterrupt(digitalPinToInterrupt(encoder1_A), encoder1_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2_A), encoder2_ISR, CHANGE);

  // Start serial communication
  Serial.begin(115200);
}

void loop() {
  // Check if data is available in the serial buffer
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    
    // Parse motor commands from the received string
    int m1a, m1b, m2a, m2b;
    sscanf(command.c_str(), "M1:%d,%d;M2:%d,%d", &m1a, &m1b, &m2a, &m2b);
    
    // Control motor 1
    analogWrite(M1_A1, abs(m1a));  // Set PWM for motor 1A
    digitalWrite(M1_A2, m1a > 0 ? HIGH : LOW);  // Set direction
    
    // Control motor 2
    analogWrite(M2_A1, abs(m2a));  // Set PWM for motor 2A
    digitalWrite(M2_A2, m2a > 0 ? HIGH : LOW);  // Set direction
  }
}

// Interrupt service routine for encoder 1
void encoder1_ISR() {
  int state1 = digitalRead(encoder1_B);
  encoder1_count += (state1 == HIGH) ? 1 : -1;  // Adjust count based on direction
}

// Interrupt service routine for encoder 2
void encoder2_ISR() {
  int state2 = digitalRead(encoder2_B);
  encoder2_count += (state2 == HIGH) ? 1 : -1;  // Adjust count based on direction
}
