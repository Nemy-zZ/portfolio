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

#include "basket.h"
#include "block.h"
#include "camera_bird.h"
#include "camera_tps.h"
#include "cursor.h"
#include "fade.h"
#include "cargo.h"
#include "insect.h"
#include "nest.h"
#include "num_insect.h"
#include "obstacle.h"
#include "pause_menu.h"
#include "player.h"
#include "score.h"
#include "timer.h"
#include "title_menu.h"

#include "waypoint.h"

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

//かご
class BasketPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos)
	{
		//かごを生成する
		Object* pPrefab = new Object("Basket");

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);
		pTransform->SetScale({ 5.0f, 5.0f, 5.0f });   //拡大率
		pPrefab->AddComponent(pTransform);

		//モデルレンダリングを追加する
		Model* pModel = new Model(pPrefab);
		pModel->SetXFile(Manager::Get()->GetXFile()->Get("basket_00.x"));
		pPrefab->AddComponent(pModel);

		//ステンシルを追加する
		Stencil* pStencil = new Stencil(pPrefab, 1, Stencil::COMPARE::IS_GREATEREQUAL, Stencil::PASS::IS_INCR);
		pPrefab->AddComponent(pStencil);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ 100.0f, 50.0f, 100.0f });
		pBoxCollider->SetOffset({ 0.0f, 25.0f, 0.0f });
		pPrefab->AddComponent(pBoxCollider);

		//かごの挙動を追加する
		Basket* pBasket = new Basket(pPrefab);
		pPrefab->AddComponent(pBasket);

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
		//ブロックを生成する
		Object* pPrefab = new Object("Block");

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);
		pPrefab->AddComponent(pTransform);

		//モデルレンダリングを追加する
		Model* pModel = new Model(pPrefab);
		pModel->SetXFile(Manager::Get()->GetXFile()->Get("block_00.x"));
		pModel->SetTexture(Manager::Get()->GetTexture()->Get("block_00.png"));
		pPrefab->AddComponent(pModel);

		//ステンシルを追加する
		Stencil* pStencil = new Stencil(pPrefab, 1, Stencil::COMPARE::IS_GREATEREQUAL, Stencil::PASS::IS_INCR);
		pPrefab->AddComponent(pStencil);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ 60.0f, 60.0f, 60.0f });
		pBoxCollider->SetOffset({ 0.0f, 30.0f, 0.0f });
		pPrefab->AddComponent(pBoxCollider);

		//ブロックの挙動を追加する
		Block* pBlock = new Block(pPrefab);
		pPrefab->AddComponent(pBlock);

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
		pPrefab->AddComponent(pTransform);

		//カメラを追加する
		Camera* pCamera = new Camera(pPrefab);
		pPrefab->AddComponent(pCamera);

		//バード視点カメラの挙動を追加する
		CameraBird* pCameraBird = new CameraBird(pPrefab);
		pPrefab->AddComponent(pCameraBird);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//カーソル
class CursorPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos)
	{
		//背景を生成する
		Object* pPrefab = new Object("Cursor");

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);
		pTransform->SetScale({ 0.25f, 0.25f, 0.25f });
		pPrefab->AddComponent(pTransform);

		//四角形レンダリングを追加する
		PlaneXZ* pPlaneXZ = new PlaneXZ(pPrefab);
		pPlaneXZ->SetTexture(Manager::Get()->GetTexture()->Get("cursor_00.png"));
		pPrefab->AddComponent(pPlaneXZ);

		//カーソルの挙動を追加する
		Cursor* pCursor = new Cursor(pPrefab);
		pPrefab->AddComponent(pCursor);

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

//床
class FieldPrefab
{
public:
	//生成処理
	static Object* Create()
	{
		//背景を生成する
		Object* pPrefab = new Object("Field");

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos({ 0.0f, 0.0f, 0.0f });
		pTransform->SetScale({ 4.0f, 1.0f, 4.0f });
		pPrefab->AddComponent(pTransform);

		//四角形レンダリングを追加する
		PlaneXZ* pPlaneXZ = new PlaneXZ(pPrefab);
		pPlaneXZ->SetTexture(Manager::Get()->GetTexture()->Get("field_00.png"));
		pPrefab->AddComponent(pPlaneXZ);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ Plane::BASE_SIZE.x * pTransform->GetScale().x, 1.0f, Plane::BASE_SIZE.y * pTransform->GetScale().z });
		pBoxCollider->SetOffset({ 0.0f, -0.5f, 0.0f });
		pBoxCollider->SetMass(99);   //質量
		pPrefab->AddComponent(pBoxCollider);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//フルーツ
class FruitPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos, int nWeight)
	{
		//フルーツを生成する
		Object* pPrefab = new Object("Fruit");
		pPrefab->SetTag(Object::TAG::IS_FRUIT);

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);
		pTransform->SetScale({ 3.0f, 3.0f, 3.0f });
		pPrefab->AddComponent(pTransform);

		//モデルレンダリングを追加する
		Model* pModel = new Model(pPrefab);
		pModel->SetXFile(Manager::Get()->GetXFile()->Get("fruit_00.x"));
		pPrefab->AddComponent(pModel);

		//ステンシルを追加する
		Stencil* pStencil = new Stencil(pPrefab, 1, Stencil::COMPARE::IS_GREATEREQUAL, Stencil::PASS::IS_INCR);
		pPrefab->AddComponent(pStencil);

		//貨物の挙動を追加する
		Cargo* pCargo = new Cargo(pPrefab, nWeight);
		pPrefab->AddComponent(pCargo);

		//リジットボディを追加する
		Rigidbody* pRigidbody = new Rigidbody(pPrefab);
		pPrefab->AddComponent(pRigidbody);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ 50.0f, 50.0f, 50.0f });
		pBoxCollider->SetOffset({ 0.0f, 25.0f, 0.0f });
		pBoxCollider->SetMass(4);   //質量
		pPrefab->AddComponent(pBoxCollider);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//虫
class InsectPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos)
	{
		//虫を生成する
		Object* pPrefab = new Object("Insect");

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);
		pTransform->SetScale({ 1.0f, 1.0f, 1.0f });
		pPrefab->AddComponent(pTransform);

		//モデルレンダリングを追加する
		Model* pModel = new Model(pPrefab);
		pModel->SetXFile(Manager::Get()->GetXFile()->Get("insect_00.x"));
		pPrefab->AddComponent(pModel);

		//虫の挙動を追加する
		Insect* pInsect = new Insect(pPrefab);
		pPrefab->AddComponent(pInsect);

		//リジットボディを追加する
		Rigidbody* pRigidbody = new Rigidbody(pPrefab);
		pRigidbody->SetGravity(0.75f);
		pRigidbody->SetFrictionAir(0.1f);   //空中用の摩擦
		pPrefab->AddComponent(pRigidbody);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ 10.0f, 20.0f, 10.0f });
		pBoxCollider->SetOffset({ 0.0f, 10.0f, 0.0f });
		pBoxCollider->SetMass(2);
		pPrefab->AddComponent(pBoxCollider);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//ゼリー
class JellyPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos, int nWeight)
	{
		//ゼリーを生成する
		Object* pPrefab = new Object("Jelly");
		pPrefab->SetTag(Object::TAG::IS_JELLY);

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);
		pTransform->SetScale({ 2.0f, 2.0f, 2.0f });
		pPrefab->AddComponent(pTransform);

		//モデルレンダリングを追加する
		Model* pModel = new Model(pPrefab);
		pModel->SetXFile(Manager::Get()->GetXFile()->Get("jelly_00.x"));
		pPrefab->AddComponent(pModel);

		//ステンシルを追加する
		Stencil* pStencil = new Stencil(pPrefab, 1, Stencil::COMPARE::IS_GREATEREQUAL, Stencil::PASS::IS_INCR);
		pPrefab->AddComponent(pStencil);

		//貨物の挙動を追加する
		Cargo* pCargo = new Cargo(pPrefab, nWeight);
		pPrefab->AddComponent(pCargo);

		//リジットボディを追加する
		Rigidbody* pRigidbody = new Rigidbody(pPrefab);
		pPrefab->AddComponent(pRigidbody);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ 35.0f, 32.0f, 35.0f });
		pBoxCollider->SetOffset({ 0.0f, 16.0f, 0.0f });
		pBoxCollider->SetMass(4);   //質量
		pPrefab->AddComponent(pBoxCollider);

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

//プレイヤー
class PlayerPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos)
	{
		//ブロックを生成する
		Object* pPrefab = new Object("Player");

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);
		pTransform->SetScale({ 0.25f, 0.25f, 0.25f });
		pPrefab->AddComponent(pTransform);

		//モデルレンダリングを追加する
		Model* pModel = new Model(pPrefab);
		pModel->LoadParts("motion_kabuto.txt");
		pPrefab->AddComponent(pModel);

		//ステンシルを追加する
		Stencil* pStencil = new Stencil(pPrefab, 1, Stencil::COMPARE::IS_ALWAYS, Stencil::PASS::IS_REPLACE);
		pPrefab->AddComponent(pStencil);

		//モーショナーを追加する
		Motioneer* pMotioneer = new Motioneer(pPrefab);
		pMotioneer->LoadMotion("motion_kabuto.txt");
		pPrefab->AddComponent(pMotioneer);

		//プレイヤーの挙動を追加する
		Player* pPlayer = new Player(pPrefab);
		pPrefab->AddComponent(pPlayer);

		//リジットボディを追加する
		Rigidbody* pRigidbody = new Rigidbody(pPrefab);
		pPrefab->AddComponent(pRigidbody);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ 50.0f, 50.0f, 50.0f });
		pBoxCollider->SetOffset({ 0.0f, 25.0f, 0.0f });
		pPrefab->AddComponent(pBoxCollider);

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
	static Object* Create()
	{
		//スコアを生成する
		Object* pPrefab = new Object("Score");

		//トランスフォームを追加する
		Transform2D* pTransform2D = new Transform2D(pPrefab);
		pTransform2D->SetPos({ 50.0f, 70.0f });   //位置
		pTransform2D->SetSize({ 500.0f, 100.0f });   //サイズ
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
		pTransform2D->SetSize({200.0f, 100.0f});   //サイズ
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
		pTransform2D->SetPos({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });   //位置
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

//キノコ
class MushroomPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos)
	{
		//キノコを生成する
		Object* pPrefab = new Object("Mushroom");
		pPrefab->SetTag(Object::TAG::IS_OBSTACLE);   //障害物

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);
		pPrefab->AddComponent(pTransform);

		//モデルレンダリングを追加する
		Model* pModel = new Model(pPrefab);
		pModel->SetXFile(Manager::Get()->GetXFile()->Get("mushroom_00.x"));
		pPrefab->AddComponent(pModel);

		//ステンシルを追加する
		Stencil* pStencil = new Stencil(pPrefab, 1, Stencil::COMPARE::IS_GREATEREQUAL, Stencil::PASS::IS_INCR);
		pPrefab->AddComponent(pStencil);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ 60.0f, 60.0f, 60.0f });
		pBoxCollider->SetOffset({ 0.0f, 30.0f, 0.0f });
		pPrefab->AddComponent(pBoxCollider);

		//障害物の挙動を追加する
		Obstacle* pObstacle = new Obstacle(pPrefab);
		pPrefab->AddComponent(pObstacle);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//巣
class NestPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos)
	{
		//巣を生成する
		Object* pPrefab = new Object("Nest");

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);
		pTransform->SetScale({ 2.5f, 2.5f, 2.5f });   //拡大率
		pPrefab->AddComponent(pTransform);

		//モデルレンダリングを追加する
		Model* pModel = new Model(pPrefab);
		pModel->SetXFile(Manager::Get()->GetXFile()->Get("nest_00.x"));
		pPrefab->AddComponent(pModel);

		//ステンシルを追加する
		Stencil* pStencil = new Stencil(pPrefab, 1, Stencil::COMPARE::IS_GREATEREQUAL, Stencil::PASS::IS_INCR);
		pPrefab->AddComponent(pStencil);

		//ボックスコライダーを追加する
		BoxCollider* pBoxCollider = new BoxCollider(pPrefab);
		pBoxCollider->SetSize({ 50.0f, 50.0f, 50.0f });
		pBoxCollider->SetOffset({ 0.0f, 25.0f, 0.0f });
		pPrefab->AddComponent(pBoxCollider);

		//かごの挙動を追加する
		Nest* pNest = new Nest(pPrefab);
		pPrefab->AddComponent(pNest);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//虫の数
class NumInsectPrefab
{
public:
	//生成処理
	static Object* Create()
	{
		//スコアを生成する
		Object* pPrefab = new Object("NumInsect");

		//トランスフォームを追加する
		Transform2D* pTransform2D = new Transform2D(pPrefab);
		pTransform2D->SetPos({ 1100.0f, 650.0f });   //位置
		pTransform2D->SetSize({ 200.0f, 100.0f });   //サイズ
		pPrefab->AddComponent(pTransform2D);

		//数字レンダリングを追加する
		Number* pNumber = new Number(pPrefab);
		pNumber->SetTexture(Manager::Get()->GetTexture()->Get("number_00.png"));
		pNumber->SetDigit(3);
		pNumber->SetNumber(Timer::START_TIMER);
		pPrefab->AddComponent(pNumber);

		//スコアの挙動を追加する
		NumInsect* pNumInsect = new NumInsect(pPrefab);
		pPrefab->AddComponent(pNumInsect);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

//ウェイポイント
class WeypointPrefab
{
public:
	//生成処理
	static Object* Create(D3DXVECTOR3 pos, int nId, std::string sFileName)
	{
		//キノコを生成する
		Object* pPrefab = new Object("Waypoint" + std::to_string(nId));
		pPrefab->SetTag(Object::TAG::IS_OBSTACLE);   //障害物

		//トランスフォームを追加する
		Transform* pTransform = new Transform(pPrefab);
		pTransform->SetPos(pos);
		pTransform->SetScale({ 0.25f, 0.25f, 0.25f });
		pPrefab->AddComponent(pTransform);

		//モデルレンダリングを追加する
		Billboard* pBillboard = new Billboard(pPrefab);
		pBillboard->SetTexture(Manager::Get()->GetTexture()->Get(sFileName));
		pPrefab->AddComponent(pBillboard);

		//初期化する
		pPrefab->Init();

		//プレハブを返す
		return pPrefab;
	}
};

#endif // !_PREFAB_H_
