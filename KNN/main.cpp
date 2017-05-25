#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

#define ATTRIBUTE_NUM 3
#define K 21

typedef struct {
	float attrs[ATTRIBUTE_NUM];
	int label;
} SData;

typedef struct {
	float distance;
	int label;
} SDistance;


void readData(vector<SData>& trainData, vector<SData>& testData);

float computeDistance(SData d1, SData d2);

bool cmp(SDistance d1, SDistance d2);

int main() {
	clock_t startTime = clock();
	vector<SData> trainData;
	vector<SData> testData;

	readData(trainData, testData);

	vector<int> preds(testData.size());

	for (int i = 0; i < testData.size(); ++i) {
		vector<SDistance> distances;
		for (int j = 0; j < trainData.size(); ++j) {
			SDistance d;
			d.distance = computeDistance(testData[i], trainData[j]);
			d.label = trainData[j].label;
			distances.push_back(d);
		}

		sort(distances.begin(), distances.end(), cmp);

		int positive = 0, negetive = 0;
		for (int i = 0; i < K; ++i) {
			if (distances[i].label == 1)
				positive++;
			else
				negetive++;
		}
		if (positive > negetive) {
			preds[i] = 1;
		}
		else {
			preds[i] = -1;
		}
	}

	int cnt = 0;
	for (int i = 0; i < testData.size(); ++i) {
		if (testData[i].label == preds[i]) {
			cnt++;
		}
	}

	cout << "预测准确率为：" << (float)cnt / testData.size() << endl;
	clock_t endTime = clock();

	cout << "用时" << (float)(endTime - startTime) / CLOCKS_PER_SEC << "秒" << endl;
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
		if (cnt % 10 < 7) {
			trainData.push_back(d);
		}
		else {
			testData.push_back(d);
		}
	}
}

float computeDistance(SData d1, SData d2) {
	float distance = 0;
	for (int i = 0; i < ATTRIBUTE_NUM; ++i) {
		distance += (d1.attrs[i] - d2.attrs[i])*(d1.attrs[i] - d2.attrs[i]);
	}
	distance /= ATTRIBUTE_NUM;
	return distance;
}

bool cmp(SDistance d1, SDistance d2) {
	return d1.distance < d2.distance;
}