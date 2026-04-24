#pragma once
#include  "CommFuncs.h"
#include  "RRI_Common.h"

////////////////////////////////////////////////////////////////////////////////////////
//
//	OPTIONS ダイアログ
//
////////////////////////////////////////////////////////////////////////////////////////
//__________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------　パラメータ定義

//#define		Comment(x)			m_Comment=x;UDF;
//#define		Com_Clear()			{m_Comment=_T("");UDF;}

#define		WIDTH_LIMIT			600
#define		HEIGHT_LIMIT		400

//__________________________________________________________________________________________________
////////////////////////////////////////////////////////////////////////////////////////////////////
class OPTIONS : public CDialogEx
{
	DECLARE_DYNAMIC(OPTIONS)

public:
	OPTIONS(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~OPTIONS();

// ダイアログ データ
	enum { IDD = IDD_OPTIONS };
//-----------------------------------------------------------------------------------------------------------
// Members
//-----------------------------------------------------------------------------------------------------------  

	void		Get_Timming() {	MSG msg;
					while (PeekMessage(&msg,0,0,0,PM_REMOVE)){
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					return;}

	PARAMS					pm;
	
	int				WIN_H[3],WIN_W[3];
	int				Cdata;
	int				Cshape;			// CSHAPE の仮変数
	int				Delete_Shape;		// Shape (sdd) を削除した
	int				Add_Shape;		// shape ファイルを読み込んだ
	int				IDno[500];		// 大きすぎか？
	//
	CString			Boot_Folder;

	BOOL			Ready;

//-----------------------------------------------------------------------------------------------------------
// Class  Function
//-----------------------------------------------------------------------------------------------------------  
	static int AFXAPI BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	BOOL		Button_Dir(CString* m_DirName,char* m_RecentPath );

	void		Disp_Buttons();
	void		Fill_Color(int  ID,COLORREF col);
	double	Get_EditRect( int ID );
	void		Disp_Color( int ID );

	void		Renew_Legend();
	void		Set_COLOR_Data(int no);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//CSliderCtrl m_TRANS_SLIDER;
	//CString m_Comment;
	//afx_msg void OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditA0();
	afx_msg void OnEnChangeEditA1();
	afx_msg void OnEnChangeEditA2();
	afx_msg void OnEnChangeEditA3();
	afx_msg void OnEnChangeEditA4();
	afx_msg void OnEnChangeEditA5();
	afx_msg void OnEnChangeEditA6();
	afx_msg void OnEnChangeEditA7();
	afx_msg void OnEnChangeEditA8();
	//afx_msg void OnStnDblclickColMeshline();
	//afx_msg void OnStnDblclickColMeshriver();
	//afx_msg void OnStnDblclickColMeshbound();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnStnDblclickColA0();
	afx_msg void OnStnDblclickColA1();
	afx_msg void OnStnDblclickColA2();
	afx_msg void OnStnDblclickColA3();
	afx_msg void OnStnDblclickColA4();
	afx_msg void OnStnDblclickColA5();
	afx_msg void OnStnDblclickColA6();
	afx_msg void OnStnDblclickColA7();
	afx_msg void OnStnDblclickColA8();
	afx_msg void OnBnClickedCheckFrame();
	afx_msg void OnStnDblclickColWlframe();
	afx_msg void OnEnChangeEditA9();
	afx_msg void OnBnClickedRadioAEq();
	afx_msg void OnBnClickedRadioAGr();
	afx_msg void OnBnClickedOk();
	//afx_msg void OnEnChangeEditHdWidth();
	//afx_msg void OnEnChangeEditHdHeight();
	//afx_msg void OnEnChangeEditRivWidth();
	//afx_msg void OnEnChangeEditRivHeight();
	//afx_msg void OnEnChangeEditApWidth();
	//afx_msg void OnEnChangeEditApHeight();
	//CString m_AP_H;
	//CString m_AP_W;
	//CString m_HD_W;
	//CString m_HD_H;
	//CString m_RP_H;
	//CString m_RP_W;
	CComboBox c_ColCombo;
	afx_msg void OnCbnSelchangeComboColor();
	afx_msg void OnBnClickedButtonColsave();
	//CComboBox m_CShape;
	//afx_msg void OnCbnSelchangeComboShape();
	//afx_msg void OnBnClickedButtonDel();
	//afx_msg void OnBnClickedButtonClear();
	//afx_msg void OnBnClickedButtonShpsave();
	afx_msg void OnStnDblclickColSp();
	afx_msg void OnPaint();
	//afx_msg void OnBnClickedButtonAddshape();
};
