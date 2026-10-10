//========================================================================================================
//
//モーショナーの処理[motioneer.cpp]
// Author MurataShinnosuke
//
//========================================================================================================
#include "motioneer.h"
#include "object.h"
#include "model.h"
#include "behavior.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void Motioneer::Init()
{
}

//========================================================================================================
//終了処理
//========================================================================================================
void Motioneer::Uninit()
{
	//モーションの数分繰り返す
	for (unsigned int i = 0; i < this->m_vMotion.size(); i++)
	{
		//モーションの終了処理を行う
		this->m_vMotion[i]->Uninit();
		delete this->m_vMotion[i];
	}

	//モーションを削除する
	this->m_vMotion.clear();
}

//========================================================================================================
//更新処理
//========================================================================================================
void Motioneer::Update()
{
	//モデルを確認する
	if (this->m_pModel == nullptr)
	{
		//モデルを取得する
		this->m_pModel = this->GetObj()->GetComponent<Model>();

		//モーションの数分繰り返す
		for (unsigned int i = 0; i < this->m_vMotion.size(); i++)
		{
			//モーションにパーツ数を設定する
			this->m_vMotion[i]->SetNumParts(this->m_pModel->GetNumParts());
		}
	}

	//現在モーションの更新処理を行う
	this->m_vMotion[this->m_nMotionPlay]->Update();
	if (this->m_vMotion[this->m_nMotionPlay]->GetEnd())
	{//モーションが終了した場合
		//モーションの終了時の処理を行う
		this->GetObj()->GetComponent<Behavior>()->EndMotion();
	}

	//パーツの数分繰り返す
	for (int i = 0; i < this->m_pModel->GetNumParts(); i++)
	{
		//モーションのキーの情報をパーツに設定する
		this->m_pModel->GetParts(i)->SetRot(this->m_vMotion[this->m_nMotionPlay]->GetRot(i));
	}
}

//=========================================================================================================
//モーションの読み込み処理
//=========================================================================================================
void Motioneer::LoadMotion(std::string sFileName)
{
	//パスにファイル名を足す
	std::string sPass = "data\\MOTION\\";
	sPass += sFileName;

	//読み込むファイルを指定する
	std::ifstream file(sPass);

	//ファイルを読み込む
	std::string sLine;
	while (std::getline(file, sLine))
	{
		//読み込んだ行を確認する
		if (sLine.find("MOTIONSET") != std::string::npos)
		{//モーションの設定の場合
			//モーションを生成する
			Motion* pMotion = new Motion;

			//ファイルを読み込む
			while (std::getline(file, sLine))
			{
				//読み込んだ行を確認する
				if (sLine.find("END_MOTIONSET") != std::string::npos)
				{//モーションの設定の終わりの場合
					//ループを抜け出す
					break;
				}
				else if (sLine.find("LOOP = ") != std::string::npos)
				{//ループの場合
					//ループの値を確認する
					std::string sLoop = sLine.substr(sLine.find("LOOP = "));
					int nLoop;
					sscanf(sLoop.c_str(), "LOOP = %d", &nLoop);
					if (nLoop == 1)
					{
						//ループに設定する
						pMotion->Loop();
					}
				}
				else if (sLine.find("KEYSET") != std::string::npos) 
				{//キーの設定
					Motion::Key key;   //読み込んだキー情報の保存用
					int nCntKey = 0;   //読み込んだキーの数

					//ファイルを読み込む
					while (std::getline(file, sLine))
					{
						//読み込んだ行を確認する
						if (sLine.find("END_KEYSET") != std::string::npos)
						{
							//ループを抜け出す
							break;
						}
						else if (sLine.find("FRAME = ") != std::string::npos)
						{//フレームの場合
							//フレームを読み込む
							std::string sFrame = sLine.substr(sLine.find("FRAME = "));
							int nFrame;
							sscanf(sFrame.c_str(), "FRAME = %d", &nFrame);
							key.nFrame = nFrame;
						}
						else if (sLine.find("KEY") != std::string::npos)
						{//キーの場合
							//次の行を読み込む
							std::getline(file, sLine);
							std::string sPos = sLine.substr(sLine.find("POS = "));
							D3DXVECTOR3 pos;
							sscanf(sPos.c_str(), "POS = %f %f %f", &pos.x, &pos.y, &pos.z);   //位置を保存
							key.vPos.push_back(pos);

							//次の行を読み込む
							std::getline(file, sLine);
							std::string sRot = sLine.substr(sLine.find("ROT = "));
							D3DXVECTOR3 rot;
							sscanf(sRot.c_str(), "ROT = %f %f %f", &rot.x, &rot.y, &rot.z);   //位置を保存
							key.vRot.push_back(rot);

							//次の行を読み込む
							std::getline(file, sLine);
						}
					}

					//キーを設定する
					pMotion->AddKey(key);
				}
			}

			//モーションを保存する
			this->m_vMotion.push_back(pMotion);
		}
	}

	//パーツの数分繰り返す
	for (unsigned int i = 0; i < this->m_vMotion.size(); i++)
	{
		//モーションの初期化処理を行う
		this->m_vMotion[i]->Init();
	}
}