#include <iostream>
#include <fstream>
#include <string>
#include "CParams.h"
#include "CNeuralNet.h"

using namespace std;

int main() {
	CParams params;
	if (!params.LoadInParameters("params.ini"))
	{
		cout << "Cannot find ini file!" << endl;
	}

	CNeuralNet neuralNet;

	return 0;
}