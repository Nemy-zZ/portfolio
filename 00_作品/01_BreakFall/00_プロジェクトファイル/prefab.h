//==========================================================================================================
//
//プレハブ？の処理[prefab.h]
// Author MurataShinnosuke
//
//==========================================================================================================
#ifndef _PREFAB_H_
#define _PREFAB_H_

#include "object.h"
#include "manager.h"

#include "camera.h"
#include "collider.h"
#include "dire_light.h"
#include "motioneer.h"
#include "rigidbody.h"
#include "stencil.h"
#include "transform.h"

#include "mask.h"
#include "menu.h"
#include "model.h"
#include "number.h"
#include "plane.h"
#include "square.h"

#include "assist_bar.h"
#include "ball.h"
#include "block.h"
#include "block_spawner.h"
#include "fade.h"
#include "pause_menu.h"
#include "score.h"
#include "stick.h"
#include "timer.h"
#include "title_menu.h"

#include "item.h"

////プレハブ
//class Prefab
//{
//public:
//	//コンストラクタ
//	Prefab()
//	{
//		this->m_vPrefab.clear();   //コンテナ
//	}
//
//	//デストラクタ
//	~Prefab()
//	{
//	}
//
//	void AddPrefab(Object* pObejct) { this->m_vPrefab.push_back(pObejct); };   //プレハブの追加
//
//
//private:
//	std::vector<Object*> m_vPrefab;   //コンテナ
//};

//アシスト棒
class AssistBarPrefab
{
public:
	//生成処理
	static Object* Create()
	{
		//背景を生成する
		Object* pPrefab = new Object("AssistBar");
		pPrefab->SetTag(Object::TAG::IS_STICK);

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetScale({ 2.5f, 0.1f, 0.1f });
		pTransform->SetPos({ 0.0f, -350.0f, 0.0f });
		pPrefab->AddComponent(pTransform);

		//四角形レンダリングを追加する
		PlaneY* pPlaneY = new PlaneY(pPrefab);
		//pPlaneY->SetTexture(Manager::Get()->GetTexture()->Get("ball_00.png"));
		pPrefab->AddComponent(pPlaneY);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ 500.0f, 20.0f, 5.0f });
		pPrefab->AddComponent(pBoxCollider);

		//棒の挙動を追加する
		AssistBar* pAssistBar = new AssistBar(pPrefab);
		pPrefab->AddComponent(pAssistBar);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//背景
class Background2DPrefab
{
public:
	//生成処理
	static Object* Create(LPDIRECT3DTEXTURE9 pTexture)
	{
		//背景を生成する
		Object* pPrefab = new Object("Background2D");

		//トランスフォームを追加する
		Transform2D* pTransform2D = new Transform2D(pPrefab);
		pTransform2D->SetSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
		pPrefab->AddComponent(pTransform2D);

		//四角形レンダリングを追加する
		Square* pSquare = new Square(pPrefab);
		pSquare->SetTexture(pTexture);
		pPrefab->AddComponent(pSquare);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//背景
class Background3DPrefab
{
public:
	//生成処理
	static Object* Create(LPDIRECT3DTEXTURE9 pTexture)
	{
		//背景を生成する
		Object* pPrefab = new Object("Background3D");

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos({ 0.0f, 0.0f, 10.0f });
		pTransform->SetScale({ 6.4f, 3.6f, 1.0f });
		pPrefab->AddComponent(pTransform);

		//四角形レンダリングを追加する
		PlaneY* pPlaneY = new PlaneY(pPrefab);
		pPlaneY->SetTexture(pTexture);
		pPrefab->AddComponent(pPlaneY);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//ボール
class BallPrefab
{
public:
	//生成処理
	static Object* Create()
	{
		//爆発を生成する
		Object* pPrefab = new Object("Ball");
		pPrefab->SetTag(Object::TAG::IS_BALL);

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetScale({ 0.1f, 0.1f, 0.1f });
		pPrefab->AddComponent(pTransform);

		//四角形レンダリングを追加する
		PlaneY* pPlaneY = new PlaneY(pPrefab);
		pPlaneY->SetTexture(Manager::Get()->GetTexture()->Get("block_00.png"));
		pPrefab->AddComponent(pPlaneY);

		//リジットボディを追加する
		Rigidbody* pRigidbody = new Rigidbody(pPrefab);
		pRigidbody->SetGravity(0.0f);
		pRigidbody->SetFriction(0.0f);
		pRigidbody->SetFrictionAir(0.0f);
		pPrefab->AddComponent(pRigidbody);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ 20.0f, 20.0f, 20.0f });
		pPrefab->AddComponent(pBoxCollider);

		//ボールの挙動を追加する
		Ball* pBall = new Ball(pPrefab);
		pPrefab->AddComponent(pBall);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//ブロック
class BlockPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos)
	{
		//背景を生成する
		Object* pPrefab = new Object("Block");
		pPrefab->SetTag(Object::TAG::IS_BLOCK);

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);   //位置の設定
		pTransform->SetScale({ 0.5f, 0.25f, 1.0f });
		pPrefab->AddComponent(pTransform);

		//四角形レンダリングを追加する
		PlaneY* pPlaneY = new PlaneY(pPrefab);
		pPlaneY->SetTexture(Manager::Get()->GetTexture()->Get("block_00.png"));
		pPrefab->AddComponent(pPlaneY);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize(Block::SIZE);
		pPrefab->AddComponent(pBoxCollider);

		//ボールの挙動を追加する
		Block* pBlock = new Block(pPrefab);
		pPrefab->AddComponent(pBlock);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//ブロックスポナー
class BlockSpawnerPrefab
{
public:
	//生成処理
	static Object* Create()
	{
		//背景を生成する
		Object* pPrefab = new Object("BlockSpawner");
		pPrefab->SetTag(Object::TAG::IS_BLOCK);

		//ブロックスポナーの挙動を追加する
		BlockSpawner* pBlockSpawner = new BlockSpawner(pPrefab);
		pPrefab->AddComponent(pBlockSpawner);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//玉追加アイテム
class AddBallItemPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos)
	{
		//生成する
		Object* pPrefab = new Object("AddBallItem");
		pPrefab->SetTag(Object::TAG::IS_ITEM);

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);   //位置の設定
		pTransform->SetScale({ 0.25f, 0.25f, 1.0f });
		pPrefab->AddComponent(pTransform);

		//四角形レンダリングを追加する
		PlaneY* pPlaneY = new PlaneY(pPrefab);
		pPlaneY->SetTexture(Manager::Get()->GetTexture()->Get("addball_item.png"));
		pPrefab->AddComponent(pPlaneY);

		//リジットボディを追加する
		Rigidbody* pRigidbody = new Rigidbody(pPrefab);
		pRigidbody->SetGravity(0.0f);
		pRigidbody->SetFriction(0.0f);
		pRigidbody->SetFrictionAir(0.0f);
		pPrefab->AddComponent(pRigidbody);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ Item::SIZE, Item::SIZE, 5.0f });
		pBoxCollider->SetMass(4);
		pPrefab->AddComponent(pBoxCollider);

		//アイテムの挙動を追加する
		AddBallItem* pItem = new AddBallItem(pPrefab);
		pPrefab->AddComponent(pItem);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//玉追加アイテム
class ClearBlockItemPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos)
	{
		//生成する
		Object* pPrefab = new Object("AddBallItem");
		pPrefab->SetTag(Object::TAG::IS_ITEM);

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);   //位置の設定
		pTransform->SetScale({ 0.25f, 0.25f, 1.0f });
		pPrefab->AddComponent(pTransform);

		//四角形レンダリングを追加する
		PlaneY* pPlaneY = new PlaneY(pPrefab);
		pPlaneY->SetTexture(Manager::Get()->GetTexture()->Get("clear_item.png"));
		pPrefab->AddComponent(pPlaneY);

		//リジットボディを追加する
		Rigidbody* pRigidbody = new Rigidbody(pPrefab);
		pRigidbody->SetGravity(0.0f);
		pRigidbody->SetFriction(0.0f);
		pRigidbody->SetFrictionAir(0.0f);
		pPrefab->AddComponent(pRigidbody);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ Item::SIZE, Item::SIZE, 5.0f });
		pBoxCollider->SetMass(4);
		pPrefab->AddComponent(pBoxCollider);

		//アイテムの挙動を追加する
		ClearBlockItem* pItem = new ClearBlockItem(pPrefab);
		pPrefab->AddComponent(pItem);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//玉追加アイテム
class AssistBarItemPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos)
	{
		//生成する
		Object* pPrefab = new Object("AddBallItem");
		pPrefab->SetTag(Object::TAG::IS_ITEM);

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);   //位置の設定
		pTransform->SetScale({ 0.25f, 0.25f, 1.0f });
		pPrefab->AddComponent(pTransform);

		//四角形レンダリングを追加する
		PlaneY* pPlaneY = new PlaneY(pPrefab);
		pPlaneY->SetTexture(Manager::Get()->GetTexture()->Get("supportbar_item.png"));
		pPrefab->AddComponent(pPlaneY);

		//リジットボディを追加する
		Rigidbody* pRigidbody = new Rigidbody(pPrefab);
		pRigidbody->SetGravity(0.0f);
		pRigidbody->SetFriction(0.0f);
		pRigidbody->SetFrictionAir(0.0f);
		pPrefab->AddComponent(pRigidbody);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ Item::SIZE, Item::SIZE, 5.0f });
		pBoxCollider->SetMass(4);
		pPrefab->AddComponent(pBoxCollider);

		//アイテムの挙動を追加する
		AssistBarItem* pItem = new AssistBarItem(pPrefab);
		pPrefab->AddComponent(pItem);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//玉追加アイテム
class AddHeartItemPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos)
	{
		//生成する
		Object* pPrefab = new Object("AddBallItem");
		pPrefab->SetTag(Object::TAG::IS_ITEM);

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);   //位置の設定
		pTransform->SetScale({ 0.25f, 0.25f, 1.0f });
		pPrefab->AddComponent(pTransform);

		//四角形レンダリングを追加する
		PlaneY* pPlaneY = new PlaneY(pPrefab);
		pPlaneY->SetTexture(Manager::Get()->GetTexture()->Get("heal_item.png"));
		pPrefab->AddComponent(pPlaneY);

		//リジットボディを追加する
		Rigidbody* pRigidbody = new Rigidbody(pPrefab);
		pRigidbody->SetGravity(0.0f);
		pRigidbody->SetFriction(0.0f);
		pRigidbody->SetFrictionAir(0.0f);
		pPrefab->AddComponent(pRigidbody);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ Item::SIZE, Item::SIZE, 5.0f });
		pBoxCollider->SetMass(4);
		pPrefab->AddComponent(pBoxCollider);

		//アイテムの挙動を追加する
		AddHeartItem* pItem = new AddHeartItem(pPrefab);
		pPrefab->AddComponent(pItem);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//カメラ
class CameraPrefab
{
public:
	//生成処理
	static Object* Create()
	{
		//カメラを生成する
		Object* pPrefab = new Object("Camera");

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos({ 0.0f, 0.0f, -500.0f });
		pPrefab->AddComponent(pTransform);

		//カメラを追加する
		Camera* pCamera = new Camera(pPrefab);
		pPrefab->AddComponent(pCamera);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//ライト
class DireLightPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 rot, D3DXCOLOR color)
	{
		//ライトを生成する
		Object* pPrefab = new Object("DirectionalLight");

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetRot(rot);
		pPrefab->AddComponent(pTransform);

		//ライトを追加する
		DireLight* pDireLight = new DireLight(pPrefab);
		pDireLight->SetColor(color);   //色
		pPrefab->AddComponent(pDireLight);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//フェード
class FadePrefab
{
public:
	//生成処理
	static Object* Create()
	{
		//背景を生成する
		Object* pPrefab = new Object("Fade");

		//トランスフォームを追加する
		Transform2D* pTransform2D = new Transform2D(pPrefab);
		pTransform2D->SetSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
		pPrefab->AddComponent(pTransform2D);

		//四角形レンダリングを追加する
		Square* pSquare = new Square(pPrefab);
		pSquare->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });   //色
		pPrefab->AddComponent(pSquare);

		//フェードの挙動を追加する
		Fade* pFade = new Fade(pPrefab);
		pPrefab->AddComponent(pFade);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//ポーズメニュー
class PauseMenuPrefab
{
public:
	//生成処理
	static Object* Create()
	{
		//ポーズメニューを生成する
		Object* pPrefab = new Object("PauseMenu");
		pPrefab->SetActive(false);   //非アクティブ

		//トランスフォームを追加する
		Transform2D* pTransform2D = new Transform2D(pPrefab);
		pTransform2D->SetPos({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });   //位置
		pTransform2D->SetSize(PauseMenu::SIZE_MENU);   //サイズ
		pPrefab->AddComponent(pTransform2D);

		//メニューレンダリングを追加する
		Menu* pMenu = new Menu(pPrefab);
		pMenu->SetTexture(Manager::Get()->GetTexture()->Get("pause_menu_00.png"));
		pMenu->SetSelectBox(PauseMenu::MENU::IS_MAX);
		pMenu->SetGap(1.0f);
		pPrefab->AddComponent(pMenu);

		//ポーズメニューの挙動を追加する
		PauseMenu* pPauseMenu = new PauseMenu(pPrefab);
		pPrefab->AddComponent(pPauseMenu);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//スコア
class ScorePrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR2 pos = {0.0f, 0.0f}, D3DXVECTOR2 size = {500.0f, 100.0f})
	{
		//スコアを生成する
		Object* pPrefab = new Object("Score");

		//トランスフォームを追加する
		Transform2D* pTransform2D = new Transform2D(pPrefab);
		pTransform2D->SetPos(pos);							//位置
		pTransform2D->SetSize(size);							 //サイズ
		pPrefab->AddComponent(pTransform2D);

		//数字レンダリングを追加する
		Number* pNumber = new Number(pPrefab);
		pNumber->SetTexture(Manager::Get()->GetTexture()->Get("number_00.png"));
		pNumber->SetDigit(8);
		pNumber->SetNumber(Timer::START_TIMER);
		pPrefab->AddComponent(pNumber);

		//スコアの挙動を追加する
		Score* pScore = new Score(pPrefab);
		pPrefab->AddComponent(pScore);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//棒
class StickPrefab
{
public:
	//生成処理
	static Object* Create()
	{
		//背景を生成する
		Object* pPrefab = new Object("Stick");
		pPrefab->SetTag(Object::TAG::IS_STICK);

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetScale({ 1.5f, 0.1f, 0.1f });
		pTransform->SetPos({ 0.0f, -300.0f, 0.0f });
		pPrefab->AddComponent(pTransform);

		//四角形レンダリングを追加する
		PlaneY* pPlaneY = new PlaneY(pPrefab);
		//pPlaneY->SetTexture(Manager::Get()->GetTexture()->Get("ball_00.png"));
		pPrefab->AddComponent(pPlaneY);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ 300.0f, 20.0f, 5.0f });
		pPrefab->AddComponent(pBoxCollider);

		//棒の挙動を追加する
		Stick* pStick = new Stick(pPrefab);
		pPrefab->AddComponent(pStick);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//タイマー
class TimerPrefab
{
public:
	//生成処理
	static Object* Create()
	{
		//タイマーを生成する
		Object* pPrefab = new Object("Timer");

		//トランスフォームを追加する
		Transform2D* pTransform2D = new Transform2D(pPrefab);
		pTransform2D->SetPos({ 50.0f, 80.0f });   //位置
		pTransform2D->SetSize({ 200.0f, 100.0f });   //サイズ
		pPrefab->AddComponent(pTransform2D);

		//数字レンダリングを追加する
		Number* pNumber = new Number(pPrefab);
		pNumber->SetTexture(Manager::Get()->GetTexture()->Get("number_00.png"));
		pNumber->SetDigit(3);
		pNumber->SetNumber(Timer::START_TIMER);
		pPrefab->AddComponent(pNumber);

		//タイマーの挙動を追加する
		Timer* pTimer = new Timer(pPrefab);
		pPrefab->AddComponent(pTimer);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//タイトルメニュー
class TitleMenuPrefab
{
public:
	//生成処理
	static Object* Create()
	{
		//タイトルメニューを生成する
		Object* pPrefab = new Object("TitleMenu");

		//トランスフォームを追加する
		Transform2D* pTransform2D = new Transform2D(pPrefab);
		pTransform2D->SetPos({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2  + 100.0f});   //位置
		pTransform2D->SetSize(TitleMenu::SIZE_MENU);   //サイズ
		pPrefab->AddComponent(pTransform2D);

		//メニューレンダリングを追加する
		Menu* pMenu = new Menu(pPrefab);
		pMenu->SetTexture(Manager::Get()->GetTexture()->Get("title_menu_00.png"));
		pMenu->SetSelectBox(TitleMenu::MENU::IS_MAX);
		pMenu->SetGap(1.0f);
		pPrefab->AddComponent(pMenu);

		//タイトルメニューの挙動を追加する
		TitleMenu* pTitleMenu = new TitleMenu(pPrefab);
		pPrefab->AddComponent(pTitleMenu);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

#endif // !_PREFAB_H_
