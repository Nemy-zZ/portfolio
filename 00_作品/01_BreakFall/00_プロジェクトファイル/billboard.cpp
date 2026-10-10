//========================================================================================================
//
//ビルボードの処理[billboard.cpp]
// Author MurataShinnosuke
//
//========================================================================================================
#include "plane.h"

//========================================================================================================
//描画処理
//========================================================================================================
void Billboard::Draw()
{
	D3DXMATRIX mtxTrans;   //計算用マトリックス
	D3DXMATRIX mtxView, mtxViewInverse;   //ビューマトリックス取得用

	//デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = Manager::GetRenderer()->GetDevice();   //デバイスへのポインタ

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Zバッファの書き込みを無効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (this->m_isMultiply == true)
	{//加算合成する場合
		//αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//カメラの逆行列を求める
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	D3DXMatrixInverse(&mtxViewInverse, NULL, &mtxView);

	//Y軸ビルボードか確認する
	if (m_isYAxis)
	{
		//Y軸回転情報の打ち消す
		mtxViewInverse._21 = 0.0f;
		mtxViewInverse._22 = 1.0f;
		mtxViewInverse._23 = 0.0f;
	}

	//移動情報の打ち消す
	mtxViewInverse._41 = 0.0f;
	mtxViewInverse._42 = 0.0f;
	mtxViewInverse._43 = 0.0f;

	//ワールドマトリックスを取得する
	D3DXMATRIX mtxWorld = this->GetMtxWorld();

	//逆行列を反映する
	D3DXMatrixMultiply(&mtxViewInverse, &mtxViewInverse, &mtxWorld);

	//ワールドマトリックスを設定する
	pDevice->SetTransform(D3DTS_WORLD, &mtxViewInverse);

	//頂点バッファをテータストリームに設定する
	pDevice->SetStreamSource(0, this->GetVtxBuffer(), 0, sizeof(VERTEX_3D));

	//頂点フォーマットを設定する
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャを設定する
	pDevice->SetTexture(0, this->GetTexture());   //設定

	//描画する
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Zバッファの書き込みを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}