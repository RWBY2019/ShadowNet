#pragma once
#include "functions.h"
#include "NeutronBase.h"
//#include <Eigen/Dense>

////////////!!!!!cinnector ƫ�ػ�

//ȫ���Ӳ�����Ԫ
template <class FunctionType>
class FullConectedNeutron : public UpdatableWNeutron<double, double, double*>
{
private:
	FunctionType Function;
public:
	typedef FunctionType functiontype;

	//Weight ��ǰָ���
	double LearningRate;			//ѧϰ����
	double *RecievedFactor;			//����һ����յķ��򴫲����� connector
	double **NextThresold;			//��һ�����ֵָ�� connector
	int NextLayerNeutronNum;
	//int LastLayerNeutronNum;
	//double *LastWeight;	//��һ������ָ������Ȩֵ
	double *NextWeight;	//��һ����ָ�������Ȩֵ



	FullConectedNeutron() : LearningRate(0.01), RecievedFactor(0), NextLayerNeutronNum(0),
		NextWeight(0), NextThresold(0) {}
	~FullConectedNeutron() { if (NextWeight) delete [] NextWeight; }
	void InitThresold(double thresold) { Thresold = thresold; }
	void _Init(double learningrate, double *recievedfactor,	
			double **nextthresold, int nextlayerneutronnum);		//ÿһ��connector��Ҫ����Init
	void Update();
	void SetInput(double input);
	double GetBackForwardFactor();
	double GetOutput();
	double *GetPThresold();
};

//�������Ԫ
template <class FunctionType>
class OutputNeutron : UnupdatableNWNeutron<double, double>
{
private:
	FunctionType Function;
public:
	typedef FunctionType functiontype;

	double Expect;

	OutputNeutron() : Expect(0) {}
	~OutputNeutron() {}
	void InitThresold(double thresold) { Thresold = thresold; }
	void InitExpect(double expect) { Expect = expect; }
	double GetOutput();
	void SetInput(double input);
	double GetBackForwardFactor();		//y * 1-y * y-y
	double *GetPThresold();
};



/***********************************************************************/
/***********************************************************************/

/****************************FullConectedNeutron**********************************/

template <class FunctionType>
void FullConectedNeutron<FunctionType>::_Init(double learningrate, double *recievedfactor,
	double **nextthresold, int nextlayerneutronnum)
{
	LearningRate = learningrate;
	RecievedFactor = recievedfactor;
	NextThresold = nextthresold;
	NextLayerNeutronNum = nextlayerneutronnum;
	NextWeight = new double[nextlayerneutronnum];
	for(int i = 0; i < nextlayerneutronnum; ++i)
		NextWeight[i] = (2.0*(double)rand() / RAND_MAX) - 1;
}

template <class FunctionType>
double FullConectedNeutron<FunctionType>::GetBackForwardFactor()
{
	double d = Function.Derivative(Output);
	double factor = 0;
	for (int i = 0; i < NextLayerNeutronNum; ++i)
		factor += NextWeight[i] * RecievedFactor[i];
	return factor * d;	
}

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
	Output = Function.GetOutput(Input - Thresold);
	return Output;
}

template <class FunctionType>
void FullConectedNeutron<FunctionType>::SetInput(double input)
{
	Input = input;
}

template <class FunctionType>
double *FullConectedNeutron<FunctionType>::GetPThresold()
{
	return &Thresold;
}
/*****************************************************************/

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
	Output = Function.GetOutput(Input - Thresold);
	return Output;
}

template <class functiontype>
void OutputNeutron<functiontype>::SetInput(double input)
{
	Input = input;
}
