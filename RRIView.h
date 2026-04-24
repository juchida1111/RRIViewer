///////////////////////////////////////////////////////////////////////////////////
// RRIView.h : CRRIView クラスのインターフェイス
//
//
///////////////////////////////////////////////////////////////////////////////////

#pragma once
#include	"RRIDoc.h"
#include	"resource.h"
//

#include	"Profile.h"
#include	"MeshProfile.h"
#include	"EDITDLG.h"


#include	"CommFuncs.h"
#include	"CommGL.h"
#include	"afxwin.h"
//
//------------------------------------------------------------------------------------  エラー定義
#define	F0_OPEN			 10
//
#define	F1_MEMORY		1000
#define	F1_OPEN			1100
#define	F1_READ			1200
//          F2    20** : memory  21** : read data
#define	F3_MEMORY		3000	// DAM _CNT
#define	F3_READ			3100
#define	F4_MEMORY		4000	// DIV_FILE
#define	F4_READ			4100	// DIV_FILE
#define	F5_MEMORY		5000	// Break Time
#define	F5_READ			5100	// Break Time
#define	F6_MEMORY		6000  // Evp
#define	F6_READ			6100	// Evp
#define	F7_MEMORY		7000	// DSET.Time_Rain
#define	F7_READ			7100	// Read Rain data
#define	F8_MEMORY		8000	// Drain
#define	F8_READ			8100	// Drain
//----------------------------------------------------------------------------
#define		RRI_SCREEN_TOOL_IMAGE   "./RRI_CONTENTS/COMMON/TOOL.bmp"
//----------------------------------------------------------------- 
#define		TOOL_DEFAULT_X	50.
#define		TOOL_DEFAULT_Y	100.
//___________________________________________________________________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////
//  クラス 定義
//-------------------------------------------------------------------------------------
class CRRIView : public CFormView
{
protected: // シリアル化からのみ作成します。
	CRRIView();
	DECLARE_DYNCREATE(CRRIView)

public:
	enum{ IDD = IDD_RRI_FORM };

//-----------------------------------------------------------------------------------------------------------
// Class  Members  表示 Common
//-----------------------------------------------------------------------------------------------------------  
	CString			m_InFileName,m_OutFileName,m_InDirName;
	CString			Bin_Folder,Bin_File;
//-----------------------------------------------------------------------------------------------------------
//  表示
//-----------------------------------------------------------------------------------------------------------
	TC_ITEM			tc_View,tc_Model;		// タブ情報
//-----------------------------------------------------------------------------------------------------------
// Class  Members   COMMON
//-----------------------------------------------------------------------------------------------------------  
	MINFO			*Udata,*Ddata;
	//
	CToolTipCtrl		m_ToolTip;
	//
	int				ComErr;		// 読み込みの段階で起きたエラーの詳細情報
	//
	int				CHANGE_DATA;
	int				Menu_Margin;
//-----------------------------------------------------------------------------------------------------------
// Dialog  Members   MODEL
//-----------------------------------------------------------------------------------------------------------  
	double			Dem_Scale;				// ONE_DEGREE_DISTANCEを１０下げたため

	//CEDITDLG*		EDDlg;
	////
	//CBTime*			BTDlg;
	int				BreakResult;
	//
	CProfile*		CPRO;						// River Profile
	CMeshProfile*	MPROF;				// Area Profile

	int				WIN_H[3],WIN_W[3];
	//
	BOOL				Bool_acent_Date;
//-----------------------------------------------------------------------------------------------------------
// Display Members  VIEWER   Profile in Mesh Area
//-----------------------------------------------------------------------------------------------------------  
	int				Mnum,Mmem;
	IXYCOORD*		MID;
	CPoint*			Mpos;
	double*			DEM;
	double*			HWL;

	CEDITDLG*		EDDlg;

//-----------------------------------------------------------------------------------------------------------
// Display Members  VIEWER   Make Shape  Temporary -> Save SBOX
//-----------------------------------------------------------------------------------------------------------  
	int				Snum,Smem;		// エリアシェイプ
	XYCOORD*			SXZ;
	int				SMnum,SMmem;	// エリアに含まれるメッシュID
	int*				SMid;
//-----------------------------------------------------------------------------------------------------------

public:
	CRRIDoc*  GetDocument() const;

//-----------------------------------------------------------------------------------------------------------
// Class  Functions    初期化・共通
//-----------------------------------------------------------------------------------------------------------  
// for CALLBACK
	static int AFXAPI BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	BOOL			Button_Dir(CString* m_DirName,char* m_RecentPath );
	BOOL			Button_Dir(CString* m_DirName, char* m_RecentPath, BOOL NewFolder);
	void			Render_Scene();
	//void			Drain_Msg() {	MSG msg;while (PeekMessage(&msg,0,0,0,PM_REMOVE)){}}
	//
	void			Get_Shape_Volume(double* vol,double* Area);
	//
	void			Init_Params();
	void			Disp_Controls();
	int			Set_Case();
	void			Set_Shapes(CString	Folder);
	//
	void			Disp_Info();
	void			Set_DispOption();
	//
	HRESULT		Read_Result_File(int Fid, int cnt, CString fname);
//-----------------------------------------------------------------------------------------------------------
// Class  Functions   Model
//-----------------------------------------------------------------------------------------------------------  
//  Viewer
	HRESULT			Set_Tex_Image();
	HRESULT			Make_Data();
//	HRESULT			Make_Model();
	HRESULT			Change_Color_MeshLine();
	HRESULT			Make_Image_Mesh();
	HRESULT			Change_Model(PARAMS* PM);

	HRESULT			Set_WL_Model();
	void				Draw_Legend();
	void				Draw_Date();
	void				Draw_Location();
	void				Redraw_Now();
	void				Record_Now();
	void				Disp_Anim_Time();
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    モデル作成
	int			Make_Mesh_Model();
	int 			Make_Mesh_Line();
	int			Make_Back_Image();
	int			Make_River_Mark(BOOL Switch);  // Switch TRUE=PM.Path, FALSE=PM.LineID

	BOOL			Get_Data_Color(BOOL Dsel, int v, int h, double val, FCOLOR* Fcol);
	BOOL			RRI_Color_Picker(double value, BOOL Mosaic, BOOL Equal, int num, 
								double* vm, COLORREF* col, FCOLOR* Fcol);
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//     スクリーンツール
	void			Make_Screen_Tool(BOOL  Tex);
	int			Decide_Tool(CPoint cp);
	void			EX_Tool(int id);
	void			Tool_Scale(BOOL inc);
	void			Tool_Move(int dir);
	//
	void			Show_Section(int xid,int yid);
	void			Show_Section_T(int xid,int yid,int Lid);
	void			Draw_Band_Area(double cx,double cy);

	BOOL			Search_River_Path();
	//
	void			Disp_MeshProfile();
	FCOLOR		Get_GIS_Color(double val,int  Dno,int* N);
	//
	void			Free_Project(  );
	//
	void			Anim_Set(BOOL SS);
	void			Check_Location_Set(BOOL SS);
	//
	void			Display_EditWindow(int xid, int yid);
	void			Save_Manager();
	void			Term_Manager();

	void            Show_AccuracyEvaluation();

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// オーバーライド
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnInitialUpdate(); // 構築後に初めて呼び出されます。
//________________________________________________________________________________________
//  表示
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

// 実装
public:
	virtual ~CRRIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:
	void CustomizeCtrls();


//-----------------------------------------------------------------------------------------------------------
// 生成された、メッセージ割り当て関数
//-----------------------------------------------------------------------------------------------------------  
// 
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CSliderCtrl		m_cSliderAnim;
	CString			m_DateInfo;
	BOOL				m_ColoredMesh;
	BOOL				m_Arrow;
	CSliderCtrl		m_cRunTime;
	CButton			c_RivPath;
	CButton			c_RivProfile;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnChangebackcolor();
	//afx_msg void OnChangemodel();
	//afx_msg void OnBnClickedVRadioColor();
	//afx_msg void OnBnClickedVRadioGray();
	afx_msg void OnBnClickedVButtonStart();
	afx_msg void OnBnClickedVButtonRec();
	afx_msg void OnBnClickedVCheckFrame();
	afx_msg void OnBnClickedVCheckBack();
	afx_msg void OnStnDblclickVMoredetail();

	afx_msg void OnNMCustomdrawSliderAnim(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedVCheckDate();
	afx_msg void OnStnDblclickVColorDate();
	afx_msg void OnBnClickedVCheckLegend();
	afx_msg void OnBnClickedVButtonRew();
	afx_msg void OnBnClickedVButtonFor();
	afx_msg void OnBnClickedVButtonBack();
	afx_msg void OnBnClickedVButtonNext();
	//afx_msg void OnStnDblclickVAnimOption();
	afx_msg void OnNMCustomdrawSliderRuntime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnDblclickVColorLegend();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg void OnStnClickedVGis0();
	afx_msg void OnStnClickedVGis1();
	afx_msg void OnStnClickedVGis2();
	afx_msg void OnStnClickedVGis4();
	afx_msg void OnStnClickedVGis5();
	afx_msg void OnStnClickedVGis3();
	afx_msg void OnBnClickedVButtonRivpath();
	afx_msg void OnBnClickedVButtonProfile();
	//afx_msg void OnBnClickedCheckMDatacolor();
	afx_msg void OnActivateSection();
	afx_msg void OnBnClickedVCheckArrow();
	//afx_msg void OnCbnSelchangeComboShape();
	//afx_msg void OnCbnSelchangeComboObspos();
	afx_msg void OnBnClickedCheckVShowdata();

	afx_msg void OnStnClickedVGis6();
	afx_msg void OnStnClickedVGis7();
	afx_msg void OnStnClickedVGis8();
	afx_msg void OnStnClickedVGis9();
	afx_msg void OnBnClickedButtonOutmax();
	afx_msg void OnBnClickedButtonChangeImage();
	afx_msg void OnBnClickedButtonAccuracyEvaluation();
	afx_msg void OnBnClickedmakehqeq();
	afx_msg void OnBnClickedVCheckLocation();
};

#ifndef _DEBUG  // RRIView.cpp のデバッグ バージョン
inline CRRIDoc* CRRIView::GetDocument() const
   { return reinterpret_cast<CRRIDoc*>(m_pDocument); }
#endif

