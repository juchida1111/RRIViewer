#pragma once
#include  "CommFuncs.h"
#include  "CommGL.h"
#include	"RRI_Common.h"
#include    "Resource.h"

#define		PICT_WIDE		1000
#define		PICT_HEIGHT		700

#define		SECTION_H_MARGIN	100	
#define		SECTION_VU_MARGIN	100	
#define		SECTION_VD_MARGIN	120	

typedef struct CalcParamVal {
	char x[19];
	char y[5];
} LPCalcParamVal;

typedef struct ObsWL {
	char	datetime[19];
	char	datetimeStr[30];
	char	kansokusuii[30];
	char	hanrankiken[5];
	char	hinan[5];
	char	hanranchui[5];
	char	suibodan[5];
} LPObsWL;

//プロトタイプ宣言
BOOL SelectFolder(HWND hWnd, LPCTSTR lpDefFolder, LPTSTR lpSelectPath, UINT iFlg, CString ttl);
int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

// CAccuracyEvaluation ダイアログ
class CAccuracyEvaluation :
	public CDialog
{
public:

	DECLARE_DYNAMIC(CAccuracyEvaluation)

	CAccuracyEvaluation(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CAccuracyEvaluation();

	// ダイアログ データ
	enum { IDD = IDD_ACCURACYEVALUATION };

	//------------------------------------------------------------------------
	CWnd*		m_Parent;
	CRect		Prect;
	int			WinH, WinW;
	//
	int			Hours, Tnum;
	int			timestep;
	CString		WinTitle;
	MINFO*		MESH;
	CTime*		ct;
	CTime		st;
	
	//
	int			xid, yid, vid, hid;  // ＲＲＩ座標に直してある。
									 //
	BOOL			isRiver;
	BOOL			Fig_Ready;		// ON_PAINTのため
									//
	GB_PARAMETER	GB;

	CString m_Hmax;
	CString m_Hmin;
	CString m_Qmax;
	CString m_Qmin;

	//------------------------------------------------------------------------
	void Make_Section();
	void Show_ObservationWaterLevel();
	void Show_SelectCalculatedLevel();
	void Show_ErrorRate();
	void getDateTimeStr(CTime ct, char str[128]);
	int CreatePltFile();
	int CreatePlot();
	int CAccuracyKadoSodoKobai();
	int CAccuracyObsWL();
	char * subStr(char * inputStr, int start, int end);
	char * getTimeFmr(time_t val);

	//------------------------------------------------------------------------

private:
	void UpdateStartDt();
	void UpdateEndDt();
	void CustomizeCtrls();
	void MoveCtrl(int srcID, int tarID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnEnChangeEditStartTime();
	afx_msg void OnEnChangeEditEndTime();
	void OnPaint();
	afx_msg void OnBnClickedButtonObservationWaterLevel();
	afx_msg void OnBnClickedButtonCalculationWatarLevel();
	afx_msg void OnBnClickedButtonErrorRate();
	afx_msg void OnBnClickedButtonDownload();
	afx_msg
	void CreatePlotCalcWL();
	void CreatePlotCalcQ();
	void CreatePlotCalcWL_RRI_Predicted();
	void DrawPicture(CString filePath);
	void OnStnClickedAccuracyPicture();
	afx_msg void OnStnClickedSectionPict();
	CStatic m_AccuracyPicture;
	afx_msg void OnEnChangeEditStarttimeY();
	afx_msg void OnBnClickedButtonCrossSec();
	afx_msg void OnBnClickedButtonRecalc();
	afx_msg void OnBnClickedButtonHqeq();
	afx_msg void OnBnClickedButtonPfPrediction();
	void createCalcWL_Data(char * startDt2, char * endDt2, int waterlvFlg);
	void createCalcQ_Data(char * startDt2, char * endDt2, int waterlvFlg);
	bool createCalcWLRRI_Predicted_Data(char * startDt2, char * endDt2, int waterlvFlg);
	void DispGraph(char * startDt2, char * endDt2);
};

