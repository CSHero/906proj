// VIEWDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "stdlib.h"
#include "time.h"
#include "GreatTracer.h"
#include "VIEWDLG.h"


ArcBallT arcBall(800,500);
ArcBallT *ArcBall =&arcBall;//ʵ�����켣����

DrawEv de; //ʵ������ȡ�ļ���
TIN *TinPointer;

AntennaInfo Ante_Info;
ModualInfo Modual_Info;


//���ź��ƶ���ʼ��
static float m_xScaling=1.0f;
static float m_yScaling=1.0f;
static float m_zScaling=1.0f;
static GLfloat X_Translating=0.0f;
static GLfloat Y_Translating=0.0f;

int EditMode=0;//�༭ģʽָʾ
int SetPointOnTin;//�Ƿ�����TIN�����õ��շ��Ż�
int BoundaryFlag;//ָʾ������Ƿ��Ѿ�Ȧ�����ĸ���
Vec3T ResultPos;//Opengl�е�����
CRect m_rect;//��ȡ���ڴ�С
Vec3T OrginPoint;//��һ������ƹ�������λ��
GLfloat Transfer_Z;//������Zֵ���ݸ��շ��Ż����öԻ���

int israwtotin=0;//�Ƿ���raw����tin�ı�ʶ 
BYTE g_HeightMap[1024*1024];//�洢raw�������

HCURSOR hCur=LoadCursor(NULL,IDC_CROSS);//���ģʽָʾ

TX_PROPER_DIALOG *TPD;//��������ԶԻ���
RX_DIALOG *RD;//���ջ����ԶԻ���
ANTE_DIALOG *AnteDlg;//�������ԶԻ���
POLYGON_TX_DIALOG *PolyTxDlg;//�෢����Ի���
POLYGON_RX_DIALOG *PolyRxDlg;//����ջ��Ի���
ReadandWrite *RW;
CRawToTIN CRT;

// CVIEWDLG �Ի���

IMPLEMENT_DYNAMIC(CVIEWDLG, CDialog)

CVIEWDLG::CVIEWDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CVIEWDLG::IDD, pParent)
{

}

CVIEWDLG::~CVIEWDLG()
{
}

void CVIEWDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CVIEWDLG::OnInitDialog(){
	CDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	IsPathAvailable=false;
	IsRenderOutput=false;
	IsRender=false;

	CMenu m_Menu;
	m_Menu.LoadMenu(IDR_VIEW_MENU);
	SetMenu(&m_Menu);
	CWnd *wnd=GetDlgItem(IDC_RENDER);
	hrenderDC=::GetDC(wnd->m_hWnd);
	if(SetWindowPixelFormat(hrenderDC)==FALSE)
		return 0;
	if(CreateViewGLContext(hrenderDC)==FALSE)
		return 0;
	//if(EditMode==0)
		ReshapeSize();
	/////////////////////////////////////////////////////////////////////////
	SetTimer(1,50,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BEGIN_MESSAGE_MAP(CVIEWDLG, CDialog)
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_FILE_DXF, &CVIEWDLG::OnFileDxf)
	ON_COMMAND(ID_FILE_CITY, &CVIEWDLG::OnFileCity)
	ON_COMMAND(ID_VIEW_RENDER, &CVIEWDLG::OnViewRender)
	ON_COMMAND(ID_VIEW_NORMAL, &CVIEWDLG::OnViewNormal)
	ON_COMMAND(ID_TRANSMMITS_POINTS, &CVIEWDLG::OnTransmmitsPoints)
	ON_COMMAND(ID_RECEIVERS_POINTS, &CVIEWDLG::OnReceiversPoints)
	ON_COMMAND(ID_TRANSMMITS_POLYGONS, &CVIEWDLG::OnTransmmitsPolygons)
	ON_COMMAND(ID_RECEIVERS_POLYGONS, &CVIEWDLG::OnReceiversPolygons)
	ON_WM_CLOSE()
	ON_COMMAND(ID_OUTPUT_POWERPATH, &CVIEWDLG::OnOutputPowerpath)
	ON_COMMAND(ID_SET_ANTENNA, &CVIEWDLG::OnSetAntenna)
	ON_COMMAND(ID_OUTPUT_RENDEROUTPUT, &CVIEWDLG::OnOutputRenderoutput)
	ON_COMMAND(ID_CLEAR_POLYGONTX, &CVIEWDLG::OnClearPolygontx)
	ON_COMMAND(ID_CLEAR_POLYGONRX, &CVIEWDLG::OnClearPolygonrx)
	ON_COMMAND(ID_EXAMPLE_PATHEXAMPLE, &CVIEWDLG::OnExamplePathexample)
	ON_COMMAND(ID_EXAMPLE_RENDEREXAMPLE, &CVIEWDLG::OnExampleRenderexample)
	ON_COMMAND(ID_EDIT_TIN, &CVIEWDLG::OnEditTin)
	ON_COMMAND(ID_EDIT_MODELTIN, &CVIEWDLG::OnEditModeltin)
	ON_COMMAND(ID_CLEAR_TIN, &CVIEWDLG::OnClearTin)
	ON_COMMAND(ID_CLEAR_PATH, &CVIEWDLG::OnClearPath)
	ON_COMMAND(ID_CLEAR_POINTTX, &CVIEWDLG::OnClearPointtx)
	ON_COMMAND(ID_CLEAR_POINTRX, &CVIEWDLG::OnClearPointrx)
	ON_COMMAND(ID_CLEAR_RENDER, &CVIEWDLG::OnClearRender)
	ON_COMMAND(ID_FILE_TIN, &CVIEWDLG::OnFileTin)
	ON_COMMAND(ID_TEST_TEST1, &CVIEWDLG::OnTestTest1)
	ON_COMMAND(ID_VIEW_32819, &CVIEWDLG::OnView32819)
	ON_COMMAND(ID_EDIT_TIN32820, &CVIEWDLG::OnEditTin32820)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_DEM, &CVIEWDLG::OnFileDem)
END_MESSAGE_MAP()


// CVIEWDLG ��Ϣ�������
GLfloat light_position0[]={-108,0,180,0.0f};
GLfloat light_specular0[]={0,0,0,1};
GLfloat light_diffuse[]={1.0f,1.0f,1.0f,0.0f};
void CVIEWDLG::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(israwtotin)
		glClearColor(0.0,0.0,0.0,0.0);
	//��ʱ�����˹���
	//glLightfv(GL_LIGHT0,GL_POSITION,light_position0);
	//glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular0);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
	//this->Opengl_light();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(X_Translating,Y_Translating,0.0f);
	//glTranslatef(-MapCenterX+X_Translating,-MapCenterY+Y_Translating,0.0f);
	glScalef(ArcBall->zoomRate, ArcBall->zoomRate, ArcBall->zoomRate);//Scale
	glScalef(1.0, 4.0, 1.0);//�˴�������Ϊ���Ӿ�Ч����û�й̶�����
	if(EditMode==0||israwtotin)//�ڷ������շ��Ż���������������ת����rawtotinʱ�������շ��Ż�ʱҲ������ת
		glMultMatrixf(ArcBall->Transform.M);     //Rotate
	glPushMatrix();
	RenderScene();
	glPopMatrix();
	SwapBuffers(hrenderDC);
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}
//////////////////////////////////////////////////////////////////////////
//////////////////////��ͼ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CVIEWDLG::RenderScene(){
	if(1==InitFlag){
		de.DrawDxf();}
	if (2==InitFlag){
		de.DrawCity(CityName,IsRender);}
	if(IsPathAvailable){
		de.DrawPath();}
	if(IsRenderOutput)
		de.RenderOutput();
	if(1==PointVision)
		de.DrawTinPoint();
	if(1==TinFlag)
		de.DrawTIN();
	if(1==IsSetTexTure)
		TinPointer->SetTextTure();
	if(SetPointOnTin==0&&EditMode!=0)
		de.DrawGround();
	if(israwtotin)
		CRT.RenderRawTIN(g_HeightMap);
	de.DrawCube();
	de.DrawRect();

}
void  CVIEWDLG::ReshapeSize(){
	float range=100.0f;
	GetSize();
	if(cr_Size.Height()==0){
		CWnd *tempWnd=GetDlgItem(IDC_RENDER);
		tempWnd->SetWindowPos(NULL,0,0,200,200,SWP_NOMOVE);
		tempWnd->ShowWindow(SW_SHOW);
		tempWnd->UpdateWindow();
	}
	glViewport(0,0,cr_Size.Width(),cr_Size.Height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(cr_Size.Width()<cr_Size.Height())
		glOrtho(0.0f,0.0f,range*cr_Size.Width()/cr_Size.Height(),-range*cr_Size.Width()/cr_Size.Height(),range,-range);
	else
		glOrtho(-range*cr_Size.Height()/cr_Size.Width(),range*cr_Size.Height()/cr_Size.Width(),-range,range,-range*2.0f,range*2.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ArcBall->setBounds((GLfloat)cr_Size.Width(),(GLfloat)cr_Size.Height());
	SetupRC();
}
void  CVIEWDLG::SetupRC(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);	
	glEnable(GL_DEPTH_TEST);	
	glEnable(GL_POLYGON_SMOOTH);
	glFrontFace(GL_CCW);		
	glShadeModel(GL_SMOOTH); 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	glClearColor(1.0f, 1.0f, 1.0f, 0.5f); 
	glClearDepth(1.0f); 
	glDepthFunc(GL_LEQUAL); 
}

void CVIEWDLG::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnSysCommand(nID, lParam);
}

void CVIEWDLG::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ArcBall->MousePt.s.X=(GLfloat)point.x;
	ArcBall->MousePt.s.Y=(GLfloat)point.y;

	GetCoor(point);
	CString tempx;
	tempx.Format("%.2f",ResultPos.x);
	CString tempy;
	tempy.Format("%.2f",ResultPos.y);
	CString tempz;
	tempz.Format("%.2f",ResultPos.z);
	Transfer_Z=ResultPos.z+1;//��Zֵ��Ϊ�߶�ֵ���ݸ��շ��Ż��Ի���

	SetDlgItemText(IDC_STATIC_X,tempx);
	SetDlgItemText(IDC_STATIC_Y,tempy);
	SetDlgItemText(IDC_STATIC_Z,tempz);
	if(EditMode==0)
		ArcBall->upstate();
	if(ArcBall->isClicked)
		OnPaint();
	if(0!=EditMode){
		::SetCursor(hCur);
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CVIEWDLG::OnLButtonDown(UINT nFlags, CPoint point){
	CString tempx,tempy;
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect crect;
	CurPos3D tempPoint ;
	GetDlgItem(IDC_RENDER)->GetClientRect(crect);
	GetDlgItem(IDC_RENDER)->ClientToScreen(crect);
	if(crect.PtInRect(point)&&EditMode==0){
		ArcBall->isClicked=TRUE;
		ArcBall->upstate();
	}
	if(EditMode==1){//���õ㷢���
		TxInfo Tx_Info;
		GetDlgItemText(IDC_STATIC_X,tempx);
		GetDlgItemText(IDC_STATIC_Y,tempy);
		Tx_Info.Pos.x=(GLfloat)atof(tempx);
		Tx_Info.Pos.y=(GLfloat)atof(tempy);
		Point_TxInfo.Add(Tx_Info);
		if(!TPD){
			TPD=new TX_PROPER_DIALOG();
			TPD->Create(IDD_TX_DIALOG);
			TPD->ShowWindow(SW_NORMAL);
		}
		else if(TPD&&!TPD->IsWindowVisible()){
			TPD->ShowWindow(SW_NORMAL);
		}
		EditMode=0;
	}
	else if(EditMode==3){//���õ���ջ�
			RxInfo Rx_Info;
			GetDlgItemText(IDC_STATIC_X,tempx);
			GetDlgItemText(IDC_STATIC_Y,tempy);

			Rx_Info.Pos.x=(GLfloat)atof(tempx);
			Rx_Info.Pos.y=(GLfloat)atof(tempy);

			Point_RxInfo.Add(Rx_Info);
			if(!RD){
				RD=new RX_DIALOG();
				RD->Create(IDD_RX_DIALOG);
				RD->ShowWindow(SW_NORMAL);
			}
			else if(RD&&!RD->IsWindowVisible())
				RD->ShowWindow(SW_NORMAL);
			EditMode=0;
	}
	else if(EditMode==2){//���ö෢���
		if(SetPointOnTin==0){
			EditMode=0;
			TxInfo PolyCenter_Point;
			GetDlgItemText(IDC_STATIC_X,tempx);
			GetDlgItemText(IDC_STATIC_Y,tempy);
			PolyCenter_Point.Pos.x=(GLfloat)atof(tempx);
			PolyCenter_Point.Pos.y=(GLfloat)atof(tempy);
			Polygon_TxInfo.Add(PolyCenter_Point);
			if(!PolyTxDlg){
				PolyTxDlg=new POLYGON_TX_DIALOG();
				PolyTxDlg->Create(IDD_POLYGON_TX_DIALOG);
				PolyTxDlg->ShowWindow(SW_NORMAL);
			}
			else if(PolyTxDlg&&!PolyTxDlg->IsWindowVisible())
				PolyTxDlg->ShowWindow(SW_NORMAL);
			EditMode=0;
		}
		else if(SetPointOnTin==1){//Ҫ���������֮�󽫱߽���������Flag����
			ScrBoundaryPoint.Add(point);
			GLBoundaryPoint.Add(ResultPos);

			BoundaryFlag++;

			if(BoundaryFlag==2){
				if(!PolyTxDlg){
					PolyTxDlg=new POLYGON_TX_DIALOG();
					PolyTxDlg->Create(IDD_POLYGON_TX_DIALOG);
					PolyTxDlg->ShowWindow(SW_NORMAL);
				}
				else if(PolyTxDlg&&!PolyTxDlg->IsWindowVisible())
					PolyTxDlg->ShowWindow(SW_NORMAL);
				EditMode=0;
			}
		}
	}
	else if(EditMode==4){//���ö���ջ�
		if(SetPointOnTin==0){
			EditMode=0;
			RxInfo PolyCenter_Point;
			GetDlgItemText(IDC_STATIC_X,tempx);
			GetDlgItemText(IDC_STATIC_Y,tempy);
			PolyCenter_Point.Pos.x=(GLfloat)atof(tempx);
			PolyCenter_Point.Pos.y=(GLfloat)atof(tempy);
			Polygon_RxInfo.Add(PolyCenter_Point);
			if(!PolyRxDlg){
				PolyRxDlg=new POLYGON_RX_DIALOG();
				PolyRxDlg->Create(IDD_POLYGON_RX_DIALOG);
				PolyRxDlg->ShowWindow(SW_NORMAL);
			}
			else if(PolyRxDlg&&!PolyRxDlg->IsWindowVisible()){
				PolyRxDlg->ShowWindow(SW_NORMAL);
			}
			EditMode=0;
		}
		else if(SetPointOnTin==1){

			ScrBoundaryPoint.Add(point);
			GLBoundaryPoint.Add(ResultPos);

			BoundaryFlag++;
			if(BoundaryFlag==2){
				if(!PolyRxDlg){
					PolyRxDlg=new POLYGON_RX_DIALOG();
					PolyRxDlg->Create(IDD_POLYGON_RX_DIALOG);
					PolyRxDlg->ShowWindow(SW_NORMAL);
				}
				else if(PolyRxDlg&&!PolyRxDlg->IsWindowVisible())
					PolyRxDlg->ShowWindow(SW_NORMAL);
				EditMode=0;
			}
			
		}
	}
	else if(EditMode==5){//����TIN�Ķ��
		GetDlgItemText(IDC_STATIC_X,tempx);
		GetDlgItemText(IDC_STATIC_Y,tempy);
		tempPoint.x=(GLfloat)atof(tempx);
		tempPoint.y=(GLfloat)atof(tempy);
		tempPoint.z=5.0f;
		T1.Add(tempPoint);
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CVIEWDLG::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ArcBall->isClicked=FALSE;
	ArcBall->isDragging=FALSE;
	CDialog::OnLButtonUp(nFlags, point);
}

void CVIEWDLG::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	IsButtonRightDown=1;
	CDialog::OnRButtonDown(nFlags, point);

	if(EditMode==0){
		GetCoor(point);
		OrginPoint.x=ResultPos.x;
		OrginPoint.y=ResultPos.y;
	}
	CDialog::OnRButtonDown(nFlags, point);
}

void CVIEWDLG::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	IsButtonRightDown=0;
	CDialog::OnRButtonUp(nFlags, point);
}

BOOL CVIEWDLG::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect crect;
	GetDlgItem(IDC_RENDER)->GetClientRect(crect);
	GetDlgItem(IDC_RENDER)->ClientToScreen(crect);
	if(crect.PtInRect(pt)) { 
		if(zDelta>0) {
			ArcBall->zoomRate+=0.01f;
		}
		else if(ArcBall->zoomRate>=0) { 
			ArcBall->zoomRate-=0.01f;
		}
		InvalidateRect(NULL,FALSE);
	} 
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//////////////////////////////////////////////////////////////////////////
//////////////////���������ģ���е�λ��//////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CVIEWDLG::GetCoor(CPoint point){
	int	viewport[4];
	double	mvmatrix[16];
	double	projmatrix[16];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

	Vec3T winPos={point.x,viewport[3]-point.y-1,0.0};
	float wz = 0;	
	glReadPixels((int)winPos.x, (int)winPos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz); 
	winPos.z = wz;	
	if (winPos.z < 1.0 && winPos.z > 0){
		gluUnProject(winPos.x, winPos.y, winPos.z, mvmatrix, projmatrix, viewport, &ResultPos.x, &ResultPos.y, &ResultPos.z);
	}
}

void CVIEWDLG::GetCoor_TIN(GLfloat tempx,GLfloat tempy){//����һ����һ��
	int	viewport[4];
	double	mvmatrix[16];
	double	projmatrix[16];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

	Vec3T winPos={tempx,viewport[3]-tempy-1,0.0};
	float wz = 0;	
	glReadPixels((int)winPos.x, (int)winPos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz); 
	winPos.z = wz;	
	if (winPos.z < 1.0 && winPos.z > 0){
		gluUnProject(winPos.x, winPos.y, winPos.z, mvmatrix, projmatrix, viewport, &ResultPos.x, &ResultPos.y, &ResultPos.z);
	}
}

void CVIEWDLG::FPSrate(){
	CString temp;
	static float framesPerSecond    = 0.0f;       // This will store our fps
	static float lastTime   = 0.0f;       // This will hold the time from the last frame
	float currentTime = GetTickCount() * 0.001f;    
	++framesPerSecond;
	if( currentTime - lastTime > 1.0f ){
		lastTime = currentTime;
		temp.Format("%.2f",framesPerSecond);
		SetDlgItemText(IDC_STATIC_FPS,CityName+DxfName+"--FPS:"+temp);
		framesPerSecond = 0;
	}
}
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BOOL CVIEWDLG::SetWindowPixelFormat(HDC hDC){
	PIXELFORMATDESCRIPTOR pixelDesc;
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL|
		PFD_DOUBLEBUFFER |
		PFD_TYPE_RGBA;
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 0;
	pixelDesc.cRedShift = 0;
	pixelDesc.cGreenBits = 0;
	pixelDesc.cGreenShift = 0;
	pixelDesc.cBlueBits = 0;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 0;
	pixelDesc.cAccumRedBits = 0;
	pixelDesc.cAccumGreenBits = 0;
	pixelDesc.cAccumBlueBits = 0;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 0;
	pixelDesc.cStencilBits = 1;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	PixelFormat = ChoosePixelFormat(hDC,&pixelDesc);
	if(PixelFormat==0){ // Choose default
		PixelFormat = 1;
		if(DescribePixelFormat(hDC,PixelFormat,
			sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
		{
			return FALSE;
		}
	}
	if(SetPixelFormat(hDC,PixelFormat,&pixelDesc)==FALSE){
		return FALSE;
	}
	return TRUE;
}
void CVIEWDLG::Opengl_light(){
	//if(openlighting=true)	
	//{
		//glPushMatrix();
		GLfloat mat_diffuse[]={1.0f,1.0f,1.f,0.5f};
		GLfloat mat_specular[]={1.0f,1.0f,1.0f,1.0f};
		GLfloat mat_shininess[]={20.0f};
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
		glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
		glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);

		//glClearColor(0.0,0.0,0.0,0.0);
		//glShadeModel(GL_SMOOTH);

		//glPolygonMode(GL_FRONT,GL_FILL);
		//glColor3f(0.0f,0.4f,1.0f);

		//for(int i=0;i<m_triangles.size();i++)
		//{
		//	double dNormal[3];
		//	dNormal[0] = m_triangles[i].Normal[0];
		//	dNormal[1] = m_triangles[i].Normal[1];
		//	dNormal[2] = m_triangles[i].Normal[2];

		//	glBegin(GL_POLYGON);
		//	glNormal3dv(dNormal);         
		//	glVertex3f(m_triangles[i].ptr[0].x, m_triangles[i].ptr[0].y, m_triangles[i].ptr[0].z);
		//	glVertex3f(m_triangles[i].ptr[1].x, m_triangles[i].ptr[1].y, m_triangles[i].ptr[1].z);
		//	glVertex3f(m_triangles[i].ptr[2].x, m_triangles[i].ptr[2].y, m_triangles[i].ptr[2].z);
		//	glEnd();
		//	glFlush();
		//} 

		//glPopMatrix();
	//}
	//else
	//	return;
}
BOOL CVIEWDLG::CreateViewGLContext(HDC hDC){
	hrenderRC = wglCreateContext(hDC);
	if(hrenderRC==NULL)
		return FALSE;
	if(wglMakeCurrent(hDC,hrenderRC)==FALSE)
		return FALSE;
	return TRUE;
}
void CVIEWDLG::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	OnPaint();
	FPSrate();
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CVIEWDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd-> GetDlgCtrlID() == IDC_STATIC_MAXDB || pWnd-> GetDlgCtrlID() == IDC_STATIC_MINDB){
		pDC->SetBkMode(TRANSPARENT);   //���ñ���͸��
		pDC->SetTextColor(RGB(255,255,255));
		return   HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
void CVIEWDLG::GetSize(){
	CWnd *pwnd=GetDlgItem(IDC_RENDER);//��ȡ�ؼ����
	pwnd->GetClientRect(cr_Size);
}
void CVIEWDLG::Deal_Rx_Vertex_Cube(RxInfo RxTemp){

	//y,z������ת��
	CubeVertex_Rx.Add(RxTemp.Pos.x+1);CubeVertex_Rx.Add(RxTemp.Pos.y+1);CubeVertex_Rx.Add(RxTemp.Pos.z);//0
	CubeVertex_Rx.Add(RxTemp.Pos.x+1);CubeVertex_Rx.Add(RxTemp.Pos.y-1);CubeVertex_Rx.Add(RxTemp.Pos.z);//1
	CubeVertex_Rx.Add(RxTemp.Pos.x-1);CubeVertex_Rx.Add(RxTemp.Pos.y-1);CubeVertex_Rx.Add(RxTemp.Pos.z);//2
	CubeVertex_Rx.Add(RxTemp.Pos.x-1);CubeVertex_Rx.Add(RxTemp.Pos.y+1);CubeVertex_Rx.Add(RxTemp.Pos.z);//3

	CubeVertex_Rx.Add(RxTemp.Pos.x+1);CubeVertex_Rx.Add(RxTemp.Pos.y+1);CubeVertex_Rx.Add(RxTemp.Pos.z+2);//4
	CubeVertex_Rx.Add(RxTemp.Pos.x+1);CubeVertex_Rx.Add(RxTemp.Pos.y-1);CubeVertex_Rx.Add(RxTemp.Pos.z+2);//5
	CubeVertex_Rx.Add(RxTemp.Pos.x-1);CubeVertex_Rx.Add(RxTemp.Pos.y-1);CubeVertex_Rx.Add(RxTemp.Pos.z+2);//6
	CubeVertex_Rx.Add(RxTemp.Pos.x-1);CubeVertex_Rx.Add(RxTemp.Pos.y+1);CubeVertex_Rx.Add(RxTemp.Pos.z+2);//7
}

void CVIEWDLG::Deal_Tx_Vertex_Cube(TxInfo TxTemp){
	//y��z������ת��
	CubeVertex_Tx.Add(TxTemp.Pos.x+1);CubeVertex_Tx.Add(TxTemp.Pos.y+1);CubeVertex_Tx.Add(TxTemp.Pos.z);//0
	CubeVertex_Tx.Add(TxTemp.Pos.x+1);CubeVertex_Tx.Add(TxTemp.Pos.y-1);CubeVertex_Tx.Add(TxTemp.Pos.z);//1
	CubeVertex_Tx.Add(TxTemp.Pos.x-1);CubeVertex_Tx.Add(TxTemp.Pos.y-1);CubeVertex_Tx.Add(TxTemp.Pos.z);//2
	CubeVertex_Tx.Add(TxTemp.Pos.x-1);CubeVertex_Tx.Add(TxTemp.Pos.y+1);CubeVertex_Tx.Add(TxTemp.Pos.z);//3

	CubeVertex_Tx.Add(TxTemp.Pos.x+1);CubeVertex_Tx.Add(TxTemp.Pos.y+1);CubeVertex_Tx.Add(TxTemp.Pos.z+2);//4
	CubeVertex_Tx.Add(TxTemp.Pos.x+1);CubeVertex_Tx.Add(TxTemp.Pos.y-1);CubeVertex_Tx.Add(TxTemp.Pos.z+2);//5
	CubeVertex_Tx.Add(TxTemp.Pos.x-1);CubeVertex_Tx.Add(TxTemp.Pos.y-1);CubeVertex_Tx.Add(TxTemp.Pos.z+2);//6
	CubeVertex_Tx.Add(TxTemp.Pos.x-1);CubeVertex_Tx.Add(TxTemp.Pos.y+1);CubeVertex_Tx.Add(TxTemp.Pos.z+2);//7

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CVIEWDLG::OnFileDxf()
{
	// TODO: �ڴ���������������

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,".dxf|*.dxf|All Files(*.*)|*.*");
	if (dlg.DoModal()==IDOK)
	{
		Dxfpath=dlg.GetPathName();
		DxfName=dlg.GetFileName();
		de.ReadDxf(Dxfpath);
		InitFlag=1;

		Point_RxInfo.RemoveAll();
		Point_TxInfo.RemoveAll();
		Polygon_RxInfo.RemoveAll();
		Polygon_TxInfo.RemoveAll();
		CubeVertex_Tx.RemoveAll();
		CubeVertex_Rx.RemoveAll();
		RenderArray.RemoveAll();

		OneReflection.RemoveAll();//һ�׷���
		TwoReflection.RemoveAll();//���׷���
		OneDiffraction.RemoveAll();//һ������
		TwoDiffraction.RemoveAll();//��������
		ReflectionToDiffraction.RemoveAll();//һ��
		DiffractionToReflection.RemoveAll();//һ��
		TwoReflectionToDiffraction.RemoveAll();//��
		ReflectionToTwoDiffraction.RemoveAll();//һ
		TwoDiffractionToReflection.RemoveAll();//��
		DiffractionToTwoReflection.RemoveAll();//һ
		TwoReflectionToTwoDiffraction.RemoveAll();

	}
}
void CVIEWDLG::OnFileCity()
{
	// TODO: �ڴ���������������

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,".City|*.City|All Files(*.*)|*.*");
	if (dlg.DoModal()==IDOK){
		Citypath=dlg.GetPathName();
		CityName=dlg.GetFileName();
		de.ReadCity(Citypath);
		InitFlag=2;

		Point_RxInfo.RemoveAll();
		Point_TxInfo.RemoveAll();
		Polygon_RxInfo.RemoveAll();
		Polygon_TxInfo.RemoveAll();
		CubeVertex_Tx.RemoveAll();
		CubeVertex_Rx.RemoveAll();
		RenderArray.RemoveAll();

		OneReflection.RemoveAll();//һ�׷���
		TwoReflection.RemoveAll();//���׷���
		OneDiffraction.RemoveAll();//һ������
		TwoDiffraction.RemoveAll();//��������
		ReflectionToDiffraction.RemoveAll();//һ��
		DiffractionToReflection.RemoveAll();//һ��
		TwoReflectionToDiffraction.RemoveAll();//��
		ReflectionToTwoDiffraction.RemoveAll();//һ
		TwoDiffractionToReflection.RemoveAll();//��
		DiffractionToTwoReflection.RemoveAll();//һ
		TwoReflectionToTwoDiffraction.RemoveAll();
	}
}

void CVIEWDLG::OnViewRender()
{
	// TODO: �ڴ���������������
	if(IsRender==TRUE)
		IsRender=FALSE;
	else
		IsRender=TRUE;
}

void CVIEWDLG::OnViewNormal()
{
	// TODO: �ڴ���������������
	EditMode=0;
	OnPaint();
}

void CVIEWDLG::OnTransmmitsPoints()
{
	// TODO: �ڴ���������������
	TxPointFlag=TRUE;
	TxPolygonFlag=FALSE;
	RxPointFlag=FALSE;
	RxPolygonFlag=FALSE;
	EditMode=1;
	OnPaint();
}

void CVIEWDLG::OnReceiversPoints()
{
	// TODO: �ڴ���������������
	TxPointFlag=FALSE;
	TxPolygonFlag=FALSE;
	RxPointFlag=TRUE;
	RxPolygonFlag=FALSE;
	EditMode=3;
	OnPaint();
}

void CVIEWDLG::OnTransmmitsPolygons()
{
	// TODO: �ڴ���������������

	TxPointFlag=FALSE;
	TxPolygonFlag=TRUE;
	RxPointFlag=FALSE;
	RxPolygonFlag=FALSE;
	EditMode=2;
	if(SetPointOnTin==0){
		MessageBox("���ڵ�ͼ�ϵ�ѡ���ε����ĵ㣡","��ʾ",MB_OK);
	}
	else if(SetPointOnTin==1)
		MessageBox("���ڵ�ͼ�Ͽ�ѡ����������","Tips",MB_OK);
	OnPaint();
}

void CVIEWDLG::OnReceiversPolygons()
{
	// TODO: �ڴ���������������
	TxPointFlag=FALSE;
	TxPolygonFlag=FALSE;
	RxPointFlag=FALSE;
	RxPolygonFlag=TRUE;
	EditMode=4;
	if(SetPointOnTin==0){
		MessageBox("���ڵ�ͼ�ϵ�ѡ���ε����ĵ㣡","��ʾ",MB_OK);
	}
	else if(SetPointOnTin==1)
		MessageBox("���ڵ�ͼ�Ͽ�ѡ����������","Tips",MB_OK);
	OnPaint();
}

void CVIEWDLG::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnClose();
}

void CVIEWDLG::AddEle(){
	GLfloat tempx,tempy;
	if(EditMode==3){
		RxInfo rx_info;

		tempx=(GLfloat)ResultPos.x;
		tempy=(GLfloat)ResultPos.y;

		rx_info.Pos.x=tempx;
		rx_info.Pos.y=tempy;

		Point_RxInfo.Add(rx_info);
	}
	else if(EditMode==1){
		TxInfo Tx_info;
		tempx=(GLfloat)ResultPos.x;
		tempy=(GLfloat)ResultPos.y;

		Tx_info.Pos.x=tempx;
		Tx_info.Pos.y=tempy;

		Point_TxInfo.Add(Tx_info);
	}
	else if(EditMode==2){
		PosInGL.x=(GLfloat)ResultPos.x;
		PosInGL.y=(GLfloat)ResultPos.y;
		PosInGL.z=(GLfloat)ResultPos.z;
	}
	else if(EditMode==4){
		PosInGL.x=(GLfloat)ResultPos.x;
		PosInGL.y=(GLfloat)ResultPos.y;
		PosInGL.z=(GLfloat)ResultPos.z;
	}
}
void CVIEWDLG::OnOutputPowerpath()
{
	// TODO: �ڴ���������������
	IsPathAvailable=TRUE;
}

void CVIEWDLG::OnSetAntenna()
{
	// TODO: �ڴ���������������
	if(!AnteDlg){
		AnteDlg=new ANTE_DIALOG();
		AnteDlg->Create(IDD_ANTE_DIALOG);
		AnteDlg->ShowWindow(SW_NORMAL);
	}
	else if(AnteDlg&&!AnteDlg->IsWindowVisible())
		AnteDlg->ShowWindow(SW_NORMAL);
}

void CVIEWDLG::MagicSort(CArray<RxInfo> &aref,int low,int high){
	int mid;
	if(low<=high){
		mid = Partition(aref,low,high);
		MagicSort(aref,low,mid-1);
		MagicSort(aref,mid+1,high);
	}
}

int CVIEWDLG::Partition(CArray<RxInfo> &aref,int i,int j){
	RxInfo t=aref[i];
	//GLfloat t=aref[i].ReceivedPower;
	while(i<j){
		while(i<j&&aref[j].ReceivedPower>t.ReceivedPower){
			j--;
		}
		if(i<j){
			aref[i]=aref[j];
			i++;
		}
		while(i<j&&aref[i].ReceivedPower<=t.ReceivedPower){
			i++;
		}
		if(i<j){
			aref[j]=aref[i];
			j--;
		}
	}
	aref[i]=t;
	return i;
}

void CVIEWDLG::PowerToColor(CArray<RxInfo> &AREF){
	//ColorRamp color;
	int indices1=(AREF.GetCount()-1)/4;
	int indices2=(AREF.GetCount()-1)/2;
	int indices3=3*(AREF.GetCount()-1)/4;

	GLfloat d1=AREF[1].ReceivedPower;
	GLfloat d2=AREF[indices1].ReceivedPower;
	GLfloat d3=AREF[indices2].ReceivedPower;
	GLfloat d4=AREF[indices3].ReceivedPower;
	GLfloat d5=AREF[AREF.GetCount()-1].ReceivedPower;

	for(int i=1;i<AREF.GetCount();i++){
		Deal_Render_Quads(AREF[i].Pos);

		if(AREF[i].ReceivedPower>=d1&&AREF[i].ReceivedPower<=d2){
		//color.red=0.0f;
		//	color.blue=1.0f;
		//	color.green=(GLfloat)(AREF[i].ReceivedPower-d1)/(d2-d1);
			RenderArray.Add(0.0f);
			RenderArray.Add((GLfloat)(AREF[i].ReceivedPower-d1)/(d2-d1));
			RenderArray.Add(1.0f);
		}
		else if(AREF[i].ReceivedPower>=d2&&AREF[i].ReceivedPower<=d3){
			//color.red=0.0f;
			//color.green=1.0f;
			//color.blue=(GLfloat)(AREF[i].ReceivedPower-d2)/(d3-d2);
			RenderArray.Add(0.0f);
			RenderArray.Add(1.0f);
			RenderArray.Add((GLfloat)(AREF[i].ReceivedPower-d2)/(d3-d2));
		}
		else if(AREF[i].ReceivedPower>=d3&&AREF[i].ReceivedPower<=d4){
			//color.blue=0.0f;
			//color.green=1.0f;
			//color.red=(GLfloat)(AREF[i].ReceivedPower-d3)/(d4-d3);
			RenderArray.Add((GLfloat)(AREF[i].ReceivedPower-d3)/(d4-d3));
			RenderArray.Add(1.0f);
			RenderArray.Add(0.0f);
		}
		else if(AREF[i].ReceivedPower>=d4&&AREF[i].ReceivedPower<=d5){
			//color.blue=0.0f;
			//color.red=1.0f;
			//color.green=(GLfloat)(AREF[i].ReceivedPower-d4)/(d5-d4);
			RenderArray.Add(1.0f);
			RenderArray.Add((GLfloat)(AREF[i].ReceivedPower-d4)/(d5-d4));
			RenderArray.Add(0.0f);
		}
	}
}

void CVIEWDLG::Deal_Render_Quads(CurPos3D temp){
	GLfloat a=space/2;
	RenderArray.Add(temp.x-a);RenderArray.Add(temp.y-a);RenderArray.Add(temp.z);//0
	RenderArray.Add(temp.x+a);RenderArray.Add(temp.y-a);RenderArray.Add(temp.z);//1
	RenderArray.Add(temp.x+a);RenderArray.Add(temp.y+a);RenderArray.Add(temp.z);//2
	RenderArray.Add(temp.x-a);RenderArray.Add(temp.y+a);RenderArray.Add(temp.z);//3

}

void CVIEWDLG::OnOutputRenderoutput()
{
	// TODO: �ڴ���������������

	if(IsRenderOutput)
		IsRenderOutput=FALSE;
	else
		IsRenderOutput=TRUE;
	if(1==isRender)
		isRender=0;
	else 
		isRender=1;
}

void CVIEWDLG::OnClearPolygontx()
{
	// TODO: �ڴ���������������
	Polygon_TxInfo.RemoveAll();

}

void CVIEWDLG::OnClearPolygonrx()
{
	// TODO: �ڴ���������������
	Polygon_RxInfo.RemoveAll();
	CubeVertex_Rx.RemoveAll();
	
}

void CVIEWDLG::OnExamplePathexample()
{
	// TODO: �ڴ���������������
	/*OneReflection.Add(-1.51f);OneReflection.Add(-18.79f);OneReflection.Add(5.0f);
	OneDiffraction.Add(-1.51f);OneDiffraction.Add(-29.77f);OneDiffraction.Add(5.0f);
	TwoReflection.Add(-1.48f);TwoReflection.Add(-12.81f);TwoReflection.Add(6.78f);TwoReflection.Add(-29.83f);TwoReflection.Add(19.94f);TwoReflection.Add(8.28f);
	TwoDiffraction.Add(-1.5f);TwoDiffraction.Add(0.19f);TwoDiffraction.Add(5.0f);TwoDiffraction.Add(-51.49f);TwoDiffraction.Add(19.96f);TwoDiffraction.Add(5.0f);
	ReflectionToDiffraction.Add(-1.45f);ReflectionToDiffraction.Add(-8.56f);ReflectionToDiffraction.Add(4.04f);ReflectionToDiffraction.Add(-51.46f);ReflectionToDiffraction.Add(20.05f);ReflectionToDiffraction.Add(8.44f);
	DiffractionToReflection.Add(-1.51f);DiffractionToReflection.Add(-0.05f);DiffractionToReflection.Add(8.01f);DiffractionToReflection.Add(-19.20f);DiffractionToReflection.Add(19.96f);DiffractionToReflection.Add(8.64f);*/

	if(Point_RxInfo.GetCount()==0||0==Point_TxInfo.GetCount()){
		MessageBox("no Tx or Rx exists! Set the Rx and Tx first!","Tips",MB_OK);
	}
	else{
		 OneReflection.Add(-1.51f-MapCenterX); OneReflection.Add(-18.79f-MapCenterY); OneReflection.Add(5.00f); 
		OneDiffraction.Add(-1.51f-MapCenterX);OneDiffraction.Add(-29.77f-MapCenterY);OneDiffraction.Add(5.00f);
		 TwoReflection.Add(-1.48f-MapCenterX); TwoReflection.Add(-12.81f-MapCenterY); TwoReflection.Add(6.78f); TwoReflection.Add(-29.83f-MapCenterX); TwoReflection.Add(19.94f-MapCenterY); TwoReflection.Add(8.28f);
		TwoDiffraction.Add(-1.50f-MapCenterX);  TwoDiffraction.Add(0.19f-MapCenterY); TwoDiffraction.Add(5.0f);TwoDiffraction.Add(-51.49f-MapCenterX);TwoDiffraction.Add(19.96f-MapCenterY);TwoDiffraction.Add(5.0f);
		ReflectionToDiffraction.Add(-1.45f-MapCenterX);ReflectionToDiffraction.Add(-8.56f-MapCenterY);ReflectionToDiffraction.Add(4.04f);ReflectionToDiffraction.Add(-51.46f-MapCenterX);ReflectionToDiffraction.Add(20.05f-MapCenterY);ReflectionToDiffraction.Add(8.44f);
		DiffractionToReflection.Add(-1.51f-MapCenterX);DiffractionToReflection.Add(-0.05f-MapCenterY);DiffractionToReflection.Add(8.01f);DiffractionToReflection.Add(-19.20f-MapCenterX);DiffractionToReflection.Add(19.96f-MapCenterY);DiffractionToReflection.Add(8.64f);
																																						
		MessageBox("·��ʾ��������ɣ��밴��ʾ·����ť�鿴","��ʾ",MB_OK);
	}
	
}

void CVIEWDLG::OnExampleRenderexample()
{
	// TODO: �ڴ���������������
	if(Polygon_RxInfo.GetCount()==0){
		MessageBox("no Polygon Rx exists!Set the Polygon Rx first!","tips",MB_OK);
	}
	else{
		GLfloat a=10.0;
		GLfloat b=100.0;
		srand((unsigned)time(NULL));
		for(int i=1;i<Polygon_RxInfo.GetCount();i++){
			Polygon_RxInfo[i].ReceivedPower=a+b*rand()/(GLfloat)RAND_MAX;
		}

		MagicSort(Polygon_RxInfo,1,Polygon_RxInfo.GetCount()-1);
		PowerToColor(Polygon_RxInfo);

		MessageBox("��Ⱦʾ��������ɣ��밴��ʾ��Ⱦ��ť�鿴","��ʾ",MB_OK);
	}
}

void CVIEWDLG::OnEditTin()
{
	// TODO: �ڴ���������������
	MessageBox("���ڴ�����������ӵ��ε㣡����Ҽ���ɻ��㣡","Tips",MB_OK);
	EditMode=5;
	PointVision=1;
	if(T1.GetCount()!=0)
		T1.RemoveAll();
	if(T2.GetCount()!=0)
		T2.RemoveAll();
	if(T3.GetCount()!=0)
		T3.RemoveAll();
	
}

void CVIEWDLG::OnEditModeltin()
{
	// TODO: �ڴ���������������

	srand((unsigned)time(NULL));
	if(T1.GetCount()==0){
		MessageBox("û����ӵ���߶�������ݣ�����޷�����TIN������ӵ���������ݺ����ԣ�","Tips",MB_OK);
		return;
	}

	if(T1.GetCount()<500){//��T1�еĵ�������500������Ϊ�Ƕ���ĵ㣬���ٸ�����߳�
		//////////////////////////////////////////////////////////////////////////
		/////////////////////////������ǣ��Ա����/////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		for(int i=0;i<T1.GetCount();i++){
			T1[i].z=(GLfloat)10*rand()/RAND_MAX;
		}
	}

	TinPointer->OnTin();
	TinPointer->CalculateNormal();

	TinFlag=1;//ָʾ���Ի�TIN��

	SetPointOnTin=1;//��ʱ�����շ��Ż�����Ҫ��Tin��

	PointVision=0;//��ʱ�Ͳ��ٻ����ˣ�ֻ��TIN

	EditMode=0;//����TIN�󣬰ѱ༭ģʽȡ��
}

void CVIEWDLG::OnClearTin()
{
	// TODO: �ڴ���������������
	T1.RemoveAll();T2.RemoveAll();T3.RemoveAll();
	TinFlag=0;
	SetPointOnTin=0;
}

void CVIEWDLG::OnClearPath()
{
	// TODO: �ڴ���������������
	IsPathAvailable=false;
}

void CVIEWDLG::OnClearPointtx()
{
	// TODO: �ڴ���������������
	Point_TxInfo.RemoveAll();
}

void CVIEWDLG::OnClearPointrx()
{
	// TODO: �ڴ���������������
	Point_RxInfo.RemoveAll();
}

void CVIEWDLG::OnClearRender()
{
	// TODO: �ڴ���������������
	RenderArray.RemoveAll();
}

void CVIEWDLG::OnFileTin()
{
	// TODO: �ڴ���������������
	RW->ReadPointofTIN();
	PointVision=1;
	EditMode=0;
}

void CVIEWDLG::OnTestTest1()
{
	// TODO: �ڴ���������������
}

void CVIEWDLG::OnView32819()
{
	// TODO: �ڴ���������������
	X_Translating=0.0f;Y_Translating=0.0f;
}

void CVIEWDLG::OnEditTin32820()
{
	// TODO: �ڴ���������������
	IsSetTexTure=1;//ָʾ���Կ�ʼ���TIN����
	TinFlag=0;
}

void CVIEWDLG::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
	//glClearColor(1.0,1.0,1.0,1.0);

	//glClear(GL_COLOR_BUFFER_BIT);

	//glViewport(0,0,cx,cy);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//if(cx<=cy)
	//	glOrtho(-500.0,500.0,-500.0*(GLfloat)cy/(GLfloat)cx,
	//	500.0*(GLfloat)cy/(GLfloat)cx,-500.0,500.0);
	//else
	//	glOrtho(-500.0*(GLfloat)cx/(GLfloat)cy,
	//	500.0*(GLfloat)cx/(GLfloat)cy,-500.0,500.0,-500.0,500.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity(); 

	//float range=100.0f;
	//GetSize();
	//if(cr_Size.Height()==0){
	//	CWnd *tempWnd=GetDlgItem(IDC_RENDER);
	//	tempWnd->SetWindowPos(NULL,0,0,200,200,SWP_NOMOVE);
	//	tempWnd->ShowWindow(SW_SHOW);
	//	tempWnd->UpdateWindow();
	//}
	//glViewport(0,0,cr_Size.Width(),cr_Size.Height());
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//if(cr_Size.Width()<cr_Size.Height())
	//	glOrtho(0.0f,0.0f,range*cr_Size.Width()/cr_Size.Height(),-range*cr_Size.Width()/cr_Size.Height(),range,-range);
	//else
	//	glOrtho(-range*cr_Size.Height()/cr_Size.Width(),range*cr_Size.Height()/cr_Size.Width(),-range,range,-range*2.0f,range*2.0f);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//ArcBall->setBounds((GLfloat)cr_Size.Width(),(GLfloat)cr_Size.Height());
	//SetupRC();
}
BOOL   CVIEWDLG::PreTranslateMessage(MSG*   pMsg)     {   
	//onkeydown���޷���Ӧ��ť�¼��ģ�ֻ��ͨ�����ش˺�����ʵ�ְ�ť�¼�������dialog based MFC������
	if(pMsg->message==WM_KEYDOWN)   
	{   
		if(pMsg->wParam==VK_UP)
			ArcBall->zoomRate+=0.2f;
		if(pMsg->wParam==VK_DOWN)
			ArcBall->zoomRate-=0.2f;
		if(pMsg->wParam==VK_SPACE)
			if(israwtotin)
				CRT.RenderFlag=!CRT.RenderFlag;
/*		if(pMsg->wParam==VK_LEFT){
			if(!light)
				glEnable(GL_LIGHT1);
			else
				glDisable(GL_LIGHT1);
		}		*/	
	}
	return CDialog::PreTranslateMessage(pMsg); 
}

void CVIEWDLG::OnFileDem(){
	// TODO: Add your command handler code here
	IsSetTexTure=0;//��ʱ���������ټ�����ͼ���������
	CString path;
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,".raw|*.raw|all files(*.*)|*.*");
	if(dlg.DoModal()==IDOK){
		path=dlg.GetPathName();
	}
	CRT.loadRawFile(path,1024*1024,g_HeightMap);
	israwtotin=1;
	SetPointOnTin=1;//��ʱ�����շ��Ż�����Ҫ��Tin��
}
