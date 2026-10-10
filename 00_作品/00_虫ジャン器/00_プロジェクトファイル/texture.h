//========================================================================================================
//
//テクスチャ処理[texture.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//テクスチャクラス
class Texture
{
public:
	typedef struct
	{
		std::string sPass;   //パス
		LPDIRECT3DTEXTURE9 pTexture;   //テクスチャ
	}List;   //リスト

	Texture();   //コンストラクタ
	~Texture();   //デストラクタ

	void Init();   //初期化処理
	void Uninit();   //終了処理
	LPDIRECT3DTEXTURE9 Get(std::string sFileName);   //取得処理

private:
	std::vector<Texture::List> m_vList;   //リスト
};

#endif // !_TEXTURE_H_
