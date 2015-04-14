#include "StdAfx.h"
#include "DrawEv.h"
#include "deelx.h"
#include "String.h"
#include "LoadTexTure.h"
#include "resource.h"
#include "VIEWDLG.h"
#include "TIN.h"

CVIEWDLG cviewdlg;//CVIEWDLG类的实例化
int isRender=0;//是否渲染结果，全局变量CVIEWDLG

GLfloat DrawVertices[24];
GLint index_list[][4] = {
	0, 1, 2, 3,
	0, 4, 7, 3,
	0, 1, 5, 4,
	6, 7, 4, 5,
	6, 2, 3, 7,
	6, 2, 1, 5
};
DrawEv::DrawEv(void)
{
}
DrawEv::~DrawEv(void)
{
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////读取地图数据////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void DrawEv::ReadDxf(CString path){
	CStdioFile readDxfVertex;
	CString temp;
	if(!tempDxfVertex.IsEmpty())
		tempDxfVertex.RemoveAll();
	if(!DxfVertex.IsEmpty())
		DxfVertex.RemoveAll();
	if(!CityVertex.IsEmpty())
		CityVertex.RemoveAll();
	if(!tempCityVertex.IsEmpty())
		tempCityVertex.RemoveAll();
	if(!MultiVertexNum.IsEmpty())
		MultiVertexNum.RemoveAll();
	if(!MultiVertexPanel.IsEmpty())
		MultiVertexPanel.RemoveAll();
	readDxfVertex.Open(path,CFile::modeRead);
	while (readDxfVertex.ReadString(temp)){
		if (temp == "3DFACE"){
			for (int i = 0; i < 4; i++){
				readDxfVertex.ReadString(temp);
			}
			if (temp == "3DFACE"){
				CString strTem[24];
				for (int i = 0; i < 24; i++){             //每24行一读，将其存储到字符串数组,四个点，一个面
					readDxfVertex.ReadString(temp);
					strTem[i] = temp;
				}
				for (int j = 1; j <= 24; j++){
					tempDxfVertex.Add(strTem[j]);
					j++;
				}
			}
		}
	}
	//把string的数组转换成float的数组
	for(int i=0;i<tempDxfVertex.GetCount();i++){
		DxfVertex.Add((GLfloat)atof(tempDxfVertex[i]));
	}
	int n=DxfVertex.GetCount()/3;
	MapCenterX=0;
	MapCenterY=0;
	MapCenterZ=0;
	for(int i=0;i<n;i++){
		MapCenterX+=(GLfloat)DxfVertex[i*3]/n;
		MapCenterY+=(GLfloat)DxfVertex[i*3+1]/n;
		MapCenterZ+=(GLfloat)DxfVertex[i*3+2]/n;
	}

	for(int i=0;i<DxfVertex.GetCount();i+=3){
		DxfVertex[i]-=MapCenterX;
		DxfVertex[i+1]-=MapCenterY;
	}
}
void DrawEv::ReadCity(CString path){
	CStdioFile cf;
	int a=0;
	char split=' ';
	CString temp,temp1;
	if(!tempDxfVertex.IsEmpty())
		tempDxfVertex.RemoveAll();
	if(!DxfVertex.IsEmpty())
		DxfVertex.RemoveAll();
	if(!CityVertex.IsEmpty())
		CityVertex.RemoveAll();
	if(!tempCityVertex.IsEmpty())
		tempCityVertex.RemoveAll();
	if(!MultiVertexNum.IsEmpty())
		MultiVertexNum.RemoveAll();
	if(!MultiVertexPanel.IsEmpty())
		MultiVertexPanel.RemoveAll();
	if(!PolygonVertex.IsEmpty())
		PolygonVertex.RemoveAll();
	if(!cf.Open(path,CFile::modeRead))
		MessageBox(NULL,"open failed!","tip",MB_OK);

	while(cf.ReadString(temp)){
		if(find_remark(temp)){
			if(4==GetVertexNum(temp)){
				for(int i=0;i<4;i++){
					cf.ReadString(temp);
					decodeCString(temp,tempCityVertex,split);
				}
			}
			else{
				int n=GetVertexNum(temp);
				MultiVertexNum.Add(n);
				for(int i=0;i<n;i++){
					cf.ReadString(temp);
					decodeCString(temp,MultiVertexPanel,split);
				}
			}
		}
	}
	for(int i=0;i<MultiVertexPanel.GetCount();i++){
		PolygonVertex.Add((GLfloat)atof(MultiVertexPanel[i]));
	}
	for(int i=0;i<tempCityVertex.GetCount();i++){
		CityVertex.Add((GLfloat)atof(tempCityVertex[i]));
	}


	//以下使地图坐标移动到原点，以便于轨迹球旋转和缩放时 地图始终处于视界中心

	int n=CityVertex.GetCount()/3;
	MapCenterX=0;
	MapCenterY=0;
	MapCenterZ=0;
	for(int i=0;i<n;i++){
		MapCenterX+=CityVertex[i*3]/n;
		MapCenterY+=CityVertex[i*3+1]/n;
		//MapCenterZ+=CityVertex[i*3+2]/n;
	}
	for(int i=0;i<n;i++){
		CityVertex[i*3]-=MapCenterX;
		CityVertex[i*3+1]-=MapCenterY;
		//CityVertex[i*3+2]-=MapCenterZ;
	}
	int m=PolygonVertex.GetCount()/3;

	for(int i=0;i<m;i++){
		PolygonVertex[i*3]-=MapCenterX;
		PolygonVertex[i*3+1]-=MapCenterY;
		//PolygonVertex[i*3+2]MapCenterZ;
	}
}
int DrawEv::find_remark(CString str){
	// declare
	static CRegexpT <char> regexp("\\bnVertices\\b.*\\d+");
	// find and match
	MatchResult result = regexp.Match(str);
	// result
	if( result.IsMatched() ){
		return 1;
	}
	else
		return 0;
}
int DrawEv::GetVertexNum(CString str){
	CString temp;
	for(int i=9;i<str.GetLength();i++){
		temp+=str[i];
	}
	int re=atoi(temp);
	return re;
}
void DrawEv::decodeCString(CString source, CStringArray &CityVertex, char division)
{
	CString temp;
	int i=0;
	int j=0;
	for ( ;i<source.GetLength();i++){
		if(source[i]==division||i==source.GetLength()-1){
			for(;j<i;j++)
				temp+=source[j];
			CityVertex.Add(temp);
			temp="";
			continue;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
/////////////////////绘制地图/////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
GLfloat fVertices[12];//三维画图
GLfloat fVertices2D[8];//二维画图
void DrawEv::DrawCity(CString CityName,bool IsRender){

	LoadTexTure *LTT=NULL;//实例化贴图类
	LTT=new LoadTexTure();

	int Assi=0;
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor3f(0.64f,0.64f,0.64f);
	for( int i=0;i<MultiVertexNum.GetCount();i++){
		glBegin(GL_POLYGON);
		int a=( MultiVertexNum[i]+Assi)*3;
		for(int j=Assi*3;j<a;j+=3){
			glVertex3f( PolygonVertex[j], PolygonVertex[j+1], PolygonVertex[j+2]);
			Assi++;
		}
		glEnd();
	}
	Assi=0;
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glLineWidth(1.0f);
	glColor3f(0.64f,0.64f,0.64f);
	for( int i=0;i< MultiVertexNum.GetCount();i++){
		glBegin(GL_POLYGON);
		int a=( MultiVertexNum[i]+Assi)*3;
		for(int j=Assi*3;j<a;j+=3){
			glVertex3f( PolygonVertex[j],PolygonVertex[j+1] , PolygonVertex[j+2]);
			Assi++;
		}
		glEnd();
	}

	byte indices[4]={0,1,2,3};
	int n= CityVertex.GetCount()/3;
	for(int i=0;i<n;){
		for(int j=0;j<4;j++){
			fVertices[j*3]= CityVertex[i*3];
			fVertices[j*3+1]= CityVertex[i*3+1];
			fVertices[j*3+2]= CityVertex[i*3+2];
			i++;
		}
		glVertexPointer(3,GL_FLOAT,0,fVertices);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glColor3f(1.0f,0.04f,0.8f);
		glDrawElements(GL_QUADS,4,GL_UNSIGNED_BYTE,indices);
		glPolygonMode(GL_FRONT,GL_FILL);
		glColor3f(0.64f,0.64f,0.64f);
		glDrawElements(GL_QUADS,4,GL_UNSIGNED_BYTE,indices);
	}
	if(CityName=="bupt.city"){
		LTT->DrawTextTure();
	}
	if(IsRender==TRUE)
		PowerRender(); 	

	free(LTT);
}

void DrawEv::DrawDxf(){
	byte indices[4];
	glLineWidth(1.0f);
	int n= DxfVertex.GetCount()/3;
	for(int i=0;i<n;){
		for(int j=0;j<4;j++){
			indices[j]=j;
			fVertices[j*3]= DxfVertex[i*3];
			fVertices[j*3+1]= DxfVertex[i*3+1];
			fVertices[j*3+2]= DxfVertex[i*3+2];
			i++;
		}
		glVertexPointer(3,GL_FLOAT,0,fVertices);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glColor3f(1.0f,0.04f,0.8f);
		glDrawElements(GL_QUADS,4,GL_UNSIGNED_BYTE,indices);
		glPolygonMode(GL_BACK,GL_FILL);
		glColor3f(0.64f,0.64f,0.64f);
		glDrawElements(GL_QUADS,4,GL_UNSIGNED_BYTE,indices);
	}
}

void DrawEv::DrawGround(){
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(20000.0f,  20000.0f,-1.0f);
	glVertex3f(20000.0f, -20000.0f,-1.0f);
	glVertex3f(-20000.0f,-20000.0f,-1.0f);
	glVertex3f(-20000.0f, 20000.0f,-1.0f);
	glEnd();
}

void DrawEv::fangge(){
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,0.0f);
	glVertex3f(-155,-155,0);
	glVertex3f(-155,-145,0);
	glVertex3f(155,-145,0);
	glVertex3f(155,-155,0);
	glEnd();
	for(GLfloat i=-155.0f;i<=155.0;i+=10){
		glBegin(GL_LINES);
		glColor3f(0.0f,0.0f,0.0f);
		glVertex3f(-155.0f,i,0.0f);
		glVertex3f(155.0f,i,0.0f);
		glEnd();
	}
	for(GLfloat i=-155;i<=155;i+=10){
		glBegin(GL_LINES);
		glColor3f(0.0f,0.0f,0.0f);
		glVertex3f(i,-155.0,0.0f);
		glVertex3f(i,155.0,0.0f);
		glEnd();
	}
}
//////////////////////////////////////////////////////////////////////////
////////////////////////////能量渲染//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void DrawEv::PowerRender(){
	GLfloat coor=200.0f;
	glShadeModel(GL_SMOOTH);
	glBegin(GL_QUADS);
	glColor3ub((GLubyte)255,(GLubyte)0,(GLubyte)0);
	glVertex3f(-coor-100.0f,-coor,0.0f);
	glColor3ub((GLubyte)255,(GLubyte)0,(GLubyte)255);
	glVertex3f(-coor-100.0f,coor,0.0f);
	glColor3ub((GLubyte)0,(GLubyte)255,(GLubyte)0);
	glVertex3f(0.0f-100.0f,coor,0.0f);
	glColor3ub((GLubyte)0,(GLubyte)0,(GLubyte)255);
	glVertex3f(0.0f-100.0f,-coor,0.0f);
	glEnd();
}
void DrawEv::RenderOutput(){
	byte indices[4]={0,1,2,3};
	int i,j;
	for(i=0;i<RenderArray.GetCount();i+=15){
		for(j=0;j<12;j++){
			fVertices[j]=RenderArray[i+j];
		}
		glVertexPointer(3,GL_FLOAT,0,fVertices);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glColor3f(RenderArray[i+j],RenderArray[i+j+1],RenderArray[i+j+2]);
		glDrawElements(GL_QUADS,4,GL_UNSIGNED_BYTE,indices);
		j=0;
	}		
}
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void DrawEv::DrawCube(){//绘制收发信机
	glPointSize(5);
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_POINTS);
		for(int i=0;i<Point_TxInfo.GetCount();i++)
			glVertex3f(Point_TxInfo[i].Pos.x,Point_TxInfo[i].Pos.y,Point_TxInfo[i].Pos.z);
		for(int i=0;i<Polygon_TxInfo.GetCount();i++)
			glVertex3f(Polygon_TxInfo[i].Pos.x,Polygon_TxInfo[i].Pos.y,Polygon_TxInfo[i].Pos.z);
		
	glEnd();
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_POINTS);
		for(int i=0;i<Point_RxInfo.GetCount();i++)
			glVertex3f(Point_RxInfo[i].Pos.x,Point_RxInfo[i].Pos.y,Point_RxInfo[i].Pos.z);
		if(1!=isRender){
			for(int i=0;i<Polygon_RxInfo.GetCount();i++)
				glVertex3f(Polygon_RxInfo[i].Pos.x,Polygon_RxInfo[i].Pos.y,Polygon_RxInfo[i].Pos.z);
		}
	glEnd();
}
 
void DrawEv::DrawPath(){
	//单发单收,单发多收去遍历函数

	glLineWidth(3.0f);
	
	if(!cviewdlg.SeeFlag){
		glColor3f(1.0f,0.0f,0.0f);//直射
		glBegin(GL_LINES);
			glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
			glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
		glEnd();
	}
	if(0!=OneReflection.GetCount()){
		glColor3f(1.0f,0.4f,0.0f);//一介反射
		for(int i=0;i<OneReflection.GetCount()/3;i++){
			glBegin(GL_LINES);
				glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
				glVertex3f(OneReflection[i*3],OneReflection[i*3+1],OneReflection[i*3+2]);
				glVertex3f(OneReflection[i*3],OneReflection[i*3+1],OneReflection[i*3+2]);
				glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
			glEnd();
		}
	}
	if(0!=TwoReflection.GetCount()){
		glColor3f(0.65f,0.3f,0.8f);//二阶反射
		for(int i=0;i<TwoReflection.GetCount()/6;i++){
			glBegin(GL_LINES);
				glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
				glVertex3f(TwoReflection[i*6],TwoReflection[i*6+1],TwoReflection[i*6+2]);
				glVertex3f(TwoReflection[i*6],TwoReflection[i*6+1],TwoReflection[i*6+2]);
				glVertex3f(TwoReflection[i*6+3],TwoReflection[i*6+4],TwoReflection[i*6+5]);
				glVertex3f(TwoReflection[i*6+3],TwoReflection[i*6+4],TwoReflection[i*6+5]);
				glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
			glEnd();
		}
	}
	if(0!=OneDiffraction.GetCount()){
		glColor3f(1.0f,0.0f,0.5f);//一介绕射
		for(int i=0;i<OneDiffraction.GetCount()/3;i++){
			glBegin(GL_LINES);
				glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
				glVertex3f(OneDiffraction[i*3],OneDiffraction[i*3+1],OneDiffraction[i*3+2]);
				glVertex3f(OneDiffraction[i*3],OneDiffraction[i*3+1],OneDiffraction[i*3+2]);
				glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
			glEnd();
		}
	}
	if(0!=TwoDiffraction.GetCount()){
		glColor3f(0.5f,0.0f,0.25f);//二阶绕射
		for(int i=0;i<TwoDiffraction.GetCount()/6;i++){
			glBegin(GL_LINES);
				glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
				glVertex3f(TwoDiffraction[i*6],TwoDiffraction[i*6+1],TwoDiffraction[i*6+2]);
				glVertex3f(TwoDiffraction[i*6],TwoDiffraction[i*6+1],TwoDiffraction[i*6+2]);
				glVertex3f(TwoDiffraction[i*6+3],TwoDiffraction[i*6+4],TwoDiffraction[i*6+5]);
				glVertex3f(TwoDiffraction[i*6+3],TwoDiffraction[i*6+4],TwoDiffraction[i*6+5]);
				glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
			glEnd();
		}
	}
	if(0!=ReflectionToDiffraction.GetCount()){
		glColor3f(0.5f,0.5f,1.0f);//一反一绕
		for(int i=0;i<ReflectionToDiffraction.GetCount()/6;i++){
			glBegin(GL_LINES);
				glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
				glVertex3f(ReflectionToDiffraction[i*6],ReflectionToDiffraction[i*6+1],ReflectionToDiffraction[i*6+2]);
				glVertex3f(ReflectionToDiffraction[i*6],ReflectionToDiffraction[i*6+1],ReflectionToDiffraction[i*6+2]);
				glVertex3f(ReflectionToDiffraction[i*6+3],ReflectionToDiffraction[i*6+4],ReflectionToDiffraction[i*6+5]);
				glVertex3f(ReflectionToDiffraction[i*6+3],ReflectionToDiffraction[i*6+4],ReflectionToDiffraction[i*6+5]);
				glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
			glEnd();
		}
	}
	if(0!=DiffractionToReflection.GetCount()){
		glColor3f(0.5f,0.0f,0.0f);//一绕一反
		for(int i=0;i<DiffractionToReflection.GetCount()/6;i++){
			glBegin(GL_LINES);
			glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
			glVertex3f(DiffractionToReflection[i*6],DiffractionToReflection[i*6+1],DiffractionToReflection[i*6+2]);
			glVertex3f(DiffractionToReflection[i*6],DiffractionToReflection[i*6+1],DiffractionToReflection[i*6+2]);
			glVertex3f(DiffractionToReflection[i*6+3],DiffractionToReflection[i*6+4],DiffractionToReflection[i*6+5]);
			glVertex3f(DiffractionToReflection[i*6+3],DiffractionToReflection[i*6+4],DiffractionToReflection[i*6+5]);
			glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
			glEnd();
		}
	}
	if(0!=TwoReflectionToDiffraction.GetCount()){
		glColor3f(0.25f,0.0f,0.25f);
		for(int i=0;i<TwoReflectionToDiffraction.GetCount()/9;i++){
			glBegin(GL_LINES);//两反一绕
			glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
			glVertex3f(TwoReflectionToDiffraction[i*9],TwoReflectionToDiffraction[i*9+1],TwoReflectionToDiffraction[i*9+2]);
			glVertex3f(TwoReflectionToDiffraction[i*9],TwoReflectionToDiffraction[i*9+1],TwoReflectionToDiffraction[i*9+2]);
			glVertex3f(TwoReflectionToDiffraction[i*9],TwoReflectionToDiffraction[i*9+1],TwoReflectionToDiffraction[i*9+2]);
			glVertex3f(TwoReflectionToDiffraction[i*9],TwoReflectionToDiffraction[i*9+1],TwoReflectionToDiffraction[i*9+2]);
			glVertex3f(TwoReflectionToDiffraction[i*9+3],TwoReflectionToDiffraction[i*9+4],TwoReflectionToDiffraction[i*9+5]);
			glVertex3f(TwoReflectionToDiffraction[i*9+3],TwoReflectionToDiffraction[i*9+4],TwoReflectionToDiffraction[i*9+5]);
			glVertex3f(TwoReflectionToDiffraction[i*9+6],TwoReflectionToDiffraction[i*9+7],TwoReflectionToDiffraction[i*9+8]);
			glVertex3f(TwoReflectionToDiffraction[i*9+6],TwoReflectionToDiffraction[i*9+7],TwoReflectionToDiffraction[i*9+8]);
			glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
			glEnd();
		}
	}
	if(0!=ReflectionToTwoDiffraction.GetCount()){
		glColor3f(0.25f,0.0f,0.25f);
		for(int i=0;i<ReflectionToTwoDiffraction.GetCount()/9;i++){
			glBegin(GL_LINES);//一反两绕
			glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
			glVertex3f(ReflectionToTwoDiffraction[i*9],ReflectionToTwoDiffraction[i*9+1],ReflectionToTwoDiffraction[i*9+2]);
			glVertex3f(ReflectionToTwoDiffraction[i*9],ReflectionToTwoDiffraction[i*9+1],ReflectionToTwoDiffraction[i*9+2]);
			glVertex3f(ReflectionToTwoDiffraction[i*9+3],ReflectionToTwoDiffraction[i*9+4],ReflectionToTwoDiffraction[i*9+5]);
			glVertex3f(ReflectionToTwoDiffraction[i*9+3],ReflectionToTwoDiffraction[i*9+4],ReflectionToTwoDiffraction[i*9+5]);
			glVertex3f(ReflectionToTwoDiffraction[i*9+6],ReflectionToTwoDiffraction[i*9+7],ReflectionToTwoDiffraction[i*9+8]);
			glVertex3f(ReflectionToTwoDiffraction[i*9+6],ReflectionToTwoDiffraction[i*9+7],ReflectionToTwoDiffraction[i*9+8]);
			glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
			glEnd();
		}
	}
	if(0!=TwoDiffractionToReflection.GetCount()){
		glColor3f(0.0f,0.5f,0.0f);
		for(int i=0;i<TwoDiffractionToReflection.GetCount()/9;i++){
			glBegin(GL_LINES);//一绕两反
			glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
			glVertex3f(DiffractionToTwoReflection[i*9],DiffractionToTwoReflection[i*9+1],DiffractionToTwoReflection[i*9+2]);
			glVertex3f(DiffractionToTwoReflection[i*9],DiffractionToTwoReflection[i*9+1],DiffractionToTwoReflection[i*9+2]);
			glVertex3f(DiffractionToTwoReflection[i*9+3],DiffractionToTwoReflection[i*9+4],DiffractionToTwoReflection[i*9+5]);
			glVertex3f(DiffractionToTwoReflection[i*9+3],DiffractionToTwoReflection[i*9+4],DiffractionToTwoReflection[i*9+5]);
			glVertex3f(DiffractionToTwoReflection[i*9+6],DiffractionToTwoReflection[i*9+7],DiffractionToTwoReflection[i*9+8]);
			glVertex3f(DiffractionToTwoReflection[i*9+6],DiffractionToTwoReflection[i*9+7],DiffractionToTwoReflection[i*9+8]);
			glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
			glEnd();
		}
	}
	if(0!=TwoDiffractionToReflection.GetCount()){
		glColor3f(0.0f,0.5f,0.0f);
		for(int i=0;i<TwoDiffractionToReflection.GetCount()/9;i++){
			glBegin(GL_LINES);//两绕一反
			glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
			glVertex3f(TwoDiffractionToReflection[i*9],TwoDiffractionToReflection[i*9+1],TwoDiffractionToReflection[i*9+2]);
			glVertex3f(TwoDiffractionToReflection[i*9],TwoDiffractionToReflection[i*9+1],TwoDiffractionToReflection[i*9+2]);
			glVertex3f(TwoDiffractionToReflection[i*9+3],TwoDiffractionToReflection[i*9+4],TwoDiffractionToReflection[i*9+5]);
			glVertex3f(TwoDiffractionToReflection[i*9+3],TwoDiffractionToReflection[i*9+4],TwoDiffractionToReflection[i*9+5]);
			glVertex3f(TwoDiffractionToReflection[i*9+6],TwoDiffractionToReflection[i*9+7],TwoDiffractionToReflection[i*9+8]);
			glVertex3f(TwoDiffractionToReflection[i*9+6],TwoDiffractionToReflection[i*9+7],TwoDiffractionToReflection[i*9+8]);
			glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
			glEnd();
		}
	}
	if(0!=TwoReflectionToTwoDiffraction.GetCount()){
		glColor3f(0.0f,0.5f,0.0f);
		for(int i=0;i<TwoReflectionToTwoDiffraction.GetCount()/12;i++){
			glBegin(GL_LINES);//两绕两反
			glVertex3f(Point_TxInfo[0].Pos.x,Point_TxInfo[0].Pos.y,Point_TxInfo[0].Pos.z);
			glVertex3f(TwoReflectionToTwoDiffraction[i*12],TwoReflectionToTwoDiffraction[i*12+1],TwoReflectionToTwoDiffraction[i*12+2]);
			glVertex3f(TwoReflectionToTwoDiffraction[i*12],TwoReflectionToTwoDiffraction[i*12+1],TwoReflectionToTwoDiffraction[i*12+2]);
			glVertex3f(TwoReflectionToTwoDiffraction[i*12+3],TwoReflectionToTwoDiffraction[i*12+4],TwoReflectionToTwoDiffraction[i*12+5]);
			glVertex3f(TwoReflectionToTwoDiffraction[i*12+3],TwoReflectionToTwoDiffraction[i*12+4],TwoReflectionToTwoDiffraction[i*12+5]);
			glVertex3f(TwoReflectionToTwoDiffraction[i*12+6],TwoReflectionToTwoDiffraction[i*12+7],TwoReflectionToTwoDiffraction[i*12+8]);
			glVertex3f(TwoReflectionToTwoDiffraction[i*12+6],TwoReflectionToTwoDiffraction[i*12+7],TwoReflectionToTwoDiffraction[i*12+8]);
			glVertex3f(TwoReflectionToTwoDiffraction[i*12+9],TwoReflectionToTwoDiffraction[i*12+10],TwoReflectionToTwoDiffraction[i*12+11]);
			glVertex3f(TwoReflectionToTwoDiffraction[i*12+9],TwoReflectionToTwoDiffraction[i*12+10],TwoReflectionToTwoDiffraction[i*12+11]);
			glVertex3f(Point_RxInfo[0].Pos.x,Point_RxInfo[0].Pos.y,Point_RxInfo[0].Pos.z);
			glEnd();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void DrawEv::DrawTinPoint(){
	if(T1.GetCount()!=0){
		glPointSize(5);
		glColor3f(0.9f,0.1f,0.8f);
		glBegin(GL_POINTS);
		glEnable(GL_POINT_SMOOTH);
		for(int i=0;i<T1.GetCount();i++)
			glVertex3f(T1[i].x,T1[i].y,T1[i].z);
		glEnd();
	}
}
void DrawEv::DrawTIN(){
	if(T3.GetCount()!=0){
		glColor3f(0.0f,1.0f,0.0f);
		glLineWidth(1.5f);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		for(int i=0;i<T3.GetCount();i++){
			glBegin(GL_LINE_LOOP);
			glVertex3f(T3[i].P1.x,T3[i].P1.y,T3[i].P1.z);
			glVertex3f(T3[i].P2.x,T3[i].P2.y,T3[i].P2.z);
			glVertex3f(T3[i].P3.x,T3[i].P3.y,T3[i].P3.z);
			glEnd();
		}
	}
}

void DrawEv::DrawRect(){//绘制圈定多发射机或多接收机的虚线框
	if(SetPointOnTin==1&&GLBoundaryPoint.GetCount()==1){
		glColor3f(0.0f,0.0f,1.0f);
		glPointSize(10);
		glBegin(GL_POINTS);
			glVertex3f((GLfloat)GLBoundaryPoint[0].x,(GLfloat)GLBoundaryPoint[0].y,(GLfloat)GLBoundaryPoint[0].z+0.2);
			glVertex3f((GLfloat)ResultPos.x,(GLfloat)ResultPos.y,(GLfloat)ResultPos.z+0.2);
		glEnd();

		glColor3f(0.5,0.5,0.5);
		glLineWidth(1.0f);
		glBegin(GL_LINE_LOOP);
			glVertex3f((GLfloat)GLBoundaryPoint[0].x,(GLfloat)GLBoundaryPoint[0].y,100.0);
			glVertex3f((GLfloat)ResultPos.x,(GLfloat)GLBoundaryPoint[0].y,100.0);
			glVertex3f((GLfloat)ResultPos.x,(GLfloat)ResultPos.y,(GLfloat)ResultPos.z);
			glVertex3f((GLfloat)GLBoundaryPoint[0].x,(GLfloat)ResultPos.y,100.0);
			glVertex3f((GLfloat)GLBoundaryPoint[0].x,(GLfloat)GLBoundaryPoint[0].y,100.0);
		glEnd();
	}

}