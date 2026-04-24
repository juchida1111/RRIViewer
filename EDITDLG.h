#pragma once

#include "RRI_Common.h"


#define		ID_INIT		2000
#define		X_SIZE		65
#define		Y_SIZE		20
#define		X_MARGIN		30
#define		Y_MARGIN		30

// CEDITDLG ダイアログ

class CEDITDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CEDITDLG)

public:
	CEDITDLG(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CEDITDLG();

// ダイアログ データ
	enum { IDD = IDD_EDITDLG };

	CWnd*	m_Parent;
//
//------------------------------------------------------------------------
	int			Num,LDH,LDV;				// 左下編集枠の座標と方向増分
	int			Result,Data_Change_Cnt;
	int**		IDS;
	CEdit***		Edits;
	int**		NO;
	//
	int			CH, CV;		// OnColCTLで参照
//
//------------------------------------------------------------------------
//	void	Init_Frame_Size();
	void	Change_Frame_Size();
	void	Set_Values();
	void	Reflesh_Data();
//------------------------------------------------------------------------
//

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCancel();
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedExport();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
