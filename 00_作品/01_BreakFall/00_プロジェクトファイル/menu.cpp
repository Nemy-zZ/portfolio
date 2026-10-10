//========================================================================================================
//
//メニューの処理[menu.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "menu.h"
#include "manager.h"
#include "joypad.h"
#include "keyboard.h"
#include "object.h"
#include "transform.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void Menu::Init()
{
	//選択肢の数分繰り返す
	for (unsigned int i = 0; i < this->m_vSelectBox.size(); i++)
	{
		//選択中の選択ボックスをリセットする
		this->m_vSelectBox[i]->Reset();
	}

	//決定フラグを下ろす
	this->m_isDecide = false;

	//選択を0に
	this->m_nSelect = 0;
}

//========================================================================================================
//終了処理
//========================================================================================================
void Menu::Uninit()
{
	//選択肢の数分繰り返す
	for (unsigned int i = 0; i < this->m_vSelectBox.size(); i++)
	{
		//終了処理を行う
		this->m_vSelectBox[i]->Uninit();
		delete this->m_vSelectBox[i];
	}

	//選択肢を削除する
	this->m_vSelectBox.clear();
}

//========================================================================================================
//更新処理
//========================================================================================================
void Menu::Update()
{
	//操作処理を行う
	this->Control();

	//選択肢の数分繰り返す
	for (unsigned int i = 0; i < this->m_vSelectBox.size(); i++)
	{
		//選択肢の更新処理を行う
		this->m_vSelectBox[i]->Update();
	}
}

//========================================================================================================
//描画処理
//========================================================================================================
void Menu::Draw()
{
	//デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = Manager::Get()->GetRenderer()->GetDevice();   //デバイスへのポインタ

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//選択肢の数分繰り返す
	for (unsigned int i = 0; i < this->m_vSelectBox.size(); i++)
	{
		//選択肢の描画処理を行う
		this->m_vSelectBox[i]->Draw(this->GetTexture());
	}

	//アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//========================================================================================================
//選択肢の数の設定処理
//========================================================================================================
void Menu::SetSelectBox(int nNum)
{
	//設定する個数分繰り返す
	for (int i = 0; i < nNum; i++)
	{
		//選択肢を生成する
		SelectBox* pSelectBox = new SelectBox;

		//情報を設定する
		pSelectBox->SetSize(this->GetObj()->GetComponent<Transform2D>()->GetSize());
		pSelectBox->SetPos({ this->GetObj()->GetComponent<Transform2D>()->GetPos().x, this->GetObj()->GetComponent<Transform2D>()->GetPos().y + ((this->GetObj()->GetComponent<Transform2D>()->GetSize().y + this->m_fGap) * i) });   //位置
		pSelectBox->SetPattern(nNum);
		pSelectBox->SetOder(i);

		//初期化処理を行う
		pSelectBox->Init();

		//選択肢の最後尾に情報を保存する
		this->m_vSelectBox.push_back(pSelectBox);
	}
}

//========================================================================================================
//操作処理
//========================================================================================================
void Menu::Control()
{
	//----------------------------------------------------------------------------------------------------
	//入力処理を行う
	//----------------------------------------------------------------------------------------------------
	{
		Keyboard* pKeyboard = Manager::Get()->GetKeyboard();
		Joypad* pJoypad = Manager::Get()->GetJoypad();

		//Wキーを確認する
		if (pKeyboard->GetTrigger(DIK_W) || pJoypad->GetTrigger(Joypad::JOYKEY::IS_UP, 0))
		{//Wキーが押されている場合
			//次の選択肢に移動する
			this->m_nSelect--;
			if (this->m_nSelect < 0)
			{//0より小さい場合
				//選択を一周する
				this->m_nSelect = this->m_vSelectBox.size() - 1;
			}
		}

		//Sキーを確認する
		if (pKeyboard->GetTrigger(DIK_S) || pJoypad->GetTrigger(Joypad::JOYKEY::IS_DOWN, 0))
		{//Sキーが押されている場合
			//次の選択肢に移動する
			this->m_nSelect++;
			if (this->m_nSelect >= (int)this->m_vSelectBox.size())
			{//選択肢を超えた場合
				//選択を一周する
				this->m_nSelect = 0;
			}
		}

		//選択フラグを立てる
		this->m_vSelectBox[this->m_nSelect]->Select();

		//SPACEキーを確認する
		if (pKeyboard->GetTrigger(DIK_SPACE) || pJoypad->GetTrigger(Joypad::JOYKEY::IS_A, 0))
		{//SPACEキーが押されている場合
			//決定フラグを立てる
			this->m_isDecide = true;
		}
	}
}