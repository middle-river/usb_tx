#include "Gamepad.h"
#include "Receiver.h"

Gamepad gamepad;

void setup() {
  pinMode(PB2, INPUT);
  pinMode(PC13, OUTPUT);
  gamepad.begin();
  rec_init();
}

void loop() {
  static uint16_t old_value[8];
  delayMicroseconds(100);
  rec_process();
  bool updated = false;
  for (int i = 0; i < 8; i++) {
    if (rec.value[i] != old_value[i]) {
      updated = true;
      break;
    }
  }
  if (updated) {
    gamepad.move(rec.value);
    for (int i = 0; i < 8; i++) old_value[i] = rec.value[i];
  }

  if (digitalRead(PB2) == HIGH) {
    rec_reset();
  }

  if (rec_online()) {
    digitalWrite(PC13, LOW);
  } else if (rec_binded()) {
    digitalWrite(PC13, HIGH);
  } else {
    digitalWrite(PC13, (millis() & 0x200) ? HIGH : LOW);
  }
}
