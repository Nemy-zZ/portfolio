//------------------------------------------------------------
// @file		audio.cpp
// @brief	オーディオ
// @detail	音の処理
//------------------------------------------------------------
#include "audio.h"
#include "fmod_errors.h"

std::vector<AudioClip*> AudioClip::m_audioclips;
//=============================================================
// 初期化
//=============================================================
void Audio::Init()
{
    FMOD_RESULT result;

    // 生成
    result = FMOD::System_Create(&m_system);
    if (result != FMOD_OK)
    {
        Log::SendLog(FMOD_ErrorString(result), Log::TYPE_ERROR);
        Main::GetInstance().ExitApplication();
    }
    else
    {
        Log::SendLog("FMODの生成に成功しました");
    }

    // 初期化
    result = m_system->init(512, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        Log::SendLog(FMOD_ErrorString(result), Log::TYPE_ERROR);
        Main::GetInstance().ExitApplication();
    }
    else
    {
        Log::SendLog("FMODの初期化に成功しました");
    }


    // チャンネルを作成する
    for (int i = 0; i < Group::MAX; i++)
    {
        char num[24];
        sprintf(&num[0], "%d", i);
        m_system->createChannelGroup(num, &m_soundGroup[i]);
    }

    // 一度更新する
    Update();
}

//=============================================================
// 終了
//=============================================================
void Audio::Uninit()
{
    AudioClip::AllRelease();

    // チャンネルグループを削除する
    for (int i = 0; i < Group::MAX; i++)
    {
        m_soundGroup[i]->release();
        m_soundGroup[i] = nullptr;
    }

    // システムを削除する
    if (m_system != nullptr)
    {
        m_system->release();
        m_system = nullptr;
        Log::SendLog("FMODを終了しました");
    }
}

//=============================================================
// 更新
//=============================================================
void Audio::Update()
{
    // システムを更新する
    m_system->update();

    // 設定から音量を適用する
    //FMOD::ChannelGroup* master;
    //Manager::GetInstance()->GetAudio()->GetSystem()->getMasterChannelGroup(&master);
    //master->setVolume(Option::GetInstance().GetMasterVolume() * 0.1f);
    //Manager::GetInstance()->GetAudio()->GetChannelGroup(Group::SFX)->setVolume(Option::GetInstance().GetSEVolume() * 0.1f);
    //Manager::GetInstance()->GetAudio()->GetChannelGroup(Group::BGM)->setVolume(Option::GetInstance().GetBGMVolume() * 0.1f);
    //Manager::GetInstance()->GetAudio()->GetChannelGroup(Group::AMBIENT)->setVolume(Option::GetInstance().GetAmbientVolume() * 0.1f);
}

//=============================================================
// チャンネルを取得する
//=============================================================
FMOD::ChannelGroup* Audio::GetChannelGroup(const Group& group)
{
    if (0 <= group && group < Group::MAX)
    {
        return m_soundGroup[group];
    }
    return nullptr;
}


//=============================================================
// 2Dのオーディオクリップを参照します
//=============================================================
AudioClip* AudioClip::Ref2D(const char* path, const bool& loop, const bool& stream)
{
    // システムを取得する
    auto audio = Manager::GetInstance()->GetAudio();
    auto system = Manager::GetInstance()->GetAudio()->GetSystem();

    // 既に存在しないか
    AudioClip* clip = FindClip(path, loop, true, stream);
    if (clip != nullptr)
        return clip;

    // クリップを作成する
    clip = new AudioClip();
    clip->m_path = path;
    clip->m_loop = loop;
    clip->m_isStream = stream;
    clip->m_is2D = true;

    // サウンドを生成する
    FMOD_RESULT result;
    if (stream)
    {
        result = system->createStream(
            path,
            FMOD_2D | (loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF),
            nullptr,
            &clip->m_sound
        );
    }
    else
    {
        result = system->createSound(
            path,
            FMOD_2D | (loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF),
            nullptr,
            &clip->m_sound
        );
    }

    // 失敗したとき
    if (result != FMOD_OK)
    {
        Log::SendLog(static_cast<std::string>(path) + " のサウンド生成に失敗しました", Log::TYPE_ERROR);
        delete clip;
        clip = nullptr;
        return clip;
    }

    // リストに追加する
    m_audioclips.emplace_back(clip);

    return clip;
}

//=============================================================
// 3Dのオーディオクリップを参照します
//=============================================================
AudioClip* AudioClip::Ref3D(const char* path, const bool& loop, const bool& stream)
{
    // システムを取得する
    auto audio = Manager::GetInstance()->GetAudio();
    auto system = Manager::GetInstance()->GetAudio()->GetSystem();

    // 既に存在しないか
    AudioClip* clip = FindClip(path, loop, false, stream);
    if (clip != nullptr)
        return clip;

    // クリップを作成する
    clip = new AudioClip();
    clip->m_path = path;
    clip->m_loop = loop;
    clip->m_isStream = stream;
    clip->m_is2D = false;

    // サウンドを生成する
    FMOD_RESULT result;
    if (stream)
    {
        result = system->createStream(
            path,
            FMOD_3D | (loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF),
            nullptr,
            &clip->m_sound
        );
    }
    else
    {
        result = system->createSound(
            path,
            FMOD_3D | (loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF),
            nullptr,
            &clip->m_sound
        );
    }

    // 失敗したとき
    if (result != FMOD_OK)
    {
        Log::SendLog(static_cast<std::string>(path) + " のサウンド生成に失敗しました", Log::TYPE_ERROR);
        delete clip;
        clip = nullptr;
        return clip;
    }

    // リストに追加する
    m_audioclips.emplace_back(clip);

    return clip;
}

//=============================================================
// オーディオクリップが既に存在しているか
//=============================================================
AudioClip* AudioClip::FindClip(const char* path, const bool& loop, const bool& is2D, const bool& stream)
{
    for (auto itr = m_audioclips.begin(); itr != m_audioclips.end(); itr++)
    {
        if (path == (*itr)->m_path &&
            loop == (*itr)->m_loop &&
            is2D == (*itr)->m_is2D &&
            stream == (*itr)->m_isStream)
        { // 一致したとき
            return *itr;
        }
    }

    // 見つからなかったとき
    return nullptr;
}

//=============================================================
// オーディオクリップを解放する
//=============================================================
bool AudioClip::Release(AudioClip* clip)
{
    // システムを取得する
    auto system = Manager::GetInstance()->GetAudio()->GetSystem();

    auto itr = std::find(m_audioclips.begin(), m_audioclips.end(), clip);
    if (itr != m_audioclips.end())
    { // 見つかった
        AudioClip* findClip = *itr;
        m_audioclips.erase(itr);
        findClip->m_sound->release();
        delete findClip;
        findClip = nullptr;
        return true;
    }

    // 見つからなかった
    Log::SendLog("解放するオーディオクリップが見つかりませんでした", Log::TYPE_WARNING);
    return false;
}

//=============================================================
// すべてのオーディオクリップを解放する
//=============================================================
void AudioClip::AllRelease()
{
    for (auto itr = m_audioclips.begin(); itr != m_audioclips.end(); itr++)
    {
        (*itr)->m_sound->release();
        delete* itr;
        *itr = nullptr;
    }
    m_audioclips.clear();
}

//=============================================================
// 更新
//=============================================================
void AudioSource::Update()
{
    // 再生が終わったらチャンネルを空にする
    bool isPlaying = false;
    m_channel->isPlaying(&isPlaying);
    if (!isPlaying)
    {
        m_channel = nullptr;

        if (m_stopDestroy)
        {
            Destroy(gameObject);
        }
    }

    // オーディオの位置を設定する
    if (m_channel != nullptr)
    {
        FMOD_VECTOR pos = { 
            transform->GetWorldPosition().x,
            transform->GetWorldPosition().y,
            transform->GetWorldPosition().z
        };
        FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
        m_channel->set3DAttributes(&pos, &vel);
    }
}

//=============================================================
// 終了
//=============================================================
void AudioSource::Uninit()
{
    // エフェクトを削除する
    ClearEffect();

    // 音を止める
    Stop();
}

//=============================================================
// オーディオクリップを設定する
//=============================================================
void AudioSource::Play(AudioClip* clip, const Group& group, const bool& stopDestroy)
{
    if (clip == nullptr)
        return;

    // システムを取得する
    auto audio = Manager::GetInstance()->GetAudio();

    // 再生する（一時停止状態）
    FMOD_RESULT result = audio->GetSystem()->playSound(clip->GetSound(), nullptr, true, &m_channel);
    m_channel->setChannelGroup(audio->GetChannelGroup(group));
    if (result != FMOD_OK)
    { // 失敗
        Log::SendLog("音声の再生に失敗しました", Log::TYPE_WARNING);
        return;
    }

    // チャンネルにDSPを設定する
    int idx = 0;
    for (auto itr = m_dsps.begin(); itr != m_dsps.end(); itr++, idx++)
    {
        m_channel->addDSP(idx, (*itr).dsp);
    }

    // 情報更新
    Update();

    // 再生する
    SetPause(false);
    m_stopDestroy = stopDestroy;
}

//=============================================================
// 再生されているか
//=============================================================
bool AudioSource::IsPlaying()
{
    if (m_channel != nullptr)
    {
        bool isPlaying = false;
        m_channel->isPlaying(&isPlaying);
        return isPlaying;
    }
    return false;
}

//=============================================================
// 音量を設定する
//=============================================================
void AudioSource::SetVolume(const float& volume)
{
    if (m_channel != nullptr)
    {
        m_channel->setVolume(volume);
    }
}

//=============================================================
// ピッチを設定する
//=============================================================
void AudioSource::SetPitch(const float& pitch)
{
    if (m_channel != nullptr)
    {
        m_channel->setPitch(pitch);
    }
}

//=============================================================
// 停止する
//=============================================================
void AudioSource::Stop()
{
    if (m_channel != nullptr)
    {
        m_channel->stop();
    }
}

//=============================================================
// ポーズする
//=============================================================
void AudioSource::SetPause(const bool& paused)
{
    if (m_channel != nullptr)
    {
        m_channel->setPaused(paused);
    }
}

//=============================================================
// エフェクトを追加する
//=============================================================
FMOD::DSP* AudioSource::AddEffect(const FMOD_DSP_TYPE& type)
{
    // システムを取得する
    auto system = Manager::GetInstance()->GetAudio()->GetSystem();

    // 既にそのエフェクトが存在していないかを確認する
    for (auto itr = m_dsps.begin(); itr != m_dsps.end(); itr++)
    {
        if ((*itr).type == type)
        { // 一致したとき
            return (*itr).dsp;
        }
    }

    // データを作成する
    DSPData data;
    data.type = type;

    // DSPを作成する
    system->createDSPByType(type, &data.dsp);

    // DSPをリストに追加する
    m_dsps.emplace_back(data);

    return data.dsp;
}

//=============================================================
// エフェクトを削除する
//=============================================================
void AudioSource::RemoveEffect(const FMOD_DSP_TYPE& type)
{
    for (auto itr = m_dsps.begin(); itr != m_dsps.end(); itr++)
    {
        if ((*itr).type == type)
        {
            // チャンネルにアタッチされている場合は削除する
            if (m_channel != nullptr)
            {
                m_channel->removeDSP((*itr).dsp);
            }

            (*itr).dsp->release();
            (*itr).dsp = nullptr;
            m_dsps.erase(itr);
            break;
        }
    }
}

//=============================================================
// すべてのエフェクトを削除する
//=============================================================
void AudioSource::ClearEffect()
{
    // エフェクトを削除する
    for (auto itr = m_dsps.begin(); itr != m_dsps.end(); itr++)
    {
        // チャンネルにアタッチされている場合は削除する
        if (m_channel != nullptr)
        {
            m_channel->removeDSP((*itr).dsp);
        }

        (*itr).dsp->release();
        (*itr).dsp = nullptr;
    }
    m_dsps.clear();
}

//=============================================================
// オーディオリスナー
//=============================================================
void AudioListener::Update()
{
    // システムを取得する
    auto system = Manager::GetInstance()->GetAudio()->GetSystem();

    // 前方のベクトルを取得する
    D3DXVECTOR3 forwardPos = { sinf(transform->GetWorldEulerAngle().y), 0.0f, cosf(transform->GetWorldEulerAngle().y) };

    // 必要な情報をまとめる
    FMOD_VECTOR pos = {transform->GetWorldPosition().x, transform->GetWorldPosition().y, transform->GetWorldPosition().z };
    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
    FMOD_VECTOR forward = { forwardPos.x, forwardPos.y, forwardPos.z };
    FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };

    // リスナー
    system->set3DListenerAttributes(0, &pos, &vel, &forward, &up);
}


//=============================================================
// 初期化
//=============================================================
void AudioWallGeometry::Init()
{
    // システムを取得する
    auto system = Manager::GetInstance()->GetAudio()->GetSystem();

    // オーディオジオメトリを作成する
    system->createGeometry(2, 6, &m_geometry);

    // トランスフォームモニター
    m_monitor = new TransformMonitor(transform);

    // 壁の四隅
    FMOD_VECTOR p1 = { -0.5f, 0.5f, 0.0f };  // 左上
    FMOD_VECTOR p2 = { 0.5f, 0.5f, 0.0f };  // 右上
    FMOD_VECTOR p3 = { -0.5f, -0.5f, 0.0f };  // 左下
    FMOD_VECTOR p4 = { 0.5f, -0.5f, 0.0f };  // 右下

    // ポリゴンを追加する
    int idx = 0;
    FMOD_VECTOR tri0[3] = { p1, p2, p3 };
    FMOD_VECTOR tri1[3] = { p2, p3, p4 };
    m_geometry->addPolygon(m_directOcclusion, m_reverbOcclusion, m_doubleSided, 3, &tri0[0], &idx);
    m_geometry->addPolygon(m_directOcclusion, m_reverbOcclusion, m_doubleSided, 3, &tri1[0], &idx);

    // 前方のベクトルを取得する
    D3DXMATRIX rotMtx = transform->GetRotationMatrix();
    D3DXVECTOR3 forwardPos = { 0.0f, 0.0f, 1.0f };
    D3DXVec3TransformCoord(&forwardPos, &forwardPos, &rotMtx);

    // トランスフォームを設定する
    FMOD_VECTOR pos = { transform->GetWorldPosition().x, transform->GetWorldPosition().y, transform->GetWorldPosition().z };
    FMOD_VECTOR forward = { forwardPos.x, forwardPos.y, forwardPos.z };
    FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
    FMOD_VECTOR scale = { transform->GetWorldScale().x, transform->GetWorldScale().y, transform->GetWorldScale().z };
    m_geometry->setPosition(&pos);
    m_geometry->setRotation(&forward, &up);
    m_geometry->setScale(&scale);
}

//=============================================================
// 終了
//=============================================================
void AudioWallGeometry::Uninit()
{
    // ジオメトリを破棄する
    if (m_geometry != nullptr)
    {
        m_geometry->release();
        m_geometry = nullptr;
    }
}

//=============================================================
// 更新
//=============================================================
void AudioWallGeometry::Update()
{
    // トランスフォーム値が変更されていた場合は更新する
    if (m_monitor->HasChanged())
    {
        // 前方のベクトルを取得する
        D3DXMATRIX rotMtx = transform->GetRotationMatrix();
        D3DXVECTOR3 forwardPos = { 0.0f, 0.0f, 1.0f };
        D3DXVec3TransformCoord(&forwardPos, &forwardPos, &rotMtx);

        // トランスフォームを設定する
        FMOD_VECTOR pos = { transform->GetWorldPosition().x, transform->GetWorldPosition().y, transform->GetWorldPosition().z };
        FMOD_VECTOR forward = { forwardPos.x, forwardPos.y, forwardPos.z };
        FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
        FMOD_VECTOR scale = { transform->GetWorldScale().x, transform->GetWorldScale().y, transform->GetWorldScale().z };

        // 変更する
        m_geometry->setPosition(&pos);
        m_geometry->setRotation(&forward, &up);
        m_geometry->setScale(&scale);
    }

    // 情報の更新
    if (m_isUpdate)
    {
        m_geometry->setPolygonAttributes(0, m_directOcclusion, m_reverbOcclusion, m_doubleSided);
        m_geometry->setPolygonAttributes(1, m_directOcclusion, m_reverbOcclusion, m_doubleSided);
        m_isUpdate = false;
    }

    // アクティブ状態でオンオフを切り替える
    m_geometry->setActive(GetActive());


    //// ギズモ
    //D3DXVECTOR3 start = { -0.5f, 0.5f, 0.0f };
    //D3DXVECTOR3 end = { 0.5f, -0.5f, 0.0f };
    //start.x *= m_size.x;
    //start.y *= m_size.y;
    //end.x *= m_size.x;
    //end.y *= m_size.y;

    //// ギズモ描画
    //Gizmo.DrawBox(start, end, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), transform->GetMatrix());
}

//=============================================================
// サイズを設定する
//=============================================================
void AudioWallGeometry::SetSize(const D3DXVECTOR2& size)
{
    if (m_size != size)
    {
        m_size = size;
        FMOD_VECTOR scale = { transform->GetWorldScale().x * m_size.x, transform->GetWorldScale().y * m_size.y, transform->GetWorldScale().z };
        m_geometry->setScale(&scale);
    }
}

//=============================================================
// 直接音遮蔽率を設定する
//=============================================================
void AudioWallGeometry::SetDirectOcclusion(const float& value)
{
    if (m_directOcclusion != value)
    {
        m_directOcclusion = value;
        m_isUpdate = true;
    }
}

//=============================================================
// 残響音遮蔽率を設定する
//=============================================================
void AudioWallGeometry::SetReverbOcclusion(const float& value)
{
    if (m_reverbOcclusion != value)
    {
        m_reverbOcclusion = value;
        m_isUpdate = true;
    }
}

//=============================================================
// 両面かを設定する
//=============================================================
void AudioWallGeometry::SetDoubleSided(const bool& enabled)
{
    if (m_doubleSided != enabled)
    {
        m_doubleSided = enabled;
        m_isUpdate = true;
    }
}


//=============================================================
// 初期化
//=============================================================
void AudioReverb::Init()
{
    // システムを取得する
    auto system = Manager::GetInstance()->GetAudio()->GetSystem();

    // リバーブを作成する
    system->createReverb3D(&m_reverb);

    // プロパティを設定する
    m_properties = FMOD_PRESET_CAVE;
    m_reverb->setProperties(&m_properties);
}

//=============================================================
// 終了
//=============================================================
void AudioReverb::Uninit()
{
    if (m_reverb != nullptr)
    {
        m_reverb->release();
        m_reverb = nullptr;
    }
}

//=============================================================
// 更新
//=============================================================
void AudioReverb::Update()
{
    // 情報を更新する
    FMOD_VECTOR pos = { transform->GetWorldPosition().x, transform->GetWorldPosition().y, transform->GetWorldPosition().z };
    FMOD_RESULT result = m_reverb->set3DAttributes(&pos, m_min, m_max);

    // アクティブ状態を監視する
    result = m_reverb->setActive(true);
}

//=============================================================
// プロパティを設定する
//=============================================================
void AudioReverb::SetProperties(FMOD_REVERB_PROPERTIES properties)
{
    m_properties = properties;
    m_reverb->setProperties(&m_properties);
}

//=============================================================
// 影響範囲を設定する
//=============================================================
void AudioReverb::SetRange(const float& min, const float& max)
{
    m_min = min;
    m_max = max;
}

//=============================================================
// 初期化
//=============================================================
void PhysicalAudioSource::Init()
{
    m_impacts.clear();
    m_rolling.audioClip = nullptr;
    m_rolling.volume = 1.0f;

    m_state = STATE::IDLE;
    m_group = AudioSystem::Group::SFX;
    m_prevVelocity = btVector3(0.0f, 0.0f, 0.0f);
}

//=============================================================
// 終了
//=============================================================
void PhysicalAudioSource::Uninit()
{
    // 回転を停止する
    if (m_rollingChannel != nullptr)
    {
        m_rollingChannel->stop();
        m_rollingChannel = nullptr;
    }
}

//=============================================================
// 更新
//=============================================================
void PhysicalAudioSource::Update()
{
    RigidBody* rigidBody = gameObject->GetComponent<RigidBody>();
    if (rigidBody == nullptr) return;

    btRigidBody* rb = rigidBody->GetRigidBody();
    if (rb == nullptr) return;

    // 状態を取得する
    STATE nextState = GetState(rb);

    // 音の処理
    if (nextState != m_state)
    {
        OnExitState(m_state);
        OnEnterState(nextState);
        m_state = nextState;
    }
    OnUpdateState(m_state);


    // 回転音の位置を更新する
    if (m_rollingChannel != nullptr)
    {
        FMOD_VECTOR pos = {
            transform->GetWorldPosition().x,
            transform->GetWorldPosition().y,
            transform->GetWorldPosition().z
        };
        FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
        m_rollingChannel->set3DAttributes(&pos, &vel);
    }
}

//=============================================================
// 衝撃音を追加する
//=============================================================
void PhysicalAudioSource::AddImpactAudio(AudioClip* clip, const float& volume)
{
    if (clip == nullptr) return;
    if (clip->IsLoop()) return;

    // クリップが重複しているか
    for (auto data : m_impacts)
    {
        if (data.audioClip == clip)
        {
            return;
        }
    }

    // データを加える
    AudioData data;
    data.audioClip = clip;
    data.volume = volume;
    m_impacts.push_back(data);
}

//=============================================================
// 回転音を設定する
//=============================================================
void PhysicalAudioSource::SetRollingAudio(AudioClip* clip, const float& volume)
{
    if (clip == nullptr) return;
    if (!clip->IsLoop()) return;

    // クリップが重複しているか
    if (m_rolling.audioClip == clip)
    {
        return;
    }

    // 既に再生されていたら停止する
    if (m_rollingChannel != nullptr)
    {
        m_rollingChannel->stop();
        m_rollingChannel = nullptr;
    }

    // データをセットする
    m_rolling.audioClip = clip;
    m_rolling.volume = volume;

    // システムを取得する
    auto audio = Manager::GetInstance()->GetAudio();

    // 再生する（一時停止状態）
    FMOD_RESULT result = audio->GetSystem()->playSound(m_rolling.audioClip->GetSound(), nullptr, true, &m_rollingChannel);
    if (result != FMOD_OK)
    { // 失敗
        Log::SendLog("音声の再生に失敗しました", Log::TYPE_WARNING);
        return;
    }

    // 音の情報
    m_rollingChannel->setVolume(m_rolling.volume);
    m_rollingChannel->setChannelGroup(audio->GetChannelGroup(m_group));

    // 位置
    FMOD_VECTOR pos = {
        transform->GetWorldPosition().x,
        transform->GetWorldPosition().y,
        transform->GetWorldPosition().z
    };
    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
    m_rollingChannel->set3DAttributes(&pos, &vel);
}

//=============================================================
// 状態更新
//=============================================================
PhysicalAudioSource::STATE PhysicalAudioSource::GetState(btRigidBody* rigidBody)
{
    // ポーズ時はアイドルで返す
    //if (Pause::IsPaused()) return STATE::IDLE;

    btVector3 v = rigidBody->getLinearVelocity();
    btVector3 av = rigidBody->getAngularVelocity();

    float speed = v.length();
    float angular = av.length();

    float threshold = 10.0f;
    m_deltaV = m_prevVelocity - v;
    m_prevVelocity = v;

    if (m_deltaV.length() > threshold)
    {
        return STATE::IMPACT;
    }
    else if (speed > 0.4f && angular > 0.4f)
    {
        return STATE::ROLLING;
    }
    else
    {
        return STATE::IDLE;
    }
}

//=============================================================
// 状態
//=============================================================
void PhysicalAudioSource::OnExitState(const STATE& current)
{
    if (current == STATE::ROLLING)
    {
        if (m_rollingChannel != nullptr)
        {
            m_rollingChannel->setPaused(true);
        }
    }
}
void PhysicalAudioSource::OnEnterState(const STATE& next)
{
    switch (next)
    {
    case STATE::IMPACT:
        PlayImpact();
        break;

    case STATE::ROLLING:  
        if (m_rollingChannel != nullptr)
        {
            m_rollingChannel->setPaused(false);
        }
        break;
    }
}
void PhysicalAudioSource::OnUpdateState(const STATE& current)
{
    btRigidBody* rigidBody = gameObject->GetComponent<RigidBody>()->GetRigidBody();

    if (current == STATE::ROLLING)
    {
        // 音量を速度に応じて更新
        if (m_rollingChannel != nullptr)
        {
            float baseVolume = m_rolling.volume;
            float speed = rigidBody->getLinearVelocity().length();
            float speedFactor = std::clamp(speed / 5.0f, 0.0f, 1.0f);

            float finalVolume = baseVolume * speedFactor;

            m_rollingChannel->setVolume(finalVolume);
        }
    }
}

//=============================================================
// 再生
//=============================================================
void PhysicalAudioSource::PlayImpact()
{
    // 再生する音が無いとき
    if (m_impacts.empty())
    {
        return;
    }

    float baseVolume = 0.8f;      // 既存のベース音量
    float threshold = 0.1f;      // 音を鳴らす最小の衝撃
    float maxDeltaV = 5.0f;      // 最大衝撃として正規化
    float impactStrength = m_deltaV.length();

    if (!(impactStrength > threshold))
    {
        return;
    }

    // ランダムで再生する音を決める
    int randIdx = rand() % m_impacts.size();
    AudioData audioData = m_impacts[randIdx];


    // 衝撃の強さを 0〜1 に正規化
    float factor = std::clamp((impactStrength - threshold) / (maxDeltaV - threshold), 0.0f, 1.0f);
    float finalVolume = audioData.volume * factor;  // ベース音量に衝撃係数を掛ける

    // ピッチもランダムで決める
    float pitch = Benlib::RandomFloat(0.95f, 1.05f);

    // システムを取得する
    auto audio = Manager::GetInstance()->GetAudio();

    // 再生する（一時停止状態）
    FMOD::Channel* channel;
    FMOD_RESULT result = audio->GetSystem()->playSound(audioData.audioClip->GetSound(), nullptr, true, &channel);
    if (result != FMOD_OK)
    { // 失敗
        Log::SendLog("音声の再生に失敗しました", Log::TYPE_WARNING);
        return;
    }

    // 音の情報
    channel->setVolume(finalVolume);
    channel->setPitch(pitch);
    channel->setChannelGroup(audio->GetChannelGroup(m_group));

    // 位置
    FMOD_VECTOR pos = {
    transform->GetWorldPosition().x,
    transform->GetWorldPosition().y,
    transform->GetWorldPosition().z
    };
    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
    channel->set3DAttributes(&pos, &vel);

    // 再生する
    channel->setPaused(false);
}