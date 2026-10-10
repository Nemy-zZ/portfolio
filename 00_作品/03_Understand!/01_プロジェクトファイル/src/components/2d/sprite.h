//------------------------------------------------------------
// @file		sprite.h
// @brief	スプライト
// @detail	2Dの板ポリゴン
//------------------------------------------------------------
#ifndef _COMP_SPRITE_H_
#define _COMP_SPRITE_H_

#include "component.h"

//@brief スプライト
class Sprite : public Component
{
public:
	virtual void Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void DrawUI() override;

	//@brief 色を設定する
	void SetColor(const D3DXCOLOR& color);
	//@brief 色を取得する
	D3DXCOLOR GetColor() { return m_color; }

	//@brief テクスチャを設定する
	void SetTexture(std::string path);

	//@brief UVを設定する
	void SetUV(const D3DXVECTOR2& topLeft, const D3DXVECTOR2& topRight, const D3DXVECTOR2& bottomLeft, const D3DXVECTOR2& bottomRight);

	//@brief テクスチャをバインドする
	void BindTexture(LPDIRECT3DTEXTURE9 texture) { m_texture = texture; }

	//@brief アンカーポイントを設定する
	void SetAnchorPoint(D3DXVECTOR2 anchor) { m_anchorPoint = anchor; }
private:
	//@brief 頂点を更新する
	void UpdateVertex();

	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;	// 頂点情報
	LPDIRECT3DTEXTURE9 m_texture;				// テクスチャ
	TransformMonitor* m_monitor;					// トランスフォーム監視
	D3DXVECTOR2 m_anchorPoint;					// アンカーポイント
	D3DXCOLOR m_color;								// 色
	bool m_isFirst;
};

//@brief アニメーションスプライト
class AnimationSprite : public Sprite
{
public:
	AnimationSprite() : m_width(1), m_height(1), m_idx(0), m_isPlaying(false), m_isLoop(true), m_frameTime(0.1f), m_elapsedTime(0.0f) {}
	void Update() override;

	//@brief セルの区分けを設定する
	void SetCell(const int& width, const int& height);

	//@brief インデックスを設定する
	void SetIdx(const int& idx) { m_idx = idx; }

	//@brief ループを設定する
	void SetLoop(const bool& enabled) { m_isLoop = enabled; }

	/**
	 * @brief 1フレームの時間を設定する
	 * @param sec : 秒指定
	*/
	void SetFrameTime(const float& sec) { m_frameTime = sec; }

	//@brief 再生する
	void Play() { m_isPlaying = true; }
	//@brief 停止する
	void Stop() 
	{ 
		m_isPlaying = false; 
		m_idx = 0;
		m_frameTime = 0.0f;
	}
	//@brief 一時停止する
	void Pause() { m_isPlaying = false; }
private:
	int m_width, m_height;	// セルの数
	int m_idx;						// インデックス
	bool m_isPlaying;			// 再生中か
	bool m_isLoop;				// ループするか
	float m_frameTime;		// 何秒で切り替えるか
	
	float m_elapsedTime;		// 経過時間
};

#endif // !_COMP_SPRITE_H_
