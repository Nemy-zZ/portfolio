//------------------------------------------------------------
// @file		log.cpp
// @brief	ログ
// @detail	ログの出力をサポートします
//------------------------------------------------------------
#include "log.h"
#include <regex>
#include <fstream>
using namespace std;

// 静的メンバ変数の初期化
//std::vector<Log::LogData> Log::m_logDatas;

//=============================================================
// ログの送信
//=============================================================
void Log::SendLog(std::string message, TYPE type)
{
	// ログデータを作成する
	LogData data;
	data.type = type;
	data.message = message;

    // 時刻
    data.time = time(nullptr);
        
#if SHOW_LOG_TERMINAL
	// ログをターミナルに出力する
	OutputDebugString(WriteLog(data).c_str());
#endif

    // データを保存する
    //m_logDatas.push_back(data);
}

//=============================================================
// ログデータからログ文を生成する
//=============================================================
std::string Log::WriteLog(LogData data)
{
    // ログのテンプレートを設定する
    string log_template = LOG_TEMPLATE;

    // 時間を置き換える
    regex patternTime("%time");

    // 時間をテキストに起こす
    char buffer[128];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", localtime(&data.time));
    log_template = regex_replace(log_template, patternTime, buffer);

    // ログの種類を置き換える
    regex patternType("%type");
    string type_char = "";
    switch (data.type)
    {
    case TYPE_NORMAL:
        type_char = LOG_TAG_NORMAL;
        break;
    case TYPE_WARNING:
        type_char = LOG_TAG_WARNING;
        break;
    case TYPE_ERROR:
        type_char = LOG_TAG_ERROR;
        break;
    }
    log_template = regex_replace(log_template, patternType, type_char);

    // メッセージを置き換える
    regex patternMessage("%message");
    log_template = regex_replace(log_template, patternMessage, data.message);

    // 返す
    return log_template;
}

//=============================================================
// ログをファイルとして出力する
//=============================================================
void Log::OutputLog()
{
    //// ログのパスを設定する
    //string log_output = OUTPUT_LOG_PATH;

    //// 時間をテキストに起こす
    //time_t t = time(nullptr);
    //tm* now = localtime(&t);
    //char buffer[128];
    //strftime(buffer, sizeof(buffer), "%Y-%d-%m %H.%M.%S", now);

    //// 日時を置き換える
    //regex patternData("%data");
    //log_output = regex_replace(log_output, patternData, buffer);

    //// ファイルに書き出す
    //ofstream output;
    //output.open(log_output, ios::out);
    //for (auto itr = m_logDatas.begin(); itr != m_logDatas.end(); itr++)
    //{
    //    output << WriteLog(*itr);
    //}
    //output.close();
}