#include <iostream>  
#include <string.h>  
#include <stdio.h>  
#include <fstream>
#include <string>
#include <time.h>

#include "BPNet.h"  

using namespace std;

void readData(vector<Data>& _data);

int main()
{
	srand(time(0));

	vector<Data> data;
	//读取数据
	readData(data);

	//划分训练集、测试集
	vector<Data> trainData, testData;
	int i = 0;
	for (vector<Data>::iterator it = data.begin(); it != data.end(); ++it, ++i) {
		if ((i % 10) < 7)
			trainData.push_back(*it);
		else
			testData.push_back(*it);
	}

	BPNet *bpNet = new BPNet();
	//bpNet->Train(trainData);
	bpNet->Train2(trainData);
	bpNet->Test(testData);

	return 0;
}

void readData(vector<Data>& _data) {
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
	while (getline(inFile, line)) {
		char *p = strtok((char*)line.c_str(), split);
		int i = 0;
		Data d;
		while (p && i < 3) {
			d.x.push_back(atof(p));
			p = strtok(NULL, split);
			i++;
		}
		double label = atof(p);
		if (label == -1)
			label = 0;
		else if (label == 1)
			label = 1;
		d.y.push_back(label);
		_data.push_back(d);
	}
}