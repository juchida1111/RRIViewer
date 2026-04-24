// EDITDLG.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RRI.h"
#include "RRIView.h"
#include "EDITDLG.h"
#include "afxdialogex.h"

#include "RRI_Common.h"
#include "CommFuncs.h"
//_______________________________________________________________________________________________
/////////////////////////////////////////////////////////////////////////////////////////////////
//
extern      char			m_RecentPath[SIZE_1K];
extern      PARAMS		PM;							// 基本情報
extern      int			File_Format[ FILE_IN_NUM ]; 
extern      CString		MODEL_EDIT_NAME[FILE_IN_NUM];
extern      int			File_Order[ FILE_IN_NUM ];
extern      DATASET					DSET;			// ファイルセット
//

extern      MINFO**					MESH;
//
//
//_______________________________________________________________________________________________
/////////////////////////////////////////////////////////////////////////////////////////////////

// CEDITDLG ダイアログ

IMPLEMENT_DYNAMIC(CEDITDLG, CDialogEx)

CEDITDLG::CEDITDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEDITDLG::IDD, pParent)
{

}

CEDITDLG::~CEDITDLG()
{
}

void CEDITDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEDITDLG, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_EXPORT, &CEDITDLG::OnBnClickedExport)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////////////////////////
// CEDITDLG メッセージ ハンドラー
/////////////////////////////////////////////////////////////////////////////////////////////////
//
void CEDITDLG::PostNcDestroy() {delete this;}


BOOL CEDITDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//
	this->SetWindowTextA("Show hs max value");
	//
	IDS = NULL;
	Num = UNDEF;
	PM.Edit_Result = 0;
	PM.Data_Change_Cnt = 0;
	//
	LDH = LDV = UNDEF;
	//

	SetTimer(0,10,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
}
void CEDITDLG::OnDestroy()
{
	CDialogEx::OnDestroy();
	if( IDS != NULL ) {
		for(int v=0;v<Num;v++) {
			for(int h=0;h<Num;h++) 	delete  Edits[v][h];
			free( Edits[v] );
		}
		free( Edits );
		Free_Matrix(Num,IDS );
		Free_Matrix(Num,NO );
	}
}
BOOL CEDITDLG::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam == VK_RETURN ) {
		Reflesh_Data();
		Set_Values();
//		((CRRIView*)this->GetParent())->Edit_Mesh_Model();
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
///
// //////////////////////////////////////////////////////////////////////////////////////////////
/// 処理
// //////////////////////////////////////////////////////////////////////////////////////////////
///
void CEDITDLG::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( 0 );
	//
	Change_Frame_Size();
}
///-----------------------------------------------------------------------------------------------------------
//  ウィンドウの形状作成
///-----------------------------------------------------------------------------------------------------------
void CEDITDLG::Change_Frame_Size()
{
	CWnd*	cw;
	//
	if( IDS != NULL ) {
		for(int v=0;v<Num;v++) {
			for(int h=0;h<Num;h++) 	delete  Edits[v][h];
			free( Edits[v] );
		}
		free( Edits );
		Free_Matrix(Num,IDS );
		Free_Matrix(Num,NO );
	}
	//
	Num = PM.SizeID*2 + 1;
	IDS = Make_Matrix(Num,Num,0);
	NO  = Make_Matrix(Num,Num,0);
	//
	CRect	Wrect;
	this->GetWindowRect( &Wrect );
	Wrect.right = Wrect.left + Num*(X_SIZE+1) + X_MARGIN;
	Wrect.bottom = Wrect.top + Num*(Y_SIZE+1) + Y_MARGIN + 50;
	this->MoveWindow(&Wrect,TRUE);
	this->RedrawWindow();
//
//	エディット枠
	if ((Edits = (CEdit***)malloc(Num*sizeof(void *))) == NULL) OnCancel();
	for (int i = 0; i < Num; i++) {
		if ((Edits[i] = (CEdit**)malloc(Num*sizeof(CEdit *))) == NULL) OnCancel();
	}
	//
	int		Cid = ID_INIT,xst,yst;
	//
	this->GetClientRect(&Wrect);
	yst = Wrect.bottom - ( Y_SIZE + Y_MARGIN/2 +30 );

	for(int v=0;v<Num;v++) {
		xst = X_MARGIN/2;
		for(int h=0;h<Num;h++) {
			IDS[v][h] = Cid++;
			Edits[v][h] = new CEdit;
			if (!Edits[v][h]->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_CENTER,
								CRect(xst, yst, xst+X_SIZE,yst+Y_SIZE), this, IDS[v][h]) ) return;
			//
			Edits[v][h]->RedrawWindow();
			xst += (X_SIZE+1);
			//

		}
		yst -= (Y_SIZE +1 );
	}
//**********************************************
//
//	OK CANCEL ボタンの移動
	CRect	BTrect,Mrect;
	//
	//cw = GetDlgItem(IDOK);
	//cw->GetClientRect( &BTrect );
	////
	//Mrect.left = Wrect.right - (int)(BTrect.Width()*2.9);  Mrect.right = Mrect.left + BTrect.Width();
	//Mrect.top  = Wrect.bottom - 30;  Mrect.bottom = Mrect.top + BTrect.Height();
	//cw->MoveWindow(&Mrect,TRUE);
	//cw->RedrawWindow();
	////
	//cw = GetDlgItem( IDC_SAVEAS );
	//cw->GetClientRect( &BTrect );
	////
	//Mrect.left = Wrect.right - (int)(BTrect.Width()*1.9);  Mrect.right = Mrect.left + BTrect.Width();
	//Mrect.top  = Wrect.bottom - 30;  Mrect.bottom = Mrect.top + BTrect.Height();
	//cw->MoveWindow(&Mrect,TRUE);
	//cw->RedrawWindow();
	//
	cw = GetDlgItem( IDCANCEL );
	cw->GetClientRect( &BTrect );
	//
	Mrect.left = Wrect.right - (int)(BTrect.Width()*1.1);  Mrect.right = Mrect.left + BTrect.Width();
	Mrect.top  = Wrect.bottom - 30;  Mrect.bottom = Mrect.top + BTrect.Height();
	cw->MoveWindow(&Mrect,TRUE);
	cw->RedrawWindow();
	//
	//cw = GetDlgItem(ID_IMPORT);
	//cw->GetClientRect(&BTrect);
	////
	//Mrect.left = Wrect.left + (int)(BTrect.Width()*0.05);  Mrect.right = Mrect.left + BTrect.Width();
	//Mrect.top = Wrect.bottom - 30;  Mrect.bottom = Mrect.top + BTrect.Height();
	//cw->MoveWindow(&Mrect, TRUE);
	//cw->RedrawWindow();
	//
	cw = GetDlgItem(ID_EXPORT);
	cw->GetClientRect(&BTrect);
	//
	Mrect.left = Wrect.left + (int)(BTrect.Width()*1.1);  Mrect.right = Mrect.left + BTrect.Width();
	Mrect.top = Wrect.bottom - 30;  Mrect.bottom = Mrect.top + BTrect.Height();
	cw->MoveWindow(&Mrect, TRUE);
	cw->RedrawWindow();
	//
	Set_Values();
}
///-----------------------------------------------------------------------------------------------------------
//  値の設定
///-----------------------------------------------------------------------------------------------------------
void CEDITDLG::Set_Values()
{
	if( PM.Cam_Dir == CAM_DIR_UP ) {
		LDH = PM.CX - PM.SizeID;
		LDV = PM.CY - PM.SizeID;
	}
	else if( PM.Cam_Dir == CAM_DIR_RIGHT ) {
		LDH = PM.CY - PM.SizeID;
		LDV = PM.CX + PM.SizeID;
	}
	else if( PM.Cam_Dir == CAM_DIR_LEFT ) {
		LDH = PM.CY + PM.SizeID;
		LDV = PM.CX - PM.SizeID;
	}
	//if ((LDH < 0) LDH = 0;
	//if (LDH >PM.SizeID+1) LDH = PM.
	//
	// データ位置の記憶 と 表示
	CString		str;
	//
	for (int v = 0; v<Num; v++) {
		for(int h=0;h<Num;h++) {
			if( PM.Cam_Dir == CAM_DIR_UP ) 			NO[v][h] = (LDV+v)*PM.MHnum + LDH+h;
			else if( PM.Cam_Dir == CAM_DIR_RIGHT ) 		NO[v][h] = (LDH+h)*PM.MHnum + LDV-v;
			else if( PM.Cam_Dir == CAM_DIR_LEFT ) 		NO[v][h] = (LDH-h)*PM.MHnum + LDV+v;
			//
			CH = LDH + h;
			CV = LDV + v;
			//
			Edits[v][h]->EnableWindow( TRUE );
			if (MESH[LDV + v][LDH + h].flag == 1) {
				str = _T("");
				if (PM.Values[LDV + v][LDH + h] != NODATA) 
					Make_String(PM.Values[LDV + v][LDH + h], &str,2);
				Edits[v][h]->SetWindowTextA(str);
			}
			else if (MESH[LDV + v][LDH + h].flag == 2) {
				str = _T("");
				if (PM.Values[LDV + v][LDH + h] != NODATA)
					Make_String(PM.Values[LDV + v][LDH + h], &str, 2);
				Edits[v][h]->SetWindowTextA(str);
			}
			else if (MESH[LDV + v][LDH + h].flag > 2) {
				str = _T("");
				if (PM.Values[LDV + v][LDH + h] != NODATA)
					Make_String(PM.Values[LDV + v][LDH + h], &str, 2);
				Edits[v][h]->SetWindowTextA(str);
			}
			else {
				Edits[v][h]->SetWindowTextA("");
				Edits[v][h]->EnableWindow(FALSE);//
			}
			//
		}
	}
	UDF;
}
///
// ///////////////////////////////////////////////////////////////////////////////////////////////
/// 終了処理
// ///////////////////////////////////////////////////////////////////////////////////////////////
///
// モードレス特有の処理
void CEDITDLG::OnCancel() {
	PM.Save_Proc = TRUE;
	PM.Edit_Result = 0;
	((CRRIView*)this->GetParent())->Term_Manager();
	DestroyWindow();
}

void CEDITDLG::Reflesh_Data()
{
	char	buf[SIZE_01K];
	PM.Move_Cnt = 0;
	for(int v=0;v<Num;v++ ) {
		for(int h=0;h<Num;h++ ) {
			if( Edits[v][h]->GetModify() ) {
				PM.Move_Cnt++;
				Edits[v][h]->GetWindowTextA( buf,SIZE_01K);
				PM.Values[LDV+v][LDH+h] = atof( buf );
			}
		}
	}
	//
	PM.Data_Change_Cnt += PM.Move_Cnt;
}
///
// ///////////////////////////////////////////////////////////////////////////////////////////////
///  エクスポート
// ///////////////////////////////////////////////////////////////////////////////////////////////
///

void CEDITDLG::OnBnClickedExport()
{
	CString		com, fname;
	fname.Format("%s\\hsmax.txt", DSET.Proj_Folder);
	com.Format("Is save name [ %s ] ?", fname);
	if (MessageBox(com, "file name", MB_YESNO | MB_ICONQUESTION) == IDNO) {
		if (!Button_File(FALSE, &fname, m_RecentPath, "Save hsmax", "txt")) return;
	}
	//
	FILE*	fp = fopen(fname, "w");
	if (fp == NULL) return;
	//
	fprintf(fp, "ncols	 %d \n", PM.MHnum);
	fprintf(fp, "nrows	 %d \n", PM.MVnum);
	fprintf(fp, "xllcorner	 %.1lf\n", PM.stLNG);
	fprintf(fp, "yllcorner	 %.1lf\n", PM.stLAT);
	fprintf(fp, "cellsize	 %.14lf\n", PM.Size);
	fprintf(fp, "NODATA_value  -9999 \n");
	//
	for (int v = PM.MVnum - 1; v >= 0; v--) {		// 
		for (int h = 0; h < PM.MHnum; h++) {

			if (h == 0) {
				if (MESH[v][h].hsmax != DUNDEF) fprintf(fp, "%.2lf", MESH[v][h].hsmax);
				else                          fprintf(fp, "-9999");
			}
			else     {
				if (MESH[v][h].hsmax != DUNDEF) fprintf(fp, "  %.2lf", MESH[v][h].hsmax);
				else                          fprintf(fp, "  -9999");
			}
		}
		CR;
	}
	fclose(fp);
}


HBRUSH CEDITDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (LDV < 0 || LDH < 0) return hbr;
	//
	if (nCtlColor == CTLCOLOR_EDIT) {
		// 文字色
		pDC->SetTextColor(RGB(0,0,0));
		//
		CV = CH = UNDEF;
		for (int v = 0; v < Num; v++) {
			for (int h = 0; h < Num; h++) {
				if (Edits[v][h] == pWnd) {
					CV = LDV + v;
					CH = LDH + h; break;
				}
			}
			if (CV != UNDEF)break;
		}
		if (CV < 0 || CH < 0) return hbr;
		// 背景色
		hbr = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		if (MESH[CV][CH].flag == 0) pDC->SetBkColor(RGB(190,190,190));
		else if (MESH[CV][CH].flag == 1) pDC->SetBkColor(RGB(255, 255, 255));
		else if (MESH[CV][CH].flag == 2) pDC->SetBkColor(RGB(200, 255, 255));
		else  pDC->SetBkColor(RGB(255, 200, 200));
	}
	return hbr;
}
