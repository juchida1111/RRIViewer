#include "AccuracyEvaluation.h"
#include "stdafx.h"
#include "RRI.h"
#include "afxdialogex.h"

#include  "CommFuncs.h"
#include  "CommGL.h"

#include	"RRI_Common.h"

#include    "SelectCalculatedLevel.h"
#include    "ErrorRate.h"

#include <stdio.h>
#include <direct.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <array>

#define GNUPLOT_EXE_PATH		"./Tools/gnuplot/gnuplot_x64.exe"
#define GNUPLOT_TEMP_PATH		"./Tools/gnuplot/tmp.png"
#define GNUPLOT_HIHY_PATH		"./Tools/gnuplot/hjhy.png"
#define GNUPLOT_CALCWL_PATH		"./Tools/gnuplot/CalcWL.png"

//#include "CApplication.h"
//#include "CWorkbooks.h"
//#include "CWorksheet.h"
//#include "CWorksheets.h"
//#include "CRange.h"

//CApplication app;
//CWorkbooks books;

//CPoint		BastPoint;

int			AEV_Mnum = 8;
int			AEV_Mnum_ID[] = { IDC_LABEL_1,IDC_LABEL_2,
IDC_EDIT_STARTTIME_Y, IDC_EDIT_ENDTIME_Y, IDC_BUTTON_OBSERVWATERLEVEL, IDC_BUTTON_CALCWATERLEVEL, IDC_BUTTON_ERRORRATE, IDC_BUTTON_DOWNLOAD };

extern      DATASET		DSET;						// 上下流ファイルセット
extern		PARAMS		PM;
extern      int			COUT;						// どの出力データを対象としているか

extern		int			Image_Sub, Option_Sub;
// Common Counter for HARD_COPY
extern      int			Cnt_Prof, Cnt_MProf, Cnt_Hydro;
extern      char			m_CurrentPath[SIZE_05K];
extern      char			m_RecentPath[SIZE_05K];
extern char outdirpath[1024] = { '0' };

CString					CSCLtitle;
CString					CERtitle;

char* PtChar = nullptr; // malloc用char型ポインタ

char startDt[20];
char endDt[20];

char fpath[1024] = { '0' };

CString AeObsWorkDir = "";

// CAccuracyEvaluation ダイアログ

IMPLEMENT_DYNAMIC(CAccuracyEvaluation, CDialog)
void replaceEnsign2Slash2(char* val);
int folderCopy(CString from, CString to);

char Observation_water_level2[] = { "Observation\\nwater level" };
char Calculated_water_level2[] = { "Calculated\\nwater level" };
char Predicted_water_level2[] = { "Predicted\\nwater level" };
char Stand_by_water_level_for_Flood_Defense2[] = { "Stand-by water level\\nfor Flood Defense" };
char Flood_warning_water_level2[] = { "Flood warning\\nwater level" };
char Evacuation_judgment_water_level2[] = { "Evacuation judgment\\nwater level" };
char Flood_inundation_water_level2[] = { "Flood inundation\\nwater level" };
char River_bed2[] = { "River bed[m]" };

char LocationDir[1024];
char ObservationDir[1024];
char ErrorRateDir[1024];
char PredictedDir[1024];
char CalcWLDir[1024];
char CalcQDir[1024];


std::vector<std::string> getFilePathInDir(const std::string& dir_name, const std::string& extension) noexcept(false);
std::string getDataFilePath(const std::string& dir_name, const std::string& prefix, int id) noexcept;
double readHrHsQr(const char* fileNm, int posx, int posy);

CAccuracyEvaluation::CAccuracyEvaluation(CWnd* pParent /*=NULL*/)
	: CDialog(CAccuracyEvaluation::IDD, pParent)
{
	m_Hmax = _T("");
	m_Hmin = _T("");
	m_Qmax = _T("");
	m_Qmin = _T("");
}

CAccuracyEvaluation::~CAccuracyEvaluation()
{
}

void CAccuracyEvaluation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	/*DDX_Text(pDX, IDC_EDIT_STARTTIME_Y, m_Hmax);
	DDX_Text(pDX, IDC_EDIT_ENDTIME_Y, m_Hmin);
	DDX_Text(pDX, IDC_EDIT_STARTTIME_M, m_Hmax);
	DDX_Text(pDX, IDC_EDIT_ENDTIME_M, m_Hmin);
	DDX_Text(pDX, IDC_EDIT_STARTTIME_D, m_Hmax);
	DDX_Text(pDX, IDC_EDIT_ENDTIME_D, m_Hmin);
	DDX_Text(pDX, IDC_EDIT_STARTTIME_H, m_Hmax);
	DDX_Text(pDX, IDC_EDIT_ENDTIME_H, m_Hmin);*/
	DDX_Control(pDX, IDC_ACCURACY_PICTURE, m_AccuracyPicture);
}

//---------------------------------------------------------------------------------
//   メッセージマップ
//---------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAccuracyEvaluation, CDialog)
	/*ON_WM_TIMER()*/
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_ACCURACY_PICTURE, &CAccuracyEvaluation::OnStnClickedAccuracyPicture)
	ON_STN_CLICKED(IDC_SECTION_PICT, &CAccuracyEvaluation::OnStnClickedSectionPict)
	ON_EN_CHANGE(IDC_EDIT_STARTTIME_Y, &CAccuracyEvaluation::OnEnChangeEditStartTime)
	ON_EN_CHANGE(IDC_EDIT_ENDTIME_Y, &CAccuracyEvaluation::OnEnChangeEditEndTime)

	ON_BN_CLICKED(IDC_BUTTON_RECALC, &CAccuracyEvaluation::OnBnClickedButtonRecalc)
	ON_BN_CLICKED(IDC_BUTTON_OBSERVWATERLEVEL, &CAccuracyEvaluation::OnBnClickedButtonObservationWaterLevel)
	ON_BN_CLICKED(IDC_BUTTON_CALCWATERLEVEL, &CAccuracyEvaluation::OnBnClickedButtonCalculationWatarLevel)
	ON_BN_CLICKED(IDC_BUTTON_CROSS_SEC, &CAccuracyEvaluation::OnBnClickedButtonCrossSec)
	ON_BN_CLICKED(IDC_BUTTON_ERRORRATE, &CAccuracyEvaluation::OnBnClickedButtonErrorRate)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CAccuracyEvaluation::OnBnClickedButtonDownload)
	
	ON_BN_CLICKED(IDC_BUTTON_HQEQ, &CAccuracyEvaluation::OnBnClickedButtonHqeq)
	ON_BN_CLICKED(IDC_BUTTON_PF_PREDICTION, &CAccuracyEvaluation::OnBnClickedButtonPfPrediction)
END_MESSAGE_MAP()


//---------------------------------------------------------------------------------
//   初期化
//---------------------------------------------------------------------------------
// CAccuracyEvaluation メッセージ ハンドラー
BOOL CAccuracyEvaluation::OnInitDialog()
{
	char cdir[255];
	char timeDat[255];
	char chkPath[1024];
	char Datetime[64], WL[64], buf[64];
	CTime stime, etime;

	memset(cdir, NULL, sizeof(cdir));
	memset(timeDat, NULL, sizeof(timeDat));
	memset(chkPath, NULL, sizeof(chkPath));
	memset(Datetime, NULL, sizeof(Datetime));
	memset(WL, NULL, sizeof(WL));
	memset(buf, NULL, sizeof(buf));
	memset(LocationDir, 0x00, sizeof(LocationDir));
	memset(fpath, 0x00, sizeof(fpath));

	GetCurrentDirectory(255, cdir);

	// Location ディレクトリ設定
	sprintf(LocationDir, "%s\\Location\\%s", DSET.Proj_Folder, DSET.LocationName);

	// 出力先ディレクトリ
	sprintf(buf, "%s\\%s", cdir, OUT_WATERLEVEL_DIR);
	_mkdir(buf);

	// 全期間取得
	/**  ObsWL.csvを読み込み **/
	sprintf(fpath, "%s\\ObsWL.csv", LocationDir);
	FILE*		fp2 = fopen(fpath, "r");
	if (fp2 == NULL) {
		MessageBox("** Cannot find ObsWL.csv file in selected folder.", "Fatal Error", MB_OK | MB_ICONHAND);
	}
	else {
		// １行目　タイトル
		if (fgets(buf, SIZE_1K, fp2) == NULL) {
			MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		}

		// ２行目　開始時刻
		if (fgets(buf, SIZE_1K, fp2) == NULL) {
			MessageBox("** 開始時刻の記載がありません", "Fatal Error", MB_OK | MB_ICONHAND);
		}
		else {
			sscanf(buf, "%[^,],%s", Datetime, WL);
			SetDlgItemText(IDC_EDIT_STARTTIME_Y, subStr(Datetime, 0, 4));
			SetDlgItemText(IDC_EDIT_STARTTIME_M, subStr(Datetime, 4, 6));
			SetDlgItemText(IDC_EDIT_STARTTIME_D, subStr(Datetime, 6, 8));
			SetDlgItemText(IDC_EDIT_STARTTIME_H, subStr(Datetime, 8, 10));

			this->UpdateStartDt();
		}

		// 最終行　終了時刻
		while (fgets(buf, SIZE_1K, fp2) != NULL) {
			sscanf(buf, "%[^,],%s", Datetime, WL);
		}
		if (buf == NULL) {
			MessageBox("** 終了時刻の記載がありません", "Fatal Error", MB_OK | MB_ICONHAND);
		} else {
			sscanf(buf, "%[^,],%s", Datetime, WL);
			SetDlgItemText(IDC_EDIT_ENDTIME_Y, subStr(Datetime, 0, 4));
			SetDlgItemText(IDC_EDIT_ENDTIME_M, subStr(Datetime, 4, 6));
			SetDlgItemText(IDC_EDIT_ENDTIME_D, subStr(Datetime, 6, 8));
			SetDlgItemText(IDC_EDIT_ENDTIME_H, subStr(Datetime, 8, 10));

			this->UpdateEndDt();
		}

		fclose(fp2);

	}


	// 出力先ディレクトリ設定
	sprintf(outdirpath, "%s\\%s\\", cdir, OUT_WATERLEVEL_DIR);

	// observationディレクトリ設定
	sprintf(ObservationDir, "%s\\%s\\observation", cdir, OUT_WATERLEVEL_DIR);
	
	// Error Rateディレクトリ設定
	sprintf(ErrorRateDir, "%s\\%s\\error_rate", cdir, OUT_WATERLEVEL_DIR);

	// PF Predictionディレクトリ設定
	sprintf(PredictedDir, "%s\\%s\\CalcWL(RRI-Predicted)", cdir, OUT_WATERLEVEL_DIR);

	// CalcWLディレクトリ設定
	sprintf(CalcWLDir, "%s\\%s\\CalcWL", cdir, OUT_WATERLEVEL_DIR);

	// CalcWLディレクトリ設定
	sprintf(CalcQDir, "%s\\%s\\CalcQ", cdir, OUT_WATERLEVEL_DIR);

	//エディットボックスの初期化
	AeObsWorkDir = "";
	UpdateData(FALSE);

	Fig_Ready = FALSE;

	this->CustomizeCtrls();
	CDialog::OnInitDialog();
	
	if (WinTitle.GetLength() != 0) this->SetWindowTextA(WinTitle);

	CButton* cb = (CButton *)GetDlgItem(IDC_CHECK_OW);
	sprintf(chkPath, "%s\\ObsWL.csv", LocationDir);
	if (File_Exist(chkPath)) {
		cb->SetCheck(true);
	}
	cb = (CButton *)GetDlgItem(IDC_CHECK_SW);
	sprintf(chkPath, "%s\\standard_wl.txt", LocationDir);
	if (File_Exist(chkPath)) {
		cb->SetCheck(true);
	}
	cb = (CButton *)GetDlgItem(IDC_CHECK_CS);
	sprintf(chkPath, "%s\\cross_section.txt", LocationDir);
	if (File_Exist(chkPath)) {
		cb->SetCheck(true);
	}

#ifndef SIMP_VER
	cb = (CButton*)GetDlgItem(IDC_CHECK_HQ);
	sprintf(chkPath, "%s\\HQeq.ini", LocationDir);
	if (File_Exist(chkPath)) {
		cb->SetCheck(true);
	}
	cb = (CButton *)GetDlgItem(IDC_CHECK_PF);
	sprintf(chkPath, "%s\\PF_Prediction", LocationDir);
	if (File_Exist(chkPath)) {
		cb->SetCheck(true);
	}
#endif

	// 観測水位　HR QR PF ラジオボタン
	cb = (CButton *)GetDlgItem(IDC_V_CHECK_WATER_LEVEL);
	cb->SetCheck(true);
	cb = (CButton *)GetDlgItem(IDC_RADIO_HR);
	cb->SetCheck(true);

	return TRUE;
}

void CAccuracyEvaluation::OnCancel()
{
	//GB_Term(&GB);
	CDialog::OnOK();
}


//---------------------------------------------------------------------------------
//   SET
//---------------------------------------------------------------------------------
void CAccuracyEvaluation::OnEnChangeEditStartTime() {/* UDT;*/ }
void CAccuracyEvaluation::OnEnChangeEditEndTime() { /* UDT;*/ }

void CAccuracyEvaluation::OnPaint() {
	CPaintDC dc(this); // device context for painting
	if (startDt[0] != 0x00) {
		DrawPicture(GNUPLOT_TEMP_PATH);
	}
}

void CAccuracyEvaluation::OnBnClickedButtonObservationWaterLevel()
{
	CString fpath = "";
	char dstPath[1024], pathBuf[1024];
	memset(dstPath, NULL, sizeof(dstPath));
	memset(pathBuf, NULL, sizeof(pathBuf));

	strcat(dstPath, LocationDir);

	UpdateData(TRUE);

	strcat(pathBuf, DSET.Proj_Folder);
	if (!Button_File(TRUE, &fpath, pathBuf, "", "csv")) return;

	memset(pathBuf, NULL, sizeof(pathBuf));
	strcat(pathBuf, DSET.Proj_Folder);
	strcat(pathBuf, "Location");
	if (!File_Exist(pathBuf)) {
		mkdir(pathBuf);
	}

	if (!File_Exist(dstPath)) {
		mkdir(dstPath);
	}
	sprintf(dstPath, "%s\\ObsWL.csv", dstPath);

	CopyFile(fpath, dstPath, false);

	CButton* cb = (CButton*)GetDlgItem(IDC_CHECK_OW);
	cb->SetCheck(true);
	return;
}

void CAccuracyEvaluation::OnBnClickedButtonCalculationWatarLevel()
{
	CString fpath = "";
	char dstPath[1024], pathBuf[1024];

	memset(dstPath, NULL, sizeof(dstPath));

	memset(pathBuf, NULL, sizeof(pathBuf));

	strcat(dstPath, LocationDir);

	UpdateData(TRUE);

	strcat(pathBuf, DSET.Proj_Folder);
	if (!Button_File(TRUE, &fpath, pathBuf, "", "txt")) return;

	memset(pathBuf, NULL, sizeof(pathBuf));
	strcat(pathBuf, DSET.Proj_Folder);
	strcat(pathBuf, "Location");
	if (!File_Exist(pathBuf)) {
		mkdir(pathBuf);
	}

	if (!File_Exist(dstPath)) {
		mkdir(dstPath);
	}
	sprintf(dstPath, "%s\\standard_wl.txt", dstPath);

	CopyFile(fpath, dstPath, false);

	CButton* cb = (CButton*)GetDlgItem(IDC_CHECK_SW);
	cb->SetCheck(true);
	return;
}

void CAccuracyEvaluation::OnBnClickedButtonErrorRate()
{
	char bufY[20], bufM[20], bufD[20], bufH[20];

	this->UpdateStartDt();
	this->UpdateEndDt();

	Show_ErrorRate();
}

void CAccuracyEvaluation::OnBnClickedButtonDownload()
{

	// 出力先ディレクトリ作成
	char cdir[255];
	GetCurrentDirectory(255, cdir);
	// ファイルを開く
	sprintf(outdirpath, "%s\\%s\\", cdir, OUT_WATERLEVEL_DIR);

	ShellExecute(NULL, "open", outdirpath, NULL, NULL, SW_SHOWNORMAL);
}


void CAccuracyEvaluation::Show_ObservationWaterLevel()
{
#ifndef NOT_USING
	int ret = 0;
 	ret = CAccuracyKadoSodoKobai();
	if (ret != 0) {
		return;
	}

	ret = CAccuracyObsWL();
	if (ret != 0) {
		return;
	}

	ret = CreatePltFile();
	if (ret != 0) {
		return;
	}

	ret = CreatePlot();
	if (ret != 0) {
		return;
	}
#endif // !NOT_USING
}

void CAccuracyEvaluation::Show_SelectCalculatedLevel()
{
#ifndef NOT_USING
	int* procNum;
	procNum = (int*)malloc(sizeof(int));
	
	CSelectCalculatedLevel		CSCL(this, startDt, endDt, procNum, AeObsWorkDir);
	CString		title;

	CSCLtitle.Format("計算水位の選択");

	CSCL.WinTitle = CSCLtitle;
	CRect	Wrect;
	this->GetWindowRect(&Wrect);
	CSCL.WinH = 100;
	CSCL.WinW = 250;
	CSCL.m_Parent = this;
	CSCL.Prect = Wrect;

	if (CSCL.DoModal() == IDOK) {

		if (*procNum == 1) {
			CreatePlotCalcWL();
		}
		else if (*procNum == 2) {
			CreatePlotCalcQ();
		}
		else if (*procNum == 3) {
			#ifndef SIMP_VER
			CreatePlotCalcWL_RRI_Predicted();
			#endif
		}
	}

	free(procNum);
#endif // !NOT_USING
}

void CAccuracyEvaluation::Show_ErrorRate()
{
	int waterlvFlg = 0;
	int hrRadioFlg = 0;
	int qrRadioFlg = 0;
	int selectedNo = 0;
	char fpath[1024];
	char title[256];

	memset(title, NULL, sizeof(title));
	memset(fpath, NULL, sizeof(fpath));

	CButton* cb = (CButton *)GetDlgItem(IDC_V_CHECK_WATER_LEVEL);
	waterlvFlg = cb->GetCheck();

	cb = (CButton *)GetDlgItem(IDC_RADIO_HR);
	hrRadioFlg = cb->GetCheck();

	cb = (CButton *)GetDlgItem(IDC_RADIO_QR);
	qrRadioFlg = cb->GetCheck();

	if (waterlvFlg == BST_CHECKED) {
		selectedNo = 1;
		strcat(fpath, ObservationDir);
		strcat(title, "Error rate (ObsWL)");
	}
	else if (hrRadioFlg == BST_CHECKED) {
		selectedNo = 2;
		strcat(fpath, CalcWLDir);
		strcat(title, "Error rate (hr)");
	}
#ifndef SIMP_VER
	else if (qrRadioFlg == BST_CHECKED) {
		selectedNo = 3;
		strcat(fpath, CalcQDir);
		strcat(title, "Error rate (qr)");
	}
#endif
	else {
		MessageBox("** Please select ObsWL or hr or qr radio button.", "Fatal Error", MB_OK | MB_ICONHAND);
		return;
	}

	CErrorRate		CER(this, startDt, endDt, fpath, selectedNo);

	CERtitle.Format(title);
	CER.WinTitle = CERtitle;
	CER.m_Parent = this;
	CRect	Wrect;
	CER.Prect = Wrect;

	CER.DoModal();
}

void CAccuracyEvaluation::getDateTimeStr(CTime ct, char str[128]) {
#ifndef NOT_USING
	int				len, blen;

	////現在日時を取得
	//SYSTEMTIME t;
	//GetLocalTime(&t);

	////現在日付を出力
	//printf("%04d年%02d月%02d日(%s)¥n",
	//	t.wYear,
	//	t.wMonth,
	//	t.wDay,
	//	weekStr[t.wDayOfWeek]);

	////現在時刻を出力
	//printf("%02d時%02d分%02d.%d秒¥n",
	//	t.wHour,
	//	t.wMinute,
	//	t.wSecond,
	//	t.wMilliseconds);

	sprintf(str, "%d / %2d / %2d  %02d : %02d",
		ct.GetYear(), ct.GetMonth(), ct.GetDay(),
		ct.GetHour(), ct.GetMinute());

	len = strlen(str) + 1;
	blen = 0;
	for (int i = 0; i<len; i++) if (str[i] == ' ') blen++;
#endif
}

int CAccuracyEvaluation::CreatePltFile() {

	char path[1024];
	sprintf(path, "%s\\kansokusuii.plt", ObservationDir);
	FILE* plt = fopen(path, "w");

	sprintf(path, "%s", ObservationDir);

	replaceEnsign2Slash2(path);

	fprintf(plt, "set datafile separator ','\n");
	fprintf(plt, "set xlabel \"Date Time\"\n");
	fprintf(plt, "set ylabel \"Water level[m]\"\n");
	fprintf(plt, "set xdata time\n");
	fprintf(plt, "set xlabel font \"Arial,8\"\n");
	fprintf(plt, "set ylabel font \"Arial,8\"\n");
	fprintf(plt, "set tics font \"Arial,8\"\n");
	fprintf(plt, "set key font \"Arial,8\"\n");
	fprintf(plt, "%s", "set timefmt \"%Y%m%d%H%M\"\n");
	fprintf(plt, "set xrange [\"%s\":\"%s\"]\n", startDt, endDt);
	fprintf(plt, "set xtics rotate by 270\n");
	fprintf(plt, "%s", "set format x \"%Y/%m/%d %H:%M\"\n");
	fprintf(plt, "pngfile = \"%s/kansokusuii.png\"\n", path);
	fprintf(plt, "set out \"%s/kansokusuii.png\"\n", path);
	fprintf(plt, "set key right top outside\n");

	fprintf(plt, "set size nosquare\n");
	fprintf(plt, "set key spacing 2.2\n");
	fprintf(plt, "plot \"%s/ObsWLOut.csv\" using 1:2 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"black\" title \"%s\", \\\n", path, Observation_water_level2);
	fprintf(plt, "\"%s/ObsWLOut.csv\" using 1:6 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"green\" title \"%s\", \\\n", path, Stand_by_water_level_for_Flood_Defense2);
	fprintf(plt, "\"%s/ObsWLOut.csv\" using 1:5 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"yellow\" title \"%s\", \\\n", path, Flood_warning_water_level2);
	fprintf(plt, "\"%s/ObsWLOut.csv\" using 1:4 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"orange\" title \"%s\", \\\n", path, Evacuation_judgment_water_level2);
	fprintf(plt, "\"%s/ObsWLOut.csv\" using 1:3 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"red\" title \"%s\", \\\n", path, Flood_inundation_water_level2);
#ifndef SIMP_VER_TBD
	fprintf(plt, "\"%s/RiverOut.csv\" using 1:2 axis x1y1 with lines lw 1 linecolor rgb \"gray\" title \"%s\"\n", path, River_bed2);
#endif // !SIMP_VER_TBD

	fclose(plt);

	return 0;
}

/*
 * GNU Plot を実行し、グラフ画像ファイルを生成する。
 */
int CAccuracyEvaluation::CreatePlot()
{ 
	// 変数宣言
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
	HANDLE hreadtmp, hread;
	CFile stdinput;
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	CStringA command;
	char path[1024];
	memset(path, NULL, sizeof(path));

	// 標準入力のリダイレクト準備
	CreatePipe(&hreadtmp, &stdinput.m_hFile, &sa, 0);
	DuplicateHandle(GetCurrentProcess(), hreadtmp, GetCurrentProcess(), &hread, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hreadtmp);

	// コンソール画面なしで gnuplot.exe を実行
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hread;
	if (!CreateProcess(GNUPLOT_EXE_PATH, NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		CloseHandle(hread);
		return 1;
	}
	CloseHandle(hread);

	// コマンドを送信
	command = "set terminal pngcairo\n";
	stdinput.Write(command, command.GetLength());
	strcat(path, ObservationDir);
	replaceEnsign2Slash2(path);
	command.Format("call \"%s/kansokusuii.plt\"\n", path);
	stdinput.Write(command, command.GetLength());
	command.Format("set out \"%s\"\n", GNUPLOT_TEMP_PATH);
	stdinput.Write(command, command.GetLength());
	command = "replot\n";
	stdinput.Write(command, command.GetLength());
	command = "exit\n";
	stdinput.Write(command, command.GetLength());

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	DrawPicture(GNUPLOT_TEMP_PATH);

	// 不要ファイルを削除
	//DeleteFile("tmp.png");

	return 0;

}

void CAccuracyEvaluation::CreatePlotCalcWL() {
	// 変数宣言
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
	HANDLE hreadtmp, hread;
	CFile stdinput;
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	CStringA command;
	char buf[1024];

	// 標準入力のリダイレクト準備
	CreatePipe(&hreadtmp, &stdinput.m_hFile, &sa, 0);
	DuplicateHandle(GetCurrentProcess(), hreadtmp, GetCurrentProcess(), &hread, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hreadtmp);

	// コンソール画面なしで gnuplot.exe を実行
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hread;
	if (!CreateProcess(GNUPLOT_EXE_PATH, NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		CloseHandle(hread);
		return;
	}
	CloseHandle(hread);

	// コマンドを送信
	command = "set terminal pngcairo\n";
	stdinput.Write(command, command.GetLength());
	sprintf(buf, "%s", CalcWLDir);
	replaceEnsign2Slash2(buf);
	command.Format("call \"%s/CalcWL.plt\n", buf);
	stdinput.Write(command, command.GetLength());
	command.Format("set out \"%s\"\n", GNUPLOT_TEMP_PATH);
	stdinput.Write(command, command.GetLength());
	command = "replot\n";
	stdinput.Write(command, command.GetLength());
	command = "exit\n";
	stdinput.Write(command, command.GetLength());

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	DrawPicture(GNUPLOT_TEMP_PATH);

	// 不要ファイルを削除
	//DeleteFile("tmp.png");

}

void CAccuracyEvaluation::CreatePlotCalcQ() {
	// 変数宣言
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
	HANDLE hreadtmp, hread;
	CFile stdinput;
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	CStringA command;

	// 標準入力のリダイレクト準備
	CreatePipe(&hreadtmp, &stdinput.m_hFile, &sa, 0);
	DuplicateHandle(GetCurrentProcess(), hreadtmp, GetCurrentProcess(), &hread, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hreadtmp);

	// コンソール画面なしで gnuplot.exe を実行
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hread;
	if (!CreateProcess(GNUPLOT_EXE_PATH, NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		CloseHandle(hread);
		return;
	}
	CloseHandle(hread);

	// コマンドを送信
	command = "set terminal pngcairo\n";
	stdinput.Write(command, command.GetLength());
	command = "call \"out/CalcQ/CalcQ.plt\"\n";
	stdinput.Write(command, command.GetLength());
	command.Format("set out \"%s\"\n", GNUPLOT_TEMP_PATH);
	stdinput.Write(command, command.GetLength());
	command = "replot\n";
	stdinput.Write(command, command.GetLength());
	command = "exit\n";
	stdinput.Write(command, command.GetLength());

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	DrawPicture(GNUPLOT_TEMP_PATH);

	// 不要ファイルを削除
	//DeleteFile("tmp.png");

}

void CAccuracyEvaluation::CreatePlotCalcWL_RRI_Predicted() {
	// 変数宣言
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
	HANDLE hreadtmp, hread;
	CFile stdinput;
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	CStringA command;

	// 標準入力のリダイレクト準備
	CreatePipe(&hreadtmp, &stdinput.m_hFile, &sa, 0);
	DuplicateHandle(GetCurrentProcess(), hreadtmp, GetCurrentProcess(), &hread, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hreadtmp);

	// コンソール画面なしで gnuplot.exe を実行
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hread;
	if (!CreateProcess(GNUPLOT_EXE_PATH, NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		CloseHandle(hread);
		return;
	}
	CloseHandle(hread);

	// コマンドを送信
	command = "set terminal pngcairo\n";
	stdinput.Write(command, command.GetLength());
	command = "call \"out/CalcWL(RRI-Predicted)/CalcWL(RRI-Predicted).plt\"\n";
	stdinput.Write(command, command.GetLength());
	command.Format("set out \"%s\"\n", GNUPLOT_TEMP_PATH);
	stdinput.Write(command, command.GetLength());
	command = "replot\n";
	stdinput.Write(command, command.GetLength());
	command = "exit\n";
	stdinput.Write(command, command.GetLength());

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	DrawPicture(GNUPLOT_TEMP_PATH);

	// 不要ファイルを削除
	//DeleteFile("tmp.png");

}

void CAccuracyEvaluation::DrawPicture(CString filePath) {

	CDC *cdc;

	// ピクチャコントロールを取得
	cdc = m_AccuracyPicture.GetDC();

	/** ピクチャコントロールの幅、高さを取得 **/
	RECT rect;
	int iWidth, iHeight;
	m_AccuracyPicture.GetWindowRect(&rect);
	// 幅
	iWidth = rect.right - rect.left;
	// 高さ
	iHeight = rect.bottom - rect.top;

	// 画像取得
	CImage img;
	HRESULT ret = img.Load(filePath);
	if (img.IsNull()) {
		MessageBox("** Error ** Cannot open output file", "warning", MB_OK | MB_ICONASTERISK);
		return;
	}

	// 伸縮率
	float stretchPercent;
	int imgHeight = img.GetHeight();
	stretchPercent = (float)iHeight / (float)imgHeight;
	// 伸縮後画像サイズ
	iWidth = img.GetWidth() * stretchPercent;
	iHeight = img.GetHeight() * stretchPercent;

	// 表示位置取得
	int drawPointX;
	drawPointX = ((rect.right - rect.left) - iWidth) / 2;

	CDC bmpDC;
	CBitmap *cbmp;

	cbmp = CBitmap::FromHandle(img);

	bmpDC.CreateCompatibleDC(cdc);

	CBitmap *oldbmp = bmpDC.SelectObject(cbmp);

	//伸縮すると画像が汚くなるので伸縮モードを指定
	//詳細はMSDN参照
	cdc->SetStretchBltMode(STRETCH_HALFTONE);

	//ブラシのずれを防止するためSetBrushOrgExを呼び出す
	cdc->SetBrushOrg(0, 0);

	//画像を伸縮してピクチャーボックスに表示
	//詳細はMSDN参照
	cdc->StretchBlt(drawPointX, 0, iWidth, iHeight, &bmpDC, 0, 0, img.GetWidth(), img.GetHeight(), SRCCOPY);
	bmpDC.SelectObject(oldbmp);
	//後片付け
	cbmp->DeleteObject();
	bmpDC.DeleteDC();
	ReleaseDC(cdc);

}

void CAccuracyEvaluation::OnStnClickedAccuracyPicture()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int a = 1;
	a = a + 1;
	return;
}

void CAccuracyEvaluation::OnStnClickedSectionPict()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}

/**
 * 河道断面、粗度係数、勾配データを読み込み
 * X軸、Y軸データを生成する。
**/
int CAccuracyEvaluation::CAccuracyKadoSodoKobai() {

	char		buf[SIZE_1K], placeName[SIZE_1K], sodokeisu[SIZE_1K], koubai[SIZE_1K], kijuntenKou[SIZE_1K], ksho[SIZE_1K], hyoko[SIZE_1K];
	int			v1, v2;

	struct CalcParamVal dummy;

	_mkdir(ObservationDir);

	// 河道断面、租度係数、勾配データ読み込み
	sprintf(fpath, "%s\\cross_section.txt", LocationDir);
	FILE* fp = fopen(fpath, "r");
	if (fp == NULL) {
		MessageBox("** Cannot find cross_section.txt file in selected folder.", "Fatal Error", MB_OK | MB_ICONHAND);
		return 1;
	}

	//// １行目　水位観測場所の名称読み込み
	//if (fgets(buf, SIZE_1K, fp) == NULL) {
	//	MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
	//	return 1;
	//}
	//sscanf(buf, "%s", placeName);

	// 1行目　1列目粗度係数，2列目河床勾配、基準点高　読み込み
	if (fgets(buf, SIZE_1K, fp) == NULL) {
		MessageBox("** File Format Error.", "Fatal Error", MB_OK | MB_ICONHAND);
		return 1;
	}
	sscanf(buf, "%s %s %s", sodokeisu, koubai, kijuntenKou);

	// 2行目移行読み込み
	LPCalcParamVal calcBuf[1024];

	int gyosu = 0;
	while (fgets(buf, SIZE_1K, fp) != NULL) {
		sscanf(buf, "%s %s", ksho, hyoko);
		sprintf(calcBuf[gyosu].x ,"%s", ksho);
		sprintf(calcBuf[gyosu].y, "%s", hyoko);
		gyosu++;
	}
	fclose(fp);

	gyosu = gyosu - 1;

	// 開始時刻取得
	int sy = atoi(subStr(startDt, 0, 4));
	int sM = atoi(subStr(startDt, 4, 6));
	int sd = atoi(subStr(startDt, 6, 8));
	int sh = atoi(subStr(startDt, 8, 10));
	int sm = atoi(subStr(startDt, 10, 12));
	int ss = 0;

	struct tm startmsStruct = { ss, sm, sh, sd, sM, sy - 1900};
	time_t startms = mktime(&startmsStruct);
	
	// 終了時刻取得
	int ey = atoi(subStr(endDt, 0, 4));
	int eM = atoi(subStr(endDt, 4, 6));
	int ed = atoi(subStr(endDt, 6, 8));
	int eh = atoi(subStr(endDt, 8, 10));
	int em = atoi(subStr(endDt, 10, 12));
	int es = 0;

	struct tm endmsStruct = { es, em, eh, ed, eM, ey - 1900 };
	time_t endms = mktime(&endmsStruct);

	// start - end 差を取得して、河道断面のデータ数で差を割る（当分での時間差を取得）
	time_t jikansa = (endms - startms) / gyosu;

	struct CalcParamVal *ptCalcParamVal = NULL;
	ptCalcParamVal = (struct CalcParamVal*) malloc(sizeof(struct CalcParamVal) * (gyosu + 1));
	memset(ptCalcParamVal, 0x00, sizeof(ptCalcParamVal));

	char* xchar = NULL;
	
	float ansval = 0.0;
	for (int n = 0; n < gyosu + 1; n++) {
		if (n == gyosu) {
			xchar = getTimeFmr(endms);
		} else {
			xchar = getTimeFmr(startms + (jikansa * n));
		}
		sprintf(ptCalcParamVal[n].x, "%s", xchar);
		ansval = atof(calcBuf[n].y);
		ansval = ansval - atof(kijuntenKou);
		if (ansval < 0) {
			int c = 0;
		}
		sprintf(ptCalcParamVal[n].y, "%2.1f", ansval);
	}

	sprintf(fpath, "%s\\RiverOut.csv", ObservationDir);
	FILE* kadoOut = fopen(fpath, "w");
	if (fp == NULL) {
		MessageBox("** Cannot create OUT/RiverOut.csv file in selected folder.", "Fatal Error", MB_OK | MB_ICONHAND);
		return 1;
	}
	for (int n = 0; n < gyosu + 1; n++) {
		fprintf(kadoOut, "%s,%s\n", ptCalcParamVal[n].x, ptCalcParamVal[n].y);
	}
	fclose(kadoOut);
	if (PtChar != NULL) {
		free(PtChar);
		PtChar = nullptr;
	}
	if (ptCalcParamVal != NULL) {
		free(ptCalcParamVal);
		ptCalcParamVal = nullptr;
	}

	return 0;

}

/**
 * ObsWL.csvを読み込み開始、終了時間に該当するデータを抽出し、
 * また、kijunsuii.txtから基準水位を読み込み
 * X軸、Y軸データを生成する。
**/
int CAccuracyEvaluation::CAccuracyObsWL() {

	char		dummy[SIZE_1K], Datetime[SIZE_1K], WL[SIZE_1K], hanrankiken[SIZE_1K], hinan[SIZE_1K], hanranchui[SIZE_1K], suibodan[SIZE_1K], buf[SIZE_1K];
	int			v1, v2;

	/**  kijunsuii.txtを読み込み **/
	char fpath[1024] = { '0' };
	sprintf(fpath, "%s/standard_wl.txt", LocationDir);

	FILE*		fp1 = fopen(fpath, "r");
	if (fp1 == NULL) {
		MessageBox("** Cannot find kijunsuii.txt file in selected folder.", "Fatal Error", MB_OK | MB_ICONHAND);
		return 1;
	}

	// １行目　氾濫危険水位
	if (fgets(buf, SIZE_1K, fp1) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		return 1;
	}
	sscanf(buf, "%s %s", dummy, hanrankiken);

	// ２行目　避難判断水位
	if (fgets(buf, SIZE_1K, fp1) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		return 1;
	}
	sscanf(buf, "%s %s", dummy, hinan);

	// ３行目　氾濫注意水位
	if (fgets(buf, SIZE_1K, fp1) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		return 1;
	}
	sscanf(buf, "%s %s", dummy, hanranchui);

	// ４行目　水防団待機水位
	if (fgets(buf, SIZE_1K, fp1) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		return 1;
	}
	sscanf(buf, "%s %s", dummy, suibodan);
	fclose(fp1);

	/**  ObsWL.csvを読み込み **/
	sprintf(fpath, "%s\\ObsWL.csv", LocationDir);
	FILE*		fp2 = fopen(fpath, "r");
	if (fp2 == NULL) {
		MessageBox("** Cannot find ObsWL.csv file in selected folder.", "Fatal Error", MB_OK | MB_ICONHAND);
		return 1;
	}

	// １行目　タイトル
	if (fgets(buf, SIZE_1K, fp2) == NULL) {
		MessageBox("** File Format Error (Place name not found.).", "Fatal Error", MB_OK | MB_ICONHAND);
		return 1;
	}

	// 日時、WL
	LPObsWL obsBuf[1024];

	int gyosu = 0;
	while (fgets(buf, SIZE_1K, fp2) != NULL) {
		sscanf(buf, "%[^,],%s", Datetime, WL);
		sprintf(obsBuf[gyosu].datetime, "%s", Datetime);

		// 日付フォーマット
		char *phy = subStr(Datetime, 0, 4);
		char hy[6];
		memcpy(hy, phy, 4);
		hy[4] = '/'; hy[5] = 0x00;
		char *phM = subStr(Datetime, 4, 6);
		char hM[4];
		memcpy(hM, phM, 2);
		hM[2] = '/'; hM[3] = 0x00;
		char *phd = subStr(Datetime, 6, 8);
		char hd[4];
		memcpy(hd, phd, 2);
		hd[2] = ' '; hd[3] = 0x00;
		char *phh = subStr(Datetime, 8, 10);
		char hh[4];
		memcpy(hh, phh, 2);
		hh[2] = ':'; hh[3] = 0x00;
		char *phm = subStr(Datetime, 10, 12);
		char hm[3];
		memcpy(hm, phm, 2);
		hm[2] = 0x00;

		sprintf(obsBuf[gyosu].datetimeStr, "%s", Datetime);

		sprintf(obsBuf[gyosu].kansokusuii, "%s", WL);
		sprintf(obsBuf[gyosu].hanrankiken, "%s", hanrankiken);
		sprintf(obsBuf[gyosu].hinan, "%s", hinan);
		sprintf(obsBuf[gyosu].hanranchui, "%s", hanranchui);
		sprintf(obsBuf[gyosu].suibodan, "%s", suibodan);
		gyosu++;
	}

	fclose(fp2);

	sprintf(fpath, "%s\\ObsWLOut.csv", ObservationDir);
	FILE* ObsWLOut = fopen(fpath, "w");
	if (ObsWLOut == NULL) {
		MessageBox("** Cannot create ./OUT/ObsWLOut.csv file in selected folder.", "Fatal Error", MB_OK | MB_ICONHAND);
		return 1;
	}
	long long bufi1 = 0;
	long long bufi2 = 0;
	long long bufi3 = 0;
	long long bufi4 = 0;
	for (int n = 0; n < gyosu + 1; n++) {
		if (atoll(startDt) <= atoll(obsBuf[n].datetime)
			&& atoll(obsBuf[n].datetime) <= atoll(endDt)) {
			fprintf(ObsWLOut, "%s,%s,%s,%s,%s,%s\n", 
				obsBuf[n].datetime, obsBuf[n].kansokusuii,
				obsBuf[n].hanrankiken, obsBuf[n].hinan,
				obsBuf[n].hanranchui, obsBuf[n].suibodan);

		}
	}
	fclose(ObsWLOut);
	if (PtChar != NULL) {
		free(PtChar);
		PtChar = nullptr;
	}

	return 0;

}

char* CAccuracyEvaluation::subStr(char* inputStr, int start, int end) {

//	if (PtChar != NULL) {
		free(PtChar);
		PtChar = nullptr;
//	}
	PtChar = (char *)malloc(sizeof(char)*(end - start + 1));

	for (int i = 0; i <= end - start; i++) {
		if (i == end - start) {
			PtChar[i] = 0x00;
		}
		else {
			PtChar[i] = inputStr[start + i];
		}
	}

	return PtChar;
}

char* CAccuracyEvaluation::getTimeFmr(time_t val) {

	struct tm* tmval = localtime(&val);
	
	if (PtChar != nullptr) {
		free(PtChar);
		PtChar = nullptr;
	}
	PtChar = (char *)malloc(sizeof(char) * 17);


	sprintf(PtChar, "%4d%02d%02d%02d%02d", tmval->tm_year + 1900, tmval->tm_mon,
		tmval->tm_mday, tmval->tm_hour, tmval->tm_min);

	return PtChar;
}

/*******************************************************
フォルダを指定(SelectFolder)
引数(IN)：
 hWnd ウインドウハンドル
 lpDefFolder デフォルトで表示するフォルダパス
 iFlg ダイアログオプション
 ttl ダイアログのタイトルテキスト
引数(OUT)：
 lpSelectPath 選択されたフォルダパス
*******************************************************/
BOOL SelectFolder(HWND hWnd, LPCTSTR lpDefFolder, LPTSTR lpSelectPath, UINT iFlg, CString ttl)
{
	LPMALLOC    pMalloc;
	BOOL        brtn = FALSE;


	//IMallocインターフェイスへのポインタを取得
	if (SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		BROWSEINFO        brows;
		ITEMIDLIST        *pIDlist;
		//BROWSEINFO構造体の初期化
		memset(&brows, NULL, sizeof(brows));
		//BROWSEINFO構造体に値をセット
		brows.hwndOwner = hWnd;
		brows.pszDisplayName = lpSelectPath;
		brows.lpszTitle = ttl;
		brows.lParam = (LPARAM)lpDefFolder;
		brows.pidlRoot = NULL;
		brows.ulFlags = iFlg;
		brows.lpfn = &BrowseCallbackProc;
		brows.iImage = (int)NULL;
		//フォルダ選択ダイアログの表示
		pIDlist = SHBrowseForFolder(&brows);
		if (pIDlist == NULL)
			//何も選択されなければデフォルトフォルダを返す
			strcpy_s(lpSelectPath, 512, lpDefFolder);
		else {
			//ITEMIDLIST からフルパスを得る
			SHGetPathFromIDList(pIDlist, lpSelectPath);
			brtn = TRUE;
			//ITEMIDLIST の開放
			pMalloc->Free(pIDlist);
		}
		pMalloc->Release();
	}
	return    brtn;
}
int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		//初期化処理(初期表示フォルダ指定)
		SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
		break;
	case BFFM_SELCHANGED:
		//フォルダ選択ダイアログでフォルダが選択された時に処理が必要であればここに記載
		TRACE("フォルダが選択されました¥r¥n");
		break;
	}
	return 0;
}

void replaceEnsign2Slash2(char* val) {

	for (int i = 0; i < strlen(val); i++) {

		if (val[i] == '\\') {
			val[i] = '/';
		}

	}
}

void CAccuracyEvaluation::OnBnClickedButtonCrossSec()
{
	CString fpath = "";
	char dstPath[1024], pathBuf[1024];
	memset(dstPath, NULL, sizeof(dstPath));
	memset(pathBuf, NULL, sizeof(pathBuf));

	strcat(dstPath, LocationDir);

	UpdateData(TRUE);

	strcat(pathBuf, DSET.Proj_Folder);
	if (!Button_File(TRUE, &fpath, pathBuf, "", "txt")) return;

	memset(pathBuf, NULL, sizeof(pathBuf));
	strcat(pathBuf, DSET.Proj_Folder);
	strcat(pathBuf, "Location");
	if (!File_Exist(pathBuf)) {
		mkdir(pathBuf);
	}

	if (!File_Exist(dstPath)) {
		mkdir(dstPath);
	}
	sprintf(dstPath, "%s\\cross_section.txt", dstPath);

	CopyFile(fpath, dstPath, false);

	CButton* cb = (CButton*)GetDlgItem(IDC_CHECK_CS);
	cb->SetCheck(true);
	return;
}


void CAccuracyEvaluation::OnBnClickedButtonRecalc()
{
	int ret = 0;

	this->UpdateStartDt();
	this->UpdateEndDt();

	// 基本データ作成
#ifndef SIMP_VER_TBD
	ret = CAccuracyKadoSodoKobai();
#endif
	if (ret == 0) {
		ret = CAccuracyObsWL();
		if (ret != 0) {
			return;
		}

		ret = CreatePltFile();
		if (ret != 0) {
			return;
		}

/*		ret = CreatePlot();
		if (ret != 0) {
			return;
		}*/
	}

	DispGraph(startDt, endDt);
}


void CAccuracyEvaluation::OnBnClickedButtonHqeq()
{
	CString fpath = "";
	char dstPath[1024], pathBuf[1024];
	
	memset(dstPath, NULL, sizeof(dstPath));
	memset(pathBuf, NULL, sizeof(pathBuf));

	strcat(dstPath, LocationDir);

	UpdateData(TRUE);

	strcat(pathBuf, DSET.Proj_Folder);
	if (!Button_File(TRUE, &fpath, pathBuf, "", "ini")) return;

	memset(pathBuf, NULL, sizeof(pathBuf));
	strcat(pathBuf, DSET.Proj_Folder);
	strcat(pathBuf, "Location");
	if (!File_Exist(pathBuf)) {
		mkdir(pathBuf);
	}

	if (!File_Exist(dstPath)) {
		mkdir(dstPath);
	}
	sprintf(dstPath, "%s\\HQeq.ini", dstPath);

	CopyFile(fpath, dstPath, false);

	CButton* cb = (CButton*)GetDlgItem(IDC_CHECK_HQ);
	cb->SetCheck(true);
	return;
}

void CAccuracyEvaluation::OnBnClickedButtonPfPrediction()
{
	char cDirSel[256] = {};
	char dstPath[1024], pathBuf[1024];
	CString csDstPath;
	CString srcPath;
	char cdir[255];

	memset(cdir, NULL, sizeof(cdir));
	memset(dstPath, NULL, sizeof(dstPath));

	GetCurrentDirectory(255, cdir);

	strcat(dstPath, LocationDir);

	UpdateData(TRUE);

	//フォルダ選択ダイアログオプション：
	//BIF_NEWDIALOGSTYLE(新規フォルダ作成できる)
	//BIF_RETURNONLYFSDIRS(ファイルシステムのディレクトリのみ選択できる)
	memset(cDirSel, NULL, sizeof(cDirSel));
	srcPath.Format("%s", cdir);

	if (SelectFolder(this->m_hWnd, srcPath, cDirSel, BIF_RETURNONLYFSDIRS, "Select river data folder."))
	{
		srcPath = cDirSel;
		UpdateData(FALSE);
	}

	memset(pathBuf, NULL, sizeof(pathBuf));
	strcat(pathBuf, DSET.Proj_Folder);
	strcat(pathBuf, "Location");
	if (!File_Exist(pathBuf)) {
		mkdir(pathBuf);
	}

	if (!File_Exist(dstPath)) {
		mkdir(dstPath);
	}
	sprintf(dstPath, "%s\\PF_Prediction", dstPath);

	csDstPath.Format("%s", dstPath);
	folderCopy(srcPath, csDstPath);

	CButton* cb = (CButton*)GetDlgItem(IDC_CHECK_PF);
	cb->SetCheck(true);
	return;
}

int folderCopy(CString from, CString to)
{
	char cfrom[1024];
	char cto[1024];

	memset(cfrom, NULL, sizeof(cfrom));
	memset(cto, NULL, sizeof(cto));

	strcat(cfrom, from);
	strcat(cto, to);

	strcat(cfrom, "\\*.*\0");
	strcat(cto, "\\\0");

	SHFILEOPSTRUCT tSHFile;
	ZeroMemory(&tSHFile, sizeof(SHFILEOPSTRUCT));
	tSHFile.hwnd = ::GetDesktopWindow();
	tSHFile.wFunc = FO_COPY;
	tSHFile.fFlags = FOF_MULTIDESTFILES | FOF_NOCONFIRMMKDIR;
	tSHFile.fAnyOperationsAborted = TRUE;
	tSHFile.hNameMappings = NULL;
	tSHFile.lpszProgressTitle = "";

	//from.SetAt(from.GetLength(), NULL);
	//to.SetAt(to.GetLength(), NULL);
	tSHFile.pFrom = cfrom;
	tSHFile.pTo = cto;

	return SHFileOperation(&tSHFile); // 実行  
}

/**
* 実測、予測水位データ作成(CalcWL)。
**/
void CAccuracyEvaluation::createCalcWL_Data(char* startDt2, char* endDt2, int waterlvFlg) {

	char fpath[1024], InputFile[1024];
	char outdirpath2[1024];
	char		buf[SIZE_1K], Datetime[SIZE_1K], WL[SIZE_1K];
	char cdir[255], inputFiles[1024];
	CString msgbuf;
	char title[128];

	memset(fpath, NULL, sizeof(fpath));
	memset(InputFile, NULL, sizeof(InputFile));
	memset(outdirpath2, NULL, sizeof(outdirpath2));
	memset(cdir, NULL, sizeof(cdir));
	memset(inputFiles, NULL, sizeof(inputFiles));
	memset(title, NULL, sizeof(title));

	strcat(title, "Location : ");
	strcat(title, DSET.LocationName);

	GetCurrentDirectory(255, cdir);

	std::vector<std::string> files;

	strcat(fpath, DSET.Proj_Folder);
	strcat(fpath, "\\out");
	//files = getFilePathInDir(fpath, "out");

	CString time;
	double wlval;
	int gyosu = 0;
	// 日時、WL
	LPObsWL obsBuf[1024];

	std::string outDir(fpath);
	auto dtStart = DSET.CT;
	auto dtNums = DSET.DT_Out;
	for (auto n = 0; n < dtNums; n++) {
		auto curDt = dtStart + CTimeSpan(DSET.DT_Inter * n);
		auto fPath = getDataFilePath(outDir, "hr_", n+1);

		if (fPath.empty()) continue;

		wlval = readHrHsQr(fPath.c_str(), DSET.LocationX, DSET.LocationY);
		time = curDt.Format("%Y%m%d%H%M");
		sprintf(obsBuf[gyosu].datetime, "%s", time);
		sprintf(obsBuf[gyosu].kansokusuii, "%0.2f", wlval);
		gyosu++;
	}

	// 出力先ディレクトリ作成
	GetCurrentDirectory(255, cdir);
	// ファイルを開く
	sprintf(outdirpath, "%s\\%s", cdir, OUT_WATERLEVEL_DIR);
	_mkdir(outdirpath);
	_mkdir(CalcWLDir);
	sprintf(fpath, "%s\\CalcWLOut.csv", CalcWLDir);
	FILE* CalcWLOut = fopen(fpath, "w");
	if (CalcWLOut == NULL) {
		msgbuf.Format("** Cannot create %s file in selected folder.", fpath);
		MessageBox(msgbuf, "Fatal Error", MB_OK | MB_ICONHAND);
		exit(0);
	}

	for (int n = 0; n < gyosu + 1; n++) {
		auto curDt = atoll(obsBuf[n].datetime);
		if (atoll(startDt2) <= curDt && curDt <= atoll(endDt2)) {
			fprintf(CalcWLOut, "%s,%s\n", obsBuf[n].datetime, obsBuf[n].kansokusuii);
		}
	}
	fclose(CalcWLOut);

	// ファイルを開く
	sprintf(fpath, "%s\\CalcWL.plt", CalcWLDir);
	FILE* foutPlt = fopen(fpath, "w");

	sprintf(fpath, "%s\\CalcWLOut.csv", CalcWLDir);

	fprintf(foutPlt, "%s", "set datafile separator ','\n");
	fprintf(foutPlt, "%s%s%s", "set title \"", title, "\"\n");
	fprintf(foutPlt, "%s", "set title font\"Arial,10\"\n");
	fprintf(foutPlt, "%s", "set xlabel \"Date Time\"\n");
	fprintf(foutPlt, "%s", "set ylabel \"Water level[m]\"\n");
	fprintf(foutPlt, "%s", "set xlabel font \"Arial,8\"\n");
	fprintf(foutPlt, "%s", "set ylabel font \"Arial,8\"\n");
	fprintf(foutPlt, "%s", "set tics font \"Arial,8\"\n");
	fprintf(foutPlt, "%s", "set key font \"Arial,8\"\n");
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
	replaceEnsign2Slash2(fpath);
	fprintf(foutPlt, "plot \"%s\" using 1:2 with lines lw 1 linecolor rgb \"blue\" title \"%s\"", fpath, Calculated_water_level2);

	char path[1024];
	sprintf(path, "%s", ObservationDir);
	sprintf(fpath, "%s\\ObsWLOut.csv", ObservationDir);
	replaceEnsign2Slash2(path);
	if (PathFileExists(fpath)) {
		replaceEnsign2Slash2(path);
		fprintf(foutPlt, ",\\\n");
		if (waterlvFlg == BST_CHECKED) {
			fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:2 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"black\" title \"%s\", \\\n", path, Observation_water_level2);
		}
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:6 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"green\" title \"%s\", \\\n", path, Stand_by_water_level_for_Flood_Defense2);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:5 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"yellow\" title \"%s\", \\\n", path, Flood_warning_water_level2);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:4 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"orange\" title \"%s\", \\\n", path, Evacuation_judgment_water_level2);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:3 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"red\" title \"%s\"", path, Flood_inundation_water_level2);
	}
#ifndef SIMP_VER_TBD
	sprintf(fpath, "%s\\RiverOut.csv", ObservationDir);
	if (PathFileExists(fpath)) {
		sprintf(path, "%s", ObservationDir);
		replaceEnsign2Slash2(path);
		fprintf(foutPlt, ", \\\n");
		fprintf(foutPlt, "\"%s/RiverOut.csv\" using 1:2 axis x1y1 with lines lw 1 linecolor rgb \"gray\" title \"%s\"\n", path, River_bed2);
	}
#endif
	fclose(foutPlt);
}

/**
* 実測、予測水位データ作成(CalcQ)。
**/
void CAccuracyEvaluation::createCalcQ_Data(char* startDt2, char* endDt2, int waterlvFlg) {

	char fpath[1024];

	char buf[SIZE_05K], nHQ[SIZE_05K], a1[SIZE_05K], b1[SIZE_05K], Hmax1[SIZE_05K],
		a2[SIZE_05K], b2[SIZE_05K], Hmax2[SIZE_05K], dummy[SIZE_05K],
		Datetime[SIZE_05K], WL[SIZE_05K];

	double fa1, fb1, fHmax1, fa2, fb2, fHmax2;
	int nHQNum;

	char cdir[255];
	GetCurrentDirectory(255, cdir);

	char outdirpath2[1024];
	char hqeqPath[1024];
	CString calcqPath = "";

	char inputFiles[1024];
	std::vector<std::string> files;

	char title[128];
	memset(title, NULL, sizeof(title));
	strcat(title, "Location : ");
	strcat(title, DSET.LocationName);

	memset(hqeqPath, NULL, sizeof(hqeqPath));
	strcat(hqeqPath, LocationDir);
	strcat(hqeqPath, "\\HQeq.ini");

	if (!File_Exist(hqeqPath)) {
		MessageBox("** Error ** HQeq.ini file is not found.", "Fatal Error", MB_OK | MB_ICONASTERISK);
		return;
	}

	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  読込  HQeq.ini
	FILE*		fpHqIni = fopen(hqeqPath, "r");
	if (fpHqIni == NULL) {
		MessageBox("** Error ** HQeq file is not found.", "Fatal Error", MB_OK | MB_ICONHAND);
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

	memset(fpath, NULL, sizeof(fpath));
	strcat(fpath, DSET.Proj_Folder);
	strcat(fpath, "\\out");
	files = getFilePathInDir(fpath, "out");

	// 日時、WL
	LPObsWL obsBuf[1024];

	CString time;
	double wlval;
	int gyosu = 0;

	std::string outDir(fpath);
	auto dtStart = DSET.CT;
	auto dtNums = DSET.DT_Out;
	for (auto n = 0; n < dtNums; n++) {
		auto curDt = dtStart + CTimeSpan(DSET.DT_Inter * n);
		auto fPath = getDataFilePath(outDir, "qr_", n + 1);

		if (fPath.empty()) continue;

		wlval = readHrHsQr(fPath.c_str(), DSET.LocationX, DSET.LocationY);
		time = curDt.Format("%Y%m%d%H%M");
		sprintf(obsBuf[gyosu].datetime, "%s", time);
		sprintf(obsBuf[gyosu].kansokusuii, "%0.2f", wlval);
		gyosu++;
	}

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

			if (orgVal < 0) {
				orgVal = orgVal * -1;
			}

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
	fprintf(foutPlt, "%s%s%s", "set title \"", title, "\"\n");
	fprintf(foutPlt, "%s", "set title font\"Arial,10\"\n");
	fprintf(foutPlt, "%s", "set xlabel \"Date Time\"\n");
	fprintf(foutPlt, "%s", "set ylabel \"Water level[m]\"\n");
	fprintf(foutPlt, "%s", "set xlabel font \"Arial,8\"\n");
	fprintf(foutPlt, "%s", "set ylabel font \"Arial,8\"\n");
	fprintf(foutPlt, "%s", "set tics font \"Arial,8\"\n");
	fprintf(foutPlt, "%s", "set key font \"Arial,8\"\n");
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
	replaceEnsign2Slash2(fpath);
	fprintf(foutPlt, "plot \"%s\" using 1:2 with lines lw 1 linecolor rgb \"blue\" title \"%s\"", fpath, Calculated_water_level2);

	char path[1024];
	sprintf(path, "%s\\%s\\observation", cdir, OUT_WATERLEVEL_DIR);
	sprintf(fpath, "%s\\%s\\observation\\ObsWLOut.csv", cdir, OUT_WATERLEVEL_DIR);
	replaceEnsign2Slash2(path);
	if (PathFileExists(fpath)) {
		fprintf(foutPlt, ",\\\n");
		if (waterlvFlg == BST_CHECKED) {
			fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:2 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"black\" title \"%s\", \\\n", path, Observation_water_level2);
		}
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:6 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"green\" title \"%s\", \\\n", path, Stand_by_water_level_for_Flood_Defense2);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:5 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"yellow\" title \"%s\", \\\n", path, Flood_warning_water_level2);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:4 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"orange\" title \"%s\", \\\n", path, Evacuation_judgment_water_level2);
		fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:3 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"red\" title \"%s\"", path, Flood_inundation_water_level2);
	}
#ifndef SIMP_VER_TBD
	sprintf(fpath, "%s\\%s\\observation\\RiverOut.csv", cdir, OUT_WATERLEVEL_DIR);
	if (PathFileExists(fpath)) {
		fprintf(foutPlt, ", \\\n");
		fprintf(foutPlt, "\"%s/RiverOut.csv\" using 1:2 axis x1y1 with lines lw 1 linecolor rgb \"gray\" title \"%s\"\n", path, River_bed2);
	}
#endif
	fclose(foutPlt);
}

/**
* 実測、予測水位データ作成(CalcWLRRI_Predicted)。
**/
bool CAccuracyEvaluation::createCalcWLRRI_Predicted_Data(char* startDt2, char* endDt2, int waterlvFlg) {

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
	CString bufpath;

	char title[128];
	memset(title, NULL, sizeof(title));
	strcat(title, "Location : ");
	strcat(title, DSET.LocationName);

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

	bufpath.Format("%s", LocationDir);
	std::string findPath = bufpath + "\\PF_Prediction";
	std::string ext = "csv";
	files = getFilePathInDir(findPath, ext);

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
		bufpath.Format("%s", LocationDir);
		sprintf(fpath, "%s\\%s", bufpath + "\\PF_Prediction", files[i].c_str());
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

	bool rs = false;
	do {
		if (hyfiles_cnt == 0) break;

		// ファイルを開く
		sprintf(fpath, "%s\\CalcWL(RRI-Predicted).plt", outdirpath2);
		FILE* foutPlt = fopen(fpath, "w");

		sprintf(fpath, "%s\\HJ_%s_%s.csv", outdirpath2, startt, btime);
		rename(fpathJ, fpath);

		fprintf(foutPlt, "%s", "set datafile separator ','\n");
		fprintf(foutPlt, "%s%s%s", "set title \"", title, "\"\n");
		fprintf(foutPlt, "%s", "set title font\"Arial,10\"\n");
		fprintf(foutPlt, "%s", "set xlabel \"Date Time\"\n");
		fprintf(foutPlt, "%s", "set ylabel \"Water level[m]\"\n");
		fprintf(foutPlt, "%s", "set xlabel font \"Arial,8\"\n");
		fprintf(foutPlt, "%s", "set ylabel font \"Arial,8\"\n");
		fprintf(foutPlt, "%s", "set tics font \"Arial,8\"\n");
		fprintf(foutPlt, "%s", "set key font \"Arial,8\"\n");
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
		replaceEnsign2Slash2(fpath);
		fprintf(foutPlt, "plot \"%s\" using 1:2 with lines lw 1 linecolor rgb \"blue\" title \"%s\", \\\n", fpath, Calculated_water_level2);

		int lcnt = 0;
		for (int i = 0; i < hyfiles_cnt - 1; i++) {
			replaceEnsign2Slash2(hyfiles[i]);
			if (i == 0) {
				fprintf(foutPlt, "\"%s\" using 1:2 with lines lw 1 linecolor rgb \"red\" title \"%s\", \\\n", hyfiles[i], Predicted_water_level2);
			}
			else {
				fprintf(foutPlt, "\"%s\" %s", hyfiles[i], "using 1:2 with lines lw 1 linecolor rgb \"pink\" notitle, \\\n");
			}
			free(hyfiles[i]);
			lcnt++;
		}
		replaceEnsign2Slash2(hyfiles[lcnt]);
		fprintf(foutPlt, "\"%s\" %s", hyfiles[lcnt], "using 1:2 with lines lw 1 linecolor rgb \"pink\" notitle");

		char path[1024];
		sprintf(path, "%s\\%s\\observation", cdir, OUT_WATERLEVEL_DIR);
		sprintf(fpath, "%s\\%s\\observation\\ObsWLOut.csv", cdir, OUT_WATERLEVEL_DIR);
		replaceEnsign2Slash2(path);
		if (PathFileExists(fpath)) {
			fprintf(foutPlt, ",\\\n");
			if (waterlvFlg == BST_CHECKED) {
				fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:2 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"black\" title \"%s\", \\\n", path, Observation_water_level2);
			}
			fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:6 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"green\" title \"%s\", \\\n", path, Stand_by_water_level_for_Flood_Defense2);
			fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:5 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"yellow\" title \"%s\", \\\n", path, Flood_warning_water_level2);
			fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:4 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"orange\" title \"%s\", \\\n", path, Evacuation_judgment_water_level2);
			fprintf(foutPlt, "\"%s/ObsWLOut.csv\" using 1:3 axis x1y1 with points pt 7 ps 0.3 linecolor rgb \"red\" title \"%s\"", path, Flood_inundation_water_level2);
		}
#ifndef SIMP_VER_TBD
		sprintf(fpath, "%s\\%s\\observation\\RiverOut.csv", cdir, OUT_WATERLEVEL_DIR);
		if (PathFileExists(fpath)) {
			fprintf(foutPlt, ", \\\n");
			fprintf(foutPlt, "\"%s/RiverOut.csv\" using 1:2 axis x1y1 with lines lw 1 linecolor rgb \"gray\" title \"%s\"\n", path, River_bed2);
		}
#endif
		fclose(foutPlt);
		free(hyfiles[lcnt]);
		
		rs = true;
	} while (0);

	free(hyfiles);
	return rs;
}

void CAccuracyEvaluation::DispGraph(char* startDt2, char* endDt2) {

	int waterlvFlg = 0;
	int hrRadioFlg = 0;
	int qrRadioFlg = 0;
	int pfRadioFlg = 0;

	CButton* cb = (CButton *)GetDlgItem(IDC_V_CHECK_WATER_LEVEL);
	waterlvFlg = cb->GetCheck();

	cb = (CButton *)GetDlgItem(IDC_RADIO_HR);
	hrRadioFlg = cb->GetCheck();

	cb = (CButton *)GetDlgItem(IDC_RADIO_QR);
	qrRadioFlg = cb->GetCheck();

	cb = (CButton *)GetDlgItem(IDC_RADIO_PF);
	pfRadioFlg = cb->GetCheck();

	if (hrRadioFlg == BST_CHECKED) {
		createCalcWL_Data(startDt2, endDt2, waterlvFlg);
		CreatePlotCalcWL();
	}
	else if (qrRadioFlg == BST_CHECKED) {
		createCalcQ_Data(startDt2, endDt2, waterlvFlg);
		CreatePlotCalcQ();
	}
	else if (pfRadioFlg == BST_CHECKED) {
	#ifndef SIMP_VER
		if (createCalcWLRRI_Predicted_Data(startDt2, endDt2, waterlvFlg)) {
			CreatePlotCalcWL_RRI_Predicted();
		} else {
			MessageBox("Prediction calculation failed, please check if the set date range is valid", "Error", MB_OK | MB_ICONHAND);
		}
	#endif // !SIMP_VER
	}

}

std::vector<std::string> getFilePathInDir(const std::string& dir_name, const std::string& extension) noexcept(false)
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
				//printf("%s\n", file_names.back().c_str());

			}
		} while (FindNextFile(hFind, &win32fd));

	}
	catch (...) {

	}

	FindClose(hFind);

	return file_names;
}

std::string getDataFilePath(const std::string& dir_name, const std::string& prefix, int id) noexcept
{
	std::stringstream ss;
	ss << std::setw(6) << std::setfill('0') << id;
	auto strId = ss.str();
	auto filePath = dir_name + "/" + prefix + ss.str() + ".out";
	if (File_Exist(filePath.c_str())) return filePath;
	return "";
}

double readHrHsQr(const char* fileNm, int posx, int posy)
{
	FILE*		fp;
	char			buf[SIZE_16K], inchar[64];
	int			lcnt = 0, FV, FH, pos;
	int			id, year, month, day, hour, min;
	CString		str, str1, str2;

	if ((fp = fopen(fileNm, "r")) == NULL) {
		str.Format("*** ERROR ** Cannot Open Out File...[ %s ] !", fileNm);
		return E_FAIL;
	}
	//
	double		ret;
	BOOL	breakFlg = false;
	//
	for (int v = 1; v <= PM.MVnum; v++) {
		fgets(buf, SIZE_16K, fp);						// Date
		pos = 0;
		for (int h = 1; h <= PM.MHnum; h++) {
			getstr(buf, &pos, inchar, SIZE_16K);
			if (posx == h && posy == v) {
				ret = atof(inchar);
				breakFlg = true;
				break;
			}
		}

		if (breakFlg) {
			break;
		}
	}

	fclose(fp);

	return ret;

}

void CAccuracyEvaluation::UpdateStartDt()
{
	char bufY[20], bufM[20], bufD[20], bufH[20];

	GetDlgItemText(IDC_EDIT_STARTTIME_Y, bufY, 20);
	GetDlgItemText(IDC_EDIT_STARTTIME_M, bufM, 20);
	GetDlgItemText(IDC_EDIT_STARTTIME_D, bufD, 20);
	GetDlgItemText(IDC_EDIT_STARTTIME_H, bufH, 20);
	sprintf(startDt, "%04s%02s%02s%02s00", bufY, bufM, bufD, bufH);
}

void CAccuracyEvaluation::UpdateEndDt()
{
	char bufY[20], bufM[20], bufD[20], bufH[20];

	GetDlgItemText(IDC_EDIT_ENDTIME_Y, bufY, 20);
	GetDlgItemText(IDC_EDIT_ENDTIME_M, bufM, 20);
	GetDlgItemText(IDC_EDIT_ENDTIME_D, bufD, 20);
	GetDlgItemText(IDC_EDIT_ENDTIME_H, bufH, 20);
	sprintf(endDt, "%04s%02s%02s%02s00", bufY, bufM, bufD, bufH);
}

void CAccuracyEvaluation::CustomizeCtrls()
{
#ifdef SIMP_VER
	std::array<int, 6> hideCtrlIDs{ {IDC_CHECK_HQ , IDC_BUTTON_HQEQ,
		IDC_CHECK_PF, IDC_BUTTON_PF_PREDICTION, IDC_RADIO_QR, IDC_RADIO_PF} };

	for (const auto id : hideCtrlIDs) {
		CWnd* wnd = GetDlgItem(id);
		if (wnd != nullptr) wnd->ShowWindow(SW_HIDE);
	}

	this->MoveCtrl(IDC_CHECK_CS, IDC_CHECK_HQ);
	this->MoveCtrl(IDC_BUTTON_CROSS_SEC, IDC_BUTTON_HQEQ);
	
#endif // SIMP_VER
}

void CAccuracyEvaluation::MoveCtrl(int srcID, int tarID)
{
	RECT rect;
	GetDlgItem(tarID)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	GetDlgItem(srcID)->MoveWindow(&rect);
}
