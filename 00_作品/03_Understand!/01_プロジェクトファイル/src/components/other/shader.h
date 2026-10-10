//------------------------------------------------------------
// @file		shader.h
// @brief	シェーダー
// @detail	シェーダーの命令を行います
//------------------------------------------------------------
#ifndef _SHADER_H_
#define _SHADER_H_

#include "component.h"
class Camera;

/**
 * @brief シェーダーコンポーネント
 * @details シェーダーを適用します
*/
class Shader : public Component
{
public:
	//@brief レンダーレイヤ
	enum RenderLayer
	{
		ShadowCaster,	// 影用深度
		Opaque,			// 通常
		Transparent,		// 半透明

		MAX
	};

	/**
	 * @brief シェーダーを追加する
	 * @param[in] path: シェーダーのパス
	 * @param[in] cameraTag: カメラのタグ（未入力で全体適用）
	 * @param[in] layer: レンダリングをするレイヤー
	 * @param[in] pass: 描画するパス（-1で全パス）
	*/
	ShaderData* AddShader(const std::string& path, const std::string& cameraTag = "", const RenderLayer& layer = RenderLayer::Opaque, const std::vector<int> pass = {-1});
	//@brief シェーダーを削除する
	void RemoveShader(const RenderLayer& layer, const int& idx, const std::string& cameraTag = "");
	//@brief シェーダーをクリアする
	void ClearShader();

	//@brief オブジェクト全体にシェーダーを追加します
	static void ApplyShaderToObjects(GameObject* objects, const std::string& path, const std::string& cameraTag = "", const RenderLayer& layer = RenderLayer::Opaque, const std::vector<int> pass = { -1 });

	//---------------------------- ここから内部処理 ----------------------------

	//@brief レンダーレイヤを設定する
	static void SetRenderLayer(const RenderLayer& layer) { s_renderLayer = layer; }
	//@brief レンダーレイヤを取得する
	static RenderLayer GetRenderLayer() { return s_renderLayer; }
	//@brief 描画命令があるか
	bool IsDrawCall();
	//@brief セット
	void Set();
	//@brief 描画開始
	bool Begin();
	//@brief 描画終了
	void End();
	//@brief パス描画開始
	bool BeginPass();
	//@brief パス描画終了
	void EndPass();
	//@brief マテリアルデータを変数として更新する
	void SetMaterial(D3DMATERIAL9& material, LPDIRECT3DBASETEXTURE9 texture = nullptr);
private:
	//@brief ビルトイン変数を更新する
	void UpdateVariable(Camera* camera);

	//@brief 変数をリセットする
	void ResetVariable();

	//@brief シェーダー情報
	struct ShaderInfo
	{
		ShaderData* shader;			// シェーダー
		std::vector<int> pass;		// パス
	};

	std::map<std::string, std::vector<ShaderInfo>> m_shaders[RenderLayer::MAX];

	static RenderLayer s_renderLayer;			// レンダーレイヤ
	ShaderInfo* m_beganShader;					// 開始されたシェーダー
	UINT m_shaderCounter;							// シェーダーカウンター
	std::vector<int> m_pass;						// パス
	UINT m_passNum;										// パスの数
	int m_passCounter;								// パスカウンター
};

#endif // !_SHADER_H_
