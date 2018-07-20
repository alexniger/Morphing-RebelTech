
#include "MorphOsc.h"
#include "WTFactory.h"

#include "bank1_410.h"
#include "dose3_wit.h"

#define baseFrequency (20)  /* starting frequency of first table */  // c1 = 32.7 Hz

class DualMorphing1DPatch : public Patch {
private:
  MorphOsc *osc1;
  MorphOsc *osc2;
  WTFactory *wtm;
  
  SmoothFloat freqA;
  SmoothFloat amp;
  FloatParameter Fc;
  FloatParameter Pac ;
  
public:
  DualMorphing1DPatch() {																		
	  osc1 = new MorphOsc();															
	  osc2 = new MorphOsc();	
	  wtm = new WTFactory();		
	  int foobarlen = 256;
	  int numwaves = 4;
	  FloatArray sample1(bank1[0], sizeof(bank1[0])/sizeof(float));
	  FloatArray sample2(dose2[0], sizeof(dose2[0])/sizeof(float));
	
	int val1 = wtm->makeMatrix(osc1, sample1, foobarlen, baseFrequency);
	int val2 = wtm->makeMatrix(osc2, sample1, foobarlen, baseFrequency);

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
    osc1->setFrequency(freqA/sampleRate);
   
    float morphC = getParameterValue(PARAMETER_C);  
    //if (morphC >= 1)  {
		//morphC-- ; 
		//}
    osc1->setMorphing(morphC);
    
    float morphD = getParameterValue(PARAMETER_D); 
    
	enum mode {SYNC, DETUNE};

	
	switch (2) {
		case SYNC:
			unphased = morphC + (morphD / osc1->numWaveForms);
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
    
    debugMessage("coupure", 0.0, (float) osc1->totalWaves);
    
    amp = getParameterValue(PARAMETER_B);
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
    
    for(int n = 0; n<buffer.getSize(); n++){
		left[n] = (osc1->getMorphOutput() * amp) * 0.5;				
		right[n] = (osc2->getMorphOutput()) * amp * 0.5;	
		osc1->updatePhase(); 
		osc2->updatePhase();
	}
	}
};
