//======================================================================================================
//
//ディレクショナルライトの処理[dire_light.cpp]
// Auhtor Murata Shinnosuke
//
//======================================================================================================
#include "dire_light.h"
#include "manager.h"
#include "object.h"
#include "transform.h"

//静的メンバ変数を初期化する
int DireLight::m_nTotal = 0;

//==============================================================================
// ライトの初期化処理
//==============================================================================
void DireLight::Init()
{
	//デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = Manager::Get()->GetRenderer()->GetDevice();   //デバイスへのポインタ

	//情報を設定する
	this->m_light.Type = D3DLIGHT_DIRECTIONAL;   //種類
	this->m_light.Diffuse = this->m_color;   //拡散光

	//方向を設定する
	D3DXVECTOR3 vecDir = this->GetObj()->GetComponent<Transform>()->GetRot();   //方向の取得
	D3DXVec3Normalize(&vecDir, &vecDir);	//ベクトル正規化
	this->m_light.Direction = vecDir;

	//設定する
	pDevice->SetLight(this->m_nId, &this->m_light);

	//有効にする
	pDevice->LightEnable(this->m_nId, TRUE);
}