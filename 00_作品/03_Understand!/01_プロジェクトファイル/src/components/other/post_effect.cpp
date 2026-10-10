//------------------------------------------------------------
// @file		post_effect.cpp
// @brief	ポストエフェクト
// @detail	ポストエフェクトシェーダーの命令を行います
//------------------------------------------------------------
#include "post_effect.h"

#include "components/3d/camera.h"

//=============================================================
// コンストラクタ
//=============================================================
PostEffect::PostEffect(const std::string& path, const std::vector<std::string>& techniques)
{
	auto device = Manager::GetInstance()->GetDevice();
	LPD3DXBUFFER pErrorBuffer = nullptr;

	// シェーダーを読み込む
	if (FAILED(D3DXCreateEffectFromFile(
		device,
		path.c_str(),
		NULL,
		NULL,
		0,
		NULL,
		&m_shader,
		&pErrorBuffer
	)))
	{
		// エラーログ
		Log::SendLog(static_cast<const char*>(pErrorBuffer->GetBufferPointer()));
		pErrorBuffer->Release();
		m_shader = nullptr;
	}

	if (!m_shader)
	{
		Log::SendLog("ポストエフェクト \"" + path + "\" の読み込みに失敗しました", Log::TYPE_ERROR);
	}
	else
	{
		SetTechnique(techniques);
	}
}

//=============================================================
// デストラクタ
//=============================================================
PostEffect::~PostEffect()
{
	if (m_shader != nullptr)
	{
		m_shader->Release();
		m_shader = nullptr;
	}
}

//=============================================================
// セット
//=============================================================
void PostEffect::Set(Camera* camera)
{
	// 初期化
	m_techniqueCounter = 0;
	m_passNum = 0;
	m_passCounter = 0;

	// カメラをセットする
	m_camera = camera;

	// ビルトイン変数を設定する
	UpdateVariable();
}

//=============================================================
// テクニック開始
//=============================================================
bool PostEffect::BeginTechnique()
{
	// テクニックをすべて通ったとき
	if (m_techniqueCounter >= m_techniques.size())
	{
		return false;
	}

	// テクニック開始
	if (SUCCEEDED(m_shader->SetTechnique(m_techniques[m_techniqueCounter].c_str())))
	{
		m_passCounter = 0;

		return true;
	}

	return false;
}

//=============================================================
// テクニック終了
//=============================================================
void PostEffect::EndTechnique()
{
	// テクニックカウンターをインクリメント
	m_techniqueCounter++;
}

//=============================================================
// 描画開始
//=============================================================
bool PostEffect::Begin()
{
	if (!m_shader)
	{
		return false;
	}

	return SUCCEEDED(m_shader->Begin(&m_passNum, 0));
}

//=============================================================
// 描画終了
//=============================================================
void PostEffect::End()
{
	m_shader->End();
}

//=============================================================
// パス描画開始
//=============================================================
bool PostEffect::BeginPass()
{
	// パスが定義されていないとき
	if (!m_pass.count(m_techniques[m_techniqueCounter]))
	{
		m_pass[m_techniques[m_techniqueCounter]] = { -1 };
	}

	// パスに-1を設定しているとき（全パス）
	if (m_pass[m_techniques[m_techniqueCounter]][0] == -1)
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

	// パス描画開始
	if (m_pass[m_techniques[m_techniqueCounter]][0] == -1)
	{ // 全パス
		return SUCCEEDED(m_shader->BeginPass(m_passCounter));
	}
	else
	{ // 指定パス
		return SUCCEEDED(m_shader->BeginPass(m_pass[m_techniques[m_techniqueCounter]][m_passCounter]));
	}
}

//=============================================================
// パス描画終了
//=============================================================
void PostEffect::EndPass()
{
	m_shader->EndPass();

	// パスカウンターをインクリメント
	m_passCounter++;
}

//=============================================================
// 処理中のテクスチャを設定する
//=============================================================
void PostEffect::SetProcessingTexture(LPDIRECT3DTEXTURE9 tex)
{
	if (m_shader != nullptr)
	{
		m_shader->SetTexture("_tex", tex);
	}
}

//=============================================================
// パスを指定する
//=============================================================
void PostEffect::SetPass(const std::string& technique, const std::vector<int>& pass)
{
	// パスを指定しなかったらエラー
	if (pass.empty())
	{
		Log::SendLog("シェーダーを追加する際、最低でも1つ以上 pass を設定する必要があります", Log::TYPE_ERROR);
		return;
	}

	m_pass[technique] = pass;
}

//=============================================================
// テクニックを指定する
//=============================================================
void PostEffect::SetTechnique(const std::vector<std::string>& techniques)
{
	// テクニックを指定しなかったらエラー
	if (techniques.empty())
	{
		Log::SendLog("テクニックは最低でも1つ以上設定する必要があります", Log::TYPE_ERROR);
		return;
	}

	m_techniques = techniques;
}

//=============================================================
// ビルトイン変数を更新する
//=============================================================
void PostEffect::UpdateVariable()
{
	if (m_shader != nullptr)
	{
		m_shader->SetTexture("_colorTex", m_camera->GetRenderTextures().color);
		m_shader->SetTexture("_depthTex", m_camera->GetRenderTextures().depth);

		m_shader->SetFloat("_width", m_camera->GetTextureSize().x);
		m_shader->SetFloat("_height", m_camera->GetTextureSize().y);

		float size[2] = { m_camera->GetTextureSize().x, m_camera->GetTextureSize().y };
		m_shader->SetFloatArray("_size", &size[0], 2);

		m_shader->SetFloat("_deltaTime", Main::GetInstance().GetDeltaTime());
		m_shader->SetFloat("_elapsedTime", static_cast<float>(Main::GetInstance().GetElapsedTime() * 0.001f));

		float texel[2] = { 1.0f / m_camera->GetTextureSize().x, 1.0f / m_camera->GetTextureSize().y };
		m_shader->SetFloatArray("_texel", &texel[0], 2);
	}
}
