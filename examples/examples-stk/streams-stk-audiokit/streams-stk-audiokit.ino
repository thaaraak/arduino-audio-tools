/**
 * @file streams-stk-audioout.ino
 * @brief Plays random notes on instrument. For available instruments
 * see https://pschatzmann.github.io/Arduino-STK/html/classstk_1_1Instrmnt.html
 * I used an AudioKitStream to test the output, but you can replace it with any other output class (e.g. I2SStream)
 * @author Phil Schatzmann
 * @copyright Copyright (c) 2021
 */
#include "AudioTools.h"
#include "AudioLibs/AudioSTK.h"
#include "AudioLibs/AudioKit.h"

Sitar instrument(440);
STKStream<Instrmnt> in(instrument);
AudioKitStream out;
StreamCopy copier(out, in);
MusicalNotes notes;

float note_amplitude = 0.5;
static uint16_t notes_array[] = { // frequencies aleatoric C-scale
    N_C3, N_D3, N_E3, N_F3, N_G3, N_A3, N_B3,
    N_C4, N_D4, N_E4, N_F4, N_G4, N_A4, N_B4,
    N_C5, N_D5, N_E5, N_F5, N_G5, N_A5, N_B5
};

void play() {
  static bool active=true;
  static float freq;
  static uint64_t timeout;

  if (millis()>timeout){
    if (active){
      // play note for 800 ms
      freq = notes_array[random(sizeof(notes_array)/sizeof(uint16_t))];
      instrument.noteOn(freq, note_amplitude);
      timeout = millis()+800;
      active = false;
    } else {
      // silence for 100 ms
      instrument.noteOff(note_amplitude);
      timeout = millis()+100;
      active = true;
    }
  }
}

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial,AudioLogger::Warning);

  // setup input
  auto icfg = in.defaultConfig();
  in.begin(icfg);

  // setup output
  auto ocfg = out.defaultConfig(TX_MODE);
  ocfg.copyFrom(icfg);
  ocfg.sd_active = false;
  out.begin(ocfg);

}

void loop() {
  play();
  copier.copy();
}
