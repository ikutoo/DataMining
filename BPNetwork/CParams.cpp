#include "CParams.h"

//because we will always be loading in the settings from an ini file
//we can just initialize everything to zero
double CParams::dPi = 3.14159265358979;
double CParams::dHalfPi = dPi / 2;
double CParams::dTwoPi = dPi * 2;
int CParams::iNumInputs = 0;
int CParams::iNumHidden = 0;
int CParams::iNeuronsPerHiddenLayer = 0;
int CParams::iNumOutputs = 0;
double CParams::dActivationResponse = 0;
double CParams::dBias = 0;

//this function loads in the parameters from a given file name. Returns
//false if there is a problem opening the file.
bool CParams::LoadInParameters(char* szFileName)
{
	ifstream grab(szFileName);

	//check file exists
	if (!grab)
	{
		return false;
	}

	//load in from the file
	char ParamDescription[40];

	grab >> ParamDescription;
	grab >> iNumInputs;
	grab >> ParamDescription;
	grab >> iNumHidden;
	grab >> ParamDescription;
	grab >> iNeuronsPerHiddenLayer;
	grab >> ParamDescription;
	grab >> iNumOutputs;
	grab >> ParamDescription;
	grab >> dActivationResponse;
	grab >> ParamDescription;
	grab >> dBias;

	return true;
}







