#ifndef VoroinTIN_H
#define VoroinTIN_H
#include "stdafx.h"
#include "stdlib.h"
#include "math.h"
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include "Edge.h"
#include "Vertex.h"
#include "Triangle.h"
#include "Node.h"
#include "TIN.h"
#include "ScaleVariable.h"
extern CArray <CurPos3D> T1;
using namespace std;



class VoroinTIN{
public:
	void InsertHeadNode(CNode *pNode);
	void InsertNode(CNode *pParentNode,int Flag,CNode *pChildNode);
	int FindNode(int i,CNode * pStart,CNode **pLeftTri,CNode **pRightTri,int &rEdge);
public:
	VoroinTIN();
	virtual ~VoroinTIN();
	void Clear();
	void Clear(CNode * pNode);
	void Init(int Width,int Height,int Thresdhold);
	//����������
	list<int> GenerateTin(int begin,int end);
	static int GetIndex(double Value,double * Base,int Size);
	//�жϵ��Ƿ���ֱ�ߵ��Ҳ�
	int PointIsRight(int Begin,int End,int Point);
	//�㵽ֱ�ߵľ���
	double Distance(int Point,int Begin,int End);
protected:
	//void Adjust(int &small,int &big);
	//�жϵ��Ƿ�������������
	int PointInTri(int Point,CNode * pNode,int &Edge);
	//�ֲ��Ż�
	void LOP(int begin,int end);
	bool LOP(int Index);
	void RecursiveLop(int Index);
	std::list<int> ConvexHull(int Begin,int End,int &BeginEdgeIndex,int &EndEdgeIndex);
public:
	//�������ĵ�
	std::vector<CVertex>   m_Vertexes;
	//���ɵı�
	std::vector<CEdge>     m_Edges;
	//������Ŀ��
	int m_Width;
	//������ĸ߶�
	int m_Height;
	//��ֵ
	int m_Threshold;
public:
	CNode * m_pHead;
	CNode * m_pTail;
};
#endif