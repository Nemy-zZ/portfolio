//========================================================================================================
//
//モデル処理[model.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "model.h"
#include "manager.h"
#include "object.h"
#include "transform.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void Model::Init()
{
	//パーツの数分繰り返す
	for (unsigned int i = 0; i < this->m_vParts.size(); i++)
	{
		//パーツの初期化処理を行う
		this->m_vParts[i]->Init();
	}
}

//========================================================================================================
//終了処理
//========================================================================================================
void Model::Uninit()
{
	//パーツの数分繰り返す
	for (unsigned int i = 0; i < this->m_vParts.size(); i++)
	{
		//パーツの終了処理を行う
		this->m_vParts[i]->Uninit();
		delete this->m_vParts[i];
	}

	//パーツを削除する
	this->m_vParts.clear();
}

//========================================================================================================
//更新処理
//========================================================================================================
void Model::Update()
{
	//パーツの数分繰り返す
	for (unsigned int i = 0; i < this->m_vParts.size(); i++)
	{
		//パーツの更新処理を行う
		this->m_vParts[i]->Update();
	}
}

//========================================================================================================
//描画処理
//========================================================================================================
void Model::Draw()
{
	//パーツの数分繰り返す
	for (unsigned int i = 0; i < this->m_vParts.size(); i++)
	{
		D3DXMATRIX mtxWorld;   //ワールドマトリックス

		//親の番号を取得する
		int nIdParent = this->m_vParts[i]->GetIdParent();
		if (nIdParent == -1)
		{//親がいない場合
			//ワールドマトリックスを取得する
			mtxWorld = this->GetObj()->GetComponent<Transform>()->GetMtxWorld();
		}
		else
		{//親がいる場合
			//親の情報を取得する
			mtxWorld = this->m_vParts[nIdParent]->GetMtxWorld();
		}

		//パーツの描画処理を行う
		this->m_vParts[i]->Draw(mtxWorld);
	}
}

//========================================================================================================
//Xファイルの設定処理
//========================================================================================================
void Model::SetXFile(XFile::Mesh mesh)
{
	//パーツを生成する
	Parts* pParts = new Parts;
	pParts->SetXFile(mesh);

	//パーツを保存する
	this->m_vParts.push_back(pParts);
}

//========================================================================================================
//パーツの読み込み処理
//========================================================================================================
void Model::LoadParts(std::string sFileName)
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
		if (sLine.find("MODEL_FILENAME = data/MODEL/") != std::string::npos)
		{//モデルのパスの場合
			//モデルのファイル名を保存する
			char aNameModel[128];
			sscanf(sLine.c_str(), "MODEL_FILENAME = data/MODEL/%s", &aNameModel);
			std::string sNameModel = aNameModel;

			//パーツを生成する
			Parts* pParts = new Parts;
			pParts->SetXFile(Manager::Get()->GetXFile()->Get(sNameModel));

			//パーツを保存する
			this->m_vParts.push_back(pParts);
		}
		else if (sLine.find("CHARACTERSET") != std::string::npos)
		{//キャラクター設定の場合
			//ファイルを読み込む
			int nCntParts = 0;   //読み込んだパーツの数
			while (std::getline(file, sLine))
			{
				if (sLine.find("END_CHARACTERSET") != std::string::npos)
				{//キャラクター設定の終わりの場合
					//ループを抜け出す
					break;
				}
				if (sLine.find("PARTSSET") != std::string::npos)
				{//パーツの設定の場合
					//パーツ設定の終わりまで読み込む
					while (std::getline(file, sLine))
					{
						//読み込んだ行を確認する
						if (sLine.find("END_PARTSSET") != std::string::npos)
						{//パーツ設定の終わりの場合
							//ループを抜け出す
							break;
						}
						else if (sLine.find("PARENT = ") != std::string::npos)
						{//親の番号の場合
							//親の番号を設定する
							std::string sIdParent = sLine.substr(sLine.find("PARENT = "));
							int nIdParent = -1;
							sscanf(sIdParent.c_str(), "PARENT = %d", &nIdParent);
							this->m_vParts[nCntParts]->SetIdParent(nIdParent);
						}
						else if (sLine.find("POS = ") != std::string::npos)
						{//位置の場合
							//位置を設定する
							std::string sPos = sLine.substr(sLine.find("POS = "));
							D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f };
							sscanf(sPos.c_str(), "POS = %f %f %f", &pos.x, &pos.y, &pos.z);
							this->m_vParts[nCntParts]->SetPos(pos);
						}
						else if (sLine.find("ROT = ") != std::string::npos)
						{//向きの場合
							//向きを設定する
							std::string sRos = sLine.substr(sLine.find("ROT = "));
							D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f };
							sscanf(sRos.c_str(), "ROT = %f %f %f", &rot.x, &rot.y, &rot.z);
							this->m_vParts[nCntParts]->SetRot(rot);
						}
					}

					//番号を設定する
					this->m_vParts[nCntParts]->SetId(nCntParts);
					nCntParts++;   //カウンターを進める
				}
			}
		}
	}

	//パーツの数分繰り返す
	for (unsigned int i = 0; i < this->m_vParts.size(); i++)
	{
		//パーツの初期化処理を行う
		this->m_vParts[i]->Init();
	}
}

//========================================================================================================
//テクスチャの設定処理
//========================================================================================================
void Model::SetTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//パーツの数分繰り返す
	for (unsigned int i = 0; i < this->m_vParts.size(); i++)
	{
		//テクスチャを設定する
		this->m_vParts[i]->SetTexture(pTexture);
	}
}