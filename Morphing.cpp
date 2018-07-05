#include "Morphing.h"



float Morphing::getMorphOutput(WaveTableOsc *pair , WaveTableOsc *odd) {
    // grab the appropriate extWF
    //int waveTableIdx = 0;
    //while ((this->morpheInc >= this->waveTables[waveTableIdx].topFreq) && (waveTableIdx < (this->numWaveTables - 1))) {
        //++waveTableIdx;
    //}
    //waveTable *waveTable = &this->waveTables[waveTableIdx];

    // linear interpolation
    float temp = (this->morphor);
    int intPart = temp;
    float fracPart = temp - intPart;
    float samp0;
    float samp1;
    if (intPart == 0) {
	samp0 = pair->getOutput() * (1.0-fracPart);
    samp1 = odd->getOutput() * fracPart;
	}
	else if (intPart == 1) {
    samp0 = pair->getOutput() * fracPart;
    samp1 = odd->getOutput() * (1.0-fracPart);
	}
	else {
	samp0 = pair->getOutput();
    samp1 = 0.0;
	}
    
    return samp0 + samp1;
}
