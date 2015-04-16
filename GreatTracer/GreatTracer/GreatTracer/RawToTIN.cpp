#include "stdafx.h"
#include "RawToTIN.h"
#include <algorithm>

int  MAP_SIZE=1024;
int STEP_SIZE=6;
GLfloat HEIGHT_RATIO=1.5f;

vector<vertexInfo> Vertex_Info;//所有点信息
vector<GLfloat> vertexheight;//所有点的高度

vertexCoor *p;
vertexColor *q;

CRawToTIN::CRawToTIN(void)
{
	RenderFlag=true;
}

CRawToTIN::~CRawToTIN(void)
{
}

void CRawToTIN::loadRawFile(CString filename,int nSize,byte *pHeightMap){
	FILE *pFile=NULL;
	pFile=fopen(filename,"rb");
	if(pFile==NULL){
		MessageBox(NULL,"Raw File open Error~~!","Tips",MB_OK);
		return;
	}
	fread(pHeightMap,1,nSize,pFile);
	int result=ferror(pFile);//若读取文件出错，获取错误信息
	if(result)
		MessageBox(NULL,"Read Data Error~~!","Tips",MB_OK);
	fclose(pFile);

	dealVertex(pHeightMap);
}

GLfloat CRawToTIN::GetHeight(byte *pHeightMap,int X,int Y){
	int x=X % MAP_SIZE;
	int y=Y % MAP_SIZE;

	if(!pHeightMap) return 0;
	return static_cast<GLfloat>(pHeightMap[x+(y*MAP_SIZE)]);
}
void CRawToTIN::RenderRawTIN(byte pHeightMap[]){
	if(Vertex_Info.empty()) return;

	glEnable(GL_CULL_FACE);
	if(RenderFlag)
		glBegin(GL_QUADS);
	else
		glBegin(GL_LINES);
	for(unsigned int i=0;i < Vertex_Info.size();++i){
		glColor3f(q[i].red,q[i].green,q[i].blue);
		glVertex3f(p[i].x,p[i].y,p[i].z);
	}
	glEnd();
}

void CRawToTIN::dealVertex(BYTE pHeightMap[]){
	int X = 0, Y = 0;									// Create Some Variables To Walk The Array With.
	if(!pHeightMap) return;	
	//先获取所有的颜色值，方便分段
	vertexInfo tempvertex;
	for ( X = 0; X < (MAP_SIZE-STEP_SIZE); X += STEP_SIZE){
		for ( Y = 0; Y < (MAP_SIZE-STEP_SIZE); Y += STEP_SIZE ){
			vertexheight.push_back(GetHeight(pHeightMap,X,Y));

			tempvertex.x=static_cast<GLfloat>(X);tempvertex.y=static_cast<GLfloat>(GetHeight(pHeightMap,X,Y));tempvertex.z=static_cast<GLfloat>(Y);
			Vertex_Info.push_back(tempvertex);

			tempvertex.x=static_cast<GLfloat>(X);tempvertex.y=static_cast<GLfloat>(GetHeight(pHeightMap,X,Y+STEP_SIZE));tempvertex.z=static_cast<GLfloat>(Y+STEP_SIZE);
			Vertex_Info.push_back(tempvertex);

			tempvertex.x=static_cast<GLfloat>(X+STEP_SIZE);tempvertex.y=static_cast<GLfloat>(GetHeight(pHeightMap,X+STEP_SIZE,Y+STEP_SIZE));tempvertex.z=static_cast<GLfloat>(Y+STEP_SIZE);
			Vertex_Info.push_back(tempvertex);

			tempvertex.x=static_cast<GLfloat>(X+STEP_SIZE);tempvertex.y=static_cast<GLfloat>(GetHeight(pHeightMap,X+STEP_SIZE,Y));tempvertex.z=static_cast<GLfloat>(Y);
			Vertex_Info.push_back(tempvertex);

		}
	}
	sort(vertexheight.begin(),vertexheight.end());
	for(unsigned int i=0;i<Vertex_Info.size();++i){
		Vertex_Info[i]=dealColor(Vertex_Info[i]);
	}
	for(unsigned int i=0;i<Vertex_Info.size();++i){
		MapCenterX+=Vertex_Info[i].x/Vertex_Info.size();
		MapCenterY+=Vertex_Info[i].y/Vertex_Info.size();
		MapCenterZ+=Vertex_Info[i].z/Vertex_Info.size();
	}
	for(unsigned int i=0;i<Vertex_Info.size();++i){
		Vertex_Info[i].x-=MapCenterX;
		Vertex_Info[i].y-=MapCenterY;
		Vertex_Info[i].z-=MapCenterZ;
	}
	p=new vertexCoor[Vertex_Info.size()];
	q=new vertexColor[Vertex_Info.size()];
	for(unsigned int i=0;i<Vertex_Info.size();++i){
		p[i].x=Vertex_Info[i].x;
		p[i].y=Vertex_Info[i].y;
		p[i].z=Vertex_Info[i].z;

		q[i].red=Vertex_Info[i].red;
		q[i].green=Vertex_Info[i].green;
		q[i].blue=Vertex_Info[i].blue;
	}
}

vertexInfo CRawToTIN::dealColor(vertexInfo tempvertex){
	GLfloat indice0=static_cast<GLfloat>(vertexheight[0]);
	GLfloat indice1=static_cast<GLfloat>(vertexheight[vertexheight.size()*8/10]);
	GLfloat indice2=static_cast<GLfloat>(vertexheight[vertexheight.size()*9/10]);
	GLfloat indice3=static_cast<GLfloat>(vertexheight[vertexheight.size()-1]);

	if(tempvertex.y>=indice0&&tempvertex.y<=indice1){
		tempvertex.red=0;
		tempvertex.green=(GLfloat) (1- (indice1-tempvertex.y)/(indice1-indice0));
		tempvertex.blue=0;
	}
	if(tempvertex.y>=indice1&&tempvertex.y<=indice2){
		tempvertex.red=(GLfloat)(1-(indice2-tempvertex.y)/(indice2-indice1) );
		tempvertex.green=1;
		tempvertex.blue=0;
	}
	if(tempvertex.y>indice2&&tempvertex.y<=indice3){
		tempvertex.red=1.0;
		tempvertex.green=1.0;
		tempvertex.blue=(GLfloat)(1-(indice3-tempvertex.y)/(indice3-indice2) );
	}
	return tempvertex;
}