#include "SelectCalculatedLevel.h"
#include "stdafx.h"
#include "RRI.h"
#include "afxdialogex.h"

#include  "CommFuncs.h"
#include  "CommGL.h"

#include	"RRI_Common.h"

#include <stdio.h>
#include <direct.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <stdexcept>

#define GNUPLOT_HIHY_PATH		"./Tools/gnuplot/hjhy.png"
#define GNUPLOT_CALCWL_PATH		"./Tools/gnuplot/CalcWL.png"

extern char m_RecentPath[];
extern char outdirpath[1024];

int			SCL_Mnum = 3;
int			SCL_Mnum_ID[] = { IDC_BUTTON_ACCURACYEVALUATION_1,  IDC_BUTTON_ACCURACYEVALUATION_2, IDC_BUTTON_ACCURACYEVALUATION_3 };

char startDt2[20];
char endDt2[20];

char fpath2[1024] = { '0' };

char Observation_water_level[] = { "Observation\\nwater level" };
char Calculated_water_level[] = { "Calculated\\nwater level" };
char Predicted_water_level[] = { "Predicted\\nwater level" };
char Stand_by_water_level_for_Flood_Defense[] = { "Stand-by water level\\nfor Flood Defense" };
char Flood_warning_water_level[] = { "Flood warning\\nwater level" };
char Evacuation_judgment_water_level[] = { "Evacuation judgment\\nwater level" };
char Flood_inundation_water_level[] = { "Flood inundation\\nwater level" };
char River_bed[] = { "River bed[m]" };

int* procNum;

// CSelectCalculatedLevel ダイアログ
IMPLEMENT_DYNAMIC(CSelectCalculatedLevel, CDialog);

CString sclObsWorkDir = "";

CWnd* pAE;

std::vector<std::string> get_file_path_in_dir(const std::string& dir_name, const std::string& extension) noexcept(false);
void replaceEnsign2Slash(char* val);

CSelectCalculatedLevel::CSelectCalculatedLevel(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectCalculatedLevel::IDD, pParent)
{
}

CSelectCalculatedLevel::CSelectCalculatedLevel(CWnd* pParent /*=NULL*/, char* startdt, char* enddt, int* val, CString obsWorkDir)
	: CDialog(CSelectCalculatedLevel::IDD, pParent)
{
	procNum = val;
	sprintf(startDt2, "%s", startdt);
	sprintf(endDt2, "%s", enddt);
	sclObsWorkDir = obsWorkDir;
}


CSelectCalculatedLevel::~CSelectCalculatedLevel()
{
}

//---------------------------------------------------------------------------------
//   メッセージマップ
//---------------------------------------------------------------------------------
//BEGIN_MESSAGE_MAP(CSelectCalculatedLevel, CDialog)
//	ON_BN_CLICKED(IDC_BUTTON_ACCURACYEVALUATION_1, &CSelectCalculatedLevel::OnCalculatedLevel)
//	ON_BN_CLICKED(IDC_BUTTON_ACCURACYEVALUATION_2, &CSelectCalculatedLevel::OnCalculatedFlowRate)
//	ON_BN_CLICKED(IDC_BUTTON_ACCURACYEVALUATION_3, &CSelectCalculatedLevel::OnCalculatedLevelAssimilation)
//END_MESSAGE_MAP()

//---------------------------------------------------------------------------------
//   初期化
//---------------------------------------------------------------------------------
// CAccuracyEvaluation メッセージ ハンドラー
BOOL CSelectCalculatedLevel::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (WinTitle.GetLength() != 0) this->SetWindowTextA(WinTitle);

	return TRUE;
}


void CSelectCalculatedLevel::OnTimer(UINT_PTR nIDEvent)
{
	
}

void CSelectCalculatedLevel::OnCancel()
{
	CDialog::OnOK();
}

void CSelectCalculatedLevel::OnCalculatedLevel()
{

}

void CSelectCalculatedLevel::OnCalculatedFlowRate()
{

}

void CSelectCalculatedLevel::OnCalculatedLevelAssimilation()
{

}BEGIN_MESSAGE_MAP(CSelectCalculatedLevel, CDialog)
ON_BN_CLICKED(IDC_BUTTON_ACCURACYEVALUATION_1, &CSelectCalculatedLevel::OnBnClickedButtonAccuracyevaluation1)
ON_BN_CLICKED(IDC_BUTTON_ACCURACYEVALUATION_2, &CSelectCalculatedLevel::OnBnClickedButtonAccuracyevaluation2)
ON_BN_CLICKED(IDC_BUTTON_ACCURACYEVALUATION_3, &CSelectCalculatedLevel::OnBnClickedButtonAccuracyevaluation3)
END_MESSAGE_MAP()

void CSelectCalculatedLevel::init() {
	/*
	if (startDt2[0] == 0x00) {
		MessageBox("** Error ** Input Start Time", "Fatal Error", MB_OK | MB_ICONASTERISK);
		return;
	}
	if (endDt2[0] == 0x00) {
		MessageBox("** Error ** Input End Time", "Fatal Error", MB_OK | MB_ICONASTERISK);
		return;
	}*/

	char cDirSel[512] = { '¥0' };
	UpdateData(TRUE);

	if (!sclObsWorkDir.Compare("")) {
		return;
	}

}

void CSelectCalculatedLevel::OnBnClickedButtonAccuracyevaluation1()
{

	init();
	if (!sclObsWorkDir.Compare("")) {
		return;
	}
	createCalcWL_Data();
	*procNum = 1;
	CSelectCalculatedLevel::OnOK();
}


void CSelectCalculatedLevel::OnBnClickedButtonAccuracyevaluation2()
{
	init();
	if (!sclObsWorkDir.Compare("")) {
		return;
	}
	createCalcQ_Data();
	*procNum = 2;
	CSelectCalculatedLevel::OnOK();
}


void CSelectCalculatedLevel::OnBnClickedButtonAccuracyevaluation3()
{
	init();
	if (!sclObsWorkDir.Compare("")) {
		return;
	}
	createCalcWLRRI_Predicted_Data();
	*procNum = 3;
	CSelectCalculatedLevel::OnOK();
}
/**
 * 実測、予測水位データ作成(CalcWL)。
**/
void CSelectCalculatedLevel::createCalcWL_Data() {

	char fpath[1024];

	char		buf[SIZE_1K], Datetime[SIZE_1K], WL[SIZE_1K];

	char cdir[255];
	GetCurrentDirectory(255, cdir);

	char outdirpath2[1024];

	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  読込  CalcWL
	if (!Button_File(TRUE, &InputFile, cdir, "CalcWL", "csv")) return;
	if (!File_Exist(InputFile)) {
		MessageBox("** Error ** CalcWL file is not found.", "Fatal Error", MB_OK | MB_ICONASTERISK);
		return;
	}

	FILE*		fp = fopen(InputFile, "r");
	if (fp == NULL) {
		MessageBox("** Error ** CalcWL file is not found.", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}

	// １行目　タイトル
	if (fgets(buf, SIZE_1K, fp) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}

	// 日時、WL
	LPObsWL obsBuf[1024];

	int gyosu = 0;
	while (fgets(buf, SIZE_1K, fp) != NULL) {
		sscanf(buf, "%[^,],%s", Datetime, WL);
		sprintf(obsBuf[gyosu].datetime, "%s", Datetime);
		sprintf(obsBuf[gyosu].kansokusuii, "%s", WL);
		gyosu++;
	}

	fclose(fp);

	// 出力先ディレクトリ作成
	GetCurrentDirectory(255, cdir);
	// ファイルを開く
	sprintf(outdirpath, "%s\\%s", cdir, OUT_WATERLEVEL_DIR);
	_mkdir(outdirpath);
	sprintf(outdirpath, "%s\\CalcWL", outdirpath);
	_mkdir(outdirpath);
	sprintf(fpath, "%s/CalcWLOut.csv", outdirpath);
	FILE* CalcWLOut = fopen(fpath, "w");
	if (CalcWLOut == NULL) {
		MessageBox("** Cannot create ./OUT/CalcWLOut.csv file in selected folder.", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}

	for (int n = 0; n < gyosu + 1; n++) {
		if (atoll(startDt2) <= atoll(obsBuf[n].datetime)
			&& atoll(obsBuf[n].datetime) <= atoll(endDt2)) {
			fprintf(CalcWLOut, "%s,%s\n",
				obsBuf[n].datetime, obsBuf[n].kansokusuii);
		}
	}
	fclose(CalcWLOut);

	// ファイルを開く
	sprintf(fpath, "%s\\CalcWL.plt", outdirpath);
	FILE* foutPlt = fopen(fpath, "w");

	sprintf(fpath, "%s\\CalcWLOut.csv", outdirpath);

	fprintf(foutPlt, "%s", "set datafile separator ','\n");
	fprintf(foutPlt, "%s", "set xlabel \"Date Time\"\n");
	fprintf(foutPlt, "%s", "set ylabel \"Water level[m]\"\n");
	fprintf(foutPlt, "%s", "set xdata time\n");
	fprintf(foutPlt, "%s", "set timefmt \"%Y%m%d%H%M\"\n");
	fprintf(foutPlt, "set xrange [\"%s\":\"%s\"]\n", startDt2, endDt2);
	fprintf(foutPlt, "%s", "set xtics rotate by 270\n");
	fprintf(foutPlt, "%s", "set format x \"%Y/%m/%d %H:%M\"\n");
	fprintf(foutPlt, "%s%s%s", "pngfile=\"", GNUPLOT_CALCWL_PATH, "\"\n");
	fprintf(foutPlt, "%s%s%s", "set out \"", GNUPLOT_CALCWL_PATH, "\"\n");
	fprintf(foutPlt, "%s", "set key right top outside\n");
	fprintf(foutPlt, "%s", "set size nosquare\n");
	fprintf(foutPlt, "set key spacing 2.2\n");
	fprintf(foutPlt, "%s", "set format y \"%2.1f\"\n");
	replaceEnsign2Slash(fpath);
	fprintf(foutPlt, "plot \"%s\" using 1:2 with lines lw 1 linecolor rgb \"blue\" title \"%s\"", fpath, Calculated_water_level);

	char path[1024];
	sprintf(path, "%s\\%s\\observation", cdir, OUT_WATERLEVEL_DIR);
	sprintf(fpath, "%s\\%s\\observation\\ObsWLOut.csv", cdir, OUT_WATERLEVEL_DIR);
	replaceEnsign2Slash(path);
	if (PathFileExists(fpath)) {
		fprintf(foutPlt, ",\\\n");
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:2 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"black\" title \"%s\", \\\n", path, Observation_water_level);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:6 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"green\" title \"%s\", \\\n", path, Stand_by_water_level_for_Flood_Defense);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:5 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"yellow\" title \"%s\", \\\n", path, Flood_warning_water_level);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:4 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"orange\" title \"%s\", \\\n", path, Evacuation_judgment_water_level);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:3 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"red\" title \"%s\"", path, Flood_inundation_water_level);
	}
	sprintf(fpath, "%s\\%s\\observation\\RiverOut.csv", cdir, OUT_WATERLEVEL_DIR);
	if (PathFileExists(fpath)) {
		fprintf(foutPlt, ", \\\n");
		fprintf(foutPlt, "\"%s/RiverOut.csv\" using 1:2 axis x1y1 with lines lw 1 linecolor rgb \"gray\" title \"%s\"\n", path, River_bed);
	}

	fclose(foutPlt);
}

/**
 * 実測、予測水位データ作成(CalcQ)。
**/
void CSelectCalculatedLevel::createCalcQ_Data() {

	char fpath[1024];

	char buf[SIZE_05K], nHQ[SIZE_05K], a1[SIZE_05K], b1[SIZE_05K], Hmax1[SIZE_05K],
		a2[SIZE_05K], b2[SIZE_05K], Hmax2[SIZE_05K], dummy[SIZE_05K],
		Datetime[SIZE_05K], WL[SIZE_05K];

	double fa1, fb1, fHmax1, fa2, fb2, fHmax2;
	int nHQNum;

	char cdir[255];
	GetCurrentDirectory(255, cdir);

	char outdirpath2[1024];
	CString hqeqPath = "";
	CString calcqPath = "";

	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  読込  HQeq.ini
	if (!Button_File(TRUE, &hqeqPath, cdir, "HQeq", "ini")) return;
	if (!File_Exist(hqeqPath)) {
		MessageBox("** Error ** HQeq.ini file is not found.", "Fatal Error", MB_OK | MB_ICONASTERISK);
		return;
	}

	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  読込  CalcQ.csv
	if (!Button_File(TRUE, &calcqPath, cdir, "CalcQ", "csv")) return;
	if (!File_Exist(calcqPath)) {
		MessageBox("** Error ** HQeq.ini file is not found.", "Fatal Error", MB_OK | MB_ICONASTERISK);
		return;
	}

	FILE*		fpHqIni = fopen(hqeqPath, "r");
	if (fpHqIni == NULL) {
		MessageBox("** Error ** HQeq file is not found.", "Fatal Error", MB_OK | MB_ICONHAND);
		return;
	}

	FILE*		fpCalcQ = fopen(calcqPath, "r");
	if (fpCalcQ == NULL) {
		MessageBox("** Error ** CalcQ file is not found.", "Fatal Error", MB_OK | MB_ICONHAND);
		return;
	}

	// HQeq読み込み
	// １行目　タイトル
	if (fgets(buf, SIZE_05K, fpHqIni) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}
	// 2行目　nHQ
	if (fgets(buf, SIZE_05K, fpHqIni) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}
	sscanf(buf, "%s = %s", dummy, nHQ);

	// 3行目　タイトル
	if (fgets(buf, SIZE_05K, fpHqIni) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}
	// 4行目　a1
	if (fgets(buf, SIZE_05K, fpHqIni) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}
	sscanf(buf, "%s = %s", dummy, a1);
	// 5行目　b1
	if (fgets(buf, SIZE_05K, fpHqIni) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}
	sscanf(buf, "%s = %s", dummy, b1);
	// 6行目　Hmax1
	if (fgets(buf, SIZE_05K, fpHqIni) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}
	sscanf(buf, "%s = %s", dummy, Hmax1);
	// 7行目　a1
	if (fgets(buf, SIZE_05K, fpHqIni) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}
	sscanf(buf, "%s = %s", dummy, a2);
	// 8行目　b1
	if (fgets(buf, SIZE_05K, fpHqIni) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}
	sscanf(buf, "%s = %s", dummy, b2);
	// 9行目　Hmax1
	if (fgets(buf, SIZE_05K, fpHqIni) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}
	sscanf(buf, "%s = %s", dummy, Hmax2);

	nHQNum = atof(nHQ);
	fa1 = atof(a1);
	fb1 = atof(b1);
	fHmax1 = atof(Hmax1);
	fa2 = atof(a2);
	fb2 = atof(b2);
	fHmax2 = atof(Hmax2);



	// 日時、WL
	LPObsWL obsBuf[1024];

	if (fgets(buf, SIZE_05K, fpCalcQ) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}

	int gyosu = 0;
	while (fgets(buf, SIZE_05K, fpCalcQ) != NULL) {
		sscanf(buf, "%[^,],%s", Datetime, WL);
		sprintf(obsBuf[gyosu].datetime, "%s", Datetime);
		sprintf(obsBuf[gyosu].kansokusuii, "%s", WL);
		gyosu++;
	}

	fclose(fpCalcQ);

	// 出力先ディレクトリ作成
	GetCurrentDirectory(255, cdir);
	// ファイルを開く
	sprintf(outdirpath, "%s\\%s", cdir, OUT_WATERLEVEL_DIR);
	_mkdir(outdirpath);
	sprintf(outdirpath, "%s\\CalcQ", outdirpath);
	_mkdir(outdirpath);
	sprintf(fpath, "%s/CalcQOut.csv", outdirpath);
	FILE* CalcQOut = fopen(fpath, "w");
	if (CalcQOut == NULL) {
		MessageBox("** Cannot create ./OUT/CalcQOut.csv file in selected folder.", "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}

	double orgVal = 0;
	double calcVal = 0;
	double calcVal1 = 0;
	double calcVal2 = 0;

	for (int n = 0; n < gyosu + 1; n++) {
		if (atoll(startDt2) <= atoll(obsBuf[n].datetime)
			&& atoll(obsBuf[n].datetime) <= atoll(endDt2)) {

			orgVal = atof(obsBuf[n].kansokusuii);

			calcVal1 = sqrt(orgVal / fa1) - fb1;
			if (calcVal1 < fHmax1) {
				calcVal = calcVal1;
			}
			else {
				calcVal2 = sqrt(orgVal / fa2) - fb2;
				calcVal = calcVal2;
			}

			fprintf(CalcQOut, "%s,%f\n",
				obsBuf[n].datetime, calcVal);
		}
	}
	fclose(CalcQOut);

	// ファイルを開く
	sprintf(fpath, "%s\\CalcQ.plt", outdirpath);
	FILE* foutPlt = fopen(fpath, "w");

	sprintf(fpath, "%s\\CalcQOut.csv", outdirpath);

	fprintf(foutPlt, "%s", "set datafile separator ','\n");
	fprintf(foutPlt, "%s", "set xlabel \"Date Time\"\n");
	fprintf(foutPlt, "%s", "set ylabel \"Water level[m]\"\n");
	fprintf(foutPlt, "%s", "set xdata time\n");
	fprintf(foutPlt, "%s", "set timefmt \"%Y%m%d%H%M\"\n");
	fprintf(foutPlt, "set xrange [\"%s\":\"%s\"]\n", startDt2, endDt2);
	fprintf(foutPlt, "%s", "set xtics rotate by 270\n");
	fprintf(foutPlt, "%s", "set format x \"%Y/%m/%d %H:%M\"\n");
	fprintf(foutPlt, "%s%s%s", "pngfile=\"", GNUPLOT_CALCWL_PATH, "\"\n");
	fprintf(foutPlt, "%s%s%s", "set out \"", GNUPLOT_CALCWL_PATH, "\"\n");
	fprintf(foutPlt, "%s", "set key right top outside\n");
	fprintf(foutPlt, "%s", "set size nosquare\n");
	fprintf(foutPlt, "set key spacing 2.2\n");
	fprintf(foutPlt, "%s", "set format y \"%2.1f\"\n");
	replaceEnsign2Slash(fpath);
	fprintf(foutPlt, "plot \"%s\" using 1:2 with lines lw 1 linecolor rgb \"blue\" title \"%s\"", fpath, Calculated_water_level);

	char path[1024];
	sprintf(path, "%s\\%s\\observation", cdir, OUT_WATERLEVEL_DIR);
	sprintf(fpath, "%s\\%s\\observation\\ObsWLOut.csv", cdir, OUT_WATERLEVEL_DIR);
	replaceEnsign2Slash(path);
	if (PathFileExists(fpath)) {
		fprintf(foutPlt, ",\\\n");
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:2 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"black\" title \"%s\", \\\n", path, Observation_water_level);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:6 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"green\" title \"%s\", \\\n", path, Stand_by_water_level_for_Flood_Defense);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:5 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"yellow\" title \"%s\", \\\n", path, Flood_warning_water_level);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:4 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"orange\" title \"%s\", \\\n", path, Evacuation_judgment_water_level);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:3 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"red\" title \"%s\"", path, Flood_inundation_water_level);
	}
	sprintf(fpath, "%s\\%s\\observation\\RiverOut.csv", cdir, OUT_WATERLEVEL_DIR);
	if (PathFileExists(fpath)) {
		fprintf(foutPlt, ", \\\n");
		fprintf(foutPlt, "\"%s/RiverOut.csv\" using 1:2 axis x1y1 with lines lw 1 linecolor rgb \"gray\" title \"%s\"\n", path, River_bed);
	}

	fclose(foutPlt);
}

/**
 * 実測、予測水位データ作成(CalcWLRRI_Predicted)。
**/
void CSelectCalculatedLevel::createCalcWLRRI_Predicted_Data() {

	char fpath[1024];
	char fpathJ[1024];
	char startt[1024];
	char** hyfiles;
	int hyfiles_cnt = 0;
	hyfiles = (char**)malloc(sizeof(char*) * 1024);

	char		buf[SIZE_1K], btime[SIZE_1K], time[SIZE_1K], wlevel[SIZE_1K];

	char cdir[255];
	GetCurrentDirectory(255, cdir);

	char outdirpath2[1024];

	sprintf(fpath, "%s\\%s", cdir, OUT_WATERLEVEL_DIR);
	_mkdir(fpath);

	sprintf(outdirpath2, "%s\\%s\\CalcWL(RRI-Predicted)", cdir, OUT_WATERLEVEL_DIR);
	
	//_rmdir(outdirpath2);

	CString deldir = outdirpath2;
	deldir += "?";
	deldir.SetAt(deldir.GetLength() - 1, 0);
	SHFILEOPSTRUCT drive;
	drive.hwnd = this->m_hWnd;
	drive.wFunc = FO_DELETE;
	drive.pFrom = deldir;
	drive.pTo = NULL;
	drive.fFlags = FOF_ALLOWUNDO;
	SHFileOperation(&drive);

	_mkdir(outdirpath2);

	std::vector<std::string> files;

	std::string findPath = sclObsWorkDir + "\\PF_Prediction";
	std::string ext = "csv";
	files = get_file_path_in_dir(findPath, ext);

	// ファイルを開く
	sprintf(fpathJ, "%s\\HJ.csv", outdirpath2);
	FILE* foutJ = fopen(fpathJ, "w");

	int startFlg = 0;

	for (int i = 0; i < files.size(); i++) {
		
		// ファイル名にWtMeanを含むかcheck
		if (files[i].find("WtMean") == std::string::npos) {
			continue;
		}

		// ファイル名から観測時間を抽出
		sprintf(btime, "%s", files[i].c_str());

		btime[12] = 0x00;
		if (startFlg == 0) {
			sprintf(startt, "%s", btime);
			startFlg = 1;
		}

		if (atoll(startDt2) > atoll(btime) || atoll(btime) > atoll(endDt2)) {
			continue;
		}

		// ファイルを開く
		sprintf(fpath, "%s\\%s", sclObsWorkDir + "\\PF_Prediction", files[i].c_str());
		FILE* fp = fopen(fpath, "r");

		// ファイルを開く
		sprintf(fpath, "%s\\HY_%s.csv", outdirpath2, btime);
		FILE* foutY = fopen(fpath, "w");
		hyfiles[hyfiles_cnt] = (char*)malloc(1024);
		sprintf(hyfiles[hyfiles_cnt], "%s", fpath);
		hyfiles_cnt++;

		// 観測時間まで読み飛ばす。観測時間はHJ
		// それ以降は、HYとしてデータをコピーする
		while (fgets(buf, SIZE_1K, fp) != NULL) {
			sscanf(buf, "%[^,],%s", time, wlevel);
			if (atoll(time) >= atoll(btime)) {
				if (atoll(time) == atoll(btime)) {
					fprintf(foutJ, "%s", buf);
				}
				else {
					if (atoll(time) <= atoll(endDt2)) {
						fprintf(foutY, "%s", buf);
					}
				}
			}
		}

		fclose(fp);
		fclose(foutY);

	}

	fclose(foutJ);

	// ファイルを開く
	sprintf(fpath, "%s\\CalcWL(RRI-Predicted).plt", outdirpath2);
	FILE* foutPlt = fopen(fpath, "w");

	sprintf(fpath, "%s\\HJ_%s_%s.csv", outdirpath2, startt, btime);
	rename(fpathJ, fpath);

	fprintf(foutPlt, "%s", "set datafile separator ','\n");
	fprintf(foutPlt, "%s", "set xlabel \"Date Time\"\n");
	fprintf(foutPlt, "%s", "set ylabel \"Water level[m]\"\n");
	fprintf(foutPlt, "%s", "set xdata time\n");
	fprintf(foutPlt, "%s", "set timefmt \"%Y%m%d%H%M\"\n");
	fprintf(foutPlt, "set xrange [\"%s\":\"%s\"]\n", startDt2, endDt2);
	fprintf(foutPlt, "%s", "set xtics rotate by 270\n");
	fprintf(foutPlt, "%s", "set format x \"%Y/%m/%d %H:%M\"\n");
	fprintf(foutPlt, "%s%s%s", "pngfile=\"", GNUPLOT_HIHY_PATH, "\"\n");
	fprintf(foutPlt, "%s%s%s", "set out \"", GNUPLOT_HIHY_PATH, "\"\n");
	fprintf(foutPlt, "%s", "set key right top outside\n");
	fprintf(foutPlt, "%s", "set size nosquare\n");
	fprintf(foutPlt, "set key spacing 2.2\n");
	fprintf(foutPlt, "%s", "set format y \"%2.1f\"\n");
	replaceEnsign2Slash(fpath);
	fprintf(foutPlt, "plot \"%s\" using 1:2 with lines lw 1 linecolor rgb \"blue\" title \"%s\", \\\n", fpath, Calculated_water_level);

	int lcnt = 0;
	for (int i = 0; i < hyfiles_cnt -1 ; i++) {
		replaceEnsign2Slash(hyfiles[i]);
		if (i == 0) {
			fprintf(foutPlt, "\"%s\" using 1:2 with lines lw 1 linecolor rgb \"red\" title \"%s\", \\\n", hyfiles[i], Predicted_water_level);
		}
		else {
			fprintf(foutPlt, "\"%s\" %s", hyfiles[i], "using 1:2 with lines lw 1 linecolor rgb \"pink\" notitle, \\\n");
		}
		free(hyfiles[i]);
		lcnt++;
	}
	replaceEnsign2Slash(hyfiles[lcnt]);
	fprintf(foutPlt, "\"%s\" %s", hyfiles[lcnt], "using 1:2 with lines lw 1 linecolor rgb \"pink\" notitle");
	
	char path[1024];
	sprintf(path, "%s\\%s\\observation", cdir, OUT_WATERLEVEL_DIR);
	sprintf(fpath, "%s\\%s\\observation\\ObsWLOut.csv", cdir, OUT_WATERLEVEL_DIR);
	replaceEnsign2Slash(path);
	if (PathFileExists(fpath)) {
		fprintf(foutPlt, ",\\\n");
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:2 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"black\" title \"%s\", \\\n", path, Observation_water_level);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:6 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"green\" title \"%s\", \\\n", path, Stand_by_water_level_for_Flood_Defense);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:5 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"yellow\" title \"%s\", \\\n", path, Flood_warning_water_level);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:4 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"orange\" title \"%s\", \\\n", path, Evacuation_judgment_water_level);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:3 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"red\" title \"%s\"", path, Flood_inundation_water_level);
	}
	sprintf(fpath, "%s\\%s\\observation\\RiverOut.csv", cdir, OUT_WATERLEVEL_DIR);
	if(PathFileExists(fpath)) {
		fprintf(foutPlt, ", \\\n");
		fprintf(foutPlt, "\"%s/RiverOut.csv\" using 1:2 axis x1y1 with lines lw 1 linecolor rgb \"gray\" title \"%s\"\n", path, River_bed);
	}

	free(hyfiles[lcnt]);
	free(hyfiles);

	fclose(foutPlt);



}

std::vector<std::string> get_file_path_in_dir(const std::string& dir_name, const std::string& extension) noexcept(false)
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;//defined at Windwos.h
	std::vector<std::string> file_names;

	//拡張子の設定
	std::string search_name = dir_name + "\\*." + extension;

	hFind = FindFirstFile(search_name.c_str(), &win32fd);

	try {
		if (hFind == INVALID_HANDLE_VALUE) {
			throw std::runtime_error("file not found");
		}
		do {
			if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			}
			else {
				file_names.push_back(win32fd.cFileName);
				printf("%s\n", file_names.back().c_str());

			}
		} while (FindNextFile(hFind, &win32fd));

	}
	catch (...) {

	}

	FindClose(hFind);

	return file_names;
}

void replaceEnsign2Slash(char* val) {

	for (int i = 0; i < strlen(val); i++) {

		if (val[i] == '\\') {
			val[i] = '/';
		}

	}
}