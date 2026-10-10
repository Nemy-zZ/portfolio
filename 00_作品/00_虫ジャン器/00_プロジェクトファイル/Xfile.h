//========================================================================================================
//
//Xファイル処理[Xfile.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _XFILE_H_
#define _XFILE_H_

#include "main.h"

//Xファイル
class XFile
{
public:
	typedef struct
	{
		LPD3DXMESH pMesh;   //メッシュ
		LPD3DXBUFFER  pBuffMat;   //マテリアル
		DWORD dwMat;   //マテリアルの数
	}Mesh;   //メッシュ

	typedef struct
	{
		std::string sPass;   //パス
		Mesh mesh;   //メッシュ
	}List;   //リスト

	XFile();   //コンストラクタ
	~XFile();   //デストラクタ

	void Init();   //初期化処理
	void Uninit();   //終了処理
	Mesh Get(std::string sFileName);

private:
	std::vector<XFile::List> m_vList;
};

#endif // !_XFILE_H_
