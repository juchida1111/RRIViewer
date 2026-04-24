#include "ErrorRate.h"
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
#include <map>
#include <string>
#include <stdexcept>

//int			ERT_Mnum = 1;
//int			ERT_Mnum[] = { IDC_SECTION_PICT };

extern      DATASET		DSET;						// 上下流ファイルセット
extern      int			COUT;						// どの出力データを対象としているか

extern		int			Image_Sub, Option_Sub;
// Common Counter for HARD_COPY
extern      int			Cnt_Prof, Cnt_MProf, Cnt_Hydro;
extern      char			m_CurrentPath[SIZE_05K];
extern      char			m_RecentPath[SIZE_05K];

CString		m_csPath3 = "";
char		startDt3[20];
char		endDt3[20];
int			dispNo;
// CErrorRate ダイアログ

IMPLEMENT_DYNAMIC(CErrorRate, CDialog)

//プロトタイプ宣言
BOOL SelectFolder2(HWND hWnd, LPCTSTR lpDefFolder, LPTSTR lpSelectPath, UINT iFlg, CString ttl);
int CALLBACK BrowseCallbackProc2(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

//---------------------------------------------------------------------------------
//   メッセージマップ
//---------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CErrorRate, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ERRORRATE_LIST, &CErrorRate::OnLvnItemchangedErrorrateList)
	ON_WM_SIZE()
END_MESSAGE_MAP()

decltype(auto) getObFilePath(LPCTSTR filename) {
	std::string _fObsPath = m_csPath3.GetString();
	_fObsPath += "\\";
	_fObsPath += filename;
	return _fObsPath;
}

void CErrorRate::recreateDir(CString deldir) {
	deldir += "?";
	deldir.SetAt(deldir.GetLength() - 1, 0);
	SHFILEOPSTRUCT drive;
	drive.hwnd = this->m_hWnd;
	drive.wFunc = FO_DELETE;
	drive.pFrom = deldir;
	drive.pTo = NULL;
	drive.fFlags = FOF_ALLOWUNDO;
	SHFileOperation(&drive);
	auto rs = _mkdir(deldir);
}

CErrorRate::CErrorRate(CWnd* pParent /*=NULL*/)
	: CDialog(CErrorRate::IDD, pParent)
{

}

CErrorRate::CErrorRate(CWnd* pParent /*=NULL*/, char* startdt, char* enddt, CString workDir, int selectedNo)
	: CDialog(CErrorRate::IDD, pParent)
{
	sprintf(startDt3, "%s", startdt);
	sprintf(endDt3, "%s", enddt);
	m_csPath3 = workDir;
	dispNo = selectedNo;
}

CErrorRate::~CErrorRate()
{
}

void CErrorRate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ERRORRATE_LIST, m_ErrorRateList);
}

std::vector<std::string> get_file_path_in_dir(const std::string& dir_name, const std::string& extension) noexcept(false);
void replaceEnsign2Slash(char* val);

//---------------------------------------------------------------------------------
//   初期化
//---------------------------------------------------------------------------------
// CErrorRate メッセージ ハンドラー
BOOL CErrorRate::OnInitDialog()
{

	CDialog::OnInitDialog();

	char cDirSel[512] = { '¥0' };
	//UpdateData(TRUE);

#ifdef SIMP_VER
	createDataSimpVer();
	createErrorDataSimpVer();
#else
	createData();
	createErrorData();
#endif

	if (WinTitle.GetLength() != 0) this->SetWindowTextA(WinTitle);

	char fpath[SIZE_1K], outdirpath2[SIZE_1K], buf[SIZE_1K], title[SIZE_1K], jikyo[SIZE_1K];
	char cdir[255];
	GetCurrentDirectory(255, cdir);

#ifdef SIMP_VER
	constexpr int arr_count = 1;
#else
	constexpr int arr_count = 7;
	char oneh[SIZE_1K], twoh[SIZE_1K], threeh[SIZE_1K], fourh[SIZE_1K], fiveh[SIZE_1K], sixh[SIZE_1K];
#endif
	double rmse[arr_count], peak[arr_count], nash[arr_count];

	// ファイルを開く
	sprintf(outdirpath2, "%s\\%s\\error_rate", cdir, OUT_WATERLEVEL_DIR);
	sprintf(fpath, "%s\\result.csv", outdirpath2);
	FILE* foutresult = fopen(fpath, "r");

	int loopcnt = 0;
	while (fgets(buf, SIZE_1K, foutresult) != NULL) {
#ifdef SIMP_VER
		sscanf(buf, "%s %s",title, jikyo);
#else
		sscanf(buf, "%s %[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",
			title, jikyo, oneh, twoh, threeh, fourh, fiveh, sixh);
#endif

		switch (loopcnt) {
		case 0:
			rmse[0] = atof(jikyo);
#ifndef SIMP_VER
			rmse[1] = atof(oneh);
			rmse[2] = atof(twoh);
			rmse[3] = atof(threeh);
			rmse[4] = atof(fourh);
			rmse[5] = atof(fiveh);
			rmse[6] = atof(sixh);
#endif
		case 1:
			peak[0] = atof(jikyo);
#ifndef SIMP_VER
			peak[1] = atof(oneh);
			peak[2] = atof(twoh);
			peak[3] = atof(threeh);
			peak[4] = atof(fourh);
			peak[5] = atof(fiveh);
			peak[6] = atof(sixh);
#endif
		case 2:
			nash[0] = atof(jikyo);
#ifndef SIMP_VER
			nash[1] = atof(oneh);
			nash[2] = atof(twoh);
			nash[3] = atof(threeh);
			nash[4] = atof(fourh);
			nash[5] = atof(fiveh);
			nash[6] = atof(sixh);
#endif
		}
		loopcnt++;
	}


	fclose(foutresult);
	
	// 誤差率一覧の作成
	// 列の定義
	m_ErrorRateList.InsertColumn(0, "     時制    ", LVCFMT_LEFT, 100);
	m_ErrorRateList.InsertColumn(1, "     RMSE     ", LVCFMT_RIGHT, 100);
	m_ErrorRateList.InsertColumn(2, "  ピーク誤差  ", LVCFMT_RIGHT, 100);
	m_ErrorRateList.InsertColumn(3, "     Nash     ", LVCFMT_RIGHT, 100);

	// 行を挿入
	m_ErrorRateList.InsertItem(0, _T("実況"), LVCFMT_RIGHT);
	sprintf(buf, "%.4f", rmse[0]);
	m_ErrorRateList.SetItemText(0, 1, _T(buf));
	sprintf(buf, "%.4f", peak[0]);
	m_ErrorRateList.SetItemText(0, 2, _T(buf));
	sprintf(buf, "%.4f", nash[0]);
	m_ErrorRateList.SetItemText(0, 3, _T(buf));

#ifndef SIMP_VER
	// 行を挿入
	m_ErrorRateList.InsertItem(1, _T("１時間先"), LVCFMT_RIGHT);
	sprintf(buf, "%.4f", rmse[1]);
	m_ErrorRateList.SetItemText(1, 1, _T(buf));
	sprintf(buf, "%.4f", peak[1]);
	m_ErrorRateList.SetItemText(1, 2, _T(buf));
	sprintf(buf, "%.4f", nash[1]);
	m_ErrorRateList.SetItemText(1, 3, _T(buf));

	// 行を挿入
	m_ErrorRateList.InsertItem(2, _T("２時間先"), LVCFMT_RIGHT);
	sprintf(buf, "%.4f", rmse[2]);
	m_ErrorRateList.SetItemText(2, 1, _T(buf));
	sprintf(buf, "%.4f", peak[2]);
	m_ErrorRateList.SetItemText(2, 2, _T(buf));
	sprintf(buf, "%.4f", nash[2]);
	m_ErrorRateList.SetItemText(2, 3, _T(buf));

	// 行を挿入
	m_ErrorRateList.InsertItem(3, _T("３時間先"), LVCFMT_RIGHT);
	sprintf(buf, "%.4f", rmse[3]);
	m_ErrorRateList.SetItemText(3, 1, _T(buf));
	sprintf(buf, "%.4f", peak[3]);
	m_ErrorRateList.SetItemText(3, 2, _T(buf));
	sprintf(buf, "%.4f", nash[3]);
	m_ErrorRateList.SetItemText(3, 3, _T(buf));

	// 行を挿入
	m_ErrorRateList.InsertItem(4, _T("４時間先"), LVCFMT_RIGHT);
	sprintf(buf, "%.4f", rmse[4]);
	m_ErrorRateList.SetItemText(4, 1, _T(buf));
	sprintf(buf, "%.4f", peak[4]);
	m_ErrorRateList.SetItemText(4, 2, _T(buf));
	sprintf(buf, "%.4f", nash[4]);
	m_ErrorRateList.SetItemText(4, 3, _T(buf));

	// 行を挿入
	m_ErrorRateList.InsertItem(5, _T("５時間先"), LVCFMT_RIGHT);
	sprintf(buf, "%.4f", rmse[5]);
	m_ErrorRateList.SetItemText(5, 1, _T(buf));
	sprintf(buf, "%.4f", peak[5]);
	m_ErrorRateList.SetItemText(5, 2, _T(buf));
	sprintf(buf, "%.4f", nash[5]);
	m_ErrorRateList.SetItemText(5, 3, _T(buf));
	
	// 行を挿入
	m_ErrorRateList.InsertItem(6, _T("６時間先"), LVCFMT_RIGHT);
	sprintf(buf, "%.4f", rmse[6]);
	m_ErrorRateList.SetItemText(6, 1, _T(buf));
	sprintf(buf, "%.4f", peak[6]);
	m_ErrorRateList.SetItemText(6, 2, _T(buf));
	sprintf(buf, "%.4f", nash[6]);
	m_ErrorRateList.SetItemText(6, 3, _T(buf));
#endif

	m_ErrorRateList.SetColumnWidth(0, -2);
	m_ErrorRateList.SetColumnWidth(1, -2);
	m_ErrorRateList.SetColumnWidth(2, -2);
	m_ErrorRateList.SetColumnWidth(3, -2);

	m_ErrorRateList.SetExtendedStyle(m_ErrorRateList.GetExtendedStyle() | LVS_EX_GRIDLINES);

	AdjustLayout();

	return TRUE;

}

void CErrorRate::createData() {

	char obsPath[1024];
	char fpath[1024];
	char startt[1024];
	char** hyfiles;
	int hyfiles_cnt = 0;

	memset(obsPath, 0x00, sizeof(obsPath));
	memset(fpath, 0x00, sizeof(fpath));
	memset(startt, 0x00, sizeof(startt));

	hyfiles = (char**)malloc(sizeof(char*) * 1024);

	char		buf[SIZE_1K], bufobs[SIZE_1K], btime[SIZE_1K], time[SIZE_1K], wlevel[SIZE_1K], obstime[SIZE_1K], obswlevel[SIZE_1K];

	memset(buf, 0x00, sizeof(buf));
	memset(bufobs, 0x00, sizeof(bufobs));
	memset(btime, 0x00, sizeof(btime));
	memset(time, 0x00, sizeof(time));
	memset(wlevel, 0x00, sizeof(wlevel));
	memset(obstime, 0x00, sizeof(obstime));
	memset(obswlevel, 0x00, sizeof(obswlevel));

	char cdir[255];
	memset(cdir, 0x00, sizeof(cdir));
	GetCurrentDirectory(255, cdir);

	char outdirpath2[1024];
	memset(outdirpath2, 0x00, sizeof(outdirpath2));

	sprintf(fpath, "%s\\%s", cdir, OUT_WATERLEVEL_DIR);
	_mkdir(fpath);

	sprintf(outdirpath2, "%s\\%s\\error_rate", cdir, OUT_WATERLEVEL_DIR);

	//_rmdir(outdirpath2);

	recreateDir(outdirpath2);

	std::vector<std::string> files;

	FILE* fpobs;
	//観測水位の場合
	if (dispNo == 1) {
		sprintf(obsPath, "%s\\ObsWLOut.csv", m_csPath3);
		fpobs = fopen(obsPath, "r");
	}
	// HRの場合
	else if (dispNo == 2) {
		sprintf(obsPath, "%s\\CalcWLOut.csv", m_csPath3);
		fpobs = fopen(obsPath, "r");
	}
	// QRの場合
	else if (dispNo == 3) {
		sprintf(obsPath, "%s\\CalcQOut.csv", m_csPath3);
		fpobs = fopen(obsPath, "r");
	}

	CString prjPath = DSET.Proj_Folder;
	CString locName = DSET.LocationName;
	std::string findPath = prjPath + "\\Location\\" + locName + "\\PF_Prediction";
	std::string ext = "csv";

//#ifndef SIMP_VER
	files = get_file_path_in_dir(findPath, ext);
//#endif

	int startFlg = 0;

	// ファイルを開く
	sprintf(fpath, "%s\\base_data.csv", outdirpath2);
	FILE* foutBase = fopen(fpath, "w");
	hyfiles[hyfiles_cnt] = (char*)malloc(1024);
	sprintf(hyfiles[hyfiles_cnt], "%s", fpath);
	hyfiles_cnt++;

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

		if (atoll(startDt3) > atoll(btime) || atoll(btime) > atoll(endDt3)) {
			continue;
		}

		// ファイルを開く
		sprintf(fpath, "%s\\%s", findPath.c_str(), files[i].c_str());
		FILE* fp = fopen(fpath, "r");

		while (fgets(buf, SIZE_1K, fp) != NULL) {
			sscanf(buf, "%[^,],%s", time, wlevel);
			if (atoll(time) >= atoll(btime)) {
				if (atoll(time) == atoll(btime)) {
					while (fgets(bufobs, SIZE_1K, fpobs) != NULL) {
						sscanf(bufobs, "%[^,],%[^,]", obstime, obswlevel);
						if (atoll(time) == atoll(obstime)) {
							fprintf(foutBase, "%s,%s,%s", time, obswlevel, wlevel);
							break;
						}
					}
				}
				else if (time[10] == '0' && time[11] == '0') {
					fprintf(foutBase, ",%s", wlevel);
				}
			}
		}
		fprintf(foutBase, "\n");

		fclose(fp);

	}

	fclose(foutBase);

}

void CErrorRate::createErrorData() {

	char fpath[1024];
	char buf[SIZE_1K], time[SIZE_1K], obswlevel[SIZE_1K], wlevel[SIZE_1K],
		yosoku1[SIZE_1K], yosoku2[SIZE_1K], yosoku3[SIZE_1K], 
		yosoku4[SIZE_1K], yosoku5[SIZE_1K], yosoku6[SIZE_1K];

	char cdir[255];
	GetCurrentDirectory(255, cdir);

	char outdirpath2[1024];

	sprintf(fpath, "%s\\%s", cdir, OUT_WATERLEVEL_DIR);
	_mkdir(fpath);

	sprintf(outdirpath2, "%s\\%s\\error_rate", cdir, OUT_WATERLEVEL_DIR);
	// ファイルを開く
	sprintf(fpath, "%s\\base_data.csv", outdirpath2);
	FILE* foutBase = fopen(fpath, "r");

	// ファイルを開く
	sprintf(fpath, "%s\\rmse.csv", outdirpath2);
	FILE* foutrmse = fopen(fpath, "w");

	double d_obswlevel_org, d_obswlevel, d_wlevel, d_yosoku1,
		d_yosoku2, d_yosoku3, d_yosoku4, d_yosoku5, d_yosoku6;

	double sum_obswlevel = 0, sum_wlevel = 0, sum_yosoku1 = 0,
		sum_yosoku2 = 0, sum_yosoku3 = 0, sum_yosoku4 = 0,
		sum_yosoku5 = 0, sum_yosoku6 = 0;

	double sum_rmse_obswlevel = 0, sum_rmse_wlevel = 0, sum_rmse_yosoku1 = 0,
		sum_rmse_yosoku2 = 0, sum_rmse_yosoku3 = 0, sum_rmse_yosoku4 = 0,
		sum_rmse_yosoku5 = 0, sum_rmse_yosoku6 = 0;

	double d_nash_obswlevel = 0, d_nash_wlevel = 0, d_nash_yosoku1 = 0,
		d_nash_yosoku2 = 0, d_nash_yosoku3 = 0, d_nash_yosoku4 = 0,
		d_nash_yosoku5 = 0, d_nash_yosoku6 = 0;

	double rmse_obswlevel = 0, rmse_wlevel = 0, rmse_yosoku1 = 0,
		rmse_yosoku2 = 0, rmse_yosoku3 = 0, rmse_yosoku4 = 0,
		rmse_yosoku5 = 0, rmse_yosoku6 = 0;

	double sum2_obswlevel = 0, sum2_wlevel = 0, sum2_yosoku1 = 0,
		sum2_yosoku2 = 0, sum2_yosoku3 = 0, sum2_yosoku4 = 0,
		sum2_yosoku5 = 0, sum2_yosoku6 = 0;

	double max_obswlevel = 0, max_wlevel = 0, max_yosoku1 = 0,
		max_yosoku2 = 0, max_yosoku3 = 0, max_yosoku4 = 0,
		max_yosoku5 = 0, max_yosoku6 = 0;

	int loopCnt = 0;

	// RMSE
	while (fgets(buf, SIZE_1K, foutBase) != NULL) {
		sscanf(buf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", 
			time, obswlevel, wlevel,
			yosoku1, yosoku2, yosoku3, yosoku4, yosoku5, yosoku6);

		d_obswlevel_org = atof(obswlevel);
		d_obswlevel = atof(obswlevel);
		d_wlevel = atof(wlevel);
		d_yosoku1 = atof(yosoku1);
		d_yosoku2 = atof(yosoku2);
		d_yosoku3 = atof(yosoku3);
		d_yosoku4 = atof(yosoku4);
		d_yosoku5 = atof(yosoku5);
		d_yosoku6 = atof(yosoku6);

		if (max_obswlevel < d_obswlevel) max_obswlevel = d_obswlevel;
		if (max_wlevel < d_wlevel) max_wlevel = d_wlevel;
		if (max_yosoku1 < d_yosoku1) max_yosoku1 = d_yosoku1;
		if (max_yosoku2 < d_yosoku2) max_yosoku2 = d_yosoku2;
		if (max_yosoku3 < d_yosoku3) max_yosoku3 = d_yosoku3;
		if (max_yosoku4 < d_yosoku4) max_yosoku4 = d_yosoku4;
		if (max_yosoku5 < d_yosoku5) max_yosoku5 = d_yosoku5;
		if (max_yosoku6 < d_yosoku6) max_yosoku6 = d_yosoku6;

		sum_obswlevel = sum_obswlevel + d_obswlevel;
		sum_yosoku1 = sum_yosoku1 + d_yosoku1;
		sum_yosoku2 = sum_yosoku2 + d_yosoku2;
		sum_yosoku3 = sum_yosoku3 + d_yosoku3;
		sum_yosoku4 = sum_yosoku4 + d_yosoku4;
		sum_yosoku5 = sum_yosoku5 + d_yosoku5;
		sum_yosoku6 = sum_yosoku6 + d_yosoku6;

		d_yosoku6 = pow(d_yosoku4 - d_yosoku6, 2.0);
		d_yosoku5 = pow(d_yosoku3 - d_yosoku5, 2.0);
		d_yosoku4 = pow(d_yosoku2 - d_yosoku4, 2.0);
		d_yosoku3 = pow(d_yosoku1 - d_yosoku3, 2.0);
		d_yosoku2 = pow(d_wlevel - d_yosoku2, 2.0);
		d_yosoku1 = pow(d_obswlevel - d_yosoku1, 2.0);
		d_obswlevel = pow((d_obswlevel - d_wlevel), 2.0);

		fprintf(foutrmse, "%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
			time, d_obswlevel_org, d_obswlevel, d_yosoku1, d_yosoku2, d_yosoku3,
			d_yosoku4, d_yosoku5, d_yosoku6);

		sum_rmse_obswlevel = sum_rmse_obswlevel + d_obswlevel;
		sum_rmse_yosoku1 = sum_rmse_yosoku1 + d_yosoku1;
		sum_rmse_yosoku2 = sum_rmse_yosoku2 + d_yosoku2;
		sum_rmse_yosoku3 = sum_rmse_yosoku3 + d_yosoku3;
		sum_rmse_yosoku4 = sum_rmse_yosoku4 + d_yosoku4;
		sum_rmse_yosoku5 = sum_rmse_yosoku5 + d_yosoku5;
		sum_rmse_yosoku6 = sum_rmse_yosoku6 + d_yosoku6;

		loopCnt++;
	}
	
	fclose(foutBase);
	fclose(foutrmse);

	// ファイルを開く
	sprintf(fpath, "%s\\result.csv", outdirpath2);
	FILE* foutresult = fopen(fpath, "w");

	rmse_obswlevel = sqrt(sum_rmse_obswlevel / loopCnt);
	rmse_yosoku1 = sqrt(sum_rmse_yosoku1 / loopCnt);
	rmse_yosoku2 = sqrt(sum_rmse_yosoku2 / loopCnt);
	rmse_yosoku3 = sqrt(sum_rmse_yosoku3 / loopCnt);
	rmse_yosoku4 = sqrt(sum_rmse_yosoku4 / loopCnt);
	rmse_yosoku5 = sqrt(sum_rmse_yosoku5 / loopCnt);
	rmse_yosoku6 = sqrt(sum_rmse_yosoku6 / loopCnt);

	fprintf(foutresult, "RMSE %lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
		rmse_obswlevel, rmse_yosoku1, rmse_yosoku2,
		rmse_yosoku3, rmse_yosoku4, rmse_yosoku5, rmse_yosoku6);

	max_wlevel = max_wlevel / max_obswlevel;
	max_yosoku1 = max_yosoku1 / max_obswlevel;
	max_yosoku2 = max_yosoku2 / max_obswlevel;
	max_yosoku3 = max_yosoku3 / max_obswlevel;
	max_yosoku4 = max_yosoku4 / max_obswlevel;
	max_yosoku5 = max_yosoku5 / max_obswlevel;
	max_yosoku6 = max_yosoku6 / max_obswlevel;

	fprintf(foutresult, "PEAK %lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
		max_wlevel, max_yosoku1, max_yosoku2,
		max_yosoku3, max_yosoku4, max_yosoku5, max_yosoku6);

	// ファイルを開く
	sprintf(fpath, "%s\\nash.csv", outdirpath2);
	FILE* foutnash = fopen(fpath, "w");

	// ファイルを開く
	sprintf(fpath, "%s\\base_data.csv", outdirpath2);
	foutBase = fopen(fpath, "r");

	while (fgets(buf, SIZE_1K, foutBase) != NULL) {
		sscanf(buf, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",
			time, obswlevel, wlevel,
			yosoku1, yosoku2, yosoku3, yosoku4, yosoku5, yosoku6);

		d_obswlevel = atof(obswlevel);
		d_wlevel = atof(wlevel);
		d_yosoku1 = atof(yosoku1);
		d_yosoku2 = atof(yosoku2);
		d_yosoku3 = atof(yosoku3);
		d_yosoku4 = atof(yosoku4);
		d_yosoku5 = atof(yosoku5);
		d_yosoku6 = atof(yosoku6);

		d_nash_obswlevel = pow((d_obswlevel - (sum_obswlevel / loopCnt)), 2.0);
		d_nash_wlevel = pow((d_obswlevel - d_wlevel), 2.0);
		d_nash_yosoku1 = pow((d_obswlevel - d_yosoku1), 2.0);
		d_nash_yosoku2 = pow((d_obswlevel - d_yosoku2), 2.0);
		d_nash_yosoku3 = pow((d_obswlevel - d_yosoku3), 2.0);
		d_nash_yosoku4 = pow((d_obswlevel - d_yosoku4), 2.0);
		d_nash_yosoku5 = pow((d_obswlevel - d_yosoku5), 2.0);
		d_nash_yosoku6 = pow((d_obswlevel - d_yosoku6), 2.0);

		sum2_obswlevel = sum2_obswlevel + d_nash_obswlevel;
		sum2_wlevel = sum2_wlevel + d_nash_wlevel;
		sum2_yosoku1 = sum2_yosoku1 + d_nash_yosoku1;
		sum2_yosoku2 = sum2_yosoku2 + d_nash_yosoku2;
		sum2_yosoku3 = sum2_yosoku3 + d_nash_yosoku3;
		sum2_yosoku4 = sum2_yosoku4 + d_nash_yosoku4;
		sum2_yosoku5 = sum2_yosoku5 + d_nash_yosoku5;
		sum2_yosoku6 = sum2_yosoku6 + d_nash_yosoku6;

		fprintf(foutnash, "%s, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf\n", time,
			d_nash_obswlevel, d_nash_wlevel, d_nash_yosoku1, d_nash_yosoku2,
			d_nash_yosoku3, d_nash_yosoku4, d_nash_yosoku5, d_nash_yosoku6);

	}

	fclose(foutnash);
	fclose(foutBase);

	d_wlevel = 1 - (sum2_wlevel / sum2_obswlevel);
	d_yosoku1 = 1 - (sum2_yosoku1 / sum2_obswlevel);
	d_yosoku2 = 1 - (sum2_yosoku2 / sum2_obswlevel);
	d_yosoku3 = 1 - (sum2_yosoku3 / sum2_obswlevel);
	d_yosoku4 = 1 - (sum2_yosoku4 / sum2_obswlevel);
	d_yosoku5 = 1 - (sum2_yosoku5 / sum2_obswlevel);
	d_yosoku6 = 1 - (sum2_yosoku6 / sum2_obswlevel);

	fprintf(foutresult, "NASH %lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
		d_wlevel, d_yosoku1, d_yosoku2,
		d_yosoku3, d_yosoku4, d_yosoku5, d_yosoku6);

	fclose(foutresult);
}

std::string errorRatePath() {
	char buff[SIZE_1K];
	GetCurrentDirectory(SIZE_1K, buff);
	std::string outPath(buff);

	outPath = outPath + "\\" + OUT_WATERLEVEL_DIR;
	auto mdRs = _mkdir(outPath.c_str());
	printf("rs: %d", mdRs);

	outPath += "\\error_rate";
	return outPath;
}

void CErrorRate::createDataSimpVer() {
	auto outPath = errorRatePath();
	recreateDir(outPath.c_str());

	//set output file
	outPath += "\\base_data.csv";

	//read cal data
	std::map<std::string, std::string> calDataMap;
	FILE* fpCal = fopen(getObFilePath("../CalcWL/CalcWLOut.csv").c_str(), "r");
	char buf[SIZE_1K], time[SIZE_1K], wlevel[SIZE_1K];
	memset(buf, 0x00, sizeof(buf));
	memset(time, 0x00, sizeof(time));
	memset(wlevel, 0x00, sizeof(wlevel));
	while (fgets(buf, SIZE_1K, fpCal) != NULL) {
		auto rfRs1 = sscanf(buf, "%[^,],%s", time, wlevel);
		calDataMap.insert(std::make_pair(time, wlevel));
	}
	fclose(fpCal);

	
	// ファイルを開く
	FILE* foutBase = fopen(outPath.c_str(), "w");
	FILE* fpObs = fopen(getObFilePath("../observation/ObsWLOut.csv").c_str(), "r");

	auto nStartDt = atoll(startDt3);
	auto nEndDt = atoll(endDt3);
	char bufobs[SIZE_1K], obstime[SIZE_01K], obswlevel[SIZE_01K];
	memset(bufobs, 0x00, sizeof(bufobs));
	memset(obstime, 0x00, sizeof(obstime));
	memset(obswlevel, 0x00, sizeof(obswlevel));
	while (fgets(bufobs, SIZE_1K, fpObs) != NULL) {
		auto rfRs2 = sscanf(bufobs, "%[^,],%[^,]", obstime, obswlevel);
		obstime[12] = '\0';
		auto nCurDt = atoll(obstime);
		if (nStartDt > nCurDt || nCurDt > nEndDt) continue;
		for (auto calData : calDataMap) {
			auto time = calData.first.c_str();
			auto wlevel = calData.second.c_str();
			if (nCurDt == atoll(time)) {
				fprintf(foutBase, "%s,%s,%s\n", time, obswlevel, wlevel);
				break;
			}
		}
	}
	fclose(fpObs);
	fclose(foutBase);
}

void CErrorRate::createErrorDataSimpVer() {
	auto outPath = errorRatePath();

	// ファイルを開く
	std::string fpath = outPath + "\\base_data.csv";
	FILE* foutBase = fopen(fpath.c_str(), "r");

	fpath = outPath + "\\rmse.csv";
	FILE* foutrmse = fopen(fpath.c_str(), "w");

	char buf[SIZE_006K], time[SIZE_0015K], obswlevel[SIZE_003K], wlevel[SIZE_003K];

	constexpr double dDefault = 1e-8;
	double d_obswlevel_org = dDefault, d_obswlevel = dDefault, d_wlevel = dDefault;
	double sum_obswlevel = dDefault;
	double sum_rmse_obswlevel = dDefault, rmse_obswlevel = dDefault;
	double max_obswlevel = dDefault, max_wlevel = dDefault;
	
	int loopCnt = 0;
	std::map<std::string, std::vector<double>> mapTimeToObsWLvl;

	// RMSE
	while (fgets(buf, SIZE_006K, foutBase) != NULL) {
		auto rs = sscanf(buf, "%[^,],%[^,],%s",time, obswlevel, wlevel);
		if (time[0] == '\0' || obswlevel[0] == '\0' || wlevel[0] == '\0') continue;
		time[SIZE_0015K - 1] = 0;
		obswlevel[SIZE_003K - 1] = 0;
		wlevel[SIZE_003K - 1] = 0;

		d_obswlevel_org = d_obswlevel = atof(obswlevel);
		d_wlevel = atof(wlevel);

		max_obswlevel = max(max_obswlevel, d_obswlevel);
		max_wlevel = max(max_wlevel, d_wlevel);
		sum_obswlevel += d_obswlevel;
		mapTimeToObsWLvl.insert(std::make_pair(std::string(time), std::vector<double>{d_obswlevel, d_wlevel}));

		d_obswlevel = pow((d_obswlevel - d_wlevel), 2.0);
		sum_rmse_obswlevel += d_obswlevel;
		fprintf(foutrmse, "%s,%lf,%lf\n", time, d_obswlevel_org, d_obswlevel);
		loopCnt++;
	}
	fclose(foutBase);
	fclose(foutrmse);

	//PEAK
	max_wlevel /= max_obswlevel;

	//NASH
	fpath = outPath + "\\nash.csv";
	FILE* foutnash = fopen(fpath.c_str(), "w");
	double d_nash_obswlevel = dDefault, d_nash_wlevel = dDefault;
	double sum2_obswlevel = dDefault, sum2_wlevel = dDefault;
	for (auto t_lvl : mapTimeToObsWLvl) {
		auto obsLvl = t_lvl.second[0];
		auto calLvl = t_lvl.second[1];

		d_nash_obswlevel = pow((obsLvl - (sum_obswlevel / loopCnt)), 2.0);
		sum2_obswlevel += d_nash_obswlevel;

		d_nash_wlevel = pow((obsLvl - calLvl), 2.0);
		sum2_wlevel += d_nash_wlevel;

		fprintf(foutnash, "%s, %lf, %lf\n", t_lvl.first.c_str(), d_nash_obswlevel, d_nash_wlevel);
	}
	fclose(foutnash);

	d_wlevel = 1 - (sum2_wlevel / sum2_obswlevel);


	// ファイルを開く
	fpath = outPath + "\\result.csv";
	FILE* foutresult = fopen(fpath.c_str(), "w");

	rmse_obswlevel = sqrt(sum_rmse_obswlevel / loopCnt);
	fprintf(foutresult, "RMSE %lf\n", rmse_obswlevel);
	fprintf(foutresult, "PEAK %lf\n", max_wlevel);
	fprintf(foutresult, "NASH %lf\n", d_wlevel);

	fclose(foutresult);
}


void CErrorRate::OnCancel()
{
	CDialog::OnOK();
}


void CErrorRate::OnLvnItemchangedErrorrateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
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
BOOL SelectFolder2(HWND hWnd, LPCTSTR lpDefFolder, LPTSTR lpSelectPath, UINT iFlg, CString ttl)
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
		brows.lpfn = &BrowseCallbackProc2;
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

int CALLBACK BrowseCallbackProc2(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
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

void CErrorRate::AdjustLayout()
{
	if (!m_ErrorRateList.GetSafeHwnd()) {
		return;
	}

	CRect rc;
	GetClientRect(&rc);

	m_ErrorRateList.SetWindowPos(0, 0, 0, rc.Width(), rc.Height(), SWP_NOZORDER);
}

void CErrorRate::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	AdjustLayout();
}

