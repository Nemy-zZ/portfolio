//========================================================================================================
//
//ステンシルの処理[stencil.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "stencil.h"
#include "manager.h"
#include "rendering.h"
#include "object.h"

//========================================================================================================
//描画
//========================================================================================================
void Stencil::Draw()
{
	//デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = Manager::Get()->GetRenderer()->GetDevice();

	//ステンシルバッファを有効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, this->m_nRef);   //比較する参照値設定

	//ステンシルバッファの値に対してのマスク設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	//ステンシルテストの比較方法設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, this->m_compare);

	//ステンシルテストの結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, this->m_pass);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, this->m_pass);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, Stencil::PASS::IS_KEEP);

	//親クラスの処理を行う
	this->GetObj()->GetComponent<Rendering>()->Draw();

	//ステンシルバッファを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}