#include "StdAfx.h"
#include "RawToTIN.h"

int  MAP_SIZE=1024;
int STEP_SIZE=16;
GLfloat HEIGHT_RATIO=1.5f;
bool RenderFlag=true;

CRawToTIN::CRawToTIN(void)
{
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
}

int CRawToTIN::GetHeight(byte *pHeightMap,int X,int Y){
	int x=X % MAP_SIZE;
	int y=Y % MAP_SIZE;

	if(!pHeightMap) return 0;
	return pHeightMap[x+(y*MAP_SIZE)];
}

void CRawToTIN::SetVertexColor(byte *pHeightMap,int x,int y){
	if(!pHeightMap) return;
	float fColor=-0.15f+(GetHeight(pHeightMap,x,y)/256.0f);
	glColor3f(0,0,fColor);
}

void CRawToTIN::RenderRawTIN(byte pHeightMap[]){
	int X=0,Y=0;
	int x,y,z;
	if(!pHeightMap) return;
	if(RenderFlag)
		glBegin(GL_QUADS);
	else
		glBegin(GL_LINES);
	for(X=0;X<(MAP_SIZE-STEP_SIZE);X+=STEP_SIZE)
		for(Y=0;Y<(MAP_SIZE-STEP_SIZE);Y+=STEP_SIZE){
			x = X;							
			y = GetHeight(pHeightMap, X, Y );	
			z = Y;							
			SetVertexColor(pHeightMap, x, z);
			glVertex3i(x, y, z);						// Send This Vertex To OpenGL To Be Rendered (Integer Points Are Faster)

			x = X;										
			y = GetHeight(pHeightMap, X, Y + STEP_SIZE );  
			z = Y + STEP_SIZE ;							
			SetVertexColor(pHeightMap, x, z);
			glVertex3i(x, y, z);						// Send This Vertex To OpenGL To Be Rendered

			x = X + STEP_SIZE; 
			y = GetHeight(pHeightMap, X + STEP_SIZE, Y + STEP_SIZE ); 
			z = Y + STEP_SIZE ;
			SetVertexColor(pHeightMap, x, z);
			glVertex3i(x, y, z);						// Send This Vertex To OpenGL To Be Rendered

			x = X + STEP_SIZE; 
			y = GetHeight(pHeightMap, X + STEP_SIZE, Y ); 
			z = Y;
			SetVertexColor(pHeightMap, x, z);
			glVertex3i(x, y, z);
		}
	glEnd();
	glColor4f(1.0f,1.0f,1.0f,1.0f);
}