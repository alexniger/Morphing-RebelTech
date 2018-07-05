


#include "WaveFormExtFFT.h"
#include "Morphing.h"


#include "morph1.h"
#include "morph2.h"
#include "morph3.h"
#include "morph4.h"

// oscillator

#define baseFrequency (20)  /* starting frequency of first table */  // c1 = 32.7 Hz



class WTExtWavePatch : public Patch {
private:
  WaveTableOsc *ext1;
  WaveFormExtFFT *wfe1;
  WaveTableOsc *ext2;
  WaveFormExtFFT *wfe2;
  Morphing *doublemorph;
  
  SmoothFloat freqA;
  SmoothFloat amp;
  FloatParameter Fc;
  FloatParameter Pac ;
public:
  WTExtWavePatch() {																		
	  ext1 = new WaveTableOsc();
	  wfe1 = new WaveFormExtFFT();																	
	  ext2 = new WaveTableOsc();
	  wfe2 = new WaveFormExtFFT();
	  doublemorph = new Morphing();
	  int foobarlen = 256;
	  FloatArray sample1 = FloatArray::create(foobarlen);
	  sample1.copyFrom(foobar1[0], foobarlen);
	  FloatArray sample2 = FloatArray::create(foobarlen);
	  sample2.copyFrom(foobar2[0], foobarlen);
	  FloatArray sample3 = FloatArray::create(foobarlen);
	  sample3.copyFrom(foobar3[0], foobarlen);
	  FloatArray sample4 = FloatArray::create(foobarlen);
	  sample4.copyFrom(foobar4[0], foobarlen);
	  int M = 1450; //1220;
	  int val1 = wfe1->makeWaveTableExt(sample4, ext1, baseFrequency, foobarlen, M);
	  int val2 = wfe2->makeWaveTableExt(sample3, ext2, baseFrequency, foobarlen, M);
	      //debugMessage("coupure", ext1->numWaveTables, val);
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
    ext1->setFrequency(freqA/sampleRate);
    ext2->setFrequency(freqA/sampleRate);
    
    
    
    
    float morphD = getParameterValue(PARAMETER_D)*50.0;
    
    doublemorph->setMorphing(morphD/sampleRate);
    
	//debugMessage("coupure", ext1->getOutput(), freqA);
    
    
    debugMessage("coupure", freqA, (float) 0.0);
    
    amp = getParameterValue(PARAMETER_B);
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    for(int n = 0; n<buffer.getSize(); n++){
		left[n] = (doublemorph->getMorphOutput(ext1 , ext2) * amp ) * 0.5;				
		//right[n] = (osc2->getOutput()) * ampD * 0.5;	
		ext1->updatePhase();   
		ext2->updatePhase();      
		doublemorph->updateMorph();
	}
	}
};



