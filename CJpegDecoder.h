/******************************************************
CJpegDecoder JPEGデコーダ・クラス
	Copyright 1998 K-Work.Co.,Ltd
	All rights reserved.

Ver.1.0.0

# The comment is written in Japanese (Shift-JIS).

JPEGデコーダ・クラス
	JPEGデータをRGB画像データへ変換するクラス

使用方法
	1.インスタンスを生成する
	2.SetJpegData()で画像データへのポインタとサイズを渡す
	3.SetJpegData()の返値でエラーを確認する
	4.DoDisconvert()で逆変換を実行する
	5.DoDisconvert()の返値でエラーを確認する
	6.GetPictData()でRGB画像データを取り出す

注意事項
	1.このファイルより先に"JPEG.h"をインクルードすること
	2.GetPictData()の redP, greenP, blueP の領域はCJpegDecoderがnewで確保する。
	　呼び出し側でdeleteすること。
******************************************************/
#pragma once

/**************/
/* データ定義 */
/**************/

enum
eAnalysisResult {		// ヘッダ解析結果
	emYet			= 0,			// 未解析
	emIsHTable		= 0x01,			// ハフマンテーブル有り
	emIsQTable		= 0x02,			// 量子化テーブル有り
	emIsJFIF		= 0x04,			// JFIFフォーマット指定有り
	emIsComment		= 0x08,			// コメント有り
	emIsStartData	= 0x10			// スキャンデータ有り
};

// フレーム成分指定パラメータ
struct
SFrameComponent {
	u_char		C;					// 成分識別子
	char		H;					// 水平サンプリングファクタ
	char		V;					// 垂直サンプリングファクタ
	char		Tq;					// 量子化テーブルセレクタ
};

// スキャン成分指定パラメータ
struct
SScanComponent {
	u_char		Cs;					// スキャン成分セレクタ
	char		Td;					// DCエントロピ符号化テーブルのセレクタ
	char		Ta;					// ACエントロピ符号化テーブルのセレクタ
};

// スキャンデータのヘッダ
struct
SScanHeader {
	u_char		numOfScanComp;		// スキャン成分数
	u_char		SpectralStart;		// スペクトラル開始
	u_char		SpectralEnd;		// スペクトラル終了
	char		Ah;					// サクセッシブ・アプロキシメーションの上位ビット
	char		Al;					// サクセッシブ・アプロキシメーションの下位ビット
};

// ハフマンテーブル
struct
SHuffmanDecodeTable {
	int			numOfElement;		// テーブル要素数
	int*		SizeTP;				// ハフマンサイズテーブル
	int*		CodeTP;				// ハフマン符号語テーブル
	int*		ValueTP;			// ハフマン符号語に対応した値
};

/**************/
/* クラス定義 */
/**************/

class CInBitStream;

class CJpegDecoder
{
public:
	// コンストラクタ
				CJpegDecoder( void );
	// デストラクタ
				~CJpegDecoder( void );

	// JPEGデータを設定する
	int		SetJpegData(							// 返値：処理結果
					char*		aJpegDataP,			// JPEGデータ
					int			size );				// JPEGデータのサイズ

	// 復号後の画像データへのポインタを返す。
	//		redP, greenP, blueP の領域はCJpegDecoderがnewで確保する。
	//		呼び出し側でdeleteすること。
	void		GetPictData(
					u_char**	redP,				// 復号画像へのポインタ
					u_char**	greenP,				// 復号画像へのポインタ
					u_char**	blueP,				// 復号画像へのポインタ
					int&		HSize,				// 横画素数
					int&		VSize );			// 縦画素数

	// 復号実行インタフェース
	int		DoDecode( void );						// 返値：復号結果

	// 画像特性の取得
	SJpegProperty	GetProperty( void )					// 返値：JPEG画像の特性
					{	return mProperty; };
	// エラー時のRGBデータ廃棄
	void		DeleteRGB( void );

protected:
	// 入出力,中間データ
	u_char*				mRgbP[3];					// 復号画像へのポインタ
	int*				mCompP[3];					// 画像の各色成分へのポイント(ユニットサイズ)
	int					mPreDC[3];					// 前ブロックのDC成分(3成分)
	int					mDctDataP[64];				// 1ブロック分のDCTデータ
	int					mBlockP[64];				// 1ブロック分の画素データ

	// 画像情報
public:
	SJpegProperty		mProperty;					// プロパティ
protected:
	SScanHeader			mSHeader;					// スキャンヘッダ
	SFrameComponent		mFComp[3];					// フレーム成分
	SScanComponent		mSComp[3];					// スキャン成分

	// テーブル
	SHuffmanDecodeTable	mHT[2][4];					// 成分ハフマンテーブル AC,DC*4種類
	int					mQT[4][64];					// 量子化テーブル要素 *4成分
	double				mCosT[8][8];				// cosテーブル

	// 復号時状態変数
	int					mEnable;					// 1:SOIからEOIの間を処理中, 0:それ以外
	CInBitStream		*mIBSP;						// JPEGデータ読み出し用ビットストリーム
	char				mHMax;						// 水平最大サンプリングファクタ
	char				mVMax;						// 垂直最大サンプリングファクタ
	int					mRestartInterval;			// リスタートインタバル（インタバル中のMCU数）

	// ヘッダの解析
	int			DoAnalysis( void );					// 返値：処理結果
	// マーカ識別
	eMarker		GetMarker( void );					// 返値：マーカコード
	// マーカ解釈
	int			AnalysisMarker( void );				// 返値：解析結果

	// JPEG復号実行
	void		Decode( void );
	// MCU復号
	void		DecodeMCU();
	// RGB変換
	void		makeRGB(
					int			ux,					// 横ユニット位置
					int			uy );				// 縦ユニット位置
	// エントロピー符号復号
	void		DecodeHuffmanBlock(
					int			cs );				// 成分番号
	// ハフマン符号1語の復号
	int			Decode1HuffmanCode(
					int			tc,					// テーブルクラス 0:DC成分, 1:AC成分
					int			sc );				// 成分番号
	// 逆量子化
	void		InverseQuantization( 
					int		sc );					// 成分番号
	// 1ブロック逆DCT変換
	void		InverseDCT( void );

	// ハフマンテーブル解析
	void		AnalysisDHT(
					int			s  );				// サイズ
	// 量子化テーブル解析
	void		AnalysisDQT(
					int			s  );				// サイズ
	// フレーム解析
	void		AnalysisFrame( void );
	// スキャンヘッダ解析
	void		AnalysisScan( void );
	// JFIFフォーマット解析
	void		AnalysisJFIF( void );
	// JFXXフォーマット解析
	void		AnalysisJFXX( void );

	// RGB算出用値補正
	u_char		ReviseValue(						// 返値：補正値
					double		v );				// 値
	// intサイズの領域確保
	void		NewIntMemory(
					int**		p,					// ポインタ
					int			s,					// サイズ
					int			v,					// 初期値
					char		*mes );				// エラーメッセージ
};

// ブロック数取得
#define GetBlocks(n)	(n>>3)+((n&0x07)>0)
