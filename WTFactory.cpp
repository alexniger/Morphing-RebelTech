#include "WTFactory.h"

	
	
//float WTFactory::filtExt(float filtFreq, int len, int Order)	{
	
	//float filtwave[1][len];
	////extFilter->setExtFIR(filtFreq, 60);
    ////extFilter->setFc(filtFreq);
    ////extFilter->setOrder(Order);
    //for (int idx = 0; idx < len; idx++) {
        ////filtwave[0][idx] = extFilter->process(foobar[0][idx]);
        ////filtwave[0][idx] = foobar[0][idx];
	//}
	////return filtwave;
//}

int WTFactory::calccycles(int WTlen, int fulllen) {
	
	float numcyclestest = (float) fulllen / (float) WTlen ;
	float numcyclestemp = fulllen / WTlen ;
	
	if (WTlen == fulllen) {
		return 1;
	}
	else if (numcyclestest == numcyclestemp) { // is integer
		return (int) numcyclestemp;
	}
	else return 0;
	
}
	
    
int WTFactory::makeWaveTable(MorphOsc *osc, FloatArray sample, float baseFrequency, int WTlen, int WFid, int numcycles)     {
	
	

	
	ComplexFloatArray fft = ComplexFloatArray::create(sample.getSize());
	ComplexFloatArray tmp = ComplexFloatArray::create(sample.getSize());
	fft.clear();
	FloatArray dest = FloatArray::create(sample.getSize());
    dest.clear();
//	FloatArray magn = FloatArray::create(sample.getSize());
	FloatArray zeros = FloatArray::create(sample.getSize());
    zeros.clear();
    

	dest.copyFrom(sample);
	
	fourier->fft(dest, fft);
	int fftoffs = fft.getSize();
	
	int ret;
	

	float topFreq = baseFrequency * 16.0 /sampleRate;
	
		//fftoffs *= 1;
	

	for (int i=0; i<(this->numWaveTables); i++)  {
		fftoffs /= 2;
		// int fftoffs0 = fftoffs -1;
		fft.setMagnitude(zeros, fftoffs, (fft.getSize())-fftoffs);
		tmp.copyFrom(fft);
		fourier->ifft(tmp, dest);
		ret = osc->addWaveTable(dest.getSize(), dest.getData(), topFreq, WFid, numcycles);
		//ret = osc->addWaveTableB(dest.getSize(), dest.getData(), topFreq, WFid, numcycles);
		topFreq *= 2.0;
		
	}
	

	ComplexFloatArray::destroy(fft);
	ComplexFloatArray::destroy(tmp);
	FloatArray::destroy(dest);
	FloatArray::destroy(zeros);
	return 0;
}



int WTFactory::makeMatrix(MorphOsc *osc, FloatArray fullsample, int WTlen, float baseFrequency)     {
	
	int numWFs =  fullsample.getSize() / WTlen; // calccycles(WTlen, fullsample.getSize());
	FloatArray tempsample; // = FloatArray::create((fullsample.getSize())/numWFs);
	
	for (int WFid=0 ; WFid<numWFs ; WFid++) {
	tempsample = fullsample.subArray(WFid*WTlen, WTlen);
	makeWaveTable(osc, tempsample, baseFrequency, WTlen, WFid, numWFs);
	} 
    
	// FloatArray::destroy(tempsample);
	return 0;
}
