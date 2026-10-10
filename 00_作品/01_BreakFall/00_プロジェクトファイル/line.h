//==========================================================================================================
//
//線の処理[line.h]
// Author Murata Shinnosuke
//
//==========================================================================================================
#ifndef _LINE_H_
#define _LINE_H_

#include "main.h"

//線
class Line
{
public:
	//コンストラクタ
	Line()
	{
		//メンバ変数を初期化する
		D3DXMatrixIdentity(&m_mtxWorld);
		this->m_pVtxBuffer = nullptr;
		this->m_posStart = { 0.0f, 0.0f, 0.0f };
		this->m_posEnd = { 0.0f, 0.0f, 0.0f };
		this->m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	//デストラクタ
	~Line()
	{
	}

	void Init();   //初期化処理
	void Uninit();   //終了処理
	void Update();   //更新処理
	void Draw();   //描画処理

	//設定処理
	void SetPos(D3DXVECTOR3 posStart, D3DXVECTOR3 posEnd) { this->m_posStart = posStart, this->m_posEnd = posEnd; };   //始点と終点

	//生成処理
	static Line* Create(D3DXCOLOR color)
	{
		//線を生成する
		Line* pLine = new Line;

		//情報を設定する
		pLine->m_color = color;   //色

		//線を返す
		return pLine;
	}

private:
	D3DXMATRIX m_mtxWorld;   //ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;   //頂点バッファ
	D3DXVECTOR3 m_posStart;   //始点
	D3DXVECTOR3 m_posEnd;   //終点
	D3DXCOLOR m_color;   //色
};

#endif // !_LINE_H_