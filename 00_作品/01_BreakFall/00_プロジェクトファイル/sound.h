//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	//SOUND_LABEL_TEST = 0,				//テストBGM
	SOUND_LABEL_SELECT_MOVE = 0,		//選択移動
	SOUND_LABEL_SELECT_DECIDE,		//選択決定
	SOUND_LABEL_DAMAGE,					//ダメージ
	SOUND_LABEL_BLOCK_BREAK,			//ブロック破壊音
	SOUND_LABEL_STICK_HIT,				//スティックヒット音
	SOUND_LABEL_RESULT,						//リザルトBGM
	SOUND_LABEL_TITLE,						//タイトルBGM
	SOUND_LABEL_GAME,						//ゲームBGM
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
