//========================================================================================================
//
//マスクの処理[mask.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _MASK_H_
#define _MASK_H_

#include "rendering.h"

//マスク
class Mask : public Rendering   //(親)レンダリング
{
public:
	//コンストラクタ
	Mask(Object* pObject) : Rendering(pObject)
	{
		//メンバ変数を初期化する
		this->m_pVtxBuffer = nullptr;
	}

	//デストラクタ
	~Mask() override
	{
	}

	void Init() override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	void Draw() override;   //描画処理

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;   //頂点バッファ
};

#endif // !_MASK_H_
