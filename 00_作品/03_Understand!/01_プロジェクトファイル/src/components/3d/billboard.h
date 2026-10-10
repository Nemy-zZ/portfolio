//------------------------------------------------------------
// @file		billboard.h
// @brief	ビルボード
//------------------------------------------------------------
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "component.h"

/**
 * @brief ビルボード
*/
class Billboard : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//@brief テクスチャを割り当てる
	void BindTexture(LPDIRECT3DTEXTURE9 texture) { m_texture = texture; }
	//@brief テクスチャを画像パスから設定する
	void SetTexture(const std::string& path);
	//@brief 色を設定する
	void SetColor(const D3DXCOLOR& color);

	//@brief 色を取得する
	D3DXCOLOR GetColor() { return m_color; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;		// 頂点バッファ
	LPDIRECT3DTEXTURE9 m_texture;					// テクスチャ
	D3DXCOLOR m_color;									// 色
	TransformMonitor* m_monitor;
};

#endif // !_FIELD_H_
