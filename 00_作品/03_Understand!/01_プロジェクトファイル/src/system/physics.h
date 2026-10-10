//------------------------------------------------------------
// @file			physics.h
// @brief		物理
// @detail	物理演算の処理
//------------------------------------------------------------
#ifndef _PHYSICS_H_
#define _PHYSICS_H_

// インクルード
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

// ライブラリのリンク
#pragma comment(lib, "Bullet2FileLoader.lib")
#pragma comment(lib, "Bullet3Collision.lib")
#pragma comment(lib, "Bullet3Common.lib")
#pragma comment(lib, "Bullet3Dynamics.lib")
#pragma comment(lib, "Bullet3Geometry.lib")
#pragma comment(lib, "Bullet3OpenCL_clew.lib")
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletExampleBrowserLib.lib")
#pragma comment(lib, "BulletFileLoader.lib")
#pragma comment(lib, "BulletInverseDynamics.lib")
#pragma comment(lib, "BulletInverseDynamicsUtils.lib")
#pragma comment(lib, "BulletRobotics.lib")
#pragma comment(lib, "BulletRoboticsGUI.lib")
#pragma comment(lib, "BulletSoftBody.lib")
#pragma comment(lib, "BulletWorldImporter.lib")
#pragma comment(lib, "BulletXmlWorldImporter.lib")
#pragma comment(lib, "BussIK.lib")
#pragma comment(lib, "clsocket.lib")
#pragma comment(lib, "ConvexDecomposition.lib")
#pragma comment(lib, "GIMPACTUtils.lib")
#pragma comment(lib, "gwen.lib")
#pragma comment(lib, "HACD.lib")
#pragma comment(lib, "LinearMath.lib")
#pragma comment(lib, "OpenGLWindow.lib")

// 前方宣言
class Collision;

/**
 * @brief アクションインターフェイス
 * @details 物理イベントの通知を行います
*/
class ActionInterface : public btActionInterface
{
public:
	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep) override;
	void debugDraw(btIDebugDraw* debugDrawer) {}
};

/**
 * @brief 物理クラス
 * @details 物理システムの根幹
*/
class Physics final
{
public:
	//@brief 初期化
	void Init();
	//@brief 終了
	void Uninit();
	//@brief 更新
	void Update();

	//@brief ワールドを取得する
	btDiscreteDynamicsWorld* GetWorld() { return m_world; }

	//@brief コリジョンを追加する
	void AddCollision(Collision* collision);
	//@brief コリジョンを削除する
	void RemoveCollision(Collision* collision);
	//@brief コリジョンを取得する
	std::vector<Collision*>& GetCollisions() { return m_collisions; }
	//@brief btCollisionObjectからCollisionを探す
	Collision* FindCollision(const btCollisionObject* collisionObject);

	//@brief 強制的に構築する
	//void ForceBuild(GameObject* object);
private:
	/**
	 * @brief 物理デバッグクラス
	 * @details コリジョンを視覚化します
	*/
	class DebugPhysics : public btIDebugDraw
	{
	public:
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {};
		void reportErrorWarning(const char* warningString) override {};
		void draw3dText(const btVector3& location, const char* textString) override {};
		void setDebugMode(int debugMode) override { m_debugMode = debugMode; };
		int getDebugMode() const override { return m_debugMode; };
	private:
		int m_debugMode;
	};

	btDiscreteDynamicsWorld* m_world;							// ワールド
	btDefaultCollisionConfiguration* m_collisionConfiguration;	
	btCollisionDispatcher* m_dispatcher;
	btBroadphaseInterface* m_overlappingPairCache;
	btSequentialImpulseConstraintSolver* m_solver;
	btGhostPairCallback* m_ghostPairCollback;
	DebugPhysics* m_debugDrawer;
	ActionInterface* m_actionInterface;
	std::vector<Collision*> m_collisions;
};


#endif // !_PHYSICS_H_
