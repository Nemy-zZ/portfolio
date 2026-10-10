//------------------------------------------------------------
// @file		shader.cpp
// @brief	シェーダー
// @detail	シェーダーの命令を行います
//------------------------------------------------------------
#include "shader.h"
#include "components/3d/camera.h"

Shader::RenderLayer Shader::s_renderLayer = (Shader::RenderLayer)0;

//=============================================================
// シェーダーを追加する
//=============================================================
ShaderData* Shader::AddShader(const std::string& path, const std::string& cameraTag, const RenderLayer& layer, const std::vector<int> pass)
{
	// パスを指定しなかったらエラー
	if (pass.empty())
	{
		Log::SendLog("シェーダーを追加する際、最低でも1つ以上 pass を設定する必要があります", Log::TYPE_ERROR);
		return nullptr;
	}

	// シェーダーデータを取得する
	ShaderData* shaderData = Manager::GetInstance()->GetResourceDataManager()->RefShader(path);
	if (path != "" && shaderData == nullptr) return nullptr;

	// パスを正規化する
	std::vector<int> normalPass;
	auto passItr = std::find(pass.begin(), pass.end(), -1);
	if (passItr != pass.end())
	{
		normalPass.push_back(-1);
	}
	else
	{
		normalPass = std::move(pass);
	}

	// シェーダー情報を作成する
	ShaderInfo info;
	info.shader = shaderData;
	info.pass = normalPass;

	// シェーダーリストに加える
	m_shaders[layer][cameraTag].push_back(info);

	// シェーダーデータを返す
	return shaderData;
}

//=============================================================
// シェーダーを削除する
//=============================================================
void Shader::RemoveShader(const RenderLayer& layer, const int& idx, const std::string& cameraTag)
{
	if (m_shaders[layer][cameraTag].size() > idx)
	{
		m_shaders[layer][cameraTag].erase(m_shaders[layer][cameraTag].begin() + idx);
	}
}

//=============================================================
// シェーダーをクリアする
//=============================================================
void Shader::ClearShader()
{
	for (int i = 0; i < RenderLayer::MAX; i++)
	{
		m_shaders[i].clear();
	}
}

//=============================================================
// オブジェクト全体にシェーダーを適用する
//=============================================================
void Shader::ApplyShaderToObjects(GameObject* objects, const std::string& path, const std::string& cameraTag, const RenderLayer& layer, const std::vector<int> pass)
{
	auto unity = objects->GetUnityObjects();
	for (auto obj : unity)
	{
		Shader* shader = obj->GetComponent<Shader>();
		if (shader != nullptr)
		{
			shader->AddShader(path, cameraTag, layer, pass);
		}
	}
}

//=============================================================
// 描画命令があるか
//=============================================================
bool Shader::IsDrawCall()
{
	// 現在のカメラを取得する
	Camera* currentCamera = Camera::GetCurrentCamera();
	if (currentCamera == nullptr) return false;

	// カメラタグ
	std::string cameraTag = "";
	if (m_shaders[s_renderLayer].count(currentCamera->gameObject->tag))
	{
		cameraTag = currentCamera->gameObject->tag;
	}

	// インデックスがシェーダーリストの範囲外かを判断する
	if (m_shaders[s_renderLayer][cameraTag].empty())
	{
		return false;
	}

	return true;
}

//=============================================================
// セット
//=============================================================
void Shader::Set()
{
	// 初期化
	m_beganShader = nullptr;
	m_shaderCounter = 0;
	m_passNum = 0;
	m_passCounter = 0;

	// 現在のカメラを取得する
	Camera* currentCamera = Camera::GetCurrentCamera();
	if (currentCamera == nullptr) return;

	// ビルトイン変数を設定する
	UpdateVariable(currentCamera);
}

//=============================================================
// 描画開始
//=============================================================
bool Shader::Begin()
{
	// 現在のカメラを取得する
	Camera* currentCamera = Camera::GetCurrentCamera();
	if (currentCamera == nullptr)
	{
		return false;
	}
	
	// カメラタグ
	std::string cameraTag = "";
	if (m_shaders[s_renderLayer].count(currentCamera->gameObject->tag))
	{
		cameraTag = currentCamera->gameObject->tag;
	}
	
	// インデックスがシェーダーリストの範囲外かを判断する
	if (m_shaderCounter >= static_cast<UINT>(m_shaders[s_renderLayer][cameraTag].size()))
	{
		return false;
	}

	// シェーダー情報を取得する
	m_beganShader = &m_shaders[s_renderLayer][cameraTag][m_shaderCounter];
	m_pass = m_beganShader->pass;

	// 固定パイプラインを使用するとき
	if (m_beganShader->shader == nullptr)
	{
		m_passNum = 1;
		return true;
	}

	// シェーダーを開始する
	if (SUCCEEDED(m_beganShader->shader->GetEffect()->Begin(&m_passNum, 0)))
	{
		return true;
	}
	else
	{
		m_beganShader = nullptr;
		return false;
	}
}

//=============================================================
// 描画終了
//=============================================================
void Shader::End()
{
	if (m_beganShader != nullptr && m_beganShader->shader != nullptr)
	{
		m_beganShader->shader->GetEffect()->End();
		m_beganShader = nullptr;
	}

	// シェーダーの数をインクリメント
	m_shaderCounter++;
}

//=============================================================
// パス描画開始
//=============================================================
bool Shader::BeginPass()
{
	// パスに-1を設定しているとき（全パス）
	if (m_pass[0] == -1)
	{
		if (m_passCounter >= (int)m_passNum)
		{
			return false;
		}
	}

	// パスをすべて通ったとき
	if (m_passCounter >= m_pass.size())
	{
		return false;
	}

	// シェーダーがあるとき
	if (m_beganShader != nullptr && m_beganShader->shader != nullptr)
	{
		if (m_pass[0] == -1)
		{ // 全パス
			return SUCCEEDED(m_beganShader->shader->GetEffect()->BeginPass(m_passCounter));
		}
		else
		{ // 指定パス
			return SUCCEEDED(m_beganShader->shader->GetEffect()->BeginPass(m_pass[m_passCounter]));
		}
	}
	return true;
}

//=============================================================
// パス描画終了
//=============================================================
void Shader::EndPass()
{
	if (m_beganShader->shader != nullptr)
	{
		// 変数をリセットする
		ResetVariable();

		m_beganShader->shader->GetEffect()->EndPass();
	}

	// パスカウンターをインクリメント
	m_passCounter++;
}

//=============================================================
// ビルトイン変数を更新する
//=============================================================
void Shader::UpdateVariable(Camera* camera)
{
	// カメラタグ
	std::string cameraTag = "";
	if (m_shaders[s_renderLayer].count(camera->gameObject->tag))
	{
		cameraTag = camera->gameObject->tag;
	}

	// デバイスを取得する
	auto device = Manager::GetInstance()->GetDevice();

	// WVPの計算
	D3DXMATRIX worldMtx, viewMtx, projMtx;
	device->GetTransform(D3DTS_WORLD, &worldMtx);
	device->GetTransform(D3DTS_VIEW, &viewMtx);
	device->GetTransform(D3DTS_PROJECTION, &projMtx);
	D3DXMATRIX wvp = worldMtx * viewMtx * projMtx;
	D3DXMATRIX vp = viewMtx * projMtx;

	// 法線行列
	D3DXMATRIX normalMtx = worldMtx;
	D3DXMatrixInverse(&normalMtx, NULL, &worldMtx);
	D3DXMatrixTranspose(&normalMtx, &normalMtx);

	// カメラ位置
	D3DXVECTOR3 cameraPosWorld = Camera::GetCurrentCamera()->transform->GetWorldPosition();
	float cameraPos[3] = { cameraPosWorld.x, cameraPosWorld.y, cameraPosWorld.z };

	// 変数を設定する
	for (auto itr = m_shaders[s_renderLayer][cameraTag].begin(); itr != m_shaders[s_renderLayer][cameraTag].end(); itr++)
	{
		if ((*itr).shader != nullptr)
		{
			(*itr).shader->GetEffect()->SetMatrix("_wvp", &wvp);
			(*itr).shader->GetEffect()->SetMatrix("_vp", &vp);
			(*itr).shader->GetEffect()->SetMatrix("_world", &worldMtx);
			(*itr).shader->GetEffect()->SetMatrix("_view", &viewMtx);
			(*itr).shader->GetEffect()->SetMatrix("_proj", &projMtx);
			(*itr).shader->GetEffect()->SetMatrix("_normal", &normalMtx);
			(*itr).shader->GetEffect()->SetFloatArray("_cameraPos", &cameraPos[0], 3);
			(*itr).shader->GetEffect()->SetFloat("_clipNear", Camera::GetCurrentCamera()->GetClippingPlaneNear());
			(*itr).shader->GetEffect()->SetFloat("_clipFar", Camera::GetCurrentCamera()->GetClippingPlaneFar());
		}
	}
}

//=============================================================
// 変数をリセットする
//=============================================================
void Shader::ResetVariable()
{
	D3DXVECTOR4 ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3DXVECTOR4 diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3DXVECTOR4 emissive = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3DXVECTOR4 specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	float power = 1.0f;

	if (m_beganShader->shader != nullptr)
	{
		m_beganShader->shader->GetEffect()->SetVector("_ambient", &ambient);
		m_beganShader->shader->GetEffect()->SetVector("_diffuse", &diffuse);
		m_beganShader->shader->GetEffect()->SetVector("_emissive", &emissive);
		m_beganShader->shader->GetEffect()->SetVector("_specular", &specular);
		m_beganShader->shader->GetEffect()->SetFloat("_power", power);
		m_beganShader->shader->GetEffect()->SetFloat("_useTextureV", 0.0f);
		m_beganShader->shader->GetEffect()->SetBool("_useTexture", false);
		m_beganShader->shader->GetEffect()->SetTexture("_texture", nullptr);
	}
}

//=============================================================
// マテリアル設定
//=============================================================
void Shader::SetMaterial(D3DMATERIAL9& material, LPDIRECT3DBASETEXTURE9 texture)
{
	D3DXVECTOR4 ambient = { material.Ambient.r, material.Ambient.g, material.Ambient.b, material.Ambient.a };
	D3DXVECTOR4 diffuse = { material.Diffuse.r, material.Diffuse.g, material.Diffuse.b, material.Diffuse.a };
	D3DXVECTOR4 emissive = { material.Emissive.r, material.Emissive.g, material.Emissive.b, material.Emissive.a };
	D3DXVECTOR4 specular = { material.Specular.r, material.Specular.g, material.Specular.b, material.Specular.a };
	float power = material.Power;

	if (m_beganShader != nullptr && m_beganShader->shader != nullptr)
	{
		m_beganShader->shader->GetEffect()->SetVector("_ambient", &ambient);
		m_beganShader->shader->GetEffect()->SetVector("_diffuse", &diffuse);
		m_beganShader->shader->GetEffect()->SetVector("_emissive", &emissive);
		m_beganShader->shader->GetEffect()->SetVector("_specular", &specular);
		m_beganShader->shader->GetEffect()->SetFloat("_power", power);
		m_beganShader->shader->GetEffect()->SetFloat("_useTextureV", texture != nullptr ? 1.0f : 0.0f);
		m_beganShader->shader->GetEffect()->SetBool("_useTexture", texture != nullptr);
		m_beganShader->shader->GetEffect()->SetTexture("_texture", texture);
		m_beganShader->shader->GetEffect()->CommitChanges();
	}
}