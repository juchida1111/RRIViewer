#pragma once
#include  "CommFuncs.h"
#include  "CommGL.h"
#include	"RRI_Common.h"
#include    "Resource.h"
#include "afxcmn.h"

#define		PICT_WIDE		1000
#define		PICT_HEIGHT		700

#define		SECTION_H_MARGIN	100	
#define		SECTION_VU_MARGIN	100	
#define		SECTION_VD_MARGIN	120	

// CAccuracyEvaluation ダイアログ
class CErrorRate :
	public CDialog
{
public:
	DECLARE_DYNAMIC(CErrorRate)

	CErrorRate(CWnd* pParent = NULL);   // 標準コンストラクター
	CErrorRate(CWnd * pParent, char * startdt, char * enddt, CString workDir, int selectedNo);
	virtual ~CErrorRate();

	// ダイアログ データ
	enum { IDD = IDD_ERRORRATE };

	//------------------------------------------------------------------------
	CWnd*		m_Parent;
	CRect		Prect;
	CString		WinTitle;
	CListCtrl   m_ErrorRateList;
	//------------------------------------------------------------------------
	
	//------------------------------------------------------------------------

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	void recreateDir(CString path);

public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	void createData();
	void createErrorData();
	void createDataSimpVer();
	void createErrorDataSimpVer();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnItemchangedErrorrateList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void AdjustLayout();
};

