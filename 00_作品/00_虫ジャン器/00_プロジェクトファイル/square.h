//========================================================================================================
//
//四角形の処理[square.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _SQUARE_H_
#define _SQUARE_H_

#include "rendering.h"

//四角形クラス
class Square : public Rendering   //(親)レンダリング
{
public:
	//コンストラクタ
	Square(Object* pObject) : Rendering(pObject)
	{
		//メンバ変数を初期化する
		this->m_pVtxBuffer = nullptr;
	}

	//デストラクタ
	~Square() override
	{
	}

	void Init() override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理
	void Draw() override;   //描画処理

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;   //頂点バッファ
};

#endif // !_SQUARE_H_
