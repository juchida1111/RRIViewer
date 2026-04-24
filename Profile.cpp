// Profile.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RRI.h"
#include "Profile.h"
#include "afxdialogex.h"


#include  "CommFuncs.h"
#include  "CommGL.h"

#include	"RRI_Common.h"

extern  PARAMS			PM;						// 上下流基本情報

GB_PARAMETER				GBB;
//
extern      int				MAX_TEXTURE, LAT_SPAN,LNG_SPAN,DEM_H,DEM_V;
extern      double			TEX_START_LAT,TEX_START_LNG;
extern      double			DEM_LAT,DEM_LNG,DEM_SIZE;
extern      double			ONE_DEGREE_DISTANCE;
//////
extern      FILE*					fd;


int			PRO_Mnum = 4;
int			PRO_Mnum_ID[] = {IDC_LABEL_1,IDC_EDIT_HMIN,IDC_EDIT_HMAX,IDC_BUTTON_REDRAW };

extern		int			Image_Sub,Option_Sub;

// Common Counter for HARD_COPY
extern      int			Cnt_Prof,Cnt_MProf,Cnt_Hydro;
extern      char			m_CurrentPath[SIZE_05K];

//static	GB_PARAMETER	GB;

// CProfile ダイアログ

IMPLEMENT_DYNAMIC(CProfile, CDialogEx)

CProfile::CProfile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProfile::IDD, pParent)
{

	m_Hmax = _T("");
	m_Hmin = _T("");
}

//-------------------------------------------------------------------------------------
CProfile::~CProfile()
{
}

void CProfile::OnDestroy()
{
	CDialogEx::OnDestroy();
	//
	SAFE_FREE( No );
}

void CProfile::PostNcDestroy() {
	delete this;
}
//-------------------------------------------------------------------------------------


void CProfile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HMAX, m_Hmax);
	DDX_Text(pDX, IDC_EDIT_HMIN, m_Hmin);
}


BEGIN_MESSAGE_MAP(CProfile, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_HMIN, &CProfile::OnEnChangeEditHmin)
	ON_EN_CHANGE(IDC_EDIT_HMAX, &CProfile::OnEnChangeEditHmax)
	ON_BN_CLICKED(IDC_BUTTON_REDRAW, &CProfile::OnBnClickedButtonRedraw)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CProfile メッセージ ハンドラー
//-------------------------------------------------------------------------------------
BOOL CProfile::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN  && pMsg->wParam == VK_RETURN ) {
		OnBnClickedButtonRedraw();return TRUE;
	}
	//---------------------------------------------
	// ハードコピー
	//---------------------------------------------
	if( pMsg->message == WM_KEYUP &&  pMsg->wParam == VK_SNAPSHOT ) {
		CString	fname;
		fname.Format("%s\\RIV_Profile_%d.bmp",m_CurrentPath,Cnt_Prof++);
		GBB.win.cw		= GetDlgItem( IDC_PROFILE_PICT );
		GB_Save_Window( &GBB,fname);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
//
//______________________________________________________________________________________
//int			PRO_Mnum = 4;
//int			PRO_Mnum_ID[] = {IDC_LABEL_1,IDC_EDIT_HMIN,IDC_EDIT_HMAX,IDC_BUTTON_REDRAW };
////////////////////////////////////////////////////////////////////////////////////////
BOOL CProfile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Hmin = Hmax = Dmin = Dmax = DUNDEF;
// Move Window
	CWnd*	cw;
	CRect	Wrect,Lrect,Trect,Arect,Frect;
	Wrect.left = (Prect.Width()-WinW)/2+Prect.left;
	Wrect.right = Wrect.left + WinW;
	Wrect.top = (Prect.Height()-WinH)/2;
	Wrect.bottom = Wrect.top + WinH;
	//
	this->MoveWindow(&Wrect,FALSE);
	this->RedrawWindow();
	this->GetWindowRect( &Wrect );
	//
	int		W,H;
	W = Wrect.Width();
	H = Wrect.Height();
	//
	// Picture Area
	cw = GetDlgItem( IDC_PROFILE_PICT );
	cw->MoveWindow(8,30,W-24,H-65,FALSE);
	cw->RedrawWindow();
	//
	//
	if( WinTitle.GetLength() == 0 ) WinTitle = "Profile Window";
	this->SetWindowTextA( WinTitle );                         
	//
	SetTimer(TEMP_TIMER,50,NULL);
	return TRUE; 
}
////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CProfile::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == TEMP_TIMER ) {
		KillTimer( TEMP_TIMER );
		//-------------------------------------------------------------------
		memset(&GBB.win,0,sizeof( WININFO ) );
		GBB.win.cw		= GetDlgItem( IDC_PROFILE_PICT );
		GBB.win.hWnd	= GBB.win.cw->m_hWnd;
		GBB.win.set = TRUE;
		//
		GBB.win.cw->GetClientRect( &GBB.win.Crect );
		GBB.win.cw->GetClientRect( &GBB.win.MAXrect );
		GBB.win.cWidth	= GBB.win.Crect.Width();
		GBB.win.cHeight	= GBB.win.Crect.Height();
		//
		//----------------------------------------------------
		::ValidateRect(GBB.win.hWnd, 0);					// WM_PAINT の無効化
		//_____________________________________________________
		//  ＯＰＥＮＧＬ　初期化
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		GBB.Gmax		= 2;		// ２レイヤー
		//
		if( FAILED( GB_Init( &GBB ) ) ) {MessageBox("** ERROR ** Error Initialize Open_GL.... Init !","Missed",
						MB_OK|MB_ICONEXCLAMATION);return;}
		for(int i=0;i<GBB.Gmax;i++) GBB.Gobj[i].set = FALSE;

		// -------------------------------------
		GB_Init_Font(&GBB);

		// Ａｔｔｒｉｂｕｔｅ -----------------------------------------------
		if( FAILED( GB_Attrib(&GBB, DIM_3D, ORTHO, F_COLOR( 220, 220, 220 ),0  ) )) {
			MessageBox("** ERROR ** Error Initialize Open_GL.... Attrib!","Missed",MB_OK|MB_ICONEXCLAMATION);return;}
		//_____________________________________________________
		//  時間縦断表示
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		double		RVmin = -DUNDEF,ELmax = DUNDEF;
		tmin = -DUNDEF;
		Dnum = PM.LineNum;
		for(int i=0;i<Dnum;i++) {
			if( RVmin > PM.Pval[2][i] )	 RVmin = PM.Pval[2][i];			// 最低河床高
			if( ELmax < PM.Pval[1][i] ) ELmax = PM.Pval[1][i];			// 最大標高
		}
		//
		Hmin = RVmin;
		if( Hmin >= 0. )  Hmin = 0.;		// Hmin > 0 
		Hmax = ELmax*4.;
		//
		Get_Scale_Axis( &Hmin,&Hmax,&Hrip,&Hunit,&Hfm);
		if( fmod( Hmax,Hunit) != 0. ) {
			Hmax = ((int)(Hmax/Hunit)+1)*Hunit;
			Hrip ++;
		}
		//
		form.Format("%%.%dlf",Hfm);
		m_Hmin.Format(form,Hmin);
		m_Hmax.Format(form,Hmax);
		UDF;
		//
		Get_GoodScale(Dnum,PM.Rdist,&Dmin,&Dmax,&Drip,&Dunit,&Dfm);	// Drip は1少ない
		//
		if((No=(int*)malloc( (Dnum+10)*sizeof( int ))) == NULL ) return;
		//
		Make_Time_Fig();
		//
		PM.Pro_Ready = TRUE;
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CProfile::Make_Time_Fig()
{
//----------------------------------------------------------------------------
	GB_OBJECT*	Gobj = &GBB.Gobj[ OBJID_FIG_0 ];
	if( Gobj->set ) {
		GBB.win.cw = GetDlgItem( IDC_PROFILE_PICT);
		GBobj_Delete(&GBB,0);
	}
	GBobj_Init( Gobj,1000,UNDEF);
	//
	double	Hspan,Vspan,dH,X,Y,Z;
	double	DistUnit=PM.Size*ONE_DEGREE_DISTANCE/100.;  // Dist/1000がｋｍ表示 DISTANCEが１０少ない

	int		no[4];
	int		pno,vcnt;
	double	stX;

	//
	char	str[128];
	//
	Z = -1.;
	// AXIS
	Hspan = (double)(GBB.win.cWidth - 2.*PROFILE_H_MARGIN);
	Vspan = (double)(GBB.win.cHeight - PROFILE_VU_MARGIN-PROFILE_VD_MARGIN);

	// main X -------------------------------------------------------------------------------------------
	X = PROFILE_H_MARGIN;
	Y = PROFILE_VD_MARGIN; 
	Z = -1.;
	no[0] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);

	X = (double)(GBB.win.cWidth - PROFILE_H_MARGIN); 
	Y = PROFILE_VD_MARGIN; 
	Z = -1.;
	no[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,2.f,F_COLOR(0,0,0));

	stX = PROFILE_H_MARGIN;
	dX = Hspan/Drip; 
	for (int i = 0; i <= Drip; i++) {
		X = stX+i*dX; 
		Y = PROFILE_VD_MARGIN - 3.;
		sprintf(str,"%.0lf",Dunit*i);
		pno = GBobj_Add_Text(&GBB,0,DIM_3D,str,"Lucida Sans",FONT_REGULAR,18,8, 
				X-strlen(str)*4.,Y-20.,0.,F_COLOR(0,0,0));
		if( i > 0  ) {
			X = stX+i*dX; 
			Y = PROFILE_VD_MARGIN - 3.;
			no[0] = GBobj_Add_Vert(Gobj, X, Y, -1., FALSE);

			Y = GBB.win.cHeight - PROFILE_VU_MARGIN;
			no[1] = GBobj_Add_Vert(Gobj,X,Y,-1.,FALSE);
			GBobj_Add_Poly(Gobj, 2, no, GL_LINE_STRIP, 0.5f, F_COLOR(0, 0, 0));
		}
	}
	sprintf(str,"Total Length       %.0lf (km)", PM.Rdist[Dnum-1]);
	GBobj_Add_Text(&GBB,0,DIM_2D,str,"Lucida Sans",FONT_BOLD,25,12,
				GBB.win.cWidth/2.-200.,PROFILE_VD_MARGIN/2.-10.,-1.,F_COLOR(0,0,0));

	GBobj_Add_Text(&GBB, 0, DIM_2D, "Upstream", "Lucida Sans", FONT_BOLD, 25, 12,
		PROFILE_H_MARGIN, PROFILE_VD_MARGIN *0.4, -1., F_COLOR(0, 0, 200));
	GBobj_Add_Text(&GBB, 0, DIM_2D, "Downstream", "Lucida Sans", FONT_BOLD, 25, 12,
		Hspan - GBB.win.cWidth /10, PROFILE_VD_MARGIN *0.4, -1., F_COLOR(0, 0, 200));

	// main Y H -------------------------------------------------------------------------------------------
	X = PROFILE_H_MARGIN; 
	Y = PROFILE_VD_MARGIN; 
	Z = -1.;
	no[0] = GBobj_Add_Vert(Gobj,X,Y,Z,TRUE);

	X = PROFILE_H_MARGIN; 
	Y = (double)(GBB.win.cHeight - PROFILE_VU_MARGIN); 
	Z = -1.;
	no[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,1.5f,F_COLOR(0,0,150));

	double		cY,cV;
	CString		form;

	cY = PROFILE_VD_MARGIN;
	stH = cV = (int)(Hmin/Hunit)*Hunit;
	cV = stH;
	for (int i = 0; i<Hrip; i++) {
		X = PROFILE_H_MARGIN-3.; 
		no[0] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);

		X = (double)(GBB.win.cWidth - PROFILE_H_MARGIN);
		no[1] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);
		GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,0.5f,F_COLOR(0,0,150));

		form.Format("%%.%dlf",Hfm);
		sprintf(str,form,cV);
		X = (double)(PROFILE_H_MARGIN - strlen(str)*10 -10);
		GBobj_Add_Text(&GBB,0,DIM_2D,str,"Lucida Sans",FONT_REGULAR,22,10, X,cY-5.,-1.,F_COLOR(0,0,150));
		//
		cY += (Vspan/(Hrip-1));
		cV += Hunit;
	}
	//
	if( Data_Exist ) sprintf(str,"H (m)    %d/%2d/%2d  %d:%02d",
		CCT.GetYear(),CCT.GetMonth(),CCT.GetDay(),CCT.GetHour(),CCT.GetMinute());
	else             strcpy(str,"H (m)    Data Not Exist");
	GBobj_Add_Text(&GBB,0,DIM_2D,str,"Lucida Sans",FONT_BOLD,25,12,
				PROFILE_H_MARGIN/2.+10.,GBB.win.cHeight-PROFILE_VU_MARGIN+18.,-1.,F_COLOR(0,0,150));
	//
	// RIVER BASE & BANK HEIGHT --------------------------------------------------------------------------
	Z = 0.;
	double		YL =  (double)(GBB.win.cHeight - PROFILE_VU_MARGIN); 
	//
	dX = Hspan/(Drip*Dunit);
	dH = Vspan/((Hrip-1)*Hunit);
	//
	for(int i=0;i<3;i++) {
		if( i == 0 && !Data_Exist ) continue;
		if( i == 0 )		Fcol = F_COLOR( 0,0,210 );
		else if( i == 1 )	Fcol = F_COLOR( 255,150,30 );
		else if( i == 2 )	Fcol = F_COLOR( 50,220,50 );
		if( i == 0 ) Z = 1.;
		else         Z = 0.;
		vcnt = 0;	// Add 201601
		for(int p=0;p<Dnum;p++) {
			X = PM.Rdist[p]*dX+PROFILE_H_MARGIN;
			Y = (PM.Pval[i][p] - stH)*dH + PROFILE_VD_MARGIN;
			No[vcnt] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
			vcnt++;
			//
			if (vcnt == 255) {
				if (i == 0) GBobj_Add_Poly(Gobj, vcnt, No, GL_LINE_STRIP, 2.f, Fcol);
				else         GBobj_Add_Poly(Gobj, vcnt, No, GL_LINE_STRIP, 1.f, Fcol);
				No[0] = No[vcnt - 1];
				vcnt = 1;
			}
		}
		if (i == 0) GBobj_Add_Poly(Gobj, vcnt, No, GL_LINE_STRIP, 2.f, Fcol);
		else         GBobj_Add_Poly(Gobj, vcnt, No, GL_LINE_STRIP, 1.f, Fcol);
		//
		// Legend
		//
		X = (double)(PROFILE_H_MARGIN + GBB.win.cWidth/2.);
		Y = YL- (i+1)*GBB.win.cHeight/20.;
		No[0] =  GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
		//
		X += (double)GBB.win.cWidth/10.;
		Y = YL- (i+1)*GBB.win.cHeight/20.;
		No[1] =  GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
		//
		if( i == 0 ) GBobj_Add_Poly(Gobj,2,No,GL_LINE_STRIP,2.f,Fcol);
		else         GBobj_Add_Poly(Gobj,2,No,GL_LINE_STRIP,1.f,Fcol);
		//
		X += (double)GBB.win.cWidth/30.;
		switch( i ) {
			case 0: strcpy(str,"River water level");break;
			case 1: strcpy(str,"Ground level");break;
			case 2: strcpy(str,"River Bed level");break;
		}
		GBobj_Add_Text(&GBB,0,DIM_2D,str,"Lucida Sans",FONT_REGULAR,22,10, X,Y-5.,-1.,Fcol);
	}
	//
	if( ! GBB.cam.set ) {
		GB_Get_ViewWorld( &GBB );
		GBB.cam.stH = 0; GBB.cam.Dw = GBB.cam.endH = (double)GBB.win.cWidth;
		GBB.cam.stV = 0; GBB.cam.Dh = GBB.cam.endV = (double)GBB.win.cHeight;
		GB_Redraw(&GBB,FALSE,TRUE,TRUE,OBJID_FIG_0,TRUE);
		PM.Pro_Ready = TRUE;
	}
	else {
		GB_Redraw(&GBB,FALSE,FALSE,TRUE,OBJID_FIG_0,TRUE);
	}
	GB_Render_Scene(&GBB);
}

void CProfile::OnCancel()
{
	GBB.win.cw = GetDlgItem( IDC_PROFILE_PICT);
	//
	PM.Profile_Proc = TRUE;
	PM.Profile_Result = 0;
	PM.Set_Height_Model = FALSE;
	GB_Term( &GBB );
	DestroyWindow();
}


void CProfile::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if( PM.Pro_Ready ) 	Make_Time_Fig();
}


void CProfile::OnEnChangeEditHmin(){ UDT; Hmin = atof( m_Hmin );}

void CProfile::OnEnChangeEditHmax(){ UDT; Hmax = atof( m_Hmax );}


void CProfile::OnBnClickedButtonRedraw()
{
	Get_Scale_Axis( &Hmin,&Hmax,&Hrip,&Hunit,&Hfm);
	form.Format("%%.%dlf",Hfm);
	Make_Time_Fig();
}


