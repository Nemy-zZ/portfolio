//------------------------------------------------------------
// @file		gizmo.h
// @brief	ギズモ
// @detail ギズモ機能を提供します
//------------------------------------------------------------
#ifndef _GIZMO_H_
#define _GIZMO_H_

// ギズモ
#define Gizmo GizmoSystem::GetInstance()

/**
 * @brief ギズモシステム
*/
class GizmoSystem final
{
public:
	//@brief インスタンスを取得する
	static GizmoSystem& GetInstance()
	{
		static GizmoSystem instance;
		return instance;
	}

	//@brief 解放する
	void Release();

	//@brief 3Dレンダリングする
	void Render3D();
	//@brief 2Dレンダリングする
	void Render2D();

	//@brief ラインを描画する
	void DrawLine(const D3DXVECTOR3& from, const D3DXVECTOR3& to, const D3DXCOLOR& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	//@brief 2Dラインを描画する
	void DrawLine2D(const D3DXVECTOR2& from, const D3DXVECTOR2& to, const float& bold = 1.0f, const D3DXCOLOR& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	//@brief 長方形の2Dラインを描画する
	void DrawRectangle(const D3DXVECTOR2& start, const D3DXVECTOR2& size, const float& bold = 1.0f, const D3DXCOLOR& color = { 1.0f, 1.0f, 1.0f, 1.0f }, const float& angle = 0.0f);
	//@brief ボックスの3Dラインを描画する
	void DrawBox(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXCOLOR& color = { 1.0f, 1.0f, 1.0f, 1.0f }, const D3DXMATRIX& matrix = D3DXMATRIX_IDENTITY);
private:
	//@brief ギズモオブジェクト
	template<class T> class GizmoObject
	{
	public:
		GizmoObject() : m_life(30), m_use(true) {};

		/**
		 * @brief 更新
		 * @return 破棄タイミング
		*/
		bool Update()
		{
			if (!m_use)
			{
				m_life--;
				return true;
			}
				
			m_life = 30;
			return false;
		}

		//@brief 使用状態を取得する
		bool& GetUse() { return m_use; }

		//@brief 取得
		T& Get() { return m_obj; }
	private:
		T m_obj;
		int m_life;
		bool m_use;
	};
	
	//@brief ライン3D
	class Line3D
	{
	public:
		void Init();
		void Uninit();
		void Draw();
		void Set(const D3DXVECTOR3& from, const D3DXVECTOR3& to, const D3DXCOLOR& color);
	private:
		LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;
	};
	//@brief ライン2D
	class Line2D
	{
	public:
		void Init();
		void Uninit();
		void Draw();
		void Set(const D3DXVECTOR2& from, const D3DXVECTOR2& to, const float& bold, const D3DXCOLOR& color);
	private:
		LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;
	};

private:
	// ギズモオブジェクトの管理
	std::vector<GizmoObject<Line3D>> m_line3d;
	std::vector<GizmoObject<Line2D>> m_line2d;
};


#endif // !_GIZMO_H_
