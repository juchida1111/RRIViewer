#pragma once 
#include	"stdafx.h"

#ifndef ___CCOLWIN_H_
#define ___CCOLWIN_H_

class CColWin : public CWnd
{
	DECLARE_DYNAMIC(CColWin)
private:

public:								// コンストラクタ・デストラクタ
	CColWin();						// コンストラクタ
	virtual ~CColWin();				// デストラクタ
//---------------------------------------------------------------
	int				Stat;
	COLORREF	colground;			// 背景色
	//
	BOOL		AUTOBUTTON;
	BOOL		AutoStat;
//
// メンバ関数
	BOOL	AutoLoad(const UINT nID, CWnd* pParent);	// ボタンをCColorButtonにアタッチ
	void	ChangeColor(COLORREF color); 				// 背景色取得
//
//---------------------------------------------------------------
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
#endif
