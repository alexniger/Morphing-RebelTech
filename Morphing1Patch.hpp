
#include "WaveFormOsc.h"
#include "WTMatrix.h"

#include "dose1_wit.h"
//#include "bank1_410.h"

#define baseFrequency (20)  /* starting frequency of first table */  // c1 = 32.7 Hz

#define NOF_OSC 2

class Morphing1Patch : public Patch {
private:
  WaveFormOsc *oscs[NOF_OSC];
  //WaveFormOsc *osc2;
  //WaveFormOsc *osc3;
  //WaveFormOsc *osc4;
  //WaveFormOsc *osc5;
  //WaveFormOsc *osc6;
  //WaveFormOsc *osc7;
  //WaveFormOsc *osc8;
  WTMatrix *wtm;
  
  SmoothFloat freqA;
  SmoothFloat amp;
  FloatParameter Fc;
  FloatParameter Pac ;
  
public:
  Morphing1Patch() {																		

	for (int i = 0 ; i<NOF_OSC ; i++)  {
		oscs[i] = new WaveFormOsc();
	}																
	  //osc3 = new WaveFormOsc();															
	  //osc4 = new WaveFormOsc();																	
	  //osc5 = new WaveFormOsc();															
	  //osc6 = new WaveFormOsc();																	
	  //osc7 = new WaveFormOsc();															
	  //osc8 = new WaveFormOsc();	
	  wtm = new WTMatrix();		
	  int foobarlen = 256;
	  int numosc = 3;
	  FloatArray sample(DOSE1[0], sizeof(DOSE1[0])/sizeof(float));
	  //FloatArray sample1(bank1[0], sizeof(bank1[0])/sizeof(float));
	
	int val1 = wtm->makeMatrix(oscs[0], sample, foobarlen, baseFrequency);
	int val2 = wtm->makeMatrix(oscs[1], sample, foobarlen, baseFrequency);
	//int val1 = wtm->makeMatrix(oscs[2], sample, foobarlen, baseFrequency);
	//int val2 = wtm->makeMatrix(oscs[3], sample, foobarlen, baseFrequency);
	//int val3 = wtm->makeMatrix(osc3, sample, foobarlen, baseFrequency);

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
    oscs[0]->setFrequency(freqA/sampleRate);
   
    float morphC = getParameterValue(PARAMETER_C);  
    //if (morphC >= 1)  {
		//morphC-- ; 
		//}
    oscs[0]->setMorphing(morphC);
    
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
			unphased = morphC + (morphD / oscs[0]->numWaveForms);
			if (unphased >= 1)  {
				unphased-- ; 
				}
			oscs[1]->setMorphing(unphased);
			oscs[1]->setFrequency(freqA/sampleRate);
			break;
		case DETUNE:
			detune = exp2f((note-69.0+(morphD*3.0))/12.0)*440.0;  // detune from 0 to +3 semitones
			oscs[1]->setFrequency(detune/sampleRate);
			oscs[1]->setMorphing(morphC);
			break;
		default:
			oscs[1]->setMorphing(morphD);		
			oscs[1]->setFrequency(freqA/sampleRate);
	}
    
	//debugMessage("coupure", up->getMorphOutput(), freqA);
    debugMessage("coupure", 0.0, (float) oscs[0]->totalWaves);
    
    amp = getParameterValue(PARAMETER_B);
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
    
    for(int n = 0; n<buffer.getSize(); n++){
		left[n] = (oscs[0]->getMorphOutput() * amp) * 0.5;				
		right[n] = (oscs[1]->getMorphOutput()) * amp * 0.5;	
		oscs[0]->updatePhase(); 
		oscs[1]->updatePhase();
	}
	}
};

