//========================================================================================================
//
//モーションの処理[motion.h]
// Author MurataShinnosuke
//
//========================================================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"

//モーション
class Motion
{
public:
	static const int FRAME_BLEND;

	//モーションの状態の列挙
	enum STATE
	{
		IS_START = 0,   //再生中
		IS_STOP,   //停止中
		IS_END,   //終了
	};

	//キーの構造体
	struct Key
	{
		int nFrame;   //フレーム
		std::vector<D3DXVECTOR3> vPos;
		std::vector<D3DXVECTOR3> vRot;
	};

	//コンストラクタ
	Motion()
	{
		//メンバ変数を初期化する
		this->sName = "null";   //名前
		this->m_isLoop = false;   //ループフラグ
		this->m_nFrame = 0;   //現在フレーム
		this->m_nFrameBlend = 0;   //ブレンドフレーム
		this->m_nKey = 0;   //現在キー
		this->m_vKey.clear();   //キーのコンテナ
		this->m_vRot.clear();   //向き
		this->m_isEnd = false;   //終了フラグ
	}

	//デストラクタ
	~Motion()
	{

	}

	void Init();   //初期化処理
	void Uninit();   //終了処理
	void Update();   //更新処理
	void AddKey(Motion::Key key) { this->m_vKey.push_back(key); };
	void Loop() { this->m_isLoop = true; };   //ループフラグ

	//リセット処理
	void Reset()
	{
		//再生に必要な情報をリセットする
		this->m_nFrame = 0;
		this->m_nFrameBlend = 0;
		this->m_nKey = 0;
		this->m_isEnd = false;
	}

	//パーツの数の設定
	void SetNumParts(int nNum)
	{
		//パーツの数分繰り返す
		for (int i = 0; i < nNum; i++)
		{
			//パーツの分向きをコンテナに追加する
			D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f };
			this->m_vRot.push_back(rot);
		}
	}

	D3DXVECTOR3 GetRot(int nId) { return this->m_vRot[nId]; };   //向きの取得
	bool GetEnd() { return this->m_isEnd; };   //終了フラグの取得

private:
	std::string sName;   //名前
	bool m_isLoop;   //ループフラグ
	int m_nFrame;   //現在フレーム
	int m_nFrameBlend;   //ブレンドフレームs
	int m_nKey;   //現在キー
	std::vector<Motion::Key> m_vKey;   //キー
	std::vector<D3DXVECTOR3> m_vRot;   //向き
	bool m_isEnd;   //終了フラグ
};

#endif // !_MOTION_H_