#pragma once
#include "functions.h"
#include "NeutronBase.h"
//#include <Eigen/Dense>


//全连接层类神经元
template <class FunctionType>
class FullConectedNeutron : public UpdatableWNeutron<double, double, double*>
{
private:
	FunctionType Function;
public:
	typedef FunctionType functiontype;

	//Weight 由前指向后
	double LearningRate;			//学习速率
	double *RecievedFactor;			//从下一层接收的反向传播因子 connector
	double **NextThresold;			//下一层的阈值指针 connector
	int NextLayerNeutronNum;
	double *NextWeight;				//下一层它指向的所有权值
	bool isInputSet;



	FullConectedNeutron() : LearningRate(0.01), RecievedFactor(0), NextLayerNeutronNum(0),
		NextWeight(0), NextThresold(0), isInputSet(false) {}
	~FullConectedNeutron() { if (NextWeight) delete [] NextWeight; }
	void InitThresold(double thresold) { Thresold = thresold; }
	void _Init(double learningrate, double *recievedfactor,	
			double **nextthresold, int nextlayerneutronnum);		//每一个connector需要调用Init
	void Update();
	void SetInput(double input);
	double GetBackForwardFactor();
	double GetOutput();
	double *GetPThresold();
};

//输出层神经元
template <class FunctionType>
class OutputNeutron : UnupdatableNWNeutron<double, double>
{
private:
	FunctionType Function;
public:
	typedef FunctionType functiontype;

	double Expect;
	bool isInputSet;

	OutputNeutron() : Expect(0), isInputSet(false) {}
	~OutputNeutron() {}
	void InitThresold(double thresold) { Thresold = thresold; }
	void InitExpect(double expect) { Expect = expect; }
	double GetOutput();
	void SetInput(double input);
	double GetBackForwardFactor();		//y * 1-y * y-y
	double *GetPThresold();
};


/****************************FullConectedNeutron**********************************/

//全连接层神经元
template <class FunctionType>
void FullConectedNeutron<FunctionType>::_Init(double learningrate, double *recievedfactor,
	double **nextthresold, int nextlayerneutronnum)
{
	isInputSet = false;
	LearningRate = learningrate;
	RecievedFactor = recievedfactor;
	NextThresold = nextthresold;
	NextLayerNeutronNum = nextlayerneutronnum;
	NextWeight = new double[nextlayerneutronnum];
	for(int i = 0; i < nextlayerneutronnum; ++i)
		NextWeight[i] = (2.0*(double)rand() / RAND_MAX) - 1;	//关键的随机初始化
}

//获取反向误差传播因子
template <class FunctionType>
double FullConectedNeutron<FunctionType>::GetBackForwardFactor()
{
	double d = Function.Derivative(Output);
	double factor = 0;
	for (int i = 0; i < NextLayerNeutronNum; ++i)
		factor += NextWeight[i] * RecievedFactor[i];
	return factor * d;	
}

//通过返回的传播因子更新
template <class FunctionType>
void FullConectedNeutron<FunctionType>::Update()
{
	for (int i = 0; i < NextLayerNeutronNum; ++i)
	{
		NextWeight[i] += LearningRate * RecievedFactor[i] * Output;
		*NextThresold[i] -= LearningRate * RecievedFactor[i];
	}
}

template <class FunctionType>
double FullConectedNeutron<FunctionType>::GetOutput()
{
	if (isInputSet)
	{
		Output = Function.GetOutput(Input - Thresold);
		isInputSet = false;
	}
	return Output;
}

template <class FunctionType>
void FullConectedNeutron<FunctionType>::SetInput(double input)
{
	Input = input;
	isInputSet = true;
}

template <class FunctionType>
double *FullConectedNeutron<FunctionType>::GetPThresold()
{
	return &Thresold;
}
/************************OutputNeutron*****************************************/

//输出层神经元 
template <class functiontype>
double OutputNeutron<functiontype>::GetBackForwardFactor()
{
	double d = Function.Derivative(Output);
	double factor = d * (Expect - Output);
	return factor;
}

template <class functiontype>
double *OutputNeutron<functiontype>::GetPThresold()
{
	return &Thresold;
}

template <class functiontype>
double OutputNeutron<functiontype>::GetOutput()
{
	if (isInputSet)
	{
		Output = Function.GetOutput(Input - Thresold);
		isInputSet = false;
	}
	return Output;
}

template <class functiontype>
void OutputNeutron<functiontype>::SetInput(double input)
{
	Input = input;
	isInputSet = true;
}
