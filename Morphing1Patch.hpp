
#include "WaveFormOsc.h"
#include "WTMatrix.h"

#include "AMEN_LOOreduced.h"

#define baseFrequency (20)  /* starting frequency of first table */  // c1 = 32.7 Hz

class Morphing1Patch : public Patch {
private:
  WaveFormOsc *osc;
  WaveFormOsc *osc2;
  WTMatrix *wtm;
  WTMatrix *wtm2;
  
  SmoothFloat freqA;
  SmoothFloat amp;
  FloatParameter Fc;
  FloatParameter Pac ;
  
public:
  Morphing1Patch() {																		
	  osc = new WaveFormOsc();															
	  osc2 = new WaveFormOsc();
	  wtm = new WTMatrix();	
	  wtm2 = new WTMatrix();		
	  int foobarlen = 256;
	  FloatArray sample(AL4samples[0], sizeof(AL4samples[0]));

	  FloatArray fullsample = FloatArray::create(foobarlen*4);
	
	fullsample.copyFrom(AL4samples[0], 2*foobarlen);
	
	  int val1 = wtm->makeMatrix(osc, fullsample, foobarlen, baseFrequency);
	int val2 = wtm->makeMatrix(osc2, fullsample, foobarlen, baseFrequency);


	
	FloatArray::destroy(fullsample);

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
    float note = getParameterValue(PARAMETER_A)*120.0 + 17.0;
    freqA = exp2f((note-69.0)/12.0)*440.0;
    osc->setFrequency(freqA/sampleRate);
    osc2->setFrequency(freqA/sampleRate);
    
    float morphC = getParameterValue(PARAMETER_C);  
    osc->setMorphing(morphC);
    
    float morphD = getParameterValue(PARAMETER_D);  
    osc2->setMorphing(morphD);
    
	//debugMessage("coupure", up->getMorphOutput(), freqA);
    //debugMessage("coupure", freqA, (float) 0.0);
    
    amp = getParameterValue(PARAMETER_B);
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    
    for(int n = 0; n<buffer.getSize(); n++){
		left[n] = (osc->getMorphOutput() + osc2->getMorphOutput() * amp ) * 0.5;				
		//right[n] = (osc2->getOutput()) * ampD * 0.5;	
		osc->updatePhase(); 
		osc2->updatePhase();
	}
	}
};

