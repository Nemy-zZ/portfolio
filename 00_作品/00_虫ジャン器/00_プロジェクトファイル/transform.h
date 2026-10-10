//========================================================================================================
//
//トランスフォームの処理[transform.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "component.h"

//トランスフォーム
class Transform : public Component
{
public:
	//コンストラクタ
	Transform(Object * pObject) : Component(pObject)
	{
		//メンバ変数を初期化する
		this->m_pos = { 0.0f, 0.0f, 0.0f };   //位置
		this->m_posOld = this->m_posOld;   //前回の位置
		this->m_rot = { 0.0f, 0.0f, 0.0f };   //向き
		this->m_scale = { 1.0f, 1.0f, 1.0f };   //拡大率
	}

	//デストラクタ
	~Transform() override
	{
	}

	void Update() override { this->m_posOld = this->m_pos; };   //更新処理
	void AddPos(D3DXVECTOR3 posAdd) { this->m_pos += posAdd; };   //位置の加算
	void AddRot(D3DXVECTOR3 rotAdd) { this->m_rot += rotAdd; };   //向きの加算

	void SetPos(D3DXVECTOR3 pos) { this->m_pos = pos; };   //位置の設定処理
	void SetRot(D3DXVECTOR3 rot) { this->m_rot = rot; };   //向きの設定処理
	void SetScale(D3DXVECTOR3 scale) { this->m_scale = scale; };   //拡大率の設定処理

	D3DXVECTOR3 GetPos() { return this->m_pos; };   //位置の取得
	D3DXVECTOR3 GetPosOld() { return this->m_posOld; };   //前回の位置の取得
	D3DXVECTOR3 GetRot() { return this->m_rot; };   //向きの取得
	D3DXVECTOR3 GetScale() { return this->m_scale; };   //拡大率の取得

	//ワールドマトリックスの取得処理
	D3DXMATRIX GetMtxWorld()
	{
		D3DXMATRIX mtxWorld;   //ワールドマトリックス
		D3DXMATRIX mtxRot, mtxTrans;   //計算用マトリックス

		//スケーリングを反映する
		D3DXMatrixScaling(&mtxWorld, this->m_scale.x, this->m_scale.y, this->m_scale.z);

		//向きを反映する		
		D3DXMatrixRotationYawPitchRoll(&mtxRot, this->m_rot.y, this->m_rot.x, this->m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//位置を反映する		
		D3DXMatrixTranslation(&mtxTrans, this->m_pos.x, this->m_pos.y, this->m_pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		//ワールドマトリックスを返す
		return mtxWorld;
	}

private:
	D3DXVECTOR3 m_pos;   //位置
	D3DXVECTOR3 m_posOld;   //前回の位置
	D3DXVECTOR3 m_rot;   //向き
	D3DXVECTOR3 m_scale;   //拡大率
};

//2Dトランスフォーム
class Transform2D : public Component
{
public:
	//コンストラクタ
	Transform2D(Object* pObject) : Component(pObject)
	{
		//メンバ変数を初期化する
		this->m_pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };   //位置
		this->m_size = { 0.0f, 0.0f };   //サイズ
		this->m_fRot = 0.0f;   //向き
	}

	//デストラクタ
	~Transform2D() override
	{
	}

	void SetPos(D3DXVECTOR2 pos) { this->m_pos = pos; };   //位置の設定処理
	void SetRot(float fRot) { this->m_fRot = fRot; };   //向きの設定処理
	void SetSize(D3DXVECTOR2 size) { this->m_size = size; };   //拡大率の設定処理

	D3DXVECTOR2 GetPos() { return this->m_pos; };   //位置の取得
	D3DXVECTOR2 GetSize() { return this->m_size; };   //拡大率の取得
	float GetRot() { return this->m_fRot; };   //向きの取得

private:
	D3DXVECTOR2 m_pos;   //位置
	D3DXVECTOR2 m_size;   //サイズ
	float m_fRot;   //向き
};

#endif // !_TRANSFORM_H_