//------------------------------------------------------------
// @file		lod.h
// @brief	Level of Detail
//------------------------------------------------------------
#ifndef _LOD_H_
#define _LOD_H_

#include "component.h"

//@brief LOD
class LOD : public Component
{
public:
	enum LEVEL
	{
		LEVEL_0 = 0,
		LEVEL_1,
		LEVEL_2,
		CULLED,
		MAX
	};

public:
	//@brief LODされたモデルを取得する
	ModelData* GetLODModel();

	//@brief 最もLODの高いモデルを取得する
	ModelData* GetHighLODModel();

	/**
	 * @brief LODを設定する
	 * @param[in] ratio : 画面の占める割合
	 * @param[in] modelData : モデルデータ（nullptrでカリング）
	*/
	void SetLOD(const LEVEL& level, const float& ratio, ModelData* modelData);

	/**
	 * @brief LODを設定する
	 * @param[in] ratio : 画面の占める割合
	 * @param[in] modelData : モデルデータ（nullptrでカリング）
	*/
	void SetLODLevel(const LEVEL& level, const float& ratio);

	//@brief LODを適用するか
	void SetApplyLOD(const bool& enabled) { m_applyLOD = enabled; }

	//@brief オブジェクトの画面の占める割合を求める
	float ScreenRelativeHeight();
private:
	bool m_applyLOD = true;

	//@brief LODデータ
	struct Data
	{
		float ratio = 0.0f;						// 割合
		ModelData* model = nullptr;	// モデル
	};
	Data m_lods[LEVEL::MAX];
};

#endif // !_LOD_H_
