//------------------------------------------------------------
// @file		lod.cpp
// @brief	Level of Detail
//------------------------------------------------------------
#include "lod.h"
#include "camera.h"
#include "model.h"

//=============================================================
// LODされたモデルを取得する
//=============================================================
ModelData* LOD::GetLODModel()
{
    Model* model = gameObject->GetComponent<Model>();
    if (model == nullptr) return nullptr;
    if (model->GetModelData() == nullptr) return nullptr;

    // LODを適用しないとき
    if (!m_applyLOD)
    {
        return model->GetModelData();
    }

    // 表示の占める割合を計算する
    float relative = ScreenRelativeHeight();

    // 全体の数値を取得する
    float ratio = 0.0f;
    for (int i = 0; i < LEVEL::MAX; i++)
    {
        ratio += m_lods[i].ratio;
    }

    // 描画モデルを選択する
    for (int i = 0; i < LEVEL::MAX; i++)
    {
        ratio -= m_lods[i].ratio;
        if (relative >= ratio)
        {
            return m_lods[i].model;
        }
    }

    // 該当しなかったとき
    return model->GetModelData();
}

//=============================================================
// 最もレベルの高いLODを取得する
//=============================================================
ModelData* LOD::GetHighLODModel()
{
    for (int i = 0; i < LEVEL::MAX; i++)
    {
        if (m_lods[i].model != nullptr)
        {
            return m_lods[i].model;
        }
    }
    return nullptr;
}

//=============================================================
// LODを追加する
//=============================================================
void LOD::SetLOD(const LEVEL& level, const float& ratio, ModelData* modelData)
{
    // 範囲外
    if (level >= LEVEL::MAX) return;
    if (!(0.0f < ratio && ratio < 1.0f)) return;

    // 設定
    m_lods[level].ratio = ratio;
    m_lods[level].model = modelData;
}

//=============================================================
// LODレベルを設定する
//=============================================================
void LOD::SetLODLevel(const LEVEL& level, const float& ratio)
{
    // 範囲外
    if (level >= LEVEL::MAX) return;
    if (!(0.0f < ratio && ratio < 1.0f)) return;

    // 設定
    m_lods[level].ratio = ratio;
}

//=============================================================
// 画面の占める割合を求める
//=============================================================
float LOD::ScreenRelativeHeight()
{
    const Camera* cam = Camera::GetCurrentCamera();

    // オブジェクト中心とカメラ位置
    const D3DXVECTOR3 camPos = cam->transform->GetWorldPosition();
    const D3DXVECTOR3 objPos = transform->GetWorldPosition();

    // ユークリッド距離（Z方向だけでなく3Dの距離）
    D3DXVECTOR3 disV = objPos - camPos;
    float distance = D3DXVec3Length(&disV);
    if (distance <= 0.0f) return 0.0f;

    // 半径
    float radius = gameObject->GetComponent<Model>()->GetModelData()->GetMaxRadius(transform->GetWorldScale());

    // カメラの垂直FOV
    float fovY = D3DXToRadian(45.0f);

    // 小角度近似式（LOD用途では十分）
    float ratio = (radius / distance) / tanf(fovY * 0.5f);
    ratio /= 2.0f;

    return std::clamp(ratio, 0.0f, 1.0f);
}
