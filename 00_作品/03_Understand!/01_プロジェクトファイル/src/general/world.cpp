//------------------------------------------------------------
// @file			world.cpp
// @brief		ワールド
//------------------------------------------------------------
#include "world.h"
#include <fstream>
#include "components/3d/model.h"
#include "components/3d/culling.h"

//=============================================================
// ワールドを読み込む
//=============================================================
void LoadWorld(const std::string& path, const bool& isDestroy)
{
	// jsonファイルを読み込む
	std::ifstream ifs(path.c_str());
	if (ifs.fail())
	{
		Log::SendLog("PLCWファイルが開けませんでした", Log::TYPE_ERROR);
		return;
	}

	// 他オブジェクトの破棄
	if (isDestroy)
	{
		GameObject::AllDestroy();
	}

	// json形式に変換
	std::string inputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto j = json::parse(inputData);

	// トランスフォームを構築するラムダ関数
	auto transformStructure = [](json& data, Transform& trans) {
		trans.position = {
			data["transform"]["position"][0], data["transform"]["position"][1], data["transform"]["position"][2]
		};
		trans.rotation = {
			data["transform"]["rotation"][0], data["transform"]["rotation"][1], data["transform"]["rotation"][2], data["transform"]["rotation"][3]
		};
		trans.scale = {
			data["transform"]["scale"][0], data["transform"]["scale"][1], data["transform"]["scale"][2]
		};
	};

	// オブジェクト情報を取得する
	if (j.contains("x-objects"))
	{
		// パス
		for (auto pathItr = j["x-objects"].items().begin(); pathItr != j["x-objects"].items().end(); pathItr++)
		{
			std::string path = (*pathItr).key();

			// オブジェクト
			for (auto itr = j["x-objects"][path].begin(); itr != j["x-objects"][path].end(); itr++)
			{
				// トランスフォームを構築する
				Transform trans;
				transformStructure(*itr, trans);

				// 設置する
				GameObject* placeObj = new GameObject();
				*placeObj->transform = trans;
				placeObj->AddComponent<Model>()->Load(path);
				placeObj->name = (*itr)["name"];
				placeObj->tag = (*itr)["tag"];
			}
		}
	}

	// オブジェクト情報を取得する
	if (j.contains("prefab-objects"))
	{
		// パス
		for (auto pathItr = j["prefab-objects"].items().begin(); pathItr != j["prefab-objects"].items().end(); pathItr++)
		{
			std::string path = (*pathItr).key();

			// オブジェクト
			for (auto itr = j["prefab-objects"][path].begin(); itr != j["prefab-objects"][path].end(); itr++)
			{
				// トランスフォームを構築する
				Transform trans;
				transformStructure(*itr, trans);

				// 設置する
				GameObject* placeObj = GameObject::CreateObuseObject(path, trans);
				placeObj->AddComponent<CullingData>();
				placeObj->name = (*itr)["name"];
				placeObj->tag = (*itr)["tag"];
			}
		}
	}
}
