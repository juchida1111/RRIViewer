
// CAnimOpt ダイアログ

#pragma once

#include  "RRI_Common.h"
#include  "CommFuncs.h"

////////////////////////////////////////////////////////////////////////////////////////
//
//	ANIM OPTIONS ダイアログ
//
////////////////////////////////////////////////////////////////////////////////////////
class CAnimOpt : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimOpt)

public:
	CAnimOpt(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CAnimOpt();
//-----------------------------------------------------------------------------------------------------------
// Class  MEMBERS
//-----------------------------------------------------------------------------------------------------------  
	PARAMS				pm;
//-----------------------------------------------------------------------------------------------------------
// Class  Function
//-----------------------------------------------------------------------------------------------------------  
	static int AFXAPI BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	BOOL		Button_Dir(CString* m_DirName,char* m_RecentPath );

	void		Disp_Buttons();


// ダイアログ データ
	enum { IDD = IDD_ANIMOPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//
	CComboBox m_Lfont;
	CComboBox m_Tfont;
	CString m_Head;
	//CString m_LegHeight;
	//CString m_LegWidth;
	CString m_LH;
	CString m_LW;
	CString m_TH;
	CString m_TW;
	CString m_Comment;
//	
	afx_msg void OnStnDblclickColBackplate();
	afx_msg void OnBnClickedCheckBackplate();
	afx_msg void OnCbnSelchangeComboTfont();
	afx_msg void OnEnChangeEditTw();
	afx_msg void OnEnChangeEditTh();
	afx_msg void OnEnChangeEditLw();
	afx_msg void OnEnChangeEditLh();
	//afx_msg void OnEnChangeEditLegWidth();
	//afx_msg void OnEnChangeEditLegHeight();
	afx_msg void OnCbnSelchangeComboLfont();
	afx_msg void OnBnClickedRadioTs1();
	afx_msg void OnBnClickedRadioTs2();
	afx_msg void OnBnClickedRadioTs3();
	afx_msg void OnBnClickedRadioLs1();
	afx_msg void OnBnClickedRadioLs2();
	afx_msg void OnBnClickedRadioLs3();
	afx_msg void OnEnChangeEditHead();
	afx_msg void OnBnClickedRadioRec1();
	afx_msg void OnBnClickedRadioRec2();
	afx_msg void OnBnClickedButtonFolder();
	afx_msg void OnBnClickedOk();
	CString m_ALmag;
	CString m_ALrate;
	afx_msg void OnEnChangeEditAlRate();
	afx_msg void OnEnChangeEditAlMag();
	afx_msg void OnBnClickedCheckInclude();
	afx_msg void OnEnChangeEditAlHead();
	CString m_ALhead;
};
