/******************************************************
CInBitStream ビットストリーム・クラス
******************************************************/

#pragma once

// ビット取り出しのためのマスク
static const int	kBitTestMaskT[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

class CInBitStream
{
public:
	// コンストラクタ
			CInBitStream( 
				char* 	aBuf,				// バッファ
				int		size );				// バッファのサイズ

	// ビット単位で読み出す
	int		GetBits( 						// 返値：読み出しデータ
				int 	numOfBits );		// 読み出しビット数

	// 1バイト読み出す
	u_char	GetByte( void );				// 返値：読み出した整数

	// 2バイト読み出す
	u_int	GetWord( void );				// 返値：読み出した整数

	// nバイト読み出す
	void	CopyByte(
				char*	dis,				// 格納先アドレス
				int		n );				// 読み出すバイト数

	// nバイト進める
	void	SkipByte(
				int		n );				// 読み進めるバイト数

	// 次の読み出しアドレス
	char*	GetNextAddress( void );			// 返値：次の読み出しアドレス

protected:
	u_char*	mBuf;							// 読み出しアドレス
	u_char*	mEndOfBuf;						// バッファの終了アドレス
	int		mBitPos;						// 読み出しビット位置（上位ビットが7、下位ビットが0）
	int		mNextFlag;						// 次のバイトを読んでいいかどうか
	int		mReadFlag;						// 1:読み出し可, 0:読み出し不可

	// 読み出しアドレスのインクリメントとアクセス違反のチェック
	void	IncBuf( void );
};
