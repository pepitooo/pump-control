#include <Arduino.h>

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif
#define WATER_PUMP_PIN 14
#define AIR_PUMP_PIN 15

long time_for_each_cycle = 10 * 60 * 1000L; // 10 min

long water_pump_start_after_cycle_begin = 1 * 60 * 1000L; // 1 min
long water_pump_activation_duration = 2 * 60 * 1000L; // 2 min

long air_pump_start_after_cycle_begin = .5 * 60 * 1000L; // 30 seconds
long air_pump_activation_duration = 30 * 1000L; // 30 seconds

unsigned long last_cycle_time_ms;

int water_pump_current_status = LOW;
void water_pump(int status) {
  if (status == water_pump_current_status) {
    return;
  }
  digitalWrite(WATER_PUMP_PIN, status);
  water_pump_current_status = status;

  Serial.print("Water pump : ");
  Serial.println(water_pump_current_status);
}

int air_pump_current_status = LOW;
void air_pump(int status) {
  if (status == air_pump_current_status) {
    return;
  }
  digitalWrite(AIR_PUMP_PIN, status);
  air_pump_current_status = status;

  Serial.print("Air pump : ");
  Serial.println(air_pump_current_status);
}

bool toggle_status = false;
void blink_toggle() {
  toggle_status = !toggle_status;
  if (toggle_status) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void start_new_cycle() {
  last_cycle_time_ms = millis();
  Serial.println("New cycle start");
}

void setup() {
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(AIR_PUMP_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Setup done");

  water_pump(HIGH);
  air_pump(HIGH);
  start_new_cycle();
}

int last_advance = 0;

void loop() {
  long time_since_cyle_begin = millis() - last_cycle_time_ms;

  if (time_since_cyle_begin - water_pump_start_after_cycle_begin > 0 &&
    time_since_cyle_begin - (water_pump_start_after_cycle_begin + water_pump_activation_duration) < 0) {
    water_pump(LOW);
  } else {
    water_pump(HIGH);
  }

  if (time_since_cyle_begin - air_pump_start_after_cycle_begin > 0 &&
    time_since_cyle_begin - (air_pump_start_after_cycle_begin + air_pump_activation_duration) < 0) {
    air_pump(LOW);
  } else {
    air_pump(HIGH);
  }

  if (time_since_cyle_begin - time_for_each_cycle > 0) {
    start_new_cycle();
  }
  int time_in_second = time_since_cyle_begin / 100;
  if ((time_in_second) % 10 == 0 && time_in_second != last_advance) {
    last_advance = time_in_second;
    // Serial.print(".");
    // if (last_advance % 60 == 0) {
    //   Serial.println("");
    // }
    blink_toggle();
  }
}
