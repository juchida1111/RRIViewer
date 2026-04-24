#pragma once
#include "RRI_Common.h"

#define		ID_BT_INIT		4000
#define		Y_BT_SIZE		20


// CBTDLG ダイアログ

class CBTDLG : public CDialog
{
	DECLARE_DYNAMIC(CBTDLG)

public:
	CBTDLG(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CBTDLG();

// ダイアログ データ
	enum { IDD = IDD_BREAK };
//

	CWnd*		m_Parent;
//------------------------------------------------------------------------
	int			Tnum,Mnum;
	int			CopyTimes[30];
	IXYCOORD	CopySet[30];
	int			Trow,Mrow;
	//
	CWnd*		CCC;
	//
	int*		No_ID;	// Hnum
	CStatic**	Nos;
	//
	int*		Times_ID;	// Vnum
	CEdit**		Times;
	//
	int*		MX_ID;		// Hnum
	CEdit**		MXs;
	//
	int*		MY_ID;		// Hnum
	CEdit**		MYs;
	//
	int*		App_ID;		// Hnum
	CEdit**		Apps;
//
//------------------------------------------------------------------------
	void		Search_Edit_Focas();
	//
	BOOL		Init_Set_Edits();
	//
	void		Free_All();
//
//------------------------------------------------------------------------

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAs();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonAddTime();
	afx_msg void OnBnClickedButtonDelTime();
	afx_msg void OnBnClickedButtonAddMesh();
	afx_msg void OnBnClickedButtonDelMesh();
	CStatic C_No;
	CStatic C_MV;
	CStatic C_MH;
	CStatic C_BT;
	CStatic C_AP;
};
