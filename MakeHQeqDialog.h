#pragma once


// CMakeHQeqDialog ダイアログ

class CMakeHQeqDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMakeHQeqDialog)

public:
	CMakeHQeqDialog(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CMakeHQeqDialog();

	bool uniformflowFlg;
	CString crossSectionPath;
	CString saveHqeqPath;

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAKE_HQEQ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCrossSection();
	afx_msg void OnBnClickedOk();
	void SetTitle();
	int ExecUniform();
	int ExecHq();
	afx_msg void OnBnClickedSaveHqeq();
};
