// POLYGON_RX_DIALOG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GreatTracer.h"
#include "POLYGON_RX_DIALOG.h"

CVIEWDLG *cview;

// POLYGON_RX_DIALOG �Ի���

IMPLEMENT_DYNAMIC(POLYGON_RX_DIALOG, CDialog)

POLYGON_RX_DIALOG::POLYGON_RX_DIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(POLYGON_RX_DIALOG::IDD, pParent)
{

}

POLYGON_RX_DIALOG::~POLYGON_RX_DIALOG()
{
}

void POLYGON_RX_DIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(POLYGON_RX_DIALOG, CDialog)
//	ON_EN_CHANGE(IDC_POLYGON_WIDTH_EDIT, &POLYGON_RX_DIALOG::OnEnChangePolygonWidthEdit)
	ON_BN_CLICKED(IDOK, &POLYGON_RX_DIALOG::OnBnClickedOk)
END_MESSAGE_MAP()


// POLYGON_RX_DIALOG ��Ϣ�������

BOOL POLYGON_RX_DIALOG::OnInitDialog(){
	if(SetPointOnTin==1){
		GetDlgItem(IDC_POLYGON_RX_LONGTH_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_POLYGON_RX_WIDTH_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_POLYGON_RX_HEIGHT_EDIT)->EnableWindow(FALSE);
	}
	SetDlgItemText(IDC_POLYGON_RX_LONGTH_EDIT,"100.0");
	SetDlgItemText(IDC_POLYGON_RX_WIDTH_EDIT,"100.0");
	SetDlgItemText(IDC_POLYGON_RX_HEIGHT_EDIT,"1.0");
	SetDlgItemText(IDC_POLYGON_RX_SPACE_EDIT,"5.0");

	return 0;
}

GLfloat longth,width,height,space;//����γ��������ջ��߶Ⱥͽ��ջ��ļ��
void POLYGON_RX_DIALOG::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CString temp;
	if(SetPointOnTin==0){
		GetDlgItemText(IDC_POLYGON_RX_LONGTH_EDIT,temp);
		longth=(GLfloat)atof(temp);
		GetDlgItemText(IDC_POLYGON_RX_WIDTH_EDIT,temp);
		width=(GLfloat)atof(temp);
		GetDlgItemText(IDC_POLYGON_RX_HEIGHT_EDIT,temp);
		height=(GLfloat)atof(temp);
	}
	GetDlgItemText(IDC_POLYGON_RX_SPACE_EDIT,temp);
	space=(GLfloat)atof(temp);

	if(space<1.0){
		MessageBox("δ���ý��ջ��������ջ�������ù�С�����������ã�","Tips",MB_OK);
		return;
	}
	
	if(SetPointOnTin==0){
		GLfloat minX,minY;
		int capacity=Polygon_RxInfo.GetCount();
		minX=Polygon_RxInfo[capacity-1].Pos.x-longth/2;
		minY=Polygon_RxInfo[capacity-1].Pos.y-width/2;

		RxInfo tempRxPoint;
		for(int i=0;i<longth/space;i++){
			for(int j=0;j<width/space;j++){
				tempRxPoint.Pos.x=minX+i*space;
				tempRxPoint.Pos.y=minY+j*space;
				tempRxPoint.Pos.z=height;
				Polygon_RxInfo.Add(tempRxPoint);
			}
		}
	}
	else if(SetPointOnTin==1){
		if(ScrBoundaryPoint.GetCount()==2&&GLBoundaryPoint.GetCount()==2){

			RxInfo tempRxPoint;

			longth=(GLfloat)(GLBoundaryPoint[0].x-GLBoundaryPoint[1].x);
			width=(GLfloat)(GLBoundaryPoint[0].y-GLBoundaryPoint[1].y);
			if (longth<0)
				longth*=-1;
			if(width<0)
				width*=-1;

			int m,n;//m�ǳ����Ϸ�����ĸ�����n�ǿ��Ϸ�����ĸ���
			m=(int) (longth/space);n=(int)(width/space);

			if(m<1||n<1){
				MessageBox("���õľ��η�Χ��С�����߷�����������޷����÷����","Tips",MB_OK);
				return;
			}

			GLfloat scr_space_x,scr_space_y;//��������Ļ�ϵļ��
			scr_space_x=(GLfloat)((ScrBoundaryPoint[0].x-ScrBoundaryPoint[1].x)/m);
			scr_space_y=(GLfloat)((ScrBoundaryPoint[0].y-ScrBoundaryPoint[1].y)/n);

			if(scr_space_x>0){//��һ�����ڵڶ����������
				if(scr_space_y>0){//��һ�����ڵڶ����������
					for(int i=0;i<m;i++){
						for(int j=0;j<n;j++){
							cview->GetCoor_TIN(ScrBoundaryPoint[0].x-i*scr_space_x,ScrBoundaryPoint[0].y-j*scr_space_y);
							tempRxPoint.Pos.x=(GLfloat)ResultPos.x;tempRxPoint.Pos.y=(GLfloat)ResultPos.y;tempRxPoint.Pos.z=(GLfloat)ResultPos.z+0.2;
							Polygon_RxInfo.Add(tempRxPoint);
						}
					}

					//��ɼ��㣬��ձ߽��ͱ߽��־
					ScrBoundaryPoint.RemoveAll();
					GLBoundaryPoint.RemoveAll();
					//SetPointOnTin=0;
					BoundaryFlag=0;
				}
				if(scr_space_y<0){//��һ�����ڵڶ����������
					for(int i=0;i<m;i++){
						for(int j=0;j<n;j++){
							cview->GetCoor_TIN(ScrBoundaryPoint[0].x-i*scr_space_x,ScrBoundaryPoint[0].y-j*scr_space_y);
							tempRxPoint.Pos.x=(GLfloat)ResultPos.x;tempRxPoint.Pos.y=(GLfloat)ResultPos.y;tempRxPoint.Pos.z=(GLfloat)ResultPos.z+0.2;
							Polygon_RxInfo.Add(tempRxPoint);
						}
					}
					//��ɼ��㣬��ձ߽��ͱ߽��־
					ScrBoundaryPoint.RemoveAll();
					GLBoundaryPoint.RemoveAll();
					//SetPointOnTin=0;
					BoundaryFlag=0;
				}
			}

			if(scr_space_x<0){//��һ�����ڵڶ����������
				if(scr_space_y>0){//��һ�����ڵڶ����������
					for(int i=0;i<m;i++){
						for(int j=0;j<n;j++){
							cview->GetCoor_TIN(ScrBoundaryPoint[0].x-i*scr_space_x,ScrBoundaryPoint[0].y-j*scr_space_y);
							tempRxPoint.Pos.x=(GLfloat)ResultPos.x;tempRxPoint.Pos.y=(GLfloat)ResultPos.y;tempRxPoint.Pos.z=(GLfloat)ResultPos.z+0.2;
							Polygon_RxInfo.Add(tempRxPoint);
						}
					}
					//��ɼ��㣬��ձ߽��ͱ߽��־
					ScrBoundaryPoint.RemoveAll();
					GLBoundaryPoint.RemoveAll();
					//SetPointOnTin=0;
					BoundaryFlag=0;
				}
				if(scr_space_y<0){//��һ�����ڵڶ����������
					for(int i=0;i<m;i++){
						for(int j=0;j<n;j++){
							cview->GetCoor_TIN(ScrBoundaryPoint[0].x-i*scr_space_x,ScrBoundaryPoint[0].y-j*scr_space_y);
							tempRxPoint.Pos.x=(GLfloat)ResultPos.x;tempRxPoint.Pos.y=(GLfloat)ResultPos.y;tempRxPoint.Pos.z=(GLfloat)ResultPos.z+0.2;
							Polygon_RxInfo.Add(tempRxPoint);
						}
					}
					//��ɼ��㣬��ձ߽��ͱ߽��־
					ScrBoundaryPoint.RemoveAll();
					GLBoundaryPoint.RemoveAll();
					//SetPointOnTin=0;
					BoundaryFlag=0;
				}
			}
	}
	}
	else
		MessageBox("��bug������ϵ������Ա��","Tips",MB_OK);

	OnOK();
}