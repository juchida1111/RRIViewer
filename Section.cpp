// Section.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RRI.h"
#include "Section.h"
#include "afxdialogex.h"

#include  "CommFuncs.h"
#include  "CommGL.h"

#include	"RRI_Common.h"

CPoint		BastPoint;

int			SEC_Mnum = 8;
int			SEC_Mnum_ID[]={IDC_LABEL_1,IDC_LABEL_2,
IDC_EDIT_HMIN, IDC_EDIT_HMAX, IDC_EDIT_QMIN, IDC_EDIT_QMAX, IDC_BUTTON_REDRAW, IDC_BUTTON_EXPORT };

extern      DATASET		DSET;						// 上下流ファイルセット
extern      int			COUT;						// どの出力データを対象としているか

extern		int			Image_Sub,Option_Sub;
// Common Counter for HARD_COPY
extern      int			Cnt_Prof,Cnt_MProf,Cnt_Hydro;
extern      char			m_CurrentPath[SIZE_05K];
extern      char			m_RecentPath[SIZE_05K];

// CSection ダイアログ

IMPLEMENT_DYNAMIC(CSection, CDialog)

CSection::CSection(CWnd* pParent /*=NULL*/)
	: CDialog(CSection::IDD, pParent)
{

	m_Hmax = _T("");
	m_Hmin = _T("");
	m_Qmax = _T("");
	m_Qmin = _T("");
}

CSection::~CSection()
{
}

void CSection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HMAX, m_Hmax);
	DDX_Text(pDX, IDC_EDIT_HMIN, m_Hmin);
	DDX_Text(pDX, IDC_EDIT_QMAX, m_Qmax);
	DDX_Text(pDX, IDC_EDIT_QMIN, m_Qmin);
}
//---------------------------------------------------------------------------------
//   メッセージマップ
//---------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CSection, CDialog)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_HMIN, &CSection::OnEnChangeEditHmin)
	ON_EN_CHANGE(IDC_EDIT_HMAX, &CSection::OnEnChangeEditHmax)
	ON_EN_CHANGE(IDC_EDIT_QMIN, &CSection::OnEnChangeEditQmin)
	ON_EN_CHANGE(IDC_EDIT_QMAX, &CSection::OnEnChangeEditQmax)
	ON_BN_CLICKED(IDC_BUTTON_REDRAW, &CSection::OnBnClickedButtonRedraw)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CSection::OnBnClickedButtonExport)
END_MESSAGE_MAP()


//---------------------------------------------------------------------------------
//   初期化
//---------------------------------------------------------------------------------
// CSection メッセージ ハンドラー
BOOL CSection::OnInitDialog()
{
	Fig_Ready = FALSE;
	CDialog::OnInitDialog();
	if( WinTitle.GetLength() != 0 ) this->SetWindowTextA( WinTitle );
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
	cw = GetDlgItem( IDC_SECTION_PICT );
	cw->MoveWindow(8,30,W-20,H-65,FALSE);
	cw->RedrawWindow();
	//
//
	SetTimer(TEMP_TIMER,50,NULL);

	return TRUE; 
}


void CSection::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == TEMP_TIMER ) {
		KillTimer( TEMP_TIMER );
//		Fill_Color(this,RGB(120,120,120));
		//-------------------------------------------------------------------
		memset(&GB.win,0,sizeof( WININFO ) );
		GB.win.cw		= GetDlgItem( IDC_SECTION_PICT );
		GB.win.hWnd	= GB.win.cw->m_hWnd;
		GB.win.set = TRUE;
		//
		GB.win.cw->GetClientRect( &GB.win.Crect );
		GB.win.cw->GetClientRect( &GB.win.MAXrect );
		GB.win.cWidth	= GB.win.Crect.Width();
		GB.win.cHeight	= GB.win.Crect.Height();
		//
		GB.win.cw->GetWindowRect( &GB.win.Wrect );
		::ValidateRect(GB.win.hWnd, NULL );					// WM_PAINT の無効化
		//_____________________________________________________
		//  ＯＰＥＮＧＬ　初期化
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		GB.Gmax		= 2;		// ２レイヤー
		//
		if( FAILED( GB_Init( &GB ) ) ) {MessageBox("** ERROR ** Error Initialize Open_GL.... Init !","Missed",
						MB_OK|MB_ICONEXCLAMATION);return;}
		for(int i=0;i<GB.Gmax;i++) GB.Gobj[i].set = FALSE;

		// -------------------------------------
		GB_Init_Font(&GB);

		// Ａｔｔｒｉｂｕｔｅ -----------------------------------------------
		if( FAILED( GB_Attrib(&GB, DIM_3D, ORTHO, F_COLOR( 220, 220, 220 ),0  ) )) {
			MessageBox("** ERROR ** Error Initialize Open_GL.... Attrib!","Missed",MB_OK|MB_ICONEXCLAMATION);return;}
		//_____________________________________________________
		//  断面表示
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Make_Section();
		//----------------------------------------------------
		Fig_Ready = TRUE;
		SetTimer( GB_RENDER_TIMER,100,NULL);
	}
//_____________________________________________________
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//   RENDER TIMER
//_____________________________________________________
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if( nIDEvent == GB_RENDER_TIMER ) {
		KillTimer( GB_RENDER_TIMER );
		IF_STATB( REDRAW ) GB_Render_Scene(&GB);
	}
}
//---------------------------------------------------------------------------------
//   図化
//---------------------------------------------------------------------------------
// 
void CSection::Make_Section()
{
	GB_OBJECT*	Gobj = &GB.Gobj[ OBJID_FIG_0 ];
	GBobj_Init( Gobj,UNDEF,UNDEF);
	//
	double		Hspan=0.0,Vspan=0.0,dx=0.0;
	double		dH=0.0,dQ=0.0,Hmax=0.0,Qmax=0.0,Hmin=0.0,Qmin=0.0;
	double		Hunit=0.0,Qunit=0.0;
	int			NoH[1000],NoQ[1000],no[4];
	int			Hrip,Hfm,Qrip,Qfm,Tdisp,Tunit,id,pno;
	double		X,Y,Z,stH,stQ,dX,cY,cV,stT;
	CString		form;
	char		str[128];
	CWnd*	cw;
	//
	//~~~~~~~~~~~~~~~~~~ isRiver
	if (COUT == OUT_HR || COUT == OUT_QR) {
		if (isRiver) {
			Get_SuitScale(AXIS_SMOLL, Tnum, MESH->Hval[OUT_HR], &Hmin, &Hmax, &Hrip, &Hunit, &Hfm);
			if (Hmin == Hmax) { Hmin = 0.; Hmax = 2.; }
			//
			Get_SuitScale(AXIS_SMOLL, Tnum, MESH->Hval[OUT_QR], &Qmin, &Qmax, &Qrip, &Qunit, &Qfm);
			if (Qmin == Qmax) { Qmin = 0.; Qmax = 10.; }
			Qmin = 0.;
			Get_Scale_Axis(&Qmin, &Qmax, &Qrip, &Qunit, &Qfm);
		}
	}
	else	 if(COUT == OUT_HS ) {
		Get_SuitScale(AXIS_SMOLL, Tnum, MESH->Hval[OUT_HS], &Hmin, &Hmax, &Hrip, &Hunit, &Hfm);
		if( Hmin == Hmax ) {Hmin = 0.;Hmax = 2.;}
		//
		cw = GetDlgItem( IDC_EDIT_QMIN );
		cw->EnableWindow( FALSE );
		cw = GetDlgItem( IDC_EDIT_QMAX );
		cw->EnableWindow( FALSE );
	}
	//else	 if (COUT == OUT_QU || COUT == OUT_QV) {
	//	Get_SuitScale(AXIS_SMOLL, Tnum, MESH->Hval[OUT_QR], &Qmin, &Qmax, &Qrip, &Qunit, &Qfm);
	//	if (Qmin == Qmax) { Qmin = 0.; Qmax = 10.; }
	//	Qmin = 0.;
	//	Get_Scale_Axis(&Qmin, &Qmax, &Qrip, &Qunit, &Qfm);
	//}
		//
	//
	if( Hmin > 0. ) 	Hmin = 0.;
	if( Hmax < 2. )     Hmax = 2.;

	Get_Scale_Axis(&Hmin,&Hmax,&Hrip,&Hunit,&Hfm);
	//
	stH = ((int)(Hmin/Hunit))*Hunit;
	//~~~~~~~~~~~~~~~~~~ isRiver
	if ((COUT == OUT_HR || COUT == OUT_QR) && isRiver)  stQ = ((int)(Qmin / Qunit))*Qunit;
//
	form.Format("%%.%dlf",Hfm);
	m_Hmin.Format(form,stH);
	m_Hmax.Format(form,((Hrip-1)*Hunit)+stH);
	if ((COUT == OUT_HR || COUT == OUT_QR) && isRiver)  {
		form.Format("%%.%dlf",Qfm);
		m_Qmin.Format(form,stQ);
		m_Qmax.Format(form,((Qrip-1)*Qunit)+stQ);
	}
	else m_Qmin = m_Qmax = _T("");
	UDF;
	//
	// AXIS
	Hspan = (double)(GB.win.cWidth - 2.*SECTION_H_MARGIN);
	Vspan = (double)(GB.win.cHeight - SECTION_VU_MARGIN-SECTION_VD_MARGIN);
	dx = Hspan/Tnum;
	dH = Vspan/((Hrip-1)*Hunit);
	//~~~~~~~~~~~~~~~~~~ isRiver
	if ((COUT == OUT_HR || COUT == OUT_QR) && isRiver)  dQ = Vspan / ((Qrip - 1)*Qunit);

	// main X -------------------------------------------------------------------------------------------
	X = SECTION_H_MARGIN; Y = SECTION_VD_MARGIN; Z = -1.;
	no[0] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	X = (double)(GB.win.cWidth - SECTION_H_MARGIN); Y = SECTION_VD_MARGIN; Z = -1.;
	no[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,2.f,F_COLOR(0,0,0));

	Tdisp = 5;		// Fix
	if( (Tnum-1)%Tdisp == 0 ) Tunit = (Tnum-1)/Tdisp;
	else                      Tunit = (Tnum-1)/Tdisp+1;
	//
	CTime	ctm;
	int		dt = Hours / Tdisp;
	//
	stT = SECTION_H_MARGIN;
	dX = Hspan/Tdisp; 
	for(int i=0;i<Tdisp;i++) {
		if( (id=i*Tunit) < Tnum ) {
			ctm = st + CTimeSpan(0, dt*i, 0, 0);
			X = stT+i*dX; Y = SECTION_VD_MARGIN - 3.;
			sprintf(str, "%d/%d/%d", ctm.GetYear(), ctm.GetMonth(), ctm.GetDay());
			pno = GBobj_Add_Text(&GB, 0, DIM_3D, str, "Lucida Sans", FONT_REGULAR, 18, 8,
				X - strlen(str)*4., Y - 20., 0., F_COLOR(0, 0, 0));
			sprintf(str, "%d:%02d", ctm.GetHour(), ctm.GetMinute());
			pno = GBobj_Add_Text(&GB, 0, DIM_3D, str, "Lucida Sans", FONT_REGULAR, 18, 8,
				X - strlen(str)*4., Y - 42., 0., F_COLOR(0, 0, 0));
		}
		if( i > 0 && i < Tdisp ) {
			X = stT+i*dX; Y = SECTION_VD_MARGIN - 3.;
			no[0] = GBobj_Add_Vert(Gobj,X,Y,-1.,FALSE);
			Y = GB.win.cHeight - SECTION_VU_MARGIN;
			no[1] = GBobj_Add_Vert(Gobj,X,Y,-1.,FALSE);
			GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,0.5f,F_COLOR(0,0,0));
		}
	}
	GBobj_Add_Text(&GB,0,DIM_2D,"DATE","Lucida Sans",FONT_BOLD,25,12,
				GB.win.cWidth/2.-30.,SECTION_VD_MARGIN/2.-10.,-1.,F_COLOR(0,0,0));

	// main Y H -------------------------------------------------------------------------------------------
	X = SECTION_H_MARGIN; Y = SECTION_VD_MARGIN; Z = -1.;
	no[0] = GBobj_Add_Vert(Gobj,X,Y,Z,TRUE);
	X = SECTION_H_MARGIN; Y = (double)(GB.win.cHeight - SECTION_VU_MARGIN); Z = -1.;
	no[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,1.5f,F_COLOR(0,0,150));

	cY = SECTION_VD_MARGIN;
	cV = stH;
	for(int i=0;i<Hrip;i++) {
		X = SECTION_H_MARGIN-3.; 
		no[0] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);
		X = (double)(GB.win.cWidth - SECTION_H_MARGIN);
		no[1] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);
		GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,0.5f,F_COLOR(0,0,150));

		form.Format("%%.%dlf",Hfm);
		sprintf(str,form,cV);
		X = (double)(SECTION_H_MARGIN - strlen(str)*10 -10);
		GBobj_Add_Text(&GB,0,DIM_2D,str,"Lucida Sans",FONT_REGULAR,22,10, X,cY-5.,-1.,F_COLOR(0,0,150));
		//
		cY += (Vspan/(Hrip-1));
		cV += Hunit;
	}
	//~~~~~~~~~~~~~~~~~~ isRiver
	if ((COUT == OUT_HR || COUT == OUT_QR) && isRiver)
		GBobj_Add_Text(&GB,0,DIM_2D,"H (m)","Lucida Sans",FONT_BOLD,25,12,
					SECTION_H_MARGIN/2.+10.,GB.win.cHeight-SECTION_VU_MARGIN+30.,-1.,F_COLOR(0,0,150));
	else 
		GBobj_Add_Text(&GB,0,DIM_2D,"H (m)","Lucida Sans",FONT_BOLD,25,12,
					SECTION_H_MARGIN/2.+10.,GB.win.cHeight-SECTION_VU_MARGIN+30.,-1.,F_COLOR(0,0,150));
	// main Y Q -------------------------------------------------------------------------------------------
	X = (double)(GB.win.cWidth - SECTION_H_MARGIN); 
	Y = SECTION_VD_MARGIN; Z = -1.;
	no[0] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	X = (double)(GB.win.cWidth - SECTION_H_MARGIN); 
	Y = (double)(GB.win.cHeight - SECTION_VU_MARGIN); Z = -1.;
	no[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);

	//~~~~~~~~~~~~~~~~~~ isRiver
	if ((COUT == OUT_HR || COUT == OUT_QR) && isRiver)
				GBobj_Add_Poly(Gobj, 2, no, GL_LINE_STRIP, 1.5f, F_COLOR(150, 0, 0));
	else          GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,0.5f,F_COLOR(0,0,0));

	//~~~~~~~~~~~~~~~~~~ isRiver
	if ((COUT == OUT_HR || COUT == OUT_QR) && isRiver) {
//		if (isRiver)  {
		cY = SECTION_VD_MARGIN;
		cV = stQ;
		for(int i=0;i<Qrip;i++) {
			X = SECTION_H_MARGIN; 
			no[0] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);
			X = (double)(GB.win.cWidth - SECTION_H_MARGIN + 3.);
			no[1] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);
			GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,0.5f,F_COLOR(150,0,0));

			form.Format("%%.%dlf",Hfm);
			sprintf(str,form,cV);
			X += 3.;
			GBobj_Add_Text(&GB,0,DIM_2D,str,"Lucida Sans",FONT_REGULAR,22,10, X,cY-5.,-1.,F_COLOR(150,0,0));
			//
			cY += (Vspan/(Qrip-1));
			cV += Qunit;
		}
		GBobj_Add_Text(&GB,0,DIM_2D,"Q (m3/s)","Lucida Sans",FONT_BOLD,25,12,
			GB.win.cWidth - SECTION_H_MARGIN-50.,GB.win.cHeight-SECTION_VU_MARGIN+30.,-1.,F_COLOR(150,0,0));
	}
	//
	// -------------------------------------------------------------------------------------------
	for(int i=0;i<Tnum;i++) {
		X = dx*i + SECTION_H_MARGIN;
		//~~~~~~~~~~~~~~~~~~ isRiver
		if ((COUT == OUT_HR || COUT == OUT_QR) && isRiver)
			Y = dH*(MESH->Hval[OUT_HR][i] - stH) + SECTION_VD_MARGIN;
		else          Y = dH*(MESH->Hval[OUT_HS][i] - stH) + SECTION_VD_MARGIN;
		Z = 0.;
		NoH[i] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
		//~~~~~~~~~~~~~~~~~~ isRiver
		if ((COUT == OUT_HR || COUT == OUT_QR) && isRiver) {
			Y = dQ*(MESH->Hval[OUT_QR][i] - stQ) + SECTION_VD_MARGIN;
			NoQ[i] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
		}
	}
	GBobj_Add_Poly(Gobj,Tnum,NoH,GL_LINE_STRIP,2.f,F_COLOR(100,100,250));
	//~~~~~~~~~~~~~~~~~~ isRiver
	if ((COUT == OUT_HR || COUT == OUT_QR) && isRiver) 
		GBobj_Add_Poly(Gobj, Tnum, NoQ, GL_LINE_STRIP, 2.f, F_COLOR(250, 100, 100));
	//
	GB_Get_ViewWorld( &GB );
	GB.cam.stH = 0; GB.cam.Dw = GB.cam.endH = (double)GB.win.cWidth;
	GB.cam.stV = 0; GB.cam.Dh = GB.cam.endV = (double)GB.win.cHeight;


	GB_Redraw(&GB,FALSE,TRUE,TRUE,OBJID_FIG_0,TRUE);
}


void CSection::OnCancel()
{
	GB_Term( &GB );
	CDialog::OnOK();
}



//---------------------------------------------------------------------------------
//   SET
//---------------------------------------------------------------------------------

void CSection::OnEnChangeEditHmin() {UDT;}
void CSection::OnEnChangeEditHmax() {UDT;}
void CSection::OnEnChangeEditQmin() {UDT;}
void CSection::OnEnChangeEditQmax() {UDT;}
//---------------------------------------------------------------------------------
//   REDRAW
//---------------------------------------------------------------------------------

void CSection::OnBnClickedButtonRedraw()
{
	GB_OBJECT*	Gobj = &GB.Gobj[ OBJID_FIG_0 ];
	GBobj_Delete(&GB,OBJID_FIG_0);
	GBobj_Init( Gobj,UNDEF,UNDEF);
	//
	double		Hspan,Vspan,dx;
	double		dH,dQ,Hmax,Qmax,Hmin,Qmin;
	double		Hunit,Qunit;
	int			NoH[200],NoQ[200],no[4];
	int			Hrip,Hfm,Qrip,Qfm,Tdisp,Tunit,id,pno;
	double		X,Y,Z,stH,stQ,dX,cY,cV,stT;
	CString		form;
	char		str[128];
	//
	Hmin = atof( m_Hmin );
	Hmax = atof( m_Hmax );
	Get_Scale_Axis(&Hmin,&Hmax,&Hrip,&Hunit,&Hfm);
	//
	if( isRiver ) {
		Qmin = atof( m_Qmin );
		Qmax = atof( m_Qmax );
		Get_Scale_Axis(&Qmin,&Qmax,&Qrip,&Qunit,&Qfm);
	}
//
	//
	stH = ((int)(Hmin/Hunit))*Hunit;
	if( isRiver ) stQ = ((int)(Qmin/Qunit))*Qunit;
	//
	// AXIS
	Hspan = (double)(GB.win.cWidth - 2.*SECTION_H_MARGIN);
	Vspan = (double)(GB.win.cHeight - SECTION_VU_MARGIN-SECTION_VD_MARGIN);
	dx = Hspan/Tnum;
	dH = Vspan/((Hrip-1)*Hunit);
	if( isRiver ) dQ = Vspan/((Qrip-1)*Qunit);

	// main X -------------------------------------------------------------------------------------------
	X = SECTION_H_MARGIN; Y = SECTION_VD_MARGIN; Z = -1.;
	no[0] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	X = (double)(GB.win.cWidth - SECTION_H_MARGIN); Y = SECTION_VD_MARGIN; Z = -1.;
	no[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,2.f,F_COLOR(0,0,0));

	Tdisp = 5;		// Fix
	if( (Tnum-1)%Tdisp == 0 ) Tunit = (Tnum-1)/Tdisp;
	else                      Tunit = (Tnum-1)/Tdisp+1;

	//
	CTime	ctm;
	int		dt = Hours / Tdisp;

	stT = SECTION_H_MARGIN;
	dX = Hspan/Tdisp; 
	for(int i=0;i<Tdisp;i++) {
		if( (id=i*Tunit) < Tnum ) {
			ctm = st + CTimeSpan(0, dt*i, 0, 0);
			X = stT + i*dX; Y = SECTION_VD_MARGIN - 3.;
			sprintf(str, "%d/%d/%d", ctm.GetYear(), ctm.GetMonth(), ctm.GetDay());
			pno = GBobj_Add_Text(&GB, 0, DIM_3D, str, "Lucida Sans", FONT_REGULAR, 18, 8,
				X - strlen(str)*4., Y - 20., 0., F_COLOR(0, 0, 0));
			sprintf(str, "%d:%2d", ctm.GetHour(), ctm.GetMinute());
			pno = GBobj_Add_Text(&GB, 0, DIM_3D, str, "Lucida Sans", FONT_REGULAR, 18, 8,
				X - strlen(str)*4., Y - 42., 0., F_COLOR(0, 0, 0));
		}
		if( i > 0 && i < Tdisp ) {
			X = stT+i*dX; Y = SECTION_VD_MARGIN - 3.;
			no[0] = GBobj_Add_Vert(Gobj,X,Y,-1.,FALSE);
			Y = GB.win.cHeight - SECTION_VU_MARGIN;
			no[1] = GBobj_Add_Vert(Gobj,X,Y,-1.,FALSE);
			GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,0.5f,F_COLOR(0,0,0));
		}
	}
	GBobj_Add_Text(&GB,0,DIM_2D,"DATE","Lucida Sans",FONT_BOLD,25,12,
				GB.win.cWidth/2.-30.,SECTION_VD_MARGIN/2.-10.,-1.,F_COLOR(0,0,0));

	// main Y H -------------------------------------------------------------------------------------------
	X = SECTION_H_MARGIN; Y = SECTION_VD_MARGIN; Z = -1.;
	no[0] = GBobj_Add_Vert(Gobj,X,Y,Z,TRUE);
	X = SECTION_H_MARGIN; Y = (double)(GB.win.cHeight - SECTION_VU_MARGIN); Z = -1.;
	no[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,1.5f,F_COLOR(0,0,150));

	cY = SECTION_VD_MARGIN;
	cV = stH;
	for(int i=0;i<Hrip;i++) {
		X = SECTION_H_MARGIN-3.; 
		no[0] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);
		X = (double)(GB.win.cWidth - SECTION_H_MARGIN);
		no[1] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);
		GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,0.5f,F_COLOR(0,0,150));

		form.Format("%%.%dlf",Hfm);
		sprintf(str,form,cV);
		X = (double)(SECTION_H_MARGIN - strlen(str)*10 -10);
		GBobj_Add_Text(&GB,0,DIM_2D,str,"Lucida Sans",FONT_REGULAR,22,10, X,cY-5.,-1.,F_COLOR(0,0,150));
		//
		cY += (Vspan/(Hrip-1));
		cV += Hunit;
	}
	if( isRiver ) 
		GBobj_Add_Text(&GB,0,DIM_2D,"H (m)","Lucida Sans",FONT_BOLD,25,12,
					SECTION_H_MARGIN/2.+10.,GB.win.cHeight-SECTION_VU_MARGIN+30.,-1.,F_COLOR(0,0,150));
	else 
		GBobj_Add_Text(&GB,0,DIM_2D,"H (m)","Lucida Sans",FONT_BOLD,25,12,
					SECTION_H_MARGIN/2.+10.,GB.win.cHeight-SECTION_VU_MARGIN+30.,-1.,F_COLOR(0,0,150));
	// main Y Q -------------------------------------------------------------------------------------------
	X = (double)(GB.win.cWidth - SECTION_H_MARGIN); 
	Y = SECTION_VD_MARGIN; Z = -1.;
	no[0] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	X = (double)(GB.win.cWidth - SECTION_H_MARGIN); 
	Y = (double)(GB.win.cHeight - SECTION_VU_MARGIN); Z = -1.;
	no[1] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
	if( isRiver ) GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,1.5f,F_COLOR(150,0,0));
	else          GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,0.5f,F_COLOR(0,0,0));

	if( isRiver )  {
		cY = SECTION_VD_MARGIN;
		cV = stQ;
		for(int i=0;i<Qrip;i++) {
			X = SECTION_H_MARGIN; 
			no[0] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);
			X = (double)(GB.win.cWidth - SECTION_H_MARGIN + 3.);
			no[1] = GBobj_Add_Vert(Gobj,X,cY,Z,FALSE);
			GBobj_Add_Poly(Gobj,2,no,GL_LINE_STRIP,0.5f,F_COLOR(150,0,0));

			form.Format("%%.%dlf",Hfm);
			sprintf(str,form,cV);
			X += 3.;
			GBobj_Add_Text(&GB,0,DIM_2D,str,"Lucida Sans",FONT_REGULAR,22,10, X,cY-5.,-1.,F_COLOR(150,0,0));
			//
			cY += (Vspan/(Qrip-1));
			cV += Qunit;
		}
		GBobj_Add_Text(&GB,0,DIM_2D,"Q (m3/s)","Lucida Sans",FONT_BOLD,25,12,
			GB.win.cWidth - SECTION_H_MARGIN-50.,GB.win.cHeight-SECTION_VU_MARGIN+30.,-1.,F_COLOR(150,0,0));
	}
	//
	// -------------------------------------------------------------------------------------------
	for(int i=0;i<Tnum;i++) {
		X = dx*i + SECTION_H_MARGIN;
		if (isRiver) Y = dH*(MESH->Hval[OUT_HR][i] - stH) + SECTION_VD_MARGIN;
		else          Y = dH*(MESH->Hval[OUT_HS][i] - stH) + SECTION_VD_MARGIN;
		Z = 0.;
		NoH[i] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
		//
		if( isRiver )  {
			Y = dQ*(MESH->Hval[OUT_QR][i] - stQ) + SECTION_VD_MARGIN;
			NoQ[i] = GBobj_Add_Vert(Gobj,X,Y,Z,FALSE);
		}
	}
	GBobj_Add_Poly(Gobj,Tnum,NoH,GL_LINE_STRIP,2.f,F_COLOR(100,100,250));
	if( isRiver ) GBobj_Add_Poly(Gobj,Tnum,NoQ,GL_LINE_STRIP,2.f,F_COLOR(250,100,100));
	//
	GB_Get_ViewWorld( &GB );
	GB.cam.stH = 0; GB.cam.Dw = GB.cam.endH = (double)GB.win.cWidth;
	GB.cam.stV = 0; GB.cam.Dh = GB.cam.endV = (double)GB.win.cHeight;
	//

	GB_Redraw(&GB,FALSE,FALSE,TRUE,OBJID_FIG_0,TRUE);
	SetTimer( GB_RENDER_TIMER,100,NULL);
}


BOOL CSection::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam ==  VK_RETURN	) {
			OnBnClickedButtonRedraw();return TRUE;
		}
	}
	//---------------------------------------------
	// ハードコピー
	//---------------------------------------------
	if( pMsg->message == WM_KEYUP &&  pMsg->wParam == VK_SNAPSHOT ) {
		CString	fname;
		fname.Format("%s\\Hydrograph_%d.bmp",m_CurrentPath,Cnt_Hydro++);
		GB.win.cw		= GetDlgItem( IDC_SECTION_PICT );
		GB_Save_Window( &GB,fname);
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CSection::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( Fig_Ready )    Make_Section();
}

void CSection::OnBnClickedButtonExport()
{
	CString	fname;
	CTime	tt;

	if (!Button_File(FALSE, &fname, m_RecentPath, "Export HQ ", "csv")) return;
	//
	FILE*	fp = fopen(fname, "w");
	if (fp == NULL) {
		MessageBox("** Error ** Cannot open output file", "warning", MB_OK | MB_ICONASTERISK);
		return;
	}
	//
	fprintf(fp, "Cell [ %d %d ] H & Q output\n", yid, xid);
	fprintf(fp, "time,h (m),q (m3/s)\n");
	int dt = Hours / Tnum;
	tt = st;
	for (int t = 0; t <= Tnum; t++) {
		tt = st + CTimeSpan(0l, t*dt, 0, 0);
		fprintf(fp, "%d/%02d/%02d %d:%02d", tt.GetYear(), tt.GetMonth(),
			tt.GetDay(), tt.GetHour(), tt.GetMinute());
		if (isRiver) {
			if (DSET.out_use[OUT_HR] && DSET.out_use[OUT_QR])
				fprintf(fp, ",%.3lf,%.2lf\n", MESH->Hval[OUT_HR][t], MESH->Hval[OUT_QR][t]);
			else if (DSET.out_use[OUT_HR])
				fprintf(fp, ",%.3lf\n", MESH->Hval[OUT_HR][t]);
			else		fprintf(fp, ",,%.2lf\n", MESH->Hval[OUT_QR][t]);
		}
		else {
			if (DSET.out_use[OUT_HS] && DSET.out_use[OUT_HS])
				fprintf(fp, ",%.3lf,%.3lf,%.3lf\n", MESH->Hval[OUT_HS][t], MESH->Hval[OUT_QU][t],
												MESH->Hval[OUT_QV][t]);
			else if (DSET.out_use[OUT_HS])  fprintf(fp, ",%.3lf\n", MESH->Hval[OUT_HS][t]);
		}
	}
	fclose(fp);
}