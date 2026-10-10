//========================================================================================================
//
//ボックス衝突範囲の処理[box.cpp]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "collider.h"
#include "manager.h"
#include "rigidbody.h"
#include "behavior.h"

//========================================================================================================
//初期化処理
//========================================================================================================
void BoxCollider::Init()
{
    for (int i = 0; i < BoxCollider::NUM_LINE; i++)
    {
        //線を生成する
        this->m_pLine[i] = Line::Create({ 1.0f, 0.0f, 0.0f, 1.0f });
        this->m_pLine[i]->Init();   //初期化
    }
}

//========================================================================================================
//終了処理
//========================================================================================================
void BoxCollider::Uninit()
{
    for (int i = 0; i < BoxCollider::NUM_LINE; i++)
    {
        //線を破棄する
        this->m_pLine[i]->Uninit();   //終了処理
        delete this->m_pLine[i];
        this->m_pLine[i] = nullptr;
    }
}

//========================================================================================================
//更新処理
//========================================================================================================
void BoxCollider::Update()
{
    //範囲を更新する
    this->UpdateRange();

    //線の位置の設定処理を行う
    this->UpdateLine();
}

//========================================================================================================
//更新処理
//========================================================================================================
void BoxCollider::Draw()
{
    for (int i = 0; i < BoxCollider::NUM_LINE; i++)
    {
        //線の描画処理を行う
        this->m_pLine[i]->Draw();
    }
}

//============================================================================================================
//衝突時の処理
//============================================================================================================
void BoxCollider::Collision(Collider* pColliderTarget, Physics::CONTACT contact)
{
    //ボックスコライダーに変換する
    BoxCollider* pBoxColliderTarget = (BoxCollider*)pColliderTarget;

    //衝突位置によって処理を変える
    switch (contact)
    {
    case Physics::CONTACT::IS_TOP:   //上
        //トリガーか確認する
        if (pBoxColliderTarget->GetTrigger() == false)
        {//トリガーじゃない場合
            //リジットボディを確認する
            Rigidbody* pRigidbodyTarget = pBoxColliderTarget->GetObj()->GetComponent<Rigidbody>();
            if (pRigidbodyTarget == nullptr)
            {//リジットボディが付いていない場合
                //新しい位置を計算する
                D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                centerNew.y = pBoxColliderTarget->GetRange().posMax.y + (this->GetSize().y / 2);

                //移動量を打ち消す
                Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                pRigidbody->AddMove({ 0.0f, -pRigidbody->GetMove().y,0.0f });   //移動量

                //位置を修正する
                this->SetCenter(centerNew);
            }
            else
            {//付いている場合
                //質量を確認する
                if (pBoxColliderTarget->GetMass() < this->GetMass())
                {//対象の方が軽い場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = pBoxColliderTarget->GetCenter();   //現在の位置
                    centerNew.y = this->m_range.posMin.y - (pBoxColliderTarget->GetSize().y / 2);

                    //移動量を打ち消す
                    pRigidbodyTarget->AddMove({ 0.0f, -pRigidbodyTarget->GetMove().y,0.0f });   //移動量

                    //位置を修正する
                    pBoxColliderTarget->SetCenter(centerNew);
                }
                else
                {//軽くない場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                    centerNew.y = pBoxColliderTarget->GetRange().posMax.y + (this->GetSize().y / 2);

                    //移動量を打ち消す
                    Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                    pRigidbody->AddMove({ 0.0f, -pRigidbody->GetMove().y,0.0f });   //移動量

                    //位置を修正する
                    this->SetCenter(centerNew);
                }
            }
        }

        //処理を抜け出す
        break;

    case Physics::CONTACT::IS_BOTTOM:   //下
        //トリガーか確認する
        if (pBoxColliderTarget->GetTrigger() == false)
        {//トリガーじゃない場合
            //リジットボディを確認する
            Rigidbody* pRigidbodyTarget = pBoxColliderTarget->GetObj()->GetComponent<Rigidbody>();
            if (pRigidbodyTarget == nullptr)
            {//リジットボディが付いていない場合
                //新しい位置を計算する
                D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                centerNew.y = pBoxColliderTarget->GetRange().posMin.y - (this->GetSize().y / 2);

                //移動量を打ち消す
                Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                pRigidbody->AddMove({ 0.0f, -pRigidbody->GetMove().y, 0.0f });

                //位置を修正する
                this->SetCenter(centerNew);
            }
            else
            {//付いている場合
                //質量を確認する
                if (pBoxColliderTarget->GetMass() < this->GetMass())
                {//対象の方が軽い場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = pBoxColliderTarget->GetCenter();   //現在の位置
                    centerNew.y = this->m_range.posMax.y + (pBoxColliderTarget->GetSize().y / 2);

                    //移動量を打ち消す
                    pRigidbodyTarget->AddMove({ 0.0f, -pRigidbodyTarget->GetMove().y, 0.0f });

                    //位置を修正する
                    pBoxColliderTarget->SetCenter(centerNew);
                }
                else
                {//軽くない場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                    centerNew.y = pBoxColliderTarget->GetRange().posMin.y - (this->GetSize().y / 2);

                    //移動量を打ち消す
                    Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                    pRigidbody->AddMove({ 0.0f, -pRigidbody->GetMove().y, 0.0f });

                    //位置を修正する
                    this->SetCenter(centerNew);
                }
            }
        }

        //処理を抜け出す
        break;

    case::Physics::CONTACT::IS_RIGHT:   //右
        //トリガーか確認する
        if (pBoxColliderTarget->GetTrigger() == false)
        {//トリガーじゃない場合
            //リジットボディを確認する
            Rigidbody* pRigidbodyTarget = pBoxColliderTarget->GetObj()->GetComponent<Rigidbody>();
            if (pRigidbodyTarget == nullptr)
            {//リジットボディが付いていない場合
                //新しい位置を計算する
                D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                centerNew.x = pBoxColliderTarget->GetRange().posMax.x + (this->GetSize().x / 2);

                //移動量を打ち消す
                Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                pRigidbody->AddMove({ -pRigidbody->GetMove().x, 0.0f, 0.0f });

                //位置を修正する
                this->SetCenter(centerNew);
            }
            else
            {//付いている場合
                //質量を確認する
                if (pBoxColliderTarget->GetMass() < this->GetMass())
                {//対象の方が軽い場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = pBoxColliderTarget->GetCenter();   //現在の位置
                    centerNew.x = this->m_range.posMin.x - (pBoxColliderTarget->GetSize().x / 2);

                    //移動量を打ち消す
                    pRigidbodyTarget->AddMove({ -pRigidbodyTarget->GetMove().x, 0.0f, 0.0f });

                    //位置を修正する
                    pBoxColliderTarget->SetCenter(centerNew);
                }
                else
                {//軽くない場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                    centerNew.x = pBoxColliderTarget->GetRange().posMax.x + (this->GetSize().x / 2);

                    //移動量を打ち消す
                    Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                    pRigidbody->AddMove({ -pRigidbody->GetMove().x, 0.0f, 0.0f });

                    //位置を修正する
                    this->SetCenter(centerNew);
                }
            }
        }

        //処理を抜け出す
        break;

    case::Physics::CONTACT::IS_LEFT:   //左
        //トリガーか確認する
        if (pBoxColliderTarget->GetTrigger() == false)
        {//トリガーじゃない場合
            //リジットボディを確認する
            Rigidbody* pRigidbodyTarget = pBoxColliderTarget->GetObj()->GetComponent<Rigidbody>();
            if (pRigidbodyTarget == nullptr)
            {//リジットボディが付いていない場合
                //新しい位置を計算する
                D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                centerNew.x = pBoxColliderTarget->GetRange().posMin.x - (this->GetSize().x / 2);

                //移動量を打ち消す
                Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                pRigidbody->AddMove({ -pRigidbody->GetMove().x, 0.0f, 0.0f });

                //位置を修正する
                this->SetCenter(centerNew);
            }
            else
            {//付いている場合
                //質量を確認する
                if (pBoxColliderTarget->GetMass() < this->GetMass())
                {//対象の方が軽い場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = pBoxColliderTarget->GetCenter();   //現在の位置
                    centerNew.x = this->m_range.posMax.x + (pBoxColliderTarget->GetSize().x / 2);

                    //移動量を打ち消す
                    pRigidbodyTarget->AddMove({ -pRigidbodyTarget->GetMove().x, 0.0f, 0.0f });

                    //位置を修正する
                    pBoxColliderTarget->SetCenter(centerNew);
                }
                else
                {//軽くない場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                    centerNew.x = pBoxColliderTarget->GetRange().posMin.x - (this->GetSize().x / 2);

                    //移動量を打ち消す
                    Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                    pRigidbody->AddMove({ -pRigidbody->GetMove().x, 0.0f, 0.0f });

                    //位置を修正する
                    this->SetCenter(centerNew);
                }
            }
        }

        //処理を抜け出す
        break;

    case::Physics::CONTACT::IS_BACK:   //後
        //トリガーか確認する
        if (pBoxColliderTarget->GetTrigger() == false)
        {//トリガーじゃない場合
            //リジットボディを確認する
            Rigidbody* pRigidbodyTarget = pBoxColliderTarget->GetObj()->GetComponent<Rigidbody>();
            if (pRigidbodyTarget == nullptr)
            {//リジットボディが付いていない場合
                //新しい位置を計算する
                D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                centerNew.z = pBoxColliderTarget->GetRange().posMax.z + (this->GetSize().z / 2);

                //移動量を打ち消す
                Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                pRigidbody->AddMove({ 0.0f, 0.0f, -pRigidbody->GetMove().z });

                //位置を修正する
                this->SetCenter(centerNew);
            }
            else
            {//付いている場合
                //質量を確認する
                if (pBoxColliderTarget->GetMass() < this->GetMass())
                {//対象の方が軽い場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = pBoxColliderTarget->GetCenter();   //現在の位置
                    centerNew.z = this->m_range.posMin.z - (pBoxColliderTarget->GetSize().z / 2);

                    //移動量を打ち消す
                    pRigidbodyTarget->AddMove({ 0.0f, 0.0f, -pRigidbodyTarget->GetMove().z });

                    //位置を修正する
                    pBoxColliderTarget->SetCenter(centerNew);
                }
                else
                {//軽くない場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                    centerNew.z = pBoxColliderTarget->GetRange().posMax.z + (this->GetSize().z / 2);

                    //移動量を打ち消す
                    Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                    pRigidbody->AddMove({ 0.0f, 0.0f, -pRigidbody->GetMove().z });

                    //位置を修正する
                    this->SetCenter(centerNew);
                }
            }
        }

        //処理を抜け出す
        break;

    case::Physics::CONTACT::IS_FRONT:   //前
        //トリガーか確認する
        if (pBoxColliderTarget->GetTrigger() == false)
        {//トリガーじゃない場合
            //リジットボディを確認する
            Rigidbody* pRigidbodyTarget = pBoxColliderTarget->GetObj()->GetComponent<Rigidbody>();
            if (pRigidbodyTarget == nullptr)
            {//リジットボディが付いていない場合
                //新しい位置を計算する
                D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                centerNew.z = pBoxColliderTarget->GetRange().posMin.z - (this->GetSize().z / 2);

                //移動量を打ち消す
                Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                pRigidbody->AddMove({ 0.0f, 0.0f, -pRigidbody->GetMove().z });

                //位置を修正する
                this->SetCenter(centerNew);
            }
            else
            {//付いている場合
                //質量を確認する
                if (pBoxColliderTarget->GetMass() < this->GetMass())
                {//対象の方が軽い場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = pBoxColliderTarget->GetCenter();   //現在の位置
                    centerNew.z = this->m_range.posMax.z + (pBoxColliderTarget->GetSize().z / 2);

                    //移動量を打ち消す
                    pRigidbodyTarget->AddMove({ 0.0f, 0.0f, -pRigidbodyTarget->GetMove().z });

                    //位置を修正する
                    pBoxColliderTarget->SetCenter(centerNew);
                }
                else
                {//軽くない場合
                    //新しい位置を計算する
                    D3DXVECTOR3 centerNew = this->GetCenter();   //現在の位置
                    centerNew.z = pBoxColliderTarget->GetRange().posMin.z - (this->GetSize().z / 2);

                    //移動量を打ち消す
                    Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                    pRigidbody->AddMove({ 0.0f, 0.0f, -pRigidbody->GetMove().z });

                    //位置を修正する
                    this->SetCenter(centerNew);
                }
            }
        }

        //処理を抜け出す
        break;

    case Physics::CONTACT::IS_IN:   //中
        //トリガーか確認する
        if (pBoxColliderTarget->GetTrigger() == false)
        {//トリガーじゃない場合
            //方向を計算する
            D3DXVECTOR3 vecTarget = { pBoxColliderTarget->GetCenter().x - this->GetCenter().x, 0.0f, pBoxColliderTarget->GetCenter().z - this->GetCenter().z };   //ベクトル
            float fDirection = atan2f(vecTarget.x, vecTarget.z);

            //リジットボディを確認する
            Rigidbody* pRigidbodyTarget = pBoxColliderTarget->GetObj()->GetComponent<Rigidbody>();
            if (pRigidbodyTarget == nullptr)
            {//リジットボディが付いていない場合
                //移動量を打ち消す
                Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                pRigidbody->AddMove({ -pRigidbody->GetMove() });

                //移動量を加算する
                pRigidbody->AddMove({ sinf(fDirection - D3DX_PI), 0.0f, cosf(fDirection - D3DX_PI) });
            }
            else
            {//付いている場合
                //質量を確認する
                if (pBoxColliderTarget->GetMass() < this->GetMass())
                {//対象の方が軽い場合
                    //移動量を打ち消す
                    pRigidbodyTarget->AddMove({ -pRigidbodyTarget->GetMove() });

                    //移動量を加算する
                    pRigidbodyTarget->AddMove({ sinf(fDirection), 0.0f, cosf(fDirection) });
                }
                else if (pBoxColliderTarget->GetMass() > this->GetMass())
                {//対象より重たい場合
                    //移動量を打ち消す
                    Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                    pRigidbody->AddMove({ -pRigidbody->GetMove() });

                    //移動量を加算する
                    pRigidbody->AddMove({ sinf(fDirection - D3DX_PI), 0.0f, cosf(fDirection - D3DX_PI) });
                }
                else
                {//同じ場合
                    //移動量を加算する
                    Rigidbody* pRigidbody = this->GetObj()->GetComponent<Rigidbody>();
                    pRigidbody->AddMove({ sinf(fDirection - D3DX_PI), 0.0f, cosf(fDirection - D3DX_PI) });

                    //移動量を加算する
                    pRigidbodyTarget->AddMove({ sinf(fDirection), 0.0f, cosf(fDirection) });
                }
            }
        }

        //処理を抜け出す
        break;
    }
    //挙動があるか確認する
    Behavior* pBehavior = this->GetObj()->GetComponent<Behavior>();
    if (pBehavior != nullptr)
    {
        //オブジェクトの衝突時の処理を行う
        pBehavior->Collision(pColliderTarget->GetObj(), contact);
        this->SetCollision(true);
    }

    //挙動があるか確認する
    Behavior* pBehaviorTarget = pColliderTarget->GetObj()->GetComponent<Behavior>();
    if (pBehaviorTarget != nullptr)
    {
        //オブジェクトの衝突時の処理を行う
        pBehaviorTarget->Collision(this->GetObj(), contact);
        pColliderTarget->SetCollision(true);
    }
}

//========================================================================================================
//範囲の更新処理
//========================================================================================================
void BoxCollider::UpdateRange()
{
    D3DXVECTOR3 pos = this->GetCenter();
    D3DXVECTOR3 posOld = this->GetCenterOld();

    //衝突範囲を計算する
    this->m_range.posMax =
    {
        pos.x + (this->m_size.x / 2),
        pos.y + (this->m_size.y / 2),
        pos.z + (this->m_size.z / 2),
    };   //最大範囲

    this->m_range.posMaxOld =
    {
        posOld.x + (this->m_size.x / 2),
        posOld.y + (this->m_size.y / 2),
        posOld.z + (this->m_size.z / 2),
    };   //前回の最大範囲

    this->m_range.posMin =
    {
        pos.x - (this->m_size.x / 2),
        pos.y - (this->m_size.y / 2),
        pos.z - (this->m_size.z / 2),
    };   //最小範囲

    this->m_range.posMinOld =
    {
        posOld.x - (this->m_size.x / 2),
        posOld.y - (this->m_size.y / 2),
        posOld.z - (this->m_size.z / 2),
    };   //前回の最小範囲
}

//========================================================================================================
//線の位置の設定処理
//========================================================================================================
void BoxCollider::UpdateLine()
{
    //線の位置を設定する
    //下4辺の設定
    this->m_pLine[0]->SetPos(
        {
            this->m_range.posMin.x,
            this->m_range.posMin.y,
            this->m_range.posMin.z
        },
        {
            this->m_range.posMax.x,
            this->m_range.posMin.y,
            this->m_range.posMin.z
        });

    this->m_pLine[1]->SetPos(
        {
            this->m_range.posMax.x,
            this->m_range.posMin.y,
            this->m_range.posMin.z
        },
        {
            this->m_range.posMax.x,
            this->m_range.posMin.y,
            this->m_range.posMax.z
        });

    this->m_pLine[2]->SetPos(
        {
            this->m_range.posMax.x,
            this->m_range.posMin.y,
            this->m_range.posMax.z
        },
        {
            this->m_range.posMin.x,
            this->m_range.posMin.y,
            this->m_range.posMax.z
        });

    this->m_pLine[3]->SetPos(
        {
            this->m_range.posMin.x,
            this->m_range.posMin.y,
            this->m_range.posMax.z
        },
        {
            this->m_range.posMin.x,
            this->m_range.posMin.y,
            this->m_range.posMin.z
        });

    //中4辺の設定
    this->m_pLine[4]->SetPos(
        {
            this->m_range.posMin.x,
            this->m_range.posMin.y,
            this->m_range.posMin.z
        },
        {
            this->m_range.posMin.x,
            this->m_range.posMax.y,
            this->m_range.posMin.z
        });

    this->m_pLine[5]->SetPos(
        {
            this->m_range.posMax.x,
            this->m_range.posMin.y,
            this->m_range.posMin.z
        },
        {
            this->m_range.posMax.x,
            this->m_range.posMax.y,
            this->m_range.posMin.z
        });

    this->m_pLine[6]->SetPos(
        {
            this->m_range.posMax.x,
            this->m_range.posMin.y,
            this->m_range.posMax.z
        },
        {
            this->m_range.posMax.x,
            this->m_range.posMax.y,
            this->m_range.posMax.z
        });

    this->m_pLine[7]->SetPos(
        {
            this->m_range.posMin.x,
            this->m_range.posMin.y,
            this->m_range.posMax.z
        },
        {
            this->m_range.posMin.x,
            this->m_range.posMax.y,
            this->m_range.posMax.z
        });

    //上4辺の設定
    this->m_pLine[8]->SetPos(
        {
            this->m_range.posMin.x,
            this->m_range.posMax.y,
            this->m_range.posMin.z
        },
        {
            this->m_range.posMax.x,
            this->m_range.posMax.y,
            this->m_range.posMin.z
        });

    this->m_pLine[9]->SetPos(
        {
            this->m_range.posMax.x,
            this->m_range.posMax.y,
            this->m_range.posMin.z
        },
        {
            this->m_range.posMax.x,
            this->m_range.posMax.y,
            this->m_range.posMax.z
        });

    this->m_pLine[10]->SetPos(
        {
            this->m_range.posMax.x,
            this->m_range.posMax.y,
            this->m_range.posMax.z
        },
        {
            this->m_range.posMin.x,
            this->m_range.posMax.y,
            this->m_range.posMax.z
        });

    this->m_pLine[11]->SetPos(
        {
            this->m_range.posMin.x,
            this->m_range.posMax.y,
            this->m_range.posMax.z
        },
        {
            this->m_range.posMin.x,
            this->m_range.posMax.y,
            this->m_range.posMin.z
        });

    for (int i = 0; i < BoxCollider::NUM_LINE; i++)
    {
        //線の更新処理を行う
        this->m_pLine[i]->Update();
    }
}