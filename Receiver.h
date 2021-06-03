/*
  Receiver module.
  2021-05-23  T. Nakagawa
*/

#ifndef RECEIVER_H_
#define RECEIVER_H_

struct Receiver {
  uint16_t value[8];	// [0, 1000] for ROLL, PITCH, YAW, THROT, AUX1, AUX2, AUX3, AUX4.
};

extern Receiver rec;

void rec_init();	// Initialize the receiver.
void rec_process();	// Process the receiver.
void rec_reset();	// Reset binding.
bool rec_online();
bool rec_binded();

#endif
