#pragma once

#include <vector>  
#include <iostream>

#define LAYER    3        //三层神经网络  
#define INPUT_NUM 3		  //定义输入个数
#define OUTPUT_NUM 1      //定义输出个数
#define HIDDEN_NUM 4	  //定义隐藏单元个数
#define NUM      10       //每层的最多节点数  

#define ITERS    100000     //最大迭代次数  
#define ETA   0.0001   //学习率  
#define ACCU     0.005    //每次迭代允许的误差  

#define Type double  

using namespace std;

struct Data
{
	vector<Type> x;       //输入数据  
	vector<Type> y;       //输出数据  
};

class BPNet {

public:
	BPNet();
	void Train(const vector<Data> _data);
	void Test(const vector<Data> _data);
	vector<Type> ForeCast(const vector<Type>);

private:

	void InitNetWork();         //初始化网络  
	void GetNums();             //获取输入、输出和隐含层节点数  
	void ForwardTransfer();     //正向传播子过程  
	void ReverseTransfer(int);  //逆向传播子过程  
	void CalcDelta(int);        //计算w和b的调整量  
	void UpdateNetWork();       //更新权值和阀值  
	Type GetError(int);         //计算单个样本的误差  
	Type GetAccu();             //计算所有样本的精度  
	Type Sigmoid(const Type);   //计算Sigmoid的值  

private:
	int in_num;                 //输入层节点数  
	int ou_num;                 //输出层节点数  
	int hd_num;                 //隐含层节点数  

	vector<Data> data;          //输入输出数据  

	Type w[LAYER][NUM][NUM];    //BP网络的权值  
	Type b[LAYER][NUM];         //BP网络节点的阀值  

	Type x[LAYER][NUM];         //每个神经元的值经S型函数转化后的输出值，输入层就为原值  
	Type d[LAYER][NUM];         //记录delta学习规则中delta的值  
};
