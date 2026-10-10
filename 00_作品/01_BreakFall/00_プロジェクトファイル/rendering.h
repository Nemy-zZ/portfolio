//========================================================================================================
//
//レンダリングの処理[rendering.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _RENDERING_H_
#define _RENDERING_H_

#include "component.h"

//レンダリング
class Rendering : public Component
{
public:
	//コンストラクタ
	Rendering(Object* pObject) : Component(pObject)
	{
		//メンバ変数を初期化する
		this->m_pTexture = nullptr;   //テクスチャ
		this->m_color = { 1.0f, 1.0f, 1.0f, 1.0f };   //色
	}

	//デストラクタ
	virtual ~Rendering() override
	{
	}

	virtual void Init() override = 0;   //初期化処理
	virtual void Uninit() override = 0;   //終了処理
	virtual void Update() override = 0;   //更新処理
	virtual void Draw() override = 0;   //描画処理

	void SetColor(D3DXCOLOR color) { this->m_color = color; };   //色の設定
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture) { this->m_pTexture = pTexture; };   //テクスチャの設定

	D3DXCOLOR GetColor() { return this->m_color; };   //色の取得
	LPDIRECT3DTEXTURE9 GetTexture() { return this->m_pTexture; };   //テクスチャの取得

private:
	LPDIRECT3DTEXTURE9 m_pTexture;   //テクスチャ
	D3DXCOLOR m_color;   //色
};

#endif // !_RENDERING_H_