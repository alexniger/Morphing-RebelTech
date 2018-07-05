
#include "WaveTableOsc.h"

#include "FastFourierTransform.h"
#include "Patch.h"



class WaveFormExtFFT {
	
	private:
	//WAVESHAPE_TYPE wavetype;

	FastFourierTransform *fourier; // = new FastFourierTransform();
	
	float filtExt(float filtFreq, int len, int Order);
	
	public:
	//WaveForm(WAVESHAPE_TYPE TRI_WAVE); //type);
	//~WaveForm();
	
	WaveFormExtFFT() {
		int len = 256;
		fourier = new FastFourierTransform();
		fourier->init(len);
	}
	~WaveFormExtFFT();
	int makeWaveTableExt(FloatArray sample, WaveTableOsc *ext, float baseFrequency,int WTlen, int M);
	
	}; 
