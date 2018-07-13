
#include "MorphOsc.h"

#include "FastFourierTransform.h"
#include "Patch.h"




class WTFactory {
	
	private:

	FastFourierTransform *fourier; 
	
	//float filtExt(float filtFreq, int len, int Order);
	
	public:
	
	
    int numWaveTables;
	
	WTFactory() {
		numWaveTables = 7;
		int len = 256;
		fourier = new FastFourierTransform();
		fourier->init(len);
	}
	~WTFactory();
	
	int calccycles (int WTlen, int fulllen);
	int makeWaveTable(MorphOsc *osc, FloatArray sample, float baseFrequency, int WTlen, int WFid, int numcycles);
	int makeMatrix(MorphOsc *osc, FloatArray fullsample, int WTlen, float baseFrequency);
	}; 
