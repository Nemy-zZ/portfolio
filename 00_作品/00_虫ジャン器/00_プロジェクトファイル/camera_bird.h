//==========================================================================================================
//
//バード視点カメラの処理[camera_bird.h]
// Author MurataShinnosuke
//
//==========================================================================================================
#ifndef _CAMERA_BIRD_H_
#define _CAMERA_BIRD_H_

#include "behavior.h"

//前方宣言
class Camera;
//バード視点カメラ
class CameraBird : public Behavior   //(親)挙動
{
public:
	static const D3DXVECTOR3 OFFSET_POS_REGARD;   //視点の注視点の距離

	//コンストラクタ
	CameraBird(Object* pObject) : Behavior(pObject)
	{
		//メンバ変数を初期化する
		this->m_pCamera = nullptr;   //カメラ
		this->m_pPlayer = nullptr;   //プレイヤー
	}

	//デストラクタ
	~CameraBird()
	{
	}

	void Init() override;
	void Update() override;

private:
	Object* m_pPlayer;   //プレイヤー
	Camera* m_pCamera;   //カメラ
};

#endif // !_CAMERA_BIRD_H_
