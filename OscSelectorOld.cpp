#include "OscSelectorOld.h"

OscSelectorOld::OscSelectorOld(void) {										
   for (int i = 0 ; i<NOF_OSC ; i++)  {
		oscs[i] = new MorphOsc();
	}	
	down = oscs[0];
	up = oscs[0];
	mix = 0;
}


OscSelectorOld::~OscSelectorOld(void) {
}



void OscSelectorOld::setWaveTables(WTFactory *wtf, float* banks, float baseFrequency, int Idx)  {   				
    //for (int i=0 ; i<NOF_OSC ; i++)  {
		FloatArray bank(banks, SAMPLE_LEN*NOF_SAMPLES);
		wtf->makeMatrix(oscs[Idx], bank, baseFrequency);
    //}
    
}

void OscSelectorOld::setMorphY(float morphY){
	float temp = morphY * NOF_OSC;
    int intPart = temp;
    mix = temp - intPart;
    
    down = oscs[intPart];
    intPart++;
	if (intPart >= NOF_OSC) {
        intPart = 0 ;
	}
	up = oscs[intPart];
}

//
// getOutput
//
// returns 2 oscillators outputs
//
float OscSelectorOld::get2DOutput() {
   
    float downValue = down -> getMorphOutput() * (1 - mix);

    float upValue = up -> getMorphOutput() * (mix);
    return downValue + upValue;

}


void OscSelectorOld::setFrequency(float freq)  {
   
   down -> setFrequency(freq);
   up -> setFrequency(freq);  
    
}

void OscSelectorOld::setMorphX(float morphX)  {
   
   down -> setMorphing(morphX);
   up -> setMorphing(morphX);   
    
}

void OscSelectorOld::updatePhases()  {
	
	down -> updatePhase();
    up -> updatePhase();
    
}
