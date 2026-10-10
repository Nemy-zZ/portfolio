//========================================================================================================
//
//選択肢の処理[select_box.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _SELECT_BOX_H_
#define _SELECT_BOX_H_

#include "main.h"

//選択肢クラス
class SelectBox
{
public:
	static float INIT_FADE;   //フェードの初期値

	SelectBox();   //コンストラクタ
	~SelectBox();   //デストラクタ

	void Init();   //初期化処理
	void Uninit();   //終了処理
	void Update();   //更新処理
	void Draw(LPDIRECT3DTEXTURE9 pTexture);   //描画処理
	void Select() { this->m_isSelect = true; };   //選択フラグの立てる処理

	//リセット処理
	void Reset()
	{
		//リセットする
		this->m_color.a = 0.5f;
		this->m_fFade = SelectBox::INIT_FADE;
	}

	void SetPattern(int nPattern) { this->m_nPattern = nPattern; };
	void SetOder(int nOder) { this->m_nOder = nOder; };   //順番の設定処理
	void SetPos(D3DXVECTOR2 pos) { this->m_pos = pos; };   //位置の設定処理
	void SetSize(D3DXVECTOR2 size) { this->m_size = size; };   //サイズの設定処理

	static SelectBox* Create(D3DXVECTOR2 pos, D3DXVECTOR2 size);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;   //頂点バッファ
	int m_nPattern;   //分割数
	int m_nOder;   //順番
	D3DXVECTOR2 m_pos;   //位置
	D3DXVECTOR2 m_size;   //サイズ
	D3DXCOLOR m_color;   //色
	bool m_isSelect;   //選択フラグ
	float m_fFade;   //フェードの値
};

#endif // !_SELECT_BOX_H_