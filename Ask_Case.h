#pragma once
#include "afxwin.h"


// CAsk_Case ダイアログ

//----------------------------------------------------------------------------　モード定義
#define		MODE_VIEW			0
#define		MODE_MODEL			1

#define		UPPER				0
#define		LOWER				1


class CAsk_Case : public CDialogEx
{
	DECLARE_DYNAMIC(CAsk_Case)

public:
	CAsk_Case(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CAsk_Case();

// ダイアログ データ
	enum { IDD = IDD_ASK_CASE };

//------------------------------------------------------------------------
	CString		CaseDir;
	CString		InputFile;
	//
	int			Valid_Proc;			// Change int
	//
//-----------------------------------------------------------------------------------------------------------
// Class  Function
//-----------------------------------------------------------------------------------------------------------  
	//static int AFXAPI BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	//BOOL		Button_Dir(CString* m_DirName,char* m_RecentPath );
//-----------------------------------------------------------------------------------------------------------
// Program  Function
//-----------------------------------------------------------------------------------------------------------  
	int			Read_Input_Data();
	void			Get_Full_Name(char* Dir_name,char* RelativePath,char* Full_name);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//afx_msg void OnBnClickedButtonCasedir();
	//afx_msg void OnBnClickedButtonInput();
	afx_msg void OnBnClickedOk();
	//CButton c_ProjDir;
	//CButton c_RRI;
	CButton c_OK;
	//CStatic c_Label;
	//CStatic c_Com;
	CStatic c_Com;
};
