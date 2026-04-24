#pragma once


// CShapeName ダイアログ

#include  "RRIView.h"
#include  "CommFuncs.h"

class CShapeName : public CDialog
{
	DECLARE_DYNAMIC(CShapeName)

public:
	CShapeName(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CShapeName();

// ダイアログ データ
	enum { IDD = IDD_SHAPENAME };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CString m_Name;
//	CString m_Info;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditName();
	CString m_S_VERT;
	CString m_S_AREA;
	CString m_S_MESH;
};
