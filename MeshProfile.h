#pragma once
#include  "CommFuncs.h"
#include  "CommGL.h"
#include	"RRI_Common.h"


#define		MESHPROFILE_H_MARGIN	50	
#define		MESHPROFILE_VU_MARGIN	50	
#define		MESHPROFILE_VD_MARGIN	80	
// CMeshProfile ダイアログ

class CMeshProfile : public CDialog
{
	DECLARE_DYNAMIC(CMeshProfile)

public:
	CMeshProfile(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CMeshProfile();
	//
	enum { IDD = IDD_MESHPROFILE };
//------------------------------------------------------------------------
	CString			WinTitle;
	CRect			Prect;
	int				WinH,WinW;
	//
	CTime			CCT;
	BOOL			Data_Exist;
	BOOL		Fig_Ready;		// ON_PAINTのため

	double		Hmin,Hmax,Hspan;		// 標高
	double		Dmin,Dmax,Dspan;		// 距離
	//
	int*		No;
	CString		form;
//
	double		Hunit,stH,dH;

	int			Hrip,Hfm;

	int			Dnum;
	double		Dunit,dX;
	int			Drip,Dfm;
//-----------------------------------------------------------------------------------------------------------
// Display Members  VIEWER
//-----------------------------------------------------------------------------------------------------------  
	int			Mnum,Mmem;
	IXYCOORD*	MID;
	CPoint*		Mpos;
	double*		DEM;
	double*		HWL;
//------------------------------------------------------------------------
	void		Make_Profile();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnDestroy();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_Hmax;
	CString m_Hmin;
	afx_msg void OnPaint();
	afx_msg void OnEnChangeEditHmin();
	afx_msg void OnEnChangeEditHmax();
	afx_msg void OnBnClickedButtonRedraw();
};
