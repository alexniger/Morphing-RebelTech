
#include "OscSelectorOld.h"
#include "VoltsPerOctave.h"

#define baseFrequency (20)  /* starting frequency of first table */  // c1 = 32.7 Hz

#include "bank_4101536.h"
#include "wavetrip1536.h"
#include "synth_vo1536.h"
#include "alien_sp1536.h"
#include "additive1536.h"
#include "amen_loo1536.h"
#include "esq1_lo1536.h"

class DualMorphing2DOldPatch : public Patch {
	VoltsPerOctave hz;
private:
  OscSelectorOld *morph1;
  OscSelectorOld *morph2;
  
  SmoothFloat freqA;
  SmoothFloat amp;
  
public:
  DualMorphing2DOldPatch() {																		
    float* bankset1[NOF_OSC] = { bank_4106[0], wavetrip6[0], synth_vo6[0], alien_sp6[0], additive6[0], amen_loo6[0], esq1_lo6[0]};		//, amen_loo6[0] 														

	WTFactory *wtf = new WTFactory();

	morph1 = new OscSelectorOld();
	morph2 = new OscSelectorOld();
	for (int i ; i<NOF_OSC ; i++)  {
	morph1->setWaveTables(wtf, bankset1[i], baseFrequency, i);
	morph2->setWaveTables(wtf, bankset1[i], baseFrequency, i);
	}

	  registerParameter(PARAMETER_A, "Frequency");
	  registerParameter(PARAMETER_B, "Amp");
	  registerParameter(PARAMETER_C, "Filter");
	  registerParameter(PARAMETER_D, "Q");
    debugMessage("thing"  )
;  }
  void processAudio(AudioBuffer &buffer) {
    
    float tune = getParameterValue(PARAMETER_A)*7.0 - 4.0;
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
    hz.setTune(tune);
    float freqA = hz.getFrequency(left[0]);
    float morphC = getParameterValue(PARAMETER_C);  
    float morphD = getParameterValue(PARAMETER_D); 
    
    morph1->setMorphY(morphD);
    morph1->setFrequency(freqA/sampleRate);
    morph1->setMorphX(morphC);
    morph2->setMorphY(morphD);
    morph2->setFrequency(1.33484*freqA/sampleRate);
    morph2->setMorphX(morphC+0.04);
    amp = getParameterValue(PARAMETER_B);
    
    for(int n = 0; n<buffer.getSize(); n++){
	
	left[n] = morph1->get2DOutput() * amp * 0.25;
	right[n] = morph2->get2DOutput() * amp * 0.25;				
    morph1->updatePhases();				
    morph2->updatePhases();
		
	}
	}
};
