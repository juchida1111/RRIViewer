///////////////////////////////////////////////////////////////////////////////////
// RRI 共通の変数・関数
//
//
///////////////////////////////////////////////////////////////////////////////////
#include  "CommFuncs.h"

//----------------------------------------------------------------------------　TIMER
#define		INIT_TIMER				1
#define		RENDER_TIMER				2
#define		MODELING_TIMER			3
#define		ANIMATION_TIMER			4
#define		EDITTERM_TIMER			5

#define		SECTION_RENDER_TIMER	7
#define		PROFILE_RENDER_TIMER	8

#define		SEARCH_TIMER			10

#define		FORCUS_SEARCH_TIMER		11
#define		FORCUS_SEARCH_TIMER_BT	12

#define		GB_RENDER_TIMER			13

#define		ACCURACYEVALUATION_RENDER_TIMER		15
#define		SELECTCALCULATEDLEVEL_RENDER_TIMER		16
#define		ERRORRATE_RENDER_TIMER		17

#define		TEMP_TIMER				20

#define		DEG_DISTANCE				100.   // 100km

//----------------------------------------------------------------------------　COLOR
//
#define		COLOR_BACK_VALID				RGB(230,200,200)
#define		COLOR_BACK_INVALID			RGB(230,230,230)
//
#define		COLOR_TAB_TEXT			RGB(0,0,0)
#define		COLOR_DISTAB_TEXT			RGB(150,150,80)
//
#define		DEFAULT_COL_MESH			RGB(180,180,100)
#define		DEFAULT_COL_MESHR			RGB(30,0,255)
#define		DEFAULT_COL_MESHB			RGB(255,0,0)

#define		COLOR_FOCUS		RGB(80,130,130)     // COLOR_EDIT = RGB(70,75,80)
#define		COLOR_NORMAL		RGB(70,75,80)
//----------------------------------------------------------------------------　モード定義
#define		MODE_VIEW			0
#define		MODE_MODEL			1

#define		UPPER				0
#define		LOWER				1

#define		AREA				0
#define		RIVER				1
//
#define		CAM_DIR_UP			0
#define		CAM_DIR_LEFT		1
#define		CAM_DIR_RIGHT		2
//
//
#define		BOUND_OFFSET			30.f
#define		RIVPATH_OFFSET		20.f
#define		RIVER_OFFSET			10.f

#define		LINE_OFFSET				3.		// メッシュの高さ
#define		WL_OFFSET				5.
#define		DATA_OFFSET				3.

#define		BAND_HEIGHT				-100.
#define		DATA_HEIGHT				-100.

#define		ORTHO_SCALE				0.01
#define		BACKGROUND_OFFY			50.
//
#define		ANIM_TIME_BASE			100
//
#define		VAL_INFINIT				9999999.
//
//
////----------------------------------------------------------------------------　パラメータ定義
#define		FILE_IN_NUM		23
#define		FILE_IN_USE		20
#define		FILE_OUT_NUM		10
//#define		FILE_GIS_NUM		6

#define		LOCATION_NUM	10

#define		TS_ADD_MEM		20
#define		TS_MIN_WIDTH		500
#define		TS_MIN_HEIGHT		500

//----------------------------------------------------------------------------　WINDOW
#define		WIN_HYDRO		0
#define		WIN_R_PROF		1
#define		WIN_A_PROF		2
//----------------------------------------------------------------------------　ファイル
#define		RAIN_FILE		0		// Time Series
////	
#define		ADEM_FILE		1		// Mesh Param
#define		ACC_FILE			2		// Mesh Param
#define		ADIR_FILE		3		// Mesh Param
////
#define		LU_FILE			4		// Mesh Param
#define		WIDTH_FILE		5		// Mesh Param
#define		DEPTH_FILE		6		// Mesh Param
#define		HEIGHT_FILE		7		// Mesh Param
////
#define		HS_FILE			8		// Mesh
#define		HR_FILE			9		// Mesh
#define		HG_FILE			10		// Mesh Add 14
#define		GAMPT_FILE		11		// Mesh
////
#define		HS_WLEV_FILE		12		// TimeSet
#define		HR_WLEV_FILE		13		// TimeSet
#define		QS_WLEV_FILE		14		// TimeSet
#define		QR_WLEV_FILE		15		// TimeSet
////
#define		DAM_CNT_FILE		16		// List
////
#define		DIV_FILE			17		// TimeSet
//
#define		POTENT_FILE		18		// Time Series
#define		LOCATION_FILE		19		// List
//
#define		LENGTH_FILE		20		// Mesh Param
#define		SEC_MAP_FILE		21		// Mesh Param
#define		SEC_HEADER		22		// Each 
/*
*/
#define		OUT_HS			0
#define		OUT_HR			1
#define		OUT_HG			2
#define		OUT_QR			3
#define		OUT_QU			4
#define		OUT_QV			5
#define		OUT_GU			6
#define		OUT_GV			7
#define		OUT_GAMPUT		8
#define		OUT_STORAGE		9
#define		OUT_HSMAX		9

#define		OUT_VECTOR		10

#define		OUT_WATERLEVEL_DIR "out"

//
#ifndef		_OBSPOINT
#define		_OBSPOINT
typedef		struct  _OBSPOINT {
	char		name[34];
	int			x,y;
} OBSPOINT;
#endif
//
//
#ifndef _COLVAL_FIX
#define _COLVAL_FIX
typedef	struct	_COLVAL_FIX {
	BOOL		set,Equal,Mos;
	float		alpha;
	int			num;
	double		val[11];
	COLORREF		Rcol[11];
	FCOLOR		Fcol[11];
} COLVAL_FIX;
#endif
//
#ifndef _SHAPE_BOX
#define _SHAPE_BOX
typedef	struct	_SHAPE_BOX {
	BOOL			set;
	int			ListID;
	char			name[64],info[32];
	int			Vnum,Mnum;
	XYCOORD*		XZ;
	int*			MID;
	double		Area;
	COLORREF	Col;
} SHAPE_BOX;
#endif


#ifndef		_DATASET
#define		_DATASET
typedef		struct  _DATASET {
	BOOL			set,Set_CaseDir;
	//
	CTime		ST,BT,CT;
	//--------------------------------------------- Basic Folder & File
	char			Proj_Folder[SIZE_02K];
	char			Proj_Title[SIZE_02K];
	//
	int			Cin_Need,Cin_Exist;
	int			Cout_Need,Cout_Exist;
	////
	BOOL			in_avail[ FILE_IN_NUM ],in_exist[ FILE_IN_NUM ],in_use[ FILE_IN_NUM ];
	BOOL			need_save[ FILE_IN_NUM ];
	BOOL			out_exist[ FILE_OUT_NUM ],out_use[ FILE_OUT_NUM ];
//
	char			Data_Title[SIZE_02K];
	char			RRI_File[SIZE_02K];
	char			File_names[FILE_IN_NUM][SIZE_02K];
	char			Input_names[FILE_IN_NUM][SIZE_02K];
	 

	char			Out_names[FILE_OUT_NUM][SIZE_02K];
	char			Out_Header[FILE_OUT_NUM][SIZE_02K];
	//
	double		Riv_Thresh;				// = Acc_Limit
	double		Cof_WC, Cof_WS, Cof_DC, Cof_DS;
	double		Cof_H, Cof_Hlim;
	//
	//--------------------------------------------- Parameters
	BYTE			SW_LatLong,SW_Dir;
	int			Hours,DT,DT_Riv,DT_Out;		// 計算時間と結果出力数
	int			DT_Inter;
	//
	double		Max_QV, Max_GV;
	double		ArrowMag_QV, ArrowMag_GV;
	//
	double		Ns_River;
	//
	char LocationName[48];
	int LocationX;
	int LocationY;

} DATASET;
#endif

#ifndef		_PARAMS
#define		_PARAMS
typedef		struct _PARAMS {				// 上下流モデル分
	BOOL			set,B_Color;				// セットされているか,バックのカラー表示
	BOOL			Model,WLmodel;			// モデル作成、水深モデルの作成
	BOOL			B_Back,B_Mesh;			// バックイメージ、メッシュの表示
	BOOL			B_Legend,Show_Legend;		// Show_   : 実際に表示されているか
	BOOL			B_Date,B_DateBack,Show_Date;	// Show_   : 実際に表示されているか
	BOOL			B_WLframe;
	BOOL			B_ColorMesh;
	BOOL			B_ShowTimeData;
	//----------------------------------------------Arrow
	BOOL			B_Arrow;					// Display UV vector 
	BOOL			B_IncludeRiver;
	double		Arrow_mag;				// メッシュ幅/ 最大値
	double		Arrow_UserMag;			// 倍率 １ の場合はメッシュ幅
	XYCOORD		Arrow_Pair;				// 最大値の組み合わせ
	double		Arrow_Max;
	double		Arrow_Ang;				// 矢印頭の角度
	double		Arrow_Head;				// 全体の長さ１に対する矢印頭の比率
	//--------------------------------------------- Display
	BOOL			Set_Line,Set_Height_Model,Pro_Ready;
	int			Lcnt,Pro_draw_Acnt;

	IXYCOORD		LineID[2];		// 河川の開始点、終了点

	int			LineNum,LineMem;
	//////IXYCOORD*	Path;
	//////double*		Rdist;
	IXYCOORD		Path[1000];
	double		Rdist[1000];
	double**		Pval;		// 0=H,1=Adem,2=Adem-depth,3=Adem+height

	//------------------------------------ Screen Tool
	BOOL			B_ScreenTool;						// スクリーンツールの表示
	double		Tool_Rate;						// 0.05 -0.3
	int			Mesh_Thresh;
	BOOL			B_MeshLine;
	//
	//--------------------------------------------- Shapes
	int			Snum,Smem;
	SHAPE_BOX*	SBOX;
	//
	//--------------------------------------------- Observation Points
	int			OBmem,OBnum;
	OBSPOINT*	OBpos;
	//--------------------------------------------- Option
	int			MHnum, MVnum;				// モデルメッシュ数
	int			Vnum,Hnum;				// 緯度数、経度数
	int			Tnum,Anum;				// 総メッシュ数、有効メッシュ数
	double		stLAT,stLNG,Size;
	double		endLAT, endLNG;
	//
	double		stUTM_Y, stUTM_X, UTMSize;
	double		endUTM_Y, endUTM_X;

	int			IM_Kind;					// 0: LOW, 1:MIDDLE, 2:OTHERS
	char			IM_Folder[256];
	double		Alpha;					// Back Image Transparency
	XYZCOORD		scale;
	//
	COLORREF		Col_Mesh,Col_MeshR,Col_MeshB;
	COLORREF		Col_Date,Col_DateBack,Col_Arrow;
	COLORREF		Col_LegendChar,Col_WLframe;
	//
	// Gis Color
	COLVAL_FIX	OUTCOL[11];				// Option で変更 10:はベクトル色
	//
	//// Data Color
//	COLVAL_FIX	Dcol[21];				// Evp Drain を含む
	BOOL			B_Data_Mosaic;			// EDITOR のデータ操作用
	//
	// Change Data
	//
	BOOL			Change_DCOL;		// ダイアログ全体での変更有り無し
	BOOL			Change_Display;
	BOOL			Change_Image;
	BOOL			Change_Mesh;
	BOOL			Change_Data;
	BOOL			Change_Time;
	BOOL			Change_Legend;
	//--------------------------------------------- Display Back Image
	double		Deg_Unit;				// 一度当たりの寸法,UTM の場合は寸法
	double		Coord_Unit;

	double		Data_Alpha, Back_Alpha;			// Mesh & Back Image Transparency
	//
	double		Back_Offset;						// Back Ground Image offset +value
	int			Back_stLat, Back_endLat;
	int			Back_stLng, Back_endLng;
	int			BVnum, BHnum;
	int**		Back_TexNo;
	//--------------------------------------------- 連絡用
	int			Cam_Dir;
	int			CX,CY;		// センターメッシュ
	int			SizeID;
	int			Move_Cnt;
	//
	BYTE*		Flags;
	double**		Values;
	//
	XYCOORD		Band_Area[2];
	XYCOORD		Band_Center;
	//
	BOOL			Save_Proc;
	short		Edit_Result;
	int			Data_Change_Cnt;
	//
	BOOL			Break_Proc;
	short		Break_Result;
	//
	BOOL			Profile_Proc;
	short		Profile_Result;
	//
	BOOL			MeshProfile_Ready;
	BOOL			MeshProfile_Proc;
	short		MeshProfile_Result;
	int			MPro_draw_Acnt;
	//
	int			TSDLG_minH,TSDLG_minV;
	//
	//--------------------------------------------- Animation
	FILE*		fp;						// 
	int			Acnt,SaveAcnt;
	BOOL			SaveState[ 50 ];
	BOOL			Set_SimFile;
	// 
	int			SaveCSHAPE,SaveCOUT,SaveCLABEL;
	//
	CTime		ctmin,ctmax;
	BOOL			GISfirst;
	//
	int			Anim_Speed;				// 0-10 の間　５標準
	int			Anim_Time;
	double		WLmag;					// 水深の表示倍率
	char			REC_Head[48];
	char			REC_Folder[256];
	int			Add_Info;				// 0,or 1
	//
	// TimeCode
	int			Tw,Th,Tfont,Tstyle;		// フォント幅高さ フォント番号、スタイル
	XYCOORD		Pos_Time,Size_Time;		// タイムコード表示位置（文字左下)
	//
	// Legend
	int			Lw, Lh, Lfont, Lstyle, Lsize;		// フォント幅高さ フォント番号、スタイル
	XYCOORD		Pos_Legend,Size_Legend;		// 枠の大きさ
	CRect		Area_Legend;		// スクリーンないの範囲
	BOOL LocationChk; // Display Location

} PARAMS;
#endif
#ifndef		_MINFO
#define		_MINFO
typedef		struct _MINFO {
	int			flag;			// 有効・無効		150106  0:無効、1:通常,2:河道
	int			LinePno;
	int			PolyPno;			// ＷＬ ポリゴンNo
	double		Lat,Lng;			// 開始経緯度
	double		uv[2];			// UV 開始位置　左上
	char			name[SIZE_01K];	// 地点名
	//
	double		Adem;			// 標高データ
	double		Acc;
	int			Adir;
	double		Rwidth;			// 河道の判断
	double		Rdepth;			// 縦断図表示
	double		Bheight;			// 
	//
	int			ID_RIVPATH;
	//
	double		hsmax;
	double**		Hval;			// 計算結果 Hval[ OUT_NUM ][time]
//
} MINFO;
#endif

#ifndef		_LOCATION_INFO
#define		_LOCATION_INFO
typedef		struct  _LOCATION_INFO {
	char		name[LOCATION_NUM][48];
	int			mx[LOCATION_NUM], mz[LOCATION_NUM];		// 上から座標
	int			Hid[LOCATION_NUM], Vid[LOCATION_NUM];	// 通常座標
	double		lat[LOCATION_NUM], lng[LOCATION_NUM];
	double		X[LOCATION_NUM], Z[LOCATION_NUM];		// 下からの通常位置
} LOCATION_INFO;
#endif
///////////////////////////////////////////////////////////////////////////////////
// RRI 共通 関数
//
//
///////////////////////////////////////////////////////////////////////////////////
//BOOL		Alloc_TIMESET(TIMESET*	TS);
//void		Free_TIMESET(TIMESET*	TS);
//void		Copy_TIMESET(TIMESET* TS, TIMESET* TT);
//
//void		Copy_DAMCNT(DAMCNT* tar, DAMCNT org);

void		Get_Relative_Path(char* Fname, char* Dir, char* Rname);

void		Get_SuitScale(int scaleid, int num, double* x, double* vmin, double* vmax, int* rips, double* unit, int* fm);

void		Get_Wnd_Value(int No);

void		Get_Scale_Axis(double* Hmin, double*  Hmax, int* rips, double* unit, int* fm);

void		Get_GoodScale(int Num, double* val, double* Vmin, double* Vmax,
	int* Rip, double* Unit, int* Fm);

BOOL		Save_Color_Data(CString Boot, PARAMS* PM);

CTime	StrDate_Time(char*  buf);

void		Make_String(double val, CString* str, int limit);

void		Copy_PARAMS(PARAMS* TPM, PARAMS OPM,BOOL CLEAR);


//void		Copy_DSET(DATASET* DT, DATASET DD);
//void		Delete_DSET(DATASET* DS);

int		Read_RRI_INPUT(DATASET* DS, FILE*  fp);
int		Read_LOCATION_TEXT(LOCATION_INFO* LC, FILE*  fp);

int		Get_ArcGIS_Info(char* fname, int* Hnum, int* Vnum, double* stLAT, double* stLNG, double* Size);

int		Set_File_FullName(DATASET* DS);
int		Set_Relative_Name(DATASET* DS);

int		Read_Data_File(int f, CString fname, MINFO** MESH);

BOOL		Is_Same_Pos(IXYCOORD p1, IXYCOORD p2);
//FCOLOR		CREF_FCOL(COLORREF  col);
BOOL		Is_InRect(double x, double y, XYCOORD* xy, BOOL Online);	// 特に矩形だけの場合


//20210806
//#define SIMP_VER
//#define SIMP_VER_TBD
//#define NOT_USING