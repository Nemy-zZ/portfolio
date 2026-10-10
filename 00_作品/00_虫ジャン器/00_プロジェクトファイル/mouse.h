//========================================================================================================
//
//マウス入力処理[mouse.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "input.h"

//マウスクラス
class Mouse : public Input
{
public:
	Mouse();   //コンストラクタ
	~Mouse();  //デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;   //初期化処理
	void Uninit() override;   //終了処理
	void Update() override;   //更新処理

	POINT GetPoint() { return this->m_point; };   //座標の取得処理
	D3DXVECTOR2 GetMove() { return this->m_move; };   //移動量の取得処理

private:
	void ResetPoint();   //座標のリセット処理

	POINT m_point;   //座標
	POINT m_pointOld;   //前回の座標
	D3DXVECTOR2 m_move;   //移動量
	bool m_isReset;   //マウスのリセットフラグ
};

#endif // !_MOUSE_H_