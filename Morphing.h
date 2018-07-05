#include "WaveTableOsc.h"
//#include "WaveFormExtFFT.h"


class Morphing {
	
	private:

	float morphor;
    float morphInc;
	float morphOfs;  // in order to put offset between two ext
	//FastFourierTransform *fourier; // = new FastFourierTransform();
	
	
	public:
	
	int numWaveForms;
	void setMorphing(double inc);
    void setMorphOffset(double offset);
    void updateMorph(void);
	
    float getMorphOutput(WaveTableOsc *pair , WaveTableOsc *odd);
	
	Morphing() {										
    morphor = 0.0;
    morphInc = 0.0;
    morphOfs = 0.5;
    numWaveForms = 2;
		//int len = 256;
		//fourier = new FastFourierTransform();
		//fourier->init(len);    
    //for (int idx = 0; idx < numWaveTableSlots; idx++) {    				
        //waveTables[idx].topFreq = 0;
        //waveTables[idx].waveTableLen = 0;
        //waveTables[idx].waveTable = 0;
    //}
}


	~Morphing() {										
		//for (int idx = 0; idx < numWaveTableSlots; idx++) {
			//float *temp = waveTables[idx].waveTable;
			//if (temp != 0)
				//delete [] temp;
    };
};


	
    
	 
	
	
// note: if you don't keep this in the range of 0-1, you'll need to make changes elsewhere
inline void Morphing::setMorphing(float inc) {
    morphInc = inc;
}



// note: if you don't keep this in the range of 0-1, you'll need to make changes elsewhere
inline void Morphing::setMorphOffset(float offset) {
    morphOfs = offset;
}



inline void Morphing::updateMorph() {
    morphor += morphInc;
    
    if (morphor >= 2.0)
        morphor -= 2.0;													// on conserve le décalage
}

