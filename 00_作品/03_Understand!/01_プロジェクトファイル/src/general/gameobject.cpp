//------------------------------------------------------------
// @file		gameobject.cpp
// @brief	ゲームオブジェクト
// @detail	コンポーネントの情報を持ったオブジェクトです
//------------------------------------------------------------
#include "gameobject.h"
#include "component.h"
#include "components/3d/model.h"
#include "components/3d/motion.h"
#include "components/3d/culling.h"
#include "components/3d/lod.h"
#include <string>
#include <fstream>

using namespace std;
#include <vector>

// 静的メンバ変数の初期化
std::list<GameObject*> GameObject::m_gameObjects;

//=============================================================
// コンストラクタ
//=============================================================
GameObject::GameObject(std::string vName, std::string vTag, int priority)
{
	// 基本情報
	this->name = vName;
	this->tag = vTag;
	this->m_priority = priority;

	// トランスフォームを作成する
	transform = new Transform(this);

	// 優先順位で挿入する
	int idx = static_cast<int>(m_gameObjects.size());
	for (auto itr = m_gameObjects.rbegin(); itr != m_gameObjects.rend(); itr++, idx--)
	{
		if ((*itr)->m_priority <= priority)
		{
			m_gameObjects.insert(std::next(m_gameObjects.begin(), idx), this);
			return;
		}
	}

	m_gameObjects.push_back(this);
}

//=============================================================
// デストラクタ
//=============================================================
GameObject::~GameObject()
{
	// リストから削除する
	for (auto itr = m_gameObjects.begin(); itr != m_gameObjects.end(); itr++)
	{
		if (*itr == this)
		{
			m_gameObjects.erase(itr);
			break;
		}
	}

	// トランスフォームを削除する
	if (transform != nullptr)
	{
		delete transform;
		transform = nullptr;
	}
}

//=============================================================
// 初期化
//=============================================================
void GameObject::Init()
{
	for (auto itr = m_components.begin(); itr != m_components.end(); itr++)
	{
		(*itr)->Init();
	}
}

//=============================================================
// 終了
//=============================================================
void GameObject::Uninit()
{
	for (auto itr = m_components.begin(); itr != m_components.end(); itr++)
	{
		(*itr)->Uninit();
	}
}

//=============================================================
// 更新
//=============================================================
void GameObject::Update()
{
	std::vector<Component*> queue(m_components.size());
	std::copy(m_components.begin(), m_components.end(), queue.begin());
	for (auto itr = queue.begin(); itr != queue.end(); itr++)
	{
		if ((*itr)->GetActive())
		{
			(*itr)->Update();
		}
	}
	queue.clear();
}

//=============================================================
// 描画
//=============================================================
void GameObject::Draw()
{
	for (auto itr = m_components.begin(); itr != m_components.end(); itr++)
	{
		if ((*itr)->GetActive())
		{
			(*itr)->Draw();
		}
	}
}

//=============================================================
// 描画UI
//=============================================================
void GameObject::DrawUI()
{
	for (auto itr = m_components.begin(); itr != m_components.end(); itr++)
	{
		if ((*itr)->GetActive())
		{
			(*itr)->DrawUI();
		}
	}
}

//=============================================================
// すべてのオブジェクトを更新する
//=============================================================
void GameObject::AllUpdate()
{
	for (auto itr = m_gameObjects.begin(); itr != m_gameObjects.end(); itr++)
	{
		if ((*itr)->GetActive())
		{
			(*itr)->Update();
		}
	}
}

//=============================================================
// すべてのオブジェクトを描画する
//=============================================================
void GameObject::AllDraw()
{
	for (auto itr = m_gameObjects.begin(); itr != m_gameObjects.end(); itr++)
	{
		if ((*itr)->GetActive())
		{
			CullingData* cull = (*itr)->GetComponent<CullingData>();
			if (cull != nullptr)
			{
				if (!cull->IsDraw())
				{
					continue;
				}
			}

			Shader* shader = (*itr)->GetComponent<Shader>();
			if (shader != nullptr || (shader == nullptr && Shader::GetRenderLayer() == Shader::RenderLayer::Opaque))
			{
				(*itr)->Draw();
			}
		}
	}
}

//=============================================================
// すべてのオブジェクトをUI描画する
//=============================================================
void GameObject::AllDrawUI()
{
	for (auto itr = m_gameObjects.begin(); itr != m_gameObjects.end(); itr++)
	{
		if ((*itr)->GetActive())
		{
			(*itr)->DrawUI();
		}
	}
}

//=============================================================
// 名前からゲームオブジェクトを取得する
//=============================================================
GameObject* GameObject::FindByName(const std::string& name)
{
	for (auto itr = m_gameObjects.begin(); itr != m_gameObjects.end(); itr++)
	{
		if ((*itr)->name == name)
		{
			return *itr;
		}
	}
	return nullptr;
}

//=============================================================
// タグからゲームオブジェクトを取得する
//=============================================================
GameObject* GameObject::FindByTag(const std::string& tag)
{
	for (auto itr = m_gameObjects.begin(); itr != m_gameObjects.end(); itr++)
	{
		if ((*itr)->tag == tag)
		{
			return *itr;
		}
	}
	return nullptr;
}

//=============================================================
// 名前とタグからゲームオブジェクトを取得する
//=============================================================
GameObject* GameObject::FindByNameTag(const std::string& name, const std::string& tag)
{
	for (auto itr = m_gameObjects.begin(); itr != m_gameObjects.end(); itr++)
	{
		if ((*itr)->name == name && (*itr)->tag == tag)
		{
			return *itr;
		}
	}
	return nullptr;
}

//=============================================================
// 親を設定する
//=============================================================
void GameObject::SetParent(GameObject* gameObject)
{
	if (gameObject != nullptr)
	{
		transform->SetParent(gameObject->transform);
	}
}

//=============================================================
// 親を取得する
//=============================================================
GameObject* GameObject::GetParent()
{
	if (this->transform->GetParent() != nullptr)
	{
		return this->transform->GetParent()->GetAttachObject();
	}
	return nullptr;
}

//=============================================================
// 優先順位を設定する
//=============================================================
void GameObject::SetPriority(const int& priority)
{
	if (m_priority != priority)
	{
		m_priority = priority;

		// 優先順位で移動する
		int idx = static_cast<int>(m_gameObjects.size());
		for (auto itr = m_gameObjects.rbegin(); itr != m_gameObjects.rend(); itr++, idx--)
		{
			if ((*itr)->m_priority <= priority)
			{
				m_gameObjects.splice(std::next(m_gameObjects.begin(), idx), m_gameObjects, std::find(m_gameObjects.begin(), m_gameObjects.end(), this));
				return;
			}
		}
	}
}

//=============================================================
// アクティブ状態を取得する
//=============================================================
bool GameObject::GetActive()
{
	if (!Object::GetActive())
	{
		return false;
	}

	if (transform->GetParent() != nullptr)
	{
		return transform->GetParent()->GetAttachObject()->GetActive();
	}
	return Object::GetActive();
}

//=============================================================
// コンポーネントをデタッチする
//=============================================================
void GameObject::DetachComponent(Component* component)
{
	for (auto itr = m_components.begin(); itr != m_components.end(); itr++)
	{
		if (*itr == component)
		{
			(*itr)->gameObject = nullptr;
			(*itr)->transform = nullptr;
			m_components.erase(itr);
			break;
		}
	}
}

//=============================================================
// 子オブジェクトを名前検索で取得する
//=============================================================
GameObject* GameObject::FindChildrenByName(const std::string& name)
{
	auto objects = this->GetUnityObjects(false);
	for (auto itr = objects.begin(); itr != objects.end(); itr++)
	{
		if ((*itr)->name == name)
		{
			return *itr;
		}
	}
	return nullptr;
}

//=============================================================
// 子オブジェクトを取得する
//=============================================================
std::vector<GameObject*> GameObject::GetChildren()
{
	std::vector<GameObject*> result;

	for (auto itr = m_gameObjects.begin(); itr != m_gameObjects.end(); itr++)
	{
		if ((*itr)->transform->GetParent() == this->transform)
		{ // 親に設定されているとき
			result.push_back(*itr);
		}
	}

	return result;
}

//=============================================================
// 子オブジェクトを含めたリストを取得する
//=============================================================
std::vector<GameObject*> GameObject::GetUnityObjects(const bool& includeParent)
{
	std::vector<GameObject*> result;

	if (includeParent)
	{
		result.push_back(this);
	}

	// 子オブジェクトを取得する
	std::vector<GameObject*> children = this->GetChildren();
	std::vector<GameObject*> queue;

	while (true)
	{
		for (auto itr = children.begin(); itr != children.end(); itr++)
		{
			// 結果に追加
			result.push_back(*itr);

			// 子をキューに入れる
			std::vector<GameObject*> nextChildren = (*itr)->GetChildren();
			for (auto next = nextChildren.begin(); next != nextChildren.end(); next++)
			{
				queue.push_back(*next);
			}
		}
		children.clear();

		// キュー
		if (!queue.empty())
		{
			result.push_back(queue[0]);
			children = queue[0]->GetChildren();
			queue.erase(queue.begin());
		}
		else
		{
			break;
		}
	}

	return result;
}

//=============================================================
// Obuseファイルからオブジェクトを作成する
//=============================================================
GameObject* GameObject::CreateObuseObject(const std::string& path, const Transform& transform)
{
	// jsonファイルを読み込む
	std::ifstream ifs(path.c_str());
	if (ifs.fail())
	{
		Log::SendLog("ファイルが開けませんでした", Log::TYPE_ERROR);
		return nullptr;
	}

	// json形式に変換
	std::string inputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto j = json::parse(inputData);

	// オブジェクト情報が含まれていないときはエラー
	if (!j.contains("objects") || j["objects"].size() <= 0)
	{
		Log::SendLog("オブジェクト情報が取得できませんでした", Log::TYPE_ERROR);
		return nullptr;
	}

	// プレハブオブジェクトを生成する
	GameObject* prefab = new GameObject();

	// モーションが含まれているとき
	bool isContainsMotion = false;
	if (j.contains("motions"))
	{
		prefab->AddComponent<Motion>();
		isContainsMotion = true;
	}

	// LOD
	bool lodEnabled = false;
	float lodRatio[LOD::LEVEL::MAX] = { 0.5f, 0.3f, 0.15f, 0.05f };

	if (j.contains("lod"))
	{
		if (j["lod"].contains("enabled"))
		{
			lodEnabled = j["lod"]["enabled"];
		}

		if (lodEnabled)
		{
			if (j["lod"].contains("ratio"))
			{
				if (j["lod"]["ratio"].contains("level_0"))
				{
					lodRatio[LOD::LEVEL_0] = j["lod"]["ratio"]["level_0"];
				}

				if (j["lod"]["ratio"].contains("level_1"))
				{
					lodRatio[LOD::LEVEL_1] = j["lod"]["ratio"]["level_1"];
				}

				if (j["lod"]["ratio"].contains("level_2"))
				{
					lodRatio[LOD::LEVEL_2] = j["lod"]["ratio"]["level_2"];
				}

				if (j["lod"]["ratio"].contains("culled"))
				{
					lodRatio[LOD::CULLED] = j["lod"]["ratio"]["culled"];
				}
			}
		}
	}
	
	// オブジェクト格納変数をつくる
	std::vector<GameObject*> objects(j["objects"].size() - 1);

	// オブジェクト情報を読み込む
	for (auto itr = j["objects"].items().begin(); itr != j["objects"].items().end(); itr++)
	{
		std::string id = (*itr).key();

		// ゲームオブジェクトを生成する
		GameObject* target = nullptr;
		if (id == "prefab")
		{
			target = prefab;
		}
		else
		{
			target = new GameObject();

			if (isContainsMotion)
			{
				prefab->GetComponent<Motion>()->AddParts(target);
			}
		}

		// 基本情報
		if (j["objects"][id].contains("name"))
		{
			target->name = j["objects"][id]["name"];
		}
		if (j["objects"][id].contains("tag"))
		{
			target->tag = j["objects"][id]["tag"];
		}

		// トランスフォーム情報
		if (j["objects"][id].contains("transform"))
		{
			// 位置
			if (j["objects"][id]["transform"].contains("position"))
			{
				target->transform->position = {
					j["objects"][id]["transform"]["position"][0],
					j["objects"][id]["transform"]["position"][1],
					j["objects"][id]["transform"]["position"][2]
				};
			}

			// 回転
			if (j["objects"][id]["transform"].contains("rotation"))
			{
				target->transform->rotation = {
					j["objects"][id]["transform"]["rotation"][0],
					j["objects"][id]["transform"]["rotation"][1],
					j["objects"][id]["transform"]["rotation"][2],
					j["objects"][id]["transform"]["rotation"][3]
				};
			}

			// スケール
			if (j["objects"][id]["transform"].contains("scale"))
			{
				target->transform->scale = {
					j["objects"][id]["transform"]["scale"][0],
					j["objects"][id]["transform"]["scale"][1],
					j["objects"][id]["transform"]["scale"][2]
				};
			}
		}

		// プレハブのみトランスフォームを適用する
		if (id == "prefab")
		{
			prefab->transform->position += transform.position;
			prefab->transform->rotation *= transform.rotation;
			prefab->transform->scale.x *= transform.scale.x;
			prefab->transform->scale.y *= transform.scale.y;
			prefab->transform->scale.z *= transform.scale.z;
		}

		// モデル情報
		if (j["objects"][id].contains("model"))
		{
			target->AddComponent<Model>()->Load(j["objects"][id]["model"]);
			target->AddComponent<CullingData>();

			if (lodEnabled)
			{
				// 固定された読み込み・保存パス
				std::string inputDir = "data\\MODEL";
				std::string outputDir = "data\\TEMP\\LOD";
				std::string relativePath = std::filesystem::relative(target->GetComponent<Model>()->GetModelData()->GetPath(), inputDir).string();
				std::replace(relativePath.begin(), relativePath.end(), '\\', '/');
				std::string baseName = outputDir + "\\" + relativePath.substr(0, relativePath.find_last_of('.'));

				// LODを割り当てる
				std::string level1 = baseName + "_Level1.x";
				std::string level2 = baseName + "_Level2.x";

				target->AddComponent<LOD>();
				target->GetComponent<LOD>()->SetLOD(LOD::LEVEL_0, lodRatio[LOD::LEVEL_0], target->GetComponent<Model>()->GetModelData());
				target->GetComponent<LOD>()->SetLOD(LOD::LEVEL_1, lodRatio[LOD::LEVEL_1], Manager::GetInstance()->GetResourceDataManager()->RefModel(level1));
				target->GetComponent<LOD>()->SetLOD(LOD::LEVEL_2, lodRatio[LOD::LEVEL_2], Manager::GetInstance()->GetResourceDataManager()->RefModel(level2));
				target->GetComponent<LOD>()->SetLOD(LOD::CULLED, lodRatio[LOD::CULLED], nullptr);
			}
		}

		// コライダー情報
		if (j["objects"][id].contains("collider"))
		{
			for (auto cols = j["objects"][id]["collider"].begin(); cols != j["objects"][id]["collider"].end(); cols++)
			{
				// タイプ
				if (!(*cols).contains("type")) continue;
				std::string colType = (*cols)["type"];

				// 基本設定の関数（オフセットと回転）
				auto defaultCollider = [cols](Collider* col) {
					if ((*cols).contains("offset"))
					{
						col->SetOffset({ (*cols)["offset"][0], (*cols)["offset"][1], (*cols)["offset"][2] });
					}
					if ((*cols).contains("rotation"))
					{
						col->SetRotation({ (*cols)["rotation"][0], (*cols)["rotation"][1], (*cols)["rotation"][2], (*cols)["rotation"][3] });
					}
				};

				// タイプごとの処理
				if (colType == "box")
				{
					BoxCollider* col = target->AddComponent<BoxCollider>();
					defaultCollider(col);
					if ((*cols).contains("size"))
					{
						col->SetSize({ (*cols)["size"][0], (*cols)["size"][1], (*cols)["size"][2] });
					}
				}
				else if (colType == "sphere")
				{
					SphereCollider* col = target->AddComponent<SphereCollider>();
					defaultCollider(col);
					if ((*cols).contains("radius"))
					{
						col->SetRadius((*cols)["radius"]);
					}
				}
				else if (colType == "capsule")
				{
					CapsuleCollider* col = target->AddComponent<CapsuleCollider>();
					defaultCollider(col);
					if ((*cols).contains("radius"))
					{
						col->SetRadius((*cols)["radius"]);
					}
					if ((*cols).contains("height"))
					{
						col->SetHeight((*cols)["height"]);
					}
				}
				else if (colType == "cylinder")
				{
					CylinderCollider* col = target->AddComponent<CylinderCollider>();
					defaultCollider(col);
					if ((*cols).contains("radius"))
					{
						col->SetRadius((*cols)["radius"]);
					}
					if ((*cols).contains("height"))
					{
						col->SetHeight((*cols)["height"]);
					}
				}
				else if (colType == "cone")
				{
					ConeCollider* col = target->AddComponent<ConeCollider>();
					defaultCollider(col);
					if ((*cols).contains("radius"))
					{
						col->SetRadius((*cols)["radius"]);
					}
					if ((*cols).contains("height"))
					{
						col->SetHeight((*cols)["height"]);
					}
				}
			}
		}

		// コリジョン情報
		Collision* collision = target->GetComponent<Collision>();
		if (collision != nullptr && j["objects"][id].contains("collision"))
		{
			// 摩擦量
			if (j["objects"][id]["collision"].contains("friction"))
			{
				collision->SetFriction(j["objects"][id]["collision"]["friction"]);
			}

			// プレハブのとき
			if (id == "prefab" && j["objects"][id]["collision"].contains("rigidbody"))
			{
				// リジッドボディか
				if (j["objects"][id]["collision"]["rigidbody"])
				{
					prefab->AddComponent<RigidBody>();

					// 質量
					if (j["objects"][id]["collision"].contains("mass"))
					{
						prefab->GetComponent<RigidBody>()->SetMass(j["objects"][id]["collision"]["mass"]);
					}
				}
			}

			// トリガーか
			if (prefab->GetComponent<RigidBody>() == nullptr &&
				j["objects"][id]["collision"].contains("trigger"))
			{
				collision->SetTrigger(j["objects"][id]["collision"]["trigger"]);
			}
		}

		// オブジェクトを格納する
		if (id != "prefab")
		{
			objects[atoi(id.c_str())] = target;
		}
	}

	// 親子関係
	if (j.contains("parents"))
	{
		for (auto itr = j["parents"].items().begin(); itr != j["parents"].items().end(); itr++)
		{
			int target = atoi((*itr).key().c_str());
			int parent = (*itr).value();

			if (parent != -1)
			{ // 通常
				objects[target]->SetParent(objects[parent]);
			}
			else
			{ // プレハブが親のとき
				objects[target]->SetParent(prefab);
			}
		}
	}

	// モーション
	if (isContainsMotion)
	{
		for (auto itr = j["motions"].items().begin(); itr != j["motions"].items().end(); itr++)
		{
			std::string id = (*itr).key();
			MotionData* data = new MotionData(id);

			// ループ設定
			if (j["motions"][id].contains("loop"))
			{
				data->SetLoop(j["motions"][id]["loop"]);
			}

			// 最大フレーム数設定
			if (j["motions"][id].contains("max_frame"))
			{
				data->SetMaxFrame(j["motions"][id]["max_frame"]);
			}

			// キーを追加する
			for (int partsID = 0; partsID < (int)j["objects"].size() - 1; partsID++)
			{
				std::string idx = std::to_string(partsID);
				if (j["motions"][id]["keys"].contains(idx))
				{
					for (auto keys = j["motions"][id]["keys"][idx].begin(); keys != j["motions"][id]["keys"][idx].end(); keys++)
					{
						int frame = (*keys)["frame"];
						D3DXVECTOR3 position = objects[atoi(idx.c_str())]->transform->position;
						position += D3DXVECTOR3((*keys)["transform"]["position"][0], (*keys)["transform"]["position"][1], (*keys)["transform"]["position"][2]);

						D3DXQUATERNION rotation = { (*keys)["transform"]["rotation"][0], (*keys)["transform"]["rotation"][1], (*keys)["transform"]["rotation"][2], (*keys)["transform"]["rotation"][3] };
						D3DXQUATERNION rotation_offset = objects[atoi(idx.c_str())]->transform->rotation;
						D3DXQuaternionMultiply(&rotation, &rotation_offset, &rotation);

						D3DXVECTOR3 scale = { (*keys)["transform"]["scale"][0], (*keys)["transform"]["scale"][1], (*keys)["transform"]["scale"][2] };
						scale.x *= objects[atoi(idx.c_str())]->transform->scale.x;
						scale.y *= objects[atoi(idx.c_str())]->transform->scale.y;
						scale.z *= objects[atoi(idx.c_str())]->transform->scale.z;

						// キーを追加する
						data->AddKey(atoi(idx.c_str()), frame, position, rotation, scale);
					}
				}
				else
				{
					// キーを追加する
					data->AddKey(atoi(idx.c_str()), 1, objects[atoi(idx.c_str())]->transform->position, objects[atoi(idx.c_str())]->transform->rotation, objects[atoi(idx.c_str())]->transform->scale);
				}


			}

			// モーションを登録する
			prefab->GetComponent<Motion>()->AddMotionData(data);
		}
	}

	return prefab;
}

//=============================================================
// 解放する
//=============================================================
void GameObject::Release()
{
	// 親子関係のあるオブジェクトを破棄する
	for (auto itr = m_gameObjects.begin(); itr != m_gameObjects.end(); itr++)
	{
		if (this->transform == (*itr)->transform->GetParent())
		{
			Destroy(*itr);
		}
	}

	// コンポーネントを解放する
	int idx = static_cast<int>(m_components.size());
	for (int i = idx - 1; i >= 0; i--)
	{
		Destroy(m_components[i]);
		m_components[i]->DetachGameObject();
	}
}
