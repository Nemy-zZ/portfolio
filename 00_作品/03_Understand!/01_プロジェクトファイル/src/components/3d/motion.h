//------------------------------------------------------------
// @file		motion.h
// @brief	モーション
//------------------------------------------------------------
#ifndef _MOTION_H_
#define _MOTION_H_

#include "component.h"
#include "model.h"

/**
 * @brief モーションキー
*/
class MotionKey final
{
public:
	MotionKey() : m_frame(0), m_position({ 0.0f, 0.0f, 0.0f }), m_rotation({ 0.0f, 0.0f, 0.0f, 1.0f }), m_scale({ 1.0f, 1.0f, 1.0f }) {}

	//@brief フレーム位置を設定する
	void SetFrame(const int& frame) { m_frame = frame; }
	//@brief 位置を設定する
	void SetPosition(const D3DXVECTOR3& position) { m_position = position; }
	//@brief 回転を設定する
	void SetRotation(const D3DXQUATERNION& rotation) { m_rotation = rotation; }
	//@brief スケールを設定する
	void SetScale(const D3DXVECTOR3& scale) { m_scale = scale; }

	//@brief フレーム位置を取得する
	int GetFrame() const { return m_frame; }
	//@brief 位置を取得する
	D3DXVECTOR3 GetPosition() { return m_position; }
	//@brief 回転を取得する
	D3DXQUATERNION GetRotation() { return m_rotation; }
	//@brief スケールを取得する
	D3DXVECTOR3 GetScale() { return m_scale; }
private:
	int m_frame;
	D3DXVECTOR3 m_position;
	D3DXQUATERNION m_rotation;
	D3DXVECTOR3 m_scale;
};


/**
 * @brief モーションデータ
 * @details モーション内容を格納します
*/
class MotionData final
{
public:
	MotionData(const std::string& name) { m_name = name; }

	//@brief 名前を設定する
	void SetName(const std::string& name) { m_name = name; }
	//@brief 名前を取得する
	std::string GetName() { return m_name; }

	//@brief 最大フレーム数を設定する
	void SetMaxFrame(const int& frame) { m_maxFrame = frame; }
	//@brief 最大フレーム数を取得する
	int GetMaxFrame() { return m_maxFrame; }

	//@brief ループを設定する
	void SetLoop(const bool& enabled) { m_loop = enabled; }
	//@brief ループを取得する
	bool GetLoop() { return m_loop; }

	//@brief 登録されているキーを取得する
	std::map<int, std::list<MotionKey>>& GetKeys() { return m_keys; }

	/**
	 * @brief キーを追加する
	 * @param[in] idx : パーツID
	 * @param[in] frame : フレーム位置
	 * @param[in] transform : トランスフォーム
	*/
	void AddKeyEuler(const int& idx, const int& frame,
		const D3DXVECTOR3& position = { 0.0f, 0.0f, 0.0f },
		const D3DXVECTOR3& rotation = { 0.0f, 0.0f, 0.0f },
		const D3DXVECTOR3& scale = { 1.0f, 1.0f, 1.0f });

	/**
	 * @brief キーを追加する
	 * @param[in] idx : パーツID
	 * @param[in] frame : フレーム位置
	 * @param[in] transform : トランスフォーム
	*/
	void AddKey(const int& idx, const int& frame,
		const D3DXVECTOR3& position = { 0.0f, 0.0f, 0.0f },
		const D3DXQUATERNION& rotation = { 0.0f, 0.0f, 0.0f, 1.0f },
		const D3DXVECTOR3& scale = { 1.0f, 1.0f, 1.0f });

	/**
	 * @brief キーを削除する
	 * @param[in] idx : パーツID
	 * @param[in] frame : フレーム位置
	*/
	void RemoveKey(const int& idx, const int& frame);

	/**
	 * @brief 現在のキーを取得する
	*/
	MotionKey GetCurrentKey(const int& idx, const int& frame);

	/**
	 * @brief 次のキーを取得する
	*/
	MotionKey GetNextKey(const int& idx, const int& frame);

	//@brief 指定フレームのキー情報を取得する
	MotionKey GetFrameKey(const int& idx, const int& frame);

	/**
	 * @brief キー間のフレーム数を取得する
	*/
	int GetBetweenFrame(const int& idx, const int& frame);
private:
	//@brief キーのソート
	void SortKeys(const int& idx);

	std::string m_name;			// 名前
	int m_maxFrame;				// 最大フレーム数
	bool m_loop;						// ループするか
	std::map<int, std::list<MotionKey>> m_keys;
};

//@brief モーション
class Motion : public Component
{
public:
	Motion() : m_playing(false), m_currentFrame(0), m_playingMotion(nullptr) {}

	//@brief 終了
	void Uninit() override;

	//@brief 更新
	void Update() override;

	//@brief 再生する
	bool Play(const std::string& name);

	//@brief プレイされているモーション名を取得する
	std::string GetPlayingMotionName();

	//@brief 再生中かを取得する
	bool IsPlaying() { return m_playing; }

	//@brief 現在のフレーム数を取得する
	int GetCurrentFrame() { return m_currentFrame; }

	/**
	 * @brief パーツリストにオブジェクトを追加する
	 * @param[in] object : ゲームオブジェクト
	 * @return パーツID (error: -1)
	*/
	int AddParts(GameObject* object);

	/**
	 * @brief モーションデータを追加する
	 * @param[in] data : モーションデータ
	*/
	void AddMotionData(MotionData* data);

	/**
	 * @brief モーションデータを削除する
	 * @param[in] data : モーションデータ
	*/
	void RemoveMotionData(MotionData* data);

	/**
	 * @brief モーションデータを削除する
	 * @param[in] name : モーション名
	*/
	void RemoveMotionData(const std::string& name);

	/**
	* @brief モーションデータを検索する
	* @param[in] name : モーション名
	*/
	MotionData* FindMotionData(const std::string& name);
private:
	std::vector<GameObject*> m_parts;			// パーツリスト
	std::vector<MotionData*> m_motions;		// モーションデータ
	std::map<int, Transform> m_move;			// パーツごとの移動量

	bool m_playing;											// 再生中か
	int m_currentFrame;									// 現在のフレーム
	MotionData* m_playingMotion;					// 再生中のモーション
};

#endif // !_MOTION_H_
