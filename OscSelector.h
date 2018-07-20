#include "WaveFormOsc.h"
#include "WTMatrix.h"

#define baseFrequency (20)  /* starting frequency of first table */  // c1 = 32.7 Hz
#define NOF_OSC 3

class OscSelector {
private:
	WaveFormOsc *oscs[NOF_OSC];
	WTMatrix *wtm;
    float morph1D;
    float morph2D;
    
public:

    OscSelector(void);
    ~OscSelector(void);
    int numOsc = NOF_OSC;
	for (int i = 0 ; i<NOF_OSC ; i++)  {
		oscs[i] = new WaveFormOsc();
	}
	wtm = new WTMatrix();	
	
    void setWaveTables(WTMatrix *wtm, int numOsc, FloatArray fullsample, int WTlen, float baseFrequency);
    
    void updatePhases(WaveFormOsc oscs, float morph2D, int numOsc);
	void setOscs(WaveFormOsc oscs, float freq, float morph1D, float morph2D, int numOsc);   
    float get2DOutput(WaveFormOsc oscs, float morph2D, int numOsc);
};
