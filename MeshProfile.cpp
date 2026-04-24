// MeshProfile.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RRI.h"
#include "MeshProfile.h"
#include "afxdialogex.h"

#include  "CommFuncs.h"
#include  "CommGL.h"

#include	"RRI_Common.h"

extern  PARAMS			PM;						// 上下流基本情報
extern  MINFO**			MESH;
//
extern      int				MAX_TEXTURE, LAT_SPAN,LNG_SPAN,DEM_H,DEM_V;
extern      double			TEX_START_LAT,TEX_START_LNG;
extern      double			DEM_LAT,DEM_LNG,DEM_SIZE;
extern      double			ONE_DEGREE_DISTANCE;
//

GB_PARAMETER			GBM;

extern		int			Image_Sub,Option_Sub;
// Common Counter for HARD_COPY
extern      int			Cnt_Prof,Cnt_MProf,Cnt_Hydro;
extern      char			m_CurrentPath[SIZE_05K];

// CMeshProfile ダイアログ

IMPLEMENT_DYNAMIC(CMeshProfile, CDialog)
////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

CMeshProfile::CMeshProfile(CWnd* pParent /*=NULL*/)
	: CDialog(CMeshProfile::IDD, pParent)
{
	m_Hmax = _T("");
	m_Hmin = _T("");
}

CMeshProfile::~CMeshProfile()
{
}

void CMeshProfile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HMAX, m_Hmax);
	DDX_Text(pDX, IDC_EDIT_HMIN, m_Hmin);
}


BEGIN_MESSAGE_MAP(CMeshProfile, CDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_HMIN, &CMeshProfile::OnEnChangeEditHmin)
	ON_EN_CHANGE(IDC_EDIT_HMAX, &CMeshProfile::OnEnChangeEditHmax)
	ON_BN_CLICKED(IDC_BUTTON_REDRAW, &CMeshProfile::OnBnClickedButtonRedraw)
END_MESSAGE_MAP()

void CMeshProfile::PostNcDestroy()
{
	delete this;
	CDialog::PostNcDestroy();
}

void CMeshProfile::OnDestroy()
{
	CDialog::OnDestroy();
}

BOOL CMeshProfile::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN  && pMsg->wParam == VK_RETURN ) {
		Get_Scale_Axis( &Hmin,&Hmax,&Hrip,&Hunit,&Hfm);
		Make_Profile();
		return TRUE;
	}
	//---------------------------------------------
	// ハードコピー
	//---------------------------------------------
	if( pMsg->message == WM_KEYUP &&  pMsg->wParam == VK_SNAPSHOT ) {
		CString	fname;
		fname.Format("%s\\Mesh_Profile_%d.bmp",m_CurrentPath,Cnt_MProf++);
		GBM.win.cw		= GetDlgItem( IDC_MESHPROFILE_PICT );
		GB_Save_Window( &GBM,fname);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BOOL CMeshProfile::OnInitDialog()
{
	CDialog::OnInitDialog();
	PM.MeshProfile_Ready = FALSE;
	Fig_Ready = FALSE;
	//
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
	cw = GetDlgItem( IDC_MESHPROFILE_PICT );
	cw->MoveWindow(8,30,W-20,H-65,FALSE);
	cw->RedrawWindow();

	//
	if( WinTitle.GetLength() == 0 ) WinTitle = "MESH Profile";
	this->SetWindowTextA( WinTitle );                         
	//
	SetTimer(TEMP_TIMER,50,NULL);
	return TRUE; 
}
////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CMeshProfile::OnCancel()
{
	//
	PM.MeshProfile_Proc = TRUE;
	PM.MeshProfile_Result = 0;
	//
	GBM.win.cw = GetDlgItem( IDC_MESHPROFILE_PICT);
	GB_Term( &GBM );
	DestroyWindow();
}
////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void CMeshProfile::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == TEMP_TIMER ) {
		KillTimer( TEMP_TIMER );
		//-------------------------------------------------------------------
		memset(&GBM.win,0,sizeof( WININFO ) );
		GBM.win.cw		= GetDlgItem( IDC_MESHPROFILE_PICT );
		GBM.win.hWnd	= GBM.win.cw->m_hWnd;
		GBM.win.set = TRUE;
		//
		GBM.win.cw->GetClientRect( &GBM.win.Crect );
		GBM.win.cw->GetClientRect( &GBM.win.MAXrect );
		GBM.win.cWidth	= GBM.win.Crect.Width();
		GBM.win.cHeight	= GBM.win.Crect.Height();
		//
		GBM.win.cw->GetWindowRect( &GBM.win.Wrect );
		::ValidateRect(GBM.win.hWnd, NULL );					// WM_PAINT の無効化
		//_____________________________________________________
		//  ＯＰＥＮＧＬ　初期化
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		GBM.Gmax		= 2;		// ２レイヤー
		//
		if( FAILED( GB_Init( &GBM ) ) ) {MessageBox("** ERROR ** Error Initialize Open_GL.... Init !","Missed",
						MB_OK|MB_ICONEXCLAMATION);return;}
		for(int i=0;i<GBM.Gmax;i++) GBM.Gobj[i].set = FALSE;

		// -------------------------------------
		GB_Init_Font(&GBM);

		// Ａｔｔｒｉｂｕｔｅ -----------------------------------------------
		if( FAILED( GB_Attrib(&GBM, DIM_3D, ORTHO, F_COLOR( 220, 220, 220 ),0  ) )) {
			MessageBox("** ERROR ** Error Initialize Open_GL.... Attrib!","Missed",MB_OK|MB_ICONEXCLAMATION);return;}
		//_____________________________________________________
		//  時間縦断表示
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Hmin = -DUNDEF;
		Hmax = DUNDEF;
		for(int i=0;i<Mnum;i++) {
			if( DEM[i] != DUNDEF ) {
				if( Hmin > DEM[i] ) Hmin =  DEM[i];	// 最低地盤高
				if( Hmax < DEM[i] ) Hmax =  DEM[i]; // 水位から地盤高への変更
			}
		}
		//
		if( Hmin >= 0. ) Hmin = 0.;
		Hmax *= 4.;
		Get_Scale_Axis( &Hmin,&Hmax,&Hrip,&Hunit,&Hfm);
		//
		if( fmod( Hmax,Hunit) != 0. ) {
			Hmax = ((int)(Hmax/Hunit)+1)*Hunit;
			Get_Scale_Axis( &Hmin,&Hmax,&Hrip,&Hunit,&Hfm);
		}
		//
		form.Format("%%.%dlf",Hfm);
		m_Hmin.Format(form,Hmin);
		m_Hmax.Format(form,Hmax);
		UDF;
		//
		PM.MeshProfile_Ready = TRUE;
		Make_Profile();
		//
		Fig_Ready = TRUE;
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CMeshProfile::Make_Profile()
{
	if( ! PM.MeshProfile_Ready ) return;
	//
	GB_OBJECT*	Gobj = &GBM.Gobj[ OBJID_FIG_0 ];
	if( Gobj->set ) {
		GBM.win.cw = GetDlgItem( IDC_MESHPROFILE_PICT);
		GBobj_Delete(&GBM,0);
	}
	GBobj_Init( Gobj,1000,UNDEF);
	//
	double	Hspan,Vspan,dH,X,Y,Z;

	int		no[4],No[4];
	int		pno,markspace;
	double	stX;
	//
	char	str[128];
	//
	Z = -1.;
	// AXIS
	Hspan = (double)(GBM.win.cWidth - 2.*MESHPROFILE_H_MARGIN);
	Vspan = (double)(GBM.win.cHeight - MESHPROFILE_VU_MARGIN - MESHPROFILE_VD_MARGIN);

	// main X -------------------------------------------------------------------------------------------
	X = MESHPROFILE_H_MARGIN; 
	Y = MESHPROFILE_VD_MARGIN; 
	Z = -1.;
	no[0] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);

	X = (double)(GBM.win.cWidth - MESHPROFILE_H_MARGIN); 
	Y = MESHPROFILE_VD_MARGIN; 
	Z = -1.;
	no[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,2.f,F_COLOR(0,0,0));

	markspace = 8;
	while( Mnum/markspace > 8 ) markspace += 8;
	//
	stX = MESHPROFILE_H_MARGIN;
	dX = Hspan/(Mnum-1); 
	for(int i=0;i< Mnum;i++) {
		X = stX+i*dX; 
		Y = MESHPROFILE_VD_MARGIN - 3.;
		if( i%markspace == 0 ) {
			if( MID[i].x != UNDEF ) {
				sprintf(str,"[%d,%d]",Mpos[i].x,Mpos[i].y);
				pno = GBobj_Add_Text(&GBM,0,DIM_3D,str,"Lucida Sans",FONT_REGULAR,18,8, 
						X-strlen(str)*4.,Y-20.,0.,F_COLOR(0,0,0));
			}
		}
		else if( i== Mnum-1 ) {
			if( MID[i].x != UNDEF ) {
				sprintf(str,"[%d,%d]",Mpos[Mnum-1].x,Mpos[Mnum-1].y);
				pno = GBobj_Add_Text(&GBM,0,DIM_3D,str,"Lucida Sans",FONT_REGULAR,18,8, 
						X-strlen(str)*4.,Y-40.,0.,F_COLOR(0,0,0));
			}
		}
		//
		if( i !=  0  ) {
			X = stX+i*dX; 
			Y = MESHPROFILE_VD_MARGIN - 3.;
			no[0] = GBobj_Add_Vert(Gobj,X,Y,-1.,FALSE);

			if( i%markspace == 0 )	 Y = GBM.win.cHeight - MESHPROFILE_VU_MARGIN;
			else					 Y = MESHPROFILE_VD_MARGIN + 3.;
			//
			no[1] = GBobj_Add_Vert(Gobj,X,Y,-1.,FALSE);
			GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,0.5f,F_COLOR(0,0,0));
		}
	}
	GBobj_Add_Text(&GBM,0,DIM_2D,"Mesh Location","Lucida Sans",FONT_BOLD,25,12,
				GBM.win.cWidth/2.-200.,MESHPROFILE_VD_MARGIN/2.-10.,-1.,F_COLOR(0,0,0));

	// main Y H -------------------------------------------------------------------------------------------
	X = MESHPROFILE_H_MARGIN;				// Left Vertical Axis
	Y = MESHPROFILE_VD_MARGIN; 
	Z = -1.;
	no[0] = GBobj_Add_Vert(Gobj,X,Y,Z,TRUE);

	X = MESHPROFILE_H_MARGIN; 
	Y = (double)(GBM.win.cHeight - MESHPROFILE_VU_MARGIN); 
	Z = -1.;
	no[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,1.5f,F_COLOR(0,0,150));

	X = (double)(GBM.win.cWidth - MESHPROFILE_H_MARGIN); 
	Y = MESHPROFILE_VD_MARGIN; 
	Z = -1.;
	no[0] = GBobj_Add_Vert(Gobj,X,Y,Z,TRUE);

	X = (double)(GBM.win.cWidth - MESHPROFILE_H_MARGIN); 
	Y = (double)(GBM.win.cHeight - MESHPROFILE_VU_MARGIN); 
	Z = -1.;
	no[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,1.5f,F_COLOR(0,0,150));

	double		cY,cV;
	CString		form;

	cY = MESHPROFILE_VD_MARGIN;
	stH = cV = (int)(Hmin/Hunit)*Hunit;
	cV = stH;
	for(int i=0;i<Hrip;i++) {
		X = MESHPROFILE_H_MARGIN-3.; 
		no[0] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);

		X = (double)(GBM.win.cWidth - MESHPROFILE_H_MARGIN);
		no[1] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);
		GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,0.5f,F_COLOR(0,0,150));

		form.Format("%%.%dlf",Hfm);
		sprintf(str,form,cV);
		X = (double)(MESHPROFILE_H_MARGIN - strlen(str)*10 -10);
		GBobj_Add_Text(&GBM,0,DIM_2D,str,"Lucida Sans",FONT_REGULAR,22,10, X,cY-5.,-1.,F_COLOR(0,0,150));
		//
		cY += (Vspan/(Hrip-1));
		cV += Hunit;
	}
	if( Data_Exist ) sprintf(str,"H (m)    %d/%2d/%2d  %d:%02d",
		CCT.GetYear(),CCT.GetMonth(),CCT.GetDay(),CCT.GetHour(),CCT.GetMinute());
	else             strcpy(str,"H (m)    Data Not Exist");
	GBobj_Add_Text(&GBM,0,DIM_2D,str,"Lucida Sans",FONT_BOLD,25,12,
				MESHPROFILE_H_MARGIN/2.+10.,GBM.win.cHeight-MESHPROFILE_VU_MARGIN+18.,-1.,F_COLOR(0,0,150));
	//
	// RIVER BASE & BANK HEIGHT --------------------------------------------------------------------------
	Z = 0.;
	//
	dX = Hspan/(Mnum-1);
	dH = Vspan/((Hrip-1)*Hunit);
	FCOLOR	Fcol;
	//
	// 浸水深
	if( Data_Exist ) {
		Fcol = F_COLOR( 0,0,220 );
		Z = 0.;
		for(int p=1;p<Mnum;p++) {
			if (!(MESH[MID[p - 1].y][MID[p - 1].x].flag && MESH[MID[p].y][MID[p].x].flag)) continue;
			//
			X = (p-1)*dX+MESHPROFILE_H_MARGIN;
			Y = (HWL[p-1] - stH)*dH + MESHPROFILE_VD_MARGIN;
			No[0] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
			//
			X = (p)*dX+MESHPROFILE_H_MARGIN;
			Y = (HWL[p] - stH)*dH + MESHPROFILE_VD_MARGIN;
			No[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
			//
			GBobj_Add_Poly(Gobj,2,No,GL_LINE_STRIP,1.5f,Fcol);
		}
	}
	//
	// 地盤高
	Fcol = F_COLOR( 255,150,30 );
	Z = 1.;
	for(int p=1;p<Mnum;p++) {
		if (!(MESH[MID[p - 1].y][MID[p - 1].x].flag && MESH[MID[p].y][MID[p].x].flag)) continue;
		//
		X = (p-1)*dX+MESHPROFILE_H_MARGIN;
		Y = (DEM[p-1] - stH)*dH + MESHPROFILE_VD_MARGIN;
		No[0] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
		//
		X = (p)*dX+MESHPROFILE_H_MARGIN;
		Y = (DEM[p] - stH)*dH + MESHPROFILE_VD_MARGIN;
		No[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
		//
		GBobj_Add_Poly(Gobj,2,No,GL_LINE_STRIP,1.5f,Fcol);
	}
	//
	// Legend
	double		XL,YL,YB=(double)(GBM.win.cHeight - MESHPROFILE_VU_MARGIN); 
	for(int i=0;i<2;i++) {
		if( i == 0 ) Fcol = F_COLOR( 0,0,220 );
		else         Fcol = F_COLOR( 255,150,30 );
		//
		XL = (double)(MESHPROFILE_H_MARGIN + GBM.win.cWidth/2.);
		YL = YB- (i+1)*GBM.win.cHeight/20.;
		No[0] =  GBobj_Add_Vert(Gobj,XL,YL,Z,FALSE);
		//
		XL += (double)GBM.win.cWidth/10.;
		YL = YB- (i+1)*GBM.win.cHeight/20.;
		No[1] =  GBobj_Add_Vert(Gobj,XL,YL,Z,FALSE);
		//
		if( i == 0 ) GBobj_Add_Poly(Gobj,2,No,GL_LINE_STRIP,2.f,Fcol);
		else         GBobj_Add_Poly(Gobj,2,No,GL_LINE_STRIP,1.f,Fcol);
		//
		XL += (double)GBM.win.cWidth/30.;
		switch( i ) {
			case 0: strcpy(str,"Inundation water level");break;
			case 1: strcpy(str,"Ground level");break;
		}
		GBobj_Add_Text(&GBM,0,DIM_2D,str,"Lucida Sans",FONT_REGULAR,22,10, XL,YL-5.,-1.,Fcol);
	}
	//
	if( ! GBM.cam.set ) {
		GB_Get_ViewWorld( &GBM );
		GBM.cam.stH = 0; GBM.cam.Dw = GBM.cam.endH = (double)GBM.win.cWidth;
		GBM.cam.stV = 0; GBM.cam.Dh = GBM.cam.endV = (double)GBM.win.cHeight;
		GB_Redraw(&GBM,FALSE,TRUE,TRUE,OBJID_FIG_0,TRUE);
		PM.Pro_Ready = TRUE;
	}
	else {
		GB_Redraw(&GBM,FALSE,FALSE,TRUE,OBJID_FIG_0,TRUE);
	}
	GB_Render_Scene(&GBM);
	return;
}
////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void CMeshProfile::OnPaint() {
	CPaintDC dc(this); 
	if( Fig_Ready ) Make_Profile();
}

void CMeshProfile::OnEnChangeEditHmin(){
	UDT;Hmin = atof( m_Hmin );
}

void CMeshProfile::OnEnChangeEditHmax() {
	UDT;Hmax = atof( m_Hmax );	
}

void CMeshProfile::OnBnClickedButtonRedraw() 
{	
	Get_Scale_Axis( &Hmin,&Hmax,&Hrip,&Hunit,&Hfm);
	Make_Profile();
}
