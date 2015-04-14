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
	//生成三角网
	list<int> GenerateTin(int begin,int end);
	static int GetIndex(double Value,double * Base,int Size);
	//判断点是否在直线的右侧
	int PointIsRight(int Begin,int End,int Point);
	//点到直线的距离
	double Distance(int Point,int Begin,int End);
protected:
	//void Adjust(int &small,int &big);
	//判断点是否在三角形里面
	int PointInTri(int Point,CNode * pNode,int &Edge);
	//局部优化
	void LOP(int begin,int end);
	bool LOP(int Index);
	void RecursiveLop(int Index);
	std::list<int> ConvexHull(int Begin,int End,int &BeginEdgeIndex,int &EndEdgeIndex);
public:
	//三角网的点
	std::vector<CVertex>   m_Vertexes;
	//生成的边
	std::vector<CEdge>     m_Edges;
	//点区域的宽度
	int m_Width;
	//点区域的高度
	int m_Height;
	//阈值
	int m_Threshold;
public:
	CNode * m_pHead;
	CNode * m_pTail;
};
#endif