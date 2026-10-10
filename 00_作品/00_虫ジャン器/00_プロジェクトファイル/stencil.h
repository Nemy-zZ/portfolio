//========================================================================================================
//
//ステンシルの処理[stencil.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _STENCIL_H_
#define _STENCIL_H_

#include "component.h"

//四角形クラス
class Stencil : public Component   //(親)レンダリング
{
public:
	//比較方法の列挙
	enum COMPARE
	{
		IS_ALWAYS = D3DCMP_ALWAYS,   //常に
		IS_EQUAL = D3DCMP_EQUAL,   //等しければ
		IS_GREATEREQUAL = D3DCMP_GREATEREQUAL,   //以上であれば
	};

	//合格時の反映内容の列挙
	enum PASS
	{
		IS_REPLACE = D3DSTENCILOP_REPLACE,   //置き換え
		IS_KEEP = D3DSTENCILOP_KEEP,   //キープ
		IS_INCR = D3DSTENCILOP_INCR,   //増加
		IS_DECR = D3DSTENCILOP_DECR,   //減少
	};

	//コンストラクタ
	Stencil(Object* pObject, int nRef, Stencil::COMPARE compare, Stencil::PASS pass = Stencil::PASS::IS_KEEP) : Component(pObject)
	{
		//メンバ変数を初期化する
		this->m_nRef = nRef;   //比較値
		this->m_compare = compare;   //比較方法
		this->m_pass = pass;   //反映内容
	}

	//デストラクタ
	~Stencil() override
	{
	}

	void Draw() override;   //描画処理

private:
	int m_nRef;   //参照値
	Stencil::COMPARE m_compare;   //比較方法
	Stencil::PASS m_pass;   //反映内容
};

#endif // !_STENCIL_H_
