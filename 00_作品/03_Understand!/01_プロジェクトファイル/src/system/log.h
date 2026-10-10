//------------------------------------------------------------
// @file		log.h
// @brief	ログ
// @detail	ログの出力をサポートします
//------------------------------------------------------------
#ifndef _LOG_H_
#define _LOG_H_

//-----------------------------------------------------------
// 設定項目

// ログをターミナルに出力するか
#define SHOW_LOG_TERMINAL	(false)

// ログをファイルとして出力するか
#define OUTPUT_LOG_FILE		(false)

// ログを保存するパス
#define OUTPUT_LOG_PATH		"logs\\log-%data.txt"

// ログテンプレート
#define LOG_TEMPLATE				"%time > [%type] %message\n"

// タグの設定
#define LOG_TAG_NORMAL		"LOG"
#define LOG_TAG_WARNING		"WARNING"
#define LOG_TAG_ERROR			"ERROR"

//-----------------------------------------------------------


//@brief ログクラス
class Log final
{
public:
	//@brief 種類
	enum TYPE
	{
		TYPE_NORMAL,		// 通常
		TYPE_WARNING,	// 注意
		TYPE_ERROR,		// エラー
	};

	//@brief ログデータ
	struct LogData
	{
		TYPE type;					// 種類
		std::string message;	// メッセージ
		time_t time;					// 時刻
	};

	//@brief ログを送信する
	//@param message: 送信するメッセージ
	//@param type: ログの種類
	static void SendLog(std::string message, TYPE type = TYPE_NORMAL);

	//@brief ログデータからログ文を生成する
	static std::string WriteLog(LogData data);

	//@brief ログをファイルとして出力する
	static void OutputLog();

private:
	//static std::vector<LogData> m_logDatas;	// ログのデータ
};

#endif // !_LOG_H_
