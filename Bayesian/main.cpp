#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

typedef unsigned char uint_8;

const int ATTRIBUTE_NUM = 3;
const vector<int> CLASS_VALUES = { -1,1 };

float priPs[2] = { 0 };
float conPs[2][3][2] = { 0 };
float thresholds[3] = { 0.5,0.5,0.5 };

typedef struct {
	bool attrs[ATTRIBUTE_NUM];
	int label;
} SData;

typedef struct {
	float distance;
	int label;
} SDistance;


void readData(vector<SData>& trainData, vector<SData>& testData);

void calProbability(const vector<SData>& trainData);

int main() {
	vector<SData> trainData;
	vector<SData> testData;

	readData(trainData, testData);

	vector<int> preds(testData.size());

	//计算先验概率和条件概率
	calProbability(trainData);

	int cnt = 0;
	for (int i = 0; i < testData.size(); ++i) {
		if (testData[i].label == preds[i]) {
			cnt++;
		}
	}

	cout << "预测准确率为：" << (float)cnt / testData.size() << endl;

	return 0;
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
	float vals[ATTRIBUTE_NUM];
	while (getline(inFile, line)) {
		char *p = strtok((char*)line.c_str(), split);
		int i = 0;
		SData d;
		while (p && i < 3) {
			vals[i] = atof(p);
			p = strtok(NULL, split);
			i++;
		}
		d.label = atoi(p);

		//规范化数据
		vals[0] = (vals[0] + 1.87) / (0.956 - (-1.87));
		vals[1] = (vals[1] + 0.228) / (4.38 - (-0.228));
		vals[2] = (vals[2] + 1.92) / (0.521 - (-1.92));

		//将数据离散化
		for (int i = 0; i < ATTRIBUTE_NUM; ++i) {
			if (vals[i] > thresholds[i])
				d.attrs[i] = true;
			else
				d.attrs[i] = false;
		}

		cnt++;
		if (cnt % 10 < 7) {
			trainData.push_back(d);
		}
		else {
			testData.push_back(d);
		}
	}
}

void calProbability(const vector<SData>& trainData) {
	int cntPri[2] = { 0 };
	int cntCon[2][3][2] = { 0 };
	for (int i = 0; i < trainData.size(); ++i) {
		if (trainData[i].label == CLASS_VALUES[0]) {
			cntPri[0]++;
			for (int j = 0; j < 3; ++j) {
				
			}
		}
		else {
			cntPri[1]++;
		}
	}
	for (int i = 0; i < 2; ++i) {
		priPs[i] = (float)cntPri[i] / trainData.size();
	}
}