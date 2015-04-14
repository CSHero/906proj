#ifndef CEDGE_H
#define CEDGE_H
#include <list>
#include "Triangle.h"
#include "Vertex.h"
#include "Node.h"
class CEdge
{
public:
	CEdge();
	CEdge(int BeginV,int EndV,int RPoint)
	{
		m_BeginVertex=BeginV;
		m_EndVertex=EndV;
		m_iRPoint=RPoint;
	}
	CEdge(int BeginV,int EndV)
	{
		m_BeginVertex=BeginV;
		m_EndVertex=EndV;
	}
	virtual ~CEdge();
protected:
public:
	int		m_BeginVertex;
	int		m_EndVertex;
	CNode *   m_iLeftTriangle;
	CNode *   m_iRightTriangle;
	int     m_iLPoint;
	int     m_iRPoint;
};
#endif