// MakeHQeqDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RRI.h"
#include "MakeHQeqDialog.h"
#include "afxdialogex.h"
#include "AccuracyEvaluation.h"
#include <shlwapi.h>


#define PY_HQ_EXE_PATH		"./Tools/py_HQ.exe"
#define PY_UNIFLOW_EXE_PATH "./Tools/py_uniflow.exe"


// CMakeHQeqDialog ダイアログ

IMPLEMENT_DYNAMIC(CMakeHQeqDialog, CDialogEx)

CMakeHQeqDialog::CMakeHQeqDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAKE_HQEQ, pParent)
{

}

CMakeHQeqDialog::~CMakeHQeqDialog()
{
}

void CMakeHQeqDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CMakeHQeqDialog::OnInitDialog() {
	CDialogEx::OnInitDialog();
	SetTitle();
	return true;
}

BEGIN_MESSAGE_MAP(CMakeHQeqDialog, CDialogEx)
	ON_BN_CLICKED(IDC_CROSS_SECTION, &CMakeHQeqDialog::OnBnClickedCrossSection)
	ON_BN_CLICKED(IDOK, &CMakeHQeqDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SAVE_HQEQ, &CMakeHQeqDialog::OnBnClickedSaveHqeq)
END_MESSAGE_MAP()


// CMakeHQeqDialog メッセージ ハンドラー


void CMakeHQeqDialog::OnBnClickedCrossSection()
{
	char cdir[255] = {};
	GetCurrentDirectory(255, cdir);

	UpdateData(TRUE);

	if (!Button_File(TRUE, &crossSectionPath, cdir, "", "txt")) return;
	if (!File_Exist(crossSectionPath)) {
		MessageBox("** Error ** Cross Section file is not found.", "Fatal Error", MB_OK | MB_ICONASTERISK);
		return;
	}

	char waBuf[MAX_PATH * 2];
	GetCurrentDirectory(255, cdir);

	PathRelativePathTo(waBuf, cdir, FILE_ATTRIBUTE_DIRECTORY, crossSectionPath, FILE_ATTRIBUTE_ARCHIVE);

	SetDlgItemText(IDC_CROSS_SECTION_EDT, waBuf);

}

void CMakeHQeqDialog::OnBnClickedSaveHqeq()
{
	char cdir[255] = {};
	char selDir[MAX_PATH * 2];

	GetCurrentDirectory(255, cdir);

	UpdateData(TRUE);

	if (!SelectFolder(this->m_hWnd, cdir, selDir, BIF_RETURNONLYFSDIRS, "Select HQeq.ini Save Folder.")) return;

	char waBuf[MAX_PATH * 2];
	GetCurrentDirectory(255, cdir);
	CString relPath = "";

	PathRelativePathTo(waBuf, cdir, FILE_ATTRIBUTE_DIRECTORY, selDir, FILE_ATTRIBUTE_DIRECTORY);
	saveHqeqPath = selDir;
	saveHqeqPath = saveHqeqPath + "\\HQeq.ini";
	relPath = waBuf;
	relPath = relPath + "\\HQeq.ini";

	SetDlgItemText(IDC_SAVE_HQEQ_EDT, relPath);
}



void CMakeHQeqDialog::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (uniformflowFlg) {
		ExecUniform();
	}
	else {
		ExecHq();
	}
	//CDialogEx::OnOK();
}

void CMakeHQeqDialog::SetTitle()
{
	if (uniformflowFlg) {
		SetDlgItemText(IDC_MAKE_HQEQ_LABEL, "Make HQeq（uniform flow) ");
		SetDlgItemText(IDC_CROSS_SECTION, "Cross section…");
	}
	else {
		SetDlgItemText(IDC_MAKE_HQEQ_LABEL, "Make HQeq（H,Q DATA) ");
		SetDlgItemText(IDC_CROSS_SECTION, "H,Qdata");
	}

}

int CMakeHQeqDialog::ExecUniform()
{
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

	char pm[MAX_PATH] = {};
	CString cmd;
	cmd.Format("%s %s out_hq.txt", PY_UNIFLOW_EXE_PATH, crossSectionPath);
	strcpy(pm, cmd);
	if (!CreateProcess(NULL, pm, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		CloseHandle(hread);
		return 1;
	}
	CloseHandle(hread);

	// コマンドを送信
	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	// 標準入力のリダイレクト準備
	CreatePipe(&hreadtmp, &stdinput.m_hFile, &sa, 0);
	DuplicateHandle(GetCurrentProcess(), hreadtmp, GetCurrentProcess(), &hread, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hreadtmp);

	cmd.Format("%s out_hq.txt %s", PY_HQ_EXE_PATH, saveHqeqPath);
	strcpy(pm, cmd);

	if (!CreateProcess(NULL, pm, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		CloseHandle(hread);
		return 1;
	}
	CloseHandle(hread);

	// コマンドを送信
	WaitForSingleObject(pi.hProcess, INFINITE);

	int btnid = MessageBox("Process execution is complete.", "Process Message.", MB_OK);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return 0;

}

int CMakeHQeqDialog::ExecHq()
{
	// 変数宣言
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
	HANDLE hreadtmp, hread;
	CFile stdinput;
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi;
	CStringA command;

	HRESULT hResult = E_FAIL;

	// 標準入力のリダイレクト準備
	CreatePipe(&hreadtmp, &stdinput.m_hFile, &sa, 0);
	DuplicateHandle(GetCurrentProcess(), hreadtmp, GetCurrentProcess(), &hread, 0, TRUE, DUPLICATE_SAME_ACCESS);
	CloseHandle(hreadtmp);

	// コンソール画面なしで gnuplot.exe を実行
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hread;

	char pm[MAX_PATH];
	CString cmd;
	cmd.Format("%s %s %s", PY_HQ_EXE_PATH, crossSectionPath, saveHqeqPath);
	strcpy(pm, cmd);
	if (!CreateProcess(NULL, pm, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		CloseHandle(hread);
		return 1;
	}
	CloseHandle(hread);

	// コマンドを送信
	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD dwExitCode;
	BOOL bResult = ::GetExitCodeProcess(pi.hProcess, &dwExitCode);
	hResult = HRESULT_FROM_WIN32(::GetLastError());

	int btnid = MessageBox("Process execution is complete.", "Process Message.", MB_OK);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return 0;

}
