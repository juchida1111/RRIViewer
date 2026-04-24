#pragma once
//
//  プログラムの性格定義
//
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define		VER_64					// 64bit Compile 
/// """""""""""" 
#define		_USE_SDI				////   ＳＤＩ使用
#define		_USE_VIEWER_FUNC			////  VIEWER  用 の拡張関数を使用する。
///___________________________________________________________________
//
#ifdef  _USE_SDI
#define		RETURN(x)		{SB->SetPaneText(0,x,TRUE);return;}
#define		ERR_RETURN(x)	{Beep(494,100);Beep(850,100);SB->SetPaneText(0,x,TRUE);return;}
#define		COMMENT(x)		{SB->SetPaneText(0,x,TRUE);}
#define		ERR_COMMENT(x)	{Beep(494,100);Beep(850,100);SB->SetPaneText(0,x,TRUE);}
#define		NON_COMMENT(x)	;

#else

#define		RETURN(x)		{m_Comment=x;UDF;return;}
#define		COMMENT(x)		{m_Comment=x;UDF;}
#define		NON_COMMENT(x)	;
#endif
//

#define		STAT(i)					Gp.State[i]
#define		pSTAT(i)				Gp->State[i]
#define		IF_STAT(i)				if( Gp.State[i] )
#define		IF_pSTAT(i)				if( Gp->State[i] )
#define		IFNOT_STAT(i)			if( !Gp.State[i] )
#define		IFNOT_pSTAT(i)			if( !Gp->State[i] )
//
#ifdef		_USE_VIEWER_FUNC
#define		STATG(i)				Gp.State[i]
#define		pSTATG(i)				Gp->State[i]
#define		IF_STATG(i)				if( Gp.State[i] )
#define		IF_pSTATG(i)			if( Gp->State[i] )
#define		IFNOT_STATG(i)			if( !Gp.State[i] )
#define		IFNOT_pSTATG(i)			if( !Gp->State[i] )

#endif
//
///□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//////////////////////////////////////////////////////////////////////////////////////////
///    オブジェクトレイヤー    プログラム使用範囲は 1－15
//
//------------------------------------------------------------- for APPLICATION LAYER

#define		MAX_OBJECT						20
#define		OBJID_SEARCH_AREA				3
//------------------------------------------------------------- for APPLICATION LAYER
#define		OBJID_BACK						1
#define		OBJID_LINE						2
#define		OBJID_WL						0

#define		OBJID_MARK						3
#define		OBJID_TIME						4
#define		OBJID_LEGEND					5
#define		OBJID_TOOL						6
#define		OBJID_LOCATION					7
//------------------------------------------------------------- for Time Char & Legend
#define		OBJID_COPYRIGHT					7
//------------------------------------------------------------- for PROGRAM
#define		OBJID_WORK						12
#define		OBJID_PROG_WORK					13
#define		OBJID_MOUSE_BAND				14
//------------------------------------------------------------- for Csection or Cprofile
#define		OBJID_BACKS						0
#define		OBJID_LINES						1
///
//////////////////////////////////////////////////////////////////////////////////////////
///========================================================================================
//
///    ＢＯＯＬＩＡＮ				プログラム使用範囲は 20-60
//
//---------------------------------------------------------- Program Setting
// 
#define		MAX_BOOLEAN					60				// 指定
//
// Reserved for Mouse & KeyBoard
//
#define		IS_READY					0
#define		MS_LB_DOWN					1
#define		MS_RB_DOWN					2
//
#define		MS_MOVE_START				4
#define		MS_RUBBER_START				5
//
#define		KEY_CTRL_DOWN				7
#define		KEY_SHIFT_DOWN				8
#define		KEY_ALT_DOWN				9
//
// Reserved for Viewing
//
#define		SET_WORLD_VIEW				10
#define		IS_PERSPECTIVE				11
//
// Render Contorol
#define		REDRAW						12
#define		RELIST						13		// GLobj ごとに指定
#define		REVIEW						14
#define		RESET_ATTR					15
//
// Reserved for Setting
//
#define		USE_FONT					18
#define		USE_LIGHT					19
//---------------------------------------------------------- Program Setting
// 
#define		ON_SCREEN					20
//
#define		DATA_SETTING_MODE			21

#define		SELECT_RIV_PATH				23
#define		DECIDE_RIV_PATH				24

#define		SET_MESHPROFILE				25
#define		SET_SHAPE					26
//
#define		RUN_ANIMATION				30
#define		RECORD_MODE					31
#define		HALT						32
//
#define		MOVE_DATE					33
#define		MOVE_LEGEND					34
#define		MOVE_SCREEN_TOOL			35

#define		DISP_EDITWIN				41
#define		DISP_BREAKTIME				42
#define		DISP_SECTION				43
#define		DISP_PROFILE				44
#define		DISP_MESHPROFILE			45

#define		SHOW_COLORDATA				48
//
//////////////////////////////////////////////////////////////////////////////////////////
//========================================================================================
//
//    GB オブジェクトレイヤー		 VIEWER SubWindowのための設定
//--------------------------------------
#ifdef _USE_VIEWER_FUNC
//--------------------------------------

#define		STATB(i)				GB.State[i]
#define		pSTATB(i)				Gp->State[i]
#define		IF_STATB(i)				if( GB.State[i] )
#define		IF_pSTATB(i)			if( Gp->State[i] )
#define		IFNOT_STATB(i)			if( !GB.State[i] )
#define		IFNOT_pSTATB(i)			if( !Gp->State[i] )
//
///
//
#define		GB_MAX_OBJECT				20
#define		GB_OBJID_SEARCH_AREA		6
#define		GB_MAX_BOOLEAN				100
//------------------------------------------------------------- for APPLICATION LAYER
#define		OBJID_FIG_0						0
#define		OBJID_FIG_1						1
#define		OBJID_FIG_2						2
#define		OBJID_FIG_3						3
//------------------------------------------------------------- for PROGRAM
#define		GB_OBJID_WORK					7
#define		GB_OBJID_PROG_WORK				8
#define		GB_OBJID_MOUSE_BAND				9
//------------------------------------------------------------- for Csection or Cprofile
//

#define		DISP_ACCURACYEVALUATION			50
#define		DISP_SELECTCALCULATEDLEVEL		51
#define		DISP_ERRORRATE					52

#endif