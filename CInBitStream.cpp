/******************************************************
CInBitStream ビットストリーム・クラス
	Copyright 1998 K-Work.Co.,Ltd
	All rights reserved.

Ver.1.0.0

# The comment is written in Japanese (Shift-JIS).
******************************************************/

#include <string.h>

#ifdef _MSC_VER						// Visual C++のバージョンが定義されていたら
#include "JPEG.h"			// Visual C++
#else
#include "JPEG.h"					// Code Warrior
#endif

#include "CInBitStream.h"

CInBitStream::CInBitStream(
	char* 	aBuf,						// バッファ
	int		size )						// バッファのサイズ
{
	// データバッファの設定
	mBuf = (u_char*)aBuf;				// バッファ
	mEndOfBuf = mBuf + size;			// バッファの最終アドレス

	// 状態変数初期化
	mBitPos = 7;						// 最上位ビット
	mNextFlag = 1;						// 読み飛ばし無し
	mReadFlag = 1;						// アクセスエラー無し
}

// 読み出しアドレスのインクリメントとアクセス違反のチェック
void
CInBitStream::IncBuf( void )
{
	if( ++mBuf >= mEndOfBuf )			// 次のアクセスでエラー
		mReadFlag = 0;
}

// ビット単位で読み出す
int										// 読み出したビット値：エラーコード
CInBitStream::GetBits(
	int		numOfBits )					// 読み出すビット数
{
	if( numOfBits <= 0 )
		return 0;						// エラー

	u_char	c;
	int		r = 0;						// 返値
	while( numOfBits ){
		if( mBitPos < 0 ){				// 次のバイトを読み出すとき
			mBitPos = 7;				// 読み出しビット位置更新
			IncBuf();					// アドレス更新
			if( mNextFlag == 0 ){		// 0xffに続く0x00を読み飛ばす
				IncBuf();
				mNextFlag = 1;
			}	// mNextFlag == 0

			c = *mBuf;					// 読み出し位置の値
			if( c == 0xff ){			// マーカと同じだったら
				c = *( mBuf + 1 );		// 次のバイトをチェック
				if( c != 0 )			// マーカだったら
					return - c;			// エラーとしてコードを負にして返す
				mNextFlag = 0;			// 次に続く0x00を読み飛ばさせる
			}	// c == 0xff
		}	// mBitPos < 0

		// 返値の作成
		r <<= 1;
		r |= ( (*mBuf) & kBitTestMaskT[mBitPos--] ) ? 1 : 0;
										// 1ビット読み出しと読み出しビット位置更新
		numOfBits--;					// 読み出しビット数更新
	}	// while
	return	r;
}

// 1バイト読み出す
u_char									// 返値：読み出した整数
CInBitStream::GetByte( void )
{
	if( mReadFlag ){
		if( mBitPos != 7 ){				// 読み出し位置がバイトの途中ならば
			IncBuf();					// 次のバイトの
			mBitPos = 7;				// 最初のビットから
		}
		u_char r = *mBuf;				// 1バイト読み出し
		IncBuf();						// 読み出し位置更新
		mNextFlag = 1;					// ビット読み出し中にマーカを検出した場合0になるが、
										// GetByte()でマーカを処理するので次のバイトを読んでよい
		return	r;
	}
	throw( EBufferError( "GetByte" ) );	// バッファオーバー
	return	(u_char)0;					// Visual C++用ダミー
}

// 2バイト読み出す
u_int									// 返値：読み出した整数
CInBitStream::GetWord( void )
{
	if( mReadFlag ){
		if( mBitPos != 7 ){				// バイトの途中から読み出さない
			IncBuf();
			mBitPos = 7;
		}
		u_int r = (unsigned)*mBuf << 8;
		IncBuf();
		r |= (unsigned)*mBuf;
		IncBuf();
		return	r;
	}
	throw( EBufferError( "GetWord" ) );
	return	(u_char)0;					// Visual C++用ダミー
}

// nバイト読み出す
void
CInBitStream::CopyByte(
	char*	dis,						// 格納先アドレス
	int		n )							// 読み出すバイト数
{
	if( mBitPos != 7 ){					// バイトの途中から読み出さない
		IncBuf();
		mBitPos = 7;
	}
	if( mBuf + n > mEndOfBuf )
		throw( EBufferError( "CopyByte" ) );
	memcpy( dis, mBuf, n );
	if( (mBuf += n) >= mEndOfBuf )		// 次のアクセスでエラー
		mReadFlag = 0;
}

// nバイト進める
void
CInBitStream::SkipByte(
	int		n )							// 読み進めるバイト数
{
	if( mBuf + n > mEndOfBuf )			// この時点でアクセスエラーが確実ならば
		throw( EBufferError( "SkipByte" ) );
	if( (mBuf += n) >= mEndOfBuf )		// 次のアクセスでエラー
		mReadFlag = 0;
}

// 次の読み出しアドレス
char*									// 次のアドレス
CInBitStream::GetNextAddress( void )
{
	if( mBitPos == 7 )					// 読み出し途中でないなら
		return (char*)mBuf;
	else if( mBuf < mEndOfBuf )
		return (char*)(mBuf + 1);
	else
		return nil;
}
