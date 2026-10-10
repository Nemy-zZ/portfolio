//========================================================================================================
//
//モーションの処理[motion.cpp]
// Author MurataShinnosuke
//
//========================================================================================================
#include "motion.h"

//静的メンバ変数を初期化する
const int Motion::FRAME_BLEND = 3;   //ブレンドフレーム

//========================================================================================================
//初期化処理
//========================================================================================================
void Motion::Init()
{
}

//========================================================================================================
//終了処理
//========================================================================================================
void Motion::Uninit()
{
}

//========================================================================================================
//更新処理
//========================================================================================================
void Motion::Update()
{
	//キーの数を保存する
	int nKeyNum = this->m_vKey.size();

	//現在キーを確認する
	if (this->m_nKey < nKeyNum)
	{//現在キーがキーの数を超えていない場合
		//終了フラグを下ろす
		this->m_isEnd = false;

		//フレームを進める
		this->m_nFrame++;   //現在フレーム
		this->m_nFrameBlend++;   //ブレンドフレーム

		//向きの数分繰り返す
		for (unsigned int i = 0; i < this->m_vRot.size(); i++)
		{
			//次のキーの保存する
			int nKeyNext = this->m_nKey + 1;
			if (nKeyNext >= nKeyNum)
			{//次のキーがキーの数を超えた場合
				if (this->m_isLoop)
				{
					//次のキーを1番最初にする
					nKeyNext = 0;
				}
				else
				{
					//次のキーを現在キーの設定する
					nKeyNext = this->m_nKey;
				}
			}

			//次の向きとの差分を取る
			D3DXVECTOR3 rotDiff = this->m_vKey[nKeyNext].vRot[i] - this->m_vKey[this->m_nKey].vRot[i];

			//向きを計算する
			D3DXVECTOR3 rot =
			{
				this->m_vKey[this->m_nKey].vRot[i].x + ((rotDiff.x / this->m_vKey[this->m_nKey].nFrame) * this->m_nFrame),
				this->m_vKey[this->m_nKey].vRot[i].y + ((rotDiff.y / this->m_vKey[this->m_nKey].nFrame) * this->m_nFrame),
				this->m_vKey[this->m_nKey].vRot[i].z + ((rotDiff.z / this->m_vKey[this->m_nKey].nFrame) * this->m_nFrame)
			};

			//現在フレームを確認する
			if (this->m_nFrameBlend <= Motion::FRAME_BLEND)
			{
				//現在の位置と差分を取る
				rotDiff = rot - this->m_vRot[i];

				//向きを計算する
				D3DXVECTOR3 rot =
				{
					this->m_vRot[i].x + ((rotDiff.x / Motion::FRAME_BLEND) * this->m_nFrameBlend),
					this->m_vRot[i].y + ((rotDiff.y / Motion::FRAME_BLEND) * this->m_nFrameBlend),
					this->m_vRot[i].z + ((rotDiff.z / Motion::FRAME_BLEND) * this->m_nFrameBlend)
				};
			}

			//向きを反映させる
			this->m_vRot[i] = rot;
		}

		//現在フレームを確認する
		if (this->m_nFrame >= this->m_vKey[this->m_nKey].nFrame)
		{//現在フレームが現在キーのフレームを超えた場合
			//現在フレームをリセットする
			this->m_nFrame = 0;

			//次のキーに移行する
			this->m_nKey++;
			if (this->m_nKey >= nKeyNum)
			{//現在キーがキーの数を超えた場合
				//ループフラグを確認する
				if (this->m_isLoop == true)
				{//ループフラグが立っている場合
					//現在キーをリセットする
					this->m_nKey = 0;
				}
				else
				{
					//終了フラグを立てる
					this->m_isEnd = true;
				}
			}
		}
	}
}