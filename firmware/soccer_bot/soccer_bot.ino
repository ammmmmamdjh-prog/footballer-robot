/*
 * ═══════════════════════════════════════════════════════════════
 *  SOCCER ROBOT — Autonomous Striker (state machine edition)
 *  Author : AmirAli  ·  github.com/ammmmmamdjh-prog
 *  Board  : Arduino Mega / UNO
 *  Sensors: 360° IR ball seeker + 2 border line sensors
 *  Actuators: 2 drive motors (L298N) + solenoid kicker + dribbler
 *
 *  States: SEARCH → CHASE → ATTACK → KICK → RETURN
 * ═══════════════════════════════════════════════════════════════
 */

#include <Servo.h>

/* ── Pin map ─────────────────────────────────────────────────── */
// IR ball seeker — analog bearing (0..1023) + analog distance
const uint8_t PIN_SEEK_BEARING = A0;
const uint8_t PIN_SEEK_DIST    = A1;

// Field border line sensors (left / right, under the chassis)
const uint8_t PIN_LINE_L = A2;
const uint8_t PIN_LINE_R = A3;

// L298N drive motors
const uint8_t PIN_ENA = 5, PIN_IN1 = 7, PIN_IN2 = 8;    // left
const uint8_t PIN_ENB = 6, PIN_IN3 = 9, PIN_IN4 = 10;   // right

// Kicker (MOSFET gate) & dribbler motor
const uint8_t PIN_KICK     = 11;
const uint8_t PIN_DRIBBLER = 12;

/* ── Tuning constants ────────────────────────────────────────── */
int  CHASE_SPEED    = 180;    // 0..255
int  ATTACK_SPEED   = 220;
int  KICK_RANGE     = 25;     // seeker distance below → shoot
int  KICK_PULSE_MS  = 25;     // solenoid pulse length
int  LINE_BACKOFF_MS= 400;    // reverse time at the border
unsigned long SEARCH_TIMEOUT_MS = 4000;

/* ── State machine ───────────────────────────────────────────── */
enum BotState : uint8_t { SEARCH, CHASE, ATTACK, KICK, RETURN_HOME };
BotState state = SEARCH;
unsigned long stateSince = 0;

/* ── Setup ───────────────────────────────────────────────────── */
void setup() {
  pinMode(PIN_IN1, OUTPUT); pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT); pinMode(PIN_IN4, OUTPUT);
  pinMode(PIN_KICK, OUTPUT);
  pinMode(PIN_DRIBBLER, OUTPUT);

  stopDrive();
  Serial.begin(115200);
  Serial.println(F("SOCCER BOT ONLINE — state machine v1"));
}

/* ── Main loop ───────────────────────────────────────────────── */
void loop() {
  int bearing  = analogRead(PIN_SEEK_BEARING);   // 0..1023 → ball direction
  int distance = analogRead(PIN_SEEK_DIST);      // smaller = closer (depends on seeker)
  bool onLineL = analogRead(PIN_LINE_L) > 800;   // white border detected
  bool onLineR = analogRead(PIN_LINE_R) > 800;

  // ── Border safety has highest priority ──
  if (onLineL || onLineR) {
    backOffFromBorder(onLineL, onLineR);
    setState(SEARCH);
    return;
  }

  switch (state) {

    case SEARCH:
      spinRight(120);
      if (distance > 0) {                 // seeker sees something
        setState(CHASE);
      } else if (millis() - stateSince > SEARCH_TIMEOUT_MS) {
        setState(SEARCH);                 // keep sweeping
      }
      break;

    case CHASE:
      driveToward(bearing, CHASE_SPEED);
      digitalWrite(PIN_DRIBBLER, HIGH);   // spin dribbler
      if (distance <= KICK_RANGE) {
        setState(ATTACK);
      } else if (distance == 0) {
        setState(SEARCH);
      }
      break;

    case ATTACK:
      driveToward(bearing, ATTACK_SPEED);
      if (distance <= KICK_RANGE / 2) {
        setState(KICK);
      } else if (distance > KICK_RANGE) {
        setState(CHASE);
      }
      break;

    case KICK:
      stopDrive();
      fireKicker();
      setState(RETURN_HOME);
      break;

    case RETURN_HOME:
      driveBackward(CHASE_SPEED / 2);
      if (millis() - stateSince > 600) {
        digitalWrite(PIN_DRIBBLER, LOW);
        setState(SEARCH);
      }
      break;
  }
}

/* ── Helpers ─────────────────────────────────────────────────── */
void setState(BotState next) {
  state = next;
  stateSince = millis();
  Serial.print(F("STATE → "));
  Serial.println(next);
}

// Map seeker bearing to differential drive
void driveToward(int bearing, int speed) {
  int error = bearing - 512;              // centered ≈ 512
  int correction = error / 4;             // simple P controller
  setMotors(speed - correction, speed + correction);
}

void backOffFromBorder(bool leftHit, bool rightHit) {
  driveBackward(CHASE_SPEED);
  delay(LINE_BACKOFF_MS);
  if (leftHit)  spinRight(140), delay(250);
  if (rightHit) spinLeft(140),  delay(250);
  stopDrive();
}

void fireKicker() {
  digitalWrite(PIN_KICK, HIGH);
  delay(KICK_PULSE_MS);
  digitalWrite(PIN_KICK, LOW);
  Serial.println(F("⚡ KICK!"));
}

void setMotors(int left, int right) {
  left  = constrain(left,  -255, 255);
  right = constrain(right, -255, 255);
  digitalWrite(PIN_IN1, left  >= 0); digitalWrite(PIN_IN2, left  < 0);
  analogWrite (PIN_ENA, abs(left));
  digitalWrite(PIN_IN3, right >= 0); digitalWrite(PIN_IN4, right < 0);
  analogWrite (PIN_ENB, abs(right));
}

void stopDrive()      { setMotors(0, 0); }
void driveBackward(int s) { setMotors(-s, -s); }
void spinRight(int s) { setMotors(s, -s); }
void spinLeft(int s)  { setMotors(-s, s); }
