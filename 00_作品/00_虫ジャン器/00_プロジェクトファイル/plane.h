//========================================================================================================
//
//平面の処理[plane.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _PLANE_H_
#define _PLANE_H_

#include "rendering.h"
#include "manager.h"

//平面クラス
class Plane : public Rendering   //(親)レンダリング
{
public:
	static const D3DXVECTOR2 BASE_SIZE;   //基本サイズ

	//コンストラクタ
	Plane(Object* pObject) : Rendering(pObject)
	{
		//メンバ変数を初期化する
		this->m_pVtxBuffer = nullptr;   //バッファ
	}

	//デストラクタ
	virtual ~Plane() override
	{
	}

	void Init() override;   //初期化処理
	void Uninit() override;   //終了処理
	virtual void Draw() override;   //描画処理
	virtual void Update() override = 0;   //更新処理

protected:
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuffer() { return this->m_pVtxBuffer; };   //頂点バッファの取得

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;   //頂点バッファ
};

//XZ方向の平面
class PlaneXZ : public Plane
{
public:
	//コンストラクタ
	PlaneXZ(Object* pObject) : Plane(pObject)
	{
	}

	//デストラクタ
	~PlaneXZ() override
	{
	}

	//更新処理
	void Update() override;
};

//Y方向の平面
class PlaneY : public Plane
{
public:
	//コンストラクタ
	PlaneY(Object* pObject) : Plane(pObject)
	{
	}

	//デストラクタ
	virtual ~PlaneY() override
	{
	}

	//更新処理
	void Update() override;
};

//ビルボード
class Billboard : public PlaneY
{
public:
	//コンストラクタ
	Billboard(Object* pObject) : PlaneY(pObject)
	{
		//メンバ変数を初期化する
		this->m_isYAxis = false;   //Y軸
		this->m_isMultiply = false;   //加算合成
	}

	//デストラクタ
	~Billboard() override
	{
	}

	void Draw() override;

protected:
	void YAxis() { this->m_isYAxis = true; };   //Y軸ビルボード
	void Multiply() { this->m_isMultiply = true; };   //加算合成

private:
	bool m_isYAxis;   //Y軸
	bool m_isMultiply;   //加算合成
};

#endif // !_PLANE_H_
