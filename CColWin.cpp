//#include   "DataColor.h"
#include	"CColWin.h"
#include	"CommFuncs.h"

IMPLEMENT_DYNAMIC(CColWin, CWnd)

BEGIN_MESSAGE_MAP(CColWin, CWnd)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

CColWin::CColWin() 
{
	colground = RGB(0, 0, 0);				// #0x00,0x00,0x00
}

CColWin::~CColWin(){}

// ボタンをCCOLButtonアタッチにする
//---------------------------------------------------------------
BOOL CColWin::AutoLoad(const UINT nID, CWnd* pParent)
{
	if (!SubclassDlgItem(nID, pParent)) return false;
	return true;
}
//---------------------------------------------------------------
void CColWin::ChangeColor(COLORREF color)
{ 
	CWnd*		lpDIS(this);
	CDC*		pDC = lpDIS->GetDC();
	CRect		btnRect;
	lpDIS->GetClientRect( &btnRect );
	//
	colground = color;
	//
//	CBrush brush(color);
	pDC->FillSolidRect(&btnRect,color);
//	pDC->FillRect(btnRect, &brush);
	//
	lpDIS->ReleaseDC( pDC );
}
//---------------------------------------------------------------
void CColWin::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CColorDialog   CD;
	if( CD.DoModal() == IDCANCEL ) return;
	colground = CD.GetColor();
	ChangeColor( colground );

	CWnd::OnLButtonDblClk(nFlags, point);
}
