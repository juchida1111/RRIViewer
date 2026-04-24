// BREAK.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RRI.h"
#include "RRIView.h"
#include "BREAK.h"
#include "afxdialogex.h"
#include "RRI_Common.h"

#include "AS_Library\AS_Common.h"
#include "AS_Library\COLButton.h"
//_______________________________________________________________________________________________
/////////////////////////////////////////////////////////////////////////////////////////////////
extern      CBrush		m_brVU,m_brVD,m_brMU,m_brMD;	// 上下流域のタブ色
extern      CBrush		m_brBTN,m_brBTN_TEXT;			// ボタンの背景色ブラシ
extern      CBrush		m_brDlg,m_brDlg_TEXT;			// ダイアログの背景色ブラシ
extern      CBrush		m_brSTA,m_brSTA_TEXT;			// スタティック背景色ブラシ
extern      CBrush		m_brEDIT,m_brEDIT_TEXT;			// エディットボックス背景色ブラシ
extern      CBrush		m_brMAX,m_brMAX_TEXT;			// ? 背景色ブラシ

extern      int			CUD;							// 上下流モード
extern      PARAMS		Param[2];						// 上下流基本情報
extern      DATASET		DSET[2];						// 上下流ファイルセット

BUTTON_INFO*	BBS;
int				BBS_Num = 7;
int				BBS_ID[7]={ IDC_BUTTON_ADD_TIME,IDC_BUTTON_DEL_TIME,
							IDC_BUTTON_ADD_MESH,IDC_BUTTON_DEL_MESH,
							IDOK,IDOK_AS,IDCANCEL};
CString			BBS_Text[7] = {"Add Time","Delete Time","Add Mesh","Delete Mesh",
							"Save","Save As","Cancel"};
//
//
// CBTDLG ダイアログ

IMPLEMENT_DYNAMIC(CBTDLG, CDialog)

CBTDLG::CBTDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CBTDLG::IDD, pParent)
{

}

CBTDLG::~CBTDLG()
{
}

void CBTDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_NO, C_No);
	DDX_Control(pDX, IDC_STATIC_MV, C_MV);
	DDX_Control(pDX, IDC_STATIC_MH, C_MH);
	DDX_Control(pDX, IDC_STATIC_BT, C_BT);
	DDX_Control(pDX, IDC_STATIC_AP, C_AP);
}


BEGIN_MESSAGE_MAP(CBTDLG, CDialog)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CBTDLG::OnBnClickedOk)
	ON_BN_CLICKED(IDOK_AS, &CBTDLG::OnBnClickedAs)
	ON_BN_CLICKED(IDCANCEL, &CBTDLG::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TIME, &CBTDLG::OnBnClickedButtonAddTime)
	ON_BN_CLICKED(IDC_BUTTON_DEL_TIME, &CBTDLG::OnBnClickedButtonDelTime)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MESH, &CBTDLG::OnBnClickedButtonAddMesh)
	ON_BN_CLICKED(IDC_BUTTON_DEL_MESH, &CBTDLG::OnBnClickedButtonDelMesh)
END_MESSAGE_MAP()


// CBTDLG メッセージ ハンドラー
void CBTDLG::OnDestroy()
{
	if( BBS != NULL ) {
		for(int i=0;i<BBS_Num;i++) delete BBS[i].CB;
		free( BBS );
	}
	Free_All();
	//
	CDialog::OnDestroy();
}

void CBTDLG::PostNcDestroy() {delete this;}
//-----------------------------------------------------------------------------------------------------------
//  初期化
//-----------------------------------------------------------------------------------------------------------

BOOL CBTDLG::OnInitDialog()
{
	CDialog::OnInitDialog();
//---------------------------------------------------------------------
	Tnum = DSET[CUD].Break_Nums;
	Mnum = DSET[CUD].Break_Cells;
	for(int i=0;i<Tnum;i++) CopyTimes[i] = DSET[CUD].BreakTimes[i];
	for(int i=0;i<Mnum;i++) Copy_Coord(&CopySet[i],DSET[CUD].BreakSet[i]);
	//
//---------------------------------------------------------------------
//
// Buttons
	if( (BBS=(BUTTON_INFO*)malloc(BBS_Num*sizeof( BUTTON_INFO ))) == NULL ) return FALSE;
	for(int i=0;i<BBS_Num;i++) {
		BBS[i].id = BBS_ID[i];
		//
		BBS[i].C_B_BK		= COLOR_BUTTON_BK;
		BBS[i].C_B_BK_DIS	= COLOR_BUTTON_BK_DIS;
		BBS[i].C_TEXT		= COLOR_BUTTON_TEXT;
		BBS[i].C_TEXT_DIS	= COLOR_BUTTON_TEXT_DIS;
		//
		strcpy(BBS[i].Default_Cap,BBS_Text[i]);
		//
		BBS[i].CB =  new CCOLButton;
		if( BBS[i].CB->SubclassDlgItem(BBS[i].id,this) ) {
			BBS[i].CB->SetBackGroundColor(BBS[i].C_B_BK);
			BBS[i].CB->SetDisabledColor(BBS[i].C_TEXT_DIS);
			BBS[i].CB->SetTextColor(BBS[i].C_TEXT);
			BBS[i].CB->SetDisabledBkColor(BBS[i].C_B_BK_DIS);

			BBS[i].CB->RedrawWindow();
		}
	}
	SetTimer(0,10,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


HBRUSH CBTDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if( nCtlColor != CTLCOLOR_BTN) {
		if( CUD == UPPER ) 	{pDC->SetBkColor(COLOR_TAB_M_UPPER);hbr = static_cast<HBRUSH>(m_brMU);}
		else                {pDC->SetBkColor(COLOR_TAB_M_LOWER);hbr = static_cast<HBRUSH>(m_brMD);}
	}
	//
    if (nCtlColor == CTLCOLOR_STATIC)    {
        pDC->SetTextColor(COLOR_TEXT_WHITE);
		pDC->SetBkColor( COL_DARK_GREEN );
    }
    else if (nCtlColor == CTLCOLOR_EDIT)    {
		pDC->SetTextColor(COLOR_EDIT_TEXT);
		if( pWnd != CCC ) pDC->SetBkColor( COLOR_EDIT );
		else              pDC->SetBkColor( COLOR_FOCUS );
    }
    else if (nCtlColor == CTLCOLOR_BTN)    {
		pDC->SetBkColor(COLOR_BTN);
        pDC->SetTextColor(COLOR_BTN_TEXT);
    }
    else if (nCtlColor == CTLCOLOR_DLG)    {
        pDC->SetBkColor(COLOR_DIALOG);
        pDC->SetTextColor(COLOR_EDIT_TEXT);
    }
    else if (nCtlColor == CTLCOLOR_LISTBOX)    {
		pDC->SetBkColor( COLOR_EDIT );
        pDC->SetTextColor(COLOR_EDIT_TEXT);
    }
	return hbr;
}
//-----------------------------------------------------------------------------------------------------------
//  タイマー
//-----------------------------------------------------------------------------------------------------------

void CBTDLG::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == 0 ) {
		KillTimer( 0 );
	//---------------------------------------------------------
	//  数値のセットとデータコピー
	//
		if(! Init_Set_Edits( )) {
			MessageBox("** Error ** Initialize CEdit or Memory Allocation","Error",MB_OK);return;}
		//
		Trow = Mrow = UNDEF;
		CCC = NULL;
	//---------------------------------------------------------
	//  表示後のエディット枠のフォーカスを見つけるタイマー
		SetTimer( FORCUS_SEARCH_TIMER_BT,200,NULL);
	}
	else  { // FORCUS_SEARCH_TIMER
		Search_Edit_Focas();
	}


	CDialog::OnTimer(nIDEvent);
}
//-----------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  入力枠の作成
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------
void CBTDLG::Search_Edit_Focas()
{
	CWnd*	cw(this);
	CWnd*	tar;
	CDC*	pIM;
	HDC		hdc;
	//
	//for(int v=0;v<Vnum;v++ ) {
	//	if( (tar=cw->GetFocus()) == Times[v]) {
	//		Lrow = v;Lcol = UNDEF;
	//		CCC = (CWnd*)Times[v]; break;
	//	}
	//	for(int h=0;h<Hnum;h++) {
	//		if( tar == Edits[v][h]) {
	//			Lrow = v; Lcol = h;
	//			CCC = (CWnd*)Edits[v][h]; break;
	//		}
	//	}
	//}
	////
	//for(int v=0;v<Vnum;v++ ) {
	//	Times[v]->RedrawWindow();
	//	for(int h=0;h<Hnum;h++) 	Edits[v][h]->RedrawWindow();
	//}
	//
	return;
}
void  CBTDLG::Free_All()
{
	//  Uniqe
	//
	for(int h=0;h<Tnum;h++) {
		delete  Nos[h];
		delete  Times[h];
	}
	SAFE_FREE( No_ID );
	SAFE_FREE( Nos );
	SAFE_FREE( Times_ID );
	SAFE_FREE( Times );

	//
	for(int v=0;v<Mnum;v++) {
		delete  MXs[v];
		delete  MYs[v];
		delete  Apps[v];
	}
	SAFE_FREE( MX_ID );
	SAFE_FREE( MXs );
	SAFE_FREE( MY_ID );
	SAFE_FREE( MYs );
	SAFE_FREE( App_ID );
	SAFE_FREE( Apps );
	//
}
//-----------------------------------------------------------------------------------------------------------
//  エディター枠の作成
//-----------------------------------------------------------------------------------------------------------
BOOL	 CBTDLG::Init_Set_Edits()
{
	CRect	RNo,RTime,RMX,RMY,RAP;
	CRect	Wrect;
	CWnd*	cw;
	int		exY;
	//
	//
	// Window 枠の変更
	this->GetWindowRect( &Wrect );
	//
	// ウィンドウの大きさ設定と移動
	//
	C_No.GetWindowRect( &RNo );
	exY = RNo.bottom + 2;
	exY += Imax( Tnum,Mnum )*Y_BT_SIZE + 70;
	//
	Wrect.bottom = Wrect.top + exY;
	this->MoveWindow(&Wrect,TRUE);
	this->RedrawWindow();
	//
	this->GetClientRect( &Wrect );
	// Save Save As Cancel ボタンの移動
	// 
	CRect	BTrect,Mrect;
	//
	for(int i=0;i<BBS_Num;i++) {
		cw = GetDlgItem( BBS_ID[i] );
		cw->GetClientRect( &BTrect );
		if( i > 3 ) {
			Mrect.top  = Wrect.bottom - 61;  Mrect.bottom = Mrect.top + BTrect.Height();}
		else		{
			Mrect.top  = Wrect.bottom - 31;  Mrect.bottom = Mrect.top + BTrect.Height();}

		cw->MoveWindow(&Mrect,TRUE);
		cw->RedrawWindow();
	}
	//
	this->GetWindowRect( &Wrect );
	//
	// Data allocation  ID
	if( (No_ID=(int*)malloc(Tnum*sizeof( int ) )) == NULL ||
		(Times_ID=(int*)malloc(Tnum*sizeof( int ) )) == NULL ||
		(MX_ID=(int*)malloc(Mnum*sizeof( int ) )) == NULL ||
		(MY_ID=(int*)malloc(Mnum*sizeof( int ) )) == NULL ||
		(App_ID=(int*)malloc(Mnum*sizeof( int ) )) == NULL  ) return FALSE;

	//
	// ＩＤの埋め込み
	int  cid = ID_BT_INIT;
	for(int i=0;i<Tnum;i++) {No_ID[i] = cid++; Times_ID[i] = cid++;}
	for(int i=0;i<Mnum;i++) {MX_ID[i] = cid++;MY_ID[i] = cid++;App_ID[i] = cid++;}
	// Memory allocation  エディット枠
	//	
	if( (Nos = (CStatic**)malloc(Tnum*sizeof(void *))) == NULL ||
		(Times = (CEdit**)malloc(Tnum*sizeof(void *))) == NULL  ||
		(MXs = (CEdit**)malloc(Mnum*sizeof(void *))) == NULL  ||
		(MYs = (CEdit**)malloc(Mnum*sizeof(void *))) == NULL  ||
		(Apps = (CEdit**)malloc(Mnum*sizeof(void *))) == NULL  )  return FALSE;

	// エディット枠 の作成
	//
	C_BT.GetWindowRect( &RTime );
	C_MH.GetWindowRect( &RMX );
	C_MV.GetWindowRect( &RMY );
	C_AP.GetWindowRect( &RAP );
	//
	int		xst,yst,xsize,ysize;
	char	label[10];

	//  No
	xst = RNo.left;
	yst = RNo.bottom + 2;
	xsize = RNo.Width();
	ysize = RNo.Height();
	for(int i=0;i<Tnum;i++) {
		sprintf(label,"%d",i+1);
		Nos[i] = new CStatic;
		if(! Nos[i]->Create(label,WS_CHILD | WS_VISIBLE | WS_TABSTOP  | WS_BORDER | ES_CENTER,
							CRect(xst, yst, xst+xsize,yst+ysize), this, No_ID[i]) ) return FALSE;
		yst += (ysize + 1 );
	}
	//  Time
	xst = RTime.left;
	yst = RTime.bottom + 2;
	xsize = RTime.Width();
	ysize = RTime.Height();
	for(int i=0;i<Tnum;i++) {
		Times[i] = new CEdit;
		if(!Times[i]->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP  | WS_BORDER | ES_CENTER,
							CRect(xst, yst, xst+xsize,yst+ysize), this, Times_ID[i]) ) return FALSE;
		yst += (ysize + 1 );
	}
	//  Mesh H
	xst = RMX.left;
	yst = RMX.bottom + 2;
	xsize = RMX.Width();
	ysize = RMX.Height();
	for(int i=0;i<Mnum;i++) {
		MXs[i] = new CEdit;
		if(!MXs[i]->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP  | WS_BORDER | ES_CENTER,
							CRect(xst, yst, xst+xsize,yst+ysize), this, MX_ID[i]) ) return FALSE;
		yst += (ysize + 1 );
	}
	//  Mesh V
	xst = RMY.left;
	yst = RMY.bottom + 2;
	xsize = RMY.Width();
	ysize = RMY.Height();
	for(int i=0;i<Mnum;i++) {
		MYs[i] = new CEdit;
		if(!MYs[i]->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP  | WS_BORDER | ES_CENTER,
							CRect(xst, yst, xst+xsize,yst+ysize), this, MY_ID[i]) ) return FALSE;
		yst += (ysize + 1 );
	}
	//  App
	xst = RAP.left;
	yst = RAP.bottom + 2;
	xsize = RAP.Width();
	ysize = RAP.Height();
	for(int i=0;i<Mnum;i++) {
		Apps[i] = new CEdit;
		if(!Apps[i]->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP  | WS_BORDER | ES_CENTER,
							CRect(xst, yst, xst+xsize,yst+ysize), this, App_ID[i]) ) return FALSE;
		yst += (ysize + 1 );
	}

	// 値の設定
	//
	//
	for(int i=0;i<Tnum;i++ ) {
		cw = GetDlgItem( Times_ID[i] );
		Set_CWnd_Text(cw,CopyTimes[i],TRUE );
	}
	for(int i=0;i<Mnum;i++ ) {
		cw = GetDlgItem( MX_ID[i] );
		Set_CWnd_Text(cw,CopySet[i].x,TRUE );
		cw = GetDlgItem( MY_ID[i] );
		Set_CWnd_Text(cw,CopySet[i].y,TRUE );
		cw = GetDlgItem( App_ID[i] );
		Set_CWnd_Text(cw,(int)(CopySet[i].flag),TRUE );
	}
	//
	return TRUE;
}
//-----------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ボタン処理
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------
void CBTDLG::OnBnClickedButtonAddTime()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CBTDLG::OnBnClickedButtonDelTime()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CBTDLG::OnBnClickedButtonAddMesh()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CBTDLG::OnBnClickedButtonDelMesh()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}

//-----------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  終了処理
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------


void CBTDLG::OnBnClickedOk()
{
	((CRRIView*)this->GetParent())->Break_Manager();
	DestroyWindow();
}


void CBTDLG::OnBnClickedAs()
{
	((CRRIView*)this->GetParent())->Break_Manager();
	DestroyWindow();
}


void CBTDLG::OnBnClickedCancel()
{
	((CRRIView*)this->GetParent())->Break_Manager();
	DestroyWindow();
}


