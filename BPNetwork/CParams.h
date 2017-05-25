#ifndef CPARAMS_H
#define CPARAMS_H

#include <fstream>

using namespace std;

class CParams
{

public:

	//------------------------------------general parameters
	static double dPi;
	static double dHalfPi;
	static double dTwoPi;
	//-------------------------------------used for the neural network
	static int    iNumInputs;
	static int    iNumHidden;
	static int    iNeuronsPerHiddenLayer;
	static int    iNumOutputs;

	//for tweeking the sigmoid function
	static double dActivationResponse;
	//bias value
	static double dBias;

	//ctor
	CParams()
	{

	}

	bool LoadInParameters(char* szFileName);
};


#endif