#include <string.h>  
#include <stdio.h>  
#include <math.h>  
#include <assert.h>  
#include "Utils.h"
#include "BPNet.h"  

BPNet::BPNet() {
	InitNetWork();
}

//开始进行训练  
void BPNet::Train(const vector<Data> _data)
{
	data = _data;
	int num = data.size();

	cout << "开始训练..." << endl;
	for (int iter = 0; iter <= ITERS; iter++)
	{
		for (int cnt = 0; cnt < num; cnt++)
		{
			//第一层输入节点赋值  
			for (int i = 0; i < in_num; i++)
				x[0][i] = data.at(cnt).x[i];
			ForwardTransfer();
			ReverseTransfer(cnt);
		}
		cout << "第" << iter << "次迭代：";

		Type accu = GetAccu();
		cout << "样本误差为：" << accu << endl;
		if (accu < ACCU) break;
	}
	cout << "结束训练..." << endl;
}

//开始进行训练-随机梯度下降
void BPNet::Train2(const vector<Data> _data)
{
	data = _data;
	int num = data.size();

	cout << "开始训练..." << endl;
	for (int iter = 0; iter <= ITERS; iter++)
	{
		int cnt = rand() % in_num;

		//第一层输入节点赋值  
		for (int i = 0; i < in_num; i++)
			x[0][i] = data.at(cnt).x[i];
		ForwardTransfer();
		ReverseTransfer(cnt);

		cout << "第" << iter << "次迭代：";

		Type accu = GetAccu();
		cout << "样本误差为：" << accu << endl;
		if (accu < ACCU) break;
	}
	cout << "结束训练..." << endl;
}

//根据训练好的网络来预测输出值  
vector<Type> BPNet::ForeCast(const vector<Type> data)
{
	int n = data.size();
	assert(n == in_num);
	for (int i = 0; i < in_num; i++)
		x[0][i] = data[i];

	ForwardTransfer();
	vector<Type> v;
	for (int i = 0; i < ou_num; i++)
		v.push_back(x[2][i]);
	return v;
}

//获取网络节点数  
void BPNet::GetNums()
{
	in_num = INPUT_NUM;               //获取输入层节点数  
	ou_num = OUTPUT_NUM;               //获取输出层节点数  
	hd_num = HIDDEN_NUM;            //获取隐含层节点数  
}

//初始化网络  
void BPNet::InitNetWork()
{
	//获取网络节点数  
	GetNums();
	//初始化权值和阀值
	for (int i = 0; i < LAYER; ++i) {
		for (int j = 0; j < NUM; ++j) {
			b[i][j] = RandomClamped();
			for (int k = 0; k < NUM; ++k)
				w[i][j][k] = RandomClamped();
		}
	}
}

//工作信号正向传递子过程  
void BPNet::ForwardTransfer()
{
	//计算隐含层各个节点的输出值  
	for (int j = 0; j < hd_num; j++)
	{
		Type t = 0;
		for (int i = 0; i < in_num; i++)
			t += w[1][i][j] * x[0][i];
		t += b[1][j];
		x[1][j] = Sigmoid(t);
	}

	//计算输出层各节点的输出值  
	for (int j = 0; j < ou_num; j++)
	{
		Type t = 0;
		for (int i = 0; i < hd_num; i++)
			t += w[2][i][j] * x[1][i];
		t += b[2][j];
		x[2][j] = Sigmoid(t);
	}
}

//计算单个样本的误差  
Type BPNet::GetError(int cnt)
{
	Type ans = 0;
	for (int i = 0; i < ou_num; i++)
		ans += 0.5 * (x[2][i] - data.at(cnt).y[i]) * (x[2][i] - data.at(cnt).y[i]);
	return ans;
}

//误差信号反向传递子过程  
void BPNet::ReverseTransfer(int cnt)
{
	CalcDelta(cnt);
	UpdateNetWork();
}

//计算所有样本的精度  
Type BPNet::GetAccu()
{
	Type ans = 0;
	int num = data.size();
	for (int i = 0; i < num; i++)
	{
		int m = data.at(i).x.size();
		for (int j = 0; j < m; j++)
			x[0][j] = data.at(i).x[j];
		ForwardTransfer();
		int n = data.at(i).y.size();
		for (int j = 0; j < n; j++)
			ans += 0.5 * (x[2][j] - data.at(i).y[j]) * (x[2][j] - data.at(i).y[j]);
	}
	return ans / num;
}

//计算调整量  
void BPNet::CalcDelta(int cnt)
{
	//计算输出层的delta值  
	for (int i = 0; i < ou_num; i++)
		d[2][i] = (x[2][i] - data.at(cnt).y[i]) * x[2][i] * (1 - x[2][i]);
	//计算隐含层的delta值  
	for (int i = 0; i < hd_num; i++)
	{
		Type t = 0;
		for (int j = 0; j < ou_num; j++)
			t += w[2][i][j] * d[2][j];
		d[1][i] = t * x[1][i] * (1 - x[1][i]);
	}
}

//根据计算出的调整量对BP网络进行调整  
void BPNet::UpdateNetWork()
{
	//隐含层和输出层之间权值和阀值调整  
	for (int i = 0; i < hd_num; i++)
	{
		for (int j = 0; j < ou_num; j++)
			w[2][i][j] -= ETA * d[2][j] * x[1][i];
	}
	for (int i = 0; i < ou_num; i++)
		b[2][i] -= ETA * d[2][i];

	//输入层和隐含层之间权值和阀值调整  
	for (int i = 0; i < in_num; i++)
	{
		for (int j = 0; j < hd_num; j++)
			w[1][i][j] -= ETA * d[1][j] * x[0][i];
	}
	for (int i = 0; i < hd_num; i++)
		b[1][i] -= ETA * d[1][i];
}

//计算Sigmoid函数的值  
Type BPNet::Sigmoid(const Type x)
{
	return 1 / (1 + exp(-x));
}

//开始进行测试
void BPNet::Test(const vector<Data> _data)
{
	int num = _data.size();

	cout << "开始测试..." << endl;
	int posi = 0, nege = 0;
	for (int i = 0; i < num; ++i) {
		double c;
		vector<Type> output = ForeCast(_data.at(i).x);
		if (output[0] > 0.5)
			c = 1;
		else
			c = 0;
		if (_data[i].y[0] == c)
			posi++;
		else
			nege++;
	}
	double accu = (double)posi / num;
	cout << "准确率为：" << accu << endl;
	cout << "结束测试..." << endl;
}