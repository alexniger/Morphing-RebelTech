
#include "message.h"
#include "Patch.h"



#ifndef Test_WaveTableOsc_h
#define Test_WaveTableOsc_h


//#define c1 (32.7) , c4 (261) 

#define sampleRate (48000)

const int numWaveTableSlots = 230;

typedef struct {
    float topFreq;
    int waveTableLen;
    int waveformid;
    float *waveTable;
} waveTable;

class WaveFormOsc {
private:
    float phasor;      // phase accumulator
    float phaseInc;    // phase increment
    float phaseOfs;    // phase offset for PWM
    float morphor;
    float morphInc;
	float morphOfs;  // in order to put offset between two ext
    
    // list of wavetables
public:
    int totalWaves;
    waveTable WaveTables[numWaveTableSlots];
    int numWaveForms;
    int numWaveTables;

    WaveFormOsc(void);
    ~WaveFormOsc(void);
    void setFrequency(float pinc);
    void setPhaseOffset(float poffset);
    void updatePhase(void);
	void setMorphing(float minc);
    void setMorphOffset(float moffset);
    int addWaveTable(int len, float *waveTableIn, float topFreq, int WFid, int numcycles);
    
    float getMorphOutput();
	float getOutputAtIndex(int waveTableIdx);
    float getMorphOutputAtIndex(int oscIdx);
};


// note: if you don't keep this in the range of 0-1, you'll need to make changes elsewhere
inline void WaveFormOsc::setFrequency(float pinc) {
    phaseInc = pinc;
}



// note: if you don't keep this in the range of 0-1, you'll need to make changes elsewhere
inline void WaveFormOsc::setPhaseOffset(float poffset) {
    phaseOfs = poffset;
}



inline void WaveFormOsc::updatePhase() {
    phasor += phaseInc;
    
    if (phasor >= 1.0)
        phasor -= 1.0;													
}

// note: if you don't keep this in the range of 0-1, you'll need to make changes elsewhere
inline void WaveFormOsc::setMorphing(float mposition) {
    morphor = mposition;
    if (morphor >= 1.0)
    morphor -= 1.0;	
}


// note: if you don't keep this in the range of 0-1, you'll need to make changes elsewhere
inline void WaveFormOsc::setMorphOffset(float moffset) {
    morphOfs = moffset;
}


#endif
