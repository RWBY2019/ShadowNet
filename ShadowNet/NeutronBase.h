#pragma once

class obj
{};

//������Ԫ�� Ҳ�� 
template <class InputType, class OutputType>
class Neutron : public obj
{
public:
	InputType	Input;
	OutputType	Output;
	InputType	Thresold;

	Neutron() : Input(0), Output(0), Thresold(0) {}
	virtual ~Neutron() {}
};

//��Ȩ���ɸ��� ����Ԫ
template <class InputType, class OutputType>
class UnupdatableNWNeutron : public Neutron<InputType, OutputType>
{
};


//��Ȩ���ɸ��µ���Ԫ
template <class InputType, class OutputType, class WeightType>
class UnupdatableWNeutron : public Neutron<InputType, OutputType>
{
public:
	UnupdatableWNeutron() {}
	virtual ~UnupdatableWNeutron() {}
};

//��Ȩ�ɸ��µ���Ԫ
//������ ��Ҫʵ��Ȩֵ���º���
template <class InputType, class OutputType, class WeightType>
class UpdatableWNeutron : public Neutron<InputType, OutputType>
{
public:
	UpdatableWNeutron() {}
	virtual ~UpdatableWNeutron() {}
};
