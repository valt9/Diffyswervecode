// Motor A of module 3 and 4
int M3_A1 = 7;  // PWM pin for motor 3A
int M3_A2 = 8;  // Direction pin for motor 3A
int M4_A1 = 9;  // PWM pin for motor 4A
int M4_A2 = 10; // Direction pin for motor 4A

// Encoder pins (interrupt pins)
const int encoder3_A = 2;  // Encoder A for motor 3
const int encoder3_B = 11;  // Encoder B for motor 3
volatile int encoder3_count = 0;  // Encoder count for motor 3

const int encoder4_A = 12;  // Encoder A for motor 4
const int encoder4_B = 13;  // Encoder B for motor 4
volatile int encoder4_count = 0;  // Encoder count for motor 4

void setup() {
  // Set motor pins as output
  pinMode(M3_A1, OUTPUT);
  pinMode(M3_A2, OUTPUT);
  pinMode(M4_A1, OUTPUT);
  pinMode(M4_A2, OUTPUT);

  // Set encoder pins as input
  pinMode(encoder3_A, INPUT);
  pinMode(encoder3_B, INPUT);
  pinMode(encoder4_A, INPUT);
  pinMode(encoder4_B, INPUT);
  
  // Attach interrupts to encoder pins
  attachInterrupt(digitalPinToInterrupt(encoder3_A), encoder3_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder4_A), encoder4_ISR, CHANGE);

  // Start serial communication
  Serial.begin(115200);
}

void loop() {
  // Check if data is available in the serial buffer
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    
    // Parse motor commands from the received string
    int m3a, m3b, m4a, m4b;
    sscanf(command.c_str(), "M1:%d,%d;M2:%d,%d", &m3a, &m3b, &m4a, &m4b);
    
    // Control motor 3
    analogWrite(M3_A1, abs(m3a));  // Set PWM for motor 3A
    digitalWrite(M3_A2, m3a > 0 ? HIGH : LOW);  // Set direction
    
    // Control motor 4
    analogWrite(M4_A1, abs(m4a));  // Set PWM for motor 4A
    digitalWrite(M4_A2, m4a > 0 ? HIGH : LOW);  // Set direction
  }
}

// Interrupt service routine for encoder 3
void encoder3_ISR() {
  int state3 = digitalRead(encoder3_B);
  encoder3_count += (state3 == HIGH) ? 1 : -1;  // Adjust count based on direction
}

// Interrupt service routine for encoder 4
void encoder4_ISR() {
  int state4 = digitalRead(encoder4_B);
  encoder4_count += (state4 == HIGH) ? 1 : -1;  // Adjust count based on direction
}
