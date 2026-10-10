//======================================================================================================
//
//ディレクショナルライトの処理[dire_light.h]
// Auhtor Murata Shinnosuke
//
//======================================================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_ 

#include "component.h"

class DireLight : public Component   //(親)コンポーネント
{
public:
	//コンストラクタ
	DireLight(Object* pObject) : Component(pObject)
	{
		//メンバ変数を初期化する
		this->m_nId = DireLight::m_nTotal;
		ZeroMemory(&this->m_light, sizeof(D3DLIGHT9));

		//総数を追加する
		DireLight::m_nTotal++;
	}

	//デストラクタ
	~DireLight()
	{
	}

	void Init() override;   //初期化処理

	void SetColor(D3DXCOLOR color) { this->m_color = color; };   //色の設定

private:
	int m_nId;   //番号
	D3DLIGHT9 m_light;   //ライト
	D3DXCOLOR m_color;   //色

	static int m_nTotal;   //個数
};

#endif // !_LIGHT_H_