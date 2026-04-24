//////////////////////////////////////////////////////////////////////////////////////////
// 
//      Art&Science  Standerd Main Library Header 
//
//													Update  2011/6
//
//////////////////////////////////////////////////////////////////////////////////////////
//                                                
#pragma once

#include	"stdafx.h"
#include	"stdlib.h"
#include	"stdio.h"
#include	"io.h"
#include	"fcntl.h"
#include	"io.h"
#include	"SYS\STAT.H "
#include	"math.h"
//
//  ƒvƒƒOƒ‰ƒ€‚Ì«Ši’è‹`
//
#define		_USE_SDI			////   ‚r‚c‚hŽg—p
//#define		_USE_DIALOG			////   ƒ_ƒCƒAƒƒOŽg—p
// ƒ{ƒ^ƒ“F‚Ì•ÏX
//#define		_USE_COLOR_DIALOG	//// ƒJƒ‰[ƒ_ƒCƒAƒƒO‚ðŽg—p
//
#define		_USE_CDC			//// ‚b‚c‚bƒRƒ“ƒgƒ[ƒ‹‚ðŽg—p
//
//
#define		_USE_IMAGE			////   ƒCƒ[ƒW‘€ì‚ðŽg—p
#define		_USE_JPEG			////   ‚i‚o‚d‚fƒ‰ƒCƒuƒ‰ƒŠ[‚ðŽg—p
//
//
//#define		_USE_OBJECT
//
//#define		_USE_DX				////   ‚c‚h‚q‚d‚b‚sQ‚w‚ðŽg—p
//
#define		_USE_OPENGL			////   ‚n‚o‚d‚mQ‚f‚k‚ðŽg—p
//
//                                                
//////////////////////////////////////////////////////////////////////////////////////////
// ƒh	440
// ƒŒ	494	
// ƒ~	554
// ƒtƒ@	587
// ƒ\	659
// ƒ‰	740
// ƒV	830
// ƒh	880
//
#ifdef  _USE_SDI
#define		RETURN(x)		{SB->SetPaneText(0,x,TRUE);return;}
#define		ERR_RETURN(x)		{Beep(494,100);Beep(850,100);SB->SetPaneText(0,x,TRUE);return;}
#define		COMMENT(x)			SB->SetPaneText(0,x,TRUE)
#define		ERR_COMMENT(x)	{Beep(494,100);Beep(850,100);SB->SetPaneText(0,x,TRUE);}
#define		NON_COMMENT(x)	;
//

#else

#define		RETURN(x)		m_Comment=x;UDF;return;
#define		COMMENT(x)		m_Comment=x;UDF;
#define		NON_COMMENT(x)		;
#endif

