#pragma once
#include  "CommFuncs.h"
#include  "CommGL.h"
#include	"RRI_Common.h"
#include    "Resource.h""
#include "AccuracyEvaluation.h"

#define		PICT_WIDE		1000
#define		PICT_HEIGHT		700

#define		SECTION_H_MARGIN	100	
#define		SECTION_VU_MARGIN	100	
#define		SECTION_VD_MARGIN	120

class CSelectCalculatedLevel :
	public CDialog
{
public:
	DECLARE_DYNAMIC(CSelectCalculatedLevel)

	CSelectCalculatedLevel(CWnd* pParent = NULL);   // 標準コンストラクター
	CSelectCalculatedLevel(CWnd * pParent, char * startdt, char * enddt, int * val, CString obsWorkDir);
	CSelectCalculatedLevel(CWnd * pParent, char* startdt, char* enddt);
	~CSelectCalculatedLevel();

	// ダイアログ データ
	enum { IDD = IDD_SELECTCALCULATEDLEVEL };

	//------------------------------------------------------------------------
	CWnd*		m_Parent;
	CRect		Prect;
	int			WinH, WinW;
	CString		WinTitle;
	CString		InputFile;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnCalculatedLevel();
	afx_msg void OnCalculatedFlowRate();
	afx_msg void OnCalculatedLevelAssimilation();
	DECLARE_MESSAGE_MAP()
	afx_msg
		void init();
	void OnBnClickedButtonAccuracyevaluation1();
	void Show_CalcWaterLevel();
	void createCalcWL_Data();
	void createCalcWL_Data(char * startDt2, char * endDt2);
	void createCalcQ_Data(char * startDt2, char * endDt2);
	void createCalcWLRRI_Predicted_Data(char * startDt2, char * endDt2);
	void createCalcQ_Data();
	void createCalcWLRRI_Predicted_Data();
	void CAccuracyObsWL();
	char * subStr(char * inputStr, int start, int end);
	char * getTimeFmr(time_t val);
	afx_msg void OnBnClickedButtonAccuracyevaluation2();
	afx_msg void OnBnClickedButtonAccuracyevaluation3();
};

