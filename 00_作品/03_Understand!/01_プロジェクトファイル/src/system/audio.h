//------------------------------------------------------------
// @file		audio.h
// @brief	オーディオ
// @detail	音の処理
//------------------------------------------------------------
#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "fmod.hpp"
#include "component.h"

namespace AudioSystem
{
	//@brief グループ（カテゴリ）
	enum Group
	{
		SFX,				// 効果音
		BGM,				// BGM
		AMBIENT,		// 環境

		MAX				// 最大
	};
}
using namespace AudioSystem;


/**
 * @brief オーディオシステム
 * @details 音の機能全般を管理します
*/
class Audio
{
public:
	Audio() : m_system(nullptr), m_soundGroup() {}

	//@brief 初期化
	void Init();
	//@brief 終了
	void Uninit();
	//@brief 更新
	void Update();
	//@brief システムを取得する
	FMOD::System* GetSystem() { return m_system; }
	//@brief チャンネルグループを取得する
	FMOD::ChannelGroup* GetChannelGroup(const Group& group);

private:
	FMOD::System* m_system;
	FMOD::ChannelGroup* m_soundGroup[Group::MAX];
};

/**
 * @brief オーディオクリップ
 * @details 音データ
*/
class AudioClip final
{
public:
	//@brief サウンドを取得する
	FMOD::Sound* GetSound() { return m_sound; }

	//@brief ループするか
	bool IsLoop() { return m_loop; }

	//@brief ストリーミング再生か
	bool IsStreaming() { return m_isStream; }

	//@brief 2D再生か
	bool Is2D() { return m_is2D; }

	//@brief 2Dのオーディオクリップを生成します
	static AudioClip* Ref2D(const char* path, const bool& loop = false, const bool& stream = false);

	//@brief 3Dのオーディオクリップを生成します
	static AudioClip* Ref3D(const char* path, const bool& loop = false, const bool& stream = false);

	/**
	 * @brief オーディオクリップを検索する
	 * @return 見つかったAudioClip（未発見はnullptr）
	*/
	static AudioClip* FindClip(const char* path, const bool& loop, const bool& is2D, const bool& stream);

	/**
	 * @brief オーディオクリップを解放します
	 * @return 成功 / 失敗
	*/
	static bool Release(AudioClip* clip);

	/**
	 * @brief すべてのオーディオクリップを解放する
	*/
	static void AllRelease();
private:
	//@brief コンストラクタ
	AudioClip() = default;
	~AudioClip() = default;

	FMOD::Sound* m_sound;	// サウンド

	// 情報
	std::string m_path;			// パス
	bool m_loop;						// ループ
	bool m_isStream;				// ストリーミング
	bool m_is2D;						// 2Dサウンド

	static std::vector<AudioClip*> m_audioclips;	// 登録されているオーディオクリップ一覧
};

/**
 * @brief オーディオソース
 * @details 音を再生します
*/
class AudioSource : public Component
{
public:
	AudioSource() : m_channel(nullptr), m_stopDestroy(false) {}
	void Update() override;
	void Uninit() override;

	/**
	 * @brief サウンドを再生します
	 * @param[in] オーディオクリップ
	*/
	void Play(AudioClip* clip, const Group& group, const bool& stopDestroy = false);

	//@brief 再生されているか
	bool IsPlaying();
	//@brief 音量を設定する
	void SetVolume(const float& volume);
	//@brief ピッチを設定する
	void SetPitch(const float& pitch);
	//@brief 停止する
	void Stop();
	//@brief ポーズする
	void SetPause(const bool& paused);
	//@brief エフェクトを追加する（再生前に実行）
	FMOD::DSP* AddEffect(const FMOD_DSP_TYPE& type);
	//@brief エフェクトを削除する
	void RemoveEffect(const FMOD_DSP_TYPE& type);
	//@brief すべてのエフェクトを削除する
	void ClearEffect();
private:
	FMOD::Channel* m_channel;				// チャンネル
	bool m_stopDestroy;

	struct DSPData
	{
		FMOD::DSP* dsp;
		FMOD_DSP_TYPE type;
	};
	std::vector<DSPData> m_dsps;	// エフェクト
};

/**
 * @brief 物理的オーディオソース
 * @details 物体の挙動を元に適した音を鳴らします
*/
class PhysicalAudioSource : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	//@brief 衝撃音を追加する
	void AddImpactAudio(AudioClip* clip, const float& volume = 1.0f);
	//@brief 回転音を追加する
	void SetRollingAudio(AudioClip* clip, const float& volume = 1.0f);
	//@brief オーディオグループを設定する
	void SetAudioGroup(const Group& group) { m_group = group; }

private:
	enum STATE
	{
		IDLE = 0,	// アイドル
		IMPACT,	// 衝撃
		ROLLING,	// 回転中
	};

	//@brief 状態更新
	STATE GetState(btRigidBody* rigidBody);
	void OnEnterState(const STATE& next);
	void OnExitState(const STATE& current);
	void OnUpdateState(const STATE& current);

	//@brief 衝撃音再生
	void PlayImpact();

	// 音声
	struct AudioData
	{
		AudioClip* audioClip;
		float volume;
	};
	std::vector<AudioData> m_impacts;	// 衝撃音
	AudioData m_rolling;							// 回転音

	// 再生
	FMOD::Channel* m_rollingChannel;

	// 情報
	STATE m_state;
	AudioSystem::Group m_group;
	btVector3 m_deltaV;
	btVector3 m_prevVelocity;
};


/**
 * @brief オーディオリスナー
 * @details 音を聞くための耳
*/
class AudioListener : public Component
{
public:
	void Update() override;
private:

};

/**
 * @brief オーディオウォールジオメトリ
 * @details 音を遮蔽・反響させる壁
*/
class AudioWallGeometry : public Component
{
public:
	AudioWallGeometry() : m_directOcclusion(1.0f), m_reverbOcclusion(0.0f), m_doubleSided(true), m_isUpdate(false){}
	void Init() override;
	void Uninit() override;
	void Update() override;

	//@brief サイズを設定する
	void SetSize(const D3DXVECTOR2& size);
	//@brief 直接音遮蔽率を設定する
	void SetDirectOcclusion(const float& value);
	//@brief 残響音遮蔽率を設定する
	void SetReverbOcclusion(const float& value);
	//@brief 両面かを設定する
	void SetDoubleSided(const bool& enabled);

private:
	float m_directOcclusion;						// 直接音遮蔽率
	float m_reverbOcclusion;						// 残響音遮蔽率
	bool m_doubleSided;							// 両面
	bool m_isUpdate;								// 情報アップデートされたか

	D3DXVECTOR2 m_size;						// サイズ
	TransformMonitor* m_monitor;
	FMOD::Geometry* m_geometry;		// ジオメトリ
};


/**
 * @brief オーディオリバーブ
 * @details 音の残響特性
*/
class AudioReverb : public Component
{
public:
	AudioReverb() : m_reverb(nullptr), m_min(0.0f), m_max(0.0f){}
	void Init() override;
	void Uninit() override;
	void Update() override;

	/**
	 * @brief プロパティを設定します
	 * @param[in] properties : FMOD_PRESETを参照 または FMOD_REVERB_PROPERTIESでカスタム設定
	*/
	void SetProperties(FMOD_REVERB_PROPERTIES properties);

	/**
	 * @brief 影響範囲を設定します（minからmaxの間）
	 * @param[in] min : 最短距離
	 * @param[in] max : 最大距離
	*/
	void SetRange(const float& min, const float& max);

private:
	FMOD::Reverb3D* m_reverb;
	FMOD_REVERB_PROPERTIES m_properties;
	float m_min;
	float m_max;
};

#endif // !_AUDIO_H_
