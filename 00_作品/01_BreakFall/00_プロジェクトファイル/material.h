//==========================================================================================================
//
//マテリアルの処理[material.h]
// Author Murata Shinnosuke
//
//==========================================================================================================
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

//インクルード
#include "component.h"

//マテリアル
class Material : public Component   //(親)コンポーネント
{
public:
	//コンストラクタ
	Material()
	{
		//メンバ変数を初期化する
		this->m_color = { 1.0f, 1.0f, 1.0f, 1.0f };   //色
		this->m_pTexture = nullptr;   //テクスチャ
	}

	//デストラクタ
	~Material()
	{
	}

	void SetColor(D3DXCOLOR color) { this->m_color = color; };   //色の設定処理
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture) { this->m_pTexture = pTexture; };   //テクスチャの設定処理

	D3DXCOLOR GetColor() { return this->m_color; };   //色の取得処理
	LPDIRECT3DTEXTURE9 GetTexture() { return this->m_pTexture; };   //テクスチャの取得処理

	//生成処理
	static Material* Create(Object* pObject)
	{
		//マテリアルを生成する
		Material* pMaterial = new Material;

		//情報を設定する
		pMaterial->SetObject(pObject);

		//マテリアルを返す
		return pMaterial;
	}

private:
	D3DXCOLOR m_color;   //色
	LPDIRECT3DTEXTURE9 m_pTexture;   //テクスチャ
};

#endif // !_MATERIAL_H_
