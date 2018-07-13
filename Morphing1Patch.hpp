
#include "WaveFormOsc.h"
#include "WTMatrix.h"

#include "AMEN_LOOhalf.h"

#define baseFrequency (20)  /* starting frequency of first table */  // c1 = 32.7 Hz

class Morphing1Patch : public Patch {
private:
  WaveFormOsc *osc;
  WaveFormOsc *osc2;
  WTMatrix *wtm;
//  WTMatrix *wtm2;
  
  SmoothFloat freqA;
  SmoothFloat amp;
  FloatParameter Fc;
  FloatParameter Pac ;
  
public:
  Morphing1Patch() {																		
	  osc = new WaveFormOsc();															
	  osc2 = new WaveFormOsc();	
	  wtm = new WTMatrix();	
	//  wtm2 = new WTMatrix();		
	  int foobarlen = 256;
	  int numwaves = 4;
	  FloatArray sample(AL32samples[0], sizeof(AL32samples[0])/sizeof(float));
	
	int val1 = wtm->makeMatrix(osc, sample, foobarlen, baseFrequency);
	int val2 = wtm->makeMatrix(osc2, sample, foobarlen, baseFrequency);

	  //debugMessage("coupure", osc->numWaveForms, fullsample.getSize());
	  //freq = getFloatParameter("Frequency", baseFrequency, c5, c3, 0.97, 0.0, Patch::EXP);
	  registerParameter(PARAMETER_A, "Frequency");
	  registerParameter(PARAMETER_B, "Amp");
	  registerParameter(PARAMETER_C, "Filter");
	  registerParameter(PARAMETER_D, "Q");
    
  }
  void processAudio(AudioBuffer &buffer) {
    //float freq = getParameterValue(PARAMETER_A)*(20000-baseFrequency) + baseFrequency;
	//float C = getParameterValue(PARAMETER_C);
    //float multC = pow(2.0 , ((C - (pow((C-0.5)*7.0/25.0 , 2.0) - 0.07)) * 2.094 - 1.047));
    float unphased = 0.0;
    float detune = 0.0;
    
    float note = getParameterValue(PARAMETER_A)*120.0 + 17.0;
    freqA = exp2f((note-69.0)/12.0)*440.0;
    osc->setFrequency(freqA/sampleRate);
   
    float morphC = getParameterValue(PARAMETER_C);  
    //if (morphC >= 1)  {
		//morphC-- ; 
		//}
    osc->setMorphing(morphC);
    
    float morphD = getParameterValue(PARAMETER_D); 
    
	enum mode {SYNC, DETUNE};
    //bool switchSync = 0;
    //bool switchDetune = 0;
    //if (switchDetune == 1 && switchSync == 0) {
		//mode = 2;
	//}
	//else if (switchSync == 1) {
			//mode = 1;
		//}
		//else mode = 0
	
	switch (2) {
		case SYNC:
			unphased = morphC + (morphD / osc->numWaveForms);
			if (unphased >= 1)  {
				unphased-- ; 
				}
			osc2->setMorphing(unphased);
			osc2->setFrequency(freqA/sampleRate);
			break;
		case DETUNE:
			detune = exp2f((note-69.0+(morphD*3.0))/12.0)*440.0;  // detune from 0 to +3 semitones
			osc2->setFrequency(detune/sampleRate);
			osc2->setMorphing(morphC);
			break;
		default:
			osc2->setMorphing(morphD);		
			osc2->setFrequency(freqA/sampleRate);
	}
    
	//debugMessage("coupure", up->getMorphOutput(), freqA);
    debugMessage("coupure", 0.0, (float) osc->totalWaves);
    
    amp = getParameterValue(PARAMETER_B);
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
    
    for(int n = 0; n<buffer.getSize(); n++){
		left[n] = (osc->getMorphOutput() * amp) * 0.5;				
		right[n] = (osc2->getMorphOutput()) * amp * 0.5;	
		osc->updatePhase(); 
		osc2->updatePhase();
	}
	}
};

