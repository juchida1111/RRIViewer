// 
////////////////////////////////////////////////////////////////////////////////////////
//
//	OPTIONS.cpp : 実装ファイル
//
////////////////////////////////////////////////////////////////////////////////////////
//

#include	"stdafx.h"
#include	"RRI.h"
#include	"OPTIONS.h"
#include	"afxdialogex.h"
#include	<direct.h>
#include	<io.h>
#include	<fcntl.h>
#include	<sys/stat.h>

//_______________________________________________________________________________________________
/////////////////////////////////////////////////////////////////////////////////////////////////
extern      int			CV;					// ORTHO,PERS モード
extern      int			COUT;				// どのGISデータを対象としているか
extern      int			CSHAPE;				// 現在のシェイプ

//
extern		int			Image_Sub,Option_Sub;
extern      CBrush		m_brVU,m_brVD,m_brMU,m_brMD;	// 上下流域のタブ色
extern      CBrush		m_brBTN,m_brBTN_TEXT;			// ボタンの背景色ブラシ
extern      CBrush		m_brDlg,m_brDlg_TEXT;			// ダイアログの背景色ブラシ
extern      CBrush		m_brSTA,m_brSTA_TEXT;			// スタティック背景色ブラシ
extern      CBrush		m_brEDIT,m_brEDIT_TEXT;			// エディットボックス背景色ブラシ
extern      CBrush		m_brMAX,m_brMAX_TEXT;			// ? 背景色ブラシ
extern      CString		OUT_MENU_NAME[];
//
extern      char			m_RecentPath[SIZE_1K];
extern      PARAMS		PM;
extern      DATASET		DSET;						// 上下流ファイルセット
//
static int		Sub[6]={10,20,30,40,60,120};
//
extern      int				MAX_TEXTURE, LAT_SPAN,LNG_SPAN,DEM_H,DEM_V;
extern      double			TEX_START_LAT,TEX_START_LNG;
extern      double			DEM_LAT,DEM_LNG,DEM_SIZE;
extern      double			ONE_DEGREE_DISTANCE;
extern      char				IMAGE_FOLDER[];
//
//
//BUTTON_INFO*				BF;
//int			Option_Num = 7;
//int			Option_ID[] = {IDC_BUTTON_ADDSHAPE,IDC_BUTTON_SHPSAVE,IDC_BUTTON_DEL,IDC_BUTTON_CLEAR,
//							IDC_BUTTON_COLSAVE,IDOK,IDCANCEL};
//CString		Option_Text[] = {"Read Shape","File Save","Delete","All Clear","File save","Excute","Cancel"};

//int			RES_Num=3;
//int			ID_RES[3]={IDC_RADIO_LOW,IDC_RADIO_MIDDLE,IDC_RADIO_OTHER};
//int			DISP_Num=2;
//int			ID_DISP[2]={IDC_RADIO_SHOW,IDC_RADIO_HIDE};
//int			EXP_Num=2;
//int			ID_EXP[2]={IDC_RADIO_CONT,IDC_RADIO_MOSAIC};
int			UNIT_A_Num=2;
int			ID_UNIT_A[2]={IDC_RADIO_A_EQ,IDC_RADIO_A_GR};
//
int			PAIR_A_Num=9;
int			ID_PAIR_A[9][3] = {
								{IDC_A_LABEL_0,IDC_EDIT_A_0,IDC_COL_A_0},
								{IDC_A_LABEL_1,IDC_EDIT_A_1,IDC_COL_A_1},
								{IDC_A_LABEL_2,IDC_EDIT_A_2,IDC_COL_A_2},
								{IDC_A_LABEL_3,IDC_EDIT_A_3,IDC_COL_A_3},
								{IDC_A_LABEL_4,IDC_EDIT_A_4,IDC_COL_A_4},
								{IDC_A_LABEL_5,IDC_EDIT_A_5,IDC_COL_A_5},
								{IDC_A_LABEL_6,IDC_EDIT_A_6,IDC_COL_A_6},
								{IDC_A_LABEL_7,IDC_EDIT_A_7,IDC_COL_A_7},
								{IDC_A_LABEL_8,IDC_EDIT_A_8,IDC_COL_A_8}};

// OPTIONS ダイアログ

IMPLEMENT_DYNAMIC(OPTIONS, CDialogEx)

OPTIONS::OPTIONS(CWnd* pParent /*=NULL*/)
	: CDialogEx(OPTIONS::IDD, pParent)
{
	Ready = FALSE;
}

OPTIONS::~OPTIONS()
{
}

void OPTIONS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COLOR, c_ColCombo);
}


BEGIN_MESSAGE_MAP(OPTIONS, CDialogEx)
	ON_WM_TIMER()
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER, &OPTIONS::OnNMCustomdrawSlider)
	ON_EN_CHANGE(IDC_EDIT_A_0, &OPTIONS::OnEnChangeEditA0)
	ON_EN_CHANGE(IDC_EDIT_A_1, &OPTIONS::OnEnChangeEditA1)
	ON_EN_CHANGE(IDC_EDIT_A_2, &OPTIONS::OnEnChangeEditA2)
	ON_EN_CHANGE(IDC_EDIT_A_3, &OPTIONS::OnEnChangeEditA3)
	ON_EN_CHANGE(IDC_EDIT_A_4, &OPTIONS::OnEnChangeEditA4)
	ON_EN_CHANGE(IDC_EDIT_A_5, &OPTIONS::OnEnChangeEditA5)
	ON_EN_CHANGE(IDC_EDIT_A_6, &OPTIONS::OnEnChangeEditA6)
	ON_EN_CHANGE(IDC_EDIT_A_7, &OPTIONS::OnEnChangeEditA7)
	ON_EN_CHANGE(IDC_EDIT_A_8, &OPTIONS::OnEnChangeEditA8)
	//ON_STN_DBLCLK(IDC_COL_MESHLINE, &OPTIONS::OnStnDblclickColMeshline)
	//ON_STN_DBLCLK(IDC_COL_MESHRIVER, &OPTIONS::OnStnDblclickColMeshriver)
	//ON_STN_DBLCLK(IDC_COL_MESHBOUND, &OPTIONS::OnStnDblclickColMeshbound)
	ON_STN_DBLCLK(IDC_COL_A_0, &OPTIONS::OnStnDblclickColA0)
	ON_STN_DBLCLK(IDC_COL_A_1, &OPTIONS::OnStnDblclickColA1)
	ON_STN_DBLCLK(IDC_COL_A_2, &OPTIONS::OnStnDblclickColA2)
	ON_STN_DBLCLK(IDC_COL_A_3, &OPTIONS::OnStnDblclickColA3)
	ON_STN_DBLCLK(IDC_COL_A_4, &OPTIONS::OnStnDblclickColA4)
	ON_STN_DBLCLK(IDC_COL_A_5, &OPTIONS::OnStnDblclickColA5)
	ON_STN_DBLCLK(IDC_COL_A_6, &OPTIONS::OnStnDblclickColA6)
	ON_STN_DBLCLK(IDC_COL_A_7, &OPTIONS::OnStnDblclickColA7)
	ON_STN_DBLCLK(IDC_COL_A_8, &OPTIONS::OnStnDblclickColA8)
	ON_EN_CHANGE(IDC_EDIT_A_9, &OPTIONS::OnEnChangeEditA9)

	ON_BN_CLICKED(IDC_CHECK_FRAME, &OPTIONS::OnBnClickedCheckFrame)
	ON_STN_DBLCLK(IDC_COL_WLFRAME, &OPTIONS::OnStnDblclickColWlframe)
	ON_BN_CLICKED(IDC_RADIO_A_EQ, &OPTIONS::OnBnClickedRadioAEq)
	ON_BN_CLICKED(IDC_RADIO_A_GR, &OPTIONS::OnBnClickedRadioAGr)
	ON_BN_CLICKED(IDOK, &OPTIONS::OnBnClickedOk)
	//ON_EN_CHANGE(IDC_EDIT_HD_WIDTH, &OPTIONS::OnEnChangeEditHdWidth)
	//ON_EN_CHANGE(IDC_EDIT_HD_HEIGHT, &OPTIONS::OnEnChangeEditHdHeight)
	//ON_EN_CHANGE(IDC_EDIT_RIV_WIDTH, &OPTIONS::OnEnChangeEditRivWidth)
	//ON_EN_CHANGE(IDC_EDIT_RIV_HEIGHT, &OPTIONS::OnEnChangeEditRivHeight)
	//ON_EN_CHANGE(IDC_EDIT_AP_WIDTH, &OPTIONS::OnEnChangeEditApWidth)
	//ON_EN_CHANGE(IDC_EDIT_AP_HEIGHT, &OPTIONS::OnEnChangeEditApHeight)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOR, &OPTIONS::OnCbnSelchangeComboColor)
	ON_BN_CLICKED(IDC_BUTTON_COLSAVE, &OPTIONS::OnBnClickedButtonColsave)
	//ON_CBN_SELCHANGE(IDC_COMBO_SHAPE, &OPTIONS::OnCbnSelchangeComboShape)
	//ON_BN_CLICKED(IDC_BUTTON_DEL, &OPTIONS::OnBnClickedButtonDel)
	//ON_BN_CLICKED(IDC_BUTTON_CLEAR, &OPTIONS::OnBnClickedButtonClear)
	//ON_BN_CLICKED(IDC_BUTTON_SHPSAVE, &OPTIONS::OnBnClickedButtonShpsave)
	ON_STN_DBLCLK(IDC_COL_SP, &OPTIONS::OnStnDblclickColSp)
	ON_WM_PAINT()
	//ON_BN_CLICKED(IDC_BUTTON_ADDSHAPE, &OPTIONS::OnBnClickedButtonAddshape)
END_MESSAGE_MAP()


// OPTIONS メッセージ ハンドラー
void OPTIONS::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Disp_Buttons();
}
//-----------------------------------------------------------------------------------------------------------
//   CALL BACK
//-----------------------------------------------------------------------------------------------------------
int AFXAPI OPTIONS::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED && lpData)
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	return 0;
}
//-----------------------------------------------------------------------------------------------------------
//   BUTTON_DIR
//-----------------------------------------------------------------------------------------------------------
BOOL	OPTIONS::Button_Dir(CString* m_DirName,char* m_RecentPath )
{
	CWnd			*pWnd;
	static char		pszRetPath[CHARSIZE];
	LPMALLOC		pMalloc;
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

//-----------------------------------------------------------------------------------------------------------
//   INIT DIALOG
//-----------------------------------------------------------------------------------------------------------
BOOL OPTIONS::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//
	// Ｐａｒａｍ　のコピー
//	memcpy(&pm,&PM,sizeof( PARAMS ));
	Copy_PARAMS(&pm, PM,TRUE);
	Cdata = COUT;
	Cshape = CSHAPE;
	Delete_Shape = 0;
	Add_Shape   = UNDEF;
	//
	//
	for (int k = 0; k < 11; k++) {
		if (k >= pm.OUTCOL[Cdata].num) pm.OUTCOL[Cdata].val[k] = VAL_INFINIT;
	}

	// 透明度スライダー
	//
	this->SetWindowTextA("Model Display Options");
	//
	for (int i = 0; i < FILE_OUT_NUM; i++) 	c_ColCombo.AddString(OUT_MENU_NAME[i]);
	c_ColCombo.SetCurSel(Cdata);
	//
	SetTimer(0,30,NULL);
	//
	return TRUE;  // return TRUE unless you set the focus to a control
}
//-----------------------------------------------------------------------------------------------------------
//   PreTranslateMessage
//-----------------------------------------------------------------------------------------------------------
BOOL OPTIONS::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN  && pMsg->wParam == VK_RETURN ) {
		Renew_Legend();return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
//===========================================================================================================
//   INIT SETUP in OnTimer
//===========================================================================================================
//
void OPTIONS::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( 0 ); 
	//
	pm.Change_Display = pm.Change_DCOL = FALSE;
	pm.Change_Data = pm.Change_Image = pm.Change_Legend = pm.Change_Mesh = pm.Change_Time = FALSE;
	//
	for (int k = 0; k < 11; k++) {
		if (k >= PM.OUTCOL[Cdata].num) PM.OUTCOL[Cdata].val[k] = VAL_INFINIT;
	}
	//
	Ready = TRUE;
	Disp_Buttons();
	////
	//Disp_Color(Cdata);
}
//-----------------------------------------------------------------------------------------------------------
//   DISPLAY BUTTONS
//-----------------------------------------------------------------------------------------------------------
void OPTIONS::Disp_Buttons()
{
	CButton*		cb;
	CButton			*cb2,*cb4;
	CStatic 		*cs;
	CEdit			*ce;
	CString			str;
	//
	Cshape = UNDEF;
	//
	// 2つ口 チェックボタン
	//
	for(int i=0;i<2;i++) {	
		cb4 = (CButton*)GetDlgItem( ID_UNIT_A[i] );
		//
		if( i == (int)pm.OUTCOL[ Cdata ].Equal ) cb4->SetCheck( 0 );
		else									 cb4->SetCheck( 1 );
	}
	UDF;
	//
	//
	Disp_Color( Cdata );	
	//
	// 浸水深メッシュカラー
	Fill_Color( IDC_COL_WLFRAME , pm.Col_WLframe );
	cb = (CButton*)GetDlgItem( IDC_CHECK_FRAME );
	if( pm.B_WLframe )	cb->SetCheck( 1 );
	else					cb->SetCheck( 0 );
	//
}
//-----------------------------------------------------------------------------------------------------------
//   DISP_LEGEND COLOR
//-----------------------------------------------------------------------------------------------------------
void OPTIONS::Disp_Color( int ID )
{
	if( !Ready ) return;
	//
	c_ColCombo.SetCurSel( ID );
	//
	CWnd*			cw;
	CString			str,vals;
	CStatic 			*cs;
	CEdit			*ce,*ce0;
	CRect			rect;
	int				DID;

	// AREA LEGEND
	DID = ID;		// Ｕ，Ｖの場合はＵに統一する。
	if (ID == OUT_QV) DID = OUT_QU;
	if (ID == OUT_GV) DID = OUT_GU;
	//
	ce0 =  (CEdit*)GetDlgItem( IDC_EDIT_A_9 );
	for(int i=0;i<PAIR_A_Num;i++) {
		if( i == 0 ) {
			Set_Double_Str(pm.OUTCOL[DID].val[i], &str, 1);
			ce0->SetWindowTextA( str );
		}
		//
		//  i に対するものは　pm.OUTCOL[ Cdata ].num-1
		cs = (CStatic*)GetDlgItem( ID_PAIR_A[i][0] );
		ce = (CEdit*)GetDlgItem( ID_PAIR_A[i][1] );
		//
		if( i == 0 ){
			if (pm.OUTCOL[DID].Equal)	str = "<= h <";
			else							str= "< h <=";
		}
		else  if (i < pm.OUTCOL[DID].num - 1)	{
			Set_Double_Str(pm.OUTCOL[DID].val[i], &vals, 0);
			if (pm.OUTCOL[DID].Equal)	str.Format("%s <= h <", vals);
			else							str.Format("%s < h <=", vals);
		}
		else  if (i == pm.OUTCOL[DID].num - 1)	{
			Set_Double_Str(pm.OUTCOL[DID].val[i], &vals, 0);
			if (pm.OUTCOL[DID].Equal)	str.Format("%s <= ", vals);
			else							str.Format("%s < ", vals);
		}
		else               str = _T("");
		//
		cs->SetWindowTextA( str );
		//
		if (i < pm.OUTCOL[DID].num - 1)		Set_Double_Str(pm.OUTCOL[DID].val[i + 1], &vals, 1);
		else									vals = _T("");
		ce->SetWindowTextA(vals);
		//
		if (i <= pm.OUTCOL[DID].num)	Fill_Color(ID_PAIR_A[i][2], pm.OUTCOL[DID].Rcol[i]);
		else							Fill_Color(ID_PAIR_A[i][2],RGB(105,105,115));
		//
		cw = GetDlgItem(ID_PAIR_A[i][2]);
		cw->GetClientRect(&rect);
		cw->ValidateRect(&rect);
		//
		UDF;
	}
}
//-----------------------------------------------------------------------------------------------------------
//   FILL RECT
//-----------------------------------------------------------------------------------------------------------
void OPTIONS::Fill_Color(int  ID,COLORREF col)
{
	CWnd*	cw = GetDlgItem( ID );
	CDC*	dc = cw->GetDC();
	CRect	rect;
	//
	cw->GetClientRect( &rect );
	dc->FillSolidRect(&rect,col);
	//
	cw->ReleaseDC( dc );
	cw->ValidateRect(&rect);
}
//
//__________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// コントロールハンドラー
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//_____________________________________________________
//  RADIO BUTTONS  IMAGE
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
////_____________________________________________________
////  RADIO BUTTONS  DISPLAY in Animation
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void OPTIONS::OnBnClickedRadioCont()
//{
//	UDT;
//	if( pm.OUTCOL[Cdata].Mos ) {
//		pm.OUTCOL[Cdata].Mos = FALSE;
//		pm.Change_Data = pm.Change_Legend = TRUE;
//		//
//		Disp_Buttons();
//	}
//}
//
//void OPTIONS::OnBnClickedRadioMosaic()
//{
//	UDT;
//	if( ! pm.OUTCOL[Cdata].Mos ) {
//		pm.OUTCOL[Cdata].Mos = TRUE;
//		pm.Change_Data = pm.Change_Legend = TRUE;
//		//
//		Disp_Buttons();
//	}
//}

//_____________________________________________________
//  SLIDER  MODEL TRANSPARENCY
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void OPTIONS::OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	//
//	UDT;
//	int	  Pos = m_TRANS_SLIDER.GetPos();
//	pm.Alpha = (double)(10-Pos)/10.;
//	pm.Change_Image = TRUE;
//	// 
//	*pResult = 0;
//}
//_____________________________________________________
//  EDIT  SCALE OFFSET
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//_____________________________________________________
//  LEGEND  AREA
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double	 OPTIONS::Get_EditRect( int ID )
{
	return 0.;
	//char		buf[128];
	//int			len;
	//CEdit*		cw = (CEdit*)GetDlgItem( ID );
	//cw->GetLine(0,buf,128);
	////
	//len = strlen( buf );
	//if( len == 0 )		return VAL_INFINIT;
	//else				return atof( buf ); 
}
void OPTIONS::OnEnChangeEditA0() { UDT; }
void OPTIONS::OnEnChangeEditA1() { UDT; }
void OPTIONS::OnEnChangeEditA2()  { UDT; }
void OPTIONS::OnEnChangeEditA3() { UDT; }
void OPTIONS::OnEnChangeEditA4() { UDT; }
void OPTIONS::OnEnChangeEditA5()  { UDT; }
void OPTIONS::OnEnChangeEditA6()  { UDT; }
void OPTIONS::OnEnChangeEditA7()  { UDT; }
void OPTIONS::OnEnChangeEditA8() { UDT; }
void OPTIONS::OnEnChangeEditA9() { UDT; }
//_____________________________________________________
// COLOR
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//void OPTIONS::OnStnDblclickColMeshline()
//{
//	CColorDialog   CD;
//	if( CD.DoModal() == IDCANCEL ) return;
//	//
//	pm.Col_Mesh = CD.GetColor();
//	pm.Change_Mesh = TRUE;
//	Disp_Buttons();
//}
//
//void OPTIONS::OnStnDblclickColMeshriver()
//{
//	CColorDialog   CD;
//	if( CD.DoModal() == IDCANCEL ) return;
//	//
//	pm.Col_MeshR = CD.GetColor();
//	pm.Change_Mesh = TRUE;
//	Disp_Buttons();
//}
//
//void OPTIONS::OnStnDblclickColMeshbound()
//{
//	CColorDialog   CD;
//	if( CD.DoModal() == IDCANCEL ) return;
//	//
//	pm.Col_MeshB = CD.GetColor();
//	pm.Change_Mesh = TRUE;
//	Disp_Buttons();
//}
//_____________________________________________________
// COLOR  ARRAY  AREA
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void OPTIONS::OnCbnSelchangeComboColor()
{
	Cdata = c_ColCombo.GetCurSel();
	Disp_Color(Cdata);
}
void OPTIONS::Set_COLOR_Data(int no)
{
	CColorDialog   CD;
	if (CD.DoModal() == IDCANCEL) return;
	//
	int DID = Cdata;		// Ｕ，Ｖの場合はＵに統一する。
	if (Cdata == OUT_QV) DID = OUT_QU;
	if (Cdata == OUT_GV) DID = OUT_GU;
	//
	pm.OUTCOL[DID].Rcol[no] = CD.GetColor();
	pm.OUTCOL[DID].Fcol[no] = F_COLOR(GetRValue(pm.OUTCOL[DID].Rcol[no]),
		GetGValue(pm.OUTCOL[DID].Rcol[no]),
		GetBValue(pm.OUTCOL[DID].Rcol[no]));
	pm.Change_Data = pm.Change_Legend = TRUE;
	//
	Disp_Color(DID);
}
void OPTIONS::OnStnDblclickColA0() {Set_COLOR_Data(0);}
void OPTIONS::OnStnDblclickColA1()
{
	if (pm.OUTCOL[Cdata].num < 2) { return; }
	Set_COLOR_Data(1);
}
void OPTIONS::OnStnDblclickColA2()
{
	if (pm.OUTCOL[Cdata].num < 3) { return; }
	Set_COLOR_Data(2);
}
void OPTIONS::OnStnDblclickColA3()
{
	if (pm.OUTCOL[Cdata].num < 4) { return; }
	Set_COLOR_Data(3);
}
void OPTIONS::OnStnDblclickColA4()
{
	if (pm.OUTCOL[Cdata].num < 5) { return; }
	Set_COLOR_Data(4);
}
void OPTIONS::OnStnDblclickColA5()
{
	if (pm.OUTCOL[Cdata].num < 6)  { return; }
	Set_COLOR_Data(5);
}
void OPTIONS::OnStnDblclickColA6()
{
	if (pm.OUTCOL[Cdata].num < 7)  { return; }
	Set_COLOR_Data(6);
}
void OPTIONS::OnStnDblclickColA7()
{
	if (pm.OUTCOL[Cdata].num < 8)  { return; }
	Set_COLOR_Data(7);
}
void OPTIONS::OnStnDblclickColA8()
{
	if (pm.OUTCOL[Cdata].num < 9)  { return; }
	Set_COLOR_Data(8);
}


//_____________________________________________________
// WL Frame 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void OPTIONS::OnBnClickedCheckFrame()
{
	CButton*	cb = (CButton*)GetDlgItem( IDC_CHECK_FRAME );
	if( cb->GetCheck() ) pm.B_WLframe = TRUE;
	else                 pm.B_WLframe = FALSE;
	pm.Change_Data = TRUE;
}


void OPTIONS::OnStnDblclickColWlframe()
{
	CColorDialog   CD;
	if( CD.DoModal() == IDCANCEL ) return;
	//
	pm.Col_WLframe = CD.GetColor();
	pm.Change_Data = TRUE;
	Disp_Buttons();
}
//_____________________________________________________
//Symbol Equel
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void OPTIONS::OnBnClickedRadioAEq(){
	UDT;pm.OUTCOL[ Cdata ].Equal = TRUE;
	Disp_Buttons();
	pm.Change_Data = pm.Change_Legend = TRUE;
}

void OPTIONS::OnBnClickedRadioAGr(){
	UDT;pm.OUTCOL[ Cdata ].Equal = FALSE;Disp_Buttons();
	pm.Change_Data = pm.Change_Legend = TRUE;
}

void OPTIONS::OnBnClickedOk()
{
	Renew_Legend();
	//
	CDialogEx::OnOK();
}

//_____________________________________________________
//Window Size
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//void OPTIONS::OnEnChangeEditHdWidth()	{UDT;WIN_W[WIN_HYDRO]=atoi(m_HD_W);}
//void OPTIONS::OnEnChangeEditHdHeight()	{UDT;WIN_H[WIN_HYDRO]=atoi(m_HD_H);}
//void OPTIONS::OnEnChangeEditRivWidth()	{UDT;WIN_W[WIN_R_PROF]=atoi(m_RP_W);}
//void OPTIONS::OnEnChangeEditRivHeight()	{UDT;WIN_H[WIN_R_PROF]=atoi(m_RP_H);}
//void OPTIONS::OnEnChangeEditApWidth()	{UDT;WIN_W[WIN_A_PROF]=atoi(m_AP_W);}
//void OPTIONS::OnEnChangeEditApHeight()	{UDT;WIN_H[WIN_A_PROF]=atoi(m_AP_H);}
//_____________________________________________________
// Save File
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void OPTIONS::OnBnClickedButtonColsave()
{
	Renew_Legend();
	Disp_Buttons();
	//
	if( ! Save_Color_Data( Boot_Folder,&pm ) ) return; 
	//
	return;
}

//---------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
//   SHAPE MANAGING
////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------

//void OPTIONS::OnCbnSelchangeComboShape()
//{
//	int		id = m_CShape.GetCurSel();
//	Cshape = IDno[ id ];		// id=0 IDno=UNDEF; id=1 IDno=0
//	Fill_Color(IDC_COL_SP,pm.SBOX[ Cshape ].Col);
//	pm.Change_Mesh = TRUE;
//}
//
//void OPTIONS::OnBnClickedButtonDel()
//{
//	if( Cshape == UNDEF ) return;
//	pm.SBOX[ Cshape ].set = FALSE;
//	Delete_Shape++;
//	Disp_Buttons();
//	pm.Change_Mesh = TRUE;
//}
//
//void OPTIONS::OnBnClickedButtonClear()
//{
//	for(int i=0;i<pm.Snum;i++) pm.SBOX[i].set = FALSE;
//	Delete_Shape = UNDEF;
//	Disp_Buttons();
//	pm.Change_Mesh = TRUE;
//}
//
//void OPTIONS::OnBnClickedButtonShpsave()
//{
//	if( Cshape == UNDEF ) return;
//	//
//	CString		Folder,fname;
//	char		Path[SIZE_05K];
//	//
//	Folder.Format("%s\\SHAPES",DSET.Proj_Folder);
//	if( ! PathIsDirectory( Folder ) ) ::mkdir( Folder );
//	strcpy(Path,Folder);
//	//
//	fname.Format("%s.sdd",pm.SBOX[Cshape].name);
//	if( !Button_File(FALSE,&fname,Path,"Save SDD","sdd") ) {
//		m_Comment="Cancel....";UDF;return;}
//	//
//	int		fc;
//	if((fc=_open(fname,_O_BINARY|_O_WRONLY|_O_CREAT,_S_IREAD | _S_IWRITE)) == -1 ) return;
//	//
//	write(fc,(void*)&pm.SBOX[Cshape],sizeof( SHAPE_BOX ) );
//	write(fc,(void*)pm.SBOX[Cshape].XZ,sizeof( XYCOORD )*pm.SBOX[Cshape].Vnum );
//	write(fc,(void*)pm.SBOX[Cshape].MID,sizeof( int )*pm.SBOX[Cshape].Mnum);
//	close( fc );	
//}
////
////  シェイプの追加
////
//void OPTIONS::OnBnClickedButtonAddshape()
//{
//	CString			Sname,ttl,Sn;
//	int					Mnum,Mmem;
//	int					Shapes,*Snum;
//	XYCOORD**		SXZ;
//	int*				SMid;
//	//
//	if( ! Button_File(TRUE,&Sname,m_RecentPath,"Shape file","shp") ) return;
//	Get_Title(Sname, &ttl);
//	Sn = ttl.Left( ttl.GetLength() - 4 );
//	//_____________________________________________________________________________
//	// 　複数のシェイプに対応
//	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	//
//	SHAPE_HEAD			shead;
//	RECORD_HEAD		rhead;
//	DATA_HEAD			dhead;
//	int						fp;
//	int						Code,Filelen;
//	int						Rno,Clen,TClen;
//	//------------------------------------------------------------
//	// シェイプメインファイルオープンと判断
//	//------------------------------------------------------------
//	if((fp = _open( Sname,_O_BINARY|_O_RDONLY,0)) == -1 ) return;
//	//
//	// SHAPE HEADER
//	if(_read(fp,(void*)&shead,sizeof( SHAPE_HEAD )) != sizeof( SHAPE_HEAD ) ) return;
//	if(!(shead.type == 3 || shead.type == 5) ) {_close( fp );return;	}
//	//
//	Code = Big_to_Little_I( shead.code );
//	if( Code != 9994 ) {_close( fp );return;	}
//	//
//	Filelen = Big_to_Little_I( shead.file_length ) * 2;		// WORD数 定義
//	//-------------------------------------------------------------------------------------------------------------------------------------------
//	/*	
//		ここでは、複数レコードを読むことはできるが、1レコード中の複数パートには対応していない。
//		したがって、part が１以外の場合は読み込みを中止する。
//
//		シェイプは　マトリックスで読み込む Mmem × Smem[m](ｍごとに可変）
//		とりあえず Mmem＝１０で確保しておく
//	*/
//	Mmem = 10;
//	SXZ = (XYCOORD**)malloc(Mmem*sizeof( XYCOORD*));
//	if( SXZ == NULL) {_close( fp );return;	}
//	Snum = (int*)malloc(Mmem*sizeof( int ));
//
//	//
//	//-------------------------------------------------------------------------------------------------------------------------------------------
//	int						part,part_num;
//	int						Vmem,Vnum;
//	int*					no;
//	//
//	Vmem = 10000;
//	POINT_DATA*		vert = (POINT_DATA*)malloc(Vmem*sizeof( POINT_DATA));
//	if( vert == NULL ) {SAFE_FREE( SXZ ); close( fp );return;	}
//	//
//	TClen = 100;			// shead 分
//	Shapes = 0;
//	//
//	while( TRUE ) {
//		//
//		/*	
//				typedef	struct _RECORD_HEAD {
//					BYTE		no[4];
//					BYTE		clen[4];
//				} RECORD_HEAD;
//		*/
//		if(_read(fp,(void*)&rhead,sizeof( RECORD_HEAD )) != sizeof( RECORD_HEAD ) ) break;
//		//
//		Rno = Big_to_Little_I( rhead.no );
//		Clen = Big_to_Little_I( rhead.clen )*2;
//		TClen += Clen;
//		//
//		/*	
//				typedef struct  _DATA_HEAD {
//					int			type;			
//					BYTE		area[32];
//					int			Numpart;
//					int			Numvert;
//				} DATA_HEAD;
//		*/
//		if(_read(fp,(void*)&dhead,sizeof(DATA_HEAD )) != sizeof( DATA_HEAD ) ) {
//			while( Shapes >= 0 ) SAFE_FREE( SXZ[ Shapes-- ]);
//			SAFE_FREE( SXZ ); SAFE_FREE( vert );_close( fp ); return;
//		}
//	//-------------------------------------------------------------------------------------------------------------------------------------------
//		if( dhead.Numpart != 1 ) {
//			while( Shapes >= 0 ) SAFE_FREE( SXZ[ Shapes-- ]);
//			SAFE_FREE( SXZ ); SAFE_FREE( vert );_close( fp ); return;}		// free SXZ ?
//		//
//		Snum[ Shapes ] = dhead.Numvert;
//		if( (SXZ[Shapes] = (XYCOORD*)malloc(dhead.Numvert*sizeof( XYCOORD ) ) ) == NULL ) {
//			while( Shapes >= 0 ) SAFE_FREE( SXZ[ Shapes-- ]);
//			SAFE_FREE( SXZ ); SAFE_FREE( vert );_close( fp ); return;}
//		//
//		if( dhead.Numvert >= Vmem ) {
//				if((vert=(POINT_DATA*)realloc(vert, (dhead.Numvert+100)*sizeof( POINT_DATA ))) == NULL ) {
//					_close( fp ); return;
//				}
//			Vmem = dhead.Numvert+100;
//		}
//	//-------------------------------------------------------------------------------------------------------------------------------------------
//		// 読み込み
//		//
//		if(_read(fp,(void*)&part,sizeof(int)) != sizeof(int) ) {
//			while( Shapes >= 0 ) SAFE_FREE( SXZ[ Shapes-- ]);
//			SAFE_FREE( SXZ ); SAFE_FREE( vert );_close( fp ); return;}	
//		if(_read(fp,(void*)vert,sizeof(POINT_DATA)*dhead.Numvert) != sizeof(POINT_DATA)*dhead.Numvert) {
//			while( Shapes >= 0 ) SAFE_FREE( SXZ[ Shapes-- ]);
//			SAFE_FREE( SXZ ); SAFE_FREE( vert );_close( fp ); return;}
//	//-------------------------------------------------------------------------------------------------------------------------------------------
//		// 置き換え
//		// メッシュの開始点は DEM_*** ではなく、Ademの開始点を使用する。
//		for(int i=0;i<dhead.Numvert;i++){
//			SXZ[Shapes][i].x = (vert[i].x - pm.stLNG )*ONE_DEGREE_DISTANCE;
//			SXZ[Shapes][i].y = (vert[i].y - pm.stLAT )*ONE_DEGREE_DISTANCE;
//		}
//		Shapes++;
//	}
//	_close( fp );
//	free( vert );
//	if( Shapes == 0 ) {
//		while( Shapes >= 0 ) SAFE_FREE( SXZ[ Shapes-- ]);
//		SAFE_FREE( SXZ ); return;
//	}
//	//																		シェイプ設定終了
//	//=====================================================================
//	//
//	// 　仮のメッシュ領域　確保    Shapes だけのラベルを作成する
//	//
//	//=====================================================================
//	//
//	double			Area;
//	double			unit=pm.Size*ONE_DEGREE_DISTANCE;
//	int				SMnum, SMmem = 10000;
//	// メッシュIDを探す
//	XYCOORD		vmin,vmax;
//	int				xst,xend,zst,zend;		// バウンダリー範囲
//	double			X,Z;
//	//
//	SMid = (int*)malloc(SMmem*sizeof( int ));		// メッシュID
//	//
//	for(int S=0;S<Shapes;S++ ) {
//		Find_Min_Max(Snum[S],SXZ[S],&vmin,&vmax);
//
//		xst = (int)(vmin.x/(pm.Size*ONE_DEGREE_DISTANCE))-1;  xst = ( xst > 0 )? xst:0;
//		zst = (int)(vmin.y/(pm.Size*ONE_DEGREE_DISTANCE))-1;  zst = ( zst > 0 )? zst:0;
//		xend = (int)(vmax.x/(pm.Size*ONE_DEGREE_DISTANCE))+1;  
//		xend = ( xend < pm.Hnum-1 )? xend:pm.Hnum-1;
//		zend = (int)(vmax.y/(pm.Size*ONE_DEGREE_DISTANCE))+1;  
//		zend = ( zend < pm.Vnum-1 )? zend: pm.Vnum-1;
//		//
//		SMnum = 0;
//		for(int v=zst;v<=zend;v++) {
//			Z = ((double)v + 0.5)*(pm.Size*ONE_DEGREE_DISTANCE);
//			for(int h=xst;h<=xend;h++) {
//				X = ((double)h + 0.5)*(pm.Size*ONE_DEGREE_DISTANCE);
//				if( Is_InRegion(X,Z,Snum[S],SXZ[S]) ) 	SMid[SMnum++] = v*pm.Hnum + h;
//				//
//				if( SMnum == SMmem ) {
//					SMmem += 100;
//					SMid = (int*)realloc(SMid,SMmem*sizeof( int ));
//				}
//			}
//		}
//		Area = Get_Area(Snum[S],SXZ[S])/10000.;
//		//_______________________________________________________
//		// 　シェイプ登録
//		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//		SHAPE_BOX*	  SBOX=&pm.SBOX[pm.Snum];
//		Sname.Format("%s_%d",Sn,S+1);
//		strcpy( SBOX->name,Sname);
//
//		SBOX->Vnum = Snum[S];
//		if( (SBOX->XZ=(XYCOORD*)malloc(Snum[S]*sizeof( XYCOORD ))) == NULL )  return;
//		for(int i=0;i<Snum[S];i++) {SBOX->XZ[i].x = SXZ[S][i].x;  SBOX->XZ[i].y = SXZ[S][i].y;}
//
//		SBOX->Mnum = SMnum;
//		if( (SBOX->MID=(int*)malloc(SMnum*sizeof( int ))) == NULL ) {
//			while( Shapes >= 0 ) SAFE_FREE( SXZ[ Shapes-- ]);
//			SAFE_FREE( SXZ ); 
//			SAFE_FREE( SBOX->XZ ); return;}
//		for(int i=0;i<SMnum;i++) {SBOX->MID[i] = SMid[i];}
//
//		//
//		// Color
//		int							cval[3];
//		for(int k=0;k<3;k++ ) cval[k] = (rand()%100) + 150;
//		SBOX->Col = RGB( cval[0],cval[1],cval[2] );
//		SBOX->Area = Area;
//		sprintf(SBOX->info," --[ %.1lf km2]",Area);
//		//
//		SBOX->set = TRUE;
//		//
//		Add_Shape = pm.Snum;
//		//
//		pm.Snum++;
//		if( pm.Snum == pm.Smem ){
//			pm.Smem += 10;
//			pm.SBOX = (SHAPE_BOX*)realloc( pm.SBOX,pm.Smem*sizeof( SHAPE_BOX ));
//			for(int s=pm.Snum;s<pm.Smem;s++) memset((void*)&pm.SBOX[s],0,sizeof( SHAPE_BOX ) );
//		}
//	}
//	//
//	Disp_Buttons();
//	pm.Change_Mesh = TRUE;
//	//
//	return;
//}
//
void OPTIONS::OnStnDblclickColSp()
{
	if( Cshape  == UNDEF ) return;
	//
	CColorDialog   CD;
	if( CD.DoModal() == IDCANCEL ) return;
	//
	pm.SBOX[ Cshape ].Col = CD.GetColor();
	Disp_Buttons();
	pm.Change_Mesh = TRUE;
}

void OPTIONS::Renew_Legend()
{
	CString			str;
	CStatic 			*cs;
	CEdit			*ce, *ce0;
	double			val;
	int				DID;
	// 
	DID = Cdata;		// Ｕ，Ｖの場合はＵに統一する。
	if (Cdata == OUT_QV) DID = OUT_QU;
	if (Cdata == OUT_GV) DID = OUT_GU;
	//
	int		Num = pm.OUTCOL[DID].num;
	//
	ce0 = (CEdit*)GetDlgItem(IDC_EDIT_A_9);
	for (int i = 0; i<PAIR_A_Num; i++) {
		if (i == 0) {
			ce0->GetWindowTextA(str);
			pm.OUTCOL[DID].val[i] = atof(str);
		}
		//--------------------------------------------------------- color は変更はできている。
		ce = (CEdit*)GetDlgItem(ID_PAIR_A[i][1]);
		//
		ce->GetWindowTextA(str);
		if (str.GetLength() != 0) {
			val = atof(str);
			if (val < pm.OUTCOL[DID].val[i]) {  // ０では１の編集 i=5 では６の編集
				Num = i; break;
			}
			pm.OUTCOL[DID].val[i + 1] = val;
		}
		else {
			Num = i+1; break;
		}
			
	}
	pm.OUTCOL[DID].num = Num;
	//
	if (Cdata == OUT_QV || Cdata == OUT_GV) {
		for (int i = 0; i < pm.OUTCOL[DID].num; i++) {
			pm.OUTCOL[Cdata].val[i] = pm.OUTCOL[DID].val[i];
			pm.OUTCOL[Cdata].Rcol[i] = pm.OUTCOL[DID].Rcol[i];
			pm.OUTCOL[Cdata].Fcol[i] = pm.OUTCOL[DID].Fcol[i];
		}
		pm.OUTCOL[Cdata].num = pm.OUTCOL[DID].num;
	}
	//
	Disp_Buttons();
}
