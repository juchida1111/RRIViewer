// AnimOpt.cpp : 実装ファイル
//
#pragma once

#include "stdafx.h"
#include "RRI.h"
#include "AnimOpt.h"
#include "afxdialogex.h"
//_______________________________________________________________________________________________
/////////////////////////////////////////////////////////////////////////////////////////////////
extern      int			CV;					// ORTHO,PERS モード
extern		int			Image_Sub,Option_Sub;
//
extern      char			m_RecentPath[SIZE_1K];
extern      PARAMS		PM;

extern      CString		Fonts[12];

int			TSTYLE_NUM=3,TSTYLE_ID[3]={IDC_RADIO_TS1,IDC_RADIO_TS2,IDC_RADIO_TS3};
int			LSTYLE_NUM=3,LSTYLE_ID[3]={IDC_RADIO_LS1,IDC_RADIO_LS2,IDC_RADIO_LS3};
int			TFSIZE_NUM=2,TFSIZE_ID[2]={IDC_EDIT_TW,IDC_EDIT_TH};
int			LFSIZE_NUM=2,LFSIZE_ID[2]={IDC_EDIT_LW,IDC_EDIT_LH};
int			LEGSIZE_NUM=2,LEGSIZE_ID[2]={IDC_EDIT_LEG_WIDTH,IDC_EDIT_LEG_HEIGHT};
//
int			ADINFO_NUM=2,ADINFO_ID[2]={IDC_RADIO_REC1,IDC_RADIO_REC2};
//_______________________________________________________________________________________________
/////////////////////////////////////////////////////////////////////////////////////////////////
// CAnimOpt ダイアログ

IMPLEMENT_DYNAMIC(CAnimOpt, CDialogEx)

CAnimOpt::CAnimOpt(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAnimOpt::IDD, pParent)
{

	m_Head = _T("");
	//m_LegHeight = _T("");
	//m_LegWidth = _T("");
	m_LH = _T("");
	m_LW = _T("");
	m_TH = _T("");
	m_TW = _T("");
	m_Comment = _T("");
	m_ALmag = _T("");
	m_ALrate = _T("");
	m_ALhead = _T("");
}

CAnimOpt::~CAnimOpt()
{
}

void CAnimOpt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LFONT, m_Lfont);
	DDX_Control(pDX, IDC_COMBO_TFONT, m_Tfont);
	DDX_Text(pDX, IDC_EDIT_HEAD, m_Head);
	//DDX_Text(pDX, IDC_EDIT_LEG_HEIGHT, m_LegHeight);
	//DDX_Text(pDX, IDC_EDIT_LEG_WIDTH, m_LegWidth);
	DDX_Text(pDX, IDC_EDIT_LH, m_LH);
	DDX_Text(pDX, IDC_EDIT_LW, m_LW);
	DDX_Text(pDX, IDC_EDIT_TH, m_TH);
	DDX_Text(pDX, IDC_EDIT_TW, m_TW);
	DDX_Text(pDX, IDC_COMMENT, m_Comment);
	//  DDX_Control(pDX, IDC_BUTTON_FOLDER, m_cFolder);
	DDX_Text(pDX, IDC_EDIT_AL_MAG, m_ALmag);
	DDX_Text(pDX, IDC_EDIT_AL_RATE, m_ALrate);
	DDX_Text(pDX, IDC_EDIT_AL_HEAD, m_ALhead);
}


BEGIN_MESSAGE_MAP(CAnimOpt, CDialogEx)
	//ON_WM_CTLCOLOR()
	ON_STN_DBLCLK(IDC_COL_BACKPLATE, &CAnimOpt::OnStnDblclickColBackplate)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_BACKPLATE, &CAnimOpt::OnBnClickedCheckBackplate)
	ON_CBN_SELCHANGE(IDC_COMBO_TFONT, &CAnimOpt::OnCbnSelchangeComboTfont)
	ON_EN_CHANGE(IDC_EDIT_TW, &CAnimOpt::OnEnChangeEditTw)
	ON_EN_CHANGE(IDC_EDIT_TH, &CAnimOpt::OnEnChangeEditTh)
	ON_EN_CHANGE(IDC_EDIT_LW, &CAnimOpt::OnEnChangeEditLw)
	ON_EN_CHANGE(IDC_EDIT_LH, &CAnimOpt::OnEnChangeEditLh)
	//ON_EN_CHANGE(IDC_EDIT_LEG_WIDTH, &CAnimOpt::OnEnChangeEditLegWidth)
	//ON_EN_CHANGE(IDC_EDIT_LEG_HEIGHT, &CAnimOpt::OnEnChangeEditLegHeight)
	ON_CBN_SELCHANGE(IDC_COMBO_LFONT, &CAnimOpt::OnCbnSelchangeComboLfont)
	ON_BN_CLICKED(IDC_RADIO_TS1, &CAnimOpt::OnBnClickedRadioTs1)
	ON_BN_CLICKED(IDC_RADIO_TS2, &CAnimOpt::OnBnClickedRadioTs2)
	ON_BN_CLICKED(IDC_RADIO_TS3, &CAnimOpt::OnBnClickedRadioTs3)
	ON_BN_CLICKED(IDC_RADIO_LS1, &CAnimOpt::OnBnClickedRadioLs1)
	ON_BN_CLICKED(IDC_RADIO_LS2, &CAnimOpt::OnBnClickedRadioLs2)
	ON_BN_CLICKED(IDC_RADIO_LS3, &CAnimOpt::OnBnClickedRadioLs3)
	ON_EN_CHANGE(IDC_EDIT_HEAD, &CAnimOpt::OnEnChangeEditHead)
	ON_BN_CLICKED(IDC_RADIO_REC1, &CAnimOpt::OnBnClickedRadioRec1)
	ON_BN_CLICKED(IDC_RADIO_REC2, &CAnimOpt::OnBnClickedRadioRec2)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER, &CAnimOpt::OnBnClickedButtonFolder)
	ON_BN_CLICKED(IDOK, &CAnimOpt::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_AL_RATE, &CAnimOpt::OnEnChangeEditAlRate)
	ON_EN_CHANGE(IDC_EDIT_AL_MAG, &CAnimOpt::OnEnChangeEditAlMag)
	ON_BN_CLICKED(IDC_CHECK_INCLUDE, &CAnimOpt::OnBnClickedCheckInclude)
	ON_EN_CHANGE(IDC_EDIT_AL_HEAD, &CAnimOpt::OnEnChangeEditAlHead)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------------------------------------
//   CALL BACK
//-----------------------------------------------------------------------------------------------------------
int AFXAPI CAnimOpt::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED && lpData)
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	return 0;
}
//-----------------------------------------------------------------------------------------------------------
//   BUTTON_DIR
//-----------------------------------------------------------------------------------------------------------
BOOL	CAnimOpt::Button_Dir(CString* m_DirName,char* m_RecentPath )
{
	CWnd				*pWnd;
	static char		pszRetPath[CHARSIZE];
	LPMALLOC			pMalloc;
	//--------------------------------------
	//	ディレクトリ選択ダイアログの表示
	//--------------------------------------
	if (SUCCEEDED(SHGetMalloc(&pMalloc)))
	{ 
		char		cRetPath[CHARSIZE];
		ITEMIDLIST	*pList;
		BROWSEINFO	Info;

		pWnd = (CWnd*)AfxGetApp()->m_pMainWnd;
		HWND hWnd = pWnd->m_hWnd; 
		Info.hwndOwner = hWnd;
		Info. pidlRoot = 0;
		Info.pszDisplayName = cRetPath;
		Info.lpszTitle =  _T("フォルダを選択してください");
		Info.lpfn   = BrowseCallbackProc;
		Info.lParam = (LPARAM)m_RecentPath;
		Info.ulFlags = BIF_RETURNONLYFSDIRS;	//フォルダの選択のみ
		

		pWnd->ModifyStyle(WS_CHILD,WS_POPUP,0);

		//---  ダイアログ表示  -------------
		if( (pList = (ITEMIDLIST	*)SHBrowseForFolder(&Info)) == NULL ) {pMalloc->Release();return FALSE;}
		// ITEMIDLIST からフルパスを得る
		SHGetPathFromIDList(pList,pszRetPath);
		//ITEMIDLIST の開放
		pMalloc->Free(pList);
		//選択ディレクトリ表示
		*m_DirName = pszRetPath;
		strcpy(m_RecentPath,*m_DirName);
		pMalloc->Release();
		//
	}
	return TRUE;
}


BOOL CAnimOpt::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Ｐａｒａｍ　のコピー
	Option_Sub = Image_Sub;
	memcpy(&pm, &PM, sizeof(PARAMS));
	//
	this->SetWindowTextA("Model Animation Options");
//
	//
	SetTimer(0,30,NULL);
	return TRUE;  
}
//-----------------------------------------------------------------------------------------------------------
//   PreTranslateMessage
//-----------------------------------------------------------------------------------------------------------

BOOL CAnimOpt::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN  && pMsg->wParam == VK_RETURN ) {
		Disp_Buttons();return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
//===========================================================================================================
//   INIT SETUP in OnTimer
//===========================================================================================================
//
void CAnimOpt::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( 0 );
	//
	for(int i=0;i<12;i++) {
		m_Tfont.AddString( Fonts[i] );
		m_Lfont.AddString( Fonts[i] );
	}
	Disp_Buttons();
}

void CAnimOpt::Disp_Buttons()
{
	CButton*	cb=(CButton*)GetDlgItem( IDC_CHECK_BACKPLATE );
	CStatic*	cs=(CStatic*)GetDlgItem( IDC_COL_BACKPLATE );
	//
	if( pm.B_DateBack ) cb->SetCheck( 1 );
	else                cb->SetCheck( 0 );
	// バックカラー
	Fill_Color( (CWnd*)cs , pm.Col_DateBack );
	//
	// Arrow 
	cb=(CButton*)GetDlgItem( IDC_CHECK_INCLUDE );
	if( pm.B_IncludeRiver ) cb->SetCheck( 1 );
	else					cb->SetCheck( 0 );
	//
	m_ALhead.Format("%.1lf",pm.Arrow_Head);
	m_ALrate.Format("%.0lf",pm.Arrow_Ang);
	m_ALmag.Format("%.1lf",pm.Arrow_UserMag);
	//
	// Font
	m_Tfont.SetCurSel( pm.Tfont );
	m_Lfont.SetCurSel( pm.Lfont );
	//
	// Edit
	m_TW.Format("%d",pm.Tw);
	m_TH.Format("%d",pm.Th);
	m_LW.Format("%d",pm.Lw);
	m_LH.Format("%d",pm.Lh);
	//m_LegWidth.Format("%.0lf",pm.Size_Legend.x);
	//m_LegHeight.Format("%.0lf",pm.Size_Legend.y);
	////
	m_Head = pm.REC_Head;
	UDF;
	//
	// Radio
	for(int i=0;i<3;i++) {
		cb=(CButton*)GetDlgItem( TSTYLE_ID[i] );
		if( pm.Tstyle-1 == i ) cb->SetCheck( 1 );
		else                   cb->SetCheck( 0 );
		//
		cb=(CButton*)GetDlgItem( LSTYLE_ID[i] );
		if( pm.Lstyle-1 == i ) cb->SetCheck( 1 );
		else                   cb->SetCheck( 0 );
	}
	for(int i=0;i<ADINFO_NUM;i++) {
		cb=(CButton*)GetDlgItem( ADINFO_ID[i] );
		if( pm.Add_Info == i ) cb->SetCheck( 1 );
		else                   cb->SetCheck( 0 );
	}
	UDF;
	//
	// Radio
	cb=(CButton*)GetDlgItem( IDC_BUTTON_FOLDER );
	cb->SetWindowTextA( pm.REC_Folder );
}
void CAnimOpt::OnStnDblclickColBackplate()
{
	CColorDialog   CD;
	if( CD.DoModal() == IDCANCEL ) return;
	//
	pm.Col_DateBack = CD.GetColor();
	Disp_Buttons();
}
//===========================================================================================================
//   Response Message
//===========================================================================================================
//
void CAnimOpt::OnBnClickedCheckInclude() 
{
	UDT;
	CButton*	cb=(CButton*)GetDlgItem( IDC_CHECK_INCLUDE );
	if( cb->GetCheck() ) pm.B_IncludeRiver = TRUE;
	else                 pm.B_IncludeRiver = FALSE;
}

void CAnimOpt::OnEnChangeEditAlHead(){UDT;pm.Arrow_Head = atof(m_ALhead);Disp_Buttons();}

void CAnimOpt::OnEnChangeEditAlRate() {UDT;pm.Arrow_Ang = atof( m_ALrate );}
void CAnimOpt::OnEnChangeEditAlMag() {UDT;pm.Arrow_UserMag = atof( m_ALmag );}

void CAnimOpt::OnBnClickedCheckBackplate()
{
	UDT;
	CButton*	cb=(CButton*)GetDlgItem( IDC_CHECK_BACKPLATE );
	if( cb->GetCheck() ) pm.B_DateBack = TRUE;
	else                 pm.B_DateBack = FALSE;
}

void CAnimOpt::OnCbnSelchangeComboTfont()
{
	UDT;
	pm.Tfont = m_Tfont.GetCurSel();
	Disp_Buttons();
}
void CAnimOpt::OnEnChangeEditTw() {	UDT; pm.Tw = atoi( m_TW );}
void CAnimOpt::OnEnChangeEditTh() {	UDT; pm.Th = atoi( m_TH );}

void CAnimOpt::OnEnChangeEditLw() {	UDT; pm.Lw = atoi( m_LW );pm.Change_Data=TRUE;}
void CAnimOpt::OnEnChangeEditLh() {	UDT; pm.Lh = atoi( m_LH );pm.Change_Data=TRUE;}

//void CAnimOpt::OnEnChangeEditLegWidth()  {	UDT; pm.Size_Legend.x = atof( m_LegWidth );pm.Change_Data=TRUE;}
//void CAnimOpt::OnEnChangeEditLegHeight() { UDT; pm.Size_Legend.y = atof(m_LegHeight); pm.Change_Data = TRUE; }

void CAnimOpt::OnCbnSelchangeComboLfont()
{
	UDT;
	pm.Lfont = m_Lfont.GetCurSel();
	pm.Change_Data=TRUE;
	Disp_Buttons();
}


void CAnimOpt::OnBnClickedRadioTs1() {UDT;pm.Tstyle = 1;Disp_Buttons();}
void CAnimOpt::OnBnClickedRadioTs2() {UDT;pm.Tstyle = 2;Disp_Buttons();}
void CAnimOpt::OnBnClickedRadioTs3() {UDT;pm.Tstyle = 3;Disp_Buttons();}

void CAnimOpt::OnBnClickedRadioLs1() {UDT;pm.Lstyle = 1;Disp_Buttons();pm.Change_Data=TRUE;}
void CAnimOpt::OnBnClickedRadioLs2() {UDT;pm.Lstyle = 2;Disp_Buttons();pm.Change_Data=TRUE;}
void CAnimOpt::OnBnClickedRadioLs3() {UDT;pm.Lstyle = 3;Disp_Buttons();pm.Change_Data=TRUE;}

void CAnimOpt::OnEnChangeEditHead() {UDT;strcpy( pm.REC_Head,m_Head);}

void CAnimOpt::OnBnClickedRadioRec1() {UDT;pm.Add_Info = 0;Disp_Buttons();}
void CAnimOpt::OnBnClickedRadioRec2() {UDT;pm.Add_Info = 1;Disp_Buttons();}

void CAnimOpt::OnBnClickedButtonFolder()
{
	CString		fdir;
	if( ! Button_Dir(&fdir,m_RecentPath) ) {m_Comment="Canceled.....";UDF;return;}
	//
	CButton*	cb=(CButton*)GetDlgItem( IDC_BUTTON_FOLDER );
	strcpy( pm.REC_Folder,fdir);
	cb->SetWindowTextA( pm.REC_Folder );
}


void CAnimOpt::OnBnClickedOk()
{
	CDialogEx::OnOK();
}
