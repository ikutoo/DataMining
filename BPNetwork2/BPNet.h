#pragma once

#include <vector>  
#include <iostream>

#define LAYER    3        //����������  
#define INPUT_NUM 3		  //�����������
#define OUTPUT_NUM 1      //�����������
#define HIDDEN_NUM 4	  //�������ص�Ԫ����
#define NUM      10       //ÿ������ڵ���  

#define ITERS    100000     //����������  
#define ETA   0.0001   //ѧϰ��  
#define ACCU     0.005    //ÿ�ε�����������  

#define Type double  

using namespace std;

struct Data
{
	vector<Type> x;       //��������  
	vector<Type> y;       //�������  
};

class BPNet {

public:
	BPNet();
	void Train(const vector<Data> _data);
	void Test(const vector<Data> _data);
	vector<Type> ForeCast(const vector<Type>);

private:

	void InitNetWork();         //��ʼ������  
	void GetNums();             //��ȡ���롢�����������ڵ���  
	void ForwardTransfer();     //���򴫲��ӹ���  
	void ReverseTransfer(int);  //���򴫲��ӹ���  
	void CalcDelta(int);        //����w��b�ĵ�����  
	void UpdateNetWork();       //����Ȩֵ�ͷ�ֵ  
	Type GetError(int);         //���㵥�����������  
	Type GetAccu();             //�������������ľ���  
	Type Sigmoid(const Type);   //����Sigmoid��ֵ  

private:
	int in_num;                 //�����ڵ���  
	int ou_num;                 //�����ڵ���  
	int hd_num;                 //������ڵ���  

	vector<Data> data;          //�����������  

	Type w[LAYER][NUM][NUM];    //BP�����Ȩֵ  
	Type b[LAYER][NUM];         //BP����ڵ�ķ�ֵ  

	Type x[LAYER][NUM];         //ÿ����Ԫ��ֵ��S�ͺ���ת��������ֵ��������Ϊԭֵ  
	Type d[LAYER][NUM];         //��¼deltaѧϰ������delta��ֵ  
};
