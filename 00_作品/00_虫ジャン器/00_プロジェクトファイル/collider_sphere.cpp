//========================================================================================================
//
//球体衝突範囲の処理[collider_sphere.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "collider.h"
#include "benlib.h"
#include "manager.h"
#include "transform.h"
#include "rigidbody.h"
#include "behavior.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void SphereCollider::Init()
{
}

//========================================================================================================
//終了処理
//========================================================================================================
void SphereCollider::Uninit()
{
}

//========================================================================================================
//更新処理
//========================================================================================================
void SphereCollider::Update()
{
    //トリガーフラグを確認する
    if (this->GetTrigger() == false)
    {
        //コンポーネントを確認する
        if (this->GetObj()->GetComponent<Rigidbody>() != nullptr)
        {//リジットボディが付いている場合
            //当たり判定の演算回数分繰り返す
            for (int i = 0; i < Collider::CALC_PHYSICS; i++)
            {
                //当たり判定の演算を行う
                Physics::Get()->Calc(this);
            }
        }
    }
}

//========================================================================================================
//更新処理
//========================================================================================================
void SphereCollider::Draw()
{

}

//============================================================================================================
//衝突時の処理
//============================================================================================================
void SphereCollider::Collision(Collider* pColliderTarget, Physics::CONTACT contact)
{
    //球体コライダーに変換する
    SphereCollider* pSphereColliderTarget = (SphereCollider*)pColliderTarget;

    //自身と対象の距離を計算する
    D3DXVECTOR3 dis = Benlib::Distance(
        this->GetObj()->GetComponent<Transform>()->GetPos(), 
        pSphereColliderTarget->GetObj()->GetComponent<Transform>()->GetPos());

    //入り込んでいるか確認する
    float fRadiusSum = this->m_fRadius + pSphereColliderTarget->GetRadius();
    if (fRadiusSum < dis.x && fRadiusSum < dis.y)
    {
        //方向を計算する
        D3DXVECTOR3 dire = Benlib::Direction(
            this->GetObj()->GetComponent<Transform>()->GetPos(),
            pSphereColliderTarget->GetObj()->GetComponent<Transform>()->GetPos());

        //位置を修正する
        D3DXVECTOR3 posNew =
        {
            pSphereColliderTarget->GetObj()->GetComponent<Transform>()->GetPos().x - fRadiusSum * sinf(dire.x) * sinf(dire.y),
            pSphereColliderTarget->GetObj()->GetComponent<Transform>()->GetPos().y - fRadiusSum * cosf(dire.x),
            pSphereColliderTarget->GetObj()->GetComponent<Transform>()->GetPos().z - fRadiusSum * sinf(dire.x) * cosf(dire.y),
        };
    }

    //オブジェクトの衝突時の処理を行う
    this->GetObj()->GetComponent<Behavior>()->Collision(pColliderTarget->GetObj(), contact);
    pColliderTarget->GetObj()->GetComponent<Behavior>()->Collision(this->GetObj(), contact);
}