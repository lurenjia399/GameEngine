#pragma once
#include <string>
#include <vector>
using namespace std;


// ����
struct FParamElement
{
	string Name;			//��������
	bool bConst;			//const������
	bool bPointer;			//ָ�������
	bool bReference;		//���ò�����
	string Type;			//��������

	FParamElement():Name("null"), bConst(false), bPointer(false), bReference(false), Type("null")
	{}
};

// ����
struct FVariableAnalysis : public FParamElement
{
	string CodeType; //�������ͣ�����ue����uproperty����д�Ĳ���
	bool bStatic; // ��̬������
	FVariableAnalysis() :CodeType(""), bStatic(false)
	{}
};

// ����
struct FFunctionAnalysis
{
	std::vector<FParamElement> ParamArray;//�����Ĳ���
	FParamElement ReturnParam;//�����ķ���ֵ

	string FunctionName;//��������
	bool bStatic;//��̬������
	bool bVirtual;//�麯����
	string CodeType;//�������ͣ�����ue����ufunction����д�Ĳ���

	FFunctionAnalysis(): ParamArray({}), ReturnParam({}), FunctionName(""), bStatic(false), bVirtual(false), CodeType("")
	{}

};

// ��
struct FClassAnalysis
{
	std::vector<FFunctionAnalysis> Functions;//����
	std::vector<FVariableAnalysis> Variable;//����

	string APIName;//api����
	string ClassName;//������

	std::vector<string> InheritName;//�̳�������
	int CodeLine;//���������һ�У�
};

// �ռ��Ĳ�������
enum class ECollectionParamType
{
	TYPE_PARAM,	// ��Ϊ����
	TYPE_RETURN, // ��Ϊ����ֵ
};