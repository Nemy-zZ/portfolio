//========================================================================================================
//
//数字の処理[number.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "number.h"
#include "manager.h"
#include "object.h"
#include "transform.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void Number::Init()
{
}

//========================================================================================================
//終了処理
//========================================================================================================
void Number::Uninit()
{
	//桁数の分繰り返す
	for (unsigned int i = 0; i < this->m_vDigid.size(); i++)
	{
		//桁の終了処理を行う
		this->m_vDigid[i]->Uninit();
		delete this->m_vDigid[i];
	}

	//桁をクリアする
	this->m_vDigid.clear();
}

//========================================================================================================
//更新処理
//========================================================================================================
void Number::Update()
{
	//桁数の分繰り返す
	for (unsigned int i = 0; i < this->m_vDigid.size(); i++)
	{
		//桁の更新処理を行う
		this->m_vDigid[i]->Update();
	}
}

//========================================================================================================
//描画処理
//========================================================================================================
void Number::Draw()
{
	//デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = Manager::Get()->GetRenderer()->GetDevice();   //デバイスへのポインタ

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//桁数の分繰り返す
	for (unsigned int i = 0; i < this->m_vDigid.size(); i++)
	{
		//桁の描画処理を行う
		this->m_vDigid[i]->Draw(this->GetTexture());
	}
	//アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//========================================================================================================
//テクスチャの設定処理
//========================================================================================================
void Number::SetNumber(int nNumber)
{
	//数字を設定する
	this->m_nNumber = nNumber;
	int nKeepNumber = this->m_nNumber;   //数値の保持用

	//桁数の分繰り返す
	for (unsigned int i = 0; i < this->m_vDigid.size(); i++) 
	{
		//桁に数字を設定する
		int nBara = nKeepNumber % 10;
		nKeepNumber /= 10;
		this->m_vDigid[this->m_vDigid.size() - (i + 1)]->SetValue(nBara);
	}
}

//========================================================================================================
//桁数の設定処理
//========================================================================================================
void Number::SetDigit(int nDigit)
{
	//桁サイズを計算する
	D3DXVECTOR2 sizeDigit = this->GetObj()->GetComponent<Transform2D>()->GetSize();
	sizeDigit.x /= nDigit;

	for (int i = 0; i < nDigit; i++)
	{
		//桁を生成する
		Digit* pDigid = new Digit;
		pDigid->SetSize(sizeDigit);   //サイズ
		pDigid->SetPos({ this->GetObj()->GetComponent<Transform2D>()->GetPos().x + (sizeDigit.x * i), this->GetObj()->GetComponent<Transform2D>()->GetPos().y });   //位置
		pDigid->Init();

		//桁の最後尾に保存する
		this->m_vDigid.push_back(pDigid);
	}
}