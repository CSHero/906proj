// POLYGON_TX_DIALOG.cpp : 实现文件
//

#include "stdafx.h"
#include "GreatTracer.h"
#include "POLYGON_TX_DIALOG.h"

CVIEWDLG *cviewdlg1;

// POLYGON_TX_DIALOG 对话框

IMPLEMENT_DYNAMIC(POLYGON_TX_DIALOG, CDialog)

POLYGON_TX_DIALOG::POLYGON_TX_DIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(POLYGON_TX_DIALOG::IDD, pParent)
{
}

POLYGON_TX_DIALOG::~POLYGON_TX_DIALOG()
{
}

void POLYGON_TX_DIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(POLYGON_TX_DIALOG, CDialog)
	ON_BN_CLICKED(IDOK, &POLYGON_TX_DIALOG::OnBnClickedOk)
END_MESSAGE_MAP()


// POLYGON_TX_DIALOG 消息处理程序
BOOL POLYGON_TX_DIALOG::OnInitDialog(){
	if(SetPointOnTin==1){
		GetDlgItem(IDC_POLYGON_HEIGHT_EDIT)->EnableWindow(FALSE);//禁用高度编辑框
		GetDlgItem(IDC_POLYGON_LONGTH_EDIT)->EnableWindow(FALSE);//禁用
		GetDlgItem(IDC_POLYGON_WIDTH_EDIT)->EnableWindow(FALSE);//禁用
	}
	SetDlgItemText(IDC_POLYGON_LONGTH_EDIT,"100.0");
	SetDlgItemText(IDC_POLYGON_WIDTH_EDIT,"100.0");
	SetDlgItemText(IDC_POLYGON_HEIGHT_EDIT,"1.0");
	SetDlgItemText(IDC_POLYGON_SPACE_EDIT,"0.0");
	SetDlgItemText(IDC_POLYGON_POWER_EDIT,"0.0");
	SetDlgItemText(IDC_POLYGON_CARRIERFRE_EDIT,"900");
	SetDlgItemText(IDC_POLYGON_ANTE_EDIT,"1");
	SetDlgItemText(IDC_POLYGON_WAVEFORM_EDIT,"0");
	return 0;
}
void POLYGON_TX_DIALOG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp;
	GLfloat longth,width,height,space,power,carrierfrequencies;
	int ante,waveform;
	if(SetPointOnTin!=1){//不是在Tin上设收发信机时才Enable 高度 宽度 长度框
		GetDlgItemText(IDC_POLYGON_LONGTH_EDIT,temp);
		longth=(GLfloat)atof(temp);
		GetDlgItemText(IDC_POLYGON_WIDTH_EDIT,temp);
		width=(GLfloat)atof(temp);
		GetDlgItemText(IDC_POLYGON_HEIGHT_EDIT,temp);
		height=(GLfloat)atof(temp);
	}
	GetDlgItemText(IDC_POLYGON_SPACE_EDIT,temp);
	space=(GLfloat)atof(temp);
	GetDlgItemText(IDC_POLYGON_POWER_EDIT,temp);
	power=(GLfloat)atof(temp);
	GetDlgItemText(IDC_POLYGON_CARRIERFRE_EDIT,temp);
	carrierfrequencies=(GLfloat)atof(temp);
	GetDlgItemText(IDC_POLYGON_ANTE_EDIT,temp);
	ante=atoi(temp);
	GetDlgItemText(IDC_POLYGON_WAVEFORM_EDIT,temp);
	waveform=atoi(temp);
	
	if(SetPointOnTin==0){
		GLfloat minX,minY;
		long capacity=Polygon_TxInfo.GetCount();
		minX=Polygon_TxInfo[capacity-1].Pos.x-longth/2;
		minY=Polygon_TxInfo[capacity-1].Pos.y-width/2;

		TxInfo tempTxPoint;

		if (longth/space<1||width/space<1){
			MessageBox("设置的矩形范围过小，或者发射机间距过大！无法设置发射机","Tips",MB_OK);
			return;
		}

		if(space<1){
			MessageBox("未设置发射机间隔或发射机的间隔过小！请重新设置！","Tips",MB_OK);
			return;
		}
		for(int i=0;i<longth/space;i++){
			for(int j=0;j<width/space;j++){
				tempTxPoint.Pos.x=minX+i*space;
				tempTxPoint.Pos.y=minY+j*space;
				tempTxPoint.Pos.z=height;
				tempTxPoint.CarrierFrequency=carrierfrequencies;
				tempTxPoint.AntennaType=ante;
				tempTxPoint.InputPower=power;
				tempTxPoint.WaveForm=waveform;
				Polygon_TxInfo.Add(tempTxPoint);
			}
		}
	}

	else if(SetPointOnTin==1){
		if(ScrBoundaryPoint.GetCount()==2&&GLBoundaryPoint.GetCount()==2){

			TxInfo tempTxPoint;

			longth=(GLfloat)(GLBoundaryPoint[0].x-GLBoundaryPoint[1].x);
			width=(GLfloat)(GLBoundaryPoint[0].y-GLBoundaryPoint[1].y);
			if (longth<0)
				longth*=-1;
			if(width<0)
				width*=-1;

			int m,n;//m是长度上发射机的个数，n是宽上发射机的个数
			m=(int) (longth/space);n=(int)(width/space);

			if(m<1||n<1){
				MessageBox("设置的矩形范围过小，或者发射机间距过大！无法设置发射机","Tips",MB_OK);
				return;
			}

			GLfloat scr_space_x,scr_space_y;//表现在屏幕上的间隔
			scr_space_x=(GLfloat)((ScrBoundaryPoint[0].x-ScrBoundaryPoint[1].x)/m);
			scr_space_y=(GLfloat)((ScrBoundaryPoint[0].y-ScrBoundaryPoint[1].y)/n);

			if(scr_space_x>0){//第一个点在第二个点的右面
				if(scr_space_y>0){//第一个点在第二个点的下面
					for(int i=0;i<m;i++){
						for(int j=0;j<n;j++){
							cviewdlg1->GetCoor_TIN(ScrBoundaryPoint[0].x-i*scr_space_x,ScrBoundaryPoint[0].y-j*scr_space_y);
							tempTxPoint.Pos.x=(GLfloat)ResultPos.x;tempTxPoint.Pos.y=(GLfloat)ResultPos.y;tempTxPoint.Pos.z=(GLfloat)ResultPos.z+0.2;
							tempTxPoint.CarrierFrequency=carrierfrequencies;
							tempTxPoint.AntennaType=ante;
							tempTxPoint.InputPower=power;
							tempTxPoint.WaveForm=waveform;
							Polygon_TxInfo.Add(tempTxPoint);
						}
					}

					//完成计算，清空边界点和边界标志
					ScrBoundaryPoint.RemoveAll();
					GLBoundaryPoint.RemoveAll();
					//SetPointOnTin=0;
					BoundaryFlag=0;
				}
				if(scr_space_y<0){//第一个点在第二个点的上面
					for(int i=0;i<m;i++){
						for(int j=0;j<n;j++){
							cviewdlg1->GetCoor_TIN(ScrBoundaryPoint[0].x-i*scr_space_x,ScrBoundaryPoint[0].y-j*scr_space_y);
							tempTxPoint.Pos.x=(GLfloat)ResultPos.x;tempTxPoint.Pos.y=(GLfloat)ResultPos.y;tempTxPoint.Pos.z=(GLfloat)ResultPos.z+0.2;
							//tempTxPoint.Pos.x=ResultPos.x;tempTxPoint.Pos.y=ResultPos.y;tempTxPoint.Pos.z=ResultPos.z;
							tempTxPoint.CarrierFrequency=carrierfrequencies;
							tempTxPoint.AntennaType=ante;
							tempTxPoint.InputPower=power;
							tempTxPoint.WaveForm=waveform;
							Polygon_TxInfo.Add(tempTxPoint);
						}
					}
					//完成计算，清空边界点和边界标志
					ScrBoundaryPoint.RemoveAll();
					GLBoundaryPoint.RemoveAll();
					//SetPointOnTin=0;
					BoundaryFlag=0;
				}
			}
			
			if(scr_space_x<0){//第一个点在第二个点的左面
				if(scr_space_y>0){//第一个点在第二个点的下面
					for(int i=0;i<m;i++){
						for(int j=0;j<n;j++){
							cviewdlg1->GetCoor_TIN(ScrBoundaryPoint[0].x-i*scr_space_x,ScrBoundaryPoint[0].y-j*scr_space_y);
							tempTxPoint.Pos.x=(GLfloat)ResultPos.x;tempTxPoint.Pos.y=(GLfloat)ResultPos.y;tempTxPoint.Pos.z=(GLfloat)ResultPos.z+0.2;
							//tempTxPoint.Pos.x=ResultPos.x;tempTxPoint.Pos.y=ResultPos.y;tempTxPoint.Pos.z=ResultPos.z;
							tempTxPoint.CarrierFrequency=carrierfrequencies;
							tempTxPoint.AntennaType=ante;
							tempTxPoint.InputPower=power;
							tempTxPoint.WaveForm=waveform;
							Polygon_TxInfo.Add(tempTxPoint);
						}
					}
					//完成计算，清空边界点和边界标志
					ScrBoundaryPoint.RemoveAll();
					GLBoundaryPoint.RemoveAll();
					//SetPointOnTin=0;
					BoundaryFlag=0;
				}
				if(scr_space_y<0){//第一个点在第二个点的上面
					for(int i=0;i<m;i++){
						for(int j=0;j<n;j++){
							cviewdlg1->GetCoor_TIN(ScrBoundaryPoint[0].x-i*scr_space_x,ScrBoundaryPoint[0].y-j*scr_space_y);
							tempTxPoint.Pos.x=(GLfloat)ResultPos.x;tempTxPoint.Pos.y=(GLfloat)ResultPos.y;tempTxPoint.Pos.z=(GLfloat)ResultPos.z+0.2;
							//tempTxPoint.Pos.x=ResultPos.x;tempTxPoint.Pos.y=ResultPos.y;tempTxPoint.Pos.z=ResultPos.z;
							tempTxPoint.CarrierFrequency=carrierfrequencies;
							tempTxPoint.AntennaType=ante;
							tempTxPoint.InputPower=power;
							tempTxPoint.WaveForm=waveform;
							Polygon_TxInfo.Add(tempTxPoint);
						}
					}
					//完成计算，清空边界点和边界标志
					ScrBoundaryPoint.RemoveAll();
					GLBoundaryPoint.RemoveAll();
					//SetPointOnTin=0;
					BoundaryFlag=0;
				}
			}
		}
		else
			MessageBox("点选有bug！联系开发人员！","Tips",MB_OK);
	}
	OnOK();
	free(cviewdlg1);
}
