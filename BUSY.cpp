// BUSY.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RRI.h"
#include "BUSY.h"
#include "afxdialogex.h"

#include  "CommFuncs.h"
//#include "AS_Library\AS_Common.h"
//#include "AS_Library\COLButton.h"


// CBUSY ダイアログ

IMPLEMENT_DYNAMIC(CBUSY, CDialog)

CBUSY::CBUSY(CWnd* pParent /*=NULL*/)
	: CDialog(CBUSY::IDD, pParent)
{
	//  m_Title = _T("");
}

CBUSY::~CBUSY()
{
}
void CBUSY::OnDestroy()
{
	CDialog::OnDestroy();
}
void CBUSY::PostNcDestroy() {
	//delete this;
}


void CBUSY::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Prog);
}


BEGIN_MESSAGE_MAP(CBUSY, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CBUSY メッセージ ハンドラー


void CBUSY::Set_Prog(int st,int end) {
	m_Prog.SetRange( st,end);
	m_Prog.SetStep( 1 );
}
void	CBUSY::Set_Pos(int pos ) { m_Prog.SetPos( pos );}

void	CBUSY::Set_Title(CString str ) {
	this->SetWindowTextA( str );}

void CBUSY::Close()
{	DestroyWindow(); }


