//********************************************************************************************************
//
//メイン処理[main.h]
// Author Murata Shinnosuke
//
//********************************************************************************************************
#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#define DIRECTINPUT_VERSION (0x0800)
#include "d3dx9.h"
#include "dinput.h"
#include "xaudio2.h"
#include "Xinput.h"

//ライブラリのリンク
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "xinput.lib")  

//マクロ定義
#define SCREEN_WIDTH (1280) //ウインドウの幅
#define SCREEN_HEIGHT (720) //ウインドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define SECONDS_FLAME (60)   //１秒間のフレーム
#define CLASS_NAME "WindowClass"    //ウィンドウクラスの名前
#define WINDOW_NAME "BreakFall" //ウインドウの名前
#define MAX_STRING (128)

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;   //頂点座標
	float rhw;   //座標変換用係数
	D3DCOLOR col;   //頂点カラー
	D3DXVECTOR2 tex;   //テクスチャ座標
}VERTEX_2D;   //頂点情報[2D]

//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos; //頂点座標
	D3DXVECTOR3 nor;   //法線ベクトル
	D3DCOLOR col;	 //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
} VERTEX_3D;

#endif // !_MAIN_H_
