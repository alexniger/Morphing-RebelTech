

#include "WaveFormOsc.h"

#include "FastFourierTransform.h"
#include "Patch.h"




class WTMatrix {
	
	private:

	FastFourierTransform *fourier; 
	
	//float filtExt(float filtFreq, int len, int Order);
	
	public:
	
	
    int numWaveTables;
    waveTable waveTables[numWaveTableSlots];
	
	WTMatrix() {
		numWaveTables = 7;
		int len = 256;
		fourier = new FastFourierTransform();
		fourier->init(len);
	}
	~WTMatrix();
	
	int calccycles (int WTlen, int fulllen);
	int makeWaveTable(WaveFormOsc *osc, FloatArray sample, float baseFrequency, int WTlen, int WFid, int numcycles);
	int makeMatrix(WaveFormOsc *osc, FloatArray fullsample, int WTlen, float baseFrequency);
	}; 
