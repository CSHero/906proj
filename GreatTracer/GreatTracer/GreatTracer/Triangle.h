#ifndef CTRIANGLE_H
#define CTRIANGLE_H
class CTriangle
{
public:
	CTriangle();
	virtual ~CTriangle();
protected:
public:
	//�㰴��ʱ������
	int m_iPoints[3];
	//�߰���ʱ������
	int m_iEdges[3];
};
#endif