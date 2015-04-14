#ifndef CTRIANGLE_H
#define CTRIANGLE_H
class CTriangle
{
public:
	CTriangle();
	virtual ~CTriangle();
protected:
public:
	//点按逆时针排列
	int m_iPoints[3];
	//边按逆时针排列
	int m_iEdges[3];
};
#endif