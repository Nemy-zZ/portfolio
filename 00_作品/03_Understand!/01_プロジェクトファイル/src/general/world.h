//------------------------------------------------------------
// @file			world.h
// @brief		ワールド
//------------------------------------------------------------
#ifndef _WORLD_H_
#define _WORLD_H_

/**
 * @brief ワールドを読み込みます
 * @param[in] path : PLCWファイルのパス
 * @param[in] isDestroy : 他のオブジェクトを破棄するか
*/
void LoadWorld(const std::string& path, const bool& isDestroy = false);

#endif // !_WORLD_H_
