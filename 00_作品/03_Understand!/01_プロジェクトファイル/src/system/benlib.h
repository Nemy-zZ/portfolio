//------------------------------------------------------------
// @file		benlib.h
// @brief	便利関数ライブラリ
// @detail	使用頻度の高い処理がまとめられています
//------------------------------------------------------------
#ifndef _BENLIB_H_
#define _BENLIB_H_

#define D3DXMATRIX_IDENTITY (D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, 1.0f))

//@brief 便利な機能を提供します
namespace Benlib
{
	/**
	 * @brief 座標1と座標2の距離を計算します
	 * @param[in] pos1 : 座標1
	 * @param[in] pos2 : 座標2
	 * @return 距離 (float)
	*/
	float PosDistance(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

	/**
	 * @brief 座標1と座標2の平面距離を計算します
	 * @param[in] pos1 : 座標1
	 * @param[in] pos2 : 座標2
	 * @return 平面距離 (float)
	*/
	float PosPlaneDistance(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

	/**
	 * @brief 方向ベクトルと長さから座標を計算します
	 * @param[in] start : スタート位置
	 * @param[in] vec : 方向ベクトル
	 * @param[in] length : 距離
	 * @return 座標 (D3DXVECTOR3)
	*/
	D3DXVECTOR3 VectorDistance(const D3DXVECTOR3& start, D3DXVECTOR3 vec, const float& length);

	/**
	 * @brief 座標1から座標2の角度を計算します
	 * @param[in] pos1 : 座標1
	 * @param[in] pos2 : 座標2
	 * @return 角度 (float)
	*/
	float PosAngle(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

	/**
	 * @brief 座標1から座標2の回転を計算します
	 * @param[in] pos1 : 座標1
	 * @param[in] pos2 : 座標2
	 * @return 回転 (D3DXVECTOR3)
	*/
	D3DXVECTOR3 PosRotation(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

	/**
	 * @brief 法線ベクトルを算出します
	 * @param[in] pos1 : 座標1
	 * @param[in] pos2 : 座標2
	 * @param[in] pos3 : 座標3
	 * @return 法線ベクトル (D3DXVECTOR3)
	*/
	D3DXVECTOR3 CalcNormalVector(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& pos3);

	/**
	 * @brief 座標1から座標2の回転を計算します
	 * @param[in] pos1 : 座標1
	 * @param[in] pos2 : 座標2
	 * @return 回転 (D3DXVECTOR3)
	*/
	D3DXQUATERNION LookAt(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);


	/**
	 * @brief 整数型のランダム値を生成します
	 * @param[in] min : 最小値
	 * @param[in] max : 最大値
	 * @return ランダム値 (int)
	*/
	int RandomInt(const int& min, const int& max);

	/**
	 * @brief 浮動小数点型のランダム値を生成します
	 * @param[in] min : 最小値
	 * @param[in] max : 最大値
	 * @return ランダム値 (float)
	*/
	float RandomFloat(const float& min, const float& max);

	/**
	 * @brief 角度からラジアンに変換します
	 * @param[in] degree : 角度
	 * @return ラジアン (float)
	*/
	float DegreeToRadian(const float& degree);

	/**
	@brief ラジアンから角度に変換します
	@param[in] radian : ラジアン
	@return 角度 (float)
	*/
	float RadianToDegree(const float& radian);

	/**
	 * @brief クォータニオンからYXZオイラー角に変換します
	 * @param[in] q : クォータニオン
	 * @return オイラー角（YXZ）
	*/
	D3DXVECTOR3 QuaternionToEuler(D3DXQUATERNION q);

	/**
	 * @brief ファイルポインタを現在位置から指定数分戻します
	 * @param[in] file : ファイルポインタ
	 * @param[in] backCount : 戻る文字数
	*/
	void BackFileChar(FILE* file, const int& backCount);

	/**
	 * @brief パスを指定数分削除します
	 * @param[out] filePath : パス (文字列)
	 * @param[in] split : パス数
	*/
	void RemovePathSplit(char* filePath, const int& split);
	void RemovePathSplit(std::string& filePath, const int& split);

	/**
	 * @brief パスの区切りを指定文字に置き換える
	 * @param[out] filePath : パス (文字列)
	 * @param[in] split : 新しい区切り文字
	*/
	void ReplacePathSplit(char* filePath, const char& split);


#ifdef CV_VERSION_MAJOR
	/**
	 * @brief cv::MatをLPDIRECT3DTEXTURE9に変換する
	 * @param[in] mat : OpenCVの画像データ
	 * @param[out] outTexture : 変換後のテクスチャ
	*/
	bool MatToTexture(const cv::Mat& mat, LPDIRECT3DTEXTURE9* outTexture);
#endif


#ifdef BULLET_COLLISION_COMMON_H
	/**
	 * @brief D3DXVECTOR3をbtVector3に変換します
	*/
	btVector3 ToBtVector3(const D3DXVECTOR3& value);
#endif
}

#endif