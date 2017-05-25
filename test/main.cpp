#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <cmath>

using namespace std;

#define ATTRIBUTE_NUM 3
//#define K 21

typedef struct {
	float attrs[ATTRIBUTE_NUM];
	int label;
} SData;

typedef struct {
	float distance;
	int label;
} SDistance;


void readData(vector<SData>& trainData, vector<SData>& testData);

float CaculateCValue(int x);

bool search(float list[], float x, int k);


int main() {
	vector<SData> trainData;
	vector<SData> testData;
	float CValue;

	readData(trainData, testData);

	vector<int> preds(testData.size());
	CValue = CaculateCValue(3);
	cout << CValue << endl;
	for (int i = 0; i < trainData.size(); i++) {
		if (trainData[i].label = 1) {

		}
	}
}



float CaculateCValue(int x) {
	vector<SData> trainData;
	vector<SData> testData;
	vector<SData> trainData_1;
	vector<SData> trainData_2;
	int count1 = 0, count2 = 0, isone1 = 0, isone2 = 0;
	float outcome[10];
	int choose = 0;
	readData(trainData, testData);

	float y[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int k = 1;
	y[0] = trainData[1].attrs[x];
	for (int i = 1; i < trainData.size(); i++) {
		if (search(y, trainData[i].attrs[x], k)) {
			;
		}
		else
		{
			y[k] = trainData[i].attrs[x];
			k = k++;
		}
	}

	for (int i = 0; i < k; i++) {
		for (int j = 0; j < trainData.size(); j++) {
			if (trainData[j].attrs[x] < y[i]) {
				trainData_1.push_back(trainData[j]);
				count1++;
			}
			else {
				trainData_2.push_back(trainData[j]);
				count2++;
			}
		}
		for (int n = 0; n < count1; n++) {
			if (trainData_1[n].label == 1) {
				isone1++;
			}
		}
		for (int n = 0; n < count2; n++) {
			if (trainData_2[n].label == 1) {
				isone2++;
			}
		}
		outcome[i] = (count1 / trainData.size())*(-log(isone1 / (count1 + 1)) - log((count1 - isone1) / (count1 + 1)))
			+ (count2 / trainData.size())*(-log(isone2 / count2 + 1) - log((count2 - isone2) / count2 + 1));

	}
	for (int i = 1; i < k; i++) {
		if (outcome[i] < outcome[i - 1]) {
			choose = i;
		}
	}
	return y[choose];
}

void readData(vector<SData>& trainData, vector<SData>& testData) {
	string filePath = "titanic.csv";
	ifstream inFile(filePath);
	if (!inFile) {
		cout << "文件打开失败！" << endl;
		exit(1);
	}
	string line;
	for (int i = 0; i < 8; ++i) {
		getline(inFile, line);
	}

	const char* split = ",";
	int cnt = 0;
	while (getline(inFile, line)) {
		char *p = strtok((char*)line.c_str(), split);
		int i = 0;
		SData d;
		while (p && i < 3) {
			d.attrs[i] = atof(p);
			p = strtok(NULL, split);
			i++;
		}
		d.label = atoi(p);

		//规范化数据
		d.attrs[0] = (d.attrs[0] + 1.87) / (0.956 - (-1.87));
		d.attrs[1] = (d.attrs[1] + 0.228) / (4.38 - (-0.228));
		d.attrs[2] = (d.attrs[2] + 1.92) / (0.521 - (-1.92));

		cnt++;
		if (rand() % 10 < 7) {
			trainData.push_back(d);
		}
		else {
			testData.push_back(d);
		}
	}
}

bool search(float list[], float x, int k) {
	for (int i = 0; i < k; i++) {
		if (list[i] == x) {
			return true;
		}
	}
	return false;
}
