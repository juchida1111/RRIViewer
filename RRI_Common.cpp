#include	<afxwin.h>
#include	<strsafe.h>
#include	"RRI_Common.h"
//-----------------------------------------------------------------------------------------------------------
//  GLOBAL 
extern		char			m_RecentPath[];
extern      int			File_Format[];
extern      int			File_Order[];
extern      CString		File_Title[];
extern      DATASET		DSET;						// ファイルセット
extern      PARAMS		PM;							// 基本情報

//-----------------------------------------------------------------------------------------------------------
//  ALLOC TIMESET 

//
//-----------------------------------------------------------------------------------------------------------
//  Relative Path
//
void	Get_Relative_Path(char* Fname, char* Dir, char* Rname)
{
	int		flen, len, cnt;
	char		LongName[SIZE_1K];
	char		DirName[SIZE_1K];
	len = (int)strlen(Dir);
	strcpy(DirName, Dir);
	for (int i = 0; i < len; i++) if (DirName[i] == 0x5c) DirName[i] = 0x2f;
	//
	flen = (int)strlen(Fname);
	strcpy(LongName, Fname);
	for (int i = 0; i < flen; i++) if (LongName[i] == 0x5c) LongName[i] = 0x2f;
	//

	if (len == 0) {
		strcpy(Rname, LongName); return;
	}
	if (LongName[len] == 0x2f) {	// 通常処理
		LongName[len - 1] = '.'; LongName[len] = 0x2f;
		cnt = 0;
		for (int i = len - 1; i<flen; i++) Rname[cnt++] = LongName[i];
		Rname[cnt] = 0x00;
	}
	else strcpy(Rname, LongName);
	//
	return;
}
/////////////////////////////////////////////////////////////////////////////
//
void	Get_SuitScale(int scaleid, int num, double* x, double* vmin, double* vmax, int* rips, double* unit, int* fm)
{
	int		i, base1, cnt;
	double	drift, val;
	double	maxval, minval;
	double	vspan;

	// 最大・最小値
	*vmin = *vmax = DUNDEF;
	for (i = 0; i<num; i++) {
		if (i == 0) { maxval = minval = x[i]; }
		else {
			if (maxval < x[i])  maxval = x[i];
			if (minval > x[i])  minval = x[i];
		}
	}
	//if( *vmax != DUNDEF ) maxval = *vmax;
	//else                  *vmax  = maxval;
	//if( *vmin != DUNDEF ) minval = *vmin;
	//else                  *vmin  = minval;
	*vmin = minval;
	*vmax = maxval;
	vspan = maxval - minval;
	// 最大・最小値範囲
	val = log10(vspan);
	if (val >= 0.) 	base1 = (int)log10(vspan);
	else				base1 = (int)(log10(vspan) - 1.);
	drift = pow(10., (log10(vspan) - (double)base1));

	// 最大値範囲
	if (scaleid == AXIS_SMOLL) {
		if (drift <= 1.0001)		{ *rips = 3; *unit = 1.*pow(10., base1) / 2.; if (val >= 0.) base1--; }
		else if (drift <= 1.50001)	{ *rips = 4; *unit = 1.5*pow(10., base1) / 3.; if (val >= 0.) base1--; }
		else if (drift <= 2.0001)	{ *rips = 3; *unit = 2.*pow(10., base1) / 2.; if (val >= 0.) base1--; }
		else if (drift <= 3.0001)	{ *rips = 4; *unit = 3.*pow(10., base1) / 3.; }
		else if (drift <= 4.0001)	{ *rips = 3; *unit = 4.*pow(10., base1) / 2.; }
		else if (drift <= 5.0001)	{ *rips = 3; *unit = 5.*pow(10., base1) / 2.5; }
		else if (drift <= 6.0001)	{ *rips = 4; *unit = 6.*pow(10., base1) / 3.; }
		else if (drift <= 8.0001)	{ *rips = 5; *unit = 8.*pow(10., base1) / 4.; }
		else					{ *rips = 6; *unit = 10.*pow(10., base1) / 5.; }
	}
	else if (scaleid == AXIS_MIDDLE) {
		if (drift <= 1.0001)		{ *rips = 6; *unit = 1.*pow(10., base1) / 5.; if (val >= 0.) base1--; }
		else if (drift <= 1.50001)	{ *rips = 4; *unit = 1.5*pow(10., base1) / 3.; if (val >= 0.) base1--; }
		else if (drift <= 2.0001)	{ *rips = 5; *unit = 2.*pow(10., base1) / 4.; if (val >= 0.) base1--; }
		else if (drift <= 3.0001)	{ *rips = 4; *unit = 3.*pow(10., base1) / 3.; }
		else if (drift <= 4.0001)	{ *rips = 5; *unit = 4.*pow(10., base1) / 4.; }
		else if (drift <= 5.0001)	{ *rips = 6; *unit = 5.*pow(10., base1) / 5.; }
		else if (drift <= 6.0001)	{ *rips = 4; *unit = 6.*pow(10., base1) / 3.; }
		else if (drift <= 8.0001)	{ *rips = 5; *unit = 8.*pow(10., base1) / 4.; }
		else					{ *rips = 6; *unit = 10.*pow(10., base1) / 5.; }
	}
	else if (scaleid == AXIS_LARGE) {
		if (drift <= 1.0001)		{ *rips = 11; *unit = 1.*pow(10., base1) / 10.; if (val >= 0.) base1--; }
		else if (drift <= 1.20001)	{ *rips = 7; *unit = 1.2*pow(10., base1) / 6.; if (val >= 0.) base1--; }
		else if (drift <= 1.40001)	{ *rips = 8; *unit = 1.4*pow(10., base1) / 7.; if (val >= 0.) base1--; }
		else if (drift <= 1.60001)	{ *rips = 9; *unit = 1.6*pow(10., base1) / 8.; if (val >= 0.) base1--; }
		else if (drift <= 1.80001)	{ *rips = 10; *unit = 1.8*pow(10., base1) / 9.; if (val >= 0.) base1--; }
		else if (drift <= 2.0001)	{ *rips = 11; *unit = 2.*pow(10., base1) / 10.; if (val >= 0.) base1--; }
		else if (drift <= 4.0001)	{ *rips = 9; *unit = 4.*pow(10., base1) / 8.; if (val >= 0.) base1--; }
		else if (drift <= 5.0001)	{ *rips = 11; *unit = 5.*pow(10., base1) / 10.; }
		else if (drift <= 6.0001)	{ *rips = 7; *unit = 6.*pow(10., base1) / 6.; }
		else if (drift <= 8.0001)	{ *rips = 9; *unit = 8.*pow(10., base1) / 8.; }
		else					{ *rips = 11; *unit = 10.*pow(10., base1) / 10.; }
	}
	if (*vmin == DUNDEF) {
		cnt = (int)(minval / (*unit));
		if (minval >= 0.) *vmin = (*unit)*cnt;
		else               *vmin = (*unit)*(cnt - 1);
	}
	if (base1 >= 0) *fm = 0;
	else             *fm = -base1;
	return;
}
///////////////////////////////////////////////////////////////////////////////////
// RRI 共通 関数
//
//
///////////////////////////////////////////////////////////////////////////////////
void	Get_Scale_Axis(double* Hmin, double*  Hmax, int* rips, double* unit, int* fm)
{
	double	vspan, val, drift;
	int		base1;
	//
	vspan = *Hmax - *Hmin;
	// 最大・最小値範囲
	val = log10(vspan);
	if (val >= 0.) 	base1 = (int)log10(vspan);
	else				base1 = (int)(log10(vspan) - 1.);
	drift = pow(10., (log10(vspan) - (double)base1));

	// 範囲検索
	if (drift <= 1.0001)			{ *rips = 3; *unit = 1.*pow(10., base1) / 2.; if (val >= 0.) base1--; }
	else if (drift <= 1.50001)	{ *rips = 4; *unit = 1.5*pow(10., base1) / 3.; if (val >= 0.) base1--; }
	else if (drift <= 2.0001)	{ *rips = 3; *unit = 2.*pow(10., base1) / 2.; if (val >= 0.) base1--; }
	else if (drift <= 2.5001)	{ *rips = 6; *unit = 2.5*pow(10., base1) / 5.; if (val >= 0.) base1--; }
	else if (drift <= 3.0001)	{ *rips = 4; *unit = 3.*pow(10., base1) / 3.; }
	else if (drift <= 4.0001)	{ *rips = 3; *unit = 4.*pow(10., base1) / 2.; }
	else if (drift <= 5.0001)	{ *rips = 3; *unit = 5.*pow(10., base1) / 2.; }
	else if (drift <= 6.0001)	{ *rips = 4; *unit = 6.*pow(10., base1) / 3.; }
	else if (drift <= 7.0001)	{ *rips = 3; *unit = 7.*pow(10., base1) / 2.; }
	else if (drift <= 7.5001)	{ *rips = 4; *unit = 7.5*pow(10., base1) / 3.; }
	else if (drift <= 8.0001)	{ *rips = 5; *unit = 8.*pow(10., base1) / 4.; }
	else if (drift <= 9.0001)	{ *rips = 4; *unit = 9.*pow(10., base1) / 3.; }
	else					{ *rips = 6; *unit = 10.*pow(10., base1) / 5.; }
	//
	//  もし ｜最小値 負値 ｜　＜ UNIT
	if (*Hmin < 0.) {
		if (fabs(*Hmin) < *unit) { *Hmin = -(*unit); }
	}
	//
	*fm = 0;
	while (fmod(((*unit)*pow(10., (double)(*fm))), 1.) > EPS4) {
		(*fm)++;
		if ((*fm) > 7) break;
	}
	//
	return;
}

void	Get_GoodScale(int Num, double* val, double* Vmin, double* Vmax, int* Rip, double* Unit, int* Fm)
{
	*Vmin = -DUNDEF; *Vmax = DUNDEF;			// とりあえず距離の寸法
	for (int i = 0; i<Num; i++) {
		if (*Vmin > val[i]) *Vmin = val[i];
		if (*Vmax < val[i]) *Vmax = val[i];
	}
	//
	int		rank, rip, Brip;
	double	Multi, Tunit, Bunit, Trate, Brate;

	rank = (int)log10(*Vmax);
	Multi = pow(10., rank);
	//
	Bunit = DUNDEF;
	for (int i = 1; i <= 5; i++) {				// 1***, 4***,8***, を 1000,2000,3000,... で探る
		Tunit = i*Multi;
		if (Tunit > *Vmax) break;
		if (fmod(*Vmax, Tunit) == 0.) rip = (int)(*Vmax / Tunit);
		else                          rip = (int)(*Vmax / Tunit) + 1;
		if (rip < 2 || rip > 8) continue;
		//
		if (Bunit == DUNDEF) {
			Bunit = Tunit;
			Brip = rip;
			Brate = Trate = (Tunit - fmod((*Vmax), Tunit)) / Tunit;
		}
		else {
			Trate = (Tunit - fmod((*Vmax), Tunit)) / Tunit;
			if (Trate < Brate) { Bunit = Tunit; Brate = Trate; Brip = rip; }
		}
	}
	//
	Multi = pow(10., rank - 1);			// 1***, 4***,8***, を 100,200,300,... で探る
	for (int i = 1; i <= 5; i++) {
		Tunit = i*Multi;
		if (fmod(*Vmax, Tunit) == 0.) rip = (int)(*Vmax / Tunit);
		else                          rip = (int)(*Vmax / Tunit) + 1;
		if (rip < 2 || rip > 8) continue;
		//
		Trate = (Tunit - fmod((*Vmax), Tunit)) / Tunit;
		if (Trate < Brate) { Bunit = Tunit; Brate = Trate; Brip = rip; }
	}

	//
	*Rip = Brip;
	*Unit = Bunit;
	//
	if (Bunit > 1.) *Fm = 0;
	else {
		rank = 1;
		while (fmod(Bunit*pow(10., rank), 1.) != 0.) rank++;
		*Fm = rank;
	}
}
BOOL	Save_Color_Data(CString Boot, PARAMS* PM)
{
	CString		fname;
	//
	fname.Format("%s\\RRI_CONTENTS\\Color_Data_V.dat", Boot);
	FILE*		fp = fopen(fname, "w");
	if (fp == NULL) return FALSE;
	//
	for (int i = 0; i<FILE_OUT_NUM; i++) {
		if (PM->OUTCOL[i].set) {
			fprintf(fp, "%d  %d %d %d %.2lf\n", i+100, PM->OUTCOL[i].num,
				(int)PM->OUTCOL[i].Equal, (int)PM->OUTCOL[i].Mos, PM->OUTCOL[i].alpha);
			for (int k = 0; k<PM->OUTCOL[i].num; k++) {
				fprintf(fp, "%lf %d %d %d\n", PM->OUTCOL[i].val[k],
					GetRValue(PM->OUTCOL[i].Rcol[k]), GetGValue(PM->OUTCOL[i].Rcol[k]), GetBValue(PM->OUTCOL[i].Rcol[k]));
			}
		}
	}
	fclose(fp);
	return TRUE;
}


CTime	StrDate_Time(char*  buf)
{
	char		cstr[SIZE_1K];
	int			pos, stg, len;
	int			val, year, month, day, hour, min, sec;
	//
	len = (int)strlen(buf);
	pos = stg = 0;
	year = month = day = hour = min = sec = UNDEF;
	//
	for (int k = 0; k<SIZE_1K; k++) cstr[k] = 0x00;
	for (int i = 0; i<len; i++) {
		if (buf[i] == '/' || buf[i] == ':' || (stg == 2 && pos != 0 && buf[i] == ' ')) {	// day と時間の間
			val = atoi(cstr);
			switch (stg) {
			case 0: year = val; stg++; break;
			case 1: month = val; stg++; break;
			case 2: day = val; stg++; break;
			case 3: hour = val; stg++; break;
			case 4: min = val; stg++; break;
			case 5: sec = val; stg++; break;
			}
			for (int k = 0; k<SIZE_1K; k++) cstr[k] = 0x00;
			pos = 0;
		}
		else 	cstr[pos++] = buf[i];
	}
	if (pos > 0)  {
		if (stg == 4)				{ min = atoi(cstr); stg++; }
		else if (stg == 5)		{ sec = atoi(cstr); stg++; }
		else                        return  NULL;
	}
	if (stg <= 4) return NULL;
	//
	if (stg == 5) return ::CTime(year, month, day, hour, min, 0, -1);
	else             return ::CTime(year, month, day, hour, min, sec, -1);
}

void		Copy_PARAMS(PARAMS* TPM, PARAMS OPM,BOOL CLEAR)
{
	if (CLEAR) memset(TPM, 0, sizeof(PARAMS));

	TPM->B_Color = OPM.B_Color;
	TPM->B_Back = OPM.B_Back;
	TPM->B_Mesh = OPM.B_Mesh;
	TPM->B_Legend = OPM.B_Legend;
	TPM->Show_Date = OPM.Show_Date;
	TPM->Show_Legend = OPM.Show_Legend;
	TPM->B_Date = OPM.B_Date;
	TPM->B_DateBack = OPM.B_DateBack;
	TPM->B_ShowTimeData = OPM.B_ShowTimeData;
	TPM->B_WLframe = OPM.B_WLframe;
	TPM->B_ColorMesh = OPM.B_ColorMesh;
	//
	TPM->B_Arrow = OPM.B_Arrow;
	TPM->B_IncludeRiver = OPM.B_IncludeRiver;
	TPM->Arrow_UserMag = OPM.Arrow_UserMag;
	TPM->Arrow_mag = OPM.Arrow_mag;
	TPM->Arrow_UserMag = OPM.Arrow_UserMag;
	TPM->Arrow_Ang = OPM.Arrow_Ang;
	TPM->Arrow_Head = OPM.Arrow_Head;
	//
	Copy_Coord(&TPM->LineID[0], OPM.LineID[0]);
	Copy_Coord(&TPM->LineID[1], OPM.LineID[1]);
	//
	TPM->Add_Info = OPM.Add_Info;
	//
	TPM->stLAT = OPM.stLAT;
	TPM->stLNG = OPM.stLNG;
	TPM->Size = OPM.Size;
	//
	//TPM->IM_Kind = OPM.IM_Kind;
	//strcpy(TPM->IM_Folder, OPM.IM_Folder);
	//
	TPM->Data_Alpha = OPM.Data_Alpha;
	TPM->Alpha = OPM.Alpha;
	Copy_Coord(&TPM->scale, OPM.scale);
	//
	// Shape Initial
	if (TPM->Smem != 0) SAFE_FREE(TPM->SBOX);
	TPM->Smem = OPM.Smem;
	TPM->Snum = OPM.Snum;
	if ((TPM->SBOX = (SHAPE_BOX*)malloc(sizeof(SHAPE_BOX)*TPM->Smem)) == NULL) {
		TPM->SBOX = NULL; TPM->Smem = 0;
	}
	for (int s = 0; s<TPM->Smem; s++) memcpy((void*)&TPM->SBOX[s], &OPM.SBOX[s], sizeof(SHAPE_BOX));
	//
	TPM->Col_Mesh = OPM.Col_Mesh;
	TPM->Col_MeshR = OPM.Col_MeshR;
	TPM->Col_MeshB = OPM.Col_MeshB;
	//
	TPM->Col_Date = OPM.Col_Date;
	TPM->Col_DateBack = OPM.Col_DateBack;
	TPM->Col_LegendChar = OPM.Col_LegendChar;
	TPM->Col_Arrow = OPM.Col_Arrow;
	TPM->Col_WLframe = OPM.Col_WLframe;
	////
	for (int k = 0; k < FILE_OUT_NUM; k++) {
		TPM->OUTCOL[k].set = OPM.OUTCOL[k].set;
		TPM->OUTCOL[k].alpha = OPM.OUTCOL[k].alpha;
		TPM->OUTCOL[k].num = OPM.OUTCOL[k].num;
		TPM->OUTCOL[k].Equal = OPM.OUTCOL[k].Equal;
		TPM->OUTCOL[k].Mos = OPM.OUTCOL[k].Mos;
		for (int n = 0; n < 11; n++) {
			TPM->OUTCOL[k].val[n] = OPM.OUTCOL[k].val[n];
			TPM->OUTCOL[k].Rcol[n] = OPM.OUTCOL[k].Rcol[n];
			TPM->OUTCOL[k].Fcol[n] = OPM.OUTCOL[k].Fcol[n];
		}
	}
	// 
	TPM->B_Data_Mosaic = OPM.B_Data_Mosaic;
	TPM->Data_Alpha = OPM.Data_Alpha;
	TPM->Back_Alpha = OPM.Back_Alpha;
	//
	TPM->Anim_Speed = OPM.Anim_Speed;
	TPM->Anim_Time = OPM.Anim_Time;
	TPM->WLmag = OPM.WLmag;
	strcpy(TPM->REC_Head, OPM.REC_Head);
	strcpy(TPM->REC_Folder, OPM.REC_Folder);




	TPM->SizeID = OPM.SizeID;
	TPM->B_WLframe = OPM.B_WLframe;	// 浸水枠線
	//
	TPM->ctmin = OPM.ctmin;
	TPM->ctmax = OPM.ctmax;
	//
	TPM->Tw = OPM.Tw;
	TPM->Th = OPM.Th;
	TPM->Tfont = OPM.Tfont;					// Lucida Sans
	TPM->Tstyle = OPM.Tstyle;		// FONT_REGULAR=1, 2, 3
	//
	//
	TPM->Lw = OPM.Lw;
	TPM->Lh = OPM.Lh;
	TPM->Lfont = OPM.Lfont;					// Arial
	TPM->Lstyle = OPM.Lstyle;		// FONT_REGULAR=1, 2, 3
	TPM->Lsize = OPM.Lsize;
}
//
/// ////////////////////////////////////////////////////////////////////////////////
// 
///  FILE I/O
//
/// ////////////////////////////////////////////////////////////////////////////////

int		Read_RRI_INPUT(DATASET* DS, FILE*  fp)
{
	char			buf[SIZE_1K], inchar[SIZE_01K];
	int			pos, ival, fileno = 0, Lowno = 0, id;
	double		dval;
	//_______________________________________________________________
	//  諸元のコピー
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (int i = 0; i<FILE_IN_NUM; i++) {
		DS->in_exist[i] = DS->in_use[i] = FALSE;
		DS->File_names[i][0] = 0x00;
		DS->Input_names[i][0] = 0x00;
		if (i < FILE_OUT_NUM) {
			DS->Out_names[i][0] = 0x00;
			DS->out_exist[i] = DS->out_use[i] = FALSE;
		}
	}
	//_______________________________________________________________
	//	読込開始
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Lowno = 1;
	while (fgets(buf, SIZE_1K, fp) != NULL) {
		if (Lowno > 100 && strlen(buf) == 0) break;	// １００行目の改行

		pos = 0; getstr(buf, &pos, inchar, SIZE_01K);
		switch (Lowno) {
			//_______________________________________________________________
			//		1 基本ファイル
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		case 1: strcpy(DS->Proj_Title, inchar); break;
		case 3: strcpy(DS->Input_names[RAIN_FILE], inchar); DS->in_use[RAIN_FILE] = TRUE;
			DS->Cin_Need++; break;
		case 4: strcpy(DS->Input_names[ADEM_FILE], inchar); DS->in_use[ADEM_FILE] = TRUE;
			DS->Cin_Need++; break;
		case 5: strcpy(DS->Input_names[ACC_FILE], inchar); DS->in_use[ACC_FILE] = TRUE;
			DS->Cin_Need++; break;
		case 6: strcpy(DS->Input_names[ADIR_FILE], inchar); DS->in_use[ADIR_FILE] = TRUE;
			DS->Cin_Need++; break;
			//_______________________________________________________________
			//		2 [simulation]諸元
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		case 8: sscanf(buf, "%d", &ival); DS->SW_LatLong = (BYTE)ival;break;
		case 10: sscanf(buf, "%d", &DS->Hours); break;
		case 11: sscanf(buf, "%d", &DS->DT); break;
		case 12: sscanf(buf, "%d", &DS->DT_Riv); break;
		case 13: sscanf(buf, "%d", &DS->DT_Out); DS->DT_Inter = DS->Hours * 3600 / DS->DT_Out; break;

			//_______________________________________________________________
			//		3 [landuse]諸元
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		case 18: sscanf(buf, "%lf", &DS->Ns_River); break;
			//_______________________________________________________________
			//		 河川の幅、深さの決定
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		case 38: sscanf(buf, "%lf", &DS->Riv_Thresh); break;
		case 39: sscanf(buf, "%lf", &DS->Cof_WC); break;
		case 40: sscanf(buf, "%lf", &DS->Cof_WS); break;
		case 41: sscanf(buf, "%lf", &DS->Cof_DC); break;
		case 42: sscanf(buf, "%lf", &DS->Cof_DS); break;
		case 43: sscanf(buf, "%lf", &DS->Cof_H); break;
		case 44: sscanf(buf, "%lf", &DS->Cof_Hlim); break;

			//_______________________________________________________________

			//_______________________________________________________________
			//		6 河川ファイルの設定
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		case 46: sscanf(buf, "%d", &ival);
			DS->in_use[WIDTH_FILE] = DS->in_use[DEPTH_FILE] = DS->in_use[HEIGHT_FILE] = (BOOL)ival;
			break;
		case 47: strcpy(DS->Input_names[WIDTH_FILE], inchar); break;
		case 48: strcpy(DS->Input_names[DEPTH_FILE], inchar); break;
		case 49: strcpy(DS->Input_names[HEIGHT_FILE], inchar); break;
	//_______________________________________________________________
	//		OUT 設定ファイル
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		case 87: pos = 0;
			for (int i = 0; i < FILE_OUT_NUM; i++) {		// OUT_TECPLOT を除く
				getstr(buf, &pos, inchar, SIZE_01K);
				DS->out_use[i] = (BOOL)atoi(inchar);
			}
			break;
		case 88: strcpy(DS->Out_Header[OUT_HS], inchar); break;
		case 89: strcpy(DS->Out_Header[OUT_HR], inchar); break;
		case 90: strcpy(DS->Out_Header[OUT_HG], inchar); break;
		case 91: strcpy(DS->Out_Header[OUT_QR], inchar); break;
		case 92: strcpy(DS->Out_Header[OUT_QU], inchar); break;
		case 93: strcpy(DS->Out_Header[OUT_QV], inchar); break;
		case 94: strcpy(DS->Out_Header[OUT_GU], inchar); break;
		case 95: strcpy(DS->Out_Header[OUT_GV], inchar); break;
		case 96: strcpy(DS->Out_Header[OUT_GAMPUT], inchar); break;
		case 97: strcpy(DS->Out_Header[OUT_STORAGE], inchar); break;
			//
		case 99: DS->in_use[LOCATION_FILE] = (BOOL)atoi(inchar); break;
		case 100: strcpy(DS->Input_names[LOCATION_FILE], inchar); break;
			//
		default: break;
		}
		Lowno++;
	}
	if (Lowno < 100) return 100;
	fclose(fp);
	//
	Set_File_FullName(DS);
	///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// 存在確認
	char		fname[SIZE_05K];
	DS->Cin_Exist = 0;
	for (int i = 0; i < FILE_IN_NUM; i++) {
//		if (!DS->in_use[i]) continue;
		//
		DS->in_exist[i] = FALSE;
		if (File_Exist(DS->File_names[i])) { DS->in_exist[i] = TRUE; DS->Cin_Exist++; }
	}
	//
	DS->Cout_Exist = 0;
	for (int i = 0; i < FILE_OUT_NUM; i++) {
		if (!DS->out_use[i]) continue;
		if (i != OUT_STORAGE ) sprintf(fname, "%s000001.out", DS->Out_names[i]);
		else                  strcpy(fname,DSET.Out_names[i]);
		//
		DS->out_exist[i] = FALSE;
		if (File_Exist(fname)) { DS->out_exist[i] = TRUE; DS->Cout_Exist++; }
	}
	//
	return 0;
}

int		Read_LOCATION_TEXT(LOCATION_INFO* LC, FILE*  fp)
{
	char	buf[SIZE_1K];
	int		Lowno = 0;

	// Clear
	for (int i = 0; i<LOCATION_NUM; i++) {
		LC->name[i][0] = 0x00;
		LC->mz[i] = 0;
		LC->mx[i] = 0;
		LC->Hid[i] = 0;
		LC->Vid[i] = 0;
		LC->lat[i] = 0.0;
		LC->lng[i] = 0.0;
		LC->X[i] = 0.0;
		LC->Z[i] = 0.0;
	}

	// Read
	while (fgets(buf, SIZE_1K, fp) != NULL) {
		sscanf(buf, "%s %d %d", &LC->name[Lowno], &LC->mz[Lowno], &LC->mx[Lowno]);
		Lowno++;
	}

	return 0;
}

void		Make_String(double val, CString* str, int limit)
{
	CString		form;
	int			rank = 0;
	//
	if (val == 0.) *str = "0";
	else {
		double		aval = fabs(val) + EPS8;
		//
		while (fmod(aval*pow(10., rank), 1.) >= EPS3) { rank++; if (rank > 4) break; }
		//
		if (limit != UNDEF) {
			if (rank > limit) rank = limit;
		}
		//
		if (rank == 0)		str->Format("%d", (int)val);
		else	{
			form.Format("%%.%dlf", rank);
			str->Format(form, val);
		}
	}
}

int		Get_ArcGIS_Info(char* fname, int* Hnum, int* Vnum, double* stLAT, double* stLNG, double* Size)
{
	FILE*	fp = fopen(fname, "r");
	if (fp == NULL) return 1;
	// -------------------------------------------------------------------------Setting
	fscanf(fp, "%*s %d", Hnum);						// 諸元の読込
	fscanf(fp, "%*s %d", Vnum);
	fscanf(fp, "%*s %lf", stLNG);
	fscanf(fp, "%*s %lf", stLAT);
	fscanf(fp, "%*s %lf", Size);
	fscanf(fp, "%*s %*s");
	//
	fclose(fp);
	return 0;
}

int		Set_File_FullName(DATASET* DS)
{
	if (strlen(DS->Proj_Folder) == 0) return UNDEF;
	//
	for (int f = 0; f < FILE_IN_NUM; f++) {
		if (strlen(DS->Input_names[f]) == 0) continue;
		//
		sprintf(DS->File_names[f], "%s%s", DS->Proj_Folder, &DS->Input_names[f][1]);
		for (int i = 0; i < strlen(DS->File_names[f]); i++){
			if (DS->File_names[f][i] == 0x5c) DS->File_names[f][i] = 0x2f;
		}
	}
	for (int f = 0; f < FILE_OUT_NUM; f++) {
		if (strlen(DS->Out_Header[f]) == 0) continue;
		//
		sprintf(DS->Out_names[f], "%s%s", DS->Proj_Folder, &DS->Out_Header[f][1]);
		for (int i = 0; i < strlen(DS->Out_names[f]); i++){
			if (DS->Out_names[f][i] == 0x5c) DS->Out_names[f][i] = 0x2f;
		}
	}
	return 0;
}


int		Set_Relative_Name(DATASET* DS)
{
	for (int f = 0; f < FILE_IN_NUM; f++) {
		if (strlen(DS->File_names[f]) == 0) continue;
		//
		Get_Relative_Path(DS->File_names[f], DS->Proj_Folder, DS->Input_names[f]);
	}
	return 0;
}

///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ファイルの読み込み
int  Read_Data_File(int f, CString fname, MINFO** MESH)
{
	FILE*		fp;
	char			buf[SIZE_16K], inchar[SIZE_01K], name[SIZE_01K];
	int			pos, cnt;
	int			ID, vr;
	int			nH, nV, nT;
	//CString		Estr;
	double		val, vmax, vmin;
	short		sval, smax, smin;
	int			Lcnt = 0;		// ファイル行数
	//
	if ((fp = fopen(fname, "r")) == NULL) { return 101; }
	//

	//-----------------------------------------------------------------------------------------------------
	//  File Format <= 1  1->12
	//
	if (File_Format[f] <= 1)  {	// Param+Mesh  or Mesh
		if (File_Format[f] == 0) for (int i = 0; i<6; i++) fgets(buf, SIZE_12K, fp);	// Param + mesh
		//		cnt = 0;
		vmax = DUNDEF; vmin = -DUNDEF;
		smax = SUNDEF; smin = -SUNDEF;
		for (int v = 0; v<PM.MVnum; v++) {
			if (fgets(buf, SIZE_16K, fp) == NULL) { fclose(fp); return 1; }
			pos = 0;
			vr = PM.MVnum - v - 1;
			for (int h = 0; h<PM.MHnum; h++)  {
				if (getstr(buf, &pos, inchar, SIZE_01K) == EOF)  {
					fclose(fp);
					return 100;
				}
				//				cnt = (PM.Vnum - v - 1)*PM.Hnum + h;
				val = DUNDEF;
				sval = SUNDEF;
				switch (f) {
				case 1: val = MESH[vr][h].Adem = atof(inchar); break;
				case 2: val = MESH[vr][h].Acc = atof(inchar); break;
				case 3: val = MESH[vr][h].Adir = atoi(inchar); break;

				case 5: val = MESH[vr][h].Rwidth = atof(inchar); break;
				case 6: val = MESH[vr][h].Rdepth = atof(inchar); break;
				case 7: val = MESH[vr][h].Bheight = atof(inchar); break;
				}
				if (val != DUNDEF) {
					if (vmax < val) vmax = val; if (val > 0. && vmin > val) vmin = val;
				}
				if (sval != SUNDEF) {
					if (smax < sval) smax = sval; if (sval > 0 && smin > sval) smin = sval;
				}

				//
				// メッシュ有効・無効の識別　Adem ファイルによる
				// 河川フラッグは　ACC ファイルによる。
				if (f == ADEM_FILE) {
					if (MESH[vr][h].Adem  > -900.)	{	// 本来　-9999
						MESH[vr][h].flag = 1;
						PM.Anum++;
					}
					else	{
						MESH[vr][h].flag = 0;
					}
				}
				else if (f == ACC_FILE) {
					if (MESH[vr][h].flag == 1 && MESH[vr][h].Acc >= DSET.Riv_Thresh) MESH[vr][h].flag = 2;
				}
			}
		}
	}

	fclose(fp);
	//
	return 0;
}

BOOL		Is_Same_Pos(IXYCOORD p1, IXYCOORD p2)
{
	if ((p1.x == p2.x) && (p1.y == p2.y)) return TRUE;
	return FALSE;
}
//FCOLOR		CREF_FCOL(COLORREF  col)
//{
//	return F_COLOR(GetRValue(col), GetGValue(col), GetBValue(col));
//}
BOOL		Is_InRect(double x, double y, XYCOORD* xy, BOOL Online)
{
	double	xmin, xmax, ymin, ymax;
	xmin = ymin = -DUNDEF;
	xmax = ymax = DUNDEF;
	for (int i = 0; i < 4; i++) {
		if (xmin > xy[i].x) xmin = xy[i].x;
		if (xmax < xy[i].x) xmax = xy[i].x;
		if (ymin > xy[i].y) ymin = xy[i].y;
		if (ymax < xy[i].y) ymax = xy[i].y;
	}
	if (Online) {
		if (x >= xmin && x <= xmax) {
			if (y >= ymin && y <= ymax) return TRUE;
		}
	}
	else {
		if (x > xmin && x < xmax) {
			if (y > ymin && y < ymax) return TRUE;
		}
	}
	return FALSE;
}
