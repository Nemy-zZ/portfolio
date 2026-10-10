//------------------------------------------------------------
// @file		post_effect.h
// @brief	ポストエフェクト
// @detail	ポストエフェクトシェーダーの命令を行います
//------------------------------------------------------------
#ifndef _POST_EFFECT_H_
#define _POST_EFFECT_H_

class Camera;

/**
 * @brief ポストエフェクト（基底）
*/
class PostEffect
{
public:
	PostEffect(const std::string& path, const std::vector<std::string>& techniques);
	virtual ~PostEffect();

	//@brief 初期化
	virtual void Init() {}
	//@brief 終了
	virtual void Uninit() {}
	//@brief 更新
	virtual void Update() {}

	//@brief アクティブ状態を設定する
	void SetActive(const bool& isActived) { m_isActive = isActived; }
	//@brief アクティブ状態を取得する
	bool IsActive() { return m_isActive; }
	//@brief エフェクトを取得する
	LPD3DXEFFECT GetShader() { return m_shader; }

	//@brief セット
	void Set(Camera* camera);
	//@brief テクニック開始
	bool BeginTechnique();
	//@brief テクニック終了
	void EndTechnique();
	//@brief 描画開始
	bool Begin();
	//@brief 描画終了
	void End();
	//@brief パス描画開始
	bool BeginPass();
	//@brief パス描画終了
	void EndPass();

	//@brief 処理中のテクスチャをセットする
	void SetProcessingTexture(LPDIRECT3DTEXTURE9 tex);

	//----------------- 設定項目 -----------------
	
	//@brief パスを指定する
	void SetPass(const std::string& technique, const std::vector<int>& pass = { -1 });
	//@brief テクニックを指定する
	void SetTechnique(const std::vector<std::string>& techniques);
private:
	// ビルトイン変数を更新する
	void UpdateVariable();

	std::vector<std::string> m_techniques;
	int m_techniqueCounter;

	std::map<std::string, std::vector<int>> m_pass;
	UINT m_passNum;
	int m_passCounter;

	LPD3DXEFFECT m_shader;
	Camera* m_camera;
	bool m_isActive = true;
};

#endif // !_POST_EFFECT_H_
