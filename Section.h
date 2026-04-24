#pragma once
#include  "CommFuncs.h"
#include  "CommGL.h"
#include	"RRI_Common.h"

#define		PICT_WIDE		1000
#define		PICT_HEIGHT		700

#define		SECTION_H_MARGIN	100	
#define		SECTION_VU_MARGIN	100	
#define		SECTION_VD_MARGIN	120	

// CSection ダイアログ

class CSection : public CDialog
{
	DECLARE_DYNAMIC(CSection)

public:
	CSection(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSection();

// ダイアログ データ
	enum { IDD = IDD_SECTION };

//------------------------------------------------------------------------
	CWnd*		m_Parent;
	CRect		Prect;
	int			WinH,WinW;
	//
	int			Hours,Tnum;
	int			timestep;
	CString		WinTitle;
	MINFO*		MESH;
	CTime*		ct;
	CTime		st;
	//
	int			xid, yid,vid,hid;  // ＲＲＩ座標に直してある。
	//
	BOOL			isRiver;
	BOOL			Fig_Ready;		// ON_PAINTのため
	//
	GB_PARAMETER	GB;
//------------------------------------------------------------------------
	void	Make_Section();
	void	Redraw_Section();
//------------------------------------------------------------------------

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	CString m_Hmax;
	CString m_Hmin;
	CString m_Qmax;
	CString m_Qmin;
	afx_msg void OnEnChangeEditHmin();
	afx_msg void OnEnChangeEditHmax();
	afx_msg void OnEnChangeEditQmin();
	afx_msg void OnEnChangeEditQmax();
	afx_msg void OnBnClickedButtonRedraw();
	afx_msg void OnBnClickedButtonAccuracyEvaluation();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonExport();
};
