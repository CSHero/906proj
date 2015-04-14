#ifndef CVERTEX_H
#define CVERTEX_H
class CVertex
{
public:
	CVertex();
	CVertex(int x,int y,int z=0);
	virtual ~CVertex();
	bool operator <(const CVertex & Vertex)const;
	bool operator ==(const CVertex & Vertex)const;
protected:
public:
	int m_x;
	int m_y;
	int m_z;
};
#endif
