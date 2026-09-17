#include <Arduino.h>

#define TRIG_PIN 5
#define ECHO_PIN 18

// Simulated ESP32-CAM prediction button
#define CAMERA_BUTTON 15

#define GREEN_LED 21
#define YELLOW_LED 22
#define RED_LED 23
#define BUZZER 19


// Wokwi may return an unreliable HC-SR04 value during the demo.
// Therefore distanceMode selects a predictable test distance:
//
// distanceMode 0 -> 40 cm: safe distance
// distanceMode 1 -> 20 cm: warning distance
// distanceMode 2 -> 10 cm: critical distance


int distanceMode = 2;

float readRealDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return 999.0;
  }

  return duration / 58.0;
}

// returns a simulated distance based on distanceMode.
float readSimulatedDistanceCm() {
  if (distanceMode == 0) {
    return 40.0;
  }

  if (distanceMode == 1) {
    return 20.0;
  }

  return 10.0;
}

const char* distanceModeName() {
  if (distanceMode == 0) {
    return "SAFE_TEST_DISTANCE";
  }

  if (distanceMode == 1) {
    return "WARNING_TEST_DISTANCE";
  }

  return "CRITICAL_TEST_DISTANCE";
}

bool simulatedCameraInference() {
  // Button pressed means the simulated ESP32-CAM detects a person.

  return digitalRead(CAMERA_BUTTON) == LOW;
}

String makeDecision(
    bool personDetected,
    float distanceCm) {

  // No person detected: system is safe.
  if (!personDetected) {
    return "NORMAL";
  }

  // Person detected and very close to the machine.
  if (distanceCm <= 10.0) {
    return "CRITICAL";
  }

  // Person detected at a moderate distance.
  if (distanceCm <= 20.0) {
    return "WARNING";
  }

  // Person detected but outside the warning distance.
  return "NORMAL";
}

void applyAction(String state) {
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  if (state == "NORMAL") {
    digitalWrite(GREEN_LED, HIGH);
  }

  else if (state == "WARNING") {
    digitalWrite(YELLOW_LED, HIGH);
  }

  else if (state == "CRITICAL") {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(CAMERA_BUTTON, INPUT_PULLUP);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  applyAction("NORMAL");

  Serial.println("MacroVision Claw safety monitor started");
  Serial.println("GPIO 15 button: simulated ESP32-CAM prediction");
  Serial.print("Fixed demo distance: ");
  Serial.print(readSimulatedDistanceCm());
  Serial.print(" cm (");
  Serial.print(distanceModeName());
  Serial.println(")");
}

void loop() {
  bool personDetected =
      simulatedCameraInference();

  // ----------------------------------------------------------
  // DEMO MODE:
  // Use a fixed, deterministic test distance (set via distanceMode
  // above). This keeps the demo predictable regardless of what
  // the simulated ultrasonic sensor happens to report.
  // ----------------------------------------------------------
  float distanceCm =
      readSimulatedDistanceCm();

  // ----------------------------------------------------------
  // REAL SENSOR MODE:
  // To use the actual HC-SR04 later, replace the previous line
  // with:
  //
  // float distanceCm = readRealDistanceCm();
  // ----------------------------------------------------------

  String state =
      makeDecision(personDetected, distanceCm);

  applyAction(state);

  Serial.print("Camera prediction: ");
  Serial.print(
      personDetected ? "PERSON_DETECTED" : "NO_PERSON");

  Serial.print(" | Simulated distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm | Mode: ");
  Serial.print(distanceModeName());

  Serial.print(" | Decision: ");
  Serial.println(state);

  delay(500);
}
