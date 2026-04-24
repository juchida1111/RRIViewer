#pragma once


// CProfile ダイアログ

#pragma once
#include  "CommFuncs.h"
#include  "CommGL.h"
#include	"RRI_Common.h"

//#define		PROFILE_WIDE		900
//#define		PROFILE_HEIGHT		600
//
#define		PROFILE_H_MARGIN	70	
#define		PROFILE_VU_MARGIN	50	
#define		PROFILE_VD_MARGIN	80	


class CProfile : public CDialogEx
{
	DECLARE_DYNAMIC(CProfile)

public:
	CProfile(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CProfile();

// ダイアログ データ
	enum { IDD = IDD_PROFILE };
//------------------------------------------------------------------------
	CString		WinTitle;
	CRect		Prect;				// 親のサイズ
	int			WinH,WinW;
	//
	BOOL		Data_Exist;
	CTime		CCT;
	//
	BOOL		All_New;
	double		Hmin,Hmax,Hspan;		// 標高
	double		Dmin,Dmax,Dspan;		// 距離
//
//	
	int*			No;
	FCOLOR		Fcol;
	CString		form;
//
	double		Hunit,stH,dH;
	double		tmin;

	int			Hrip,Hfm;

	int			Dnum;
	double		Dunit,dX;
	int			Drip,Dfm;
//------------------------------------------------------------------------
	void	Make_Time_Fig();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	virtual void OnCancel();
	afx_msg void OnPaint();
	CString m_Hmax;
	CString m_Hmin;
	afx_msg void OnEnChangeEditHmin();
	afx_msg void OnEnChangeEditHmax();
	afx_msg void OnBnClickedButtonRedraw();
};
