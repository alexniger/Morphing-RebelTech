
#include "MorphOsc.h"
#include "WTFactory.h"
#include "VoltsPerOctave.h"

#include "bank_4101536.h"
#include "wavetrip1536.h"
#include "synth_vo1536.h"
#include "alien_sp1536.h"
#include "additive1536.h"
#include "amen_loo1536.h"

#define baseFrequency (20)  /* starting frequency of first table */  

class DualMorphing2DPatch : public Patch {
	VoltsPerOctave hz;
private:
  MorphOsc *oscs[NOF_OSC*2];
  WTFactory *wtf;
  
  SmoothFloat freqA;
  SmoothFloat amp;
  
public:
  DualMorphing2DPatch() {																		

	for (int i = 0 ; i<(NOF_OSC*2) ; i++)  {
		oscs[i] = new MorphOsc();
	}								
	  wtf = new WTFactory();

	  FloatArray sample1(amen_loo6[0], SAMPLE_LEN*NOF_SAMPLES);
	wtf->makeMatrix(oscs[0], sample1, baseFrequency);	
	wtf->makeMatrix(oscs[0+NOF_OSC], sample1, baseFrequency);	
	  FloatArray sample2(additive6[0], SAMPLE_LEN*NOF_SAMPLES);
	wtf->makeMatrix(oscs[1], sample2, baseFrequency);	
	wtf->makeMatrix(oscs[1+NOF_OSC], sample2, baseFrequency);	
	  FloatArray sample3(wavetrip6[0], SAMPLE_LEN*NOF_SAMPLES);
	wtf->makeMatrix(oscs[2], sample3, baseFrequency);
	wtf->makeMatrix(oscs[2+NOF_OSC], sample3, baseFrequency);
	  //FloatArray sample4(alien_sp6[0], SAMPLE_LEN*NOF_SAMPLES);	
	//wtf->makeMatrix(oscs[3], sample4, baseFrequency);	
	//wtf->makeMatrix(oscs[3+NOF_OSC], sample4, baseFrequency);
	  //FloatArray sample5(synth_vo6[0], SAMPLE_LEN*NOF_SAMPLES);	
	//wtf->makeMatrix(oscs[4], sample5, baseFrequency);
	//wtf->makeMatrix(oscs[4+NOF_OSC], sample5, baseFrequency);
	  //FloatArray sample6(bank_4106[0], SAMPLE_LEN*NOF_SAMPLES);	
	//wtf->makeMatrix(oscs[5], sample6, baseFrequency);
	//wtf->makeMatrix(oscs[5+NOF_OSC], sample6, baseFrequency);

	  registerParameter(PARAMETER_A, "Frequency");
	  registerParameter(PARAMETER_B, "Amp");
	  registerParameter(PARAMETER_C, "Filter");
	  registerParameter(PARAMETER_D, "Q");
    
  }
  void processAudio(AudioBuffer &buffer) {
    float tune = getParameterValue(PARAMETER_A)*7.0 - 4.0;
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
    hz.setTune(tune);
    float freqA = hz.getFrequency(left[0]);
    float morphC = getParameterValue(PARAMETER_C);  
    float morphD = getParameterValue(PARAMETER_D); 
    
    float morphIdx = morphD * NOF_OSC;	
    int intPart = morphIdx; 
    float fracPart = morphIdx - intPart;
    
    MorphOsc* oscDownLeft = oscs[intPart];
    oscDownLeft->setFrequency(freqA/sampleRate);
    oscDownLeft->setMorphing(morphC);
    //MorphOsc* oscDownRight = oscs[intPart+6];
    //oscDownRight->setFrequency(freqA*1.008/sampleRate);
    //oscDownRight->setMorphing(morphC);
    
    intPart++;
    if (intPart >= NOF_OSC) {
        intPart = 0 ;
	}

    MorphOsc* oscUpLeft = oscs[intPart];
    oscUpLeft->setFrequency(freqA/sampleRate);
    oscUpLeft->setMorphing(morphC);
    //MorphOsc* oscUpRight = oscs[intPart+6];
    //oscUpRight->setFrequency(freqA*1.008/sampleRate);
    //oscUpRight->setMorphing(1.0-morphC);
    
    
    amp = getParameterValue(PARAMETER_B);
    
    for(int n = 0; n<buffer.getSize(); n++){
    
    float downLeft = oscDownLeft->getMorphOutput() * (1 - fracPart);
	oscDownLeft->updatePhase(); 
    float upLeft = oscUpLeft->getMorphOutput() * (fracPart);
	oscUpLeft->updatePhase();
    
    //float downRight = oscDownRight->getMorphOutput() * (1 - fracPart);
	//oscDownRight->updatePhase(); 
    //float upRight = oscUpRight->getMorphOutput() * (fracPart);
	//oscUpRight->updatePhase();
	
	left[n] = (downLeft + upLeft ) * amp * 0.5;	
	//right[n] = (downRight + 0.0 ) * amp * 0.5;				
		
	}
	}
};
