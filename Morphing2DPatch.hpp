
#include "WaveFormOsc.h"
#include "WTMatrix.h"
//#include "OscSelector.h"

#include "AMEN_LOOreduced.h"

#define baseFrequency (20)  /* starting frequency of first table */  // c1 = 32.7 Hz

#define NOF_OSC 3

class Morphing2DPatch : public Patch {
private:
  WaveFormOsc *oscs[NOF_OSC];
//  WaveFormOsc *osc2;
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
  Morphing2DPatch() {																		

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
	  int numOsc = 3;
	  FloatArray sample(AL4samples[0], sizeof(AL4samples[0])/4*2/sizeof(float));
	
	int val1 = wtm->makeMatrix(oscs[0], sample, foobarlen, baseFrequency);
	int val3 = wtm->makeMatrix(oscs[2], sample, foobarlen, baseFrequency);
	int val2 = wtm->makeMatrix(oscs[1], sample, foobarlen, baseFrequency);
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
    
    int numOsc = 3;
    
    float note = getParameterValue(PARAMETER_A)*120.0 + 17.0;
    freqA = exp2f((note-69.0)/12.0)*440.0;
    float morphC = getParameterValue(PARAMETER_C);  
    //if (morphC >= 1)  {
		//morphC-- ; 
		//}
    float morphD = getParameterValue(PARAMETER_D); 
    
    float temp = morphD * numOsc;	
    int intPart = temp;
    
    oscs[intPart]->setFrequency(freqA/sampleRate);
    oscs[intPart]->setMorphing(morphC);
    
    intPart++;
    if (intPart >= numOsc) {
        intPart -= intPart ;
	}
    
    oscs[intPart]->setMorphing(morphC);		
	oscs[intPart]->setFrequency(freqA/sampleRate);
    
    debugMessage("coupure", 0.0, (float) oscs[0]->totalWaves);
    
    amp = getParameterValue(PARAMETER_B);
    FloatArray left = buffer.getSamples(LEFT_CHANNEL);
    //FloatArray right = buffer.getSamples(RIGHT_CHANNEL);
    
    for(int n = 0; n<buffer.getSize(); n++){
		
    float morphIdx = morphD * numOsc;	
    int intP = morphIdx;
    float fracP = morphIdx - intP;
    
    float down = oscs[intP]->getMorphOutput() * (1 - fracP);
	oscs[intP]->updatePhase(); 
	intP++;
	if (intP >= numOsc) {
        intP -= intP ;
	}
    float up = oscs[intP]->getMorphOutput() * (fracP);
	oscs[intP]->updatePhase();
	
	left[n] = (down + up ) * amp * 0.5;				
		//right[n] = (oscs[1]->getMorphOutput()) * amp * 0.5;
		
	}
	}
};
