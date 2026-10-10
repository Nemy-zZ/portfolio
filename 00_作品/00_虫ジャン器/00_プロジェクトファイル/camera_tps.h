//==========================================================================================================
//
//TPS視点カメラの処理[camera_tps.h]
// Author MurataShinnosuke
//
//==========================================================================================================
#ifndef _CAMERA_TPS_H_
#define _CAMERA_TPS_H_

#include "behavior.h"

//前方宣言
class Camera;
//TPS視点カメラ
class CameraTPS : public Behavior   //(親)挙動
{
public:
	static const float DISTANCE_VIEW_TO_REGARD;   //視点の注視点の距離
	static const float DISTANCE_TARGET_TO_FULCRUM;   //視点の注視点の距離

	//コンストラクタ
	CameraTPS(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_pCamera = nullptr;   //カメラ
		this->m_posFulcrum = { 0.0f, 0.0f, 0.0f };   //支点の位置
		this->m_posTarget = { 0.0f, 0.0f, 0.0f };   //対象の位置
		this->m_fSensi = 0.02f;   //感度
		this->m_isAim = false;   //エイムフラグ
	}

	//デストラクタ
	~CameraTPS()
	{
	}

	void Init() override;
	void Update() override;

private:
	void Control();

	Camera* m_pCamera;   //カメラ
	D3DXVECTOR3 m_posFulcrum;   //支点の位置
	D3DXVECTOR3 m_posTarget;   //対象の位置
	float m_fSensi;   //感度
	bool m_isAim;   //エイムフラグ
};

#endif // !_CAMERA_TPS_H_
