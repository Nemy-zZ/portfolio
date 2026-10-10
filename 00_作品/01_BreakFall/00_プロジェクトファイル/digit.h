//========================================================================================================
//
//桁の処理[digit.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _DIGIT_H_
#define _DIGIT_H_

#include "main.h"

//桁クラス
class Digit
{
public:
	//コンストラクタ
	Digit()
	{
		//メンバ変数を初期化する
		this->m_pVtxBuffer = nullptr;
		this->m_nValue = 0;
		this->m_pos = { 0.0f, 0.0f };
		this->m_size = { 0.0f, 0.0f };
	}

	//デストラクタ
	~Digit()
	{
	}

	void Init();   //初期化処理
	void Uninit();	  //終了処理
	void Update();	  //更新処理
	void Draw(LPDIRECT3DTEXTURE9 pTexutre);	//描画処理

	void SetValue(int nValue) { this->m_nValue = nValue; };   //数値の設定処理
	void SetPos(D3DXVECTOR2 pos) { this->m_pos = pos; };   //位置の設定処理
	void SetSize(D3DXVECTOR2 size) { this->m_size = size; };   //サイズの設定処理

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;   //頂点バッファ
	int m_nValue;   //数値
	D3DXVECTOR2 m_pos;   //位置
	D3DXVECTOR2 m_size;   //サイズ
};

#endif // !_DIGIT_H_