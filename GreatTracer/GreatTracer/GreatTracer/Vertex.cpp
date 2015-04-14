#include "stdafx.h"
#include "Vertex.h"

CVertex::CVertex()
{

}
CVertex::~CVertex()
{

}
CVertex::CVertex(int x,int y,int z)
{
	m_x=x;
	m_y=y;
	m_z=z;
}
bool CVertex::operator <(const CVertex & Vertex)const
{
	if (m_x<Vertex.m_x)
	{
		return true;
	}
	else if (m_x==Vertex.m_x)
	{
		if (m_y<Vertex.m_y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
bool CVertex::operator ==(const CVertex &Vertex)const
{
	if (m_x==Vertex.m_x&&m_y==Vertex.m_y)
	{
		return true;
	}
	else
	{
		return false;
	}
}
