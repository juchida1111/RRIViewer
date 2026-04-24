#pragma once

// CBUSY ダイアログ

class CBUSY : public CDialog
{
	DECLARE_DYNAMIC(CBUSY)

public:
	CBUSY(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CBUSY();

// ダイアログ データ
	enum { IDD = IDD_BUSY };

	CString		Wtitle;
	BOOL			eval;		// プログレスバーの有効無効

	void	Set_Prog(int st,int end); 
	void	Set_Pos(int pos );
	void	Set_Title(CString str);
	void	Close();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_Prog;
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
};
