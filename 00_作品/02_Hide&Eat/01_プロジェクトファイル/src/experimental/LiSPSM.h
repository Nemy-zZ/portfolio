//=============================================================
//
// LiSPSM [LiSPSM.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _LISPSM_H_
#define _LISPSM_H_

#include "component/3d/camera.h"

//------------------------------------------------------------------------------
//! ポイントリストクラス
class	CVec3List {
private:
	std::vector<D3DXVECTOR3>		m_cPointList;

public:
	//! コンストラクタ
	CVec3List() {}
	//! デストラクタ
	~CVec3List() { m_cPointList.clear(); }

	//! ポイントを追加する
	void	Add(D3DXVECTOR3& vec)
	{
		m_cPointList.push_back(vec);
	}

	//! ポイントをクリアする
	void	Clear(void)
	{
		m_cPointList.clear();
	}

	//! ポイントを変換する
	void	Transform(D3DXMATRIX* pMat)
	{
		int		num = m_cPointList.size();
		for (int i = 0; i < num; i++) {
			D3DXVECTOR3		vec = m_cPointList[i];
			D3DXVec3TransformCoord(&vec, &vec, pMat);
			m_cPointList[i] = vec;
		}
	}

	//! AABBを取得する
	void	GetAABB(D3DXVECTOR3* pMax, D3DXVECTOR3* pMin)
	{
		int		num = m_cPointList.size();
		if (num > 0) {
			*pMax = *pMin = m_cPointList[0];
			for (int i = 1; i < num; i++) {
				if (pMax->x < m_cPointList[i].x) { pMax->x = m_cPointList[i].x; }
				else if (pMin->x > m_cPointList[i].x) { pMin->x = m_cPointList[i].x; }
				if (pMax->y < m_cPointList[i].y) { pMax->y = m_cPointList[i].y; }
				else if (pMin->y > m_cPointList[i].y) { pMin->y = m_cPointList[i].y; }
				if (pMax->z < m_cPointList[i].z) { pMax->z = m_cPointList[i].z; }
				else if (pMin->z > m_cPointList[i].z) { pMin->z = m_cPointList[i].z; }
			}
		}
	}

	CVec3List& operator=(const CVec3List& b)
	{
		Clear();
		int		num = b.m_cPointList.size();
		for (int i = 0; i < num; i++) {
			m_cPointList.push_back(b.m_cPointList[i]);
		}
		return *this;
	}

	D3DXVECTOR3& operator[](int n)
	{
		return m_cPointList[n];
	}
};

// LiSPSM
class CLiSPSM
{
public:
    void Init();
    void Uninit();
	void Draw(CCamera* pCamera);
    void DrawShadowMap();
    void DrawReceiver();
    void DrawTexture();
    void CalcLightTrans();

    ID3DXEffect* GetShadowMap() { return m_pShadowMap; }
	LPDIRECT3DTEXTURE9 GetShadowTex() { return m_pShadowTex; }
    LPDIRECT3DSURFACE9& GetBackBuffer() { return m_pBackBuffer; }
    LPDIRECT3DSURFACE9& GetZBuffer() { return m_pZBuffer; }
private:
	void CalcLispMatrix(CVec3List* pList);
	void CalcPointsOfViewBolume(CVec3List* pList, D3DXMATRIX& matView, float fAngle, float fAspect, float fNear, float fFar);
	void CalcUpVector(D3DXVECTOR3* pUp, const D3DXVECTOR3* pView, const D3DXVECTOR3* pLight);
	void ScaleMatrixToFit(D3DXMATRIX* pOut, const D3DXVECTOR3* pMax, const D3DXVECTOR3* pMin);

	CCamera* pTargetCamera;
    ID3DXEffect* m_pShadowMap;                                   // シャドウマップ
    LPDIRECT3DTEXTURE9 m_pShadowTex;                     // シャドウテクスチャ
    LPDIRECT3DVERTEXDECLARATION9 m_pVertDecl0;	    // 頂点宣言
    LPDIRECT3DVERTEXDECLARATION9 m_pVertDecl1;		// 頂点宣言
    LPDIRECT3DSURFACE9 m_pBackBuffer;
    LPDIRECT3DSURFACE9 m_pZBuffer;
    LPDIRECT3DSURFACE9 m_pRenderZ;

    D3DXHANDLE m_aDrawEffHan[8];

	D3DXMATRIX				m_matLightTrans;					// ライト変換行列
	D3DXVECTOR3			m_vLightDir;							// ライトの方向
	D3DXVECTOR3			m_vLightAngle;						// ライトの角度
};

#endif // !_LISPSM_H_
