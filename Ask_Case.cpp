// Ask_Case.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RRI.h"
#include "RRIView.h"
#include "Ask_Case.h"
#include "afxdialogex.h"

#include	"CommFuncs.h"
//_______________________________________________________________________________________________
/////////////////////////////////////////////////////////////////////////////////////////////////
//
extern      char			m_RecentPath[];
extern      int			CV;								// ORTHO,PERS モード
extern      PARAMS		PM;						// 上下流基本情報
extern      DATASET		DSET;						// 上下流ファイルセット
extern      LOCATION_INFO	LC;						// location.txt情報
//
extern      int				MAX_TEXTURE, LAT_SPAN,LNG_SPAN,DEM_H,DEM_V;
extern      double			TEX_START_LAT,TEX_START_LNG;
extern      double			DEM_LAT,DEM_LNG,DEM_SIZE;
extern      double			ONE_DEGREE_DISTANCE;

// CAsk_Case ダイアログ

IMPLEMENT_DYNAMIC(CAsk_Case, CDialogEx)

CAsk_Case::CAsk_Case(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAsk_Case::IDD, pParent)
{

}

CAsk_Case::~CAsk_Case()
{
}

void CAsk_Case::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BUTTON_CASEDIR, c_ProjDir);
	//DDX_Control(pDX, IDC_BUTTON_INPUT, c_RRI);
	DDX_Control(pDX, IDOK, c_OK);
	//DDX_Control(pDX, IDC_INPUT_LABEL, c_Label);
	//DDX_Control(pDX, IDC_COMMENT, c_Com);
	DDX_Control(pDX, IDC_COM, c_Com);
}


BEGIN_MESSAGE_MAP(CAsk_Case, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON_CASEDIR, &CAsk_Case::OnBnClickedButtonCasedir)
	//ON_BN_CLICKED(IDC_BUTTON_INPUT, &CAsk_Case::OnBnClickedButtonInput)
	ON_BN_CLICKED(IDOK, &CAsk_Case::OnBnClickedOk)
END_MESSAGE_MAP()


// CAsk_Case メッセージ ハンドラー


BOOL CAsk_Case::OnInitDialog()
{
	CDialogEx::OnInitDialog();
//
	this->SetWindowTextA(" Project Folder");
	//
	Valid_Proc  = 0;
	//
	return TRUE; 
}

void CAsk_Case::OnBnClickedOk()
{
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  読込  RRI_INPUT
	if (!Button_File(TRUE, &InputFile, m_RecentPath, "RRI_Input", "txt")) return;
	//
	char		Fullpath[SIZE_1K];
	strcpy(Fullpath, InputFile);
	Get_Dir(Fullpath, DSET.Proj_Folder);
	//
	FILE*	fp = fopen(InputFile, "r");
	//
	if (Read_RRI_INPUT(&DSET, fp)){ 
		fclose(fp);
		c_Com.SetWindowTextA("** Missed **  Error Occured in Reading RRI File"); return;
	}
	fclose(fp);

	// 読込 location.txt
	InputFile = DSET.Proj_Folder;
	InputFile += "\\location.txt";

	if (File_Exist(InputFile)) {
		//
		fp = fopen(InputFile, "r");
		//
		if (Read_LOCATION_TEXT(&LC, fp)) {
			fclose(fp);
			c_Com.SetWindowTextA("** Missed **  Error Occured in Reading location File"); return;
		}
		fclose(fp);
	}


	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  OUT_STORAGE は基本的に無視、このエリアはｈｓの最大値とする。 OUT_HSMAX = OUT_STORAGE

	if (DSET.in_use[OUT_HS]) 
		DSET.in_use[OUT_HSMAX] = DSET.in_avail[OUT_HSMAX] = DSET.in_exist[OUT_HSMAX] = TRUE;

	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  DEM file から基本的な情報の取得

	if (Get_ArcGIS_Info(DSET.File_names[ADEM_FILE], &PM.MHnum, &PM.MVnum, &PM.stLAT, &PM.stLNG, &PM.Size)){
		c_Com.SetWindowTextA("** Missed **  Error Get ArcGIS Information"); return;
	}
	if (!DSET.SW_LatLong) PM.Deg_Unit = PM.Coord_Unit = (int)(1. / PM.Size + 0.05);
	else       {
		PM.stUTM_X = PM.stLNG;
		PM.stUTM_Y = PM.stLAT;
		PM.Deg_Unit = PM.UTMSize = PM.Size;
		PM.Coord_Unit = 1. / PM.Size;
	}

	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  開始時間の取得
	CString		fname;

	fname.Format("%s/time.dat", DSET.Proj_Folder);

	int			pos,y,m,d,h,mn,s,Bd;
	char			buf[SIZE_1K],inchar[SIZE_01K];
	//
	fp = fopen(fname, "r");
	CTime	Tt;
	if (fp != NULL) {
		fgets(buf, SIZE_1K, fp);
		Tt = StrDate_Time(buf);
		fclose( fp );
	}
	else {
		if (MessageBox("**Error** Time.dat file cannot be found.\n Set Temporary start time at 2000/ 1/ 1 0:00... OK?", 
			"ask", MB_YESNO | MB_ICONQUESTION) == IDNO) exit(0);
		Tt = ::CTime(2000, 1, 1, 0, 0, 0, -1);
	}
	//
	DSET.ST = DSET.CT = Tt;
	DSET.set = TRUE;
//
	CDialogEx::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
//	ファイル名の結合  \\ -> /  LINAX
//
void	CAsk_Case::Get_Full_Name(char* Dir_name,char* RelativePath,char* Full_name)
{
	char	Copy_Dir[SIZE_1K];
	strcpy( Copy_Dir,Dir_name);
	//
	for (int i = 0; i < strlen(Copy_Dir); i++) {
		if (Copy_Dir[i] == 0x5c) Copy_Dir[i] = 0x2f;
	}
	for (int i = 0; i < strlen(RelativePath); i++) {
		if (RelativePath[i] == 0x5c) RelativePath[i] = 0x2f;
	}
	//
	if (RelativePath[0] == '.' && RelativePath[1] == '/') {
		sprintf(Full_name,"%s/%s",Dir_name,&RelativePath[2]);
	}
	else if( RelativePath[0] == '.' && RelativePath[1] == '.' &&RelativePath[2] == '/') {
		for(int i=strlen(Copy_Dir)-1;i>=0;i--) {
			if( Copy_Dir[i] == '/' ) {
				Copy_Dir[i] = 0x00;
				sprintf(Full_name,"%s/%s",Dir_name,&RelativePath[3]);
				break;
			}
		}
	}
	else {
		sprintf(Full_name,"%s/%s",Dir_name,RelativePath);
	}
	//
	for(int i=0;i<(int)strlen( Full_name );i++) {
		if (Full_name[i] == 0x5c) Full_name[i] = '/';
	}
}


