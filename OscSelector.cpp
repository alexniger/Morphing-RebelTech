#include "OscSelector.h"

OscSelector::OscSelector(void) {										// initialisation
    morph1D = 0.0;
    morph2D = 0.0;
}


OscSelector::~OscSelector(void) {
}



void OscSelector::setWaveTables(WTMatrix *wtm, int numOsc, FloatArray fullsample, int WTlen, float baseFrequency)  {   				
    	
    for (int i=0 ; i<numOsc ; i++)  {
		wtm->makeMatrix(oscs[i], sample, foobarlen, baseFrequency);
    }
    return 0;
}

//
// getOutput
//
// returns 2 oscillators outputs
//
float OscSelector::get2DOutput(WaveFormOsc oscs, float morph2D, int numOsc) {
	
    waveTable *waveTable = &this->WaveTables[waveTableIdx];

    float temp = morph2D * numOsc;
    int intPart = temp;
    float fracPart = temp - intPart;
    
    float down = oscs[intPart] -> getMorphOutput() * (1 - fracPart);
	if (++intPart >= numOsc) {
        intPart -= intPart ;
	}
	else intPart++;
    float up = oscs[intPart] -> getMorphOutput() * (fracPart);
    return down + up;

}


void OscSelector::setOscs(WaveFormOsc oscs, float freq, float morph1D, float morph2D, int numOsc)  {
	
	float temp = morph2D * numOsc;	
    int intPart = temp;
    
    oscs [intPart] -> setFrequency(freq);
    oscs [intPart] -> setMorphing(morph1D);
    
    if (++intPart >= numOsc) {
        intPart -= intPart ;
	}
	else intPart++;
	
    oscs [intPart] -> setFrequency(freq);
    oscs [intPart] -> setMorphing(morph1D);   
    
}

void updatePhases(WaveFormOsc oscs, float morph2D, int numOsc)  {
	
	float temp = morph2D * numOsc;	
    int intPart = temp;
    
    oscs [intPart] -> updatePhase();
    
	if (++intPart >= numOsc) {
        intPart -= intPart ;
	}
	else intPart++;

    oscs [intPart] -> updatePhase();
    
}
