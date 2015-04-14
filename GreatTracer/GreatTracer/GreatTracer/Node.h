#ifndef CNODE_H
#define CNODE_H
#include "Triangle.h"
class CNode
{
public:
	CNode();
public:
	CNode * m_pParent;
	CNode * m_pLeft;
	CNode * m_pRight;
	CNode * m_pL;
	CNode * m_pM;
	CNode * m_pR;
	CTriangle * m_pTriangle;
protected:
private:
};
#endif