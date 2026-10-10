//------------------------------------------------------------
// @file		culling.h
// @brief	カリング
//------------------------------------------------------------
#ifndef _CULLING_H_
#define _CULLING_H_

#include "component.h"
#include "camera.h"
#include "components/other/camera_options.h"

//@brief カリング情報コンポーネント
class CullingData : public Component
{
public:
	CullingData() : m_radius(-1.0f), m_isFrustum(true), m_isOcclusion(true) {}

	//@brief 初期化
	void Init() override;
	//@brief 終了
	void Uninit() override;
	//@brief 描画
	void DrawCulling(OcclusionCulling* occlusion);

	//@brief 描画するかを設定します (CullingManagerで実行されます)
	void SetDraw(const bool& isDraw) { m_isDraw = isDraw; }
	//@brief 描画するかを取得します
	bool IsDraw() const { return m_isDraw; }

	//@brief 半径を設定します (0.0以下で自動設定)
	void SetRadius(const float& radius = -1.0f) { m_radius = radius; }
	//@brief 半径を取得します
	const float GetRadius();

	//@brief カリング情報をリセットする
	void ResetCulledData();
	//@brief フラスタムカリング情報をセットする
	void SetCulledFrustum(const bool& isCulled) { m_culledFrustum = isCulled; }
	//@brief オクルージョンカリング情報をセットする
	void SetCulledOcclusion(const bool& isCulled) { m_culledOcclusion = isCulled; }
	//@brief フラスタムカリング情報を取得する
	bool IsCulledFrustum() { return m_culledFrustum; }
	//@brief オクルージョンカリング情報を取得する
	bool IsCulledOcclusion() { return m_culledOcclusion; }

	//@brief フラスタムカリングを設定します
	void SetFrustumCulling(const bool& enabled) { m_isFrustum = enabled; }
	//@brief フラスタムカリングを取得します
	const bool IsFrustumCulling() { return m_isFrustum; }

	//@brief オクルージョンカリングを設定します
	void SetOcclusionCulling(const bool& enabled) { m_isOcclusion = enabled; }
	//@brief オクルージョンカリングを取得します
	const bool IsOcclusionCulling() { return m_isOcclusion; }
	////@brief オクルージョンクエリ―を取得する
	//IDirect3DQuery9* GetOcclusionQuery() { return m_occlusionData.occlusionQuery; }
	////@brief ピクセルが描画されたか
	//bool IsDrawPixels() { return m_occlusionData.isDrawPixels; }
private:
	// 結果
	bool m_isDraw;				// 描画するか

	// 情報
	float m_radius;				// 半径

	//struct OcclusionData
	//{
	//	IDirect3DQuery9* occlusionQuery = nullptr;
	//	//bool isDrawPixels = false;
	//};
	//OcclusionData m_occlusionData;

	// カリング状況
	bool m_culledFrustum;
	bool m_culledOcclusion;

	// カリング有効状況
	bool m_isFrustum;			// フラスタム
	bool m_isOcclusion;		// オクルージョン
};

//@brief カリングマネージャー
class CullingManager final
{
public:
	//@brief インスタンスを取得する
	static CullingManager& GetInstance()
	{
		static CullingManager instance;
		return instance;
	}

	//@brief カリング情報の更新
	void Update(Camera* camera);

	//@brief カリング描画
	void Draw(Camera* camera);

	//@brief 結果
	void Result();

private:
	CullingManager() = default;

	// カリング数
	int m_frustumCullNum;
	int m_occlusionCullNum;
};

#endif // !_CULLING_H_
