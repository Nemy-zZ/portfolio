//========================================================================================================
//
//パーツの処理[parts.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _PARTS_H_
#define _PARTS_H_

#include "main.h"
#include "Xfile.h"

//パーツ
class Parts
{
public:
	Parts(int nPriority = 3);   //コンストラクタ
	~Parts();   //デストラクタ

	void Init();   //初期化処理
	void Uninit();   //終了処理
	void Update();   //更新処理
	void Draw(D3DXMATRIX mtxWorldParent);   //描画処理

	void SetXFile(XFile::Mesh mesh) { this->m_mesh = mesh; };   //パーツの結びつけ処理
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture) { this->m_pTexture = pTexture; };   //テクスチャの結びつけ処理
	void SetId(int nId) { this->m_nId = nId; };
	void SetIdParent(int nId) { this->m_nIdParent = nId; };   //親の設定処理
	void SetPos(D3DXVECTOR3 pos) { this->m_pos = pos; };
	void SetRot(D3DXVECTOR3 rot) { this->m_rot = rot; };

	int GetId() { return this->m_nId; };   //番号の取得
	int GetIdParent() { return this->m_nIdParent; };   //親の番号の取得
	D3DXVECTOR3 GetPos() { return this->m_pos; };
	D3DXVECTOR3 GetRot() { return this->m_rot; };
	D3DXMATRIX GetMtxWorld() { return this->m_mtxWorld; };   //ワールドマトリックスの取得処理

private:
	int m_nId;   //番号
	XFile::Mesh m_mesh;   //メッシュ
	LPDIRECT3DTEXTURE9 m_pTexture;   //テクスチャ
	D3DXMATRIX m_mtxWorld;   //ワールドマトリックス
	int m_nIdParent;   //親の番号
	D3DXVECTOR3 m_pos;   //位置
	D3DXVECTOR3 m_rot;   //向き
};

#endif // !_PARTS_H_
