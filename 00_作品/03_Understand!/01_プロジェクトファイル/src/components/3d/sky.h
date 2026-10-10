//------------------------------------------------------------
// @file		sky.h
// @brief	空
//------------------------------------------------------------
#ifndef _SKY_H_
#define _SKY_H_

#include "component.h"

/**
 * @brief スカイボックス
 * @details ボックス状で空を表現します
*/
class Skybox : public Component
{
public:
	// 面
	enum FACE
	{
		TOP,			// 上
		BOTTOM,	// 下
		FRONT,		// 正面
		BACK,		// 背後
		LEFT,		// 左
		RIGHT		// 右
	};

	// テクスチャタイプ
	enum TEXTURETYPE
	{
		SINGLE,		// 1枚
		SPLIT		// 分割（6枚）
	};

	void Init() override;
	void Uninit() override;

	//@brief 空を描画します
	void DrawSky();

	//@brief テクスチャタイプを設定する
	void SetTextureType(TEXTURETYPE textureType);
	//@brief テクスチャを設定する
	void SetTexture(const std::string& path, const FACE& face = TOP);
	//@brief スカイボックスを読み込む
	void LoadSkybox(const std::string& path);

private:
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff[6];		// 頂点バッファ
	LPDIRECT3DTEXTURE9 m_texture[6];					// テクスチャ
	D3DXCOLOR m_color;										// 色
	TEXTURETYPE m_textureType;							// テクスチャタイプ
};

#endif // !_SKY_H_
