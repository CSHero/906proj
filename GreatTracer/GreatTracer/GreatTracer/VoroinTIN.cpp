#include "stdafx.h"
#include "VoroinTIN.h"

void VoroinTIN::Init(int Width,int Height,int Thresdhold){
	m_Height=Height;
	m_Width=Width;
	m_Threshold=Thresdhold;
}
VoroinTIN::VoroinTIN(){
	m_pHead=m_pTail=0;
}
VoroinTIN::~VoroinTIN(){
	Clear();
}
std::list<int> VoroinTIN::GenerateTin(int begin,int end)//Éú³ÉÈý½ÇÍø
{
	//for(int i=0;i<T1.GetCount();i++){
	//	m_Vertexes.push_back((int)T1[i].x);
	//	m_Vertexes.push_back((int)T1[i].y);
	//	m_Vertexes.push_back((int)T1[i].z);
	//}



	std::list<int> RetureValL;
	int BEdgeIndex,EEdgeIndex;
	RetureValL=ConvexHull(begin,end,BEdgeIndex,EEdgeIndex);
	int BeginEdgeLopIndex=m_Edges.size();
	int FinishPointIndex=RetureValL.back();
	int HeadPointIndex=RetureValL.front();
	int CurEdgeIndex=BEdgeIndex+1;
	std::list<int>::iterator CurPointIter=RetureValL.begin();
	++CurPointIter;
	++CurPointIter;
	bool Test1=true;
	bool Test2=true;
	bool flag=false;
	while ((*CurPointIter)!=FinishPointIndex)
	{
		flag=true;
		m_Edges.push_back(CEdge(HeadPointIndex,*CurPointIter));
		std::list<int>::iterator BeforePointIter=CurPointIter;
		std::list<int>::iterator NextPointIter=CurPointIter;
		++NextPointIter;
		--BeforePointIter;
		m_Edges.back().m_iRPoint=(*BeforePointIter);
		m_Edges.back().m_iLPoint=(*NextPointIter);
		CTriangle *Triangle=new CTriangle;
		if (Test1)
		{
			Triangle->m_iEdges[0]=BEdgeIndex;
			Test1=false;
		}
		else
		{
			Triangle->m_iEdges[0]=m_Edges.size()-2;
		}
		Triangle->m_iEdges[1]=CurEdgeIndex;
		Triangle->m_iEdges[2]=m_Edges.size()-1;
		Triangle->m_iPoints[0]=HeadPointIndex;
		Triangle->m_iPoints[1]=(*BeforePointIter);
		Triangle->m_iPoints[2]=(*CurPointIter);
		CNode * pnode=new CNode;
		pnode->m_pTriangle=Triangle;
		InsertHeadNode(pnode);
		m_Edges.back().m_iRightTriangle=pnode;
		if (Test2)
		{
			m_Edges[BEdgeIndex].m_iLeftTriangle=pnode;
			m_Edges[BEdgeIndex].m_iLPoint=(*CurPointIter);
			Test2=false;
		}
		else
		{
			m_Edges[m_Edges.size()-2].m_iLeftTriangle=pnode;
		}
		m_Edges[CurEdgeIndex].m_iLeftTriangle=pnode;
		m_Edges[CurEdgeIndex].m_iLPoint=HeadPointIndex;
		CurPointIter=NextPointIter;
		++CurEdgeIndex;
	}
	CTriangle * LastTriangle=new CTriangle;
	LastTriangle->m_iPoints[0]=HeadPointIndex;
	LastTriangle->m_iPoints[2]=*CurPointIter;
	--CurPointIter;
	LastTriangle->m_iPoints[1]=(*CurPointIter);
	if (flag)
	{
		LastTriangle->m_iEdges[0]=m_Edges.size()-1;
		LastTriangle->m_iEdges[1]=CurEdgeIndex;
		LastTriangle->m_iEdges[2]=EEdgeIndex;
	}
	else
	{
		LastTriangle->m_iEdges[0]=BEdgeIndex;
		LastTriangle->m_iEdges[1]=CurEdgeIndex;
		LastTriangle->m_iEdges[2]=EEdgeIndex;
	}
	CNode * plast=new CNode;
	plast->m_pTriangle=LastTriangle;
	InsertHeadNode(plast);
	if(flag)
	{
		m_Edges.back().m_iLeftTriangle=plast;
		m_Edges[CurEdgeIndex].m_iLeftTriangle=plast;
		m_Edges[CurEdgeIndex].m_iLPoint=HeadPointIndex;
		m_Edges[EEdgeIndex].m_iLeftTriangle=plast;
		m_Edges[EEdgeIndex].m_iLPoint=(*CurPointIter);
	}
	else
	{
		m_Edges[BEdgeIndex].m_iLeftTriangle=plast;
		m_Edges[CurEdgeIndex].m_iLeftTriangle=plast;
		m_Edges[EEdgeIndex].m_iLeftTriangle=plast;
		m_Edges[EEdgeIndex].m_iLPoint=(*CurPointIter);
		m_Edges[CurEdgeIndex].m_iLPoint=HeadPointIndex;
		++CurPointIter;
		m_Edges[BEdgeIndex].m_iLPoint=*CurPointIter;
	}
	std::set<int> PointSet;
	std::list<int>::iterator InsertToSetIter=RetureValL.begin();
	while(InsertToSetIter!=RetureValL.end())
	{
		PointSet.insert(*InsertToSetIter);
		++InsertToSetIter;
	}
	CNode * pFind=m_pHead;
	for (int i=begin;i<=end;++i)
	{
		if (!PointSet.count(i))
		{
			int EdgeOfPoint;
			CNode * pLeftTriangle;
			CNode * pRightTriangle;
			int Result=FindNode(i,pFind,&pLeftTriangle,&pRightTriangle,EdgeOfPoint);
			if(Result==3)
			{
				int NE1,NE2,NE3;
				int P0,P1,P2;
				int E0,E1,E2;
				CNode * TR1, * TR2,* TR3;
				TR1=new CNode;
				TR2=new CNode;
				TR3=new CNode;
				P0=pLeftTriangle->m_pTriangle->m_iPoints[0];
				P1=pLeftTriangle->m_pTriangle->m_iPoints[1];
				P2=pLeftTriangle->m_pTriangle->m_iPoints[2];
				E0=pLeftTriangle->m_pTriangle->m_iEdges[0];
				E1=pLeftTriangle->m_pTriangle->m_iEdges[1];
				E2=pLeftTriangle->m_pTriangle->m_iEdges[2];
				NE1=m_Edges.size();
				m_Edges.push_back(CEdge(i,P2));
				m_Edges.push_back(CEdge(i,P0));
				m_Edges.push_back(CEdge(i,P1));
				NE3=m_Edges.size()-1;
				NE2=NE1+1;
				m_Edges[NE1].m_iLPoint=P0;
				m_Edges[NE1].m_iRPoint=P1;
				m_Edges[NE2].m_iLPoint=P1;
				m_Edges[NE2].m_iRPoint=P2;
				m_Edges[NE3].m_iLPoint=P2;
				m_Edges[NE3].m_iRPoint=P0;
				CTriangle * Triangle1=new CTriangle;
				CTriangle * Triangle2=new CTriangle;
				CTriangle * Triangle3=new CTriangle;
				Triangle1->m_iPoints[0]=i;
				Triangle1->m_iPoints[1]=P1;
				Triangle1->m_iPoints[2]=P2;
				Triangle1->m_iEdges[0]=NE3;
				Triangle1->m_iEdges[1]=E1;
				Triangle1->m_iEdges[2]=NE1;
				Triangle2->m_iPoints[0]=i;
				Triangle2->m_iPoints[1]=P2;
				Triangle2->m_iPoints[2]=P0;
				Triangle2->m_iEdges[0]=NE1;
				Triangle2->m_iEdges[1]=E2;
				Triangle2->m_iEdges[2]=NE2;
				Triangle3->m_iPoints[0]=i;
				Triangle3->m_iPoints[1]=P0;
				Triangle3->m_iPoints[2]=P1;
				Triangle3->m_iEdges[0]=NE2;
				Triangle3->m_iEdges[1]=E0;
				Triangle3->m_iEdges[2]=NE3;
				TR1->m_pTriangle=Triangle1;
				TR2->m_pTriangle=Triangle2;
				TR3->m_pTriangle=Triangle3;
				InsertNode(pLeftTriangle,-1,TR1);
				InsertNode(pLeftTriangle,0,TR2);
				InsertNode(pLeftTriangle,1,TR3);
				m_Edges[NE1].m_iLeftTriangle=TR2;
				m_Edges[NE1].m_iRightTriangle=TR1;
				m_Edges[NE2].m_iLeftTriangle=TR3;
				m_Edges[NE2].m_iRightTriangle=TR2;
				m_Edges[NE3].m_iLeftTriangle=TR1;
				m_Edges[NE3].m_iRightTriangle=TR3;
				if (m_Edges[E0].m_BeginVertex==P0)
				{
					m_Edges[E0].m_iLeftTriangle=TR3;
					m_Edges[E0].m_iLPoint=i;
				}
				else
				{
					m_Edges[E0].m_iRightTriangle=TR3;
					m_Edges[E0].m_iRPoint=i;
				}


				if (m_Edges[E1].m_BeginVertex==P1)
				{
					m_Edges[E1].m_iLeftTriangle=TR1;
					m_Edges[E1].m_iLPoint=i;
				}
				else
				{
					m_Edges[E1].m_iRightTriangle=TR1;
					m_Edges[E1].m_iRPoint=i;
				}
				if (m_Edges[E2].m_BeginVertex==P2)
				{
					m_Edges[E2].m_iLeftTriangle=TR2;
					m_Edges[E2].m_iLPoint=i;
				}
				else
				{
					m_Edges[E2].m_iRightTriangle=TR2;
					m_Edges[E2].m_iRPoint=i;
				}
				pFind=pLeftTriangle;
			}
			else if (Result==4)
			{
				int x,y,m,n;
				int j=0;
				for (;j<3;++j)
				{
					if(m_Edges[EdgeOfPoint].m_iRightTriangle->m_pTriangle->m_iEdges[j]==EdgeOfPoint)
					{
						break;
					}
				}
				y=m_Edges[EdgeOfPoint].m_iRightTriangle->m_pTriangle->m_iEdges[(j+1)%3];
				x=m_Edges[EdgeOfPoint].m_iRightTriangle->m_pTriangle->m_iEdges[(j+2)%3];
				for (j=0;j<3;++j)
				{
					if (m_Edges[EdgeOfPoint].m_iLeftTriangle->m_pTriangle->m_iEdges[j]==EdgeOfPoint)
					{
						break;
					}
				}
				m=m_Edges[EdgeOfPoint].m_iLeftTriangle->m_pTriangle->m_iEdges[(j+1)%3];
				n=m_Edges[EdgeOfPoint].m_iLeftTriangle->m_pTriangle->m_iEdges[(j+2)%3];
				CNode* TR1,*TR2,*TR3,*TR4;
				int B=m_Edges[EdgeOfPoint].m_BeginVertex;
				int E=m_Edges[EdgeOfPoint].m_EndVertex;
				int R=m_Edges[EdgeOfPoint].m_iRPoint;
				int L=m_Edges[EdgeOfPoint].m_iLPoint;
				m_Edges[EdgeOfPoint].m_BeginVertex=i;
				int E1=m_Edges.size();
				m_Edges.push_back(CEdge(i,R));
				m_Edges.push_back(CEdge(i,B));
				m_Edges.push_back(CEdge(i,L));
				int E3=m_Edges.size()-1;
				int E2=E1+1;
				m_Edges[EdgeOfPoint].m_iLPoint=L;
				m_Edges[EdgeOfPoint].m_iRPoint=R;
				m_Edges[E1].m_iLPoint=E;
				m_Edges[E1].m_iRPoint=B;
				m_Edges[E2].m_iLPoint=R;
				m_Edges[E2].m_iRPoint=L;
				m_Edges[E3].m_iLPoint=B;
				m_Edges[E3].m_iRPoint=E;

				CTriangle *tr1=new CTriangle;
				tr1->m_iPoints[0]=i;
				tr1->m_iPoints[1]=R;
				tr1->m_iPoints[2]=E;
				tr1->m_iEdges[0]=E1;
				tr1->m_iEdges[1]=x;
				tr1->m_iEdges[2]=EdgeOfPoint;
				TR1=new CNode;
				TR1->m_pTriangle=tr1;
				InsertNode(pRightTriangle,-1,TR1);
				CTriangle *tr2=new CTriangle;
				tr2->m_iPoints[0]=i;
				tr2->m_iPoints[1]=B;
				tr2->m_iPoints[2]=R;
				tr2->m_iEdges[0]=E2;
				tr2->m_iEdges[1]=y;
				tr2->m_iEdges[2]=E1;
				TR2=new CNode;
				TR2->m_pTriangle=tr2;
				InsertNode(pRightTriangle,0,TR2);
				CTriangle *tr3=new CTriangle;
				tr3->m_iPoints[0]=i;
				tr3->m_iPoints[1]=L;
				tr3->m_iPoints[2]=B;
				tr3->m_iEdges[0]=E3;
				tr3->m_iEdges[1]=n;
				tr3->m_iEdges[2]=E2;
				TR3=new CNode;
				TR3->m_pTriangle=tr3;
				InsertNode(pLeftTriangle,-1,TR3);
				CTriangle *tr4=new CTriangle;
				tr4->m_iPoints[0]=i;
				tr4->m_iPoints[1]=E;
				tr4->m_iPoints[2]=L;
				tr4->m_iEdges[0]=EdgeOfPoint;
				tr4->m_iEdges[1]=m;
				tr4->m_iEdges[2]=E3;
				TR4=new CNode;
				TR4->m_pTriangle=tr4;
				InsertNode(pLeftTriangle,0,TR4);
				m_Edges[EdgeOfPoint].m_iLeftTriangle=TR4;
				m_Edges[EdgeOfPoint].m_iRightTriangle=TR1;
				m_Edges[E1].m_iLeftTriangle=TR1;
				m_Edges[E1].m_iRightTriangle=TR2;
				m_Edges[E2].m_iLeftTriangle=TR2;
				m_Edges[E2].m_iRightTriangle=TR3;
				m_Edges[E3].m_iLeftTriangle=TR3;
				m_Edges[E3].m_iRightTriangle=TR4;
				if (m_Edges[x].m_BeginVertex==E)
				{
					m_Edges[x].m_iRightTriangle=TR1;
					m_Edges[x].m_iRPoint=i;
				}
				else
				{
					m_Edges[x].m_iLeftTriangle=TR1;
					m_Edges[x].m_iLPoint=i;
				}


				if (m_Edges[y].m_BeginVertex==R)
				{
					m_Edges[y].m_iRightTriangle=TR2;
					m_Edges[y].m_iRPoint=i;
				}
				else
				{
					m_Edges[y].m_iLeftTriangle=TR2;
					m_Edges[y].m_iLPoint=i;
				}
				if (m_Edges[n].m_BeginVertex==B)
				{
					m_Edges[n].m_iRightTriangle=TR3;
					m_Edges[n].m_iRPoint=i;
				}
				else
				{
					m_Edges[n].m_iLeftTriangle=TR3;
					m_Edges[n].m_iLPoint=i;
				}
				if (m_Edges[m].m_BeginVertex==L)
				{
					m_Edges[m].m_iRightTriangle=TR4;
					m_Edges[m].m_iRPoint=i;
				}
				else
				{
					m_Edges[m].m_iLeftTriangle=TR4;
					m_Edges[m].m_iLPoint=i;
				}
				pFind=pLeftTriangle;
			}
			else
			{
				int x,y;
				int j=0;
				for (;j<3;++j)
				{
					if (m_Edges[EdgeOfPoint].m_iLeftTriangle->m_pTriangle->m_iEdges[j]==EdgeOfPoint)
					{
						break;
					}
				}
				y=m_Edges[EdgeOfPoint].m_iLeftTriangle->m_pTriangle->m_iEdges[(j+1)%3];
				x=m_Edges[EdgeOfPoint].m_iLeftTriangle->m_pTriangle->m_iEdges[(j+2)%3];
				CNode * TR1,* TR2;
				int B=m_Edges[EdgeOfPoint].m_BeginVertex;
				int E=m_Edges[EdgeOfPoint].m_EndVertex;
				int L=m_Edges[EdgeOfPoint].m_iLPoint;

				m_Edges[EdgeOfPoint].m_BeginVertex=i;
				int E1=m_Edges.size();
				m_Edges.push_back(CEdge(B,i));
				m_Edges.push_back(CEdge(i,L));
				int E2=E1+1;
				m_Edges[EdgeOfPoint].m_iLPoint=L;
				m_Edges[EdgeOfPoint].m_iRPoint=-1;
				m_Edges[E1].m_iLPoint=L;
				m_Edges[E1].m_iRPoint=-1;
				m_Edges[E2].m_iLPoint=B;
				m_Edges[E2].m_iRPoint=E;
				CTriangle * tr1=new CTriangle;
				tr1->m_iPoints[0]=i;
				tr1->m_iPoints[1]=E;
				tr1->m_iPoints[2]=L;
				tr1->m_iEdges[0]=EdgeOfPoint;
				tr1->m_iEdges[1]=y;
				tr1->m_iEdges[2]=E2;
				TR1=new CNode;
				TR1->m_pTriangle=tr1;
				InsertNode(pLeftTriangle,-1,TR1);
				CTriangle *tr2=new CTriangle;
				tr2->m_iPoints[0]=i;
				tr2->m_iPoints[1]=L;
				tr2->m_iPoints[2]=B;
				tr2->m_iEdges[0]=E2;
				tr2->m_iEdges[1]=x;
				tr2->m_iEdges[2]=E1;
				TR2=new CNode;
				TR2->m_pTriangle=tr2;
				InsertNode(pLeftTriangle,0,TR2);
				m_Edges[EdgeOfPoint].m_iLeftTriangle=TR1;
				m_Edges[E1].m_iLeftTriangle=TR2;
				m_Edges[E2].m_iLeftTriangle=TR2;
				m_Edges[E2].m_iRightTriangle=TR1;
				if (m_Edges[x].m_BeginVertex==B)
				{
					m_Edges[x].m_iRightTriangle=TR2;
					m_Edges[x].m_iRPoint=i;
				}
				else
				{
					m_Edges[x].m_iLeftTriangle=TR2;
					m_Edges[x].m_iLPoint=i;
				}
				if (m_Edges[y].m_BeginVertex==L)
				{
					m_Edges[y].m_iRightTriangle=TR1;
					m_Edges[y].m_iRPoint=i;
				}
				else
				{
					m_Edges[y].m_iLeftTriangle=TR1;
					m_Edges[y].m_iLPoint=i;
				}
				pFind=pLeftTriangle;
			}
		}	
	}
	int EndEdgeLopIndex=m_Edges.size()-1;
	for (int mm=BeginEdgeLopIndex;mm<=EndEdgeLopIndex;++mm)
	{
		RecursiveLop(mm);
	}
	return RetureValL;
}
bool VoroinTIN::LOP(int Index)
{
	if (m_Edges[Index].m_iRPoint!=-1)
	{
		double cosa,cosb;
		int Vx1,Vy1,Vx2,Vy2;
		int i=Index;
		int B,E,L,R;
		B=m_Edges[i].m_BeginVertex;
		E=m_Edges[i].m_EndVertex;
		L=m_Edges[i].m_iLPoint;
		R=m_Edges[i].m_iRPoint;
		CNode * TR1,*TR2;
		TR1=m_Edges[i].m_iLeftTriangle;
		TR2=m_Edges[i].m_iRightTriangle;
		Vx1=m_Vertexes[B].m_x-m_Vertexes[L].m_x;
		Vy1=m_Vertexes[B].m_y-m_Vertexes[L].m_y;
		Vx2=m_Vertexes[E].m_x-m_Vertexes[L].m_x;
		Vy2=m_Vertexes[E].m_y-m_Vertexes[L].m_y;
		cosa=(Vx1*Vx2+Vy1*Vy2)/(sqrt((double)(Vx1*Vx1+Vy1*Vy1))*sqrt((double)(Vx2*Vx2+Vy2*Vy2)));
		Vx1=m_Vertexes[B].m_x-m_Vertexes[R].m_x;
		Vy1=m_Vertexes[B].m_y-m_Vertexes[R].m_y;
		Vx2=m_Vertexes[E].m_x-m_Vertexes[R].m_x;
		Vy2=m_Vertexes[E].m_y-m_Vertexes[R].m_y;
		cosb=(Vx1*Vx2+Vy1*Vy2)/(sqrt((double)(Vx1*Vx1+Vy1*Vy1))*sqrt((double)(Vx2*Vx2+Vy2*Vy2)));
		if (cosa+cosb<0)
		{
			int m,n,x,y;
			int j=0;
			for (;j<3;++j)
			{
				if (m_Edges[i].m_iLeftTriangle->m_pTriangle->m_iEdges[j]==i)
				{
					break;
				}
			}
			n=m_Edges[i].m_iLeftTriangle->m_pTriangle->m_iEdges[(j+1)%3];
			m=m_Edges[i].m_iLeftTriangle->m_pTriangle->m_iEdges[(j+2)%3];
			for (j=0;j<3;++j)
			{
				if (m_Edges[i].m_iRightTriangle->m_pTriangle->m_iEdges[j]==i)
				{
					break;
				}
			}
			x=m_Edges[i].m_iRightTriangle->m_pTriangle->m_iEdges[(j+1)%3];
			y=m_Edges[i].m_iRightTriangle->m_pTriangle->m_iEdges[(j+2)%3];
			m_Edges[i].m_BeginVertex=L;
			m_Edges[i].m_EndVertex=R;
			m_Edges[i].m_iLPoint=E;
			m_Edges[i].m_iRPoint=B;
			TR1->m_pTriangle->m_iPoints[0]=L;
			TR1->m_pTriangle->m_iPoints[1]=R;
			TR1->m_pTriangle->m_iPoints[2]=E;
			TR1->m_pTriangle->m_iEdges[0]=i;
			TR1->m_pTriangle->m_iEdges[1]=y;
			TR1->m_pTriangle->m_iEdges[2]=n;

			TR2->m_pTriangle->m_iPoints[0]=L;
			TR2->m_pTriangle->m_iPoints[1]=B;
			TR2->m_pTriangle->m_iPoints[2]=R;
			TR2->m_pTriangle->m_iEdges[0]=m;
			TR2->m_pTriangle->m_iEdges[1]=x;
			TR2->m_pTriangle->m_iEdges[2]=i;

			m_Edges[i].m_iLeftTriangle=TR1;
			m_Edges[i].m_iRightTriangle=TR2;

			if (m_Edges[m].m_BeginVertex==L)
			{
				m_Edges[m].m_iLeftTriangle=TR2;
				m_Edges[m].m_iLPoint=R;
			}
			else
			{
				m_Edges[m].m_iRightTriangle=TR2;
				m_Edges[m].m_iRPoint=R;				
			}

			if (m_Edges[x].m_BeginVertex==B)
			{
				m_Edges[x].m_iLeftTriangle=TR2;
				m_Edges[x].m_iLPoint=L;
			}
			else
			{
				m_Edges[x].m_iRightTriangle=TR2;
				m_Edges[x].m_iRPoint=L;				
			}

			if (m_Edges[y].m_BeginVertex==R)
			{
				m_Edges[y].m_iLeftTriangle=TR1;
				m_Edges[y].m_iLPoint=L;
			}
			else
			{
				m_Edges[y].m_iRightTriangle=TR1;
				m_Edges[y].m_iRPoint=L;				
			}

			if (m_Edges[n].m_BeginVertex==E)
			{
				m_Edges[n].m_iLeftTriangle=TR1;
				m_Edges[n].m_iLPoint=R;
			}
			else
			{
				m_Edges[n].m_iRightTriangle=TR1;
				m_Edges[n].m_iRPoint=R;
			}
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
void VoroinTIN::LOP(int begin,int end)
{
	for (int i=begin;i<=end;++i)
	{
		LOP(i);
	}
}

int VoroinTIN::PointInTri(int Point,CNode * pNode,int & Edge)
{
	int Vecx1,Vecy1,Vecx2,Vecy2;
	Vecx1=m_Vertexes[pNode->m_pTriangle->m_iPoints[1]].m_x-m_Vertexes[pNode->m_pTriangle->m_iPoints[0]].m_x;
	Vecy1=m_Vertexes[pNode->m_pTriangle->m_iPoints[1]].m_y-m_Vertexes[pNode->m_pTriangle->m_iPoints[0]].m_y;
	Vecx2=m_Vertexes[Point].m_x-m_Vertexes[pNode->m_pTriangle->m_iPoints[0]].m_x;
	Vecy2=m_Vertexes[Point].m_y-m_Vertexes[pNode->m_pTriangle->m_iPoints[0]].m_y;
	int Tempa=Vecx1*Vecy2;
	int Tempb=Vecx2*Vecy1;
	if (Tempa>Tempb)
	{
		return -1;
	}
	else if (Tempa==Tempb)
	{
		int XMin,XMax;
		XMin=m_Vertexes[pNode->m_pTriangle->m_iPoints[1]].m_x;
		XMax=m_Vertexes[pNode->m_pTriangle->m_iPoints[0]].m_x;
		/*Adjust(XMin,XMax);*/
		if(XMin>XMax){
			XMax=XMax^XMin;
			XMin=XMin^XMax;
			XMax=XMax^XMin;
		}
		if (XMin!=XMax)
		{
			if (m_Vertexes[Point].m_x>XMax||m_Vertexes[Point].m_x<XMin)
			{
				return -1;
			}
			else
			{
				Edge=pNode->m_pTriangle->m_iEdges[0];
				return 0;
			}
		}
		else
		{
			int YMin,YMax;
			YMax=m_Vertexes[pNode->m_pTriangle->m_iPoints[1]].m_y;
			YMin=m_Vertexes[pNode->m_pTriangle->m_iPoints[0]].m_y;
			/*Adjust(YMin,YMax);*/
			if(YMin>YMax){
				YMax=YMax^YMin;
				YMin=YMin^YMax;
				YMax=YMax^YMin;
			}
			if (m_Vertexes[Point].m_y>YMax||m_Vertexes[Point].m_y<YMin)
			{
				return -1;
			}
			else
			{
				Edge=pNode->m_pTriangle->m_iEdges[0];
				return 0;
			}
		}

	}
	Vecx1=m_Vertexes[pNode->m_pTriangle->m_iPoints[2]].m_x-m_Vertexes[pNode->m_pTriangle->m_iPoints[1]].m_x;
	Vecy1=m_Vertexes[pNode->m_pTriangle->m_iPoints[2]].m_y-m_Vertexes[pNode->m_pTriangle->m_iPoints[1]].m_y;
	Vecx2=m_Vertexes[Point].m_x-m_Vertexes[pNode->m_pTriangle->m_iPoints[1]].m_x;
	Vecy2=m_Vertexes[Point].m_y-m_Vertexes[pNode->m_pTriangle->m_iPoints[1]].m_y;
	Tempa=Vecx1*Vecy2;
	Tempb=Vecx2*Vecy1;
	if (Tempa>Tempb)
	{
		return -1;
	}
	else if (Tempa==Tempb)
	{
		int XMin,XMax;
		XMin=m_Vertexes[pNode->m_pTriangle->m_iPoints[1]].m_x;
		XMax=m_Vertexes[pNode->m_pTriangle->m_iPoints[2]].m_x;
		//Adjust(XMin,XMax);
		if(XMin>XMax){
		XMax=XMax^XMin;
		XMin=XMin^XMax;
		XMax=XMax^XMin;
	}
		if (XMin!=XMax)
		{
			if (m_Vertexes[Point].m_x>XMax||m_Vertexes[Point].m_x<XMin)
			{
				return -1;
			}
			else
			{
				Edge=pNode->m_pTriangle->m_iEdges[1];
				return 0;
			}
		}
		else
		{
			int YMin,YMax;
			YMax=m_Vertexes[pNode->m_pTriangle->m_iPoints[1]].m_y;
			YMin=m_Vertexes[pNode->m_pTriangle->m_iPoints[2]].m_y;
			//Adjust(YMin,YMax);
			if(YMin>YMax){
				YMax=YMax^YMin;
				YMin=YMin^YMax;
				YMax=YMax^YMin;
			}
			if (m_Vertexes[Point].m_y>YMax||m_Vertexes[Point].m_y<YMin)
			{
				return -1;
			}
			else
			{
				Edge=pNode->m_pTriangle->m_iEdges[1];
				return 0;
			}
		}

	}
	Vecx1=m_Vertexes[pNode->m_pTriangle->m_iPoints[0]].m_x-m_Vertexes[pNode->m_pTriangle->m_iPoints[2]].m_x;
	Vecy1=m_Vertexes[pNode->m_pTriangle->m_iPoints[0]].m_y-m_Vertexes[pNode->m_pTriangle->m_iPoints[2]].m_y;
	Vecx2=m_Vertexes[Point].m_x-m_Vertexes[pNode->m_pTriangle->m_iPoints[2]].m_x;
	Vecy2=m_Vertexes[Point].m_y-m_Vertexes[pNode->m_pTriangle->m_iPoints[2]].m_y;
	Tempa=Vecx1*Vecy2;
	Tempb=Vecx2*Vecy1;
	if (Tempa>Tempb)
	{
		return -1;
	}
	else if (Tempa==Tempb)
	{
		int XMin,XMax;
		XMin=m_Vertexes[pNode->m_pTriangle->m_iPoints[2]].m_x;
		XMax=m_Vertexes[pNode->m_pTriangle->m_iPoints[0]].m_x;
		//Adjust(XMin,XMax);
		if(XMin>XMax){
			XMax=XMax^XMin;
			XMin=XMin^XMax;
			XMax=XMax^XMin;
		}
		if (XMin!=XMax)
		{
			if (m_Vertexes[Point].m_x>XMax||m_Vertexes[Point].m_x<XMin)
			{
				return -1;
			}
			else
			{
				Edge=pNode->m_pTriangle->m_iEdges[2];
				return 0;
			}
		}
		else
		{
			int YMin,YMax;
			YMax=m_Vertexes[pNode->m_pTriangle->m_iPoints[2]].m_y;
			YMin=m_Vertexes[pNode->m_pTriangle->m_iPoints[0]].m_y;
			//Adjust(YMin,YMax);
			if(YMin>YMax){
				YMax=YMax^YMin;
				YMin=YMin^YMax;
				YMax=YMax^YMin;
			}
			if (m_Vertexes[Point].m_y>YMax||m_Vertexes[Point].m_y<YMin)
			{
				return -1;
			}
			else
			{
				Edge=pNode->m_pTriangle->m_iEdges[2];
				return 0;
			}
		}

	}
	return 1;
}

std::list<int> VoroinTIN::ConvexHull(int Begin,int End,int & BeginEdgeIndex,int & EndEdgeIndex)
{
	std::list<int>   PointsIndex;
	const int column=10;
	const int row=10;
	std::vector<int> Block[row+1][column+1];
	double dR[row+2];
	double Space=m_Height/double(row);
	dR[0]=0.0;
	{
		for (int i=1;i<row+2;++i)
		{
			dR[i]=dR[i-1]+Space;
		}
	}
	if (dR[row+1]<=m_Height)
	{
		dR[row+1]=m_Height+1.0;
	}
	double dC[column+2];
	Space=(m_Vertexes[End].m_x-m_Vertexes[Begin].m_x)/double(column);
	dC[0]=m_Vertexes[Begin].m_x;
	{
		for (int i=1;i<column+2;++i)
		{
			dC[i]=dC[i-1]+Space;
		}
	}
	if (dC[column+1]<=m_Vertexes[End].m_x)
	{
		dC[column+1]=m_Vertexes[End].m_x+1.0;
	}
	{
		for (int i=Begin;i<=End;++i)
		{
			int c=VoroinTIN::GetIndex(m_Vertexes[i].m_x,dC,column+2);
			int r=VoroinTIN::GetIndex(m_Vertexes[i].m_y,dR,row+2);
			Block[r][c].push_back(i);
		}
	}
	{
		for (int i=Begin;i<=End;++i)
		{
			if (m_Vertexes[i].m_x==m_Vertexes[Begin].m_x)
			{
				PointsIndex.push_back(i);
			}
			else
			{
				break;
			}
		}
	}
	int R_Block=row;
	bool flag=true;
	int max=-1;
	std::vector<int> TempBottom;
	while (flag&&R_Block>=0)
	{
		for (int i=0;i<column+1;++i)
		{
			std::vector<int> & v=Block[R_Block][i];
			std::vector<int>::iterator iter=v.begin();
			while(iter!=v.end())
			{
				if(m_Vertexes[*iter].m_y>max)
				{
					max=m_Vertexes[*iter].m_y;
					TempBottom.clear();
					TempBottom.push_back(*iter);
				}
				else if (m_Vertexes[*iter].m_y==max)
				{
					TempBottom.push_back(*iter);
				}
				++iter;
			}
		}
		if (TempBottom.size()==0)
		{
			--R_Block;
		}
		else
		{
			flag=false;
		}
	}
	for (std::vector<int>::size_type s=0;s<TempBottom.size();++s)
	{
		PointsIndex.push_back(TempBottom[s]);
	}
	{
		for (int i=End;i>=Begin;--i)
		{
			if (m_Vertexes[i].m_x==m_Vertexes[End].m_x)
			{
				PointsIndex.push_back(i);
			}
			else
			{
				break;
			}
		}
	}
	int C_Block=0;
	flag=true;
	int min=m_Height+1;
	std::vector<int> TempTop;
	while (flag&&C_Block<=row)
	{
		for (int i=0;i<column+1;++i)
		{
			std::vector<int> & v=Block[C_Block][i];
			std::vector<int>::iterator iter=v.begin();
			while(iter!=v.end())
			{
				if(m_Vertexes[*iter].m_y<min)
				{
					min=m_Vertexes[*iter].m_y;
					TempTop.clear();
					TempTop.push_back(*iter);
				}
				else if (m_Vertexes[*iter].m_y==min)
				{
					TempTop.push_back(*iter);
				}
				++iter;
			}
		}
		if (TempTop.size()==0)
		{
			++C_Block;
		}
		else
		{
			flag=false;
		}
	}
	{
		std::vector<int>::reverse_iterator Riter=TempTop.rbegin();
		while (Riter!=TempTop.rend())
		{
			PointsIndex.push_back(*Riter);
			++Riter;
		}
	}
	PointsIndex.unique();
	if (PointsIndex.back()==PointsIndex.front())
	{
		PointsIndex.pop_back();
	}
	int head=PointsIndex.front();
	PointsIndex.push_back(head);
	double Dista=-1.00;
	std::vector<int> MFarPoints;
	std::list<int>::iterator Iter=PointsIndex.begin();
	std::list<int>::iterator NextIter;
	bool IsDown=true;
	while((*Iter)!=head||IsDown)
	{
		NextIter=Iter;
		++NextIter;
		if ((*Iter)==End)
		{
			IsDown=false;
		}
		int iSmallRow,iSmallColumn,iBigRow,iBigColumn;
		iSmallRow=VoroinTIN::GetIndex(m_Vertexes[*Iter].m_y,dR,row+2);
		iSmallColumn=VoroinTIN::GetIndex(m_Vertexes[*Iter].m_x,dC,column+2);
		iBigRow=VoroinTIN::GetIndex(m_Vertexes[*NextIter].m_y,dR,row+2);
		iBigColumn=VoroinTIN::GetIndex(m_Vertexes[*NextIter].m_x,dC,column+2);
		//Adjust(iSmallRow,iBigRow);
		if(iSmallRow>iBigRow){
			iSmallRow=iSmallRow^iBigRow;
			iBigRow^=iSmallRow;
			iSmallRow^=iBigRow;
		}
		//Adjust(iSmallColumn,iBigColumn);
		if(iSmallColumn>iBigColumn){
			iSmallColumn=iSmallColumn^iBigColumn;
			iBigColumn^=iSmallColumn;
			iSmallColumn^=iBigColumn;
		}
		for (int Cj=iSmallColumn;Cj<=iBigColumn;++Cj)
		{
			for (int Ri=iSmallRow;Ri<=iBigRow;++Ri)
			{
				std::vector<int> & TemIndex=Block[Ri][Cj];
				for (std::vector<int>::size_type i=0;i<TemIndex.size();++i)
				{
					int IsRight=PointIsRight((*Iter),(*NextIter),TemIndex[i]);
					if (IsRight>0)
					{
						double TempDis=Distance(TemIndex[i],(*Iter),(*NextIter));
						if (TempDis>Dista)
						{
							Dista=TempDis;
							MFarPoints.clear();
							MFarPoints.push_back(TemIndex[i]);
						}
						else if (TempDis==Dista)
						{
							MFarPoints.push_back(TemIndex[i]);
						}
					}
				}
			}
		}
		if (MFarPoints.empty())
		{
			Iter=NextIter;
			Dista=-1.0;
		}
		else
		{
			if (IsDown)
			{
				for (std::vector<int>::size_type i=0;i<MFarPoints.size();++i)
				{
					PointsIndex.insert(NextIter,MFarPoints[i]);
				}
			}
			else
			{
				std::list<int>::iterator TempIter=NextIter;
				for (std::vector<int>::size_type i=0;i<MFarPoints.size();++i)
				{
					TempIter=PointsIndex.insert(TempIter,MFarPoints[i]);
				}
			}
			Dista=-1.0;
			MFarPoints.clear();
		}
	}
	BeginEdgeIndex=m_Edges.size();
	int BeginIndex=PointsIndex.front();
	std::list<int>::iterator IterEdge=PointsIndex.begin();
	std::list<int>::iterator NextIterEdge=IterEdge;
	++NextIterEdge;
	m_Edges.push_back(CEdge(*IterEdge,*NextIterEdge,-1));
	IterEdge=NextIterEdge;
	while((*IterEdge)!=BeginIndex)
	{
		NextIterEdge=IterEdge;
		++NextIterEdge;
		m_Edges.push_back(CEdge(*IterEdge,*NextIterEdge,-1));
		IterEdge=NextIterEdge;
	}
	EndEdgeIndex=m_Edges.size()-1;
	PointsIndex.pop_back();
	return PointsIndex;
}

int VoroinTIN::GetIndex(double Value,double * Base,int Size)
{
	int low=0;
	int high=Size-1;
	int mid;
	while (low<=high)
	{
		mid=(low+high)/2;
		if (Base[mid]==Value)
		{

			return mid;
		}
		else if (Base[mid]>Value)
		{
			high=mid-1;
		}
		else
		{
			low=mid+1;
		}
	}
	return high;
}
int VoroinTIN::PointIsRight(int Begin,int End,int Point)
{
	int x1,y1,x2,y2;
	x1=m_Vertexes[Point].m_x-m_Vertexes[Begin].m_x;
	y1=m_Vertexes[Point].m_y-m_Vertexes[Begin].m_y;
	x2=m_Vertexes[End].m_x-m_Vertexes[Begin].m_x;
	y2=m_Vertexes[End].m_y-m_Vertexes[Begin].m_y;
	return x2*y1-x1*y2;
}
double VoroinTIN::Distance(int Point,int Begin,int End)
{
	double ReturnVal;
	int x1,y1,x2,y2;
	x1=m_Vertexes[Point].m_x-m_Vertexes[Begin].m_x;
	y1=m_Vertexes[Point].m_y-m_Vertexes[Begin].m_y;
	x2=m_Vertexes[End].m_x-m_Vertexes[Begin].m_x;
	y2=m_Vertexes[End].m_y-m_Vertexes[Begin].m_y;
	double Temp=x1*y2-x2*y1;
	ReturnVal=(Temp*Temp)/(x2*x2+y2*y2);
	return ReturnVal;
}
//void VoroinTIN::Adjust(int &small,int &big){
//	if (small>big)
//	{
//		int temp=big;
//		big=small;
//		small=temp;
//	}
//}
void VoroinTIN::RecursiveLop(int Index)
{
	if(LOP(Index))
	{
		int j=0;
		for (;j<3;++j)
		{
			if (m_Edges[Index].m_iLeftTriangle->m_pTriangle->m_iEdges[j]==Index)
			{
				break;
			}
		}
		int x=m_Edges[Index].m_iLeftTriangle->m_pTriangle->m_iEdges[(j+1)%3];
		int y=m_Edges[Index].m_iLeftTriangle->m_pTriangle->m_iEdges[(j+2)%3];
		for (j=0;j<3;++j)
		{
			if (m_Edges[Index].m_iRightTriangle->m_pTriangle->m_iEdges[j]==Index)
			{
				break;
			}
		}
		int m=m_Edges[Index].m_iRightTriangle->m_pTriangle->m_iEdges[(j+1)%3];
		int n=m_Edges[Index].m_iRightTriangle->m_pTriangle->m_iEdges[(j+2)%3];
		RecursiveLop(x);
		RecursiveLop(y);
		RecursiveLop(m);
		RecursiveLop(n);
	}
	else
	{
		return;
	}
}
void VoroinTIN::InsertHeadNode( CNode * pNode)
{
	if (m_pTail)
	{
		pNode->m_pLeft=m_pTail;
		m_pTail->m_pRight=pNode;
		m_pTail=pNode;
	}
	else
	{
		m_pHead=m_pTail=pNode;
	}
}
void VoroinTIN::InsertNode(CNode * pParentNode,int Flag,CNode * pChildNode)
{
	switch (Flag)
	{
	case 1:
		pParentNode->m_pR=pChildNode;
		pChildNode->m_pParent=pParentNode;
		pParentNode->m_pM->m_pRight=pChildNode;
		pChildNode->m_pLeft=pParentNode->m_pM;
		break;
	case -1:
		pParentNode->m_pL=pChildNode;
		pChildNode->m_pParent=pParentNode;
		break;
	case 0:
		pParentNode->m_pM=pChildNode;
		pChildNode->m_pParent=pParentNode;
		pParentNode->m_pL->m_pRight=pChildNode;
		pChildNode->m_pLeft=pParentNode->m_pL;
		break;
	}
}

int VoroinTIN::FindNode(int i,CNode * pStart,CNode **pLeftTri,CNode **pRightTri,int &rEdge)
{
	int Edge;
	int Result=PointInTri(i,pStart,Edge);
	if (Result==1)
	{
		int f=-2;
		int e;
		CNode * pNode=pStart->m_pL;
		while (pNode)
		{
			f=PointInTri(i,pNode,e);
			if (f>=0)
			{
				break;
			}
			else
			{
				pNode=pNode->m_pRight;
			}
		}
		if (f==1)
		{
			*pLeftTri=pNode;
			return 3;
		}
		if (f==0)
		{
			if (m_Edges[e].m_iRPoint!=-1)
			{
				*pLeftTri=m_Edges[e].m_iLeftTriangle;
				*pRightTri=m_Edges[e].m_iRightTriangle;
				rEdge=e;
				return 4;
			}
			else
			{
				*pLeftTri=m_Edges[e].m_iLeftTriangle;
				rEdge=e;
				return 2;
			}
		}
		if (f==-2)
		{
			*pLeftTri=pStart;
			return 3;
		}
	}
	else if (Result==0)
	{
		if(m_Edges[Edge].m_iRPoint==-1)
		{
			*pLeftTri=m_Edges[Edge].m_iLeftTriangle;
			rEdge=Edge;
			return 2;
		}
		else
		{
			*pLeftTri=m_Edges[Edge].m_iLeftTriangle;
			*pRightTri=m_Edges[Edge].m_iRightTriangle;
			rEdge=Edge;
			return 4;
		}
	}
	else
	{
		CNode * pTemp=pStart->m_pParent;
		int Flag;
		while (pTemp)
		{
			Flag=PointInTri(i,pTemp,Edge);
			if (Flag>=0)
			{
				break;
			}
			else
			{
				pTemp=pTemp->m_pParent;
			}
		}
		if (pTemp)
		{
			if (Flag==0)
			{
				if(m_Edges[Edge].m_iRPoint==-1)
				{
					*pLeftTri=m_Edges[Edge].m_iLeftTriangle;
					rEdge=Edge;
					return 2;
				}
				else
				{
					*pLeftTri=m_Edges[Edge].m_iLeftTriangle;
					*pRightTri=m_Edges[Edge].m_iRightTriangle;
					rEdge=Edge;
					return 4;
				}
			}
			else
			{
				CNode * pTop=pTemp;
				CNode * pCur=pTemp->m_pL;
				int f;
				int e;
				while (pCur)
				{
					f=PointInTri(i,pCur,e);
					if (f==1)
					{
						pTop=pCur;
						pCur=pCur->m_pL;
					}
					else if (f==0)
					{
						if(m_Edges[e].m_iRPoint==-1)
						{
							*pLeftTri=m_Edges[e].m_iLeftTriangle;
							rEdge=e;
							return 2;
						}
						else
						{
							*pLeftTri=m_Edges[e].m_iLeftTriangle;
							*pRightTri=m_Edges[e].m_iRightTriangle;
							rEdge=e;
							return 4;
						}
					}
					else
					{
						pCur=pCur->m_pRight;
					}
				}
				*pLeftTri=pTop;
				return 3;
			}
		}
		else
		{
			CNode * pn=m_pHead;
			int f;
			int e;
			while(pn)
			{
				f=PointInTri(i,pn,e);
				if (f==1)
				{
					CNode * pTop=pn;
					CNode * pCur=pTop->m_pL;
					while (pCur)
					{
						f=PointInTri(i,pCur,e);
						if (f==1)
						{
							pTop=pCur;
							pCur=pCur->m_pL;
						}
						else if (f==0)
						{
							if(m_Edges[e].m_iRPoint==-1)
							{
								*pLeftTri=m_Edges[e].m_iLeftTriangle;
								rEdge=e;
								return 2;
							}
							else
							{
								*pLeftTri=m_Edges[e].m_iLeftTriangle;
								*pRightTri=m_Edges[e].m_iRightTriangle;
								rEdge=e;
								return 4;
							}
						}
						else
						{
							pCur=pCur->m_pRight;
						}
					}
					*pLeftTri=pTop;
					return 3;
				}
				else if (f==0)
				{
					if(m_Edges[e].m_iRPoint==-1)
					{
						*pLeftTri=m_Edges[e].m_iLeftTriangle;
						rEdge=e;
						return 2;
					}
					else
					{
						*pLeftTri=m_Edges[e].m_iLeftTriangle;
						*pRightTri=m_Edges[e].m_iRightTriangle;
						rEdge=e;
						return 4;
					}
				}
				else
				{
					pn=pn->m_pRight;
				}
			}
		}
	}
}

void VoroinTIN::Clear(CNode * pNode)
{
	if (pNode->m_pL)
	{
		Clear(pNode->m_pL);
	}
	if (pNode->m_pM)
	{
		Clear(pNode->m_pM);
	}
	if (pNode->m_pR)
	{
		Clear(pNode->m_pR);
	}
	delete pNode->m_pTriangle;
	delete pNode;
}
void VoroinTIN::Clear()
{
	if (m_pHead)
	{
		CNode * pNode=m_pHead;
		CNode * pNext=pNode->m_pRight;
		Clear(pNode);
		while(pNext)
		{
			pNode=pNext;
			pNext=pNext->m_pRight;
			Clear(pNode);
		}
		m_pHead=m_pTail=0;
	}
}