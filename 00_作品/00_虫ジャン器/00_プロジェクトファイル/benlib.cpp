//========================================================================================================
//
//便利関数処理[benlib.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#include "benlib.h"

//========================================================================================================
//対象との距離の算出処理
//========================================================================================================
D3DXVECTOR3 Benlib::Distance(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget)
{
	//距離を計算する
	float fDistanceYZ = (posTarget.y - pos.y) * (posTarget.y - pos.y) + (posTarget.z - pos.z) * (posTarget.z - pos.z);
	fDistanceYZ = (float)sqrt(fDistanceYZ);   //平方根

	//距離を計算する
	float fDistanceXY = (posTarget.x - pos.x) * (posTarget.x - pos.x) + (posTarget.y - pos.y) * (posTarget.y - pos.y);
	fDistanceXY = (float)sqrt(fDistanceXY);   //平方根

	//距離を計算する
	float fDistanceXZ = (posTarget.x - pos.x) * (posTarget.x - pos.x) + (posTarget.z - pos.z) * (posTarget.z - pos.z);
	fDistanceXZ = (float)sqrt(fDistanceXZ);   //平方根

	//距離を返す
	return {fDistanceYZ, fDistanceXZ, fDistanceXY };
}

//========================================================================================================
//対象の方向の算出処理
//========================================================================================================
D3DXVECTOR3 Benlib::Direction(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget)
{
	//ベクトルを計算する
	D3DXVECTOR3 vec = posTarget - pos;

	//方向を計算する
	float fDirectionYZ = atan2f(vec.y, vec.z);
	float fDirectionXY = atan2f(vec.x, vec.y);
	float fDirectionXZ = atan2f(vec.x, vec.z);

	//方向を返す
	return{fDirectionYZ, fDirectionXZ, fDirectionXY };
}

//========================================================================================================
//乱数の生成処理
//========================================================================================================
int Benlib::CreateRand(int nMax, int nMin)
{
	//最低値を絶対値にする
	int nMinAbs = abs(nMin);

	//乱数を生成する
	int nRand = rand() % (nMax + nMinAbs);
	nRand -= nMinAbs;

	//乱数を生成する
	return nRand;
}