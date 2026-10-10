//========================================================================================================
//
//アニメーションの処理[animation.h]
// Author MurataShinnosuke
//
//========================================================================================================
#include "main.h"

//アニメーションクラス
class Animation
{
public:
	static const int MAX_NUM = 8;   //最大数

	Animation();   //コンストラクタ
	~Animation();   //デストラクタ

	void Init();   //初期化処理
	void Uninit();   //終了処理
	void Update();   //更新処理
	void Reset();
	int GetId() { return this->m_nId; };   //番号の取得処理
	int GetFrame() { return this->m_nFrame; };   //フレームの取得処理
	int GetPattern() { return this->m_nPattern; };   //パターンの取得処理
	LPDIRECT3DTEXTURE9 GetTexture() { return this->m_pTexture; };

	static Animation* Create(int nPattern, int nTime, LPDIRECT3DTEXTURE9 pTexture);   //生成処理
	static void UpdateAll();   //全更新処理
	static Animation* Get(int nId) { return Animation::m_apAnimation[nId]; };   //取得処理

private:
	int m_nId;   //番号
	int m_nFrame;   //フレーム
	int m_nPattern;   //パターン
	int m_nTime;   //時間
	int m_nCnt;   //カウンター
	LPDIRECT3DTEXTURE9 m_pTexture;   //テクスチャ

	static Animation* m_apAnimation[Animation::MAX_NUM];   //アニメーションの情報
};