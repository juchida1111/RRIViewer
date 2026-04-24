//////////////////////////////////////////////////////////////////////////////////////////
// 
//      Standerd  Common  Library Header 
//																Revised  2019/10
//
//////////////////////////////////////////////////////////////////////////////////////////
//
#pragma	once

#include	<afxwin.h>
#include	<cstringt.h>
#include	<math.h>

//
//  プログラムの性格定義    定義変更　2019/10
//
#ifndef		MAIN_CONF
#define		MAIN_CONF
#include	"Proj_Conf.h"		// Property のインクルードディレクトリで　＄(SolusionDir) を指示する。
#endif
//--------------------------------------------------------------------

#ifndef NO_WARN_MBCS_MFC_DEPRECATION
#define  NO_WARN_MBCS_MFC_DEPRECATION
#define	_CRT_SECURE_NO_DEPRECATE
#endif
//
//
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//
//	DEFINE
//
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//
//----------------------------------------------------------------------------　TIMER
#define		INIT_TIMER						1
#define		RENDER_TIMER					2
#define		MODELING_TIMER					3
#define		ANIMATION_TIMER					4
#define		EDITTERM_TIMER					5

#define		SECTION_RENDER_TIMER			7
#define		PROFILE_RENDER_TIMER			8

#define		SEARCH_TIMER					10

#define		FORCUS_SEARCH_TIMER				11
#define		FORCUS_SEARCH_TIMER_BT			12

#define		GB_RENDER_TIMER					13

#define		TEMP_TIMER						20
//
//////////////////////////////////////////////////////////////////////////////////////////
//
#define		BYTE				unsigned char

#define		X_AXIS			1
#define		Y_AXIS			2
#define		Z_AXIS			3
#define		AXIS_SMOLL		1
#define		AXIS_MIDDLE	2
#define		AXIS_LARGE		3
//
#define		CHARSIZE		1024
#define		INCHARSIZE		32
//
#define		SIZE_16K		16384
#define		SIZE_12K		12288
#define		SIZE_8K			8192
#define		SIZE_4K			4096
#define		SIZE_2K			2048
#define		SIZE_1K			1024
#define		SIZE_05K		512
#define		SIZE_02K		256
#define		SIZE_01K		128
#define		SIZE_006K		64
#define		SIZE_003K		32
#define		SIZE_0015K		16

#define		UNDEF			-999999
#define		SUNDEF			(short)-9999
#define		DUNDEF			-999999.
#define		FUNDEF			-999999.f

#define		FLAG_ON			(BYTE)1
#define		FLAG_OFF		(BYTE)0
#define		ON					1
#define		OFF					0
#define		NODATA			-9999.

#define		SORT_INC			1
#define		SORT_DEC			2

#define		DEFAULT_RANDAM   5000
//
//-----------------------------------------------------------
#define		SAFE_FREE(p)			{ if(p) { free( p ); (p)=NULL; } }
#define		SAFE_DELETE(p)		{ if(p) { delete (p);     (p)=NULL; } }
//
///////////////////////////////////////////////////////////////////////////////////////
//	色定義
#define		COL_BLACK					RGB(0,0,0)
#define		COL_GRAY					RGB(150,150,150)
#define		COL_WHITE					RGB(255,255,255)
#define		COL_RED						RGB(255,0,0)
#define		COL_GREEN					RGB(0,255,0)
#define		COL_DARK_GREEN				RGB(0,100,0)
#define		COL_BLUE					RGB(0,0,255)
#define		COL_DARK_BLUE				RGB(0,0,100)
#define		COL_YELLOW					RGB(255,255,0)
#define		COL_CYAN					RGB(0,255,255)
#define		COL_MAZENTA					RGB(255,0,255)

#define		COLOR_BLACK				RGB(0,0,0)
#define		COLOR_GRAY				RGB(150,150,150)
#define		COLOR_WHITE				RGB(255,255,255)
#define		COLOR_RED				RGB(255,0,0)
#define		COLOR_GREEN				RGB(0,255,0)
#define		COLOR_DARK_GREEN		RGB(0,100,0)
#define		COLOR_BLUE				RGB(0,0,255)
#define		COLOR_DARK_BLUE			RGB(0,0,100)
#define		COLOR_YELLOW			RGB(255,255,0)
#define		COLOR_CYAN				RGB(0,255,255)
#define		COLOR_MAZENTA			RGB(255,0,255)
//
///////////////////////////////////////////////////////////////////////////////////////
//  誤差
#define		EPS10			1.E-10
#define		EPS8			1.E-8
#define		EPS6			1.E-6
#define		EPS5			1.E-5
#define		EPS4			1.E-4
#define		EPS3			1.E-3
#define		EPS2			1.E-2
#define		EPS1			0.1
#define		LOOSE_EPS		1.E-3
//
///////////////////////////////////////////////////////////////////////////////////////
// マクロ
#define		CR			fprintf(fp,"\n")
#define		CROUT		fprintf(fout,"\n")

#define		UDF			UpdateData( FALSE )
#define		UDT			UpdateData( TRUE )

#define		BEEP		Beep(0x120,300)
#define		EOB			-1
//
///////////////////////////////////////////////////////////////////////////////////////
// 数値
#define		PI					3.14159265358979323846
#define		PAI					3.14159265358979323846

#define		MAJOR_RADIUS				6378137.	  // GRS_80
#define		MINOR_RADIUS				6356752.314	  // GRS_80
//
///////////////////////////////////////////////////////////////////////////////////////
//	
//  幾何学・数学的構造体の定義
//
///////////////////////////////////////////////////////////////////////////////////////
//
#ifndef   _IXYCOORD
#define   _IXYCOORD
typedef	struct _IXYCOORD {
	BYTE	flag;
	int		x,y;
} IXYCOORD;
#endif
////
#ifndef   _FXYCOORD
#define   _FXYCOORD
typedef	struct _FXYCOORD {
	BYTE	flag;
	double	x,y;
} FXYCOORD;
#endif

#ifndef   _XYCOORD
#define   _XYCOORD
typedef	struct _XYCOORD {
	BYTE	flag;
	double	x,y;
} XYCOORD;
#endif
//
//	最大最少値検索用
//
#ifndef   _MINMAX
#define   _MINMAX
typedef	struct _MINMAX {
	BYTE		flag;
	double	xmin,xmax;
	double	ymin,ymax;
	double	zmin,zmax;
} MINMAX;
#endif
//
// For 3 Dimensional Coordinate
//
#ifndef   _XYZCOORD
#define   _XYZCOORD
typedef	struct _XYZCOORD {
	BYTE	flag;
	double	x,y,z;
} XYZCOORD;
#endif
//
///////////////////////////////////////////////////////////////////////////////////////
//	空間定義
//
#ifndef   _D3_PLANE
#define   _D3_PLANE
typedef	struct _D3_PLANE {
	XYZCOORD	XYZ[3];
	double		a,b,c,d;
} D3_PLANE;
#endif
//
#ifndef   _D3_LINE
#define   _D3_LINE
typedef	struct _D3_LINE {
	XYZCOORD	XYZ[2],VEC;
	double		x0,y0,z0;
	double		l,m,n,len;
} D3_LINE;
#endif
//
///////////////////////////////////////////////////////////////////////////////////////
// COLOR ATTRIBUTE
//
#ifndef _FCOLOR
#define _FCOLOR
typedef	struct	_FCOLOR {
	BOOL			set;
	float			red;
	float			green;
	float			blue;
} FCOLOR;
#endif

#ifndef _COLVAL
#define _COLVAL
typedef	struct	_COLVAL {
	BOOL			set;
	double			val;
	COLORREF		Rcol;
	FCOLOR			Fcol;
} COLVAL;
#endif
//  
/////////////////////////////////////////////////////////////////////////////////////// 
//	シェイプ
///////////////////////////////////////////////////////////////////////////////////////
//
#ifndef		_SHAPE_SET
#define		_SHAPE_SET
typedef  struct  _SHAPE_HEAD {
	BYTE		code[4];			// big
	BYTE		dummy[20];
	BYTE		file_length[4];		// big
	int			version;
	int			type;
	BYTE		odd[64];
} SHAPE_HEAD;
//
typedef	struct _RECORD_HEAD {
	BYTE		no[4];
	BYTE		clen[4];
} RECORD_HEAD;
//
typedef struct  _DATA_HEAD {
	int			type;
	double		area[4];
	int			Numpart;
	int			Numvert;
} DATA_HEAD;
//
typedef struct  _POLYDATA_HEAD {
	int			type;
	BYTE		area[32];
	int			Numpart;
	int			Numvert;
} POLYDATA_HEAD;
//
typedef		struct _POINT_DATA {
	double		x;
	double		y;
} POINT_DATA;

#endif
//-------------------------------------------------------------
// OBJECT Vert 特性　OBJ_Vert_Attribute( stat
//-------------------------------------------------------------
#define		OBJ_INIT_NUM	500
//
/////===================================================================================
//     ＫＩＮＤ
//-------------------------------------------------------------
#define		OBJ_POINT				0x0001
#define		OBJ_LINE				0x0010
#define		OBJ_POLY				0x0100
//
//
//□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
//
#define		IMAGE_BMP			1
#define		IMAGE_JPG			2
#define		IMAGE_TIF			3
//
#define		RGBA_BYTE		BYTE
//
///////////////////////////////////////////////////////////////////////////////////////
// イメージデータヘッダー　色情報　構造体
///////////////////////////////////////////////////////////////////////////////////////
#ifndef		_CMATRIX
#define		_CMATRIX
typedef struct _CMATRIX {
	RGBTRIPLE*	CH;
} CMATRIX;
#endif
//
//
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//=====================================================================================
//
//
//  プログラムの定義
//
//
//													改訂　2019/8
//=====================================================================================
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//
/*-------------------------------------------*/
/*  行列メモリ　　　                       
/*-------------------------------------------*/
////
BYTE**			Make_Matrix(int H,int V,BYTE Initval);
short**			Make_Matrix(int H,int V,short Initval);
int**			Make_Matrix(int H, int V, int Initval);
float**			Make_Matrix(int H, int V, float Initval);
double**		Make_Matrix(int H, int V, double Initval);
COLORREF**		Make_Matrix(int H,int V,COLORREF Initval);
char**			Make_Matrix(int H,int V,char c);
//
double**		Resize_Matrix(double** Org, int H, int OV, int V, double Initval);	// 現在のH,Vより大きいことは前提
char**			Resize_Matrix(char** Org, int H, int OV,int V, char Initval);
double**		Resize_Matrix(double** MAT, int H, int V, int oldH, int oldV, double Initval);	// 現在のH,Vより大きいことは前提
char**			Resize_Matrix(char** MAT, int H, int V, int oldH, int oldV, char Initval);
//
void				Free_Matrix(int V,BYTE** MAT);
void				Free_Matrix(int V,short** MAT);
void				Free_Matrix(int V, int** MAT);
void				Free_Matrix(int V, float** MAT);
void				Free_Matrix(int V,double** MAT);
void				Free_Matrix(int V,COLORREF** MAT);
void				Free_Matrix(int V,char** MAT);

/*-------------------------------------------*
**  入出力支援　　                       
**-------------------------------------------*/
//
BOOL				Button_File(BOOL fid,CString* InputFile,char* CurrentPath,char* title,char* pfr);
//
void				Get_Title(CString longname,CString* title);
void				Get_Title(char* longname,char* title);
void				Get_Dir(CString longname,CString* tdir);
void				Get_Dir(char* longname,char* dir);
// 
int					getstr(char* str,int* p,char* buf,int SIZE);
int					sgetcsv(char* str,int* p,char* buf);
int					sgetfmt(char* str,int* p,int n,char* buf);
int					sgettab(char* str,int* p,char* buf);
int					Is_Include(char* str,char c,int* pos);
////
////
BOOL				File_Exist( char*	fname );
BOOL				File_Exist(CString	fname);
BOOL				Folder_Exist(char*	Dirname);
BOOL				Folder_Exist(CString	Dirname);
////
void				Delete_Folder(char* Folder);
void				Shift_Unix(CString* fname);
void				Shift_Unix(char* fname);
////
////
void				Set_CWnd_Text(CWnd* cw,double val,int limit,BOOL DispNegative);
void				Set_CWnd_Text(CWnd* cw,int val,BOOL DispNegative);
void				Set_CWnd_Text(CWnd* cw,short val,BOOL DispNegative);
void				Set_CWnd_Text(CWnd* cw,BYTE val);
void				Set_CWnd_Text(CWnd* cw,BOOL val);
//-----------------------------------------------------------------------------------------------------------
//   FILL RECT
//-----------------------------------------------------------------------------------------------------------
void				Fill_Color( CWnd* cw,COLORREF col);
//-----------------------------------------------------------------------------------------------------------
//   FILL VALUE
//-----------------------------------------------------------------------------------------------------------
void				Set_Double_Str(double val,CString* str,int defaultnum);
//
void				Static_Format(CWnd* cw,CString str,UINT DT_ID,COLORREF Ctext,COLORREF Cback);
void				Static_Format(CWnd* cw,CString str,int FW,int FH,UINT DT_ID,COLORREF Ctext,COLORREF Cback);
//
/*-------------------------------------------*
**  補間関数                       
**-------------------------------------------*/
////
double			Fmin(double x1,double x2);
double			Fmin(double x1,double x2,double x3);
double			Fmin(double x1,double x2,double x3,double x4);
//
double			Fmax(double x1,double x2);
double			Fmax(double x1,double x2,double x3);
double			Fmax(double x1,double x2,double x3,double x4);
//
int					Imin(int x1, int x2);
int					Imin(int x1, int x2, int x3);
int					Imin(int x1, int x2, int x3, int x4);
////
int					Imax(int x1, int x2);
int					Imax(int x1,int x2,int x3);
int					Imax(int x1,int x2,int x3,int x4);

void				Find_Min_Max(int num, double* val, double* vmin, double* vmax);
void				Find_Min_Max(int num, XYCOORD* val, XYCOORD* vmin, XYCOORD* vmax);
//////////////////////////////////////////////////////////////////////
//-- コピー 同一性判定 ---------------------------------------------------------
//
void			Copy_Coord(IXYCOORD* txy, IXYCOORD oxy);
void			Copy_Coord(XYCOORD* txy, XYCOORD oxy);
void			Copy_Coord(XYCOORD* txy, XYZCOORD oxyz);
void			Copy_Coord(XYZCOORD* txyz, XYZCOORD oxyz);
//
//////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------*


**  幾何係数関連  


**-------------------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////
//-- 距離  ---------------------------------------------------------
//
double			Get_Distance(XYCOORD p1,XYCOORD p2);
double			Get_Distance(double x0,double y0,double x1,double y1);
//
double			Get_Distance(XYZCOORD p1,XYZCOORD p2);
double			Get_Distance(double x0,double y0,double z0,double x1,double y1,double z1);
//
//////////////////////////////////////////////////////////////////////
//--　ソート　並べ替え  ---------------------------------------------------------
//
int					I_Sort(int num,int* vec,int flag);
////
int					Index_Sort(int num,double* X,int* no,int flag);		// ｆlag: 昇順、降順　SORT_INC　SORT_DEC
//
//////////////////////////////////////////////////////////////////////
//--　ベクトル　内外積  ---------------------------------------------------------
//    
int					Set_Plane(XYZCOORD p1,XYZCOORD p2,XYZCOORD p3,D3_PLANE* Plane);
////
//// ２次元   内積値が返る 角度は０→π
double			MV_Scalar(XYCOORD v1,XYCOORD v2,double* ang,double* E);
//// 面積が返る 角度はーπ～π
double			MP_Vector(XYCOORD p0,XYCOORD p1,XYCOORD p2,double* vec,double* ang);
double			MV_Vector(XYCOORD v1,XYCOORD v2,double* vec,double* ang);
//
// ３次元　  内積値が返る 角度は０→π　
double			MP_Scalar(XYZCOORD p0,XYZCOORD p1,XYZCOORD p2,double* ang,double* E);	// 戻り値は内積
double			MV_Scalar(XYZCOORD v1,XYZCOORD v2,double* ang,double* E);
// ｒｖは単位ベクトル、戻り値は面積を返す
double			MP_Vector(XYZCOORD p0,XYZCOORD p1,XYZCOORD p2,XYZCOORD* rv,double* ang);
double			MV_Vector(XYZCOORD v1,XYZCOORD v2,XYZCOORD* rv,double* ang);
//
//////////////////////////////////////////////////////////////////////
//--　領域内検査  ---------------------------------------------------------
////
////    ０：領域外、１領域線上、２、領域内、　UNDEF：平面上に無い
int					Is_InRegion(double x,double y,int num,XYCOORD* reg);        //　全面変更,2013/1
//
////   角度調査
double			Dir_Ang(double x0,double y0,double xt,double yt);		// ０～２π
double			Dir_Ang_PP(double x0,double y0,double xt,double yt);	// -π～π
double			Dir_Ang_PP(double x0,double y0,double x1,double y1,double x2,double y2);	// -π～π
////
//// 反時計まわりを正（0 < PAI ), 時計回り（0 > -PAI )を負値で返す。　→外積？
double			Get_Cross_Ang_PP(double x0,double y0,double x1,double y1,double x2,double y2);
double			Get_Cross_Ang_PP(XYCOORD p1,XYCOORD p2,XYCOORD p3);
//
double			Get_Area(int num, XYCOORD* XY);  // 求積
//////////////////////////////////////////////////////////////////////
//--　交点 交角  ---------------------------------------------------------
//
// 戻り値　点と線の距離，ただし　交点（xc,yc) が左右構成ポイントの中にあること
double			Near_Line(double x,double y,int num,XYCOORD* XY,double* xc,double* yc);
int				Get_Cross_Point(XYCOORD p1,XYCOORD p2,XYCOORD p3,XYCOORD p4,XYCOORD*  cp);

/*-------------------------------------------*
**  カラー属性             
**-------------------------------------------*/
//////////////////////////////////////////////////////////////////////
//
FCOLOR			F_COLOR(double red,double green,double blue );
FCOLOR			F_COLOR(int red,int green,int blue);
BOOL			SAME_FCOLOR( FCOLOR COL1,FCOLOR COL2);
void			Copy_FCOLOR(FCOLOR* tar,FCOLOR org);
COLORREF		FCOL_CREF(FCOLOR fcol);
FCOLOR			CREF_FCOL(COLORREF cref);

//
//////////////////////////////////////////////////////////////////////
/*-------------------------------------------*
**  地球緯度経度　座標変換
**-------------------------------------------*/
double				Atan_All(double x, double y);
void				DegtoDeg3(double deg, int* v1, int* v2, int* v3);
////  土研の方法
double				Get_GCS_Distance(double lng1, double lat1, double lng2, double lat2);
void				PLtoDMS(double v, int* dd, int* hun, int* sec);
//
//
///////////////////////////////////////////////////////////////////////////////////////
//
// IMAGE FILE FUNCTIONS
//
///////////////////////////////////////////////////////////////////////////////////////
//
// 総合
BYTE*			Get_Image_File( CString fname,int* H,int* V,int* imbyte);	
// 読込プレーン数指定
BYTE*			Get_Image_File( CString fname,int image_plane,int* H,int* V,int*imbyte);
BOOL			Get_Image_Size( CString fname,int* H,int* V,int* imbyte);	// サイズのみ

/*-------------------------------------------*
**  BMPファイルコントロール
**-------------------------------------------*/
BOOL			Get_BMP_Size(CString fname,int* H,int* V,int* imbyte);
BYTE*			Get_BMP_File(CString fname,int* H,int* V,int* imbyte);
BYTE*			Get_BMP_File(CString fname, int* H, int* V, int* imbyte, BOOL IndexCol);
BOOL			Save_BMP_File(CString fname,int Width,int Height,COLORREF* RGB );
/*-------------------------------------------*
**  TIFFファイルコントロール
**-------------------------------------------*/
//								for TIFF file
#ifndef    _TIF_IFD
#define    _TIF_IFD
typedef  struct _TIF_IFD {
	unsigned short		tag;
	unsigned short		type;
	int					field;
	int					pos;
} TIF_IFD;
//
typedef  struct _TIF_INFO {
	BOOL		LB;			// FALSE: Little Indian  TRUE: Big Indian
	int			width, height;		// H,V
	int			byte, form,comp;	// BitperSample, SampleFormat, Compression
	int			LpS;					// Low per Strip １ストリップの中の行数
	int			Numoff, * offset;
	int			Numbyte, * Bytecnt;
} TIF_INFO;
#endif
//
BOOL			Get_TIF_Size(CString fname, int* Width, int* Height, int* imbyte, int* err);
BYTE*			Get_TIF_File(CString fname, int* H, int* V, TIF_INFO* tinfo, int* err);
BOOL			Get_TIF_Info(CString fname, TIF_INFO* tinfo, int* err);
void			Free_Tiff_Info(TIF_INFO* tinfo);

/*-------------------------------------------*
**  GEOTIFFファイルコントロール 2019/10  by Art&Science
**-------------------------------------------*/
#define		GDAL_PATH  ".\\Tools\\GDAL"					//******  GDAL プログラムを格納するディレクトリー 
//
//#define		CODING					//***********************　 開発中定義　頒布実行形式作成の時はコメント化
//
#define		COLOR_STEP    1    	// 補間方法の指示　　その値の範囲の色は一定
#define		COLOR_CONT    2	 	// 現RGBと次のRGBで連続変化

typedef  struct _Color_Val {
	UCHAR			inv;			// ival, もしくは val の境界条件 inv=1: ≦　　inv=2: ＜
	int				ival;			// 整数閾値
	float			fval;
	COLORREF		col;			//　色
} Color_Val;
//
/*  ID
		 ID=0: DIR
		 ID=1; ELV
		 ID=2;UPA
		 ID=3;UPG
		 ID=4;WTH (void)
*/
COLORREF*		Get_GeoTiff_Image(CString fname, int* H, int* V,int ID, int Inter, int Cnum, Color_Val* Ctable, int* err);
COLORREF		Get_Col_Val(int val, int inter, int Cnum, Color_Val* Ctable);
COLORREF		Get_Col_Val(float val, int inter, int Cnum, Color_Val* Ctable);
///*-------------------------------------------*
//**  etc
//**-------------------------------------------*/
double			Dtof(char* buf);
double			Dtof(CString str);

