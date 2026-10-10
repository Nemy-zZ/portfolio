//------------------------------------------------------------
// @file		camera.h
// @brief	カメラ
// @detail	3D空間に存在するオブジェクトを描画するためのデバイス
//------------------------------------------------------------
#ifndef _COMP_CAMERA_H_
#define _COMP_CAMERA_H_

#include "component.h"
#include "sky.h"
#include "components/other/post_effect.h"

// 定義
constexpr float CLIPPING_NEAR = 0.1f;			// 最短描画距離
constexpr float CLIPPING_FAR = 1000.0f;			// 最高描画距離
constexpr float CAMERA_LENGTH = 100.0f;		// 視点と注視点の距離

//@brief カメラコンポーネント
class Camera : public Component
{
public:
	//@brief 描画テクスチャ
	struct RenderTextures
	{
		LPDIRECT3DTEXTURE9 color = nullptr;				// カラーテクスチャ
		LPDIRECT3DTEXTURE9 depth = nullptr;				// 深度テクスチャ
		LPDIRECT3DTEXTURE9 postProcessed = nullptr;	// ポストプロセス適用済み
	};

	//@brief 投影モード
	enum MODE
	{
		MODE_PERSPECTIVE,		// 透視投影
		MODE_PARALLEL,			// 平行投影
	};

	//@brief 初期化
	void Init() override;
	//@brief 終了
	void Uninit() override;
	//@brief 更新
	void Update() override;
	//@brief 描画ポリゴンの描画
	void DrawPolygon();

	//@brief 上向きベクトルを設定する
	void SetVectorUp(D3DXVECTOR3 up) { m_vecU = up; }
	//@brief 視点と注視点の距離を設定する
	void SetLength(float length) { m_length = length; }
	//@brief 視野角を設定する
	void SetFOV(const float& fov) { m_fov = fov; }
	//@brief 最短描画距離を設定する
	void SetClippingPlaneNear(float value) { m_clippingPlanes.min = value; }
	//@brief 最高描画距離を設定する
	void SetClippingPlaneFar(float value) { m_clippingPlanes.max = value; }
	//@brief 投影モードを設定する
	void SetMode(MODE mode) { m_mode = mode; }
	//@brief 加算値を設定する
	void SetAddition(const D3DXVECTOR3& value) { m_addition = value; }

	//@brief 上向きベクトルを取得する
	D3DXVECTOR3 GetVectorUp() { return m_vecU; }
	//@brief 視点と注視点の距離を取得する
	float GetLength() { return m_length; }
	//@brief 視野角を取得する
	float GetFOV() { return m_fov; }
	//@brief 最短描画距離を取得する
	float GetClippingPlaneNear() { return m_clippingPlanes.min; }
	//@brief 最高描画距離を取得する
	float GetClippingPlaneFar() { return m_clippingPlanes.max; }
	//@brief 投影モードを取得する
	MODE GetMode() { return m_mode; }
	//@brief 加算値を取得する
	D3DXVECTOR3 GetAddition() { return m_addition; }
	//@brief プロジェクションマトリックスを取得する
	D3DXMATRIX GetProjectionMatrix() { return m_projMtx; }
	//@brief ビューマトリックスを取得する
	D3DXMATRIX GetViewMatrix() { return m_viewMtx; }

	//@brief テクスチャサイズを設定する
	bool SetTextureSize(const UINT& width, const UINT& height);
	//@brief テクスチャサイズを取得する
	D3DXVECTOR2 GetTextureSize() { return D3DXVECTOR2((float)m_textureSize[0], (float)m_textureSize[1]); }
	//@brief 描画位置を設定する
	void SetDrawPosition(const D3DXVECTOR2& position);
	//@brief レンダーテクスチャを取得する
	RenderTextures& GetRenderTextures() { return m_renderTextures; }

	//@brief ポストエフェクトを追加する
	template<class T> T* AddPostEffect()
	{
		// 既に存在しないか
		for (auto fx : m_postEffects)
		{
			if (typeid(T) == typeid(*fx))
			{
				return (T*)fx;
			}
		}

		T* effect = new T();
		m_postEffects.push_back(effect);
		return effect;
	}
	//@brief ポストエフェクトを削除する
	template<class T> void RemovePostEffect()
	{
		for (auto fx : m_postEffects)
		{
			if (typeid(T) == typeid(*fx))
			{
				PostEffect* val = *fx;
				m_postEffects.erase(fx);

				val->Uninit();
				delete val;
				val = nullptr;
				return;
			}
		}
	}
	//@brief ポストエフェクトを有効/無効化
	template<class T> void SetActivePostEffect(const bool& isActived)
	{
		for (auto fx : m_postEffects)
		{
			if (typeid(T) == typeid(*fx))
			{
				fx->SetActive(isActived);
				return;
			}
		}
	}
	//@brief ポストエフェクトを取得する
	template<class T> T* GetPostEffect()
	{
		for (auto fx : m_postEffects)
		{
			if (typeid(T) == typeid(*fx))
			{
				return (T*)fx;
			}
		}
		return nullptr;
	}

	//@brief ビューポートの位置を設定する
	void SetViewportPosition(float x, float y);
	//@brief ビューポートのサイズを設定する
	void SetViewportSize(float width, float height);
	//@brief 表示設定
	void SetVisible(const bool& visible) { m_visible = visible; }
	//@brief 表示設定を取得する
	bool GetVisible() { return m_visible; }

	//@brief クリア時のカラーを設定する
	void SetClearColor(const D3DXCOLOR& color) { m_clearColor = color; }
	//@brief クリア時のカラーを取得する
	D3DXCOLOR GetClearColor() { return m_clearColor; }

	//@brief カメラの設置
	void SetCamera();
	//@brief 描画開始
	bool Begin();
	//@brief 描画終了
	void End();

	//@brief すべてのカメラを取得する
	static const std::vector<Camera*>& GetAllCameras() { return m_cameras; }

	//@brief 現在のカメラを取得する
	static Camera* GetCurrentCamera() { return m_currentCamera; }

private:
	//@brief レンダーテクスチャの解放
	void ReleaseRenderTextures();

	//@brief 描画ポリゴンの初期化
	void InitRenderPolygon();

	//@brief 描画ポリゴンの更新
	void UpdateDrawPolygon();

	D3DXVECTOR3 m_posR;				// 注視点
	D3DXVECTOR3 m_vecU;				// 上向きベクトル
	float m_length;							// 視点と注視点の距離
	float m_fov;								// 視野角
	Range<float> m_clippingPlanes;	// 描画範囲
	MODE m_mode;							// 投影モード
	D3DXCOLOR m_clearColor;			// クリア時のカラー
	D3DXVECTOR3 m_addition;		// 加算

	D3DXMATRIX m_projMtx;			// プロジェクションマトリックス
	D3DXMATRIX m_viewMtx;			// ビューマトリックス
	D3DVIEWPORT9 m_viewport;		// ビューポート
	bool m_visible;							// 表示するか

	static std::vector<Camera*> m_cameras;
	static Camera* m_currentCamera;				// 現在のカメラ

	// テクスチャ描画データ
	RenderTextures m_renderTextures;
	LPDIRECT3DSURFACE9 m_colorSurface = nullptr;
	LPDIRECT3DSURFACE9 m_depthSurface = nullptr;
	LPDIRECT3DSURFACE9 m_depthStencil = nullptr;
	LPDIRECT3DSURFACE9 m_oldRT, m_oldDS;
	D3DXVECTOR2 m_drawPosition = { 0.0f,0.0f };
	UINT m_textureSize[2] = { 0, 0 };
	LPDIRECT3DTEXTURE9 m_postA, m_postB;
	LPDIRECT3DSURFACE9 m_surfPostA, m_surfPostB;

	std::vector<PostEffect*> m_postEffects;

	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff = nullptr;
};

#endif // !_COMP_CAMERA_H_