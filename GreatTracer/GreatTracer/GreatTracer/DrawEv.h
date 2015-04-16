#pragma once

#include "stdafx.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glaux.h"
#include "deelx.h"
#include "ScaleVariable.h"
#include "VIEWDLG.h"

extern CArray <RxInfo> Polygon_RxInfo;//����εĶ���ջ�����
extern CArray <RxInfo> Point_RxInfo;//����ջ���Ϣ����
extern CArray <TxInfo> Point_TxInfo;//�㷢����Ķ෢�������
extern CArray <TxInfo> Polygon_TxInfo;//����εĶ෢�������
extern CArray <AntennaInfo> AnteInfo;//��������
extern CArray <ModualInfo> ModualWay;//���Է�ʽ����

extern CArray <CurPos3D> T1;
extern CArray<line> T2;
extern CArray<Triangle> T3;

extern CArray <GLfloat> CubeVertex_Tx;//�շ��Ż���С����������
extern CArray <GLfloat> CubeVertex_Rx;
extern CArray <GLfloat> RenderArray;//�����Ⱦ��Ϣ������

extern CArray <GLfloat> OneReflection;//һ�׷���
extern CArray <GLfloat> TwoReflection;//���׷���
extern CArray <GLfloat> OneDiffraction;//һ������
extern CArray <GLfloat> TwoDiffraction;//��������
extern CArray <GLfloat> ReflectionToDiffraction;//һ��һ��
extern CArray <GLfloat> DiffractionToReflection;//һ��һ��
extern CArray <GLfloat> TwoReflectionToDiffraction;//����һ��
extern CArray <GLfloat> ReflectionToTwoDiffraction;//һ������
extern CArray <GLfloat> TwoDiffractionToReflection;//����һ��
extern CArray <GLfloat> DiffractionToTwoReflection;//һ������
extern CArray <GLfloat> TwoReflectionToTwoDiffraction;//��������

extern GLfloat MapCenterX,MapCenterY,MapCenterZ;

extern CArray <Vec3T> GLBoundaryPoint;
extern int BoundaryFlag;
extern int SetPointOnTin;
extern Vec3T ResultPos;

class DrawEv
{
public:
	DrawEv(void);//���캯��
	~DrawEv(void);
	//��ȡdxf��city�ļ�
	void ReadDxf(CString path);
	void ReadCity(CString path);
	//����dxf��city��ͼ
	void DrawDxf();
	void DrawCity(CString CityName,bool IsRender);

	void DrawGround();	//���Ƶ���
	void DrawCube();	//�����շ��Ż�
	void DrawPath();	//��������·��
	void PowerRender();//������Ⱦ��ʽ������ʾ��
	
	void RenderOutput();	//��Ⱦ���
	void DrawTinPoint();//��TIN�ĵ�
	void DrawTIN();//��������
private:
	CStringArray tempCityVertex;//.city��ͼ�������ʱ�������
	CStringArray  tempDxfVertex;//.dxf��ͼ�������ʱ�������
	CStringArray MultiVertexPanel;//3DFACE����εĶ���Ĵ������
	int find_remark(CString str);//ƥ���ѯ
	int GetVertexNum(CString str);//��ȡ����ζ������
	void decodeCString(CString source,CStringArray &CityVertex,char division);//����������д������
public:
	CArray <GLfloat> CityVertex;//*.city��ͼ�е��ı��ζ�������
	CArray <GLfloat> DxfVertex;//3DFACE�Ķ���������
	CArray <INT32> MultiVertexNum;//����εĶ�����
	CArray <GLfloat> PolygonVertex;//����εĶ�������
	GLuint	texture[1];		// ������ͼ����
	void DrawRect();

};
