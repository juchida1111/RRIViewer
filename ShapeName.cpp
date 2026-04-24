// ShapeName.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RRI.h"
#include "ShapeName.h"
#include "afxdialogex.h"

#include  "CommFuncs.h"
//_______________________________________________________________________________________________
/////////////////////////////////////////////////////////////////////////////////////////////////

int			SN_Num = 2;
int			SN_ID[] = {IDOK,IDCANCEL};
CString		SN_Text[] = {"Save","Cancel"};

// CShapeName ダイアログ

IMPLEMENT_DYNAMIC(CShapeName, CDialog)

CShapeName::CShapeName(CWnd* pParent /*=NULL*/)
	: CDialog(CShapeName::IDD, pParent)
{

	m_Name = _T("");
	//  m_Info = _T("");
	m_S_VERT = _T("");
	m_S_AREA = _T("");
	m_S_MESH = _T("");
}

CShapeName::~CShapeName()
{
}

void CShapeName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	//  DDX_Text(pDX, IDC_INFO, m_Info);
	DDX_Text(pDX, IDC_INFO, m_S_VERT);
	DDX_Text(pDX, IDC_INFO2, m_S_AREA);
	DDX_Text(pDX, IDC_INFO3, m_S_MESH);
}


BEGIN_MESSAGE_MAP(CShapeName, CDialog)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CShapeName::OnEnChangeEditName)
END_MESSAGE_MAP()




BOOL CShapeName::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShapeName::OnEnChangeEditName(){UDT;}
