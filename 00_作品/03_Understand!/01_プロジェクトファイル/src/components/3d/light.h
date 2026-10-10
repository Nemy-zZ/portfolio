//------------------------------------------------------------
// @file		light.h
// @brief	ライト
// @detail	3Dオブジェクトを表示するために必要なライト
//------------------------------------------------------------
#ifndef _COMP_LIGHT_H_
#define _COMP_LIGHT_H_

#include "component.h"

//@brief ライトコンポーネント
class D3DLight : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	//@brief タイプを設定する
	void SetType(D3DLIGHTTYPE type) { m_light.Type = type; }
	//@brief 拡散光を設定する
	void SetDiffuse(D3DXCOLOR col) { m_light.Diffuse = col; }
	//@brief 方向を設定する
	void SetDirection(D3DXVECTOR3 dir) { m_light.Direction = dir; }

	//@brief デフォルトのライト設定
	static void SetDefaultLight(GameObject* gameObject);
private:
	D3DLIGHT9 m_light;		// ライト
	static std::vector<D3DLight*> m_lights;
};

#endif // !_COMP_LIGHT_H_
