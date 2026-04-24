#pragma once
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Common Libraly Header for OpenGL
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include    "CommFuncs.h"
//
////////
//
#include	<gl/gl.h>
#include	<gl/glu.h>
//
//
//  プログラムの性格定義    定義変更　2019/10
//
#ifndef		MAIN_CONF
#include	"Proj_Conf.h"		// Property のインクルードディレクトリで　＄(SolusionDir) を指示する。
#endif

/////////////////////////////////////////////////////////////////////////////////////////  MACROS
// 
// ＭＡＣＲＯＳ
//
///////////////////////////////////////////////////////////////////////////////////////// DEFINE 
//
#define		ID_PICT					1800


#define		FRAME_MARGIN			10
#define		MENU_MARGIN				260		// 注意＊＊＊
//
#define		DIM_2D					2
#define		DIM_3D					3
//
#define		ORTHO					0
#define		VIEW_XY					1
#define		VIEW_XZ					2
#define		VIEW_ZY					3

#define		PERS					1
#define		PERSPECTIVE				1
//
#define		MARGIN_RATE				0.1			// ５％
#define		MAX_LIGHT				5
//
// Font
#define		DEFAULT_SIZE			32
//
#define		VK_ALTKEY				18
//
#define		FONT_BASE				0
#define		FONT_LISTNUM			255
//
#define		MANAGE_ATTR				1
#define		MANAGE_COLOR			2
#define		MANAGE_POINT_COLOR		3
#define		MANAGE_TEX				4
//
#define		POLYLINE_SUBDEV			72
//
#define		ALL_OBJECTS				999
//
//
// ＣＯＯＲＤＩＮＡＴＥ Ｚ ＶＡＬＵＥ
#define		BACK_Z					-1.
#define		CHAR_Z					1.
#define		MESH_Z					1.
#define		LINE_Z					5.
#define		ARROW_Z					8.
#define		RIVER_Z					7.
#define		BOUNDARY_Z				8.

#define		SELECT_Z				15.
#define		TS_Z					10.
#define		MOUSE_Z					15.

#define		NORMAL_Z				0.
#define		LINE_SELECT_Z			0.7
//
#define		SELECT_OFFSET			1.f
//
//------------------------------------------------------------------------Program Unique
// 
#define		CIRCLE_SUBDEV			128
//
//--------------------------------------------------------------------------------------
//////
//////
//
///////////////////////////////////////////////////////////////////////////////////////// TYPEDEF
//
//---------------------------------------------------------------------------　ウィンドウ情報
#ifndef _WININFO
#define _WININFO
typedef	struct	_WININFO {
	BOOL				set;
	int					wWidth,wHeight;			// 全体スクリーン幅・高さ
	CRect				WinRect,WinCRect;			// ウィンドウ全体の大きさ、クライアントの大きさ
	int					cWidth,cHeight;				// 表示領域のスクリーン幅・高さ
	CWnd				*cw,*parentcw;			// 表示域、親ウィンドウ
	HWND				hWnd;
	//
	CStatic				Cpict;
	CRect				Crect;							// 表示領域
	CRect				Wrect;						// マウス感知ウィンドウ領域
	CRect				MAXrect;						// 最大表示領域 （初期設定画面の大きさ）
} WININFO;
#endif
//
//---------------------------------------------------------------------------　移動・回転・スケール
#ifndef _LOCAL_COORD
#define _LOCAL_COORD
//
#define		F_TRANS			1
#define		F_ROTATION		2
#define		F_SCALE			3
//
typedef	struct	_LOCAL_COORD {
	BOOL				set;
	float				offx,offy,offz;			// F_TRANS
	float				ang,rotx,roty,rotz;		// F_ROTATION 角度（度）rotはベクトル
	float				scx,scy,scz;				// F_SCALE
} LOCAL_COORD;
#endif
//
//---------------------------------------------------------------------------　カメラ情報
#ifndef _CAMERA_INFO
#define _CAMERA_INFO
typedef	struct	_CAMERA_INFO {
	BOOL			set;
	double			aspect;
	double			Dw,Dh;
	double			MinDh,MaxDh;
	// 主にパース
	double			viewang;							// 視野角（度）
	double			Near,Far;
	//
	double			cx,cy,cz;							// カメラ位置
	double			tx,ty,tz;							// 注視点
	// PERSPECTIVE の場合の回転角
	double			radius,angH,angV;				// 球体半径とH:θ＝ｘｚ平面、V:φ＝Y方向
	XYZCOORD		viewdir;
	//
	double			stH,endH,stV,endV;				// 画面サイズ　ＯＲＴＨＯ
	double			xmin,ymin,zmin,xmax,ymax,zmax; // オブジェクトデータ
	//
} CAMERA_INFO;
#endif
//
//--------------------------------------------------------------------------　ライト情報
#ifndef _LIGHT_INFO
#define _LIGHT_INFO
//
typedef	struct	_LIGHT_INFO {
	BOOL		set;
	float		pos[4];					// x,y,z, w -> x/w, y/w, z/w  w=0:平行光源
	float		amb[4],dif[4],spc[4];	// r,g,b,a
} LIGHT_INFO;
#endif
//
//--------------------------------------------------------------------------　マテリアル情報
#ifndef _MATERIAL_INFO
#define _MATERIAL_INFO
//
typedef	struct	_MATERIAL_INFO {
	BOOL		set;
	float		amb[4],dif[4],spc[4];		// r,g,b,a
	float		emission[4],shininess[1];	// emission 4×[0,1] shininess １×[0,128]
} MATERIAL_INFO;
#endif
//
//--------------------------------------------------------------------------　テクスチャー情報
#ifndef _TEX_INFO
#define _TEX_INFO
//
typedef	struct	_TEX_INFO {
	BOOL			set;
	GLuint			texID;
	char			fname[64];
	int				width,height;			// オリジナル幅・高さ
	int				magW,magH;
	int				subdev;
	int				image_plane;			// イメージプレーン数 １，３、４
	//
	int				TresH,TresV;		// 表示するテクスチャーの解像度
	BYTE*			image;				//  texw*texh*image_plane
} TEX_INFO;
#endif
//
//--------------------------------------------------------------------------　ポリゴン情報
#ifndef _POLYDEF
#define _POLYDEF
//
#define		FONT_REGULAR	1		// for Fstyle
#define		FONT_BOLD		2
#define		FONT_ITALIC		3
//
typedef	struct	_POLYDEF {
	BOOL				set;				// データそのものがセットされているか
	BOOL				display;			// 表示するかどうか
	BOOL				both;				// 両面かどうか
	BYTE				flag;				// 有効(FLAG_ON)・無効(FLAG_OFF)
	BYTE				dim;				// DIM_2D,DIM_3D
	//
	int					kind;
	int					num;
	int*				no;
	//
	LOCAL_COORD			local;
	//
	float				Alpha;
	//
	int					manage;				// 属性管理:０、色管理：１、テクスチャー管理：２、ポイント色：３
	FCOLOR				col;
	char				attr[SIZE_006K];
	int					attr_no;
	//
	float				psize;			// ポイントサイズ、ラインの幅
	float				lwidth;			// ラインの太さ
	// TEXT
	BOOL				Fcontinue;		// フォントが同じ
	char*				FontName;
	int					Fstyle,fw,fh;	// スタイル、幅、高さ
	char*				str;			// 現在 ASCII コードのみ
	//
	//// Texture
	BOOL				texed;
	int					texno;
	FXYCOORD*			uv;				// no[num] と同じ
	// Display List
	BOOL				reList;
	BOOL				isList;
	int					ListNo;
	//
	float				normal[3];
	double				xmin,xmax,ymin,ymax,zmin,zmax;
	int					imemo[3];		// 予備情報
	double				dmemo[3];		// 予備情報
} POLYDEF;
//
#endif
//
//------------------------------------------------------------------------------
// GL OBJECT
//------------------------------------------------------------------------------
//
#define		GL_BOX							0x0010
#define		GL_TEXT							0x0100		// 注意
#define		GL_NAS							0x0200
//
////////////////////////////////////////////////////////////////////////////////////
//  SOLID TYPE  kind = GL_NAS の場合 imemo[0] に格納
//
#define			NAS_31		1		// ３角錐
#define			NAS_33		2		// ３角柱
#define			NAS_44		3		// ４角柱
//
////////////////////////////////////////////////////////////////////////////////////
//  
//
#ifndef _GL_OBJECT
#define _GL_OBJECT
//
//
typedef	struct	_GL_OBJECT {
	BOOL					set,display;
	BOOL					select;			// 選択されているか
	BOOL					Frame;			// ＲＲＩ：浸水深のBOXフレームを表示するかどうか
	BYTE					flag;				// 有効(FLAG_ON)・無効(FLAG_OFF)
	//
	LOCAL_COORD				local;
	// Attribute
	short					pattern;			// 線種パターン
	FCOLOR					fcol;				// 全体統一の色
	//
	// vert
	int						vnum,vmem;
	XYZCOORD*				vert;
	BOOL					vcolset;			// ポイントの色が設定されているかどうか。
	FCOLOR*					vcol;
	float*					valpha;
	//
	// Poly
	MATERIAL_INFO			mat;				// 同じ材質のものを１つのオブジェクトとする。
	int						pnum,pmem;
	POLYDEF*				poly;
	//
	char					memo[24];
	double					xmin,xmax,ymin,ymax,zmin,zmax;
	//
} GL_OBJECT;
//
#endif

//------------------------------------------------------------------------------
// GL PARAMETER
//------------------------------------------------------------------------------
#ifndef _GL_PARAMETER
#define _GL_PARAMETER
typedef	struct	_GL_PARAMETER {
	//初期値　---------------------------------------------
	BOOL					State[MAX_BOOLEAN];		// ＊すべてのフラッグ
	char					m_CurrentPath[SIZE_05K];	// 変えてはいけない　#####
	char					m_RecentPath[SIZE_05K];	// 変えてはいけない　#####
	//
	// システム設定------------------------------------------------------ 
	GLubyte					*renderer,*version,*vendor;
	int						Mlimit,CPUnum;
	//
	// ウィンドウ設定--------------------------------------------------------
	WININFO					win;						// ウィンドウ情報
	PIXELFORMATDESCRIPTOR	pfd;			// ピクセルフォーマット
	int						pfdID;
	HGLRC					hRCT;						// レンダリングコンテキスト
	CDC*					pDC;						// デバイスコンテキスト
	//
	// ViewPort 設定--------------------------------------------------------
	//
	int						Dimension;					// 2,or 3 Dimension
	FCOLOR					Bcolor;					// 背景色
	int						view;						// Model 1,2 のCV値の保存
	int						ViewDir;					// 追加 201511 VIEW_XY,XZ,ZY
	CAMERA_INFO				cam[2];					// カメラ情報 モデル別、ORTHO,PERS別
	CAMERA_INFO				Homecam[2];			// 初期データ
	//
	// ライト設定-------------------------------------------------------- State USE_LIGHT
	int						Lnum;						// ライトの数
	LIGHT_INFO*				LT;						// ライト構造体
	//
	// テクスチャーデータ設定-----------------------------------------------------------
	int						Max_Res;					// イメージの最大幅、高さ　
	int						SUBDEV_BASE;
	//
	BOOL					SetTex;					// テクスチャーがセットされているか
	int						Texmem;
	int						Texnum;					// テクスチャーの数
	int						Disp_Size;				// 表示テクスチャーの画像解像度 1,2,4
	TEX_INFO*				Tex;						// テクスチャー構造体
	//
	BOOL					isGray;					// モノクロで処理するか、カラーか？
	//
	// オブジェクト設定------------------------------------------------------
	//
	int						STnum;					// 選択個数
	int						Gmax;						// MAX_OBJECT
	GL_OBJECT*				Gobj;
	//
	// 属性管理------------------------------------------------------
	//
	int						AttrNum,AttrMem;		// 属性数
	int						AttrPage;				// ページ数
	char					**AttrList;				// 属性リスト
	FCOLOR*					ATcol;					// 属性カラー
	BOOL*					Valid;						// 属性表示、非表示
	int						Select_No;				// 選択属性の番号
	FCOLOR					Frame_fcol;				// ＲＲＩ：フレームカラー
	//
	// フォント設定------------------------------------------------------ State USE_FONT
	//
	CFont*					OldFont;
	int						FontBase;
	//
	// データ設定------------------------------------------------------ 
	//
	CString					ImageFile;
	double					imgWspan,imgHspan;	// イメージの実サイズ( mm )
	int						resW,resH;				// メイン画像サイズ
	//
	// 画面保存設定------------------------------------------------------ 
	//
	int						Hcopy_cnt;				// ハードコピーカウント
//
} GL_PARAMETER;
#endif
//
//--------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////// 
//
/////////////////////////////////////////////////////////////////////////////////////////// ＦＵＮＣＴＩＯＮ
//--------------------------------------------------------------------------------------------------
//
// Start & End
HRESULT		GL_Init(GL_PARAMETER* Gp);
HRESULT		GL_Term( GL_PARAMETER* Gp);
HRESULT		GL_Attrib( GL_PARAMETER* Gp,int Dim,int View,int ViewDir,FCOLOR Bcolor,int LightNum);

void			GL_View2D(GL_PARAMETER* Gp); 
void			GL_View3D();
//
///////////////////////////////////////////////////////////////////		フォント管理
//--------------------------------------------------------------------------------------------------
//
HRESULT		GL_Init_Font(GL_PARAMETER* Gp);
HRESULT		GL_Change_Font(GL_PARAMETER* Gp,char* FontName,int Style,int Height,int Width);
//
int				GLobj_Add_Text(GL_PARAMETER* Gp,int	 ID,BYTE Dim,char* str,char* Fname,int Style,
								int fh,int fw,double x,double y,double z,FCOLOR col);
int				GLobj_Add_Text(GL_PARAMETER* Gp,int ID,BYTE Dim,char* str,double x,double y,double z);
int				GLobj_Change_Text(GL_PARAMETER* Gp,int ID,int no, char* str);
//
///////////////////////////////////////////////////////////////////		レンダー関連
//--------------------------------------------------------------------------------------------------
//
/* 
	GL_Render_Scene が基本。他は意識する必要はない。
	描画するには　STAT（REDRAW) をコントロールする。
*/
HRESULT		GL_Render_Scene( GL_PARAMETER* Gp );
HRESULT		GLobj_Render( GL_PARAMETER* Gp,int no );
HRESULT		GLobj_Make_DispList( GL_PARAMETER* Gp,int no,int pno );
HRESULT		GLobj_Render_Text( GL_PARAMETER* Gp,int no,int pno );
void			GL_Redraw( GL_PARAMETER* Gp,BOOL world,BOOL review,BOOL relist,int ID,BOOL redraw );
void			GL_Relist( GL_PARAMETER* Gp,int Gid,int Pid);
void			GL_Clear( GL_PARAMETER* Gp );
//
///////////////////////////////////////////////////////////////////		ビュー管理
//--------------------------------------------------------------------------------------------------
//
/* 
	GL_Get_ViewWorld は最初の全体視野を決めるときだけ。
	Gp.cam.set が変更のサイン
*/
HRESULT		GL_Get_ViewWorld( GL_PARAMETER*	Gp );
HRESULT		GL_Set_ViewPort( GL_PARAMETER*	Gp );
BOOL		GL_Get_Coord(GL_PARAMETER*  Gp,CPoint point,double* X,double* Y);
//
//
///////////////////////////////////////////////////////////////////		ライト管理
//--------------------------------------------------------------------------------------------------
//
/* 
	ライトの数 Lno（０－７）だけ GL_Set_Light　を定義
*/
HRESULT		GL_Set_Light( GL_PARAMETER*	Gp,int Lno,int KIND,float v1,float v2,float v3,float v4 );
//
///////////////////////////////////////////////////////////////////		ＴＥＸ 管理
//--------------------------------------------------------------------------------------------------
//
/* 
	GL_Tex_Init でTexnum だけ確保する。
	GL_Set_TextureでTexID番号とファイル名をバインドする。
	GLobj_Add_Poly（拡張）で使用するtexID番号を指定する。
*/
HRESULT		GL_Tex_Init( GL_PARAMETER*  Gp,int Texmem);
HRESULT		GL_Tex_Resize( GL_PARAMETER*  Gp,int Texmem);
int				GL_Set_Texture( GL_PARAMETER*  Gp, CString fname,int image_plane );

int				GL_Set_Bitmap( GL_PARAMETER*  Gp,int Width,int Height,int Colbyte,int Disp_Size,BYTE* bmpdata );
int				GL_Set_Bitmap( int no,GL_PARAMETER*  Gp,int Width,int Height,
												int Colbyte,int Disp_Size,BYTE* bmpdata );
int				GL_Set_Bitmap( int no,GL_PARAMETER*  Gp,int Width,int Height,
												int Colbyte,int Disp_Size,BYTE* bmpdata,BOOL isGray );
//
///////////////////////////////////////////////////////////////////		画面の保存
//--------------------------------------------------------------------------------------------------
//
HRESULT		GL_Save_Window(GL_PARAMETER* Gp );
HRESULT		GL_Save_Window(GL_PARAMETER* Gp,CString	fname);
//
//--------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------
//   オブジェクト操作  FUNCTION
//--------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------
/* 
	GL_OBJECT の基本操作
*/
HRESULT		GLobj_Init( GL_OBJECT* Gobj,int vmem,int pmem);
void		GLobj_Clear( GL_OBJECT* Gobj);
HRESULT		GLobj_Delete(GL_PARAMETER* Gp,int ID);
HRESULT		GLobj_Resize( GL_OBJECT* Gobj,int vmem,int pmem);
HRESULT		GLobj_Copy( GL_OBJECT* tobj,GL_OBJECT* obj,BOOL vcopy,BOOL pcopy);
//
//
///////////////////////////////////////////////////////////////////		ポイント管理
//--------------------------------------------------------------------------------------------------
//
int			GLobj_Add_Vert(GL_OBJECT* obj,double x,double y,double z,BOOL check);
int			GLobj_Add_Vert(GL_OBJECT* obj,double x,double y,double z,BOOL check,FCOLOR col,double Alpha);
//
int			GLobj_Set_Vert(GL_OBJECT* obj,int vno,double x,double y,double z);
int			GLobj_Search_Vert(GL_OBJECT* obj,double x,double y,double z);
int			GLobj_Delete_Vert(GL_PARAMETER* Gp,int ID);  // 未使用・重複点を整理
//
///////////////////////////////////////////////////////////////////		ポリゴン管理
//--------------------------------------------------------------------------------------------------
//
			/* 属性管理ポリゴン　*/
int			GLobj_Add_Poly(GL_PARAMETER*  Gp,int id,int num,int* no,int kind,char* attr);

			/* 色管理ポリゴン　おもに点・線　*/
int			GLobj_Add_Poly(GL_OBJECT* obj,int num,int* no,int kind,float val);		  // ポイント色を使用
int			GLobj_Add_Poly(GL_OBJECT* obj,int dim,int num,int* no,int kind,float val);// ポイント色を使用
//
int			GLobj_Add_Poly(GL_OBJECT* obj,int num,int* no,int kind,FCOLOR col);		// デフォルト val = 1.;
int			GLobj_Add_Poly(GL_OBJECT* obj,int num,int* no,int kind,float val,FCOLOR col);
int			GLobj_Add_Poly(GL_OBJECT* obj,int dim,int num,int* no,int kind,float val,FCOLOR col);

			/* テクスチャー管理ポリゴン　*/
int			GLobj_Add_Poly(GL_OBJECT* obj,int num,int* no,int kind,int texno,FXYCOORD* uv);
int			GLobj_Add_Poly(GL_OBJECT* obj, int dim, int num, int* no, int kind, int texno, FXYCOORD* uv);

int			GLobj_Add_Poly(GL_PARAMETER*  Gp, int id, int num, int* no, int kind, CString fname,
						FXYCOORD* uv,int image_plane);
int			GLobj_Add_Poly(GL_PARAMETER*  Gp,int id,int num,int* no,int H,int V,int image_plane,BYTE* image);
//
//  ポリゴン　削除・コピー
int			GLobj_Remove_Poly(GL_OBJECT* obj,int pno);				// 指定したポリゴンを除去　整列はしない。
int			GLobj_Delete_Poly(GL_OBJECT* obj,int pno);				// ポリゴン除去後、整列を行う。
int			GLobj_Delete_OffPoly(GL_PARAMETER* Gp,int ID);
HRESULT		GLobj_Copy_Poly(GL_OBJECT* tobj,int tno,GL_OBJECT* obj,int pno);
//
///////////////////////////////////////////////////////////////////		属性管理
//--------------------------------------------------------------------------------------------------
HRESULT		GLobj_Set_Material( GL_OBJECT*	obj,int KIND,float v1,float v2,float v3,float v4 );
//
int			Set_Attr_List( GL_PARAMETER* Gp,char*	attr);
int			Set_Attr_List( GL_PARAMETER* Gp,char*	attr,int red,int green,int blue);
int			Get_Attr_No(GL_PARAMETER* Gp,char*	attr);
int			Get_Attr_Color(GL_PARAMETER* Gp,char*	attr,FCOLOR* col);
int			Get_Attr_Color(GL_PARAMETER* Gp,int ano,FCOLOR col);
//
///////////////////////////////////////////////////////////////////　移動・回転・スケール
//--------------------------------------------------------------------------------------------------
HRESULT		GLobj_Change_Rotation( GL_PARAMETER*	Gp,int No,float v1,float v2,float v3 );
HRESULT		GLobj_Change_Scale( GL_PARAMETER*		Gp,int No,float v1,float v2,float v3 );
HRESULT		GLobj_Change_Transfer( GL_PARAMETER*	Gp,int No,float v1,float v2,float v3 );
//
///////////////////////////////////////////////////////////////////　座標変換
//--------------------------------------------------------------------------------------------------
//  TRANS
BOOL			GL_Trans_Pos( GL_PARAMETER* Gp,CPoint pt,double* mx,double* my);
BOOL			GL_Get_Coord( GL_PARAMETER* Gp,CPoint pt,XYZCOORD* xyz);
BOOL			GL_Trans_3D(double x,double y,double z,XYZCOORD ang,XYZCOORD tran,double *X,double *Y,double *Z);
//
///////////////////////////////////////////////////////////////////　選択
//--------------------------------------------------------------------------------------------------
//  SELECT OBJECT
//  
int				GLobj_Select_Ortho(GL_PARAMETER*  Gp,int ID,CPoint pt,BOOL bpoly,BOOL bline,BOOL bpos,int* Kind);
//
//---------------------------------------------------------------------------------------------------
// Utility
void			Change_Cursor( GL_PARAMETER* Gp,LPSTR Cid );
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
/*


//
//		Libraly Source for VIEWER
//



*/
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//
#ifdef _USE_VIEWER_FUNC
//--------------------------------------------------------------------------------------
//
#define		GB_BOX							0x0010
#define		GB_TEXT							0x0100		// 注意
#define		GB_NAS							0x0200
//
//
#ifndef _GB_OBJECT
#define _GB_OBJECT
//
//#define		GB_TEXT							0x000a
//
typedef	struct	_GB_OBJECT {
	BOOL				set,display;
	BOOL				select;			// 選択されているか
	BOOL				Frame;			// ＲＲＩ：浸水深のBOXフレームを表示するかどうか
	BYTE				flag;			// 有効(FLAG_ON)・無効(FLAG_OFF)
	//
	LOCAL_COORD	local;
	// Attribute
	short				pattern;			// 線種パターン
	FCOLOR				fcol;			// 全体統一の色
	//
	// vert
	int					vnum,vmem;
	XYZCOORD*			vert;
	BOOL				vcolset;			// ポイントの色が設定されているかどうか。
	FCOLOR*				vcol;
	float*				valpha;
	//
	// Poly
	MATERIAL_INFO		mat;				// 同じ材質のものを１つのオブジェクトとする。
	int					pnum,pmem;
	POLYDEF*			poly;
	//
	char				memo[24];
	double				xmin,xmax,ymin,ymax,zmin,zmax;
	//
} GB_OBJECT;
//
#endif

//------------------------------------------------------------------------------
// GB PARAMETER
//------------------------------------------------------------------------------
#ifndef _GB_PARAMETER
#define _GB_PARAMETER
typedef	struct	_GB_PARAMETER {
	//初期値　---------------------------------------------
	BOOL						State[GB_MAX_BOOLEAN];			// ＊すべてのフラッグ
	char						m_CurrentPath[SIZE_05K];			// 変えてはいけない　#####
	//
	// システム設定------------------------------------------------------ 
	GLubyte						*renderer,*version,*vendor;
	int							Mlimit,CPUnum;
	//
	// ウィンドウ設定--------------------------------------------------------
	WININFO						win;			// ウィンドウ情報
	PIXELFORMATDESCRIPTOR		pfd;	// ピクセルフォーマット
	int							pfdID;
	HGLRC						hRCT;				// レンダリングコンテキスト
	CDC*						pDC;				// デバイスコンテキスト
	//
	// ViewPort 設定--------------------------------------------------------
	//
	int							Dimension;		// 2,or 3 Dimension
	FCOLOR						Bcolor;			// 背景色
	int							view;				 
	CAMERA_INFO					cam;				// カメラ情報 モデル別
	//
	// ライト設定-------------------------------------------------------- State USE_LIGHT
	int							Lnum;				// ライトの数
	LIGHT_INFO*					LT;				// ライト構造体
	//
	// テクスチャーデータ設定-----------------------------------------------------------
	int							Max_Res;			// イメージの最大幅、高さ　
	int							SUBDEV_BASE;
	//
	BOOL						SetTex;			// テクスチャーがセットされているか
	int							Texmem;
	int							Texnum;			// テクスチャーの数
	int							Disp_Size;		// 表示テクスチャーの画像解像度 1,2,4
	TEX_INFO*					Tex;				// テクスチャー構造体
	//
	BOOL						isGray;			// モノクロで処理するか、カラーか？
	//
	// オブジェクト設定------------------------------------------------------
	//
	int							STnum;					// 選択個数
	int							Gmax;					// MAX_OBJECT
	GB_OBJECT*					Gobj;
	//
	// 属性管理------------------------------------------------------
	//
	int							AttrNum,AttrMem;		// 属性数
	int							AttrPage;				// ページ数
	char						**AttrList;				// 属性リスト
	FCOLOR*						ATcol;					// 属性カラー
	BOOL*						Valid;						// 属性表示、非表示
	int							Select_No;				// 選択属性の番号
	FCOLOR						Frame_fcol;				// ＲＲＩ：フレームカラー
	//
	// フォント設定------------------------------------------------------ State USE_FONT
	//
	CFont*						OldFont;
	int							FontBase;
	//
	// データ設定------------------------------------------------------ 
	//
	CString						ImageFile;
	double						imgWspan,imgHspan;	// イメージの実サイズ( mm )
	int							resW,resH;				// メイン画像サイズ
	//
	// 画面保存設定------------------------------------------------------ 
	//
	int							Hcopy_cnt;			// ハードコピーカウント
//
} GB_PARAMETER;
#endif
//
//--------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////// 
//
/////////////////////////////////////////////////////////////////////////////////////////// ＦＵＮＣＴＩＯＮ
//--------------------------------------------------------------------------------------------------
//
// Start & End
HRESULT		GB_Init(GB_PARAMETER* Gp);
HRESULT		GB_Term( GB_PARAMETER* Gp);
HRESULT		GB_Attrib( GB_PARAMETER* Gp,int Dim,int View,FCOLOR Bcolor,int LightNum);

void			GB_View2D(GB_PARAMETER* Gp); 
void			GB_View3D();
//
///////////////////////////////////////////////////////////////////		フォント管理
//--------------------------------------------------------------------------------------------------
//

HRESULT		GB_Init_Font(GB_PARAMETER* Gp);
HRESULT		GB_Change_Font(GB_PARAMETER* Gp,char* FontName,int Style,int Height,int Width);
//
int				GBobj_Add_Text(GB_PARAMETER* Gp,int	 ID,BYTE Dim,char* str,char* Fname,int Style,
							int fh,int fw,double x,double y,double z,FCOLOR col);
int				GBobj_Add_Text(GB_PARAMETER* Gp,int ID,BYTE Dim,char* str,double x,double y,double z);
int				GBobj_Change_Text(GB_PARAMETER* Gp,int ID,int no, char* str);
//
///////////////////////////////////////////////////////////////////		レンダー関連
//--------------------------------------------------------------------------------------------------
//
/* 
	GB_Render_Scene が基本。他は意識する必要はない。
	描画するには　STAT（REDRAW) をコントロールする。
*/
HRESULT		GB_Render_Scene( GB_PARAMETER* Gp );
HRESULT		GBobj_Render( GB_PARAMETER* Gp,int no );
HRESULT		GBobj_Make_DispList( GB_PARAMETER* Gp,int no,int pno );
HRESULT		GBobj_Render_Text( GB_PARAMETER* Gp,int no,int pno );
void			GB_Redraw( GB_PARAMETER* Gp,BOOL world,BOOL review,BOOL relist,int ID,BOOL redraw );
void			GB_Relist( GB_PARAMETER* Gp,int Gid,int Pid);
void			GB_Clear( GB_PARAMETER* Gp );
//
///////////////////////////////////////////////////////////////////		ビュー管理
//--------------------------------------------------------------------------------------------------
//
/* 
	GB_Get_ViewWorld は最初の全体視野を決めるときだけ。
	Gp.cam.set が変更のサイン
*/
HRESULT		GB_Get_ViewWorld( GB_PARAMETER*	Gp );
HRESULT		GB_Set_ViewPort( GB_PARAMETER*	Gp );
BOOL		GB_Get_Coord(GB_PARAMETER*  Gp,CPoint point,double* X,double* Y);
//
//
///////////////////////////////////////////////////////////////////		ライト管理
//--------------------------------------------------------------------------------------------------
//
/* 
	ライトの数 Lno（０－７）だけ GB_Set_Light　を定義
*/
HRESULT		GB_Set_Light( GB_PARAMETER*	Gp,int Lno,int KIND,float v1,float v2,float v3,float v4 );
//
///////////////////////////////////////////////////////////////////		ＴＥＸ 管理
//--------------------------------------------------------------------------------------------------
//
HRESULT		GB_Tex_Init( GB_PARAMETER*  Gp,int Texmem);
HRESULT		GB_Tex_Resize( GB_PARAMETER*  Gp,int Texmem);
int				GB_Set_Texture( GB_PARAMETER*  Gp, CString fname,int image_plane );
int				GB_Set_Bitmap( GB_PARAMETER*  Gp,int Width,int Height,int Colbyte,int Disp_Size,BYTE* bmpdata );
int				GB_Set_Bitmap( int no,GB_PARAMETER*  Gp,int Width,int Height,
							int Colbyte,int Disp_Size,BYTE* bmpdata );
int				GB_Set_Bitmap( int no,GB_PARAMETER*  Gp,int Width,int Height,
							int Colbyte,int Disp_Size,BYTE* bmpdata,BOOL isGray );
//
///////////////////////////////////////////////////////////////////		画面の保存
//--------------------------------------------------------------------------------------------------
//
HRESULT		GB_Save_Window(GB_PARAMETER* Gp );
HRESULT		GB_Save_Window(GB_PARAMETER* Gp,CString	fname);
//
//--------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------
//   オブジェクト操作  FUNCTION
//--------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------
/* 
	GB_OBJECT の基本操作
*/
HRESULT		GBobj_Init( GB_OBJECT* Gobj,int vmem,int pmem);
void		GBobj_Clear( GB_OBJECT* Gobj);
HRESULT		GBobj_Delete(GB_PARAMETER* Gp,int ID);
HRESULT		GBobj_Resize( GB_OBJECT* Gobj,int vmem,int pmem);
HRESULT		GBobj_Copy( GB_OBJECT* tobj,GB_OBJECT* obj,BOOL vcopy,BOOL pcopy);
//
//
///////////////////////////////////////////////////////////////////		ポイント管理
//--------------------------------------------------------------------------------------------------
//
int				GBobj_Add_Vert(GB_OBJECT* obj,double x,double y,double z,BOOL check);
int				GBobj_Add_Vert(GB_OBJECT* obj,double x,double y,double z,BOOL check,FCOLOR col,double Alpha);
//
int				GBobj_Set_Vert(GB_OBJECT* obj,int vno,double x,double y,double z);
int				GBobj_Search_Vert(GB_OBJECT* obj,double x,double y,double z);
int				GBobj_Delete_Vert(GB_PARAMETER* Gp,int ID);  // 未使用・重複点を整理
							// delpos : TRUE ポリゴン内の重複ポイント整理、FALSE : 整理しない
//
///////////////////////////////////////////////////////////////////		ポリゴン管理
//--------------------------------------------------------------------------------------------------
//
			/* 属性管理ポリゴン　*/
int				GBobj_Add_Poly(GB_PARAMETER*  Gp,int id,int num,int* no,int kind,char* attr);

			/* 色管理ポリゴン　おもに点・線　*/
int				GBobj_Add_Poly(GB_OBJECT* obj,int num,int* no,int kind,float val);		  // ポイント色を使用
int				GBobj_Add_Poly(GB_OBJECT* obj,int dim,int num,int* no,int kind,float val);// ポイント色を使用
//
int				GBobj_Add_Poly(GB_OBJECT* obj,int num,int* no,int kind,FCOLOR col);		// デフォルト val = 1.;
int				GBobj_Add_Poly(GB_OBJECT* obj,int num,int* no,int kind,float val,FCOLOR col);
int				GBobj_Add_Poly(GB_OBJECT* obj,int dim,int num,int* no,int kind,float val,FCOLOR col);

			/* テクスチャー管理ポリゴン　*/
int				GBobj_Add_Poly(GB_OBJECT* obj,int num,int* no,int kind,int texno,FXYCOORD* uv);
int				GBobj_Add_Poly(GB_PARAMETER*  Gp,int id,int num,int* no,int kind,CString fname,
						FXYCOORD* uv,int image_plane);
int				GBobj_Add_Poly(GB_PARAMETER*  Gp,int id,int num,int* no,int H,int V,int image_plane,BYTE* image);
//
//  ポリゴン　削除・コピー
int				GBobj_Remove_Poly(GB_OBJECT* obj,int pno);				// 指定したポリゴンを除去　整列はしない。
int				GBobj_Delete_Poly(GB_OBJECT* obj,int pno);				// ポリゴン除去後、整列を行う。
int				GBobj_Delete_OffPoly(GB_PARAMETER* Gp,int ID);
HRESULT			GBobj_Copy_Poly(GB_OBJECT* tobj,int tno,GB_OBJECT* obj,int pno);
//
///////////////////////////////////////////////////////////////////		属性管理
//--------------------------------------------------------------------------------------------------
HRESULT			GBobj_Set_Material( GB_OBJECT*	obj,int KIND,float v1,float v2,float v3,float v4 );
//
int				Set_Attr_List( GB_PARAMETER* Gp,char*	attr);
int				Set_Attr_List( GB_PARAMETER* Gp,char*	attr,int red,int green,int blue);
int				Get_Attr_No(GB_PARAMETER* Gp,char*	attr);
int				Get_Attr_Color(GB_PARAMETER* Gp,char*	attr,FCOLOR* col);
int				Get_Attr_Color(GB_PARAMETER* Gp,int ano,FCOLOR col);
//
///////////////////////////////////////////////////////////////////　移動・回転・スケール
//--------------------------------------------------------------------------------------------------
HRESULT		GBobj_Change_Rotation( GB_PARAMETER*	Gp,int No,float v1,float v2,float v3 );
HRESULT		GBobj_Change_Scale( GB_PARAMETER*		Gp,int No,float v1,float v2,float v3 );
HRESULT		GBobj_Change_Transfer( GB_PARAMETER*	Gp,int No,float v1,float v2,float v3 );
//
//
///////////////////////////////////////////////////////////////////　座標変換
//--------------------------------------------------------------------------------------------------
//  TRANS
BOOL			GB_Trans_Pos( GB_PARAMETER* Gp,CPoint pt,double* mx,double* my);
BOOL			GB_Get_Coord( GB_PARAMETER* Gp,CPoint pt,XYZCOORD* xyz);
BOOL			GB_Trans_3D(double x,double y,double z,XYZCOORD ang,XYZCOORD tran,double *X,double *Y,double *Z);
//
///////////////////////////////////////////////////////////////////　選択
//--------------------------------------------------------------------------------------------------
//  SELECT OBJECT
//  
int				GBobj_Select_Ortho(GB_PARAMETER*  Gp,int ID,CPoint pt,BOOL bpoly,BOOL bline,BOOL bpos,int* Kind);
//
//---------------------------------------------------------------------------------------------------
// Utility
void			Change_Cursor( GB_PARAMETER* Gp,LPSTR Cid );
//---------------------------------------------------------------------------------------------------
#endif