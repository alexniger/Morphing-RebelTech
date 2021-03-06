
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

class Morphing2DPatch : public Patch {
	VoltsPerOctave hz;
private:
  MorphOsc *oscs[NOF_OSC];
  WTFactory *wtf;
  
  SmoothFloat freqA;
  SmoothFloat amp;
  
public:
  Morphing2DPatch() {																		

	for (int i = 0 ; i<NOF_OSC ; i++)  {
		oscs[i] = new MorphOsc();
	}								
	  wtf = new WTFactory();

	  FloatArray sample1(amen_loo6[0], SAMPLE_LEN*NOF_SAMPLES);
	wtf->makeMatrix(oscs[0], sample1, baseFrequency);	
	  FloatArray sample2(additive6[0], SAMPLE_LEN*NOF_SAMPLES);
	wtf->makeMatrix(oscs[1], sample2, baseFrequency);	
	  FloatArray sample3(wavetrip6[0], SAMPLE_LEN*NOF_SAMPLES);
	wtf->makeMatrix(oscs[2], sample3, baseFrequency);
	  //FloatArray sample4(alien_sp6[0], SAMPLE_LEN*NOF_SAMPLES);	
	//wtf->makeMatrix(oscs[3], sample4, baseFrequency);
	  //FloatArray sample5(synth_vo6[0], SAMPLE_LEN*NOF_SAMPLES);	
	//wtf->makeMatrix(oscs[4], sample5, baseFrequency);
	  //FloatArray sample6(bank_4106[0], SAMPLE_LEN*NOF_SAMPLES);	
	//wtf->makeMatrix(oscs[5], sample6, baseFrequency);
	
	  registerParameter(PARAMETER_A, "Frequency");
	  registerParameter(PARAMETER_B, "Amp");
	  registerParameter(PARAMETER_C, "Filter");
	  registerParameter(PARAMETER_D, "Q");
    
  }
  void processAudio(AudioBuffer &buffer) {
    float tune = getParameterValue(PARAMETER_A)*7.0 - 4.0;
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    hz.setTune(tune);
    float freqA = hz.getFrequency(left[0]);
    float morphC = getParameterValue(PARAMETER_C);  
    float morphD = getParameterValue(PARAMETER_D); 
    
    float morphIdx = morphD * NOF_OSC;	
    int intPart = morphIdx; 
    float fracPart = morphIdx - intPart;
    
    MorphOsc* oscDown = oscs[intPart];
    oscDown->setFrequency(freqA/sampleRate);
    oscDown->setMorphing(morphC);
    
    intPart++;
    if (intPart >= NOF_OSC) {
        intPart = 0 ;
	}

    MorphOsc* oscUp = oscs[intPart];
	oscUp->setFrequency(freqA/sampleRate);
    oscUp->setMorphing(morphC);		
    
    
    amp = getParameterValue(PARAMETER_B);
    
    for(int n = 0; n<buffer.getSize(); n++){
    
    float down = oscDown->getMorphOutput() * (1 - fracPart);
	oscDown->updatePhase(); 
    float up = oscUp->getMorphOutput() * (fracPart);
	oscUp->updatePhase();
	
	left[n] = (down + up ) * amp * 0.5;				
		
	}
	}
};
