// RRIView.cpp : CRRIView クラスの実装
//

#include "stdafx.h"
#include  <stdlib.h>


// SHARED_HANDLERS は、プレビュー、サムネイル、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "RRI.h"
#endif

#include "RRIDoc.h"
#include "RRIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//___________________________________________________________________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////
// COMMON_LIBRALY
#include  "CommFuncs.h"
#include  "CommGL.h"
#include	"Proj_Conf.h"					// 2019/10 追加
//#include	"Proj_Conf.h"					// 2019/10 追加
#include	<io.h>
#include	<fcntl.h>


#include	"RRI_Common.h"

#include	"BUSY.h"
#include	"OPTIONS.h"
#include	"AnimOpt.h"
#include	"Section.h"
#include    "AccuracyEvaluation.h"
#include    "SelectCalculatedLevel.h"
#include	"Profile.h"
#include	"MeshProfile.h"
#include	"ShapeName.h"
#include	"MakeHQeqDialog.h"

#include "Ask_Case.h"

///　/////////////////////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern					CStatusBar*		SB;	
GL_PARAMETER				Gp;
CPoint					LastPoint;
XYCOORD					Band_Area[4];
IXYCOORD					Prof_ST[2],Oldid;
BOOL						COMMENT_SWITCH;
BOOL						First_Time;
//
// Debug
FILE*					fd;
int						XID,YID;
CString					CStitle;

int						MAX_TEXTURE, LAT_SPAN,LNG_SPAN,DEM_H,DEM_V;
double					TEX_START_LAT,TEX_START_LNG;
double					DEM_LAT,DEM_LNG,DEM_SIZE;
double					ONE_DEGREE_DISTANCE;
char						IMAGE_FOLDER[SIZE_1K];
//
//GB_PARAMETER			GBB;
//
char						m_RecentPath[SIZE_05K];
char						m_CurrentPath[SIZE_05K];

int						CVIEW;							// ORTHO,PERS モード
int						CLABEL;							// 現在対象のデータ
int						COUT;							// どの出力データを対象としているか
int						CID;								// 現在選択されているメッシュ
int						CSHAPE;							// 現在のシェイプ

int						Image_Sub,Option_Sub;				// バックプレートの分割数
PARAMS					PM;								// 上下流基本情報
DATASET					DSET;							// 上下流ファイルセット
LOCATION_INFO			LC;								// location.txt情報

MINFO**					MESH;


CBitmap					*pBM[6];						// ビットマップのポインタ
//
int				Cnt_Prof,Cnt_MProf,Cnt_Hydro;
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//   スクリーンツール
int						SCHnum, SCVnum;
double					NOWX, NOWY;					// スクリーンツール横縦、現在位置
BYTE*					Tool_Image;					// スクリーンイメージ

int						offx[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
int						offy[] = { 0, -1, -1, -1, 0, 1, 1, 1 };
int						direct[] = { 16, 32, 64, 128, 1, 2, 4, 8 };		// 流入方向
int						Ddirect[] = { 1, 2, 4, 8, 16, 32, 64, 128 };	// 流下方向

//																			コントロール定義
///  //////////////////////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int			ViewerOff=218,ModelerOff=450;
CString		Fonts[12]={	"Arial","Calibri","Candara","Corbel","Impact","Lucida Sans","Vrinda",
								"Cambria","Garamond","Georgia","Minion","Times New Roman"};
int			OUT_Num = 10;
int			OUT_ID[10] = {IDC_V_GIS_0,IDC_V_GIS_1,IDC_V_GIS_2,IDC_V_GIS_3,IDC_V_GIS_4,
							IDC_V_GIS_5,IDC_V_GIS_6, IDC_V_GIS_7, IDC_V_GIS_8, IDC_V_GIS_9 };
CString		OUT_MENU_NAME[10]={"hs(Inundation)","hr(River W.D.)","hg(GroundW.D.)","qr(River Disc)","qu",
							"qv", "gu", "gv", "gampt_ff", "hs max" };

int			ODView_Num = 2;
int			ODView_ID[] = {IDC_V_BUTTON_RIVPATH,IDC_V_BUTTON_PROFILE};
CString		ODView_Text[] = {"Set River Path","Show Profile"};

int			BMP_Num = 6;
int			BMP_ID[6] = {IDB_BITMAP_START,IDB_BITMAP_REWIND,
						 IDB_BITMAP_BACK,IDB_BITMAP_NEXT,
						 IDB_BITMAP_FORWARD,IDB_BITMAP_REC};
int			BButton_ID[6] = { IDC_V_BUTTON_START, IDC_V_BUTTON_REW,
							IDC_V_BUTTON_BACK, IDC_V_BUTTON_NEXT,
							IDC_V_BUTTON_FOR, IDC_V_BUTTON_REC };

int			V_COL_NUM = 2,V_COL_ID[2] = {IDC_V_COLOR_DATE,IDC_V_COLOR_LEGEND};
int			V_CHECK_NUM = 5,
			V_CHECK_ID[5] = {IDC_V_CHECK_FRAME,IDC_V_CHECK_BACK,IDC_V_CHECK_DATE,IDC_V_CHECK_LEGEND,IDC_CHECK_V_SHOWDATA};
int			V_RADIO_NUM = 2,V_RADIO_ID[2]={  IDC_V_RADIO_COLOR,IDC_V_RADIO_GRAY};
//
int			ANIM_SET_NUM = 9;
int			ANIM_SET_ID[] = { IDC_V_BUTTON_REW, IDC_V_BUTTON_BACK, IDC_V_BUTTON_START, IDC_V_BUTTON_NEXT, IDC_V_BUTTON_FOR,
							IDC_SLIDER_RUNTIME, IDC_V_BUTTON_REC, IDC_V_INFO_DATE, IDC_V_INFO_TIME };
// 0:param+mesh,1:mesh,2:TimeSet Place&Value  > 3:DAM_CNT_FILE 4:DIV_FILE,5:BREAK_FILE,6:EVP_FILE
//int			File_Format[ FILE_IN_NUM ] = {	7,0,0,0,0,  0,0,0,0,0,  1,1,1,2,2,  3,2,4,5,6,8}; 
//int			File_Order[ FILE_IN_NUM ] = {	-1,2,0,0,3,  0,3,2,2,2,  3,3,4,2,2,  -1,2,2,-1,5,5};
int			File_Format[FILE_IN_NUM] = { 5, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 2, 5, 8, -1, -1, -1 };
int			File_Order[FILE_IN_NUM] = { -1, 2, 0, 0, 0, 2, 2, 2, 3, 3, 3, 4, 1, 1, 1, 1, 0, 0, 1, -1, 2, 0, -1 };

CString		File_Title[FILE_IN_NUM] = {"Rain Data","DEM Data","Accmulate","Direction",
								"NS Slope","Land Usage",
								"NS River","Riv Width","Riv Depth","Bank Height",
								"Init H Slope","Init H River","Green Gampt Val",
								"Boundary Water Level","Partial Discharge",
								"Dam List","Dam Discharge",
								"Divided Discharge",
								"Break Time Condition","Evaporation","Drain"};
//
int			BIN_NUMS = 6;
CString		BIN_NAMES[]={"rri.exe","RRI_initcreat.exe","RRI_inundation_feedback.exe",
							"RRI_lower_set.exe","lower.bat","upper.bat"};
IXYCOORD		Tool_Frame[4] = { { (BYTE)1, 0, 0 }, { (BYTE)1, 0, 161 }, { (BYTE)1, 60, 161 }, { (BYTE)1, 60, 0 } };

IXYCOORD		Tool_Func[6][4] = { { { (BYTE)1, 14, 6 }, { (BYTE)1, 14, 34 }, { (BYTE)1, 46, 34 }, { (BYTE)1, 46, 6 } },
{ { (BYTE)1, 14, 41 }, { (BYTE)1, 14, 60 }, { (BYTE)1, 46, 60 }, { (BYTE)1, 46, 41 } },
{ { (BYTE)1, 5, 64 }, { (BYTE)1, 5, 98 }, { (BYTE)1, 23, 98 }, { (BYTE)1, 23, 64 } },
{ { (BYTE)1, 36, 64 }, { (BYTE)1, 36, 98 }, { (BYTE)1, 54, 98 }, { (BYTE)1, 54, 64 } },
{ { (BYTE)1, 14, 103 }, { (BYTE)1, 14, 121 }, { (BYTE)1, 46, 121 }, { (BYTE)1, 46, 103 } },
{ { (BYTE)1, 14, 128 }, { (BYTE)1, 14, 156 }, { (BYTE)1, 46, 156 }, { (BYTE)1, 46, 128 } } };

//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CRRIView

IMPLEMENT_DYNCREATE(CRRIView, CFormView)
//---------------------------------------------------------------------------------------------   MESSAGE MAP
//
BEGIN_MESSAGE_MAP(CRRIView, CFormView)
	//ON_WM_DRAWITEM()
	//ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_CHANGEBACKCOLOR, &CRRIView::OnChangebackcolor)
	//ON_COMMAND(ID_CHANGEMODEL, &CRRIView::OnChangemodel)
	//ON_BN_CLICKED(IDC_V_RADIO_COLOR, &CRRIView::OnBnClickedVRadioColor)
	//ON_BN_CLICKED(IDC_V_RADIO_GRAY, &CRRIView::OnBnClickedVRadioGray)
	ON_BN_CLICKED(IDC_V_BUTTON_START, &CRRIView::OnBnClickedVButtonStart)
	ON_BN_CLICKED(IDC_V_BUTTON_REC, &CRRIView::OnBnClickedVButtonRec)
	ON_BN_CLICKED(IDC_V_CHECK_FRAME, &CRRIView::OnBnClickedVCheckFrame)
	ON_BN_CLICKED(IDC_V_CHECK_BACK, &CRRIView::OnBnClickedVCheckBack)
	ON_STN_DBLCLK(IDC_V_MOREDETAIL, &CRRIView::OnStnDblclickVMoredetail)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ANIM, &CRRIView::OnNMCustomdrawSliderAnim)
	ON_BN_CLICKED(IDC_V_CHECK_DATE, &CRRIView::OnBnClickedVCheckDate)
	ON_STN_DBLCLK(IDC_V_COLOR_DATE, &CRRIView::OnStnDblclickVColorDate)
	ON_BN_CLICKED(IDC_V_CHECK_LEGEND, &CRRIView::OnBnClickedVCheckLegend)
	ON_BN_CLICKED(IDC_V_BUTTON_REW, &CRRIView::OnBnClickedVButtonRew)
	ON_BN_CLICKED(IDC_V_BUTTON_FOR, &CRRIView::OnBnClickedVButtonFor)
	ON_BN_CLICKED(IDC_V_BUTTON_BACK, &CRRIView::OnBnClickedVButtonBack)
	ON_BN_CLICKED(IDC_V_BUTTON_NEXT, &CRRIView::OnBnClickedVButtonNext)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_RUNTIME, &CRRIView::OnNMCustomdrawSliderRuntime)
	ON_STN_DBLCLK(IDC_V_COLOR_LEGEND, &CRRIView::OnStnDblclickVColorLegend)
	ON_WM_LBUTTONDBLCLK()
	ON_STN_CLICKED(IDC_V_GIS_0, &CRRIView::OnStnClickedVGis0)
	ON_STN_CLICKED(IDC_V_GIS_1, &CRRIView::OnStnClickedVGis1)
	ON_STN_CLICKED(IDC_V_GIS_2, &CRRIView::OnStnClickedVGis2)
	ON_STN_CLICKED(IDC_V_GIS_4, &CRRIView::OnStnClickedVGis4)
	ON_STN_CLICKED(IDC_V_GIS_5, &CRRIView::OnStnClickedVGis5)
	ON_STN_CLICKED(IDC_V_GIS_3, &CRRIView::OnStnClickedVGis3)
	ON_BN_CLICKED(IDC_V_BUTTON_RIVPATH, &CRRIView::OnBnClickedVButtonRivpath)
	ON_BN_CLICKED(IDC_V_BUTTON_PROFILE, &CRRIView::OnBnClickedVButtonProfile)
	ON_COMMAND(ID_ACTIVATE_SECTION, &CRRIView::OnActivateSection)
	ON_BN_CLICKED(IDC_V_CHECK_ARROW, &CRRIView::OnBnClickedVCheckArrow)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_V_SHOWDATA, &CRRIView::OnBnClickedCheckVShowdata)

	ON_STN_CLICKED(IDC_V_GIS_6, &CRRIView::OnStnClickedVGis6)
	ON_STN_CLICKED(IDC_V_GIS_7, &CRRIView::OnStnClickedVGis7)
	ON_STN_CLICKED(IDC_V_GIS_8, &CRRIView::OnStnClickedVGis8)
	ON_STN_CLICKED(IDC_V_GIS_9, &CRRIView::OnStnClickedVGis9)
	ON_BN_CLICKED(IDC_BUTTON_OUTMAX, &CRRIView::OnBnClickedButtonOutmax)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_IMAGE, &CRRIView::OnBnClickedButtonChangeImage)
	ON_BN_CLICKED(IDC_BUTTON_ACCURACYEVALUATION, &CRRIView::OnBnClickedButtonAccuracyEvaluation)
	ON_BN_CLICKED(makeHQeq, &CRRIView::OnBnClickedmakehqeq)
	ON_BN_CLICKED(IDC_V_CHECK_LOCATION, &CRRIView::OnBnClickedVCheckLocation)
END_MESSAGE_MAP()

///- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// CRRIView コンストラクション/デストラクション
///- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

CRRIView::CRRIView()
	: CFormView(CRRIView::IDD)
{
	m_DateInfo = _T("");
}

CRRIView::~CRRIView()
{
}
//_______________________________________________________________
//  Destroy
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void CRRIView::OnDestroy()
{
	CFormView::OnDestroy();
	//
	if( FAILED( GL_Term( &Gp ))){
		RETURN("** ERROR ** GL_TERM PROCESS ERROR");}
	///--------------------------------------------------------------Minfo data
	//  Application Memory
	Free_Project();
	//
	if (Tool_Image != NULL) SAFE_FREE(Tool_Image);
	//
	for(int i=0;i<BMP_Num;i++)  if( pBM[i] != NULL ) delete pBM[i];
	BMP_Num = 0;
	//
	if( Mmem >  0 ) {
		SAFE_FREE( MID );
		SAFE_FREE( Mpos );
		SAFE_FREE( DEM );
		SAFE_FREE( HWL );
		Mnum = Mmem = 0;
	}
	if( Smem > 0 )	SAFE_FREE( SXZ );
	if( SMmem > 0 ) SAFE_FREE( SMid );
}
///---------------------------------------------------------------------
//    プロジェクトの開放

void CRRIView::Free_Project()
{
	// Param 
	//
	if (PM.Flags != NULL)	SAFE_FREE(PM.Flags);
	Free_Matrix(PM.MVnum, PM.Values);
	//
	if (PM.Smem != 0) {
		for (int i = 0; i<PM.Smem; i++) {
			if (PM.SBOX[i].Mnum != 0)  SAFE_FREE(PM.SBOX[i].MID);
			if (PM.SBOX[i].Vnum != 0)  SAFE_FREE(PM.SBOX[i].XZ);
			PM.SBOX[i].set = FALSE;
		}
		SAFE_FREE(PM.SBOX);
	}
	// 
	////////SAFE_FREE(PM.Path);
	////////SAFE_FREE(PM.Rdist);
//	if (PM.Pval != NULL) Free_Matrix(4, PM.Pval);
	//// 
	if (PM.OBmem > 0) {
		SAFE_FREE(PM.OBpos);		// OBS
		PM.OBmem = PM.OBnum = 0;
	}
	//
	// MESH
	//
	for (int i = 0; i < PM.MVnum; i++) {
		for (int h = 0; h < PM.MHnum; h++) {
			if (MESH[i][h].Hval == NULL) continue;
			for (int n = 0; n < FILE_OUT_NUM; n++)
				SAFE_FREE(MESH[i][h].Hval[n]);
			SAFE_FREE(MESH[i][h].Hval);
		}
		SAFE_FREE(MESH[i]);
	}
	SAFE_FREE(MESH);
	//
	Free_Matrix(PM.BVnum, PM.Back_TexNo);
	//
	PM.set = 0;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// DDX  & PreCreatWindow
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void CRRIView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_ANIM, m_cSliderAnim);
	DDX_Control(pDX, IDC_SLIDER_RUNTIME, m_cRunTime);
	DDX_Check(pDX, IDC_V_CHECK_ARROW, m_Arrow);
	//DDX_Control(pDX, IDC_COMBO_SHAPE, m_ComboShape);
	//DDX_Text(pDX, IDC_V_VOLUME, m_Volume);
	//DDX_Control(pDX, IDC_COMBO_OBSPOS, m_ComboOBS);
	DDX_Control(pDX, IDC_V_BUTTON_RIVPATH, c_RivPath);
	DDX_Control(pDX, IDC_V_BUTTON_PROFILE, c_RivProfile);
}
BOOL CRRIView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}
///===========================================================================================================
//   初期化
///===========================================================================================================
void CRRIView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
///_______________________________________________________________
//  環境ファイルの読み込み
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	FILE*		fp = fopen("./RRI_CONTENTS/RRI.env","r");
	char			buf[SIZE_1K],head[SIZE_1K],inval1[SIZE_1K],inval2[SIZE_1K];
	int			v1,v2;
	//
	if( fp == NULL ) {
		MessageBox("** Cannot find  ./RRI_CONTENTS/RRI.env file","Fatal Error",MB_OK|MB_ICONHAND);
		exit(0);
	}
	Menu_Margin				= MENU_MARGIN;
	//
	while (fgets(buf, SIZE_1K, fp) != NULL) {
		sscanf(buf,"%s %s %s",head,inval1,inval2);
		if( strcmp( head,"[IMAGE_FOLDER]") == 0 ) {
			strcpy(IMAGE_FOLDER,inval1);
		}
		else  if( strcmp( head,"[VIEWER_MENU_WIDTH]") == 0 ) {
			Menu_Margin = atoi(inval1);
		}
	}
	fclose( fp );
	//
	Bool_acent_Date = TRUE;			// hsmax に入る前の時刻表示のON/OFF
///_______________________________________________________________
//  各種パラメータの初期化
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Init_Params();
	::GetCurrentDirectoryA(SIZE_1K, Gp.m_CurrentPath);
	//
	strcpy(m_CurrentPath,Gp.m_CurrentPath);
	strcpy(m_RecentPath,Gp.m_CurrentPath);
	m_cSliderAnim.SetRange(0,10,FALSE);
	m_cSliderAnim.SetPos( PM.Anim_Speed );
  
///_____________________________________________________
//  Validate Rect Color Window
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//IDC_V_COLOR_DATE,IDC_V_COLOR_LEGEND
	CWnd*	cw;
	CRect	COLr;
	cw = GetDlgItem( IDC_V_COLOR_DATE );
	cw->GetClientRect( &COLr );
	cw->ValidateRect( &COLr );
	cw = GetDlgItem( IDC_V_COLOR_LEGEND );
	cw->GetClientRect( &COLr );
	cw->ValidateRect( &COLr );
	//
	this->CustomizeCtrls();
	Disp_Controls();
	//
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	//_______________________________________________________________
//  開始
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///_______________________________________________________________
//  ケースの読み込み
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	CAsk_Case		AC;
	int				ret;
	//
	/// ケースの読込　-----------------------------------------------------

	if ((ret = AC.DoModal()) == IDCANCEL) {			// DSET.set 
		COMMENT("Cancel...."); exit(0);
	}
	// RRI_input にエラーが発生した場合
	if (AC.Valid_Proc) exit(0);
	//
	if (!DSET.SW_LatLong) {
		PM.endLAT = PM.stLAT + PM.Size*PM.MVnum;
		LAT_SPAN = (int)(fmod((PM.endLAT - PM.stLAT), 1.) == 0. ? (PM.endLAT - PM.stLAT) : (PM.endLAT - PM.stLAT) + 1.);
		PM.endLNG = PM.stLNG + PM.Size*PM.MHnum;
		LNG_SPAN = (int)(fmod((PM.endLNG - PM.stLNG), 1.) == 0. ? (PM.endLNG - PM.stLNG) : (PM.endLNG - PM.stLNG) + 1.);
		DEM_SIZE = PM.Size;
		PM.Deg_Unit = ONE_DEGREE_DISTANCE = 1. / PM.Size;
	}
	else {
		PM.endUTM_Y = PM.stUTM_Y + PM.UTMSize*PM.MVnum;
		PM.endUTM_X = PM.stUTM_X + PM.UTMSize*PM.MHnum;
		DEM_SIZE = PM.UTMSize;
		PM.Deg_Unit = ONE_DEGREE_DISTANCE = 1. / PM.UTMSize;
	}

	//_____________________________________________________
	///  スクリーンツールイメージの読み込みと透明処理
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int		cnt, id, iplane;
	BYTE*	image;
	BYTE		rr, gg, bb;
	image = Get_Image_File(RRI_SCREEN_TOOL_IMAGE, &SCHnum, &SCVnum, &iplane);
	//
	//
	if ((Tool_Image = (BYTE*)malloc(4 * SCHnum*SCVnum)) == NULL) {
			MessageBox("** Error **\nCannot Allocate Tool Image.....", "Error", MB_OK);
		DestroyWindow(); exit(1);
	}
	id = cnt = 0;
	for (int v = 0; v < SCVnum; v++) {
		for (int h = 0; h < SCHnum; h++) {
			rr = Tool_Image[cnt++] = image[id++];
			gg = Tool_Image[cnt++] = image[id++];
			bb = Tool_Image[cnt++] = image[id++];
			if (rr == 0 && gg == 0 && bb == 0)  Tool_Image[cnt++] = 0;
			else                            Tool_Image[cnt++] = 255;
		}
	}
	SAFE_FREE(image);

///_______________________________________________________________
//  GO
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	SetTimer(INIT_TIMER, 50, NULL);
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// DEBUG 
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#ifdef _DEBUG
void CRRIView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRRIView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRRIDoc* CRRIView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRRIDoc)));
	return (CRRIDoc*)m_pDocument;
}
#endif //_DEBUG
//_______________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////
//   ウィンドウの更新
/////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//   OnPaint
void CRRIView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//
	if( PM.Model ) GL_Redraw(&Gp,FALSE,FALSE,FALSE,UNDEF,TRUE);
	Disp_Controls();
}
///-----------------------------------------------------------------------------------------------------------
////   OnCtlColor
////-----------------------------------------------------------------------------------------------------------

//HBRUSH CRRIView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
//	//
//	// Back Color
//	if( CMODE == MODE_VIEW ) {
//		if( CUD == UPPER ) 	{pDC->SetBkColor(COLOR_TAB_V_UPPER);hbr = static_cast<HBRUSH>(m_brVU);}
//		else                {pDC->SetBkColor(COLOR_TAB_V_LOWER);hbr = static_cast<HBRUSH>(m_brVD);}
//	}
//	else {
//		if( CUD == UPPER ) 	{pDC->SetBkColor(COLOR_TAB_M_UPPER);hbr = static_cast<HBRUSH>(m_brMU);}
//		else                {pDC->SetBkColor(COLOR_TAB_M_LOWER);hbr = static_cast<HBRUSH>(m_brMD);}
//	}
//	//
//    if (nCtlColor == CTLCOLOR_STATIC)    {
//        pDC->SetTextColor(COLOR_TEXT_WHITE);
//    }
//    else if (nCtlColor == CTLCOLOR_EDIT)    {
//        pDC->SetTextColor(COLOR_EDIT_TEXT);
//		pDC->SetBkColor( COLOR_EDIT );
//		hbr =  static_cast<HBRUSH>(m_brEDIT);
//    }
//    else if (nCtlColor == CTLCOLOR_BTN)    {
//        pDC->SetTextColor(COLOR_BTN_TEXT);
//		hbr =  static_cast<HBRUSH>(m_brDlg);
//    }
//    else if (nCtlColor == CTLCOLOR_DLG)    {
//        pDC->SetBkColor(COLOR_DIALOG);
//		hbr =  static_cast<HBRUSH>(m_brDlg);
//    }
//    else if (nCtlColor == CTLCOLOR_LISTBOX)    {
//        pDC->SetTextColor(COLOR_EDIT_TEXT);
//		pDC->SetBkColor( COLOR_EDIT );
//		hbr =  static_cast<HBRUSH>(m_brEDIT);
//    }
//	return hbr;
//}
//-----------------------------------------------------------------------------------------------------------
//   CALL BACK
//-----------------------------------------------------------------------------------------------------------
int AFXAPI CRRIView::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED && lpData)
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	return 0;
}
//-----------------------------------------------------------------------------------------------------------
//   BUTTON_DIR
//-----------------------------------------------------------------------------------------------------------
BOOL	CRRIView::Button_Dir(CString* m_DirName,char* m_RecentPath )
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
		if( (pList = (ITEMIDLIST*)SHBrowseForFolder(&Info)) == NULL ) {pMalloc->Release();return FALSE;}
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
BOOL	CRRIView::Button_Dir(CString* m_DirName, char* m_RecentPath,BOOL NewFolder)
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
		Info.pidlRoot = 0;
		Info.pszDisplayName = cRetPath;
		Info.lpszTitle = _T("フォルダを選択してください");
		Info.lpfn = BrowseCallbackProc;
		Info.lParam = (LPARAM)m_RecentPath;
		if(NewFolder ) Info.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
		else          Info.ulFlags = BIF_RETURNONLYFSDIRS;	//フォルダの選択のみ


		pWnd->ModifyStyle(WS_CHILD, WS_POPUP, 0);

		//---  ダイアログ表示  -------------
		if ((pList = (ITEMIDLIST*)SHBrowseForFolder(&Info)) == NULL) { pMalloc->Release(); return FALSE; }
		// ITEMIDLIST からフルパスを得る
		SHGetPathFromIDList(pList, pszRetPath);
		//ITEMIDLIST の開放
		pMalloc->Free(pList);
		//選択ディレクトリ表示
		*m_DirName = pszRetPath;
		strcpy(m_RecentPath, *m_DirName);
		pMalloc->Release();
		//
	}
	return TRUE;
}
///===========================================================================================================
//   マウスアクション・キーボード
///===========================================================================================================
BOOL CRRIView::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_MOUSEMOVE ) { m_ToolTip.RelayEvent(pMsg);}
	//
	//if( pMsg->message == WM_LBUTTONDOWN ) {
	//	Disp_Info();
	//}
	//---------------------------------------------
	// システムキー　ダウン
	//---------------------------------------------
	if(pMsg->message ==  WM_SYSKEYDOWN ) {
		if( pMsg->wParam == VK_ALTKEY ) STATG( KEY_ALT_DOWN ) = TRUE; 
	}
	else if(pMsg->message ==  WM_SYSKEYUP ) {
		if( pMsg->wParam == VK_ALTKEY ) STATG( KEY_ALT_DOWN ) = FALSE; 
	}
	//---------------------------------------------
	// キー　ダウン
	//---------------------------------------------
	else if(pMsg->message == WM_KEYDOWN ) {
		switch( pMsg->wParam ) {
			case VK_RETURN		:	return TRUE;

			case VK_CONTROL		:	STATG( KEY_CTRL_DOWN ) = TRUE; 
									break;
			case VK_SHIFT		:	STATG( KEY_SHIFT_DOWN ) = TRUE; 
									break;
			case 'A'			:   IF_STATG( KEY_CTRL_DOWN ) {		// 全画面表示
										GL_Redraw(&Gp,TRUE,TRUE,TRUE,ALL_OBJECTS,TRUE);
									}
									break;
			case VK_LEFT		:   IF_STATG( KEY_CTRL_DOWN ) {		// 全画面表示
										if( CVIEW == ORTHO ) {
											Gp.cam[CVIEW].viewdir.x = 1.;
											Gp.cam[CVIEW].viewdir.y = Gp.cam[CVIEW].viewdir.z = 0.;
											GL_Redraw(&Gp,TRUE,TRUE,FALSE,UNDEF,TRUE);
											//IF_STATG( DISP_EDITWIN ) {
											//	PM.Cam_Dir = CAM_DIR_LEFT;
											//	EDDlg->Reflesh_Data();
											//	EDDlg->Set_Values();
											//}
										}
									}
									break;
			case VK_UP			:   IF_STATG( KEY_CTRL_DOWN ) {		// 全画面表示
										if( CVIEW == ORTHO ) {
											Gp.cam[CVIEW].viewdir.z = 1.;
											Gp.cam[CVIEW].viewdir.x = Gp.cam[CVIEW].viewdir.y = 0.;
											GL_Redraw(&Gp,TRUE,TRUE,FALSE,UNDEF,TRUE);
											//IF_STATG( DISP_EDITWIN ) {
											//	PM.Cam_Dir = CAM_DIR_UP;
											//	EDDlg->Reflesh_Data();
											//	EDDlg->Set_Values();
											//}
										}
									}
									break;
			case VK_RIGHT		:   IF_STATG( KEY_CTRL_DOWN ) {		// 全画面表示
										if( CVIEW == ORTHO ) {
											Gp.cam[CVIEW].viewdir.x = -1.;
											Gp.cam[CVIEW].viewdir.y = Gp.cam[CVIEW].viewdir.z = 0.;
											GL_Redraw(&Gp,TRUE,TRUE,FALSE,UNDEF,TRUE);
											//IF_STATG( DISP_EDITWIN ) {
											//	PM.Cam_Dir = CAM_DIR_RIGHT;
											//	EDDlg->Reflesh_Data();
											//	EDDlg->Set_Values();
											//}
										}
									}
									break;
			case 'S'			:	Show_Section(XID,YID);
									break;

		}
	}
	//---------------------------------------------
	// キー　アップ
	//---------------------------------------------
	else if( pMsg->message == WM_KEYUP ) {	
		switch( pMsg->wParam ) {
			case VK_RETURN		:	return TRUE;

			//case VK_TAB			:	if( PM.Model ) {
			//							CVIEW = ! CVIEW;
			//							Gp.view = CVIEW;
			//							//
			//							CWnd*		cw;
			//							CButton*	cb;
			//							if( CVIEW == ORTHO ) {					// 流速矢印表示を有効にする。
			//								cw = GetDlgItem( IDC_V_CHECK_ARROW );	cw->EnableWindow( TRUE );
			//								cw = GetDlgItem( IDC_V_LABEL_UV );		cw->EnableWindow( TRUE );

			//							}
			//							else {
			//								cb = (CButton*)GetDlgItem( IDC_V_CHECK_ARROW );	// 流速の矢印を消す
			//								if( PM.B_Arrow ) {
			//									PM.B_Arrow = FALSE;
			//									cb->SetCheck( FALSE );
			//									//
			//									GLobj_Delete(&Gp, OBJID_WL);
			//									GL_Redraw(&Gp, FALSE, FALSE, TRUE, OBJID_WL, TRUE);
			//								}
			//								cb->EnableWindow( FALSE );
			//								cw = GetDlgItem( IDC_V_LABEL_UV );		cw->EnableWindow( FALSE );
			//							}
			//							//
			//							Change_Cursor(&Gp,IDC_WAIT);
			//							//Make_Model();		// カメラのセットがなされていなければWorld	に設定

			//							Redraw_Now();
			//							if( Gp.cam[CVIEW].set )	GL_Redraw(&Gp,FALSE,TRUE,TRUE,ALL_OBJECTS,TRUE);
			//							else								GL_Redraw(&Gp,TRUE,TRUE,TRUE,ALL_OBJECTS,TRUE);
			//						}
			//						Disp_Info();
			//						break;


			case VK_CONTROL		:	STATG( KEY_CTRL_DOWN ) = FALSE; 
									break;

			case VK_SHIFT		:	STATG( KEY_SHIFT_DOWN ) = FALSE; 
									break;
			//
			case VK_SNAPSHOT	:   IF_STATG( IS_READY ) {
										COMMENT("NOW SAVING WINDOWS .....");
										if( FAILED( GL_Save_Window( &Gp ))) {
											COMMENT("**** ERROR **** Memory Alloc Error"); break;}
									}
									COMMENT("End Save .....!");
									//
									break;
		}
	}
	////

	return CFormView::PreTranslateMessage(pMsg);
}
//-----------------------------------------------------------------------------------------------------------
//   マウス　移動
//-----------------------------------------------------------------------------------------------------------
void CRRIView::OnMouseMove(UINT nFlags, CPoint point)
{
	if( ! PM.Model ) {CFormView::OnMouseMove(nFlags, point);return;}
	IFNOT_STATG( ON_SCREEN ) {CFormView::OnMouseMove(nFlags, point);return;}
//
///--------------------------------------------------------------------------------- 基本定数
	double		mx,my,aH,aV,Rxz;
	CString		str;
	CRect rect;
	Gp.win.cw->GetClientRect(&rect);
	int		id,xid,yid;
	int		No[4];
	//
	if (!rect.PtInRect(point)) {							// 領域を外れた場合
		STATG( MS_LB_DOWN ) = FALSE;
		STATG( MS_RB_DOWN ) = FALSE;
		::ReleaseCapture();
	}
	//
	double  xspan = (double)rect.Width();
	double  yspan = (double)rect.Height();
	//
	double	movex = (double)(point.x - LastPoint.x)/xspan * Gp.cam[CVIEW].Dw;
	double	movey = (double)(point.y - LastPoint.y)/yspan * Gp.cam[CVIEW].Dh;
	//
	GL_Trans_Pos(&Gp,point,&mx,&my);
	//
	xid = (int)mx;
	yid = (int)my;

//
///--------------------------------------------------------------------------------- ツールチップ
	if( CVIEW == ORTHO ) {	
		if( xid != Oldid.x || yid != Oldid.y ) {
			if( xid >= 0 && xid < PM.MHnum && yid >= 0 && yid < PM.MVnum ) {
				id = yid*PM.MHnum + xid;
				m_ToolTip.Pop();
				Oldid.x = xid;
				Oldid.y = yid;
			}
		}
	}
//____________________________________________________________________
/// 　左マウス操作
//--------------------------------------------------------------------
///
	if( nFlags&MK_LBUTTON ) {
		IFNOT_STATG(KEY_CTRL_DOWN) {
			//_______________________________________________________
			/// 　エディットウィンドウの移動
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			if (STATG(DISP_EDITWIN) && STATG(MS_RUBBER_START)) {
				// Safety

				if (xid < 0 || xid >(PM.MHnum + PM.SizeID) || yid < 0 || yid >(PM.MVnum + PM.SizeID)) goto EndProc;
				PM.CX = xid;
				if (PM.CX - PM.SizeID-1 < 0)		PM.CX = PM.SizeID+1;
				if (PM.CX + PM.SizeID >= PM.MHnum)	PM.CX = PM.MHnum - PM.SizeID - 1;
				PM.CY = yid;
				if (PM.CY - PM.SizeID-1 < 0)		PM.CY = PM.SizeID+1;
				if (PM.CY + PM.SizeID >= PM.MVnum)	PM.CY = PM.MVnum - PM.SizeID - 1;
				//
				mx = PM.CX;
				my = PM.CY;
				Draw_Band_Area(mx, my);
				//
				EDDlg->Reflesh_Data();
// ???				if (PM.Move_Cnt != 0) Edit_Mesh_Model();	// この段階で表示
				//
				EDDlg->Set_Values();
			}
		}
		///_______________________________________________________
		// 　Shift 浸水縦断の断面定義
		///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if ( !(nFlags&MK_CONTROL)  && (nFlags&MK_SHIFT) && CVIEW == ORTHO ) {	
			if( STATG( MS_RUBBER_START ) ) {
				GL_OBJECT*	obj = &Gp.Gobj[OBJID_MOUSE_BAND];
				GLobj_Delete( &Gp,OBJID_MOUSE_BAND );
				GLobj_Init(&Gp.Gobj[OBJID_MOUSE_BAND],5,2);
				//
				int		xx = Prof_ST[0].x,yy = Prof_ST[0].y;
				double	rate;

				if( ! (xx == xid && yy == yid ) ) {
					if( fabs( xid - xx ) != 0 ) {
						rate = (double)(fabs( yid - yy ))/(double)(fabs( xid - xx ));
						if( rate > 0.5 && rate <= 1. )	{
							if( yid >= yy && xid >= xx)			yid = (xid-xx) + yy;
							else if( yid >= yy && xid < xx)		yid = (xx-xid) + yy;
							else if( yid < yy && xid >= xx)		yid = yy - (xid-xx);
							else if( yid < yy && xid < xx)		yid = yy - (xx-xid);
						}
						else if( rate > 1. && rate < 2. )	{
							if( yid >= yy && xid >= xx)			xid = (yid-yy) + xx;
							else if( yid >= yy && xid < xx)		xid = (yy-yid) + xx;
							else if( yid < yy && xid >= xx)		xid = xx - (yid-yy);
							else if( yid < yy && xid < xx)		xid = xx - (yy-yid);
						}
						else if(  rate <= 0.5 )				yid = yy;
						else if(  rate >= 2. )				xid = xx;
					}
					else	xid = xx;
					//
					CID = yid*PM.MHnum + xid;
					mx = (double)xid;
					my = (double)yid;

					No[0] = GLobj_Add_Vert( obj,PM.Band_Area[0].x, BAND_HEIGHT,PM.Band_Area[0].y, FALSE);
					No[1] = GLobj_Add_Vert( obj,mx,BAND_HEIGHT,my, FALSE);

					GLobj_Add_Poly(obj,2,No,GL_LINES,3.f,F_COLOR(255,0,0));
					GL_Redraw(&Gp,FALSE,FALSE,TRUE,OBJID_MOUSE_BAND,TRUE);
					Prof_ST[1].x = xid;
					Prof_ST[1].y = yid;
					STATG( SET_MESHPROFILE ) = TRUE;
				}
			}
		}
		///_______________________________________________________
		// 　移動
		///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		else if( (nFlags&MK_CONTROL) && !(nFlags&MK_SHIFT)  ) {	// 移動　カメラ視点移動  コントロールキー有効
			//
			if( CVIEW == ORTHO ) {						// ＯＲＴＨＯの場合　カメラ位置を変えない
				Gp.cam[CVIEW].endH -= movex;  Gp.cam[CVIEW].stH -= movex;
				Gp.cam[CVIEW].endV += movey;  Gp.cam[CVIEW].stV += movey;
				Gp.cam[CVIEW].set = FALSE;
			}
			else {
				Gp.cam[CVIEW].tx -= movex;  Gp.cam[CVIEW].cx -= movex;
				Gp.cam[CVIEW].tz += movey;  Gp.cam[CVIEW].cz += movey;
				Gp.cam[CVIEW].set = FALSE;
			}
			//
			GL_Redraw(&Gp,FALSE,TRUE,FALSE,UNDEF,TRUE);
		}
	/////_______________________________________________________
	//// 　パースのみの回転
	/////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//	else if( STATG( KEY_ALT_DOWN ) && CVIEW == PERS  ) {	// 回転　ＰＥＲＳのみ
	//		aH = atan( movex/Gp.cam[CVIEW].radius);	Gp.cam[CVIEW].angH -= aH;
	//		aV = -atan( movey/Gp.cam[CVIEW].radius);	Gp.cam[CVIEW].angV += aV;
	//		// Camera Position
	//		Gp.cam[CVIEW].cy = Gp.cam[CVIEW].radius*sin( Gp.cam[CVIEW].angV ) + Gp.cam[CVIEW].ty;
	//		Rxz = Gp.cam[CVIEW].radius*cos( Gp.cam[CVIEW].angV );
	//		Gp.cam[CVIEW].cx = Rxz*cos( Gp.cam[CVIEW].angH ) + Gp.cam[CVIEW].tx;
	//		Gp.cam[CVIEW].cz = Rxz*sin( Gp.cam[CVIEW].angH ) + Gp.cam[CVIEW].tz;
	//		// Camera Vector
	//		double		AV=0.;
	//		AV = sqrt( 1. + cos( Gp.cam[CVIEW].angV )*cos( Gp.cam[CVIEW].angV ));
	//		Gp.cam[CVIEW].viewdir.x = -cos( Gp.cam[CVIEW].angH ) / AV;
	//		Gp.cam[CVIEW].viewdir.z = -sin( Gp.cam[CVIEW].angH ) / AV;
	//		Gp.cam[CVIEW].viewdir.y = -cos( Gp.cam[CVIEW].angV ) / AV;

	//		Gp.cam[CVIEW].set = FALSE;
	//		//
	//		GL_Redraw(&Gp,FALSE,TRUE,FALSE,UNDEF,TRUE);
	//	}
	}
///____________________________________________________________________
// 　右マウス操作
///--------------------------------------------------------------------

	else if( nFlags&MK_RBUTTON ) {
		///_______________________________________________________
		// 　浸水ボリュームのシェイプ定義
		///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if( (nFlags&MK_SHIFT) &&  STATG( SET_SHAPE ) ) {
			if( LastPoint.x != point.x  || LastPoint.y != point.y ) { 
				GL_OBJECT*	obj = &Gp.Gobj[OBJID_MOUSE_BAND];
				GLobj_Delete( &Gp,OBJID_MOUSE_BAND );
				GLobj_Init(&Gp.Gobj[OBJID_MOUSE_BAND],UNDEF,5);
				//
				if( Snum == Smem-2 ) {
					Smem += 100;
					SXZ = (XYCOORD*)realloc(SXZ,sizeof( XYCOORD )*Smem);
				}
				//
				int*	no;
				no = (int*)malloc(Smem*sizeof( int ));
				SXZ[Snum].x = mx;
				SXZ[Snum].y = my; Snum++;
				//
				///
				for(int i=0;i<Snum;i++) no[i] = GLobj_Add_Vert( obj,SXZ[i].x, BAND_HEIGHT,SXZ[i].y, FALSE);
				//
				GLobj_Add_Poly(obj,Snum,no,GL_LINE_STRIP,2.f,F_COLOR(255,0,0));
				GL_Redraw(&Gp,FALSE,FALSE,TRUE,OBJID_MOUSE_BAND,TRUE);
				//
				LastPoint.x = point.x;
				LastPoint.y = point.y;
				str.Format("Snum %d",Snum); COMMENT( str );
				free( no );
			}
		}
		///_______________________________________________________
		// 　パースのみの回転
		///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		else if( (nFlags&MK_CONTROL) &&  CVIEW == PERS )  {
			Gp.cam[CVIEW].ty -= movey;  Gp.cam[CVIEW].cy -= movey;
			Gp.cam[CVIEW].set = FALSE;
			//
			GL_Redraw(&Gp,FALSE,TRUE,FALSE,UNDEF,TRUE);
		}
		///_______________________________________________________
		// 　VIEWER の場合、時刻・凡例の移動
		///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		else if( STATG( MOVE_DATE ) ||  STATG( MOVE_LEGEND ) ) {	
			movex = (double)(point.x - LastPoint.x);
			movey = (double)(LastPoint.y - point.y);
			if( STATG( MOVE_DATE ) ) {
				PM.Pos_Time.x += movex;
				PM.Pos_Time.y += movey;
				Draw_Date();
				GL_Redraw(&Gp,FALSE,FALSE,TRUE,OBJID_TIME,TRUE);
			}
			else {
				int		px, py;
				px = point.x - Gp.win.Wrect.left;
				py = Gp.win.cHeight - (point.y - Gp.win.Wrect.top);
				if (PM.B_Legend) {
					if (px >= PM.Area_Legend.left && px <= PM.Area_Legend.right) {
						if (py >= PM.Area_Legend.bottom && py <= PM.Area_Legend.top) {
							STATG(MOVE_LEGEND) = TRUE;
						}
					}
				}
				PM.Pos_Legend.x += movex;
				PM.Pos_Legend.y += movey;
				PM.Change_Data = TRUE;
				Draw_Legend();
				Draw_Location();
				GL_Redraw(&Gp,FALSE,FALSE,TRUE,OBJID_LEGEND,TRUE);
			}
		}
		else if (PM.B_ScreenTool && STATG(MOVE_SCREEN_TOOL)){
			movex = (double)(point.x - LastPoint.x);
			movey = (double)(point.y - LastPoint.y);
			NOWX += movex;
			NOWY -= movey;
			Make_Screen_Tool(FALSE);
		}
	}
EndProc:
	//
	LastPoint = point;
	CFormView::OnMouseMove(nFlags, point);
}
//-----------------------------------------------------------------------------------------------------------
//   マウス　センターホイール
//   Ｘ，Ｙ平面を　Ｘ，Ｚ平面に変換
//-----------------------------------------------------------------------------------------------------------
BOOL CRRIView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (!PM.Model) return FALSE;
	///
	// 画面ズーム
	///
	if( nFlags&MK_CONTROL ) {	// コントロールキー有効
		double	yspan	= Gp.cam[CVIEW].cy - Gp.cam[CVIEW].ty;
		double	zspan	= (Gp.cam[CVIEW].zmax - Gp.cam[CVIEW].zmin)/2.;
		double	zcenter	= (Gp.cam[CVIEW].zmax + Gp.cam[CVIEW].zmin)/2.;
		double	ang;
		double	Hspan,Vspan,CT;
		//
		if( CVIEW == PERS ) {
			ang = Gp.cam[CVIEW].viewang;
			if( zDelta > 0 ) ang *= 0.95;
			else             ang *= 1.05;
			Gp.cam[CVIEW].viewang = ang; 
//	
			Gp.cam[CVIEW].Dh   = 2.*Gp.cam[CVIEW].radius*tan( Gp.cam[CVIEW].viewang*PI/360. );
			Gp.cam[CVIEW].Dw   = Gp.cam[CVIEW].Dh*Gp.cam[CVIEW].aspect;
			//
		}
		else {		// ORTHO の場合					修正済
			CT = (Gp.cam[CVIEW].stV + Gp.cam[CVIEW].endV)/2.;
			Vspan = (Gp.cam[CVIEW].endV - Gp.cam[CVIEW].stV)/2.;
			if( zDelta > 0 ) Vspan *= 0.95;
			else             Vspan *= 1.05;
			//
			Gp.cam[CVIEW].endV = Vspan + CT;
			Gp.cam[CVIEW].stV = CT - Vspan;
			//
			Gp.cam[CVIEW].Dh = fabs((Gp.cam[CVIEW].endV - Gp.cam[CVIEW].stV));
			Gp.cam[CVIEW].Dw = Gp.cam[CVIEW].Dh*Gp.cam[CVIEW].aspect;
			Hspan = Gp.cam[CVIEW].Dw / 2.;
			//
			//
			CT = (Gp.cam[CVIEW].stH + Gp.cam[CVIEW].endH)/2.;
			Gp.cam[CVIEW].endH = CT + Hspan;
			Gp.cam[CVIEW].stH =  CT - Hspan;
			//
			Gp.cam[CVIEW].set = FALSE;
		}

		//
		GL_Redraw(&Gp,FALSE,TRUE,FALSE,UNDEF,TRUE);
	}
	//
	// 編集画面の拡大・縮小
	//
	else if (STATG(DISP_EDITWIN)) {
		int		OldID = PM.SizeID;
		if (zDelta > 0) {
			PM.SizeID -= 3;
			if (PM.SizeID < 3) PM.SizeID = 3;
		}
		else   {
			PM.SizeID += 3;
			if (PM.SizeID > 10) PM.SizeID = 10;
		}
		if (OldID != PM.SizeID) {
			EDDlg->Reflesh_Data();
			EDDlg->Change_Frame_Size();
			Draw_Band_Area(DUNDEF, DUNDEF);
		}
	}

	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}
//-----------------------------------------------------------------------------------------------------------
//   マウス　左ボタン
//-----------------------------------------------------------------------------------------------------------
void CRRIView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!PM.Model) return;
	//
	CPoint  ap = point;
	ap.x -= Gp.win.Wrect.left;
	ap.y -= Gp.win.Wrect.top;
	CRect rect;
	Gp.win.cw->GetClientRect(&rect);
	if (!rect.PtInRect(ap)) {CFormView::OnLButtonDown(nFlags, point);return;}// 領域を外れた場合
	//
	STATG( MS_LB_DOWN ) = TRUE;
	LastPoint = point;
//	SetCapture();
	double		mx,my,du=1.;
	int			xid,yid;
	//
	CString		str;
	//	Tool_Frame;
	int		ret;
	if (STATG(SET_WORLD_VIEW) && PM.B_ScreenTool) {
		if ((ret = Decide_Tool(point)) > 0) { EX_Tool(ret); return; }
	}
	//
	GL_Trans_Pos(&Gp,point,&mx,&my);
	xid = (int)mx;
	yid = (int)my;
	CID = yid*PM.MHnum + xid;
	//
	if( CVIEW == ORTHO && PM.Model) {
		if( ! (nFlags&MK_SHIFT) && ! (nFlags&MK_CONTROL)   ) {	// VIEWER,EDITOR 共通
			//
			int		ret;
			if (STATG(SET_WORLD_VIEW) && PM.B_ScreenTool) {
				if ((ret = Decide_Tool(point)) > 0) { EX_Tool(ret); return; }
			}
			//
			if (DSET.set) {
				str = _T("");
				XID = YID = UNDEF;
				if( (xid >= 0 && xid < PM.MHnum) &&  (yid >= 0 && yid < PM.MVnum )) {
					XID =xid;
					YID =yid;
					if (strlen(MESH[yid][xid].name) != 0) str.Format("Mesh No. [ %d, %d ] %s",
						xid + 1, PM.MVnum - yid, MESH[yid][xid].name);
					else        str.Format("Mesh No. [ %d, %d ] %d",xid+1,PM.MVnum-yid,MESH[yid][xid].flag);
					COMMENT( str );
				}			
			}
			//
		}
		//
		else if( CVIEW == ORTHO) {	// VIEWER のみ
			if( nFlags&MK_SHIFT && !(nFlags&MK_CONTROL)) {
				if ((xid >= 0 && xid < PM.MHnum) && (yid >= 0 && yid < PM.MVnum)) {
					PM.Band_Area[0].x = mx;
					PM.Band_Area[0].y = my;
					Prof_ST[0].x = xid;
					Prof_ST[0].y = yid;
					STATG(MS_RUBBER_START) = TRUE;
				}
			}
		}
	}
	//
	CFormView::OnLButtonDown(nFlags, point);
}

void CRRIView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!PM.Model) return;
	//
	CPoint  ap = point;
	ap.x -= Gp.win.Wrect.left;
	ap.y -= Gp.win.Wrect.top;
	CRect rect;
	Gp.win.cw->GetClientRect(&rect);
	if (!rect.PtInRect(ap)) { CFormView::OnLButtonDown(nFlags, point); return; }// 領域を外れた場合
	//
	STATG( MS_LB_DOWN ) = FALSE;
	//
	if (STATG(SET_WORLD_VIEW) && PM.B_ScreenTool) {
		if (Decide_Tool(point) > 0) 
			return;
	}
	if (STATG(MS_RUBBER_START)) {				// EDITOR: DISP_EDITWIN時があるがMOVEの方で対応。
							
		IF_STATG( SET_MESHPROFILE ) {				// SET_MESHPROFILE: バンドは消さない。
			Disp_MeshProfile();
			STATG( SET_MESHPROFILE ) = FALSE;
			STATG( DISP_MESHPROFILE ) = TRUE;
		}
		if (!STATG(DISP_EDITWIN))  STATG(MS_RUBBER_START) = FALSE;
		::ReleaseCapture();
	}
	//
	else if ( STATG(SELECT_RIV_PATH) ) {
		double		mx,my;
		int			id,xid,yid,pno;
		CString		str;
		//
		GL_Trans_Pos(&Gp,point,&mx,&my);
		xid = (int)mx;
		yid = (int)my;
		if ((yid >= 0 && yid <PM.MVnum) && (xid >= 0 && xid <PM.MHnum)){
			str.Format("Mesh Area [%d %d] kind %d", xid + 1, yid + 1, MESH[yid][xid].flag);
			COMMENT(str);
			//
			if (MESH[yid][xid].Acc >= DSET.Riv_Thresh) {			// ACCで判断
				PM.LineID[ PM.Lcnt ].x = xid;
				PM.LineID[PM.Lcnt].y = yid;
				PM.Lcnt++;
				if( PM.Lcnt == 2 ) {
					if( Search_River_Path() ) {
						Make_River_Mark(TRUE);
						PM.Set_Line = TRUE;
						STATG( SELECT_RIV_PATH ) = FALSE;
						STATG( DECIDE_RIV_PATH ) = TRUE;
					}
					else {
						MessageBox("** Error **\nCannot Find Path","Error",MB_OK|MB_ICONHAND);
						for (int i = 0; i<PM.LineNum; i++) 	MESH[PM.Path[i].y][PM.Path[i].x].ID_RIVPATH = SUNDEF;
						//SAFE_FREE( PM.Path );
						//SAFE_FREE( PM.Rdist );
						PM.Lcnt = PM.LineNum = PM.LineMem = 0;
						//
						PM.Set_Line = FALSE;
						STATG( SELECT_RIV_PATH ) = FALSE;
						Disp_Info();
						COMMENT("Initialized  Setting River Path");
						//
						GLobj_Delete(&Gp, OBJID_MARK);
						GL_Redraw(&Gp, FALSE, FALSE, TRUE, OBJID_MARK, TRUE);
					}
				}
				else {
					if( MessageBox("Select Downstream-end River Mesh","River Path",MB_YESNO|MB_ICONEXCLAMATION ) == IDNO) {
						STATG( SELECT_RIV_PATH ) = FALSE;
						COMMENT( "** Canceled Specity  River Path");
						return;
					}
					MESH[yid][xid].ID_RIVPATH = 1;
					Make_River_Mark(FALSE);
				}
				Disp_Info();
			}
			else {
				MessageBox("** Error **\nThis Mesh is not River Mesh\nSelece River Mesh","Error",MB_OK|MB_ICONHAND);
			}
		}
	}
	//
	//
	CFormView::OnLButtonUp(nFlags, point);
}

void CRRIView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (!PM.Model) return;
	//
	CPoint  ap = point;
	ap.x -= Gp.win.Wrect.left;
	ap.y -= Gp.win.Wrect.top;
	CRect rect;
	Gp.win.cw->GetClientRect(&rect);
	if (!rect.PtInRect(ap)) { CFormView::OnLButtonDown(nFlags, point); return; }// 領域を外れた場合
	//
	double		mx,my;
	int			xid,yid;
	//
	if (STATG(SET_WORLD_VIEW) && PM.B_ScreenTool) {
		if (Decide_Tool(point) > 0)
			return;
	}
	//
	if( PM.set && CVIEW == ORTHO ) {
		IFNOT_STATG( RUN_ANIMATION ) {		// アニメーションが走っていないとき
			///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//   ハイドログラフの表示
			if( GL_Trans_Pos( &Gp,point,&mx,&my) ) {
				int			id,xid,yid;
				xid = (int)mx;
				yid = (int)my;

				//
				if ((xid >= 0 && xid < PM.MHnum) && (yid >= 0 && yid < PM.MVnum)) {
					IFNOT_STATG( DISP_SECTION ) {
						if (COUT == OUT_HS)							 Show_Section(xid, yid);
						if (COUT == OUT_HR  &&  MESH[yid][xid].flag == 2) Show_Section(xid, yid);
						if (COUT == OUT_QR  &&  MESH[yid][xid].flag == 2) Show_Section(xid, yid);
						//
						if (COUT == OUT_HSMAX) {	// OUT_HSMAX = OUT_STORAGE
							// 編集ダイアログ発生のための処理
							IFNOT_STATG(DISP_EDITWIN) {
								if (xid - PM.SizeID < 0)					xid = PM.SizeID;
								if (xid + PM.SizeID >= PM.MHnum)	xid = PM.MHnum - PM.SizeID - 1;
								if (yid - PM.SizeID < 0)					yid = PM.SizeID;
								if (yid + PM.SizeID >= PM.MVnum)	yid = PM.MVnum - PM.SizeID - 1;
								//
								Draw_Band_Area((double)xid, (double)yid);
								STATG(MS_RUBBER_START) = TRUE;
								//
								Display_EditWindow(xid, yid);
							}
						}
					}
				}
			}
		}
	}
	//
	CFormView::OnLButtonDblClk(nFlags, point);
}
void CRRIView::Show_Section(int xid,int yid)
{
	int dataIdx = xid;
	int dataIdy = yid;
	int posIdx = dataIdx + 1;
	int posIdy = PM.MVnum - dataIdy;

	for (int locCnt = 0; locCnt < LOCATION_NUM; locCnt++) {
		if (posIdy == LC.mz[locCnt] && posIdx == LC.mx[locCnt]) {
			strcpy(DSET.LocationName, LC.name[locCnt]);
			DSET.LocationX = posIdx;
			DSET.LocationY = posIdy;

			CStitle.Format("Accuracy Evaluation [ %d %d ]", posIdx, posIdy);
			Show_AccuracyEvaluation();
			return;
		}
	}

	CSection		CS(this);
	CString		title;
	//
	CS.Tnum =	DSET.DT_Out;	// 浸水深のハイドロ
	CS.Hours	=	DSET.Hours;
	CS.st = DSET.ST;
	CS.MESH = &MESH[dataIdy][dataIdx];
	//
	CS.xid = posIdx;
	CS.yid = posIdy;
	CS.vid = dataIdy;
	CS.hid = dataIdx;
	//
	//
	if (MESH[dataIdy][dataIdx].flag == 2) {
		CS.isRiver = TRUE;
		CStitle.Format("River H Q Information at [ %d %d ]", posIdx, posIdy);
	}
	else {
		CS.isRiver = FALSE;
		CStitle.Format("Mesh Inundation Depth at [ %d %d ]", posIdx, posIdy);
	}
	CS.WinTitle = CStitle;
	CRect	Wrect;
	this->GetWindowRect( &Wrect );
	CS.WinH = WIN_H[ WIN_HYDRO ];
	CS.WinW = WIN_W[ WIN_HYDRO ];
	CS.m_Parent = this;
	CS.Prect = Wrect;
	//
	STATG( DISP_SECTION ) = TRUE;
	CS.DoModal();
	STATG( DISP_SECTION ) = FALSE;
}
							//
void CRRIView::Draw_Band_Area(double cx,double cy)
{
	GL_OBJECT*	obj = &Gp.Gobj[OBJID_MOUSE_BAND];
	GLobj_Delete( &Gp,OBJID_MOUSE_BAND );
	GLobj_Init(&Gp.Gobj[OBJID_MOUSE_BAND],5,2);
	//
	if( cx == DUNDEF ) {
		cx = PM.Band_Center.x;
		cy = PM.Band_Center.y;
	}
	//
	double  xst,xend,zst,zend;
	int		No[4];
	xst = cx - PM.SizeID*PM.Size*ONE_DEGREE_DISTANCE;
	xend = cx + (PM.SizeID+1)*PM.Size*ONE_DEGREE_DISTANCE;
	zst = cy - PM.SizeID*PM.Size*ONE_DEGREE_DISTANCE;
	zend = cy + (PM.SizeID+1)*PM.Size*ONE_DEGREE_DISTANCE;
	//
	No[0] = GLobj_Add_Vert( obj,xst, BAND_HEIGHT,zst, FALSE);
	No[1] = GLobj_Add_Vert( obj,xend,BAND_HEIGHT,zst, FALSE);
	No[2] = GLobj_Add_Vert( obj,xend,BAND_HEIGHT,zend,FALSE);
	No[3] = GLobj_Add_Vert( obj,xst, BAND_HEIGHT,zend,FALSE);
	//
	GLobj_Add_Poly(obj,4,No,GL_LINE_LOOP,1.5f,F_COLOR(255,0,0));
	GL_Redraw(&Gp,FALSE,FALSE,TRUE,OBJID_MOUSE_BAND,TRUE);
	//
	PM.Band_Area[0].x = xst;
	PM.Band_Area[0].y = zst;
	PM.Band_Area[1].x = xend;
	PM.Band_Area[1].y = zend;
	PM.Band_Center.x = cx;
	PM.Band_Center.y = cy;
	//
	STATG(MS_RUBBER_START) = TRUE;
}
//-----------------------------------------------------------------------------------------------------------
//   マウス　右ボタン
//-----------------------------------------------------------------------------------------------------------
void CRRIView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (!PM.Model) return;
	//
	CRect rect;
	Gp.win.cw->GetClientRect(&rect);
	if (!rect.PtInRect(point)) {CFormView::OnRButtonDown(nFlags, point);return;}// 領域を外れた場合
	//
	STATG( MS_RB_DOWN ) = TRUE;
	LastPoint = point;
//	SetCapture();
	double		mx,my;
	//---------------------------------------------------------------------
	// パース縦移動
	//---------------------------------------------------------------------
	if(STATG( ON_SCREEN ) && CVIEW == PERS && nFlags&MK_CONTROL) {
		CFormView::OnRButtonDown(nFlags, point);return;
	}
	//
	GL_Trans_Pos(&Gp,point,&mx,&my);
	//---------------------------------------------------------------------
	// SHAPE の開始
	//---------------------------------------------------------------------
	if(CVIEW == ORTHO && nFlags&MK_SHIFT) {
		STATG( SET_SHAPE ) = TRUE;
		if( Smem == 0 ) {
			Smem = 100;
			if((SXZ=(XYCOORD*)malloc(Smem*sizeof( XYCOORD ))) == NULL ) return;
		}
		Snum = 0;
		SXZ[Snum].x = mx; SXZ[Snum].y = my;  Snum++;
	}
	else {
		//
		//---------------------------------------------------------------------
		// 日付・凡例の移動
		//---------------------------------------------------------------------
		//
		int		px,py;
		px = point.x - Gp.win.Wrect.left;
		py = Gp.win.cHeight - (point.y-Gp.win.Wrect.top);
		if( PM.Show_Date ) {
			if( px >= PM.Pos_Time.x && px <= (PM.Pos_Time.x+PM.Size_Time.x)) {
				if( py >= PM.Pos_Time.y && py <= (PM.Pos_Time.y+PM.Size_Time.y)) {
					STATG( MOVE_DATE ) = TRUE;
					CFormView::OnRButtonDown(nFlags, point);return;
				}
			}
		}
		if( PM.Show_Legend ) {
			if( px >= PM.Pos_Legend.x && px <= (PM.Pos_Legend.x+PM.Size_Legend.x)) {
				if( py >= PM.Pos_Legend.y && py <= (PM.Pos_Legend.y+PM.Size_Legend.y)) {
					STATG( MOVE_LEGEND ) = TRUE;
					CFormView::OnRButtonDown(nFlags, point);return;
				}
			}
		}
		// スクリーンツールの移動
		if (PM.B_ScreenTool){
			double		cx, cy;
			XYCOORD		Area[4];
			//
			cx = (double)(point.x - Gp.win.Wrect.left);
			cy = (double)(Gp.win.Wrect.bottom - point.y);
			for (int i = 0; i < 4; i++) {
				Area[i].x = (double)(Tool_Frame[i].x + NOWX);
				Area[i].y = (double)(Tool_Frame[i].y + NOWY);
			}
			if (Is_InRect(cx, cy, Area, TRUE)){ 
				STATG(MOVE_SCREEN_TOOL) = TRUE; return; }
		}
		//
		//---------------------------------------------------------------------
		// Menu
		//---------------------------------------------------------------------
		CMenu menu;
		menu.CreatePopupMenu();
		//
		menu.AppendMenu(MF_STRING|MF_ENABLED,ID_ACTIVATE_SECTION,"Activate");
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING|MF_ENABLED,ID_CHANGEBACKCOLOR,"Change Back Color");

		ClientToScreen(&point);
		menu.TrackPopupMenu(
			TPM_LEFTALIGN  |	//クリック時のX座標をメニューの左辺にする
			TPM_RIGHTBUTTON,	//右クリックでメニュー選択可能とする
			point.x,point.y,	//メニューの表示位置
			this            	//このメニューを所有するウィンドウ
		);
		menu.DestroyMenu();
	}
	//
	CFormView::OnRButtonDown(nFlags, point);
}
//----------------------------------------------------------------------
void CRRIView::OnActivateSection()
{
	GL_Redraw(&Gp,FALSE,FALSE,FALSE,UNDEF,TRUE);
}
//----------------------------------------------------------------------

void CRRIView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (!PM.Model) return;
	//
	CRect rect;
	Gp.win.cw->GetClientRect(&rect);
	if (!rect.PtInRect(point)) {CFormView::OnRButtonUp(nFlags, point);return;}// 領域を外れた場合

	STATG( MS_RB_DOWN ) = FALSE;
	::ReleaseCapture();
	//
	STATG(MOVE_SCREEN_TOOL) = STATG(MOVE_DATE) = STATG(MOVE_LEGEND) = FALSE;
	//---------------------------------------------------------------------
	// シェイプエリアの特定と保存
	//---------------------------------------------------------------------
	IF_STATG( SET_SHAPE ){
		STATG( SET_SHAPE ) = FALSE;
		if( Snum < 3 ){CFormView::OnRButtonUp(nFlags, point);return;}
		//_______________________________________________________
		// 　領域の閉曲線化
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		GL_OBJECT*	obj = &Gp.Gobj[OBJID_MOUSE_BAND];
		GLobj_Delete( &Gp,OBJID_MOUSE_BAND );
		GLobj_Init(&Gp.Gobj[OBJID_MOUSE_BAND],UNDEF,5);
		int*	no;
		no = (int*)malloc(Smem*sizeof( int ));
		for(int i=0;i<Snum;i++) no[i] = GLobj_Add_Vert( obj,SXZ[i].x, BAND_HEIGHT,SXZ[i].y, FALSE);
		//
		GLobj_Add_Poly(obj,Snum,no,GL_LINE_LOOP,2.f,F_COLOR(255,0,0));
		GL_Redraw(&Gp,FALSE,FALSE,TRUE,OBJID_MOUSE_BAND,TRUE);
		//
		free( no );
		//_______________________________________________________
		// 　仮のメッシュ領域　確保
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		double	Area = Get_Area(Snum,SXZ),unit=PM.Size*ONE_DEGREE_DISTANCE;
		int     Need = 2*(int)(Area/unit/unit);
		if( SMmem < Need ) {
			if( SMmem == 0 ) SMid = (int*)malloc(Need*sizeof( int ));
			else             SMid = (int*)realloc(SMid,Need*sizeof( int ));
			SMmem = Need;
		}
		//
		// メッシュIDを探す
		XYCOORD		vmin,vmax;
		int			xst,xend,zst,zend;		// バウンダリー範囲
		double		X,Z;

		Find_Min_Max(Snum,SXZ,&vmin,&vmax);

		xst = (int)(vmin.x/(PM.Size*ONE_DEGREE_DISTANCE))-1;  xst = ( xst > 0 )? xst:0;
		zst = (int)(vmin.y/(PM.Size*ONE_DEGREE_DISTANCE))-1;  zst = ( zst > 0 )? zst:0;
		xend = (int)(vmax.x/(PM.Size*ONE_DEGREE_DISTANCE))+1;  
		xend = ( xend < PM.MHnum-1 )? xend:PM.MHnum-1;
		zend = (int)(vmax.y/(PM.Size*ONE_DEGREE_DISTANCE))+1;  
		zend = ( zend < PM.MVnum-1 )? zend: PM.MVnum-1;
		//
		SMnum = 0;
		for(int v=zst;v<=zend;v++) {
			Z = ((double)v + 0.5)*(PM.Size*ONE_DEGREE_DISTANCE);
			for(int h=xst;h<=xend;h++) {
				X = ((double)h + 0.5)*(PM.Size*ONE_DEGREE_DISTANCE);
				if( Is_InRegion(X,Z,Snum,SXZ) ) 	SMid[SMnum++] = v*PM.MHnum + h;
				//
				if( SMnum == SMmem ) {
					SMmem += 20;
					SMid = (int*)realloc(SMid,SMmem*sizeof( int ));
				}
			}
		}
		//_______________________________________________________
		// 　シェイプ名称
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		CShapeName		SN(this);

		SN.m_S_VERT.Format("%d",Snum);
		SN.m_S_AREA.Format("%.1lf",Get_Area(Snum,SXZ)/10000.);
		SN.m_S_MESH.Format("%d",SMnum);
		//
		if( SN.DoModal() == IDCANCEL ) {
			GLobj_Delete( &Gp,OBJID_MOUSE_BAND );
			GL_Redraw(&Gp,FALSE,FALSE,FALSE,UNDEF,TRUE);
			{CFormView::OnRButtonUp(nFlags, point);return;}
		}

		//_______________________________________________________
		// 　エリアラバー消去
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		GLobj_Delete( &Gp,OBJID_MOUSE_BAND );
		GL_Redraw(&Gp,FALSE,FALSE,FALSE,UNDEF,TRUE);
		Change_Color_MeshLine();
	}
	//
	CFormView::OnRButtonUp(nFlags, point);
}
void CRRIView::Get_Shape_Volume(double* vol,double* Area)
{
	////if( CSHAPE == 0 ) {m_Volume="";UDF;return;}
	////
	//SHAPE_BOX*		SBOX = &PM.SBOX[CSHAPE-1];
	//if( ! SBOX->set ) return;
	////
	//double	span_lat,span_lng,ang;
	//int		v,tm = PM.Acnt;
	//*vol = 0.;
	//*Area = 0.;
	////
	////if( tm == PM.GISnum[COUT] ) tm = PM.GISnum[COUT]-1;
	////if( tm < 0 ) {
	////	m_Volume=_T("");UDF;return;}

	//////
	////span_lat = PM.Size*( EARTH_RADIUS_GRS*PAI )/180.;
	////for(int	i=0;i<SBOX->Mnum;i++) {
	////	if( ! MESH[ SBOX->MID[i] ].flag ) continue;
	////	//
	////	v = (int)(SBOX->MID[i]/PM.MHnum);
	////	ang = ( PM.stLAT+v*PM.Size )*PAI/180.;
	////	span_lng = PM.Size*( EARTH_RADIUS_GRS * cos( ang ) )*PAI/180.;
	////	(*Area) += (span_lng * span_lat);
	////	(*vol) += MESH[SBOX->MID[i]].Hval[0][tm] * span_lng * span_lat;
	////}
	//m_Volume.Format("%.1lf",(*vol)/1000.);	// 
	//UDF;
}

///===========================================================================================================
//   コマンドハンドラー
///===========================================================================================================
void CRRIView::OnChangebackcolor()
{
	CColorDialog	cdlg;
	COLORREF		cref;
	//
	if( cdlg.DoModal() == IDCANCEL ) return;
	//
	cref = cdlg.GetColor();
	Gp.Bcolor = F_COLOR( GetRValue( cref ),GetGValue( cref ),GetBValue( cref ) );
	STATG( REDRAW ) = TRUE;
//
	return;
}

void CRRIView::Init_Params()
{
	CVIEW		= ORTHO;
	CLABEL		= 1;		// Adem Altitude 
	COUT		= 0;		// AREA H
	CHANGE_DATA = UNDEF;
	CSHAPE		= 0;		// シェイプ０はNone
	//
	CPRO			= NULL;
	//
	First_Time	= TRUE;
	Cnt_Prof = Cnt_MProf = Cnt_Hydro = 0;		// for Hard_Copy
	//
	Snum = Smem = 0;
	SXZ = NULL;
	SMnum = SMmem = 0;
	SMid = NULL;
	//
	COMMENT_SWITCH = TRUE;
	//
	m_ColoredMesh = FALSE;
	m_Arrow = FALSE;
	///-------------------------------------------------------------------------DSET
	//
	memset(&DSET,0,sizeof( DATASET ));

	for(int i=0;i<3;i++) { WIN_W[i] = 900;WIN_H[i]=600;}
	//
	//CSHAPE = m_ComboShape.AddString("-- shapes --");	// CSHAPE－１がPM.Snumと等しい
	//m_ComboShape.SetCurSel( CSHAPE );
	///------------------------------------------------------------------------ PARAM
	//
	memset(&PM,0,sizeof( PARAMS ));

	PM.B_Color = PM.B_Back = PM.B_Mesh = PM.B_Legend = TRUE;
	PM.B_Date = PM.B_DateBack =  TRUE;
	PM.B_ShowTimeData = TRUE;
	//
	PM.Add_Info = 1;
	//
	PM.stLAT = PM.stLNG = PM.Size = DUNDEF;
	//
	PM.IM_Kind = 0;
	strcpy(PM.IM_Folder, IMAGE_FOLDER );
	//
	PM.Data_Alpha = PM.Alpha = 1.;
	PM.scale.x = PM.scale.y = PM.scale.z = 1.;
	PM.Back_Offset = 100.;
	//
	PM.B_IncludeRiver = FALSE;
	PM.Arrow_UserMag  = 1.;
	PM.Arrow_Ang = 50.;
	PM.Arrow_Head = 0.3;
	//
	// Shape Initial
	PM.Smem = 10;
	if((PM.SBOX = (SHAPE_BOX*)malloc(sizeof( SHAPE_BOX )*PM.Smem)) == NULL ) {
		PM.SBOX = NULL;PM.Smem = 0;}
	for(int s=0;s<PM.Smem;s++) memset((void*)&PM.SBOX[s],0,sizeof( SHAPE_BOX ) );
	//
	PM.Snum = 0;
	//
	PM.OBpos = NULL;
	PM.OBmem = PM.OBnum = 0;
	//
	//
	PM.Mesh_Thresh = 2;
	//
	PM.Tool_Rate = 0.3;
	//
	PM.Col_Mesh = DEFAULT_COL_MESH;
	PM.Col_MeshR	= DEFAULT_COL_MESHR;
	PM.Col_MeshB  = DEFAULT_COL_MESHB;
	//
	PM.Col_Date	= COL_YELLOW;
	PM.Col_DateBack = COL_BLACK;
	PM.Col_LegendChar = COL_BLACK;
	PM.Col_Arrow	= COL_RED;
	//
	PM.SizeID = 8;
	PM.B_WLframe = FALSE;	// 浸水枠線
	//
	////
	for (int k = 0; k<=FILE_OUT_NUM; k++) memset(&PM.OUTCOL[k], 0, sizeof(COLVAL_FIX));
	// 
	//for(int k=0;k<OUT_NUM;k++) PM.GISmem[k] = PM.GISnum[k] = UNDEF;
	//PM.GISfirst = TRUE;
	PM.ctmin = PM.ctmax = 0;
	//
	PM.Anim_Speed = 5;
	PM.Anim_Time = 500;
	PM.WLmag = 1.;
	PM.Col_WLframe = RGB( 0,0,255 );
	strcpy(PM.REC_Head,"Image");
	//
	PM.Tw	= 15;
	PM.Th	= 30;
	PM.Tfont = 5;					// Lucida Sans
	PM.Tstyle = FONT_REGULAR;		// FONT_REGULAR=1, 2, 3
	//
	//
	PM.Lw	= 8;
	PM.Lh	= 20;
	PM.Lfont = 0;					// Arial
	PM.Lstyle = FONT_REGULAR;		// FONT_REGULAR=1, 2, 3
	//
	PM.Pos_Legend.x = 50.;
	PM.Pos_Legend.y = 400.;
	PM.Lsize = 20;
	//
	PM.TSDLG_minH = PM.TSDLG_minV = UNDEF;
	//
	PM.B_Data_Mosaic = TRUE;
	//
	Udata =(MINFO*)NULL;
	//
	Dem_Scale = 10.;
	Image_Sub = 10;
	//
	for(int i=0;i<BMP_Num;i++) pBM[i] = NULL;
	//
	Mnum = Mmem = 0;
	MID = NULL; Mpos = NULL; DEM = HWL = NULL;
//_____________________________________________________
//  READ INI FILE 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// color data

	FILE*			fp;
	char				buf[256],inchar[64];
	int				No,ID,num,pos,r,g,b;
	COLVAL_FIX		*C0;
	if((fp=fopen("./RRI_CONTENTS/COLOR_DATA_V.DAT","r")) == NULL ) {
		MessageBox("** Error **\nCannot Find [ /RRI_CONTENTS/COLOR_DATA_V.DAT ]..","Error",MB_OK);
		exit( 1 );
	}
	//
	while (fgets(buf, 256, fp) != NULL) {
		pos = 0;
		getstr(buf,&pos,inchar,64);	No = atoi( inchar );
		//
		ID = UNDEF;
		if (No >= 100) { ID = No - 100; C0 = &PM.OUTCOL[ID]; }
		//
		getstr(buf, &pos, inchar, 64);  num = atoi(inchar); if (ID != UNDEF) C0->num = atoi(inchar);
		//
		getstr(buf, &pos, inchar, 64);  if (ID != UNDEF)  C0->Equal = (BOOL)atoi(inchar);
		//
		getstr(buf, &pos, inchar, 64);   if (ID != UNDEF) C0->Mos = (BOOL)atoi(inchar);
		//
		getstr(buf, &pos, inchar, 64);  if (ID != UNDEF)  C0->alpha = (float)atof(inchar);
		//
		for(int n=0;n<num;n++) {
			if (ID != UNDEF) {
				fgets(buf, 256, fp); pos = 0;
				getstr(buf, &pos, inchar, 64); C0->val[n] = atof(inchar);
				getstr(buf, &pos, inchar, 64); r = atoi(inchar);
				getstr(buf, &pos, inchar, 64); g = atoi(inchar);
				getstr(buf, &pos, inchar, 64); b = atoi(inchar);
				C0->Rcol[n] = RGB(r, g, b);
				C0->Fcol[n] = F_COLOR(r, g, b);
			}
		}
		C0->set = TRUE;
	}
	fclose( fp );		
	//_____________________________________________________
	//  BitMap Buttons
	/*int			BMP_Num = 6;
	int			BMP_ID[6] = {IDB_BITMAP_START,IDB_BITMAP_REWIND,
							IDB_BITMAP_BACK,IDB_BITMAP_NEXT,
							IDB_BITMAP_FORWARD,IDB_BITMAP_REC};
	*/
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	CButton*	cb;
	CBitmap*	bb;
	//
	for (int i = 0; i<BMP_Num; i++) {
		cb = (CButton*)GetDlgItem(BButton_ID[i]);
		bb = new CBitmap;
		bb->LoadBitmapA(BMP_ID[i]);
		cb->SetBitmap(*bb);
		//
		pBM[i] = bb;
	}
}
///-------------------------------------------------------
//   RENDER SCENE
///-------------------------------------------------------

void	CRRIView::Render_Scene() {
	if( FAILED( GL_Render_Scene( &Gp)) ) {RETURN( "Failed !");}
	RETURN("Succeeded");
}

///===========================================================================================================
//   ON  TIMER 
///===========================================================================================================
void CRRIView::OnTimer(UINT_PTR nIDEvent)
{
//_____________________________________________________
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  INIT TIMER 
//_____________________________________________________
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if( nIDEvent == INIT_TIMER ) {
		KillTimer( INIT_TIMER );
		//
		COMMENT("Now Initializing...");
///_____________________________________________________
//  表示領域の確保
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		CRect		m_ARect;
		CWnd*		cw=(this);
		int			Xspan,Yspan;
		//
		cw->GetWindowRect( &Gp.win.WinRect );
		cw->GetClientRect( &m_ARect );
		Gp.win.WinCRect = m_ARect;
		//
		Gp.win.wWidth = m_ARect.Width();
		Gp.win.wHeight= m_ARect.Height();
		//
		m_ARect.left	+= (Menu_Margin+FRAME_MARGIN);
		m_ARect.right	-= FRAME_MARGIN;
		m_ARect.top		+= FRAME_MARGIN;
		m_ARect.bottom	-= FRAME_MARGIN;
		Xspan = m_ARect.Width(); Yspan = m_ARect.Height();
		Gp.win.Cpict.Create(NULL,WS_VISIBLE|SS_SUNKEN,m_ARect,cw,ID_PICT);
		//
		/// Get Information
		Gp.win.cw		= GetDlgItem( ID_PICT );
		Gp.win.hWnd		= Gp.win.cw->m_hWnd;
		//
		Gp.win.cw->GetClientRect( &Gp.win.Crect );
		//m_ARect.top = m_ARect.left = 0;
		//m_ARect.bottom = 1013; m_ARect.right = 1654;
//		Gp.win.cw->GetClientRect( &Gp.win.MAXrect );
		Gp.win.MAXrect = Gp.win.Crect;
		Gp.win.cWidth	= Gp.win.Crect.Width();
		Gp.win.cHeight	= Gp.win.Crect.Height();
		//
		///  マウス信号にあった領域の変更
		Gp.win.cw->GetWindowRect( &Gp.win.Wrect );
		Gp.win.Wrect.left -=( Gp.win.WinRect.left + ( Gp.win.WinRect.Width() - Gp.win.wWidth )/2 -1);
		Gp.win.Wrect.top  -=( Gp.win.WinRect.top + ( Gp.win.WinRect.Height() - Gp.win.wHeight )/2 -1);
		Gp.win.Wrect.right = Gp.win.Wrect.left + Gp.win.Crect.Width();
		Gp.win.Wrect.bottom = Gp.win.Wrect.top + Gp.win.Crect.Height();
		//
		Fill_Color(Gp.win.cw, RGB(0, 0, 0));
		///----------------------------------------------------
		::ValidateRect(Gp.win.hWnd,&Gp.win.Crect );			// WM_PAINT の無効化
//_____________________________________________________
//  ＯＰＥＮＧＬ　初期化
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		Gp.Gmax		= MAX_OBJECT;
		if( FAILED( GL_Init( &Gp ) ) ) {
			RETURN("****  ERROR **** GL_Init Failed");
		}
		for(int i=0;i<Gp.Gmax;i++) Gp.Gobj[i].set = FALSE;

		//  Font   -------------------------------------
		GL_Init_Font(&Gp);

		// Attribute          --------------------------
		FCOLOR	bcol=F_COLOR( 0, 0, 0 );
		//if( FAILED( GL_Attrib(&Gp, DIM_3D, ORTHO, bcol,0  ) )) return;
		if( FAILED( GL_Attrib(&Gp, DIM_3D, ORTHO, VIEW_XZ,bcol,0  ) )) return;

		//Gp.cam[CVIEW].viewdir.z = 1.;
		//Gp.cam[CVIEW].viewdir.x = Gp.cam[CVIEW].viewdir.y = 0.;

		// Base Texture       --------------------------
		//if( ! Gp.SetTex ) {
		//	if( FAILED( Set_Tex_Image() ) ) exit( 0 );
		//}
		//
		CString		str;
		str.Format("Ready.....OpenGL [ GPU::: %s ( %s ), ver %s ::: Max Res  %d ]",
								Gp.renderer,Gp.vendor,Gp.version,Gp.Max_Res);
		COMMENT(str);
///_______________________________________________________________
//  ツールチップの初期化
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		cw = GetDlgItem(ID_PICT);
		m_ToolTip.AddTool(cw, "");
		m_ToolTip.Pop();
///_____________________________________________________
//  Set Parameter after decided GP.win
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		PM.Pos_Time.x = 100.;
		PM.Pos_Time.y = Gp.win.cHeight - 100.;
		PM.Col_Date = RGB(255,255,0);
		PM.Pos_Legend.x = 50.;
		PM.Pos_Legend.y = 300.;
		PM.Size_Legend.x = 40.;
		PM.Size_Legend.y = 400.;
///_____________________________________________________
//  START
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// プロジェクトフォルダの取得　　INIT のAsK_Case でRRI＿Inputは読まれている
		/// 
		if (Set_Case()) exit(1);				// MESH にアロケート ファイルの読み込み
		Change_Cursor(&Gp, IDC_WAIT);  

		///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//  ツールの作成

		NOWX = TOOL_DEFAULT_X; NOWY = TOOL_DEFAULT_Y;
		Make_Screen_Tool(TRUE);				// ツールの作成とテックスの初期化
		//
		///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//  表示

		Check_Location_Set(FALSE);

		Make_Mesh_Line();
		if(!DSET.SW_LatLong) Make_Back_Image();
		//
		//Make_Mesh_Model();
		Set_WL_Model();
		//
		Draw_Date();
		Draw_Legend();
		Draw_Location();
		//
		Disp_Controls();
		GL_Redraw(&Gp,TRUE,TRUE,TRUE,ALL_OBJECTS,TRUE);
		//
		SetTimer( RENDER_TIMER,100,NULL);
	}
//_____________________________________________________
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//   RENDER TIMER
///_____________________________________________________
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if( nIDEvent == RENDER_TIMER ) {
		IF_STATG( REDRAW ) {
			if( COMMENT_SWITCH ) COMMENT("Now Prepare Screen.....");
			Change_Cursor( &Gp,IDC_WAIT );
			Gp.win.cw = GetDlgItem( ID_PICT );
			Render_Scene();
//	
			Change_Cursor( &Gp,IDC_ARROW );
			//
			if (PM.Acnt != PM.SaveAcnt) Disp_Anim_Time();
			PM.SaveAcnt = PM.Acnt;
			//
			double		vol,Area;
			Get_Shape_Volume(&vol,&Area);
			//
			Disp_Info();
			//
			if( First_Time ) {			// 最初の表示がないというエラー回避
				GL_Redraw(&Gp,FALSE,FALSE,TRUE,ALL_OBJECTS,TRUE);
				First_Time = FALSE;
				STATG(ON_SCREEN) = TRUE;
				PM.Model = TRUE;
				//
			}
		}
		//
		IF_STATG( MS_RUBBER_START ) {
			HWND	hw = ::GetFocus();
		}

		// Profile Fig
		if( PM.Profile_Proc ) {		// Cancel も入場させて すべてをクリヤーする。
			PM.Profile_Proc = FALSE;
			STATG(DISP_PROFILE) = FALSE;
			CPRO = NULL;
		}
		// Mesh Profile Fig
		if( PM.MeshProfile_Proc ) {		// Cancel も入場させて すべてをクリヤーする。
			PM.MeshProfile_Proc = FALSE;
			STATG(DISP_MESHPROFILE) = FALSE;
			//
			SAFE_FREE( MID );
			SAFE_FREE( Mpos );
			SAFE_FREE( DEM );
			SAFE_FREE( HWL );
			Mnum = Mmem = 0;
			PM.MPro_draw_Acnt = UNDEF;
			//
			MPROF = NULL;
			//
			GLobj_Delete( &Gp,OBJID_MOUSE_BAND );	// バンドの消去
			GL_Redraw(&Gp,FALSE,FALSE,TRUE,OBJID_MOUSE_BAND,TRUE);
		}
		///_____________________________________________________
		//  連動　縦断図
		///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		CTime	Now;
		if( STATG( DISP_PROFILE ) && PM.Set_Line ) {
			if( PM.Acnt <= DSET.DT_Out ) {
				int		Dcnt = PM.Acnt;

				Now = DSET.ST + CTimeSpan(0l, (DSET.Hours / DSET.DT_Out*PM.Acnt), 0, 0);	// メインの表示時刻
				CPRO->CCT = Now;	// メインの表示時刻
				CPRO->Data_Exist = TRUE;
				//
				if( (PM.Pro_draw_Acnt != Dcnt ) && PM.Pro_Ready ) {
					for(int i=0;i<PM.LineNum;i++) {
						PM.Pval[0][i] = MESH[PM.Path[i].y][PM.Path[i].x].Adem +
							MESH[PM.Path[i].y][PM.Path[i].x].Hval[OUT_HR][Dcnt] - MESH[PM.Path[i].y][PM.Path[i].x].Rdepth;
					}
					//
					PM.Pro_draw_Acnt = Dcnt;
					CPRO->Make_Time_Fig();
				}
			}
		}
		if( STATG( DISP_MESHPROFILE ) && PM.MeshProfile_Ready )  {
			if (PM.Acnt <= DSET.DT_Out) {
				int		Dcnt = PM.Acnt;
				//
				Now = DSET.ST + CTimeSpan(0l, (DSET.Hours / DSET.DT_Out*PM.Acnt), 0, 0);	// メインの表示時刻
				MPROF->CCT = Now;	// メインの表示時刻
				//
				MPROF->Data_Exist = TRUE;
				//
				if( PM.MPro_draw_Acnt != Dcnt)  {
					for(int i=0;i<Mnum;i++) {
						HWL[i] = DEM[i];
						if (MESH[MID[i].y][MID[i].x].flag)
							HWL[i] = DEM[i] + MESH[MID[i].y][MID[i].x].Hval[OUT_HS][Dcnt];
					}
					//
					PM.MPro_draw_Acnt = Dcnt;
					MPROF->HWL = HWL;
					MPROF->Make_Profile();
				}
			}
		}
	}
//_____________________________________________________
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//   ANIMATION TIMER
///_____________________________________________________
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	else if( nIDEvent == ANIMATION_TIMER ) {
		if( PM.Acnt <= DSET.DT_Out ) {
			COMMENT("Now Prepare Screen.....");
			//
			// Draw Data
			Redraw_Now();			
			// 
			Disp_Anim_Time();
			// Render
			Render_Scene();
			//Recording
			Record_Now();
			//
			double		vol,Area;
			Get_Shape_Volume(&vol,&Area);
			//
			//_____________________________________________________
			//  連動　縦断図
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			CTime	Now;
			if( PM.Profile_Proc ) {		// Cancel も入場させて すべてをクリヤーする。
				PM.Profile_Proc = FALSE;
				STATG(DISP_PROFILE) = FALSE;
				CPRO = NULL;
			}
			if( STATG( DISP_PROFILE ) && PM.Set_Line ) {
				if (PM.Acnt <= DSET.DT_Out) {
					int		Dcnt = PM.Acnt;
					//
					Now = DSET.ST + CTimeSpan(0l, (DSET.Hours / DSET.DT_Out*PM.Acnt), 0, 0);	// メインの表示時刻
					CPRO->CCT = Now;	// メインの表示時刻
					CPRO->Data_Exist = TRUE;
					if ((PM.Pro_draw_Acnt != Dcnt) && PM.Pro_Ready) {
						for(int i=0;i<PM.LineNum;i++) {
							if (CPRO->Data_Exist) 	PM.Pval[0][i] = MESH[PM.Path[i].y][PM.Path[i].x].Adem +
								MESH[PM.Path[i].y][PM.Path[i].x].Hval[OUT_HR][Dcnt] - MESH[PM.Path[i].y][PM.Path[i].x].Rdepth;
							else                    PM.Pval[OUT_HS][i] = 0.;
						}
						//
						PM.Pro_draw_Acnt = Dcnt;
						CPRO->Make_Time_Fig();
					}
				}
			}
			if( STATG( DISP_MESHPROFILE ) && PM.MeshProfile_Ready )  {
				int		Dcnt = PM.Acnt;
				//
				Now = DSET.ST + CTimeSpan(0l, (DSET.Hours / DSET.DT_Out*PM.Acnt), 0, 0);	// メインの表示時刻
				MPROF->CCT = Now;	// メインの表示時刻
				//
				MPROF->Data_Exist = TRUE;
				//
				if (PM.MPro_draw_Acnt != Dcnt)  {
					for (int i = 0; i<Mnum; i++) {
						HWL[i] = DEM[i];
						if (MESH[MID[i].y][MID[i].x].flag)
							HWL[i] = DEM[i] + MESH[MID[i].y][MID[i].x].Hval[OUT_HS][Dcnt];
					}
					//
					PM.MPro_draw_Acnt = Dcnt;
					MPROF->HWL = HWL;
					MPROF->Make_Profile();
				}
			}
			//
			// Set Next
			PM.Acnt++;
			if (PM.Acnt <= DSET.DT_Out) m_cRunTime.SetPos(PM.Acnt);
			COMMENT("Ready......");Change_Cursor( &Gp,IDC_ARROW );
		}
		///
		if (PM.Acnt > DSET.DT_Out) {
			PM.Acnt = DSET.DT_Out;
			//
			delete pBM[0];
			pBM[0] = new CBitmap;
			pBM[0]->LoadBitmapA( IDB_BITMAP_START );

			CButton*	cb = (CButton*)GetDlgItem( IDC_V_BUTTON_START );
			cb->SetBitmap( *pBM[0] );
			STATG( RUN_ANIMATION ) = FALSE;
			//
			// Stop Recording
			if( STATG( RECORD_MODE ) ) {
				OnBnClickedVButtonRec();
			}
			// End
			KillTimer( ANIMATION_TIMER );	//*** 注意
			//
			SetTimer(RENDER_TIMER,100,NULL);
			COMMENT("End Animation......");
		}
	}

	CFormView::OnTimer(nIDEvent);
}
//___________________________________________________________________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ケース設定、データファイルの読込
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-----------------------------------------------------------------------------------------------------------
//  Set Input Files
//-----------------------------------------------------------------------------------------------------------
int	 CRRIView::Set_Case()
{
	if( DSET.Cout_Exist <= 0  ) {
		MessageBox("Output results not exist...", "exit", MB_OK | MB_ICONHAND); return 1;
	}

//
///  モデルの作成と計算結果の読込----------------------------------------

	Change_Cursor( &Gp,IDC_WAIT );
	if(FAILED( Make_Data() ))   {	// MESH アロケート ,基本ファイルの読み込み
		COMMENT("** Error ** Failed Make Datas");Change_Cursor( &Gp,IDC_ARROW );return 100;}


	//			getstr(buf,&pos,inchar,SIZE_01K); lines = atoi( inchar );
	//			PM.OBpos[PM.OBnum].y = PM.MVnum - lines;
	//			PM.OBnum++;
	//		}
	//		fclose( fp );
	//	}
	//}
//
//  表示-----------------------------------------------------
	Change_Cursor( &Gp,IDC_ARROW );
	Disp_Info();
	return 0;
}
//-----------------------------------------------------------------------------------------------------------
//  SET SHAPES
//-----------------------------------------------------------------------------------------------------------
void CRRIView::Set_Shapes(CString	Folder)
{
	struct _finddata_t		c_file;		// time_write が作成時間を示す
	long					hFile;
	int						fc,id;
	CString					fname;
	//
	::SetCurrentDirectoryA( Folder );
	if( (hFile = _findfirst( "*.*", &c_file )) == -1L ) RETURN("No Shape Files");
	//
	do {
		fname = c_file.name;
		if( fname.Right(3) == "sdd") {
			fc = _open(fname,_O_BINARY|_O_RDONLY);
			id = PM.Snum;
			read(fc,(void*)&PM.SBOX[ id ],sizeof( SHAPE_BOX ) );
			PM.SBOX[id].XZ = (XYCOORD*)malloc( sizeof( XYCOORD )*PM.SBOX[id].Vnum);
			PM.SBOX[id].MID = (int*)malloc( sizeof( int )*PM.SBOX[id].Mnum);
			//
			read(fc,(void*)PM.SBOX[id].XZ,sizeof( XYCOORD )*PM.SBOX[id].Vnum );
			read(fc,(void*)PM.SBOX[id].MID,sizeof( int )*PM.SBOX[id].Mnum);
			//
			close( fc );
			PM.Snum++;
		}
	} while( _findnext(hFile,&c_file) == 0L );
	_findclose( hFile );
}
//-----------------------------------------------------------------------------------------------------------
//  RRI INPUT
//-----------------------------------------------------------------------------------------------------------
//void CRRIView::OnBnClickedMButtonInput()
//{
//	CSETFILE	SF;
//	//
//	SF.MESH = MESH;		// MESHにデータがセットされていることが必要
//	//
//	if( SF.DoModal() != IDCANCEL ) { 
//		if( DSET.Cgis_Exist > 0 ) {
//			CTabCtrl*	 MTab = (CTabCtrl*)GetDlgItem( IDC_TAB_MENU );
//			MTab->EnableWindow( TRUE );
//		}
//	}
//	//
//	Disp_Controls();
//}
//-----------------------------------------------------------------------------------------------------------
//  モデル作成
//-----------------------------------------------------------------------------------------------------------
HRESULT CRRIView::Make_Data()
{
	if( ! DSET.set ) return E_FAIL;
	//
	FILE*		fp;
	CString		fname,str;
	//
	double		Clat,Clng,val;
	int			id;
	//
	int			Hn,Vn;
	char			buf[SIZE_1K];
	//
	if((MESH = (MINFO**)calloc(PM.MVnum,sizeof(void* ))) == NULL ) return E_FAIL;
	for (int v = 0; v < PM.MVnum; v++) {
		if ((MESH[v] = (MINFO*)calloc(PM.MHnum,sizeof(MINFO))) == NULL) return E_FAIL;
	}
	//
	Clat = PM.stLAT;
	for(int v=0;v<PM.MVnum;v++) {						// MINFO の初期設定
		Clng = PM.stLNG;
		for(int h=0;h<PM.MHnum;h++) {
			id = v*PM.MHnum + h;
			MESH[v][h].Lat	= Clat;
			MESH[v][h].Lng	= Clng;
			//
			MESH[v][h].PolyPno = MESH[v][h].LinePno	= UNDEF;
			MESH[v][h].uv[0]  = fmod( Clng,1. );
			MESH[v][h].uv[1]	= fmod(Clat+PM.Size,1.);
			//
			MESH[v][h].Adem = MESH[v][h].Rdepth = MESH[v][h].Rwidth = DUNDEF;
			MESH[v][h].Bheight = MESH[v][h].Acc = DUNDEF;
			MESH[v][h].Adir  = UNDEF;
			MESH[v][h].ID_RIVPATH = UNDEF;
			//
			MESH[v][h].Hval = NULL;
			Clng += PM.Size;
		}
		Clat += PM.Size;
	}
	// 連絡用
	if( (PM.Flags = (BYTE*)malloc(PM.MHnum*PM.MVnum)) == NULL ) {
		for (int v = 0; v < PM.MVnum; v++) SAFE_FREE(MESH[v]);
		SAFE_FREE( MESH );return E_FAIL;}
	if( (PM.Values = Make_Matrix(PM.MHnum,PM.MVnum,0.)) == NULL ) {
		for (int v = 0; v < PM.MVnum; v++) SAFE_FREE(MESH[v]);
		SAFE_FREE(MESH); SAFE_FREE(PM.Flags); return E_FAIL;
	}
	//
	PM.Tnum = PM.MHnum*PM.MVnum;
	PM.Anum = 0;
///------------------------------------------------------------------------------------------------------------
//  READ MODEL DATA START    2015 0201
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	COMMENT("Now  Reading  Data.....");
	for (int f = ADEM_FILE; f<=HEIGHT_FILE; f++) {
		if (f == LU_FILE) continue;
		//
		if (DSET.in_use[f]) {
			fname = DSET.File_names[f];
			Read_Data_File(f, fname, MESH);
			DSET.in_avail[f] = TRUE;
		}
		else {
			if (f >= WIDTH_FILE && f <= HEIGHT_FILE) {
				for (int v = 0; v < PM.MVnum; v++) {						// MINFO の初期設定
					for (int h = 0; h < PM.MHnum; h++) {
						if (!MESH[v][h].flag) continue;
						//
						switch (f) {
						case WIDTH_FILE:	if (MESH[v][h].Acc >= DSET.Riv_Thresh)  MESH[v][h].Rwidth = DSET.Cof_WC*pow(MESH[v][h].Acc, DSET.Cof_WS);
											else MESH[v][h].Rwidth = NODATA;
											break;
						case DEPTH_FILE:if (MESH[v][h].Acc >= DSET.Riv_Thresh)  MESH[v][h].Rdepth = DSET.Cof_DC*pow(MESH[v][h].Acc, DSET.Cof_DS);
										else MESH[v][h].Rdepth = NODATA;
										break;
						case HEIGHT_FILE:if (MESH[v][h].Acc >= DSET.Cof_Hlim)  MESH[v][h].Bheight = DSET.Cof_H;
										 else MESH[v][h].Bheight = NODATA;
										 break;
						}
					}
				}

			}
		}
	}
///------------------------------------------------------------------------------------------------------------
// 有効なメッシュだけにHVALのメモリーを与えておく
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for(int v=0;v<PM.MVnum;v++) {						// MINFO の初期設定
		for(int h=0;h<PM.MHnum;h++) {
			id = v*PM.MHnum + h;
			if( ! MESH[v][h].flag ) continue;
			//	
			if( (MESH[v][h].Hval = (double**)malloc(FILE_OUT_NUM*sizeof( void *))) == NULL ) return E_FAIL;
			for (int k = 0; k < FILE_OUT_NUM; k++) 	MESH[v][h].Hval[k] = NULL;
			// 存在した場合にアロケートする。
		}
	}

///------------------------------------------------------------------------------------------------------------
// OUT ファイルの読込
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int			cnt=0;
	CBUSY*		CB;
	CB = new CBUSY;
	CB->Create( IDD_BUSY );
	CB->Set_Prog( 0,DSET.Cout_Exist*2 );
	CB->ShowWindow( SW_SHOW );
	//
	COUT = 0;				// Inundeation Data
	CB->Set_Pos( cnt++ );

	//
	for(int f=0;f<FILE_OUT_NUM-1;f++) {		// Storage は読まない。

		if( ! DSET.out_use[f]  ||  ! DSET.out_exist[f]  ) {	// 使わない　もしくは　存在しない
			if( COUT == f ) COUT++;
			continue;
		}
		///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// ｈｓ の場合は各メッシュのhsmaxをUNDEFにしておく

		for (int v = 0; v < PM.MVnum; v++) {
			for (int h = 0; h < PM.MHnum; h++) {
				if (MESH[v][h].flag){
					if (f != OUT_STORAGE )
						MESH[v][h].Hval[f] = (double*)calloc((DSET.DT_Out + 1), sizeof(double)); // 出力は１から　0を含む
					else
						MESH[v][h].Hval[f] = (double*)calloc(2, sizeof(double)); // 本来は1つだけでいい 
				}
				if (f == OUT_HS) MESH[v][h].hsmax = DUNDEF;
			}
		}
		str.Format("Now reading  %s", OUT_MENU_NAME[f]);
		CB->Set_Title( str );
		CB->Set_Pos( cnt++ );	
		//
		if (f != OUT_STORAGE) {
			for (int t = 1; t <= DSET.DT_Out; t++) {
				fname.Format("%s%06d.out", DSET.Out_names[f], t);
				if (FAILED(Read_Result_File(f, t, fname))) return E_FAIL;
				///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
				//   データ読み込み後の処理 vector
				if (f == OUT_QV) {		// U,V が読み込まれた
					double	Vmax;
					Vmax = PM.Arrow_Pair.x*PM.Arrow_Pair.x + PM.Arrow_Pair.y*PM.Arrow_Pair.y;
					PM.Arrow_Max = sqrt(Vmax);
					//
					PM.Arrow_mag = 1. / PM.Arrow_Max;  // メッシュ幅＝１とする。
				}
			}
			double	vec, Vmax = 0.;
			if (f == OUT_QV) {
				DSET.Max_QV = 0.;
				for (int t = 1; t <= DSET.DT_Out; t++) {
					for (int v = 0; v < PM.MVnum; v++) {
						for (int h = 0; h < PM.MHnum; h++) {
							if (MESH[v][h].flag == 0) continue;
							vec = MESH[v][h].Hval[OUT_QU][t] * MESH[v][h].Hval[OUT_QU][t] +
								MESH[v][h].Hval[OUT_QV][t] * MESH[v][h].Hval[OUT_QV][t];
							vec = sqrt(vec);
							if (vec > DSET.Max_QV)  DSET.Max_QV = vec;
						}
					}
				}
				DSET.ArrowMag_QV = 1.5 / DSET.Max_QV;
			}
			CB->Set_Pos(cnt++);
			//
			if (f == OUT_GV) {
				DSET.Max_GV = 0.;
				for (int t = 1; t <= DSET.DT_Out; t++) {
					for (int v = 0; v < PM.MVnum; v++) {
						for (int h = 0; h < PM.MHnum; h++) {
							if (MESH[v][h].flag == 0) continue;
							vec = MESH[v][h].Hval[OUT_GU][t] * MESH[v][h].Hval[OUT_GU][t] +
								MESH[v][h].Hval[OUT_GV][t] * MESH[v][h].Hval[OUT_GV][t];
							vec = sqrt(vec);
							if (vec > DSET.Max_QV) vec = DSET.Max_QV;
						}
					}
				}
				DSET.ArrowMag_GV = 1.5 / DSET.Max_GV;
			}
			CB->Set_Pos(cnt++);
		}
		else {		// Storage   20150219 時点で使用しない。
			if (FAILED(Read_Result_File(f, 0, DSET.Out_names[f]))) return E_FAIL;
		}
	}
	//
	CB->Close();
	//
	COUT = 0;
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	PM.set = TRUE;
	m_cRunTime.SetRange(0,DSET.DT_Out,FALSE);
	m_cRunTime.SetPos( 0 );
	//
	return S_OK;
}
///-----------------------------------------------------------------------------------------------------------
//   READ OUT FILE 使用しないが残しておく
///-----------------------------------------------------------------------------------------------------------
HRESULT CRRIView::Read_Result_File(int Fid, int cnt, CString fname)
{
	FILE*		fp;
	char			buf[SIZE_16K],inchar[64];
	int			lcnt=0,FV,FH,pos;
	int			id,year,month,day,hour,min;
	CString		str,str1,str2;
	int			Overcnt, vr,idv,idh;

	if((fp=fopen(fname,"r")) == NULL ) {
		str.Format("*** ERROR ** Cannot Open Out File...[ %s ] !",fname);COMMENT(str);return E_FAIL;}
	//
	if (Fid != OUT_STORAGE) {
		double		Vmax;
		//
		Vmax = DUNDEF;
		for (int v = 0; v < PM.MVnum; v++) {
			fgets(buf, SIZE_16K, fp);						// Date
			pos = 0;
			vr = PM.MVnum - v - 1;
			for (int h = 0; h < PM.MHnum; h++) {
				getstr(buf, &pos, inchar, SIZE_16K);
				if (MESH[vr][h].flag) {
					MESH[vr][h].Hval[Fid][cnt] = atof(inchar);
					//
					if (Fid == OUT_HS) {
						if (MESH[vr][h].hsmax < MESH[vr][h].Hval[Fid][cnt])MESH[vr][h].hsmax = MESH[vr][h].Hval[Fid][cnt];
					}
					if (Vmax < MESH[vr][h].Hval[Fid][cnt]) Vmax = MESH[vr][h].Hval[Fid][cnt];
				}
			}
		}
	}
	else{			// Ｓｔｏｒａｇｅの場合は10個づつの指数表現
		idv = PM.MVnum - 1; idh = 0;
		while (fgets(buf, SIZE_16K, fp) != NULL) {
			pos = 0;
			for (int i = 0; i < 10; i++) {
				getstr(buf, &pos, inchar, 64);
				if (MESH[idv][idh].flag != 0) 	MESH[idv][idh].Hval[Fid][cnt] = atof(inchar);
				//
				idh++;
				if (idv == 0 && idh == PM.MHnum) goto End_Proc;
				//
				if (idh % PM.MHnum == 0) {	idv--; idh = 0;}
			}
		}
	}
End_Proc:
	fclose( fp );
//
	COMMENT("Ready.....");
	return S_OK;
}
//___________________________________________________________________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    モデルの作成
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-----------------------------------------------------------------------------------------------------------
//  テクスチャーの設定
//-----------------------------------------------------------------------------------------------------------
HRESULT CRRIView::Set_Tex_Image()
{
	int			lat,lng,ResH,ResV,imbyte,Resmax;
	CString		title,fname,cmd;
	BYTE*		pImage;
	CBUSY*		CB;
	char		Cname[128];
	//
	Change_Cursor( &Gp,IDC_WAIT );
	//
	CB = new CBUSY;
	CB->Create( IDD_BUSY );
	CB->Set_Prog( 0,40 );
	CB->ShowWindow( SW_SHOW );
	//
	GL_Tex_Init(&Gp,MAX_TEXTURE*2+5);				// 初期化
	//
	for(int i=0;i<MAX_TEXTURE;i++) {
		CB->Set_Pos( i+ 1 );
		lat = i/LNG_SPAN + (int)TEX_START_LAT;
		lng = i%LNG_SPAN + (int)TEX_START_LNG;
		title.Format("N%2dE%03d.bmp",lat,lng);
		fname.Format("%s\\%s",PM.IM_Folder,title);
		//
		strcpy(Cname,fname);
		if( ! File_Exist( Cname ) ) {
			CB->Close();
			cmd.Format("** Error ** File Not Exist...\n   file=%s",Cname);
			MessageBox(cmd,"Missed",MB_OK|MB_ICONASTERISK);
			return E_FAIL;
		}
		//
		if( ! Get_Image_Size(fname,&ResH,&ResV,&imbyte) ) {
			cmd.Format("** Error ** Get Information Error...   file=%s",fname);
			COMMENT( cmd );
			return E_FAIL;
		}
		//
		if( i == 0 ) {							// 登録イメージの解像度
			Gp.Disp_Size = 1;
			Resmax = max(ResH,ResV);
			while ( Resmax > Gp.SUBDEV_BASE ) {Gp.Disp_Size *= 2; Resmax /=2;}
		}
		pImage = Get_Image_File(fname,&ResH,&ResV,&imbyte);
		if( pImage == NULL ) {
			cmd.Format("** Error ** Get Image Data Error...   file=%s",fname);
			COMMENT( cmd );
			return E_FAIL;
		}
		//
		GL_Set_Bitmap(i,&Gp,ResH,ResV,imbyte,Gp.Disp_Size,pImage,FALSE);			// カラー登録
		//
		GL_Set_Bitmap(i+MAX_TEXTURE,&Gp,ResH,ResV,imbyte,Gp.Disp_Size,pImage,TRUE);	// グレイ登録
		//
		SAFE_FREE( pImage );
	}
	CB->Close();
	//
	Gp.SetTex = TRUE;
	Change_Cursor( &Gp,IDC_ARROW );
	return S_OK;
}
//-----------------------------------------------------------------------------------------------------------
//  モデル作成
//-----------------------------------------------------------------------------------------------------------
//HRESULT CRRIView::Make_Model()
//{
////	GL_OBJECT*			Mobj;
////	GL_OBJECT*			Lobj;
////	int					Mid;
////	double				X,Y,Z;
////	CFile				cf;
////	CFileException		fileException;
////	BOOL					DEM_Exist;
////	XYZCOORD*			xyz = (XYZCOORD*)malloc(DEM_H*DEM_V*sizeof( XYZCOORD ));
////	//
////	//if( xyz == NULL )  {	COMMENT("** ERROR ** Cannot Allocate");return E_FAIL;}
////	////-----------------------------------------------------------------------------------------------
////	//// DEMデータの読み込み
////	////
////	//if( CVIEW == PERS ) {
////	//	COMMENT("Read  DEM Data.....");
////	//	::SetCurrentDirectoryA( Gp.m_CurrentPath );
////	//	if( DEM_Exist = cf.Open(DSET.File_names[ADEM_FILE],CFile::modeRead,&fileException) ) {
////	//		for(int i=0;i<DEM_H*DEM_V;i++) 	cf.Read((void*)&xyz[i],sizeof( XYZCOORD ));
////	//		cf.Close();
////	//	}
////	//	else {	// ０設定
////	//		for(int v=0;v<DEM_V;v++) {
////	//			for(int h=0;h<DEM_H;h++) {
////	//				xyz[v*DEM_H+h].x = TEX_START_LNG + h/120.;
////	//				xyz[v*DEM_H+h].z = TEX_START_LAT + v/120.;
////	//				xyz[v*DEM_H+h].y = 0.;
////	//			}
////	//		}
////	//	}
////
////	//}
////	//-----------------------------------------------------------------------------------------------
////	// モデルデータの初期化
////	//
////	//Mobj = &Gp.Gobj[OBJID_BACK];
////	//Lobj = &Gp.Gobj[OBJID_LINE];
////	////
////	//GLobj_Delete(&Gp, OBJID_BACK);
////	//GLobj_Delete(&Gp, OBJID_LINE);
////	////
////	//GLobj_Init(Lobj, (PM.Anum + 10) * 4, (PM.Anum + 10));
////	///-----------------------------------------------------------------------------------------------
////	// Vert
////	//COMMENT("Model Data.....VERT");
////	//double		lat,lng;
////	//int			id,nlat,nlng;
////	//int			Step,Hcnt,Vcnt;
////	//
////	//if( PM.Size == 1./120. ) {		// ３０”
////	//	Step = 120/Image_Sub;
////	//	Hcnt = PM.MHnum/Step + 1;
////	//	Vcnt = PM.MVnum/Step + 1;
////	//}
////	//else {							// ６０”
////	//	if( Image_Sub > 60 ) Image_Sub = 60;
////	//	Step = 60/Image_Sub;
////	//	Hcnt = PM.MHnum/Step + 1;
////	//	Vcnt = PM.MVnum/Step + 1;
////	//}
//////	GLobj_Init(Mobj, Hcnt*Vcnt,Hcnt*Vcnt);	// 6分毎=0.1のメッシュ
//////	double			LAT,LNG,dT=1./(double)Image_Sub;
//////	int				cnt=0;
//////	//
//////	for(int v=0;v<=PM.MVnum;v++) {
//////		lat		= PM.stLAT + v*PM.Size;
//////		nlat	= (int)((lat - DEM_LAT)/(DEM_SIZE));
//////		for(int h=0;h<=PM.MHnum;h++) {
//////			lng		= PM.stLNG + h*PM.Size;
//////			nlng	= (int)((lng - DEM_LNG)/(DEM_SIZE));	
//////			id	= nlat*DEM_H + nlng;
//////			X	= h*PM.Size*ONE_DEGREE_DISTANCE;
//////			Z	= (v*PM.Size*ONE_DEGREE_DISTANCE);			// 左手系座標はＺ方向が下 一時保留
//////			//
////// 			if( CVIEW == PERS )	Y	= -xyz[id].y / Dem_Scale;	// 下から見ているため、負の値が大きいほうが高い
//////			else						Y  = 0.;
//////			//
//////			double oddv = abs(fmod(lat,dT)-dT),oddh = abs(fmod(lng,dT)-dT);
//////
//////			if( v == 0 || v == PM.MVnum || oddv <= EPS5 ) {
//////				if( h == 0 || h == PM.MHnum || oddh <= EPS5 )		GLobj_Add_Vert(Mobj,X,Y,Z,FALSE);
//////			}
//////			//
//////			GLobj_Add_Vert(Lobj,X,Y-LINE_OFFSET,Z,FALSE);		// 負の方が手前
//////		}
//////	}
//////	//-----------------------------------------------------------------------------------------------
//////	// Poly
//////	COMMENT("Model Data.....POLY");
//////	int					Texno,no[4],NO[4],pno,check,plus;
//////	FXYCOORD		uv[4];
//////	FCOLOR			FCOL_MESH,FCOL_MESHR,FCOL_MESHB,FCOL_MESHW;
//////	//
//////	FCOL_MESH = F_COLOR( GetRValue( PM.Col_Mesh ),GetGValue( PM.Col_Mesh ),GetBValue( PM.Col_Mesh ));
//////	FCOL_MESHR = F_COLOR( GetRValue( PM.Col_MeshR ),GetGValue( PM.Col_MeshR ),GetBValue( PM.Col_MeshR ));
//////	FCOL_MESHB = F_COLOR( GetRValue( PM.Col_MeshB ),GetGValue( PM.Col_MeshB ),GetBValue( PM.Col_MeshB ));
//////	FCOL_MESHW = F_COLOR( 250,250,250 );
//////	//
//////	strcpy(Mobj->memo,"Poly");
//////	strcpy(Lobj->memo,"Line");
//////	cnt = 0;
//////	for(int v=0;v<PM.MVnum;v++) {
//////		LAT		= PM.stLAT + v*PM.Size + EPS8;
//////		nlat	= (int)(LAT-TEX_START_LAT);		//  左下原点座標
//////		lat		= 1. - fmod(LAT,1.);					//  (v+1)：左上を0,0とするＵＶ座標のため
//////		if( lat < EPS8 ) lat = 0.;
//////		if( abs(1.-lat ) < EPS8 ) lat = 1.;
//////
//////		plus = 1;
//////		if( fmod((PM.stLNG + EPS8),dT) < EPS5 ) plus = 0;
//////			//
//////		for(int h=0;h<PM.MHnum;h++) {
////////	
//////			LNG		= PM.stLNG + h*PM.Size+ EPS8;
//////			nlng	= (int)(LNG-TEX_START_LNG);
//////			lng = fmod( LNG,1.);
//////			if( lng < EPS8 ) lng = 0.;
//////			if( abs(1.-lng ) < EPS8 ) lng = 1.;
//////			//
//////			double oddv = fmod(LAT,dT),  oddh = fmod(LNG,dT),    oddx;
//////
//////			if(v ==  0 || oddv <= EPS5 ) {
//////				if( h == 0 || oddh <= EPS5 ) {
//////					Texno =nlat*LNG_SPAN + nlng;
//////					//			
//////					if( v != 0 && h == 0 )	 {
//////						if( fmod((PM.stLNG + EPS8),dT) > EPS3 )  cnt++;
//////					}
//////					id = cnt;
//////					no[0] = id + Hcnt +plus;				
//////					uv[0].x = lng;	
//////					if( lat-dT < 0.)  uv[0].y =0.;
//////					else               uv[0].y = lat-dT;
//////					no[1] = id;								
//////					uv[1].x = lng;			uv[1].y = lat;
//////					//
//////					oddx  = lng+dT;
//////					if( h == 0 ) {
//////						int n = (int)(oddx/dT);
//////						oddx = n*dT;
//////					}
//////					no[2] = id + 1;							
//////					uv[2].x = oddx;		uv[2].y = lat;
//////					no[3] = no[0] + 1;						
//////					uv[3].x = oddx;	
//////					if( lat-dT < 0.)  uv[3].y =0.;
//////					else               uv[3].y = lat-dT;
//////			
//////					pno = GLobj_Add_Poly(Mobj,4,no,GL_TRIANGLE_FAN,Texno,uv);
//////					cnt++;
//////				}
//////			}
//////			//
//////			id = v*(PM.MHnum+1) + h;	// ポイント用ＩＤ
//////			NO[0] = id + PM.MHnum + 1;	
//////			NO[1] = id;								
//////			NO[2] = id + 1;							
//////			NO[3] = NO[0] + 1;	
//////
//////			id = v*PM.MHnum + h;		// データ用ＩＤ
//////			//
//////			check = 0;
//////			if( MESH[v][h].flag == FLAG_ON ) {
//////				//if (MESH[v][h].ID_BREAK != SUNDEF || MESH[v][h].ID_DAM_REC != SUNDEF || MESH[v][h].ID_DISC != SUNDEF ||
//////				//	MESH[v][h].ID_DIV != SUNDEF || MESH[v][h].ID_EVP != SUNDEF || MESH[v][h].ID_WLEV != SUNDEF) {
//////				//	pno = MESH[id].LinePno = GLobj_Add_Poly(Lobj,4,NO,GL_LINE_LOOP,FCOL_MESHB);
//////				//	Lobj->poly[pno].local.set = TRUE;
//////				//	Lobj->poly[pno].imemo[1] = 3;
//////				//	Lobj->poly[pno].local.offy = -BOUND_OFFSET/(PM.scale.y*PM.scale.y);
//////				//}
//////				if (MESH[v][h].ID_RIVPATH != SUNDEF) {
//////					pno = MESH[v][h].LinePno = GLobj_Add_Poly(Lobj, 4, NO, GL_LINE_LOOP, FCOL_MESHW);
//////					Lobj->poly[pno].local.set = TRUE;
//////					Lobj->poly[pno].imemo[1] = 2;
//////					Lobj->poly[pno].local.offy = -RIVPATH_OFFSET/(PM.scale.y*PM.scale.y);
//////				}
//////				else if (MESH[v][h].Rwidth > 0.)	{
//////					pno = MESH[v][h].LinePno = GLobj_Add_Poly(Lobj, 4, NO, GL_LINE_LOOP, FCOL_MESHR);
//////					Lobj->poly[pno].local.set = TRUE;
//////					Lobj->poly[pno].imemo[1] = 1;
//////					Lobj->poly[pno].local.offy = -RIVER_OFFSET/(PM.scale.y*PM.scale.y);
//////				}
//////				else	{
//////					pno = MESH[v][h].LinePno = GLobj_Add_Poly(Lobj, 4, NO, GL_LINE_LOOP, FCOL_MESH);
//////					Lobj->poly[pno].imemo[1] = 0;
//////
//////				}
//////				Lobj->poly[pno].imemo[0] = id;
//////			}
//////		}
//////	}
////	////
////	//// バックイメージの平坦化　パースの時は事前に１００ｍ下げておく.
////	////
////	Gp.Gobj[OBJID_BACK].local.set = TRUE;
////	Gp.Gobj[OBJID_BACK].local.offy = (float)PM.Back_Offset;
////	//
////	PM.Model	= TRUE;	// モデル化がされた
////	PM.B_Color = TRUE;	// バックのイメージはカラー表示である。
////
////	//
////	COMMENT("Delete Unuse Vertices");
////	GLobj_Delete_Vert(&Gp,OBJID_LINE);
////	free( xyz );
////	//
////	if( ! Gp.cam[CVIEW].set ) {
////		GL_Redraw(&Gp, TRUE, TRUE, TRUE, OBJID_BACK, TRUE);
////		GL_Redraw(&Gp,TRUE,TRUE,TRUE,OBJID_LINE,TRUE);
////	}
////	//
////	if( ! PM.B_Mesh ) Gp.Gobj[ OBJID_LINE].display = FALSE;
////	if (!PM.B_Back)  Gp.Gobj[OBJID_BACK].display = FALSE;
////	//
////	STATG( ON_SCREEN ) = TRUE;
////	//
//	return S_OK;
//}
//-----------------------------------------------------------------------------------------------------------
//   CHANGE MODEL
//-----------------------------------------------------------------------------------------------------------
HRESULT CRRIView::Change_Model(PARAMS* pm)
{
	GL_OBJECT*	obj = &Gp.Gobj[ OBJID_BACK ];
	int			Save_Kind = PM.IM_Kind ;
	char			FOLDER[SIZE_05K];
	//
	pm->Change_Display = FALSE;
	pm->Change_Data = pm->Change_Image = pm->Change_Legend = pm->Change_Mesh = pm->Change_Time = FALSE;
	// 要 再表示化　　スケールが違う場合
	if( pm->scale.y != PM.scale.y  ) {
		if( CVIEW == PERS ) {
			if (pm->scale.y != 1.) {
				Gp.Gobj[ OBJID_BACK ].local.set = Gp.Gobj[ OBJID_LINE ].local.set = Gp.Gobj[ OBJID_WL ].local.set	= TRUE;
				Gp.Gobj[ OBJID_BACK ].local.scy = Gp.Gobj[ OBJID_LINE ].local.scy = Gp.Gobj[ OBJID_WL ].local.scy = (float)pm->scale.y;
				//
				for(int p=0;p<Gp.Gobj[ OBJID_LINE ].pnum;p++) {		// POLY 数で回す
					if( Gp.Gobj[ OBJID_LINE ].poly[p].imemo[1] == 3 ) {
						Gp.Gobj[ OBJID_LINE ].poly[p].local.set = TRUE;
						Gp.Gobj[OBJID_LINE].poly[p].local.offy = -BOUND_OFFSET / (pm->scale.y*pm->scale.y);
					}
					else if( Gp.Gobj[ OBJID_LINE ].poly[p].imemo[1] == 2 ) {
						Gp.Gobj[ OBJID_LINE ].poly[p].local.set = TRUE;
						Gp.Gobj[OBJID_LINE].poly[p].local.offy = -RIVPATH_OFFSET / (pm->scale.y*pm->scale.y);
					}
					else if( Gp.Gobj[ OBJID_LINE ].poly[p].imemo[1] == 1 ) {
						Gp.Gobj[ OBJID_LINE ].poly[p].local.set = TRUE;
						Gp.Gobj[OBJID_LINE].poly[p].local.offy = -RIVER_OFFSET / (pm->scale.y*pm->scale.y);
					}
					else	Gp.Gobj[ OBJID_LINE ].poly[p].local.set = FALSE;
				}
			}
			else {
				Gp.Gobj[ OBJID_LINE ].local.set = Gp.Gobj[ OBJID_WL ].local.set	= FALSE;	// Gp.Gobj[ OBJID_BACK ] は常にTRUE　OFFSET
				Gp.Gobj[ OBJID_BACK ].local.scx = Gp.Gobj[ OBJID_LINE ].local.scx = Gp.Gobj[ OBJID_WL ].local.scx =
				Gp.Gobj[ OBJID_BACK ].local.scy = Gp.Gobj[ OBJID_LINE ].local.scy = Gp.Gobj[ OBJID_WL ].local.scy = 
				Gp.Gobj[ OBJID_BACK ].local.scz = Gp.Gobj[ OBJID_LINE ].local.scz = Gp.Gobj[ OBJID_WL ].local.scz = 1.f;
			}
			pm->Change_Display = TRUE;
		}
	}
	//
	// 要 再表示化　　オフセットが違う場合
	//if (pm->Back_Offset != PM.Back_Offset) {
	//	Gp.Gobj[ OBJID_BACK ].local.set = TRUE;
	//	Gp.Gobj[ OBJID_BACK ].local.offy = (float)pm->Back_Offset;
	//	pm->Change_Display = TRUE;
	//}
	// 要 再リスト化　　透明度
	if (pm->Alpha != PM.Alpha) {
		for (int p = 0; p<obj->pnum; p++) obj->poly[p].Alpha = (float)pm->Alpha;
		pm->Change_Image = TRUE;
	}
	// 要 再リスト化　　イメージフォルダーが異なる場合 
	//if (pm->IM_Kind != PM.IM_Kind) {

	//	// Delete Texture Contents
	//	if( Gp.Texnum != 0 ) {
	//		for(int i=0;i<Gp.Texnum;i++ ) {
	//			SAFE_FREE( Gp.Tex[i].image );
	//		}
	//		SAFE_FREE( Gp.Tex );
	//		Gp.Texmem = Gp.Texnum = 0;			// Important
	//	}
	//	// 上下流共通
	//	//strcpy( FOLDER,PM.IM_Folder);

	//	//Param[ ! CUD].IM_Kind = PM->IM_Kind;
	//	//strcpy( Param[ ! CUD].IM_Folder, PM->IM_Folder);

	//	strcpy( PM.IM_Folder,pm->IM_Folder);

	//	// Base Texture       --------------------------
	//	if( FAILED( Set_Tex_Image() ) )  {	// もとに戻す
	//		if( Gp.Texnum != 0 ) {
	//			for(int i=0;i<Gp.Texnum;i++ ) {
	//				SAFE_FREE( Gp.Tex[i].image );
	//			}
	//			SAFE_FREE( Gp.Tex );
	//			Gp.Texmem = Gp.Texnum = 0;			// Important
	//		}
	//		Set_Tex_Image();
	//		pm->Change_Image = FALSE;
	//	}
	//	else   pm->Change_Image = TRUE;
	//}
	// 要 再リスト化　　色を変えて再リスト化
	if (pm->Col_Mesh != PM.Col_Mesh || pm->Col_MeshR != PM.Col_MeshR ||
		pm->Col_MeshB != PM.Col_MeshB) {
		// 
		GL_OBJECT*	obj = &Gp.Gobj[OBJID_LINE];
		COLORREF		cc;
		for(int i=0;i<obj->pnum;i++ ) {
			cc = RGB( (int)(obj->poly[i].col.red*255),(int)(obj->poly[i].col.green*255),
						(int)(obj->poly[i].col.blue*255));
			//
			if (cc == PM.Col_Mesh) obj->poly[i].col = F_COLOR(GetRValue(pm->Col_Mesh),
				GetGValue(pm->Col_Mesh), GetBValue(pm->Col_Mesh));
			if (cc == PM.Col_MeshR) obj->poly[i].col = F_COLOR(GetRValue(pm->Col_MeshR),
				GetGValue(pm->Col_MeshR), GetBValue(pm->Col_MeshR));
			if (cc == PM.Col_MeshB) obj->poly[i].col = F_COLOR(GetRValue(pm->Col_MeshB),
				GetGValue(pm->Col_MeshB), GetBValue(pm->Col_MeshB));
		}
		//
		pm->Change_Mesh = TRUE;
	}
	//
	return S_OK;
}
//-----------------------------------------------------------------------------------------------------------
//   IMAGE MESH
//-----------------------------------------------------------------------------------------------------------
HRESULT CRRIView::Make_Image_Mesh()
{
	GL_OBJECT*			Mobj;
	PARAMS*				Pm = &PM;
	double				X,Y,Z;
	CFile				cf;
	CFileException		fileException;
	XYZCOORD*			xyz = (XYZCOORD*)malloc(DEM_H*DEM_V*sizeof( XYZCOORD ));
	//
	Change_Cursor(&Gp,IDC_WAIT);
	COMMENT("Now ReMake Model....wait");
	//
	if( xyz == NULL )  {	COMMENT("** ERROR ** Cannot Allocate");return E_FAIL;}
	//-----------------------------------------------------------------------------------------------
	// DEMデータの読み込み
	//
	if( CVIEW == PERS ) {
		COMMENT("Read  DEM Data.....");
		::SetCurrentDirectoryA( Gp.m_CurrentPath );
		if( cf.Open(DSET.File_names[ADEM_FILE],CFile::modeRead,&fileException) ) {
			for(int i=0;i<DEM_H*DEM_V;i++) 	cf.Read((void*)&xyz[i],sizeof( XYZCOORD ));
			cf.Close();
		}
		else {	// ０設定
			for(int v=0;v<DEM_V;v++) {
				for(int h=0;h<DEM_H;h++) {
					xyz[v*DEM_H+h].x = TEX_START_LNG + h/120.;
					xyz[v*DEM_H+h].z = TEX_START_LAT + v/120.;
					xyz[v*DEM_H+h].y = 0.;
				}
			}
		}

	}
	//-----------------------------------------------------------------------------------------------
	// モデルデータの初期化
	//
	Mobj = &Gp.Gobj[OBJID_BACK];
	//
	GLobj_Delete(&Gp, OBJID_BACK);
	//-----------------------------------------------------------------------------------------------
	// Vert
	/* データの左下を原点とするモデル
	*/
	COMMENT("Model Data.....VERT");
	double			lat,lng;
	int				id,nlat,nlng;
	int				Step,Hcnt,Vcnt;
	double			dT=1./(double)Image_Sub;
	//
	if( PM.Size == 1./120. ) {		// ３０”
		Step = 120/Image_Sub;
	}
	else {							// ６０”
		if( Image_Sub > 60 ) Image_Sub = 60;
		Step = 60/Image_Sub;
	}
	//おそらく割り切れても誤差のため切り上げられてしまうのではないか？
	if (fmod(PM.MHnum*PM.Size + EPS8, dT) <= EPS5)   Hcnt = (int)((PM.MHnum*PM.Size + EPS8) / dT) + 1; // 切り上げない
	else																	Hcnt = (int)ceil((PM.MHnum*PM.Size + EPS8) / dT) + 1;
	// 
	if (fmod(PM.MVnum*PM.Size + EPS8, dT) <= EPS5)   Vcnt = (int)((PM.MVnum*PM.Size + EPS8) / dT) + 1; // 切り上げない
	else																	Vcnt = (int)ceil((PM.MVnum*PM.Size + EPS8) / dT) + 1;
	//
	GLobj_Init(Mobj, Hcnt*Vcnt,Hcnt*Vcnt);	// 
	//
	for(int v=0;v<=PM.MVnum;v++) {
		lat		= PM.stLAT + v*PM.Size;
		nlat	= (int)((lat - DEM_LAT)/(DEM_SIZE));
		for(int h=0;h<=PM.MHnum;h++) {
			lng		= PM.stLNG + h*PM.Size;
			nlng	= (int)((lng - DEM_LNG)/(DEM_SIZE));	
			X	= h*PM.Size*ONE_DEGREE_DISTANCE;
			Z	= (v*PM.Size*ONE_DEGREE_DISTANCE);			// 左手系座標はＺ方向が下 一時保留
			//
			id	= nlat*DEM_H + nlng;
 			if( CVIEW == PERS ) Y	= -xyz[id].y / Dem_Scale;	// 下から見ているため、負の値が大きいほうが高い
			else					  Y  = 0.;
			//
			double oddv = fabs(fmod(lat,dT)-dT),oddh = fabs(fmod(lng,dT)-dT);

			if( v == 0 || v == PM.MVnum || oddv <= EPS5 ) {
				if( h == 0 || h == PM.MHnum || oddh <= EPS5 )		GLobj_Add_Vert(Mobj,X,Y,Z,FALSE);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------
	// Poly
	COMMENT("Model Data.....POLY");
	int				Texno,no[4],pno,cnt,plus;
	FXYCOORD		uv[4];
	double			LAT,LNG;
	//
	strcpy(Mobj->memo,"Poly");
	cnt = 0;
	//
	for(int v=0;v<PM.MVnum;v++) {
		LAT		= PM.stLAT + v*PM.Size + EPS8;
		nlat	= (int)(LAT-TEX_START_LAT);		//  左下原点座標
		lat		= 1. - fmod(LAT,1.);					//  (v+1)：左上を0,0とするＵＶ座標のため
		if( lat < EPS8 ) lat = 0.;
		if( fabs(1.-lat ) < EPS8 ) lat = 1.;

		for(int h=0;h<PM.MHnum;h++) {
			LNG		= PM.stLNG + h*PM.Size+ EPS8;
			nlng	= (int)(LNG-TEX_START_LNG);
			lng = fmod( LNG,1.);
			if( lng < EPS8 ) lng = 0.;
			if( fabs(1.-lng ) < EPS8 ) lng = 1.;
			//
			double oddv = fmod(LAT,dT),  oddh = fmod(LNG,dT),    oddx;
			//
			if(v ==  0 || oddv <= EPS5 ) {
				if( h == 0 || oddh <= EPS5 ) {
					Texno =nlat*LNG_SPAN + nlng;
					//	
					if( v != 0 && h == 0  ) cnt++;
					id = cnt;
					//
					no[0] = id + Hcnt;				
					uv[0].x = lng;	
					if( lat-dT < 0.)  uv[0].y =0.;
					else               uv[0].y = lat-dT;
					no[1] = id;								
					uv[1].x = lng;			uv[1].y = lat;
					//
					oddx  = lng+dT;
					if( h == 0 ) {
						int n = (int)(oddx/dT);
						oddx = n*dT;
					}
					no[2] = id + 1;							
					uv[2].x = oddx;		uv[2].y = lat;
					no[3] = no[0] + 1;						
					uv[3].x = oddx;	
					if( lat-dT < 0.)  uv[3].y =0.;
					else               uv[3].y = lat-dT;
			
					pno = GLobj_Add_Poly(Mobj,4,no,GL_TRIANGLE_FAN,Texno,uv);
					cnt++;
				}
			}
		}
	}
	//
	PM.Model	= TRUE;
	PM.B_Color = TRUE;
	//
	if( ! PM.B_Mesh ) Gp.Gobj[ OBJID_LINE].display = FALSE;
	if (!PM.B_Back)  Gp.Gobj[OBJID_BACK].display = FALSE;
	//
	//
	// バックイメージの平坦化
	//
	if( CVIEW == ORTHO ) {				// 
		Gp.Gobj[OBJID_BACK].local.set = TRUE;
		Gp.Gobj[OBJID_BACK].local.offy = (float)PM.Back_Offset;
	}
	else {
		if( PM.scale.y != 1.  ) {
			Mobj->local.set = TRUE;
			Mobj->local.scy = (float)PM.scale.y;
		}
	}
	free( xyz );
	Change_Cursor(&Gp,IDC_ARROW);
	//
	return S_OK;
}
//-----------------------------------------------------------------------------------------------------------
//   REDRAW MESH LINE
//-----------------------------------------------------------------------------------------------------------
HRESULT CRRIView::Change_Color_MeshLine()
{
	GL_OBJECT*			Lobj = &Gp.Gobj[OBJID_LINE];
	PARAMS*				Pm = &PM;
	int					Sid,id,pno;
	FCOLOR				Fcol;
	FCOLOR				FCOL_MESH,FCOL_MESHR,FCOL_MESHB,FCOL_MESHW;

	//
	Change_Cursor( &Gp,IDC_WAIT);
	//
	FCOL_MESH = F_COLOR( GetRValue( PM.Col_Mesh ),GetGValue( PM.Col_Mesh ),GetBValue( PM.Col_Mesh ));
	FCOL_MESHR = F_COLOR( GetRValue( PM.Col_MeshR ),GetGValue( PM.Col_MeshR ),GetBValue( PM.Col_MeshR ));
	FCOL_MESHB = F_COLOR( GetRValue( PM.Col_MeshB ),GetGValue( PM.Col_MeshB ),GetBValue( PM.Col_MeshB ));
	FCOL_MESHW = F_COLOR( 250,250,250 );
	//-----------------------------------------------------------------------------------------------
	// Poly
	for(int v=0;v<PM.MVnum;v++) {
		for(int h=0;h<PM.MHnum;h++) {
			if ( ! MESH[v][h].flag )  continue;
			//
			pno = MESH[v][h].LinePno;
			//
			//if( MESH[v][h].ID_BREAK != SUNDEF || MESH[v][h].ID_DAM_REC != SUNDEF || MESH[v][h].ID_DISC != SUNDEF ||
			//	MESH[v][h].ID_DIV != SUNDEF || MESH[v][h].ID_EVP != SUNDEF || MESH[v][h].ID_WLEV != SUNDEF ) {
			//	Lobj->poly[pno].col = FCOL_MESHB;
			//	Lobj->poly[pno].local.set = TRUE;
			//	Lobj->poly[pno].local.offy = -(RIVER_OFFSET+20.);
			//}
			if( MESH[v][h].ID_RIVPATH != SUNDEF ) {
				Lobj->poly[pno].col = FCOL_MESHW;
				Lobj->poly[pno].local.set = TRUE;
				Lobj->poly[pno].local.offy = -(RIVER_OFFSET+10.);
			}
			else if( MESH[v][h].Rwidth > 0. )	{
				Lobj->poly[pno].col = FCOL_MESHR;
				Lobj->poly[pno].local.set = TRUE;
				Lobj->poly[pno].local.offy = -RIVER_OFFSET;
			}
			else	{
				Lobj->poly[pno].col = FCOL_MESH;
				Lobj->poly[pno].local.set = FALSE;
				Lobj->poly[pno].local.offy = 0.f;
			}
		}
	}
	//
	if( CSHAPE != 0 ) {
		//Sid = CSHAPE -  1;
		//Fcol = F_COLOR(GetRValue( PM.SBOX[Sid].Col ),GetGValue( PM.SBOX[Sid].Col ),GetBValue( PM.SBOX[Sid].Col ));
		//for(int i=0;i<PM.SBOX[Sid].Mnum;i++) {
		//	id = PM.SBOX[Sid].MID[i];
		//	if( MESH[v][h].flag == FLAG_OFF )  continue;
		//	//
		//	pno = MESH[v][h].LinePno;
		//	//
		//	Lobj->poly[pno].col = Fcol;
		//	Lobj->poly[pno].local.set = TRUE;
		//	Lobj->poly[pno].local.offy = -(RIVER_OFFSET+20.);
		//}
	}
	//
	GL_Redraw(&Gp,FALSE,FALSE,TRUE,OBJID_LINE,TRUE);
	//
	STATG( ON_SCREEN ) = TRUE;
	return S_OK;
}
//___________________________________________________________________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    情報表示
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-----------------------------------------------------------------------------------------------------------
//  Display Information
//-----------------------------------------------------------------------------------------------------------
void CRRIView::Disp_Info()
{
	char		name[128];
	CWnd*	cw;
	CWnd*	cl;
	int		Ar1, Ar2;
	///
	// OUT BUTTON ----------------------------------------------------------

	for (int i = 0; i < OUT_Num; i++) {		// 出力結果選択
		cw = GetDlgItem(OUT_ID[i]);

		/*if (i == 3) {
			Static_Format(cw, OUT_MENU_NAME[i], DT_CENTER, COLOR_BACK_INVALID, COLOR_BACK_INVALID);
			cw->EnableWindow(FALSE);
			continue;
		}*/

		if (CVIEW == ORTHO) {
			if (!PM.B_Arrow) {
				if (DSET.out_use[i] && DSET.out_exist[i]) {
					if (COUT != i) {
						Static_Format((CWnd*)cw, OUT_MENU_NAME[i], DT_CENTER, COLOR_TAB_TEXT, COLOR_BACK_VALID);
					}
					else   Static_Format((CWnd*)cw, OUT_MENU_NAME[i], DT_CENTER, COL_BLACK, COL_YELLOW);
					cw->EnableWindow(TRUE);
				}
				else {
					Static_Format(cw, OUT_MENU_NAME[i], DT_CENTER, COLOR_BACK_INVALID, COLOR_BACK_INVALID);
					cw->EnableWindow(FALSE);
				}
			}
			else {
				if (DSET.out_use[i] && DSET.out_exist[i]) {
					if (i == OUT_QU || i == OUT_QV){
						if (COUT == OUT_QU || COUT == OUT_QV) {
							Static_Format((CWnd*)cw, OUT_MENU_NAME[i], DT_CENTER, COL_BLACK, COL_YELLOW);
							cw->EnableWindow(TRUE);
						}
						else Static_Format((CWnd*)cw, OUT_MENU_NAME[i], DT_CENTER, COLOR_TAB_TEXT, COLOR_BACK_VALID);
					}
					else if (i == OUT_GU || i == OUT_GV){
						if (COUT == OUT_GU || COUT == OUT_GV) {
							Static_Format((CWnd*)cw, OUT_MENU_NAME[i], DT_CENTER, COL_BLACK, COL_YELLOW);
							cw->EnableWindow(TRUE);
						}
						else Static_Format((CWnd*)cw, OUT_MENU_NAME[i], DT_CENTER, COLOR_TAB_TEXT, COLOR_BACK_VALID);
					}
					else  {
						if (COUT != i) {
							Static_Format((CWnd*)cw, OUT_MENU_NAME[i], DT_CENTER, COLOR_TAB_TEXT, COLOR_BACK_VALID);
						}
						else   Static_Format((CWnd*)cw, OUT_MENU_NAME[i], DT_CENTER, COL_BLACK, COL_YELLOW);
						cw->EnableWindow(TRUE);
					}
				}
			}
		}
		else {
			if (DSET.out_use[i] && DSET.out_exist[i]) {
				if (COUT != i) {
					Static_Format((CWnd*)cw, OUT_MENU_NAME[i], DT_CENTER, COLOR_TAB_TEXT, COLOR_BACK_VALID);
				}
				else   Static_Format((CWnd*)cw, OUT_MENU_NAME[i], DT_CENTER, COL_BLACK, COL_YELLOW);
				cw->EnableWindow(TRUE);
			}
			else {
				Static_Format(cw, OUT_MENU_NAME[i], DT_CENTER, COLOR_BACK_INVALID, COLOR_BACK_INVALID);
				cw->EnableWindow(FALSE);
			}
		}
		cw = GetDlgItem(IDC_V_CHECK_ARROW);
//		cl = GetDlgItem(IDC_V_LABEL_UV);
		if (DSET.out_exist[4] && DSET.out_exist[5]) {
			cw->EnableWindow(TRUE); //cl->EnableWindow(TRUE);
		}
		else   cw->EnableWindow(FALSE);
	}
	///
	// RIVPATH BUTTON ----------------------------------------------------------

	if (DSET.out_exist[1]) {		/// 水位データが存在していれば
		c_RivPath.EnableWindow( TRUE );
		if( PM.Set_Line ) {
			c_RivProfile.EnableWindow( TRUE );
			c_RivPath.SetWindowTextA("Delete Path");
		}
		else  {
			c_RivProfile.EnableWindow(FALSE);
			c_RivPath.SetWindowTextA("Set River Path");
		}
	}
	else  {
		c_RivPath.EnableWindow(FALSE);
		c_RivProfile.EnableWindow(FALSE);
	}
	///
	// UV Vector BUTTON ----------------------------------------------------------

	CButton* cb =(CButton*) GetDlgItem( IDC_V_CHECK_ARROW );	// 流速のベクター表示
//	cl = GetDlgItem( IDC_V_LABEL_UV );
	//
	if (DSET.out_exist[OUT_QU] && DSET.out_exist[OUT_QV]) {
		cb->EnableWindow( TRUE );
		if( PM.B_Arrow ) cb->SetCheck( 1 );
		else            cb->SetCheck( 0 );
	}
	else if (DSET.out_exist[OUT_GU] && DSET.out_exist[OUT_GV]) {
		cb->EnableWindow(TRUE);
		if (PM.B_Arrow) cb->SetCheck(1);
		else            cb->SetCheck(0);
	}
	else      {
		cb->EnableWindow( FALSE );
	}
	//
	UDF;
	return;
}
///-----------------------------------------------------------------------------------------------------------
//   Display Controls
///-----------------------------------------------------------------------------------------------------------
void CRRIView::Disp_Controls()
{
	CWnd*	cw;
//_____________________________________________________
///  VIEWER 画面のチェック
/* 
int			V_CHECK_NUM = 4,V_CHECK_ID[4] = {IDC_V_CHECK_FRAME,IDC_V_CHECK_BACK,IDC_V_CHECK_DATE,IDC_V_CHECK_LEGEND};
int			M_CHECK_NUM = 2,M_CHECK_ID[2] = {IDC_M_CHECK_FRAME,IDC_M_CHECK_BACK};
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ チェック
	CButton*	cb;
	for(int i=0;i<V_CHECK_NUM;i++) {
		cb = (CButton*)GetDlgItem( V_CHECK_ID[i] );
		switch( i ) {
			case 0: if( PM.B_Mesh )			cb->SetCheck(1);else  cb->SetCheck(0);break;
			case 1: if( PM.B_Back )			cb->SetCheck(1);else  cb->SetCheck(0);break;
			case 2: if( PM.B_Date )			cb->SetCheck(1);else  cb->SetCheck(0);break;
			case 3: if( PM.B_Legend )			cb->SetCheck(1);else  cb->SetCheck(0);break;
			case 4: if( PM.B_ShowTimeData )	cb->SetCheck(1);else  cb->SetCheck(0);break;
		}
	}

//_____________________________________________________
///  VIEWER 画面のラジオ
	for(int i=0;i<V_RADIO_NUM;i++) {
		cb = (CButton*)GetDlgItem( V_RADIO_ID[i] );
		switch( i ) {
			case 0: if( PM.B_Color )		cb->SetCheck(1);else  cb->SetCheck(0);break;
			case 1: if( PM.B_Color )		cb->SetCheck(0);else  cb->SetCheck(1);break;
		}
	}

///_____________________________________________________
//  VIEWER 画面のカラー
	CRect		rect;
	CBrush		Cbr;
	//
	for(int i=0;i<V_COL_NUM;i++) {
		cw = GetDlgItem( V_COL_ID[i] );
		if( i == 0 )		Fill_Color(cw,PM.Col_Date);
		else if( i == 1 )   Fill_Color(cw,PM.Col_LegendChar);
	}
	UDF;
///_____________________________________________________
//  情報の表示
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ カラー
	Disp_Info();

}
//-----------------------------------------------------------------------------------------------------------
//   Set Display Control
//-----------------------------------------------------------------------------------------------------------
void CRRIView::Set_DispOption()
{
	CButton* 	cv;
	CString		title;
	// バックイメージ表示の変更
	cv = (CButton*)GetDlgItem( IDC_V_CHECK_BACK );
	if( PM.B_Back ) cv->SetCheck( 1 );
	else                    cv->SetCheck( 0 );
	// メッシュライン表示の変更
	cv = (CButton*)GetDlgItem( IDC_V_CHECK_FRAME );
	if( PM.B_Mesh ) cv->SetCheck( 1 );
	else                    cv->SetCheck( 0 );
	// カラー・グレイコントロールの変更  v（カラー）,m（グレイ） の意味が違うことに注意
	//
	cv = (CButton*)GetDlgItem( IDC_V_RADIO_COLOR );
	if( PM.B_Color ) cv->SetCheck( 1 );
	else					 cv->SetCheck( 0 );
	//
	cv = (CButton*)GetDlgItem( IDC_V_RADIO_GRAY );
	if( PM.B_Color ) cv->SetCheck( 0 );
	else					 cv->SetCheck( 1 );
	//
	UDF;
	return;
}
//___________________________________________________________________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    コントロールハンドラ
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-----------------------------------------------------------------------------------------------------------
//   SHOW TIME DATA or not
//-----------------------------------------------------------------------------------------------------------
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  コントロール関数

void CRRIView::OnBnClickedCheckVShowdata()
{
	UDT;
	CButton* 	cb=(CButton*)GetDlgItem( IDC_CHECK_V_SHOWDATA );
	//
	if( cb->GetCheck() ) 	PM.B_ShowTimeData = TRUE;
	else 				PM.B_ShowTimeData = FALSE;
	Redraw_Now();
}
///-------------------------------------------------------------------------
//   Change Display Mesh Frame
///-------------------------------------------------------------------------
void CRRIView::OnBnClickedVCheckFrame()
{
	UDT;
	//
	CButton* 	cb=(CButton*)GetDlgItem( IDC_V_CHECK_FRAME );
	BOOL		Res = FALSE;
	//
	if( cb->GetCheck() ) {Gp.Gobj[ OBJID_LINE ].display = TRUE;Res = TRUE;}
	else                  Gp.Gobj[ OBJID_LINE ].display = FALSE;
	PM.B_Mesh = Res;
	//
	GL_Redraw(&Gp,FALSE,FALSE,FALSE,UNDEF,TRUE);
}
///-------------------------------------------------------------------------
//   Change Display Back Ground Image 
///-------------------------------------------------------------------------
void CRRIView::OnBnClickedVCheckBack()
{
	UDT;
	//
	CButton* 	cb=(CButton*)GetDlgItem( IDC_V_CHECK_BACK );
	BOOL		Res = FALSE;
	//
	if( cb->GetCheck() ) {Gp.Gobj[ OBJID_BACK ].display = TRUE;Res = TRUE;}
	else                  Gp.Gobj[ OBJID_BACK ].display = FALSE;
	PM.B_Back = Res;
	//
	GL_Redraw(&Gp,FALSE,FALSE,FALSE,UNDEF,TRUE);
}

///-------------------------------------------------------------------------
//   Time Code & Legend
///-------------------------------------------------------------------------
void CRRIView::OnBnClickedVCheckDate()
{
	UDT;
	//
	CButton* 	cb=(CButton*)GetDlgItem( IDC_V_CHECK_DATE );
	if( cb->GetCheck() ) {
		PM.B_Date = TRUE;
		Draw_Date();
	}
	else    {
		PM.B_Date = PM.Show_Date = FALSE;
		GLobj_Delete(&Gp, OBJID_TIME);
		GL_Redraw(&Gp,FALSE,FALSE,FALSE,UNDEF,TRUE);
	}
}
void CRRIView::OnBnClickedVCheckLegend()
{
	UDT;
	//
	CButton* 	cb=(CButton*)GetDlgItem( IDC_V_CHECK_LEGEND );
	if( cb->GetCheck() ) {
		PM.Change_Legend = PM.B_Legend = TRUE;
		Draw_Legend();
	}
	else    {
		PM.Show_Legend = PM.Change_Legend = PM.B_Legend = FALSE;
		GLobj_Delete(&Gp, OBJID_LEGEND);
		GL_Redraw(&Gp,FALSE,FALSE,FALSE,UNDEF,TRUE);
	}
}

void CRRIView::OnStnDblclickVColorDate()
{
	CColorDialog   CD;
	if( CD.DoModal() == IDCANCEL ) return;
	//
	PM.Col_Date = CD.GetColor();
	Disp_Controls();		
	//
	if( PM.B_Date )  Draw_Date();
}
void CRRIView::OnStnDblclickVColorLegend()
{
	CColorDialog   CD;
	if( CD.DoModal() == IDCANCEL ) return;
	//
	PM.Col_LegendChar = CD.GetColor();
	PM.Change_Legend = TRUE;	
	Disp_Controls();		
	//
	if( PM.B_Legend )  Draw_Legend();
}

///-------------------------------------------------------------------------
//   DISPLAY OPTIONS CONTROL
///-------------------------------------------------------------------------
void CRRIView::OnStnDblclickVMoredetail()
{
	if( ! PM.Model   ) {ERR_RETURN("** Ignore Operation ** No Model");}		// アニメーションの場合も
	if( STATG(RUN_ANIMATION)  ) {
		ERR_RETURN("** Ignore Operation **  Please Stop Animation");}
	//
	for(int i = DISP_EDITWIN;i<= DISP_MESHPROFILE;i++) {
		if( STATG( i ) )  return;
	}
	//
	OPTIONS		Ops;
	Ops.Boot_Folder = Gp.m_CurrentPath;
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ここでオプションダイアログ

	if( Ops.DoModal() == IDCANCEL ) 	return;

	CString		str;
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//
	Copy_PARAMS(&PM, Ops.pm,FALSE);
	//
	if (Ops.pm.Change_Image)	{
		GL_OBJECT*	obj = &Gp.Gobj[OBJID_BACK];
		for (int i = 0; i < obj->pnum; i++) obj->poly[i].Alpha = PM.Alpha;
		GL_Redraw(&Gp, FALSE, FALSE, TRUE, OBJID_BACK, TRUE);
	}
	if (PM.B_Mesh)		Set_WL_Model(); 
	if (PM.B_MeshLine)		Make_Mesh_Line();
	//
	if( PM.B_Legend ) 		Draw_Legend();
	if (PM.B_Date) {
		Draw_Date();
		Draw_Location();
	}

	GL_Redraw(&Gp,FALSE,FALSE,TRUE,ALL_OBJECTS,TRUE );
	//
	Disp_Controls();
//	
	return;
}

///-------------------------------------------------------------------------
//   CHANGE CURRENT OUT DATA
///-------------------------------------------------------------------------
void CRRIView::OnStnClickedVGis0()
{
	IF_STATG( RUN_ANIMATION ) return;
	IF_STATG(DISP_EDITWIN) return;
	//
	PM.B_Date = Bool_acent_Date;
	if (!DSET.out_exist[0])return;
	COUT = 0; 
	Anim_Set(TRUE);
	PM.Acnt = 0; m_cRunTime.SetPos(0);
	//
	Disp_Anim_Time(); Disp_Info();
	Redraw_Now();

	Check_Location_Set(FALSE);
}
void CRRIView::OnStnClickedVGis1() 
{
	IF_STATG( RUN_ANIMATION ) return;
	IF_STATG(DISP_EDITWIN) return;
	//
	PM.B_Date = Bool_acent_Date;
	PM.LocationChk = true;

	if (!DSET.out_exist[1])return;
	COUT = 1;
	Anim_Set(TRUE);
	PM.Acnt = 0; m_cRunTime.SetPos(0);
	//
	Disp_Anim_Time(); Disp_Info();
	Redraw_Now();

	Check_Location_Set(TRUE);

}
void CRRIView::OnStnClickedVGis2() 
{
	IF_STATG( RUN_ANIMATION ) return;
	IF_STATG(DISP_EDITWIN) return;
	//
	PM.B_Date = Bool_acent_Date;
	if (!DSET.out_exist[2])return;
	COUT = 2;  
	Anim_Set(TRUE);
	PM.Acnt = 0; m_cRunTime.SetPos(0);
	//
	Disp_Anim_Time(); Disp_Info();
	Redraw_Now();

	Check_Location_Set(FALSE);

}
void CRRIView::OnStnClickedVGis3() 
{
	IF_STATG( RUN_ANIMATION ) return;
	IF_STATG(DISP_EDITWIN) return;
	//
	PM.B_Date = Bool_acent_Date;
	PM.LocationChk = true;
	if (!DSET.out_exist[3])return;
	COUT = 3;  
	Anim_Set(TRUE);
	PM.Acnt = 0; m_cRunTime.SetPos(0);
	//
	Disp_Anim_Time(); Disp_Info();
	Redraw_Now();

	Check_Location_Set(TRUE);

}
void CRRIView::OnStnClickedVGis4() 
{
	IF_STATG( RUN_ANIMATION ) return;
	IF_STATG(DISP_EDITWIN) return;
	//
	PM.B_Date = Bool_acent_Date;
	if (!DSET.out_exist[4])return;
	COUT = 4;  
	Anim_Set(TRUE);
	PM.Acnt = 0; m_cRunTime.SetPos(0);
	//
	Disp_Anim_Time(); Disp_Info();
	Redraw_Now();

	Check_Location_Set(FALSE);

}
void CRRIView::OnStnClickedVGis5()
{
	IF_STATG( RUN_ANIMATION ) return;
	IF_STATG(DISP_EDITWIN) return;
	//
	PM.B_Date = Bool_acent_Date;
	if (!DSET.out_exist[5])return;
	COUT = 5;  
	Anim_Set(TRUE);
	PM.Acnt = 0; m_cRunTime.SetPos(0);
	//
	Disp_Anim_Time(); Disp_Info();
	Redraw_Now();

	Check_Location_Set(FALSE);

}
void CRRIView::OnStnClickedVGis6()		
{
	IF_STATG(RUN_ANIMATION) return;
	IF_STATG(DISP_EDITWIN) return;
	//
	PM.B_Date = Bool_acent_Date;
	if (!DSET.out_exist[OUT_GU])return;
	COUT = 6;
	Anim_Set(TRUE);
	PM.Acnt = 0; m_cRunTime.SetPos(0);
	//
	Disp_Anim_Time(); Disp_Info();
	Redraw_Now();

	Check_Location_Set(FALSE);

}
void CRRIView::OnStnClickedVGis7()
{
	IF_STATG(RUN_ANIMATION) return;
	IF_STATG(DISP_EDITWIN) return;
	//
	PM.B_Date = Bool_acent_Date;
	if (!DSET.out_exist[7])return;
	COUT = 7;
	Anim_Set(TRUE);
	PM.Acnt = 0; m_cRunTime.SetPos(0);
	//
	Disp_Anim_Time(); Disp_Info();
	Redraw_Now();

	Check_Location_Set(FALSE);

}
void CRRIView::OnStnClickedVGis8()
{
	IF_STATG(RUN_ANIMATION) return;
	IF_STATG(DISP_EDITWIN) return;
	//
	PM.B_Date = Bool_acent_Date;
	if (!DSET.out_exist[8])return;
	COUT = 8;
	Anim_Set(TRUE);
	PM.Acnt = 0; m_cRunTime.SetPos(0);
	//
	Disp_Anim_Time(); Disp_Info();
	Redraw_Now();

	Check_Location_Set(FALSE);

}
void CRRIView::OnStnClickedVGis9()
{
	IF_STATG(RUN_ANIMATION) return;
	//
	Bool_acent_Date = PM.B_Date;
	//
	COUT = 9;
	PM.Acnt = 0; 
	Anim_Set(FALSE);
	//
	CStatic*	cs1 = (CStatic*)GetDlgItem(IDC_V_INFO_DATE);
	CStatic*	cs2 = (CStatic*)GetDlgItem(IDC_V_INFO_TIME);
	cs1->SetWindowTextA("Total");
	cs2->SetWindowTextA(_T(""));
	PM.B_Date = FALSE;
	Redraw_Now();

	Check_Location_Set(FALSE);

}
void CRRIView::Anim_Set(BOOL SS)
{
	CWnd*		cw;
	for (int i = 0; i < ANIM_SET_NUM; i++) {
		cw = GetDlgItem(ANIM_SET_ID[i]);
		cw->EnableWindow(SS);
	}
}

void CRRIView::Check_Location_Set(BOOL SS)
{
	CWnd*		cw;
	cw = GetDlgItem(IDC_V_CHECK_LOCATION);
	cw->EnableWindow(SS);
	((CButton*)cw)->SetCheck(true);
}

///-------------------------------------------------------------------------
//  書き出し　150219　追加

void CRRIView::OnBnClickedButtonOutmax()
{
	CString		com,fname;
	fname.Format("%s\\hsmax.txt", DSET.Proj_Folder);
	com.Format("Is save name [ %s ] ?", fname);
	if (MessageBox(com, "file name", MB_YESNO | MB_ICONQUESTION) == IDNO) {
		if (!Button_File(FALSE, &fname, m_RecentPath, "Save hsmax", "txt")) return;
	}
	//
	FILE*	fp = fopen(fname, "w");
	if (fp == NULL) {	RETURN("** Error ** Cannot open save file");	}
	//
	fprintf(fp, "ncols	 %d \n", PM.MHnum);
	fprintf(fp, "nrows	 %d \n", PM.MVnum);
	fprintf(fp, "xllcorner	 %.10lf\n", PM.stLNG);
	fprintf(fp, "yllcorner	 %.10lf\n", PM.stLAT);
	fprintf(fp, "cellsize	 %.14lf\n", PM.Size);
	fprintf(fp, "NODATA_value  -9999 \n");
	//
	for (int v = PM.MVnum - 1; v >= 0; v--) {		// 
		for (int h = 0; h < PM.MHnum; h++) {

			if (h == 0) {
				if (MESH[v][h].hsmax != DUNDEF) fprintf(fp, "%.2lf", MESH[v][h].hsmax);
				else                          fprintf(fp, "-9999");
			}
			else     {
				if (MESH[v][h].hsmax != DUNDEF) fprintf(fp, "  %.2lf", MESH[v][h].hsmax);
				else                          fprintf(fp, "  -9999");
			}
		}
		CR;
	}
	fclose(fp);
}
///-------------------------------------------------------------------------
//   ANIMATION 
///-------------------------------------------------------------------------

void CRRIView::OnBnClickedVCheckArrow() { 
	IF_STATG( RUN_ANIMATION ) {UDF;return;}
	UDT;PM.B_Arrow = m_Arrow; 
	//
	if( PM.B_Arrow ) {
		m_cRunTime.SetRange(0, DSET.DT_Out, TRUE);
		PM.Acnt = 0; m_cRunTime.SetPos( 0 );
	}
	else {
		m_cRunTime.SetRange(0, DSET.DT_Out, TRUE);
		PM.Acnt = 0; m_cRunTime.SetPos( 0 );
	}
	Disp_Info();
	Redraw_Now();
}
void	CRRIView::OnBnClickedVButtonStart()
{
	if( ! PM.set ) {ERR_RETURN("** Error ** Anim Not Ready.....!");}

	// 表示切替
	CButton*	cb = (CButton*)GetDlgItem( IDC_V_BUTTON_START );
	CWnd*		cw;
	IFNOT_STATG( RUN_ANIMATION ) {
		delete pBM[0];
		pBM[0] = new CBitmap;
		pBM[0]->LoadBitmapA( IDB_BITMAP_STOP );
		cb->SetBitmap( *pBM[0] );

		STATG( RUN_ANIMATION ) = TRUE;
		//
		//-------------------------------------------------  タイマー切替
		KillTimer( RENDER_TIMER );	//*** 注意
		//
		Gp.Gobj[OBJID_WL].display = TRUE;			// 表示ON
		PM.Anim_Time = ANIM_TIME_BASE*(11-PM.Anim_Speed);
		SetTimer(ANIMATION_TIMER,PM.Anim_Time,NULL);
	}
	else {
		delete pBM[0];
		pBM[0] = new CBitmap;
		pBM[0]->LoadBitmapA( IDB_BITMAP_START );
		cb->SetBitmap( *pBM[0] );
		//
		if( PM.Acnt > 0 && PM.Acnt < DSET.DT_Out ) STATG( HALT ) = TRUE;
		STATG( RUN_ANIMATION ) = FALSE;
		// End
		//-------------------------------------------------  タイマー切替
		KillTimer( ANIMATION_TIMER );	//*** 注意
		//
		SetTimer(RENDER_TIMER,100,NULL);
	}
	//
	return;
}
//-----------------------------------------------------------------------------------------------------------
//   RECORD BUTTON
//-----------------------------------------------------------------------------------------------------------
void	CRRIView::OnBnClickedVButtonRec()
{
	CButton*	cb = (CButton*)GetDlgItem( IDC_V_BUTTON_REC );
	IFNOT_STATG( RECORD_MODE ) {
		//
		if( ! PM.B_ShowTimeData ) {
			ERR_RETURN("** Error ** Data isnot displayed.. Use print screen button instead of REC");}
		if (strlen(PM.REC_Folder) == 0) {
			MessageBox("Select animation folder", "Select", MB_OK);
			if (!Button_Dir(&m_InDirName, m_RecentPath,TRUE)) return;
			strcpy(PM.REC_Folder, m_InDirName);
		}
		//
		delete pBM[3];
		pBM[3] = new CBitmap;
		pBM[3]->LoadBitmapA( IDB_BITMAP_REC_STOP );
		cb->SetBitmap( *pBM[3] );
		STATG( RECORD_MODE ) = TRUE;
		//
		::CreateDirectory(PM.REC_Folder,NULL);
		//
		Record_Now();
	}
	else {
		delete pBM[3];
		pBM[3] = new CBitmap;
		pBM[3]->LoadBitmapA( IDB_BITMAP_REC );
		cb->SetBitmap( *pBM[3] );
		STATG( RECORD_MODE ) = FALSE;
	}
	//
	return;
}
void	CRRIView::OnNMCustomdrawSliderAnim(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// 
	UDT;
	//
	if( STATG( RUN_ANIMATION ) ) KillTimer( ANIMATION_TIMER );
	//
	PM.Anim_Speed = m_cSliderAnim.GetPos();
	PM.Anim_Time = ANIM_TIME_BASE*(11-PM.Anim_Speed);
	if( STATG( RUN_ANIMATION ) ) SetTimer(ANIMATION_TIMER,PM.Anim_Time,NULL);
	//
	*pResult = 0;
	::SetFocus( Gp.win.hWnd );
}
///-------------------------------------------------------------------------
//   CHANGE CURRENT OUT DATA
///-------------------------------------------------------------------------
void	CRRIView::Record_Now()
{
	if( STATG( RECORD_MODE ) ) {
		CString		fname;
		if( PM.Add_Info == 0 ) 
			fname.Format("%s\\%s_%4d%02d%02d.bmp",PM.REC_Folder,PM.REC_Head,
				DSET.CT.GetYear(),
				DSET.CT.GetMonth(),
				DSET.CT.GetDay());
		else  fname.Format("%s\\%s_%4d%02d%02d_%02d%02d.bmp",PM.REC_Folder,PM.REC_Head,
				DSET.CT.GetYear(),
				DSET.CT.GetMonth(),
				DSET.CT.GetDay(),
				DSET.CT.GetHour(),
				DSET.CT.GetMinute());
		GL_Save_Window(&Gp,fname);
	}
}
///-------------------------------------------------------------------------
//   Set WL   MAKE WATER LEVEL OBJECT
///-------------------------------------------------------------------------
HRESULT CRRIView::Set_WL_Model()
{
	int				id, Lid, Bid = OBJID_WL;
	int				pno,no[8],NO[4],Lcnt;
	GL_OBJECT*		obj=&Gp.Gobj[ Bid ];

	Lid = Bid -1;										// ++ 高さを参照する。 OBJID_LINE
	GL_OBJECT*		Lobj=&Gp.Gobj[ Lid ];

	double			X,Y,Z,dX,dY,dZ;
	FCOLOR			Fcol=F_COLOR(0,0,0);
	int				N,Time = PM.Acnt;
	COLORREF			Cref;

	FCOLOR	Rcol, Lcol;
	Rcol = CREF_FCOL(PM.Col_MeshR);
	Lcol = F_COLOR(255, 0, 0);

	//
	if( PM.WLmodel ) 			GLobj_Delete( &Gp,Bid );
	//_________________________________________________________________________________________
	/// /////////////////////////////////////////////////////////////////////////////////////////
	/*  
		流速矢印	PERS はない。ベクトルの大きさは指数関数に変更 α＊（１－１/E(d)）
		ANIMOTPのArrow Length Magnificationをαとする。
	*/
	if ((COUT >= OUT_QU && COUT <= OUT_GV) && PM.B_Arrow ) {
		if( FAILED( GLobj_Init(obj,PM.Anum*4+100,PM.Anum*2) ) ) return E_FAIL; 
		//
		double	VEC,U,V,ang;
		double	x,z,x0,z0,rot;
		int		Uid, Vid;
		//
		if (COUT == OUT_QU || COUT == OUT_QV) {
			Uid = OUT_QU; Vid = OUT_QV;
			PM.Arrow_mag = 4.;		// 倍率は適当
		}
		else {
			Uid = OUT_GU; Vid = OUT_GV;
			PM.Arrow_mag = 4.;
		}
		ang = PM.Arrow_Ang*PI/180./2.;
		ang = atan( PM.Arrow_Head*tan( ang )   /  (1.-PM.Arrow_Head) );
		//
		Y = -100.;
		//
		for(int v=0;v<PM.MVnum;v++) {
			for(int h=0;h<PM.MHnum;h++) {
				id = v*PM.MHnum + h;
				if (!MESH[v][h].flag) continue;
				if( MESH[v][h].Rwidth > 0. && !PM.B_IncludeRiver ) continue;
				//
				x0 = h + 0.5;
				z0 = v + 0.5;
				//
				U = MESH[v][h].Hval[Uid][Time];
				V = MESH[v][h].Hval[Vid][Time];
				if( U < EPS3 && V < EPS3 ) continue;
				VEC = sqrt( U*U + V*V );	// 大きさ
				//
				Fcol = Get_GIS_Color(VEC, OUT_VECTOR, &N);
				//
				VEC = sqrt( U*U + V*V );	// 大きさ
				VEC = PM.Arrow_mag*(1.- 1./exp( VEC ));  // 指数関数に変更
				//
				// Line
//				rot = atan2(U,V);
				rot = Atan_All(U,V);
//				if (rot < 0.) rot = 2.*PI + rot;

				x = VEC;  z = 0.;
				X = x*cos( rot ) + z*sin( rot ) + x0;
				Z = -x*sin( rot ) + z*cos( rot )+ z0;
				NO[0] = GLobj_Add_Vert(obj,x0,Y,z0,FALSE);
				NO[1] = GLobj_Add_Vert(obj,X,Y,Z,FALSE);
				//
				// Head
				no[0] = NO[1];
				x = VEC*(1.-PM.Arrow_Head);
				z = x*tan(ang);
				X = x*cos( rot ) + z*sin( rot ) + x0;
				Z = -x*sin( rot ) + z*cos( rot )+ z0;
				no[1] = GLobj_Add_Vert(obj,X,Y,Z,FALSE);
				z = -z;
				X = x*cos( rot ) + z*sin( rot ) + x0;
				Z = -x*sin( rot ) + z*cos( rot )+ z0;
				no[2] = GLobj_Add_Vert(obj,X,Y,Z,FALSE);
				//
				GLobj_Add_Poly(obj,2,NO,GL_LINE_STRIP,1.5f,Fcol);
				GLobj_Add_Poly(obj,3,no,GL_TRIANGLE_STRIP,Fcol);
			}
		}
	}
	/// /////////////////////////////////////////////////////////////////////////////////////////
	//  通常表示  PM.B_Arrowはチェックしていてもよい。
	
	else {
		if (CVIEW == ORTHO) {
			if (FAILED(GLobj_Init(obj, PM.Anum * 4 + 100, PM.Anum * 2))) return E_FAIL;
		}
		else {
			if (FAILED(GLobj_Init(obj, PM.Anum * 8 + 100, PM.Anum))) return E_FAIL;
		}
		//
		Change_Cursor(&Gp, IDC_WAIT);
		//
		dX = dZ = 1.;
		//
		if (COUT != OUT_STORAGE) {
			Lcnt = 0;
			for (int v = 0; v < PM.MVnum; v++) {
				for (int h = 0; h < PM.MHnum; h++) {
					///
					// 浸水深の場合
					if (!MESH[v][h].flag)  continue;

					if (PM.OUTCOL[COUT].Equal && MESH[v][h].Hval[COUT][PM.Acnt] < PM.OUTCOL[COUT].val[0])  continue;
					if (!PM.OUTCOL[COUT].Equal && MESH[v][h].Hval[COUT][PM.Acnt] <= PM.OUTCOL[COUT].val[0])  continue;
					//------------------------------------------------------------------------------------------------
					// 下から見ているため、負の値が大きいほうが高い
					//
					if (CVIEW == PERS)      Y = Lobj->vert[Lobj->poly[Lcnt].no[0]].y - WL_OFFSET;
					else					  Y = -DATA_OFFSET;
					//
					X = h;
					Z = v;
					dY = -(MESH[v][h].Hval[COUT][PM.Acnt] * PM.WLmag);
					//
					no[0] = GLobj_Add_Vert(obj, X, Y, Z, FALSE);
					no[1] = GLobj_Add_Vert(obj, X + dX, Y, Z, FALSE);
					no[2] = GLobj_Add_Vert(obj, X + dX, Y, Z + dZ, FALSE);
					no[3] = GLobj_Add_Vert(obj, X, Y, Z + dZ, FALSE);
					NO[0] = no[4] = GLobj_Add_Vert(obj, X, Y + dY, Z, FALSE);
					NO[1] = no[5] = GLobj_Add_Vert(obj, X + dX, Y + dY, Z, FALSE);
					NO[2] = no[6] = GLobj_Add_Vert(obj, X + dX, Y + dY, Z + dZ, FALSE);
					NO[3] = no[7] = GLobj_Add_Vert(obj, X, Y + dY, Z + dZ, FALSE);
					//
					Fcol = Get_GIS_Color(MESH[v][h].Hval[COUT][PM.Acnt], COUT, &N);
					if (CVIEW == ORTHO) {
						pno = GLobj_Add_Poly(obj, 4, no, GL_POLYGON, Fcol);
						obj->poly[pno].both = TRUE;
						if (PM.B_WLframe) {
							Fcol = F_COLOR(GetRValue(PM.Col_WLframe), GetGValue(PM.Col_WLframe), GetBValue(PM.Col_WLframe));
							GLobj_Add_Poly(obj, 4, NO, GL_LINE_LOOP, Fcol);
						}
					}
					else              GLobj_Add_Poly(obj, 8, no, GL_BOX, Fcol);
				}
			}


		}
		///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//  hs max 150219 新設
		else {
			for (int v = 0; v < PM.MVnum; v++) {
				for (int h = 0; h < PM.MHnum; h++) {
					///
					if (!MESH[v][h].flag)  continue;

					if (PM.OUTCOL[COUT].Equal && MESH[v][h].hsmax < PM.OUTCOL[COUT].val[0])  continue;
					if (!PM.OUTCOL[COUT].Equal && MESH[v][h].hsmax <= PM.OUTCOL[COUT].val[0])  continue;
					//------------------------------------------------------------------------------------------------
					// 下から見ているため、負の値が大きいほうが高い
					//
					Y = -DATA_OFFSET;
					//
					X = h;
					Z = v;
					dY = -(MESH[v][h].hsmax * PM.WLmag);
					//
					no[0] = GLobj_Add_Vert(obj, X, Y, Z, FALSE);
					no[1] = GLobj_Add_Vert(obj, X + dX, Y, Z, FALSE);
					no[2] = GLobj_Add_Vert(obj, X + dX, Y, Z + dZ, FALSE);
					no[3] = GLobj_Add_Vert(obj, X, Y, Z + dZ, FALSE);
					NO[0] = no[4] = GLobj_Add_Vert(obj, X, Y + dY, Z, FALSE);
					NO[1] = no[5] = GLobj_Add_Vert(obj, X + dX, Y + dY, Z, FALSE);
					NO[2] = no[6] = GLobj_Add_Vert(obj, X + dX, Y + dY, Z + dZ, FALSE);
					NO[3] = no[7] = GLobj_Add_Vert(obj, X, Y + dY, Z + dZ, FALSE);
					//
					Fcol = Get_GIS_Color(MESH[v][h].hsmax, COUT, &N);
					if (CVIEW == ORTHO) {
						pno = GLobj_Add_Poly(obj, 4, no, GL_POLYGON, Fcol);
						obj->poly[pno].both = TRUE;
						if (PM.B_WLframe) {
							Fcol = F_COLOR(GetRValue(PM.Col_WLframe), GetGValue(PM.Col_WLframe), GetBValue(PM.Col_WLframe));
							GLobj_Add_Poly(obj, 4, NO, GL_LINE_LOOP, F_COLOR(255, 0, 0));
						}
					}
					else              GLobj_Add_Poly(obj, 8, no, GL_BOX, Fcol);
				}
			}
		}

		// Display Location がONの場合
		if (PM.LocationChk && (COUT == 3 || COUT == 1)) {
			for (int locno = 0; locno < LOCATION_NUM; locno++) {

				if (LC.mx[locno] == 0) continue;

				if (LC.mz[locno] == 0) continue;

				id = LC.mz[locno] * (PM.MHnum + 1) + LC.mx[locno];

				X = LC.mx[locno]-1;
				Y = -DATA_OFFSET;
				Z = PM.MVnum - LC.mz[locno];
				dY = 1;

				//
				no[0] = GLobj_Add_Vert(obj, X, Y, Z, FALSE);
				no[1] = GLobj_Add_Vert(obj, X + dX, Y, Z, FALSE);
				no[2] = GLobj_Add_Vert(obj, X + dX, Y, Z + dZ, FALSE);
				no[3] = GLobj_Add_Vert(obj, X, Y, Z + dZ, FALSE);
				NO[0] = GLobj_Add_Vert(obj, X, Y + dY, Z, FALSE);
				NO[1] = GLobj_Add_Vert(obj, X + dX, Y + dY, Z, FALSE);
				NO[2] = GLobj_Add_Vert(obj, X + dX, Y + dY, Z + dZ, FALSE);
				NO[3] = GLobj_Add_Vert(obj, X, Y + dY, Z + dZ, FALSE);
				//
				// Locationのメッシュ番号の際に、ラインを赤にする
				pno = GLobj_Add_Poly(obj, 4, no, GL_POLYGON, Lcol);
				obj->poly[pno].both = TRUE;
				GLobj_Add_Poly(obj, 4, NO, GL_LINE_LOOP, Lcol);
			}

		}

		obj->Frame = PM.B_WLframe;
		Gp.Frame_fcol = F_COLOR(GetRValue(PM.Col_WLframe), GetGValue(PM.Col_WLframe),
			GetBValue(PM.Col_WLframe));
		//
		if (PM.scale.y != 1.) {
			obj->local.scy = (float)PM.scale.y;
			obj->local.set = TRUE;
		}
	}

	//
	PM.WLmodel = TRUE;
	//
	GL_Redraw(&Gp,FALSE,FALSE,TRUE,Bid,TRUE);
	//
	return S_OK;
}
//-----------------------------------------------------------------------------------------------------------
//  GET COLOR  for GIS
//---------------------------------------------------------------------
FCOLOR CRRIView::Get_GIS_Color(double	val,int	 Dno,int* N)
{
	int ID = Dno;
	if (COUT == OUT_QV || COUT == OUT_QU) {
		if (Dno == OUT_VECTOR || Dno == OUT_QV) ID = OUT_QU;
	}
	if (COUT == OUT_GU || COUT == OUT_GV) {
		if (Dno == OUT_VECTOR || Dno == OUT_GV) ID = OUT_GU;
	}
	//
	if (PM.OUTCOL[ID].Equal) 	if (val < PM.OUTCOL[ID].val[0]) return PM.OUTCOL[ID].Fcol[0];
	else  if (val <= PM.OUTCOL[ID].val[0]) return PM.OUTCOL[ID].Fcol[0];

	if (PM.OUTCOL[ID].Equal) 	{
		if (val > PM.OUTCOL[ID].val[PM.OUTCOL[ID].num - 1]) return PM.OUTCOL[ID].Fcol[PM.OUTCOL[ID].num - 1];
	}
	else  {
		if (val >= PM.OUTCOL[ID].val[PM.OUTCOL[ID].num - 1]) return PM.OUTCOL[ID].Fcol[PM.OUTCOL[ID].num - 1];
	}
	//
	for (int i = 1; i < PM.OUTCOL[ID].num; i++) {
		if (PM.OUTCOL[ID].Equal) {
			if (val < PM.OUTCOL[ID].val[i]) return PM.OUTCOL[ID].Fcol[i - 1];
		}
		else  {
			if (val <= PM.OUTCOL[ID].val[i]) return PM.OUTCOL[ID].Fcol[i - 1];
		}
	}
	return F_COLOR(0,0,0);
}
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  スクリーンツール機能

void CRRIView::Make_Screen_Tool(BOOL  Tex) // Ｔｅｘは初期化
{
	GL_OBJECT*	obj = &Gp.Gobj[OBJID_TOOL];
	GLobj_Delete(&Gp, OBJID_TOOL);
	FXYCOORD		uv[4];
	//  
	/// Init TEXTURE
	if (Tex) {
		GL_Tex_Init(&Gp, 105);
		GL_Set_Bitmap(0, &Gp, SCHnum, SCVnum, 4, 1, Tool_Image, FALSE);
	}
	//
	GLobj_Init(obj, 10, 5);
	int			no[4];
	//						反時計まわり
	no[0] = GLobj_Add_Vert(obj, NOWX, NOWY,DUNDEF, FALSE);
	uv[0].x = 0.; uv[0].y = 1.;
	no[3] = GLobj_Add_Vert(obj, NOWX, NOWY + SCVnum,DUNDEF,  FALSE);
	uv[3].x = 0.; uv[3].y = 0.;
	no[2] = GLobj_Add_Vert(obj, NOWX + SCHnum, NOWY + SCVnum,DUNDEF, FALSE);
	uv[2].x = 1.; uv[2].y = 0.;
	no[1] = GLobj_Add_Vert(obj, NOWX + SCHnum,NOWY,DUNDEF,   FALSE);
	uv[1].x = 1.; uv[1].y = 1.;
	//
	GLobj_Add_Poly(obj, DIM_2D, 4, no, GL_POLYGON, 0, uv);
	GL_Redraw(&Gp, FALSE, FALSE, TRUE, OBJID_TOOL, TRUE);
	//
	PM.B_ScreenTool = TRUE;
}

int	 CRRIView::Decide_Tool(CPoint cp)
{
	double		cx, cy;
	XYCOORD		Area[4];
	//
	cx = (double)(cp.x - Gp.win.Wrect.left);
	cy = (double)(Gp.win.Wrect.bottom - cp.y);
	for (int i = 0; i < 4; i++) {
		Area[i].x = (double)(Tool_Frame[i].x + NOWX);
		Area[i].y = (double)(Tool_Frame[i].y + NOWY);
	}
	if (!Is_InRect(cx, cy, Area, TRUE)) return 0;
	//
	/// Function
	for (int f = 0; f < 6; f++) {
		for (int i = 0; i < 4; i++) {
			Area[i].x = (double)(Tool_Func[f][i].x + NOWX);
			Area[i].y = (double)(Tool_Func[f][i].y + NOWY);
		}
		if (!Is_InRect(cx, cy, Area, TRUE)) continue;
		return f + 1;
	}
	//
	return 0;
}

void	 CRRIView::EX_Tool(int id)
{
	switch (id) {
	case 1:Tool_Scale(FALSE); break;
	case 2:Tool_Move(4); break;
	case 3:Tool_Move(16); break;
	case 4:Tool_Move(1); break;
	case 5:Tool_Move(64); break;
	case 6:Tool_Scale(TRUE); break;
	}
}

void  CRRIView::Tool_Scale(BOOL inc)
{
	double	ang;
	double	Hspan, Vspan, CT;
	int		Pixel;
	//
	CT = (Gp.cam[CVIEW].stV + Gp.cam[CVIEW].endV) / 2.;
	Vspan = (Gp.cam[CVIEW].endV - Gp.cam[CVIEW].stV) / 2.;
	if (inc)		Vspan *= (1. - PM.Tool_Rate);
	else			Vspan *= (1. + PM.Tool_Rate);
	//
	Gp.cam[CVIEW].endV = Vspan + CT;
	Gp.cam[CVIEW].stV = CT - Vspan;
	//
	Gp.cam[CVIEW].Dh = fabs((Gp.cam[CVIEW].endV - Gp.cam[CVIEW].stV));
	Gp.cam[CVIEW].Dw = Gp.cam[CVIEW].Dh*Gp.cam[CVIEW].aspect;
	Hspan = Gp.cam[CVIEW].Dw / 2.;
	//
	//
	CT = (Gp.cam[CVIEW].stH + Gp.cam[CVIEW].endH) / 2.;
	Gp.cam[CVIEW].endH = CT + Hspan;
	Gp.cam[CVIEW].stH = CT - Hspan;
	//
	Gp.cam[CVIEW].set = FALSE;
	//
	if (PM.B_MeshLine) {
		Pixel = (int)(Gp.win.cWidth / (Hspan * 2));
		if (Pixel > PM.Mesh_Thresh) Gp.Gobj[OBJID_LINE].display = TRUE;
		else                      Gp.Gobj[OBJID_LINE].display = FALSE;
	}
	//
	GL_Redraw(&Gp, FALSE, TRUE, FALSE, UNDEF, TRUE);

}

void  CRRIView::Tool_Move(int dir)
{
	CRect rect;
	Gp.win.cw->GetWindowRect(&rect);
	//
	double	movex = -PM.Tool_Rate * Gp.cam[CVIEW].Dw;
	double	movey = -PM.Tool_Rate * Gp.cam[CVIEW].Dh;
	//
	if (dir == 1)			{ movey = 0.; }
	else if (dir == 4)	{ movex = 0.; }
	else if (dir == 16)	{ movex *= -1.; movey = 0.; }
	else if (dir == 64)	{ movex = 0.; movey *= -1.; }
	//
	Gp.cam[CVIEW].endH -= movex;  Gp.cam[CVIEW].stH -= movex;
	Gp.cam[CVIEW].endV += movey;  Gp.cam[CVIEW].stV += movey;
	Gp.cam[CVIEW].set = FALSE;
	//
	GL_Redraw(&Gp, FALSE, TRUE, FALSE, UNDEF, TRUE);
}
///-----------------------------------------------------------------------------------------------------------
//   Draw Date Model
///---------------------------------------------------------------------
void	CRRIView::Draw_Date()
{
	int				Bid = OBJID_TIME;
	GL_OBJECT*		obj=&Gp.Gobj[ Bid ];
	int				no[4],len,blen;
	double			X,Y,dX,dY;
	char				str[128],cfont[48];
	double			Hmarge=1.5,Vmarge=0.15;
	FCOLOR			Fcol;
	double			Rate[12]={1.,0.9,0.85,0.85,1.1,1.,1.,0.9,1.1,1.,1.,1.05};
	//
	sprintf(str,"%d / %2d / %2d  %02d : %02d",DSET.CT.GetYear(),
		DSET.CT.GetMonth(), DSET.CT.GetDay(),
		DSET.CT.GetHour(), DSET.CT.GetMinute());
	len = strlen( str )+1;		// +1 Add 15/03/07
	blen = 0;
	for(int i=0;i<len;i++) if( str[i] == ' ' ) blen++;
	//
	GLobj_Delete( &Gp,Bid);
	strcpy(cfont,Fonts[PM.Tfont]);
	GLobj_Add_Text(&Gp,Bid,DIM_2D,str,cfont,PM.Tstyle,PM.Th,PM.Tw, 
		PM.Pos_Time.x,PM.Pos_Time.y,DUNDEF,
		F_COLOR(GetRValue(PM.Col_Date),GetGValue(PM.Col_Date),GetBValue(PM.Col_Date)));
	//
	if( PM.B_DateBack) {
		Fcol = F_COLOR(GetRValue( PM.Col_DateBack ),
			GetGValue( PM.Col_DateBack ),GetBValue( PM.Col_DateBack ));
		X = PM.Pos_Time.x-Hmarge*PM.Tw;
		PM.Size_Time.x = dX = (len*PM.Tw)*Rate[ PM.Tfont ];
		Y = PM.Pos_Time.y-Vmarge*PM.Th;
		PM.Size_Time.y = dY = PM.Th;
		//
		no[0] = GLobj_Add_Vert(obj, X,    Y,	DUNDEF, FALSE);
		no[1] = GLobj_Add_Vert(obj, X+dX, Y,	DUNDEF, FALSE);
		no[2] = GLobj_Add_Vert(obj, X+dX, Y+dY, DUNDEF, FALSE);
		no[3] = GLobj_Add_Vert(obj, X   , Y+dY, DUNDEF, FALSE);
		//
		GLobj_Add_Poly(obj,DIM_2D,4,no,GL_POLYGON,1.,Fcol);
	}
	//
	PM.Show_Date = TRUE;
	GL_Redraw(&Gp,FALSE,FALSE,TRUE,Bid,TRUE);
}
//-----------------------------------------------------------------------------------------------------------
//   Make Legend Model
//---------------------------------------------------------------------
void	CRRIView::Draw_Legend()
{
	int				Bid = OBJID_LEGEND;
	GL_OBJECT*		obj = &Gp.Gobj[Bid];
	//
	GLobj_Delete(&Gp, Bid);
	//
	FCOLOR			Fcol;
	// --------------------------------------------------------------------------------------------
	// 開始時、連続・離散表示の指示、閾値の変更等
	//
	char			str[128], cfont[64];
	double		X = PM.Pos_Legend.x, Y = PM.Pos_Legend.y;
	int			Span = PM.Lsize;
	double		cv, dx, dy, DX, DY, len;
	int			no[4], lno[2];
	CString		strnum;
	char			sunit[32];

	// Change Variable
	int			NUM;
	COLORREF*	COL;
	double*		VAL;
	//
	NUM = PM.OUTCOL[COUT].num;
	COL = PM.OUTCOL[COUT].Rcol;
	VAL = PM.OUTCOL[COUT].val;
	//
	PM.Area_Legend.bottom = (LONG)PM.Pos_Legend.y;
	PM.Area_Legend.left = (LONG)PM.Pos_Legend.x;
	//
	strcpy(sunit, "( m )");
	if (COUT >= OUT_QR && COUT <= OUT_GV) strcpy(sunit, "( m3/s )");
	else if (COUT == OUT_GAMPUT) strcpy(sunit, " ");
	else if (COUT == OUT_STORAGE) strcpy(sunit, "( m )"); // 150219 変更 hsmax
	//
	GLobj_Init(obj, 100, 100);
	DX = DX = DUNDEF;
	//
	strcpy(cfont, Fonts[PM.Lfont]);
	dx = dy = Span;

	//
	//if (PM.Dcol[COUT].Mos) {
	for (int i = 1; i<NUM+1; i++) {
		cv = Y + dy*(i-1);
		if (i == 1) {
			no[0] = GLobj_Add_Vert(obj, X, Y, DUNDEF, TRUE);
			no[1] = GLobj_Add_Vert(obj, X + dx, Y, DUNDEF, TRUE);
			GLobj_Add_Text(&Gp, Bid, DIM_2D, sunit, "Times New Roman", FONT_REGULAR,
				(int)(Span*0.8), (int)(Span*0.3), X + dx / 2, cv - dy*0.8, DUNDEF,
				F_COLOR(GetRValue(PM.Col_LegendChar), GetGValue(PM.Col_LegendChar),
				GetBValue(PM.Col_LegendChar)));
		}
		no[2] = GLobj_Add_Vert(obj, X + dx, cv + dy, DUNDEF, TRUE);
		no[3] = GLobj_Add_Vert(obj, X, cv + dy, DUNDEF, TRUE);
		//
		Fcol = F_COLOR(GetRValue(COL[i-1]), GetGValue(COL[i-1]), GetBValue(COL[i-1]));
		GLobj_Add_Poly(obj, DIM_2D, 4, no, GL_POLYGON, 1., Fcol);
		//
		if (i == 1) {
			if (PM.OUTCOL[COUT].Equal) {
				Set_Double_Str(VAL[i-1], &strnum, 0);
				sprintf(str, " >= %s", strnum);
			}
			else {
				Set_Double_Str(VAL[i], &strnum, 0);
				sprintf(str, " <= %s", strnum);
			}
		}
		else if (i <= NUM - 1) {
			if (PM.OUTCOL[COUT].Equal) {
				Set_Double_Str(VAL[i-1], &strnum, 0);
				sprintf(str, " >= %s", strnum);
			}
			else {
				Set_Double_Str(VAL[i], &strnum, 0);
				sprintf(str, " <= %s", strnum);
			}
		}
		else  {
			if (PM.OUTCOL[COUT].Equal) {
				Set_Double_Str(VAL[i-1], &strnum, 0);
				sprintf(str, " >= %s", strnum);
			}
			else {
				Set_Double_Str(VAL[i-1], &strnum, 0);
				sprintf(str, " > %s", strnum);
			}
		}
		GLobj_Add_Text(&Gp, Bid, DIM_2D, str, cfont, PM.Lstyle,
			(int)(Span*0.8), (int)(Span*0.3), X + dx, cv + Span*0.2, DUNDEF,
			F_COLOR(GetRValue(PM.Col_LegendChar), GetGValue(PM.Col_LegendChar),
			GetBValue(PM.Col_LegendChar)));

		len = (strlen(str) + 1.5)*Span*0.4 + 10;
		if (DX < len) DX = len;
		//
		no[0] = no[3];
		no[1] = no[2];
	}
	//}	
	DY = cv + 1.2*dy;
	//----------------------------------------------------------------背景
	Fcol = F_COLOR(255, 255, 255);
	X -= 10.;
	Y -= Span;
	//
	no[0] = GLobj_Add_Vert(obj, X, Y, DUNDEF, FALSE);
	no[1] = GLobj_Add_Vert(obj, X + DX, Y, DUNDEF, FALSE);
	no[2] = GLobj_Add_Vert(obj, X + DX, DY, DUNDEF, FALSE);
	no[3] = GLobj_Add_Vert(obj, X, DY, DUNDEF, FALSE);
	//
	GLobj_Add_Poly(obj, DIM_2D, 4, no, GL_POLYGON, 1., Fcol);
	//---------------------------------------------------------------------
	PM.Area_Legend.top = (LONG)(cv + Span);
	PM.Area_Legend.right = (LONG)(PM.Pos_Legend.x + 2 * Span);
	//
	PM.Change_Legend = FALSE;
	PM.Show_Legend = PM.B_Legend = TRUE;
	GL_Redraw(&Gp, FALSE, FALSE, TRUE, Bid, TRUE);	
	//
	return;
}

//-----------------------------------------------------------------------------------------------------------
//   Make Location Model
//---------------------------------------------------------------------
void	CRRIView::Draw_Location()
{
	int				Bid = OBJID_LOCATION;
	GL_OBJECT*		obj = &Gp.Gobj[Bid];
	int				no[4], len, blen;
	double			X, Y, dX, dY;
	char				str[128], cfont[48];
	double			Hmarge = 1.5, Vmarge = 0.15;
	FCOLOR			Fcol;
	double			Rate[12] = { 1.,0.9,0.85,0.85,1.1,1.,1.,0.9,1.1,1.,1.,1.05 };
	//
	len = strlen(str) + 1;		// +1 Add 15/03/07
	blen = 0;
	for (int i = 0; i<len; i++) if (str[i] == ' ') blen++;
	//
	GLobj_Delete(&Gp, Bid);

	// 描画位置(左下位置)
	X = PM.Pos_Legend.x;
	Y = PM.Pos_Legend.y + 250;

	// 凡例表示
	/*
	Fcol = F_COLOR(255, 255, 255); // 色指定（赤）
	no[0] = GLobj_Add_Vert(obj, X + 5, Y + 5, DUNDEF, FALSE);
	no[1] = GLobj_Add_Vert(obj, X - 5 + 20, Y + 5, DUNDEF, FALSE);
	no[2] = GLobj_Add_Vert(obj, X - 5 + 20, Y - 5 + 20, DUNDEF, FALSE);
	no[3] = GLobj_Add_Vert(obj, X + 5, Y - 5 + 20, DUNDEF, FALSE);
	GLobj_Add_Poly(obj, DIM_2D, 4, no, GL_POLYGON, 1., Fcol);
	*/

	Fcol = F_COLOR(255, 0, 0); // 色指定（赤）
	no[0] = GLobj_Add_Vert(obj, X, Y, DUNDEF, TRUE);
	no[1] = GLobj_Add_Vert(obj, X + 20, Y, DUNDEF, TRUE);
	no[2] = GLobj_Add_Vert(obj, X + 20, Y + 20, DUNDEF, TRUE);
	no[3] = GLobj_Add_Vert(obj, X , Y + 20, DUNDEF, TRUE);
	//
	GLobj_Add_Poly(obj, DIM_2D, 4, no, GL_POLYGON, 1., Fcol);
	// 凡例表示

	// 凡例テキスト表示
	Fcol = F_COLOR(0, 0, 0);
	sprintf(str, "Location");
	strcpy(cfont, Fonts[PM.Tfont]);
	GLobj_Add_Text(&Gp, Bid, DIM_2D, str, cfont, PM.Tstyle, PM.Th*0.6, PM.Tw*0.6,
		X + 30, Y + 5, DUNDEF, Fcol);

	// 凡例背景作成
	Fcol = F_COLOR(255, 255, 255);
	no[0] = GLobj_Add_Vert(obj, X - 10, Y - 10, DUNDEF, FALSE);
	no[1] = GLobj_Add_Vert(obj, X - 10 + 150, Y - 10, DUNDEF, FALSE);
	no[2] = GLobj_Add_Vert(obj, X - 10 + 150, Y - 10 + 40, DUNDEF, FALSE);
	no[3] = GLobj_Add_Vert(obj, X - 10, Y - 10 + 40, DUNDEF, FALSE);
	//
	GLobj_Add_Poly(obj, DIM_2D, 4, no, GL_POLYGON, 1., Fcol);

	//
	GL_Redraw(&Gp, FALSE, FALSE, TRUE, Bid, TRUE);

}


//-----------------------------------------------------------------------------------------------------------
//  Redraw Current Time
//---------------------------------------------------------------------
void	CRRIView::Redraw_Now()
{
	if( ! PM.Model ) return;
	if( COUT < 0 ) return;
	//
	Change_Cursor( &Gp,IDC_WAIT );
	//
	if (Gp.Gobj[OBJID_WL].set) GLobj_Delete(&Gp, OBJID_WL);			// WL
	if (Gp.Gobj[OBJID_TIME].set) GLobj_Delete(&Gp, OBJID_TIME);		// Time
	if (Gp.Gobj[OBJID_LEGEND].set) GLobj_Delete(&Gp, OBJID_LEGEND);	// Legend
	//
	if( PM.B_ShowTimeData ) {
		if (DSET.Cout_Exist > 0)			Set_WL_Model();

		if( PM.B_Date )			Draw_Date();
		if (PM.B_Legend) {
			Draw_Legend();
			Draw_Location();
		}
	}
	//
	GL_Redraw(&Gp,FALSE,FALSE,TRUE,ALL_OBJECTS,TRUE);
}
///-----------------------------------------------------------------------------------------------------------
//   ANIMATION
///---------------------------------------------------------------------
void	CRRIView::OnBnClickedVButtonRew()
{
	if( ! PM.set ) {ERR_RETURN("** Error ** Simulation File missed....!");}
	IF_STATG( RUN_ANIMATION ) {RETURN("** Error ** Ignore operation ....!");}
	//
	Change_Cursor( &Gp,IDC_WAIT );
	//
	PM.Acnt = 0;
	m_cRunTime.SetPos(0);
	// 
	Disp_Anim_Time();
	//
	Redraw_Now();
	Render_Scene();
	//
	//double		vol,Area;
	//Get_Shape_Volume(&vol,&Area);
	//	
	COMMENT("Ready......");Change_Cursor( &Gp,IDC_ARROW );
}

void	CRRIView::OnBnClickedVButtonFor()
{
	if( ! PM.set ) {ERR_RETURN("** Error ** Simulation File missed....!");}
	IF_STATG( RUN_ANIMATION ) {RETURN("** Error ** Ignore operation ....!");}
	//
	Change_Cursor( &Gp,IDC_WAIT );
	//
	Gp.Gobj[OBJID_WL].display = TRUE;			// 表示ON
	//
	PM.Acnt = DSET.DT_Out;
	m_cRunTime.SetPos(PM.Acnt);
	// 
	Disp_Anim_Time();
	//
	Redraw_Now();
	//
	Render_Scene();
	//
	//double		vol,Area;
	//Get_Shape_Volume(&vol,&Area);
	//	
	COMMENT("Ready......");Change_Cursor( &Gp,IDC_ARROW );
}

void	CRRIView::OnBnClickedVButtonBack()
{
	if( ! PM.set ) {ERR_RETURN("** Error ** Simulation File missed....!");}
	IF_STATG( RUN_ANIMATION ) {RETURN("** Error ** Ignore operation ....!");}
	//
	Change_Cursor( &Gp,IDC_WAIT );
	//
	Gp.Gobj[OBJID_WL].display = TRUE;			// 表示ON
	//
	PM.Acnt--;
	m_cRunTime.SetPos(PM.Acnt);
	//
	Redraw_Now();
	Render_Scene();
	// 
	Disp_Anim_Time();
	//
	//double		vol,Area;
	//Get_Shape_Volume(&vol,&Area);
	//	
	COMMENT("Ready......");Change_Cursor( &Gp,IDC_ARROW );
}

void	CRRIView::OnBnClickedVButtonNext()
{
	if( ! PM.set ) {ERR_RETURN("** Error ** Simulation File missed....!");}
	IF_STATG( RUN_ANIMATION ) {RETURN("** Error ** Ignore operation ....!");}
	//
	Change_Cursor( &Gp,IDC_WAIT );
	//
	Gp.Gobj[OBJID_WL].display = TRUE;			// 表示ON
	//
	PM.Acnt++;
	m_cRunTime.SetPos(PM.Acnt);
	//
	Redraw_Now();
	Render_Scene();
	// 
	Disp_Anim_Time();
	//
	//double		vol,Area;
	//Get_Shape_Volume(&vol,&Area);
	//	
	COMMENT("Ready......");Change_Cursor( &Gp,IDC_ARROW );
}

//void	CRRIView::OnStnDblclickVAnimOption()
//{
//	if( ! PM.Model   ) {ERR_RETURN("** Ignore Operation ** No Model");}		// アニメーションの場合も
//	//
//	if( ! STATG( RUN_ANIMATION ) ) {
//		CAnimOpt		Ops;
//		if( Ops.DoModal() == IDCANCEL ) return;
//		//
//		memcpy(&PM,&Ops.pm,sizeof(PARAMS));
//		//
//		Redraw_Now();
//	}
//	else ERR_RETURN("** Ignore Operation **  Please STOP Animation");
//}

void	CRRIView::Disp_Anim_Time()
{
	CStatic*	cs1 = (CStatic*)GetDlgItem( IDC_V_INFO_DATE );
	CStatic*	cs2 = (CStatic*)GetDlgItem( IDC_V_INFO_TIME );
	CString		str;
	//
	DSET.CT = DSET.ST + CTimeSpan(0l, 0, 0, DSET.DT_Inter*PM.Acnt);
	//
	if( PM.B_Arrow ) {	// Velocity U を使用
		str.Format("%d / %d / %d",DSET.CT.GetYear(),
			DSET.CT.GetMonth(), DSET.CT.GetDay());
		cs1->SetWindowTextA( str );
		str.Format("%d : %02d", DSET.CT.GetHour(), DSET.CT.GetMinute());
		cs2->SetWindowTextA( str ); 
	}
	else {
		str.Format("%d / %d / %d", DSET.CT.GetYear(),
			DSET.CT.GetMonth(), DSET.CT.GetDay());
		cs1->SetWindowTextA( str );
		str.Format("%d : %02d", DSET.CT.GetHour(), DSET.CT.GetMinute());
		cs2->SetWindowTextA( str ); 
	}
	Draw_Date();
	UDF;
}

void	CRRIView::OnNMCustomdrawSliderRuntime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//
	if( PM.Acnt == m_cRunTime.GetPos() ) return;
	if( ! PM.Model ) return;
	if( STATG( RUN_ANIMATION ) ) {ERR_RETURN("** IGNORE ** Please Stop Animation...!");}
	//
	CStatic*	cs1 = (CStatic*)GetDlgItem( IDC_V_INFO_DATE );
	CStatic*	cs2 = (CStatic*)GetDlgItem( IDC_V_INFO_TIME );
	CString		str;
	//
	Change_Cursor( &Gp,IDC_WAIT );
	//
	Gp.Gobj[OBJID_WL].display = TRUE;			// 表示ON
	//
	PM.Acnt = m_cRunTime.GetPos();
	//
	Redraw_Now();
	//
	//double		vol,Area;
	//Get_Shape_Volume(&vol,&Area);
	//	
	COMMENT("Ready......");Change_Cursor( &Gp,IDC_ARROW );
}
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
//
//    河道ライン設定
//
////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
// REWIND

void	CRRIView::OnBnClickedVButtonRivpath()
{
	if( CVIEW == PERS ) {ERR_RETURN("Change View Mode to ORTHOPECTIVE");}
	IF_STATG(DISP_EDITWIN) return;
	//
	//
	if( ! PM.Set_Line ) {
		PM.Lcnt = 0;
		PM.Pro_Ready = PM.Set_Height_Model = FALSE;

		STATG( SELECT_RIV_PATH ) = TRUE;
		if( MessageBox("Select Upstream-end River Mesh","River Path",MB_YESNO|MB_ICONEXCLAMATION ) == IDNO) {
			STATG( SELECT_RIV_PATH ) = FALSE;
			COMMENT( "** Canceled Specity  River Path");
			return;
		}
		COMMENT( "Select 1st River Mesh");
	}
	else {
		int		pno;
		////////SAFE_FREE( PM.Path );
		////////SAFE_FREE( PM.Rdist );
		Free_Matrix( 4,PM.Pval);
		//
		GLobj_Delete(&Gp, OBJID_MARK);
		//
		PM.Pro_Ready = PM.Set_Height_Model = FALSE;
		PM.Lcnt = PM.LineNum = PM.LineMem = 0;
		PM.Set_Line = FALSE;
		//
		STATG( SELECT_RIV_PATH ) = FALSE;
		STATG( DECIDE_RIV_PATH ) = FALSE;
//		Disp_Info();
		GL_Redraw(&Gp, FALSE, FALSE, TRUE, OBJID_MARK, TRUE);
		//
		IF_STATG( DISP_PROFILE ) CPRO->OnCancel();
	}
}

void	CRRIView::OnBnClickedVButtonProfile()
{
	IF_STATG( DISP_BREAKTIME ) return;
	IF_STATG( DISP_MESHPROFILE ) return;
	IF_STATG( DISP_SECTION ) return;
	//
	if( PM.Set_Line && ! STATG( DISP_PROFILE ) ) {
		CPRO = new CProfile;
		//
		CRect	Wrect;
		CTime	Now;
		this->GetWindowRect( &Wrect );
		CPRO->Prect = Wrect ;
		CPRO->WinH = WIN_H[WIN_R_PROF];
		CPRO->WinW = WIN_W[WIN_R_PROF];
		//
		CPRO->Create(IDD_PROFILE,this);
		//
		int		Dcnt= PM.Acnt;
		////
		CPRO->CCT = DSET.ST + CTimeSpan(0l, (DSET.Hours / DSET.DT_Out*PM.Acnt), 0, 0);	// メインの表示時刻
		////
		CPRO->Data_Exist = TRUE;
		//
		for(int i=0;i<PM.LineNum;i++) {
			PM.Pval[0][i] = MESH[PM.Path[i].y][PM.Path[i].x].Adem +
					MESH[PM.Path[i].y][PM.Path[i].x].Hval[0][Dcnt] - MESH[PM.Path[i].y][PM.Path[i].x].Rdepth;
			//
			PM.Pval[1][i] = MESH[PM.Path[i].y][PM.Path[i].x].Adem;
			PM.Pval[2][i] = MESH[PM.Path[i].y][PM.Path[i].x].Adem - MESH[PM.Path[i].y][PM.Path[i].x].Rdepth;
			PM.Pval[3][i] = MESH[PM.Path[i].y][PM.Path[i].x].Adem + MESH[PM.Path[i].y][PM.Path[i].x].Bheight;
		}
		//
		PM.Pro_draw_Acnt = Dcnt;
		PM.Pro_Ready =PM.Profile_Proc = FALSE;

		CPRO->ShowWindow( SW_SHOW );
		//
		STATG( DISP_PROFILE ) = TRUE;
	}
}
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
//
//    流域縦断
//
////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
void	CRRIView::Disp_MeshProfile()
{
	IF_STATG( DISP_EDITWIN ) return;
	IF_STATG( DISP_BREAKTIME ) return;
	IF_STATG( DISP_PROFILE ) return;
	IF_STATG( DISP_SECTION ) return;
	IF_STATG( DISP_MESHPROFILE ) return;
	IFNOT_STATG(  SET_MESHPROFILE ) return;
	//
	int		dx,dy,Cx,Cy,ctime;
	Mnum = (int)max(fabs(Prof_ST[1].x	- Prof_ST[0].x ),fabs(Prof_ST[1].y	- Prof_ST[0].y ) ) + 1;
	Mmem = Mnum + 10;
	if ((MID = (IXYCOORD*)malloc(Mmem*sizeof(IXYCOORD))) == NULL ||
	   (Mpos=(CPoint*)malloc(Mmem*sizeof(CPoint))) == NULL ||
	   (DEM=(double*)malloc(Mmem*sizeof( double))) == NULL ||
	   (HWL=(double*)malloc(Mmem*sizeof( double))) == NULL ) {ERR_RETURN("Cannot Allocate Memory... ");}
	//
	if( Prof_ST[1].x > Prof_ST[0].x )		dx =  1;
	else if( Prof_ST[1].x < Prof_ST[0].x )	dx = -1;
	else									dx = 0;
	if( Prof_ST[1].y > Prof_ST[0].y )		dy =  1;
	else if( Prof_ST[1].y < Prof_ST[0].y )	dy = -1;
	else									dy = 0;
	//
	Cx = Prof_ST[0].x; Cy = Prof_ST[0].y;
	Mnum = 0;
	ctime = PM.Acnt;
	if (ctime >= DSET.DT_Out) ctime = DSET.DT_Out - 1;
	//
	int		Dcnt;
	CTime	Now;
	BOOL		Exist = FALSE;

	Dcnt = ctime;
	Now = DSET.CT;
	Exist=TRUE;

	while( TRUE ) {
		if( (Cx >=0 && Cx < PM.MHnum ) && 
			(Cy >=0 && Cy < PM.MVnum ) ) {
			MID[Mnum].x = Cx;
			MID[Mnum].y = Cy;
			if (MESH[Cy][Cx].flag) {
				DEM[Mnum] = MESH[Cy][Cx].Adem;
				if (Exist) HWL[Mnum] = DEM[Mnum] + MESH[Cy][Cx].Hval[OUT_HS][Dcnt];
				else        HWL[Mnum] = DEM[Mnum];
			}
			else {
				DEM[Mnum] = DUNDEF;
				HWL[Mnum] = DUNDEF;
			}
			Mpos[Mnum].x = Cx+1;
			Mpos[Mnum].y = PM.MVnum - Cy;
		}
		else {
			MID[Mnum].x = MID[Mnum].y = UNDEF;
			Mpos[Mnum].x = Mpos[Mnum].y = UNDEF;
			DEM[Mnum] = DUNDEF;
			HWL[Mnum] = DUNDEF;
		}
		//
		Mnum++;
		if( Cx == Prof_ST[1].x && Cy == Prof_ST[1].y ) break;
		//
		Cx += dx;
		Cy += dy;
	}
	//
	MPROF = new CMeshProfile;
	//
	CRect	Prect;

	this->GetWindowRect( &Prect );
	MPROF->Prect = Prect;
	MPROF->WinH = WIN_H[ WIN_A_PROF ];
	MPROF->WinW = WIN_W[ WIN_A_PROF ];
	//
	MPROF->Create(IDD_MESHPROFILE,this);
	//
	MPROF->Data_Exist = Exist;
	MPROF->CCT = Now;
	//
	MPROF->Mnum = Mnum;
	MPROF->Mmem = Mmem;
	MPROF->MID = MID;
	MPROF->Mpos = Mpos;
	MPROF->DEM = DEM;
	MPROF->HWL = HWL;
	//
	PM.MPro_draw_Acnt = Dcnt;
	//
	MPROF->ShowWindow( SW_SHOW );
	STATG( DISP_MESHPROFILE ) = TRUE;
}
//
//-----------------------------------------------------------------------------------------------------------
//  RRI 用カラーピッカー
//
BOOL  CRRIView::RRI_Color_Picker(double value,BOOL Mosaic,BOOL Equal,int num,double* vm,COLORREF* col,FCOLOR* Fcol)
{
	double	span,R,G,B;
	BOOL	ex;
	//
	if( value < vm[0] ) return FALSE;
	if( value == vm[0] ) {
		if( Equal ) {
			*Fcol = F_COLOR(GetRValue( col[0] ),GetGValue( col[0] ),GetBValue( col[0] ));
			return TRUE;
		}
		else		return FALSE;
	}
	else if( value > vm[num-1] ) {
		*Fcol = F_COLOR(GetRValue( col[num-1] ),GetGValue( col[num-1] ),GetBValue( col[num-1] ));
		return TRUE;
	}
	else if( value == vm[num-1] ) {
		if( Equal ) *Fcol = F_COLOR(GetRValue( col[num-1] ),GetGValue( col[num-1] ),GetBValue( col[num-1] ));
		else	*Fcol = F_COLOR(GetRValue( col[num-2] ),GetGValue( col[num-2] ),GetBValue( col[num-2] ));
		return TRUE;
	}
	//
	for(int i=1;i<num;i++) {
		ex = FALSE;
		if( Equal ) {if( value >= vm[i-1]  && value < vm[i] ) ex = TRUE;}
		else		 {if( value > vm[i-1]  && value <= vm[i] ) ex = TRUE;}
		if( ex ) {
			if( Mosaic ) *Fcol = F_COLOR(GetRValue( col[i-1] ),GetGValue( col[i-1] ),GetBValue( col[i-1] ));
			else {
				if( value < vm[1] ) *Fcol = F_COLOR(GetRValue( col[0] ),GetGValue( col[0] ),GetBValue( col[0] ));
				else {
					span = (vm[i] - vm[i-1]);
					R = (double)(GetRValue(col[i-1]) - GetRValue(col[i-2]))*(value-vm[i-1])/span + GetRValue(col[i-2]);
					G = (double)(GetGValue(col[i-1]) - GetGValue(col[i-2]))*(value-vm[i-1])/span + GetGValue(col[i-2]);
					B = (double)(GetBValue(col[i-1]) - GetBValue(col[i-2]))*(value-vm[i-1])/span + GetBValue(col[i-2]);
					*Fcol = F_COLOR( R,G,B );
				}
			}
		}
	}
	return TRUE;
}
//
////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
//    リスト観測点

//void CRRIView::OnCbnSelchangeComboObspos()
//{
//	int		xid,yid,id,Lid = m_ComboOBS.GetCurSel();
//	Lid--;
//	xid = PM.OBpos[ Lid ].x;
//	yid = PM.OBpos[ Lid ].y;
//	id = yid*PM.MHnum + xid;
//	//
//	IFNOT_STATG( DISP_SECTION ) {
//		if(MESH[yid][xid].flag == FLAG_ON ) Show_Section_T(xid,yid,Lid);
//	}
//}
void CRRIView::Show_Section_T(int xid,int yid,int Lid)
{
	int			dt,id = yid*PM.MHnum + xid;
	CSection		CS(this);
	CString		title;
	CS.MESH = &MESH[yid][xid];
	//
	if (MESH[yid][xid].flag == 2) 	CS.isRiver = TRUE;
	else 	CS.isRiver = FALSE;
	//
	CS.st = DSET.ST;
	CS.xid = xid + 1;
	CS.yid = PM.MVnum - yid;
	CS.vid = yid;
	CS.hid = xid;
	//
	CStitle.Format("HydroGraph of %s [ %d %d ]",PM.OBpos[Lid].name,
		PM.OBpos[Lid].x+1,PM.MVnum - PM.OBpos[Lid].y);
	//
	CS.WinTitle = CStitle;
	CRect	Wrect;
	this->GetWindowRect( &Wrect );
	CS.WinH = WIN_H[ WIN_HYDRO ];
	CS.WinW = WIN_W[ WIN_HYDRO ];
	CS.m_Parent = this;
	CS.Prect = Wrect;
	//
	STATG( DISP_SECTION ) = TRUE;
	CS.DoModal();
	STATG( DISP_SECTION ) = FALSE;
	//
	//m_ComboOBS.SetCurSel( 0 );
}
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//		モデルの作成
int		CRRIView::Make_Mesh_Model()
{
	GL_OBJECT*	Mobj = &Gp.Gobj[OBJID_WL];	// 全体座標で表示
	GLobj_Delete(&Gp, OBJID_WL);
	//
	GLobj_Init(Mobj, (PM.MHnum + 1)*(PM.MVnum + 1), PM.MHnum*PM.MVnum);
	//
	/// VERT
	for (int v = 0; v <= PM.MVnum; v++) {
		for (int h = 0; h <= PM.MHnum; h++) {
			GLobj_Add_Vert(Mobj, (double)h, -MESH_Z,(double)v,  FALSE);
		}
	}
	//
	/// Polygon
	int			no[4], id, pno;
	FCOLOR		fcol, Gcol = F_COLOR(80, 80, 80);
	//
	for (int v = 0; v < PM.MVnum; v++) {
		for (int h = 0; h < PM.MHnum; h++) {
			if (MESH[v][h].flag == 0) continue;

			id = v*(PM.MHnum + 1) + h;
			no[0] = id;
			no[1] = id + 1;
			no[2] = no[1] + (int)(PM.MHnum + 1);
			no[3] = no[2] - 1;
			//
			if (Get_Data_Color(TRUE, v, h, 0., &fcol)) {
				pno = GLobj_Add_Poly(Mobj, 4, no, GL_POLYGON, fcol);
				Mobj->poly[pno].Alpha = (float)PM.Data_Alpha;
			}
		}
	}
	//
	return 0;
}
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  SELECT COLOR 
BOOL  CRRIView::Get_Data_Color(BOOL Dsel, int v, int h, double val, FCOLOR* Fcol)
{
	//	double	val;
	//
	if (COUT < 0 && COUT >= FILE_OUT_NUM) return FALSE;
	val = MESH[v][h].Hval[COUT][PM.Acnt];
	if (val <= 0.) return FALSE;
	//
	if (PM.OUTCOL[COUT].Equal) {
		if (val < PM.OUTCOL[COUT].val[0]) return FALSE;
	}
	else {
		if (val <= PM.OUTCOL[COUT].val[0]) return FALSE;
	}
	
	if (!RRI_Color_Picker(val, PM.OUTCOL[COUT].Mos, PM.OUTCOL[COUT].Equal,
		PM.OUTCOL[COUT].num, PM.OUTCOL[COUT].val, PM.OUTCOL[COUT].Rcol, Fcol)) return FALSE;
	return TRUE;
}
int		CRRIView::Make_Mesh_Line()
{
	GL_OBJECT*	Mobj = &Gp.Gobj[OBJID_LINE];	// 全体座標で表示
	GLobj_Delete(&Gp, OBJID_LINE);
	//
	GLobj_Init(Mobj, (PM.MHnum + 1)*(PM.MVnum + 1), PM.MHnum*PM.MVnum);
	//
	// VERT
	for (int v = 0; v <= PM.MVnum; v++) {
		for (int h = 0; h <= PM.MHnum; h++) {
			GLobj_Add_Vert(Mobj, (double)h, -LINE_Z,(double)v,  FALSE);
		}
	}
	//
	// Polygon
	int no[4], id, pno;
	FCOLOR		fcol, Mcol, Rcol, Bcol;
	Mcol = CREF_FCOL(PM.Col_Mesh);
	Rcol = CREF_FCOL(PM.Col_MeshR);
	Bcol = CREF_FCOL(PM.Col_MeshB);

	//
	for (int v = 0; v < PM.MVnum; v++) {
		for (int h = 0; h < PM.MHnum; h++) {

			if (MESH[v][h].flag == 0) continue;
			//
			id = v*(PM.MHnum + 1) + h;
			no[0] = id;
			no[1] = no[0] + 1;
			no[2] = no[1] + (int)(PM.MHnum + 1);
			no[3] = no[2] - 1;
			//
			if (MESH[v][h].flag == 1)	{
				pno = GLobj_Add_Poly(Mobj, DIM_3D, 4, no, GL_LINE_LOOP, 0.5f, Mcol);
			}
			else if (MESH[v][h].flag == 2)	{
				pno = GLobj_Add_Poly(Mobj, DIM_3D, 4, no, GL_LINE_LOOP, 1.f, Rcol);
				Mobj->poly[pno].local.set = TRUE;
				Mobj->poly[pno].local.offy = -2.;
			}
			else if (MESH[v][h].flag == 3)	{
				pno = GLobj_Add_Poly(Mobj, DIM_3D, 4, no, GL_LINE_LOOP, 1.f, Bcol);
				Mobj->poly[pno].local.set = TRUE;
				Mobj->poly[pno].local.offy = -3.;
			}
		}
	}

	//
	return 0;
}

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  DRAW BACK IMAGE
int		CRRIView::Make_Back_Image()		// 一度だけなのに注意
{
	PM.Back_stLat = (int)PM.stLAT;			// ※　注意　負の場合も考慮
	PM.Back_stLng = (int)PM.stLNG;
	PM.Back_endLat = (int)PM.endLAT;
	PM.Back_endLng = (int)PM.endLNG;
	//
	if (PM.stLAT < 0)		PM.Back_stLat--;		// 南半球対応
	if (PM.endLAT < 0)	PM.Back_endLat--;
	//
	if (PM.stLNG < 0)		PM.Back_stLng--;		// 西経対応
	if (PM.endLNG < 0)	PM.Back_endLng--;
	//
	//
	PM.BVnum = PM.Back_endLat - PM.Back_stLat + 1;
	PM.BHnum = PM.Back_endLng - PM.Back_stLng + 1;
	//
	GL_OBJECT*	obj = &Gp.Gobj[OBJID_BACK];
	int			lat, lng, no[4];
	CString		Iname;
	double		Ex, Ey;		// 原点位置
	double		bx, by;
	FXYCOORD		uv[4];
	int			ResH, ResV, imbyte,TexNo;
	BYTE*		pImage;
	//
	GLobj_Delete(&Gp, OBJID_BACK);
	GLobj_Init(obj, (PM.BVnum + 1)*(PM.BHnum + 1), PM.BVnum*PM.BHnum);
	//
	if (PM.stLNG >= 0.) Ex = -fmod(PM.stLNG, 1.)* PM.Deg_Unit;		// UTM の場合はここに来ない。
	else               Ex = -(PM.stLNG - PM.Back_stLng)*PM.Deg_Unit;
	//
	if (PM.stLAT >= 0.) Ey = -fmod(PM.stLAT, 1.)* PM.Deg_Unit;
	else                Ey = -(1. - fmod(fabs(PM.stLAT), 1.))* PM.Deg_Unit;
	//
	::SetCurrentDirectory(Gp.m_CurrentPath);
	PM.Back_TexNo = Make_Matrix(PM.BHnum, PM.BVnum, UNDEF);
	//
	TexNo = 1;
	for (int v = 0; v < PM.BVnum; v++){
		lat = PM.Back_stLat + v;
		by = Ey + PM.Deg_Unit*v;
		for (int h = 0; h < PM.BHnum; h++) {
			lng = PM.Back_stLng + h;
			if (lat >= 0) {
				if (lng >= 0.) Iname.Format("%s\\N%02dE%03d.bmp", PM.IM_Folder, lat, lng);
				else         Iname.Format("%s\\N%02dW%03d.bmp", PM.IM_Folder, lat, fabs(lng));
			}
			else   {
				if (lng >= 0.) Iname.Format("%s\\S%02dE%03d.bmp", PM.IM_Folder, -lat, lng);
				else          Iname.Format("%s\\S%02dW%03d.bmp", PM.IM_Folder, -lat, fabs(lng));
			}
			//
			if (!File_Exist(Iname)) continue;
			//---------------------------------------------------------------------------
			//pImage = Get_BMP_File(Iname, &ResH, &ResV, &imbyte, TRUE);
			//if (GL_Set_Bitmap(TexNo, &Gp, ResH, ResV, 3, 1, pImage) < 0) {
			//	TexNo = TexNo;
			//}
			PM.Back_TexNo[v][h] = GL_Set_Texture(&Gp, Iname, 3);
			//PM.Back_TexNo[v][h] = TexNo++;
			////
			bx = Ex + PM.Deg_Unit*h;
			no[0] = GLobj_Add_Vert(obj, bx,-BACK_Z,by,  FALSE);
			uv[0].x = 0.; uv[0].y = 1.;

			no[1] = GLobj_Add_Vert(obj, bx + PM.Deg_Unit, -BACK_Z,by,  FALSE);
			uv[1].x = 1.; uv[1].y = 1.;

			no[2] = GLobj_Add_Vert(obj, bx + PM.Deg_Unit, -BACK_Z,by + PM.Deg_Unit,  FALSE);
			uv[2].x = 1.; uv[2].y = 0.;

			no[3] = GLobj_Add_Vert(obj, bx,-BACK_Z, by + PM.Deg_Unit,  FALSE);
			uv[3].x = 0.; uv[3].y = 0.;
			//
			//
			GLobj_Add_Poly(obj, 4, no, GL_POLYGON, PM.Back_TexNo[v][h], uv);
			//
//			SAFE_FREE(pImage);
		}
	}
	//
	GL_OBJECT*	Cobj = &Gp.Gobj[OBJID_COPYRIGHT];
	GLobj_Delete(&Gp, OBJID_COPYRIGHT);
	GLobj_Init(Cobj, 10, 3);
	GLobj_Add_Text(&Gp, OBJID_COPYRIGHT, DIM_2D, "Map data (c) OpenStreetMap contributors",
		"Lucida Sans", FONT_BOLD, 18, 7,
		Gp.win.cWidth - 320., 12., -1., F_COLOR(125, 155, 200));
	//
	::SetCurrentDirectory(m_RecentPath);
	return 0;
}

int		CRRIView::Make_River_Mark(BOOL Switch)
{
	GL_OBJECT*		obj = &Gp.Gobj[OBJID_MARK];
	GLobj_Delete(&Gp, OBJID_MARK);
	int		no[4],Num;
	double	x, y;
	//
	if (Switch) {
		GLobj_Init(obj, PM.LineNum * 5, PM.LineNum + 5);		// PM.Path : PM.LineNum
		Num = PM.LineNum;
	}
	else  {
		GLobj_Init(obj, 5, 5);
		Num = 1;
	}
	//
	for (int m = 0; m < Num; m++){
		if (Switch) {
			x = (double)PM.Path[m].x;
			y = (double)PM.Path[m].y;
		}
		else {
			x = (double)PM.LineID[m].x;
			y = (double)PM.LineID[m].y;

		}
		no[0] = GLobj_Add_Vert(obj, x,-SELECT_Z, y,  FALSE);
		no[1] = GLobj_Add_Vert(obj, x + 1., -SELECT_Z, y, FALSE);
		no[2] = GLobj_Add_Vert(obj, x + 1., -SELECT_Z, y + 1., FALSE);
		no[3] = GLobj_Add_Vert(obj, x,-SELECT_Z, y + 1.,  FALSE);
		//
		GLobj_Add_Poly(obj, 4, no, GL_POLYGON, F_COLOR(255, 0, 180));
	}
	GL_Redraw(&Gp, FALSE, FALSE, TRUE, OBJID_MARK, TRUE);
	return 0;
}
BOOL		CRRIView::Search_River_Path()
{
	int		Hx, Hy, Lx, Ly, dir;
	int		idx, idy;
	double	dist,UnitKm;
	//
	if (MESH[PM.LineID[0].y][PM.LineID[0].x].Adem >= MESH[PM.LineID[1].y][PM.LineID[1].x].Adem) {
		Hx = PM.LineID[0].x; Hy = PM.LineID[0].y; Lx = PM.LineID[1].x; Ly = PM.LineID[1].y;
	}
	else { Hx = PM.LineID[1].x; Hy = PM.LineID[1].y; Lx = PM.LineID[0].x; Ly = PM.LineID[0].y; }
	//
	//////PM.LineMem = 100;
	//////if ((PM.Path = (IXYCOORD*)malloc(PM.LineMem*sizeof(IXYCOORD))) == NULL ||
	//////	(PM.Rdist = (double*)malloc(PM.LineMem*sizeof(double))) == NULL) return FALSE;
	PM.LineMem = 1000;
	PM.Pval = Make_Matrix(PM.LineMem, 4, 0.);
	// 
	UnitKm = DEG_DISTANCE / PM.Deg_Unit;  // 100km / 120
	PM.LineNum = 0;
	idx = PM.Path[PM.LineNum].x = Hx;
	idy = PM.Path[PM.LineNum].y = Hy;
	PM.Rdist[PM.LineNum++] = 0.;
	//							標高の高いほうから、流下方向を探っていく。
	while (TRUE){
		dir = UNDEF;
		for (int d = 0; d < 8; d++) {
			if (MESH[idy][idx].Adir == Ddirect[d]) { dir = d; break; }
		}
		if (dir == UNDEF) return FALSE;	// 方向が定義されていない　or 0
		//
		dist = 1.;
		if (dir % 2 == 1) dist = sqrt(2.);
		//
		idx += offx[dir];
		idy += offy[dir];
		PM.Path[PM.LineNum].x = idx;
		PM.Path[PM.LineNum].y = idy;
		PM.Rdist[PM.LineNum] = PM.Rdist[PM.LineNum-1] + dist;
		PM.LineNum++;
		//
		if (idx == Lx && idy == Ly) break;
		//
		//////if (PM.LineNum == PM.LineMem) {
		//////	PM.LineMem += 50;
		//////	if ((PM.Path = (IXYCOORD*)realloc(PM.Path, PM.LineMem*sizeof(IXYCOORD))) == NULL) return FALSE;
		//////}
		if (PM.LineNum >= 1000) {
			MessageBox("** Error **\nCannot display profile Because there is too much number of the mesh, \nPlease shorten distance.",
				"stop", MB_OK | MB_ICONERROR);
			Free_Matrix(4, PM.Pval); PM.LineNum = 0; return FALSE;
		}
	}
	//
	for (int i = 0; i < PM.LineNum; i++) PM.Rdist[i] *= UnitKm;
	// 
	return TRUE;
}

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  EDIT WINDOW 
void		CRRIView::Display_EditWindow(int xid, int yid)
{
	IF_STATG(DISP_EDITWIN) {
		EDDlg->OnCancel();
		STATG(DISP_EDITWIN) = TRUE;
	}
	// Data Set
	if (Gp.cam[CVIEW].viewdir.z == 1.)			PM.Cam_Dir = CAM_DIR_UP;
	else if (Gp.cam[CVIEW].viewdir.x == 1.)		PM.Cam_Dir = CAM_DIR_LEFT;
	else if (Gp.cam[CVIEW].viewdir.x == -1.)	PM.Cam_Dir = CAM_DIR_RIGHT;
	//
	if (xid != UNDEF) PM.CX = xid;
	if (yid != UNDEF) PM.CY = yid;
	//
	for (int v = 0; v<PM.MVnum; v++) {
		for (int h = 0; h<PM.MHnum; h++) PM.Values[v][h] = MESH[v][h].hsmax;
	}
	//
	EDDlg = new CEDITDLG;
	EDDlg->Create(IDD_EDITDLG, this);
	//
	EDDlg->m_Parent = this;
	//
	PM.Data_Change_Cnt = 0;	// 変更の全体数
	//
	EDDlg->ShowWindow(SW_SHOW);
	STATG(DISP_EDITWIN) = TRUE;
}
//_____________________________________________________
//  編集終了処理
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		CRRIView::Save_Manager()
{

	PM.Save_Proc = FALSE;
	PM.Edit_Result = 0;
	//	EDDlg = NULL;		// エラー発生、ヒープエラーを起こす
}
void		CRRIView::Term_Manager()
{
	Save_Manager();
	//
	GLobj_Delete(&Gp, OBJID_MOUSE_BAND);
	GL_Redraw(&Gp, FALSE, FALSE, TRUE, OBJID_MOUSE_BAND, TRUE);
	//
	STATG(DISP_EDITWIN) = FALSE;
	STATG(MS_RUBBER_START) = FALSE;
}

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Change Back Image

void CRRIView::OnBnClickedButtonChangeImage()
{
	char			InDir[SIZE_1K];
	CString		IMDIR;
	//
	if (!Button_Dir(&IMDIR, m_RecentPath)) return;
	strcpy(InDir, IMDIR);
	//
	struct _finddata_t	c_file;	// time_write が作成時間を示す
	long					hFile;
	//
	CString				Fname, str;
	FILE*				fp;
	char					inchar[SIZE_01K], IMname[SIZE_01K];
	int					Texno, no[4];
	double				x1, y1, x2, y2;
	double				minx, miny, maxx, maxy;
	//
	GL_OBJECT*	obj = &Gp.Gobj[OBJID_BACK];
	FXYCOORD		uv[4];
	//
	// TEX リソースの解放		ガイドＡＣＣ画像の破棄
	if (Gp.Texnum > 1) {
		for (int i = 1; i < Gp.Texnum + 1; i++)  {
			SAFE_FREE(Gp.Tex[i].image);
		}
		Gp.Texnum = 1;
	}
	GLobj_Delete(&Gp, OBJID_BACK);
	GLobj_Init(obj, 100, 100);
	//
	::SetCurrentDirectory(InDir);
	if ((hFile = _findfirst("*.loc", &c_file)) == -1L) {
		RETURN("** Error ** : Cannot find loc file......");
	}
	//
	do {
		str.Format("[%s] is found ", c_file.name); UDF;
		fp = fopen(c_file.name, "r");
		fgets(inchar, SIZE_01K, fp);	// UTM or GCS
		if (!DSET.SW_LatLong && strcmp(inchar, "UTM") == 0) { fclose(fp); continue; }
		if (DSET.SW_LatLong && strcmp(inchar, "LATLON") == 0)  { fclose(fp); continue; }
		//
		fgets(IMname, SIZE_01K, fp); IMname[strlen(IMname) - 1] = 0x00;
		Fname = IMname;

		Texno = GL_Set_Texture(&Gp, Fname, 3);
		//
		fgets(inchar, SIZE_01K, fp);
		sscanf(inchar, "%lf,%lf", &x1, &y1);
		fgets(inchar, SIZE_01K, fp);
		sscanf(inchar, "%lf,%lf", &x2, &y2);
		fclose(fp);
		//
		if (!DSET.SW_LatLong) {
			PM.Coord_Unit = PM.Deg_Unit;
			minx = (x1 - PM.stLNG)*PM.Coord_Unit;
			miny = (y1 - PM.stLAT)*PM.Coord_Unit;
			maxx = (x2 - PM.stLNG)*PM.Coord_Unit;
			maxy = (y2 - PM.stLAT)*PM.Coord_Unit;
		}
		else {
			PM.Coord_Unit = 1. / PM.UTMSize;
			minx = (x1 - PM.stUTM_X)*PM.Coord_Unit;
			miny = (y1 - PM.stUTM_Y)*PM.Coord_Unit;
			maxx = (x2 - PM.stUTM_X)*PM.Coord_Unit;
			maxy = (y2 - PM.stUTM_Y)*PM.Coord_Unit;
		}
		//
		no[0] = GLobj_Add_Vert(obj, minx, BACK_Z,miny,  FALSE);
		uv[0].x = 0.; uv[0].y = 1.;
		no[1] = GLobj_Add_Vert(obj, maxx, BACK_Z,miny,  FALSE);
		uv[1].x = 1.; uv[1].y = 1.;
		no[2] = GLobj_Add_Vert(obj, maxx, BACK_Z,maxy,  FALSE);
		uv[2].x = 1.; uv[2].y = 0.;
		no[3] = GLobj_Add_Vert(obj, minx, BACK_Z, maxy, FALSE);
		uv[3].x = 0.; uv[3].y = 0.;
		//
		GLobj_Add_Poly(obj, 4, no, GL_POLYGON, Texno, uv);


	} while (_findnext(hFile, &c_file) == 0L);
	_findclose(hFile);
	//
	/// Delete CopyRight
	GLobj_Delete(&Gp, OBJID_COPYRIGHT);
	//
	GL_Redraw(&Gp, FALSE, FALSE, TRUE, OBJID_COPYRIGHT, TRUE);
	GL_Redraw(&Gp, FALSE, FALSE, TRUE, OBJID_BACK, TRUE);
}


//---------------------------------------------------------------------------------
//   Accuracy Evaluation
//---------------------------------------------------------------------------------

void CRRIView::OnBnClickedButtonAccuracyEvaluation()
{
	//Show_Section_HC(88, 106);
	Show_AccuracyEvaluation();
}


void CRRIView::Show_AccuracyEvaluation()
{
	CAccuracyEvaluation		CAE(this);
	CString		title;
	//
	CAE.WinTitle = CStitle;
	CRect	Wrect;
	this->GetWindowRect(&Wrect);
	CAE.WinH = WIN_H[WIN_HYDRO];
	CAE.WinW = WIN_W[WIN_HYDRO];
	CAE.m_Parent = this;
	CAE.Prect = Wrect;
	//
	STATG(DISP_ACCURACYEVALUATION) = TRUE;

	CAE.DoModal();
	STATG(DISP_ACCURACYEVALUATION) = FALSE;
}


void CRRIView::OnBnClickedmakehqeq()
{
	CMakeHQeqDialog dlg(this);

	CWnd*		cw;
	if (((CButton*)GetDlgItem(IDC_UNIFORM_FLOW_CHK))->GetCheck() == BST_CHECKED) {
		dlg.uniformflowFlg = true;
	}
	else {
		dlg.uniformflowFlg = false;
	}

	dlg.DoModal();


}


void CRRIView::OnBnClickedVCheckLocation()
{
	if (PM.LocationChk) {
		PM.LocationChk = false;
	}
	else {
		PM.LocationChk = true;
	}

	PM.B_Date = Bool_acent_Date;
	if (!DSET.out_exist[1])return;
	Anim_Set(TRUE);
	PM.Acnt = 0; m_cRunTime.SetPos(0);
	//
	Disp_Anim_Time(); Disp_Info();
	Redraw_Now();

}

void CRRIView::CustomizeCtrls()
{
#ifdef SIMP_VER
	GetDlgItem(makeHQeq)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_UNIFORM_FLOW_CHK)->ShowWindow(SW_HIDE);
#endif // SIMP_VER
}
