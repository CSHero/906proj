#pragma once

#pragma comment( lib, "glaux.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "OpenGL32.lib" )

#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glaux.h"
#include "resource.h"
#include "ScaleVariable.h"
#include "vector"
#include "TIN.h"
#include "ArcBall.h"
#include "DrawEv.h"
#include "TX_PROPER_DIALOG.h"
#include "RX_DIALOG.h"
#include "ANTE_DIALOG.h"
#include "POLYGON_TX_DIALOG.h"
#include "POLYGON_RX_DIALOG.h"
#include "ReadandWrite.h"
#include "RawToTIN.h"


extern CArray <GLfloat> CubeVertex_Tx;//�շ��Ż���С����������
extern CArray <GLfloat> CubeVertex_Rx;
extern CArray <GLfloat> RenderArray;
extern GLfloat space;//���ջ���space
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

extern CArray <Vec3T> GLBoundaryPoint;//����ջ��ı߽���OpenGL�е�����
extern CArray <CPoint> ScrBoundaryPoint;//����ջ��ı߽�����Ļ�ϵ�����

extern CArray <RxInfo> Polygon_RxInfo;//���ջ���Ϣ
extern CArray <RxInfo> Point_RxInfo;
extern CArray <TxInfo> Point_TxInfo;//�������Ϣ
extern CArray <TxInfo> Polygon_TxInfo;
extern CArray <AntennaInfo> AnteInfo;//������Ϣ
extern CArray <ModualInfo> ModualWay;//���Է�ʽ��Ϣ
extern CArray<CurPos3D>T1;//���TIN�ĵ�
extern CArray<line>T2;//���TIN����
extern CArray<Triangle>T3;//���TIN��������

extern GLfloat MapCenterX,MapCenterY,MapcenterZ;//��ͼ����

extern int isRender;//�Ƿ���Ⱦ
// CVIEWDLG �Ի���

class CVIEWDLG : public CDialog
{
	DECLARE_DYNAMIC(CVIEWDLG)

public:
	CVIEWDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVIEWDLG();
	//TX or RX Point Convert to a cube with 8 Points
	void Deal_Tx_Vertex_Cube(TxInfo Temp);
	void Deal_Rx_Vertex_Cube( RxInfo Temp);
	void AddEle();//������������Ԫ��
	CurPos3D PosInGL;//��һ��float���͵�OpenGL������
	BOOL SeeFlag;//�Ƿ�ͨ��
	void MagicSort(CArray <RxInfo> &aref,int low,int high);////����
	int Partition(CArray<RxInfo> &aref,int i,int j);
	void PowerToColor(CArray <RxInfo> &AREF);//�ɽ��չ����ж���Ӧ��ɫ
	void Deal_Render_Quads(CurPos3D temp);//������Ⱦ��С�ı���
	void GetCoor_TIN(GLfloat tempx,GLfloat tempy);
private:
	BOOL SetWindowPixelFormat(HDC hDC);//�趨���ظ�ʽ
	BOOL CreateViewGLContext(HDC hDC);//View GL Context
	void RenderScene();//���Ƴ���
	void ReshapeSize();//�����ӿ�
	void Opengl_light();//���տ���
	void GetSize();//��ȡ�ؼ��ߴ�
	void SetupRC();//��ʼ������
	void GetImg();
	void FPSrate();//��ȡ֡����
	void GetCoor(CPoint point);//��ȡ�����ģ���е�λ��

//////////////////opengl����������ر���////////////////////////////////////////////////////////

	HDC hrenderDC;//DC
	HGLRC hrenderRC;//RC
	int PixelFormat;
	CRect cr_Size;//�ؼ��ߴ�洢

////////////////////ָʾ����//////////////////////////////////////////////////////

	POINT cur_pos,last_pos;//��¼���λ��
	float m_xScaling,m_yScaling,m_zScaling;//��������
	CArray <CString> Vertex;//�洢��ͼ����λ��
	CString Dxfpath,Citypath,CityName,DxfName,Imgpath;//�ļ������ļ�λ��

	int InitFlag;//��ʼ��ָʾ��־
	int  IsButtonLeftDown;//������Ҽ�ָʾ
	int IsButtonRightDown;//����Ҽ��Ƿ���
	bool TxPointFlag,TxPolygonFlag,RxPointFlag,RxPolygonFlag;//�շ��Ż�����ģʽָʾ
	bool IsRender,IsPathAvailable,IsRenderOutput;//RenderFlag & PathFlag & �Ƿ�ͨ��;
	int PointVision;//�����ά��
	int TinFlag;//�Ƿ�TIN
	int IsSetTexTure;//�Ƿ�ΪTIN��ͼ
	int israwtotin;//�Ƿ�rawtotin

// �Ի�������
	enum { IDD = IDD_VIEW_DIALOG };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();//��ʼ����������
public:
	//////////////////////////////////////////////////////////////////////////
	//������Ҫ��һЩ�Ի������Ϣ��������ť��������
	//////////////////////////////////////////////////////////////////////////
	afx_msg void OnPaint();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFileDxf();
	afx_msg void OnFileCity();
	afx_msg void OnViewRender();
	afx_msg void OnViewNormal();
	afx_msg void OnTransmmitsPoints();
	afx_msg void OnReceiversPoints();
	afx_msg void OnTransmmitsPolygons();
	afx_msg void OnReceiversPolygons();
	afx_msg void OnClose();
	afx_msg void OnOutputPowerpath();
	afx_msg void OnSetAntenna();
	afx_msg void OnOutputRenderoutput();
	afx_msg void OnClearPolygontx();
	afx_msg void OnClearPolygonrx();
	afx_msg void OnExamplePathexample();
	afx_msg void OnExampleRenderexample();
	afx_msg void OnEditTin();
	afx_msg void OnEditModeltin();
	afx_msg void OnClearTin();
	afx_msg void OnClearPath();
	afx_msg void OnClearPointtx();
	afx_msg void OnClearPointrx();
	afx_msg void OnClearRender();
	afx_msg void OnFileTin();
	afx_msg void OnTestTest1();
	afx_msg void OnView32819();
	afx_msg void OnEditTin32820();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	BOOL  PreTranslateMessage(MSG*   pMsg);   
	afx_msg void OnFileRaw();
};