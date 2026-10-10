//********************************************************************************************************
//
//カメラの処理[camera.h]
// Auhtor Murata Shinnosuke
//
//********************************************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_ 

#include "component.h"

//カメラクラス
class Camera : public Component
{
public:
	//投影方法の列挙
	enum MATRIX
	{
		IS_PERSPECTIVE = 0,   //透視投影
		IS_ORTHO,   //平行投影
		IS_MAX
	};

	//コンストラクタ
	Camera(Object* pObject) : Component(pObject)
	{
		this->m_posRegard = { 0.0f, 0.0f, 0.0f };   //注視点
		this->m_vecUp = { 0.0f, 1.0f, 0.0f };   //上方向ベクトル
		this->m_mtxProjection;   //プロジェクションマトリックス
		this->m_mtxView;   //ビューマトリックス
		this->m_mtx = Camera::MATRIX::IS_PERSPECTIVE;
	}

	//デストラクタ
	~Camera()
	{
	}

	void Update() override;   //更新
	void Set();   //設定処理

	void SetPosRegard(D3DXVECTOR3 pos) { this->m_posRegard = pos; };   //注視点の位置の設定

	D3DXVECTOR3 GetPosRegard() { return this->m_posRegard; };   //注視点の取得処理

private:
	D3DXVECTOR3 m_posRegard;   //注視点の位置
	D3DXVECTOR3 m_vecUp;   //上方向ベクトル
	D3DXMATRIX m_mtxProjection;   //プロジェクションマトリックス
	D3DXMATRIX m_mtxView;   //ビューマトリックス
	MATRIX m_mtx;   //投影方法
};

#endif // !_CAMERA_H_