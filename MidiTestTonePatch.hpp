#include "Patch.h"
#include "SineOscillator.h"

class MidiTestTonePatch : public Patch {
private:
  SineOscillator osc;
  float freq;
  float amp;
  float pitchbend;
public:
  MidiTestTonePatch() {
    osc.setSampleRate(getSampleRate());
  }
  void buttonChanged(PatchButtonId bid, uint16_t value, uint16_t samples){
    if(bid >= MIDI_NOTE_BUTTON){
      int note = bid - MIDI_NOTE_BUTTON;
      freq = 440.0f*powf(2, (note-57 + pitchbend*2)/12.0);
      amp = value/4096.0f;
    }
  }
  void processAudio(AudioBuffer &buffer) {
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    int trig;
    osc.setFrequency(10000);
    if (osc.getNextSample() != 0) {
		trig = 8;
		} 
		else trig =0; 
    for(int n = 0; n<buffer.getSize(); n++)
      left[n] = trig*amp*1;
  }
};
