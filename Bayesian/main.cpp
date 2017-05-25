#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

//常量定义
const int ATTRIBUTE_NUM = 3;
const vector<int> CLASS_VALUES = { -1,1 };

float priPs[2] = { 0 }; //保存先验概率
float conPs[2][3][2] = { 0 };//保存条件概率
float thresholds[3] = { 0 };//保存区间划分阈值

//存放数据的结构体
typedef struct {
	float attrs[ATTRIBUTE_NUM];
	int label;
} SData;

//读取数据
void readData(vector<SData>& trainData, vector<SData>& testData);

//将数据离散化
void discretization(vector<SData>& data);

//计算先验概率和条件概率
void calProbability(const vector<SData>& trainData);

//根据属性值预测类别
int classify(const SData& _data);

//根据信息增益计算阈值
float CaculateCValue(int x, const vector<SData>& trainData, const vector<SData>& testData);

bool search(float list[], float x, int k);

int main() {
	srand(time(0));

	clock_t startTime = clock();
	vector<SData> trainData;
	vector<SData> testData;

	readData(trainData, testData);
	for (int i = 0; i < 3; ++i) {
		//thresholds[i] = CaculateCValue(i, trainData, testData);
		thresholds[i] = 0.5;
	}

	discretization(trainData);
	discretization(testData);

	vector<int> preds(testData.size());

	//计算先验概率和条件概率
	calProbability(trainData);

	for (int i = 0; i < testData.size(); ++i) {
		preds[i] = classify(testData[i]);
	}

	int cnt = 0;
	for (int i = 0; i < testData.size(); ++i) {
		if (testData[i].label == preds[i]) {
			cnt++;
		}
	}

	cout << "预测准确率为：" << (float)cnt / testData.size() << endl;
	clock_t endTime = clock();

	cout << "用时" << (float)(endTime - startTime) / CLOCKS_PER_SEC;
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

		if (rand() % 10 < 7) {
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
		for (int k = 0; k < 2; ++k) {
			if (trainData[i].label == CLASS_VALUES[k]) {
				cntPri[k]++;
				for (int j = 0; j < 3; ++j) {
					if (trainData[i].attrs[j] == 1)
						cntCon[k][j][1]++;
					else
						cntCon[k][j][0]++;
				}
				break;
			}
		}
	}
	for (int i = 0; i < 2; ++i) {
		priPs[i] = (float)cntPri[i] / trainData.size();
	}
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 2; ++k) {
				conPs[i][j][k] = (float)cntCon[i][j][k] / cntPri[i];
			}
		}
	}
}

int classify(const SData& _data) {
	float p[2] = { 0 };
	for (int i = 0; i < 2; ++i) {
		p[i] = priPs[i];
		for (int j = 0; j < 3; ++j) {
			if (_data.attrs[j] == 1)
				p[i] *= conPs[i][j][1];
			else
				p[i] *= conPs[i][j][0];
		}
	}
	if (p[0] > p[1]) {
		return CLASS_VALUES[0];
	}
	else {
		return CLASS_VALUES[1];
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

float CaculateCValue(int x, const vector<SData>& trainData, const vector<SData>& testData) {
	vector<SData> trainData_1;
	vector<SData> trainData_2;
	int count1 = 0, count2 = 0, isone1 = 0, isone2 = 0;
	float outcome[10];
	int choose = 0;

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

void discretization(vector<SData>& data) {
	for (int i = 0; i < data.size(); ++i) {
		for (int j = 0; j < 3; ++j) {
			if (data[i].attrs[j] > thresholds[j])
				data[i].attrs[j] = 1;
			else
				data[i].attrs[j] = 0;
		}
	}

}