//------------------------------------------------------------
// @file			PostEffectCollection.h
// @brief		ポストエフェクト集
//------------------------------------------------------------
#ifndef _POST_EFFECT_COLLECTION_H_
#define _POST_EFFECT_COLLECTION_H_

#include "components/other/post_effect.h"

namespace PostFX
{
	//@brief グレースケール
	class Grayscale : public PostEffect
	{
	public:
		Grayscale() : PostEffect("data\\SHADER\\POST_EFFECT\\grayscale.fx", { "Grayscale" }) {}
	};

	//@brief ブラー
	class Blur : public PostEffect
	{
	public:
		Blur() : PostEffect("data\\SHADER\\POST_EFFECT\\blur.fx", { "Blur_Horizontal", "Blur_Vertical" }) {}

		//@brief ブラーの強さを調整する
		void SetPower(const float& power) 
		{ 
			GetShader()->SetFloat("g_BlurScale", power); 
			m_power = power;
		}
		
		//@brief ブラーの強さを取得する
		float GetPower() { return m_power; }

	private:
		float m_power = 1.0f;
	};

	//@brief 明るさ
	class Brightness : public PostEffect
	{
	public:
		Brightness() : PostEffect("data\\SHADER\\POST_EFFECT\\brightness.fx", { "Brightness" }) {}

		//@brief 明るさを調整する
		void SetPower(const float& power) { GetShader()->SetFloat("g_Brightness", power); }
	};

	//@brief VHS
	class VHS : public PostEffect
	{
	public:
		VHS() : PostEffect("data\\SHADER\\POST_EFFECT\\vhs.fx", { "VHS" }) {}
	};
}

#endif // !_POST_EFFECT_COLLECTION_H_
