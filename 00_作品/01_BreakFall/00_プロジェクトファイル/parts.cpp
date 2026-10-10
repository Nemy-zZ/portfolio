//========================================================================================================
//
//パーツ処理[parts.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "parts.h"
#include "manager.h"
#include "renderer.h"

//========================================================================================================
//コンストラクタ
//========================================================================================================
Parts::Parts(int nPriority)
{
	//メンバ変数を初期化する
	this->m_nId = -1;
	this->m_mesh.pMesh = nullptr;   //メッシュ
	this->m_mesh.pBuffMat = nullptr;   //マテリアル
	this->m_mesh.dwMat = 0;   //マテリアルの数
	this->m_pTexture = nullptr;   //テクスチャ
	D3DXMatrixIdentity(&this->m_mtxWorld);   //ワールドマトリックス
	this->m_nIdParent = -1;   //親
	this->m_pos = { 0.0f, 0.0f, 0.0f };   //位置
	this->m_rot = { 0.0f, 0.0f, 0.0f };   //向き
}

//========================================================================================================
//デストラクタ
//========================================================================================================
Parts::~Parts()
{
}

//========================================================================================================
//初期化処理
//========================================================================================================
void Parts::Init()
{
}

//========================================================================================================
//終了処理
//========================================================================================================
void Parts::Uninit()
{
}

//========================================================================================================
//更新処理
//========================================================================================================
void Parts::Update()
{
}

//========================================================================================================
//描画処理
//========================================================================================================
void Parts::Draw(D3DXMATRIX mtxWorldParent)
{
	D3DXMATRIX mtxRot, mtxTrans;   //計算用マトリックス
	D3DMATERIAL9 matDef;   //現在のマテリアル保存用
	D3DXMATERIAL* pMat;   //マテリアルデータへのポインタ

	//デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = Manager::Get()->GetRenderer()->GetDevice();   //デバイスへのポインタ

	//ワールドマトリックスを初期化する
	D3DXMatrixIdentity(&this->m_mtxWorld);

	//向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, this->m_rot.y, this->m_rot.x, this->m_rot.z);
	D3DXMatrixMultiply(&this->m_mtxWorld, &this->m_mtxWorld, &mtxRot);

	//位置を反映する
	D3DXMatrixTranslation(&mtxTrans, this->m_pos.x, this->m_pos.y, this->m_pos.z);
	D3DXMatrixMultiply(&this->m_mtxWorld, &this->m_mtxWorld, &mtxTrans);

	//相対値位置を反映する
	D3DXMatrixMultiply(&this->m_mtxWorld, &this->m_mtxWorld, &mtxWorldParent);

	//ワールドマトリックスを設定する
	pDevice->SetTransform(D3DTS_WORLD, &this->m_mtxWorld);

	//現在のマテリアルを取得する
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得する
	pMat = (D3DXMATERIAL*)this->m_mesh.pBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)this->m_mesh.dwMat; i++)
	{
		//マテリアルを設定する
		pDevice->SetMaterial(&pMat[i].MatD3D);

		//テクスチャを設定する
		pDevice->SetTexture(0, this->m_pTexture);

		//パーツの描画
		this->m_mesh.pMesh->DrawSubset(i);
	}
}