#include <string.h>  
#include <stdio.h>  
#include <math.h>  
#include <assert.h>  
#include "Utils.h"
#include "BPNet.h"  

BPNet::BPNet() {
	InitNetWork();
}

//��ʼ����ѵ��  
void BPNet::Train(const vector<Data> _data)
{
	data = _data;
	int num = data.size();

	cout << "��ʼѵ��..." << endl;
	for (int iter = 0; iter <= ITERS; iter++)
	{
		for (int cnt = 0; cnt < num; cnt++)
		{
			//��һ������ڵ㸳ֵ  
			for (int i = 0; i < in_num; i++)
				x[0][i] = data.at(cnt).x[i];
			ForwardTransfer();
			ReverseTransfer(cnt);
		}
		cout << "��" << iter << "�ε�����";

		Type accu = GetAccu();
		cout << "�������Ϊ��" << accu << endl;
		if (accu < ACCU) break;
	}
	cout << "����ѵ��..." << endl;
}

//��ʼ����ѵ��-����ݶ��½�
void BPNet::Train2(const vector<Data> _data)
{
	data = _data;
	int num = data.size();

	cout << "��ʼѵ��..." << endl;
	for (int iter = 0; iter <= ITERS; iter++)
	{
		int cnt = rand() % in_num;

		//��һ������ڵ㸳ֵ  
		for (int i = 0; i < in_num; i++)
			x[0][i] = data.at(cnt).x[i];
		ForwardTransfer();
		ReverseTransfer(cnt);

		cout << "��" << iter << "�ε�����";

		Type accu = GetAccu();
		cout << "�������Ϊ��" << accu << endl;
		if (accu < ACCU) break;
	}
	cout << "����ѵ��..." << endl;
}

//����ѵ���õ�������Ԥ�����ֵ  
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

//��ȡ����ڵ���  
void BPNet::GetNums()
{
	in_num = INPUT_NUM;               //��ȡ�����ڵ���  
	ou_num = OUTPUT_NUM;               //��ȡ�����ڵ���  
	hd_num = HIDDEN_NUM;            //��ȡ������ڵ���  
}

//��ʼ������  
void BPNet::InitNetWork()
{
	//��ȡ����ڵ���  
	GetNums();
	//��ʼ��Ȩֵ�ͷ�ֵ
	for (int i = 0; i < LAYER; ++i) {
		for (int j = 0; j < NUM; ++j) {
			b[i][j] = RandomClamped();
			for (int k = 0; k < NUM; ++k)
				w[i][j][k] = RandomClamped();
		}
	}
}

//�����ź����򴫵��ӹ���  
void BPNet::ForwardTransfer()
{
	//��������������ڵ�����ֵ  
	for (int j = 0; j < hd_num; j++)
	{
		Type t = 0;
		for (int i = 0; i < in_num; i++)
			t += w[1][i][j] * x[0][i];
		t += b[1][j];
		x[1][j] = Sigmoid(t);
	}

	//�����������ڵ�����ֵ  
	for (int j = 0; j < ou_num; j++)
	{
		Type t = 0;
		for (int i = 0; i < hd_num; i++)
			t += w[2][i][j] * x[1][i];
		t += b[2][j];
		x[2][j] = Sigmoid(t);
	}
}

//���㵥�����������  
Type BPNet::GetError(int cnt)
{
	Type ans = 0;
	for (int i = 0; i < ou_num; i++)
		ans += 0.5 * (x[2][i] - data.at(cnt).y[i]) * (x[2][i] - data.at(cnt).y[i]);
	return ans;
}

//����źŷ��򴫵��ӹ���  
void BPNet::ReverseTransfer(int cnt)
{
	CalcDelta(cnt);
	UpdateNetWork();
}

//�������������ľ���  
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

//���������  
void BPNet::CalcDelta(int cnt)
{
	//����������deltaֵ  
	for (int i = 0; i < ou_num; i++)
		d[2][i] = (x[2][i] - data.at(cnt).y[i]) * x[2][i] * (1 - x[2][i]);
	//�����������deltaֵ  
	for (int i = 0; i < hd_num; i++)
	{
		Type t = 0;
		for (int j = 0; j < ou_num; j++)
			t += w[2][i][j] * d[2][j];
		d[1][i] = t * x[1][i] * (1 - x[1][i]);
	}
}

//���ݼ�����ĵ�������BP������е���  
void BPNet::UpdateNetWork()
{
	//������������֮��Ȩֵ�ͷ�ֵ����  
	for (int i = 0; i < hd_num; i++)
	{
		for (int j = 0; j < ou_num; j++)
			w[2][i][j] -= ETA * d[2][j] * x[1][i];
	}
	for (int i = 0; i < ou_num; i++)
		b[2][i] -= ETA * d[2][i];

	//������������֮��Ȩֵ�ͷ�ֵ����  
	for (int i = 0; i < in_num; i++)
	{
		for (int j = 0; j < hd_num; j++)
			w[1][i][j] -= ETA * d[1][j] * x[0][i];
	}
	for (int i = 0; i < hd_num; i++)
		b[1][i] -= ETA * d[1][i];
}

//����Sigmoid������ֵ  
Type BPNet::Sigmoid(const Type x)
{
	return 1 / (1 + exp(-x));
}

//��ʼ���в���
void BPNet::Test(const vector<Data> _data)
{
	int num = _data.size();

	cout << "��ʼ����..." << endl;
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
	cout << "׼ȷ��Ϊ��" << accu << endl;
	cout << "��������..." << endl;
}