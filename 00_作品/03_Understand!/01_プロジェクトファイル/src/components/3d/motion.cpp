//------------------------------------------------------------
// @file		motion.cpp
// @brief	モーション
//------------------------------------------------------------
#include "motion.h"
#include "components/2d/text.h"
using namespace std;

//=============================================================
// 終了
//=============================================================
void Motion::Uninit()
{
	for (auto itr = m_motions.begin(); itr != m_motions.end(); itr++)
	{
		delete* itr;
		*itr = nullptr;
	}
	m_motions.clear();
}

//=============================================================
// 更新
//=============================================================
void Motion::Update()
{
	// 再生する条件が揃っていないとき
	if (!(m_playing && m_playingMotion != nullptr))
	{
		return;
	}

	// モーションを取得する
	auto motion = m_playingMotion->GetKeys();

	for (int idx = 0; idx < static_cast<int>(m_parts.size()); idx++)
	{
		// キーがあるかを確認する
		if (motion[idx].empty())
		{
			continue;
		}

		// パーツがセットされているかを確認する
		if (m_parts[idx] == nullptr)
		{
			continue;
		}

		// キー間のフレーム数
		int betweenFrame = m_playingMotion->GetBetweenFrame(idx, m_currentFrame);
		if (betweenFrame <= 0) betweenFrame = 1;

		MotionKey currentKey = m_playingMotion->GetCurrentKey(idx, m_currentFrame);
		MotionKey nextKey = m_playingMotion->GetNextKey(idx, m_currentFrame);

		// 変化量計算
		for (auto itr = motion[idx].begin(); itr != motion[idx].end(); itr++)
		{
			// 計算タイミングが来た時
			if (m_currentFrame == 0 || m_currentFrame == (*itr).GetFrame())
			{
				// 変化量を計算する
				m_move[idx].position = (nextKey.GetPosition() - m_parts[idx]->transform->position) / static_cast<float>(betweenFrame);
				m_move[idx].scale = (nextKey.GetScale() - m_parts[idx]->transform->scale) / static_cast<float>(betweenFrame);
				break;
			}
		}

		// 動かす
		m_parts[idx]->transform->position += m_move[idx].position;
		m_parts[idx]->transform->scale += m_move[idx].scale;

		// 動いたフレーム数
		int advancedFrame = m_currentFrame < currentKey.GetFrame() ? m_currentFrame + (m_playingMotion->GetMaxFrame() - currentKey.GetFrame()) : (m_currentFrame - currentKey.GetFrame());

		// 回転
		D3DXQUATERNION currentQ = currentKey.GetRotation();
		D3DXQUATERNION nextQ = nextKey.GetRotation();
		D3DXQuaternionSlerp(&m_parts[idx]->transform->rotation,
			&currentQ,
			&nextQ,
			static_cast<float>(advancedFrame) / static_cast<float>(betweenFrame)
		);
	}

	// フレームをインクリメント
	m_currentFrame++;

	// 終了時
	if (m_currentFrame >= m_playingMotion->GetMaxFrame() + 1)
	{
		if (m_playingMotion->GetLoop())
		{
			// フレームカウンターをリセットする
			m_currentFrame = 0;

			for (int idx = 0; idx < static_cast<int>(m_parts.size()); idx++)
			{
				MotionKey reset = m_playingMotion->GetFrameKey(idx, 0);
				m_parts[idx]->transform->position = reset.GetPosition();
				m_parts[idx]->transform->rotation = reset.GetRotation();
				m_parts[idx]->transform->scale = reset.GetScale();
			}
		}
		else
		{
			m_playing = false;
		}
	}
}

//=============================================================
// 再生する
//=============================================================
bool Motion::Play(const std::string& name)
{
	// モーションデータを検索する
	MotionData* motion = FindMotionData(name);

	// 存在しないとき
	if (motion == nullptr)
	{
		Log::SendLog(name + "というモーションは存在しないため、再生に失敗しました", Log::TYPE_WARNING);
		m_playing = false;
		return false;
	}

	// 既に再生されているとき
	if (motion == m_playingMotion)
	{
		m_playing = true;
		return true;
	}

	// 再生する
	m_playingMotion = motion;
	m_currentFrame = 0;
	m_playing = true;
	
	return true;
}

//=============================================================
// 再生されているモーション名を取得する
//=============================================================
std::string Motion::GetPlayingMotionName()
{
	if (m_playingMotion != nullptr)
	{
		return m_playingMotion->GetName();
	}
	return "";
}

//=============================================================
// パーツリストにオブジェクトを追加する
//=============================================================
int Motion::AddParts(GameObject* object)
{
	if (find(m_parts.begin(), m_parts.end(), object) == m_parts.end())
	{
		m_move[(int)m_parts.size()].scale = { 0.0f, 0.0f, 0.0f };
		m_parts.push_back(object);
		return static_cast<int>(m_parts.size());
	}
	return -1;
}

//=============================================================
// モーションデータを追加する
//=============================================================
void Motion::AddMotionData(MotionData* data)
{
	if (data != nullptr)
	{
		if (find(m_motions.begin(), m_motions.end(), data) == m_motions.end())
		{
			m_motions.push_back(data);
		}
	}
}

//=============================================================
// モーションデータを削除する
//=============================================================
void Motion::RemoveMotionData(MotionData* data)
{
	auto itr = find(m_motions.begin(), m_motions.end(), data);
	if (itr != m_motions.end())
	{
		m_motions.erase(itr);
	}
}

//=============================================================
// モーションデータを削除する
//=============================================================
void Motion::RemoveMotionData(const std::string& name)
{
	MotionData* motion = FindMotionData(name);
	if (motion != nullptr)
	{
		RemoveMotionData(motion);
	}
}

//=============================================================
// モーションデータを検索する
//=============================================================
MotionData* Motion::FindMotionData(const std::string& name)
{
	for (auto itr = m_motions.begin(); itr != m_motions.end(); itr++)
	{
		if (name == (*itr)->GetName())
		{
			return *itr;
		}
	}
	return nullptr;
}

//=============================================================
// キーを追加する
//=============================================================
void MotionData::AddKeyEuler(const int& idx, const int& frame, const D3DXVECTOR3& position, const D3DXVECTOR3& rotation, const D3DXVECTOR3& scale)
{
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, rotation.y, rotation.x, rotation.z);
	AddKey(idx, frame, position, q, scale);
}

//=============================================================
// キーを追加する
//=============================================================
void MotionData::AddKey(const int& idx, const int& frame, const D3DXVECTOR3& position, const D3DXQUATERNION& rotation, const D3DXVECTOR3& scale)
{
	assert(idx >= 0);

	// フレーム位置が同じとき（上書き）
	for (auto itr = m_keys[idx].begin(); itr != m_keys[idx].end(); itr++)
	{
		if (frame == (*itr).GetFrame())
		{
			(*itr).SetPosition(position);
			(*itr).SetRotation(rotation);
			(*itr).SetScale(scale);
			return;
		}
	}

	// キーを作成する
	MotionKey key;
	key.SetFrame(frame);
	key.SetPosition(position);
	key.SetRotation(rotation);
	key.SetScale(scale);

	// キーを追加する
	m_keys[idx].push_back(key);

	// 並べ替える
	SortKeys(idx);
}

//=============================================================
// キーを削除する
//=============================================================
void MotionData::RemoveKey(const int& idx, const int& frame)
{
	assert(idx >= 0);

	// キーが存在していないとき
	if (!m_keys.count(idx))
	{
		return;
	}

	// 削除する
	for (auto itr = m_keys[idx].begin(); itr != m_keys[idx].end(); itr++)
	{
		if (frame == (*itr).GetFrame())
		{
			m_keys[idx].erase(itr);
			return;
		}
	}
}

//=============================================================
// 現在のキーを取得する
//=============================================================
MotionKey MotionData::GetCurrentKey(const int& idx, const int& frame)
{
	assert(idx >= 0);

	// モーションにキーが1つのとき
	if (m_keys[idx].size() <= 1)
	{
		return *m_keys[idx].begin();
	}

	// 現在のフレームを記録する
	MotionKey current;
	bool isFinded = false;
	for (auto itr = m_keys[idx].begin(); itr != m_keys[idx].end(); itr++)
	{
		if ((*itr).GetFrame() <= frame)
		{
			current = *itr;
			isFinded = true;
		}
	}

	// 見つからなかったとき
	if (!isFinded && m_loop)
	{
		// ループ時はループ前のキーを返す
		return *std::prev(m_keys[idx].end());
	}

	return current;
}

//=============================================================
// 次のキーを取得する
//=============================================================
MotionKey MotionData::GetNextKey(const int& idx, const int& frame)
{
	assert(idx >= 0);

	// モーションにキーが1つのとき
	if (m_keys[idx].size() <= 1)
	{
		return *m_keys[idx].begin();
	}

	// 終了までに次のキーがあるとき
	for (auto itr = m_keys[idx].begin(); itr != m_keys[idx].end(); itr++)
	{
		if ((*itr).GetFrame() > frame)
		{
			return *itr;
		}
	}

	// ループのとき
	if (m_loop)
	{
		return *m_keys[idx].begin();
	}

	return GetCurrentKey(idx, frame);
}

//=============================================================
// キー間のフレーム数を取得する
//=============================================================
int MotionData::GetBetweenFrame(const int& idx, const int& frame)
{
	int current = GetCurrentKey(idx, frame).GetFrame();
	int next = GetNextKey(idx, frame).GetFrame();
	if (current < next)
	{
		return next - current;
	}
	else
	{
		return (m_maxFrame - current) + next;
	}
}

//=============================================================
// キーの並べ替え
//=============================================================
void MotionData::SortKeys(const int& idx)
{
	// std::stable_sort を使用
	std::stable_sort(m_keys[idx].begin(), m_keys[idx].end(), [](const MotionKey& a, const MotionKey& b) {
		return a.GetFrame() < b.GetFrame();
		});
}

//=============================================================
// 指定フレームのキー情報を取得する
//=============================================================
MotionKey MotionData::GetFrameKey(const int& idx, const int& frame)
{
	if (m_keys[idx].empty())
	{
		return MotionKey();
	}

	MotionKey currentKey = GetCurrentKey(idx, frame);
	MotionKey nextKey = GetNextKey(idx, frame);

	// 進んだフレーム数を計算する
	int advancedFrame = frame < currentKey.GetFrame() ? frame + (m_maxFrame - currentKey.GetFrame()) : (frame - currentKey.GetFrame());
	int betweenFrame = GetBetweenFrame(idx, frame);

	MotionKey result;
	result.SetPosition((nextKey.GetPosition() - currentKey.GetPosition()) * (float)(advancedFrame / (float)betweenFrame) + currentKey.GetPosition());
	result.SetScale((nextKey.GetScale() - currentKey.GetScale()) * (float)(advancedFrame / (float)betweenFrame) + currentKey.GetScale());

	D3DXQUATERNION resQ;
	D3DXQUATERNION currentQ = currentKey.GetRotation();
	D3DXQUATERNION nextQ = nextKey.GetRotation();
	D3DXQuaternionSlerp(&resQ,
		&currentQ,
		&nextQ,
		static_cast<float>(advancedFrame) / static_cast<float>(betweenFrame)
	);
	result.SetRotation(resQ);
	return result;
}

