//========================================================================================================
//
//モデル処理[model.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "rendering.h"
#include "Xfile.h"
#include "parts.h"
#include "motion.h"

//モデル
class Model : public Rendering   //(親)コンポーネント
{
public:
	//コンストラクタ
	Model(Object* pObject) : Rendering(pObject)
	{
		//メンバ変数を初期化する
		this->m_vParts.clear();
	}

	//デストラクタ
	virtual ~Model() override
	{
	}

	virtual void Init() override;   //初期化処理
	virtual void Uninit() override;   //終了処理
	virtual void Update() override;   //更新処理
	virtual void Draw() override;   //描画処理
	void LoadParts(std::string sFileName);   //パーツの読み込み処理

	void SetXFile(XFile::Mesh mesh);   //Xファイルの設定処理
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture);

	Parts* GetParts(int nId) { return this->m_vParts[nId]; };   //パーツの取得処理	
	int GetNumParts() { return this->m_vParts.size(); };   //パーツの数の取得処理

private:
	std::vector<Parts*> m_vParts;   //パーツ
};

#endif // !_MODEL_H_
