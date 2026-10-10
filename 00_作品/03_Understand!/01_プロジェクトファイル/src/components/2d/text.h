//------------------------------------------------------------
// @file			text.h
// @brief		テキスト
// @detail	フォントを描画します
//------------------------------------------------------------
#ifndef _TEXT_H_
#define _TEXT_H_

#include "component.h"

//@brief テキストコンポーネント
class TextUI : public Component
{
public:
	//@brief フォントの種類
	enum FontType
	{
		ZENKAKU_GOTHIC,		// Zen Kaku Gothic New
		MAX
	};

public:
	//@brief 整列
	enum ALIGN
	{
		LEFT,		// 左揃え
		CENTER,	// 中央揃え
		RIGHT		// 右揃え
	};

	//@brief 文字情報
	struct TextInfo
	{
		IDirect3DTexture9* texture = nullptr;				// 文字のテクスチャ
		D3DXVECTOR3 pos = {0.0f, 0.0f, 0.0f};		// 文字位置
		int line = 0;														// 行数
		RECT rect;														// 文字のサイズ情報を保存

		D3DXVECTOR2 scale = { 1.0f, 1.0f };
	};

	//@brief フォントデータ
	struct FontData
	{
		FontType type;
		std::string path;
		std::string name;
	};

	void Init() override;
	void Uninit() override;
	virtual void DrawUI() override;

	//@brief テキストを設定する
	void SetText(const std::string& text);
	//@brief 色を設定する
	void SetColor(const D3DXCOLOR& color);
	//@brief フォントサイズを設定する
	void SetFontSize(const int& size);
	//@brief フォントサイズを取得する
	int GetFontSize() { return m_fontSize; }
	//@brief フォントの色を設定する
	void SetFontColor(const D3DXCOLOR& color);
	//@brief 縁取りのサイズを設定する
	void SetOutlineSize(const int& size);
	//@brief 縁取りの色を設定する
	void SetOutlineColor(const D3DXCOLOR& color);
	//@brief フォントを設定する
	void SetFont(const FontType& fontType);
	//@brief 整列を設定する
	void SetAlign(const ALIGN& align);
	//@brief 透明度を設定する
	void SetAlpha(const float& fAlpha);
	//@brief 透明度を取得する
	float GetAlpha() { return m_alpha; }

	//@brief フォントを読み込む（初期化）
	static void InitLoadFont();
	static const FontData loadFonts[FontType::MAX];
protected:
	LPD3DXSPRITE m_sprite;					// スプライト
	std::vector<TextInfo> m_textInfos;		// 文字管理
	float m_alpha;									// 不透明度
private:
	void UpdateText();

	//@brief 1文字のテクスチャを生成する
	IDirect3DTexture9* CreateFontTexture(
		const char* faceName = "MS ゴシック",
		unsigned char charSet = DEFAULT_CHARSET,
		const wchar_t* wstr = L"T",
		unsigned fontHeight = 450,
		unsigned weight = 0,
		int penSize = 5,
		D3DXCOLOR edgeColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		D3DXCOLOR fillColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		int quality = 3,
		RECT* info = 0);

	// 情報
	std::string m_text;					// 文字
	std::string m_oldText;				// 前回のテキスト
	unsigned char m_charSet;		// 文字セット
	int m_fontSize;						// フォントサイズ
	float m_weight;						// フォントの太さ
	int m_outlineSize;					// アウトラインサイズ
	D3DXCOLOR m_fillColor;			// 塗りつぶし色
	D3DXCOLOR m_outlineColor;	// アウトライン色
	int m_quality;							// アンチエイリアス（1以上）
	RECT* m_info;
	ALIGN m_align;						// 整列

	// フォント情報
	std::string m_fontName;		// フォント名
};

// タイピングテキスト
class TypingTextUI : public TextUI
{
public:
	TypingTextUI();
	void Update() override;
	void DrawUI() override;
	void SetTypingSpeed(const int& nSpeed) { m_typingSpeed = nSpeed; }
private:
	int m_typingCounter;	// タイピングカウンター
	int m_typingSpeed;		// タイピングスピード
	int m_typingNum;		// タイピング済み
};

//@brief テキストタグ（基底）
class TextTag
{
public:
	enum TYPE
	{
		SIZE,
		COLOR,
		FONT_COLOR,
		EDGE_COLOR,
		SPACE,
		ICON,
	};
	TextTag(const TYPE& type) { m_type = type; }
	void SetIdx(const int& idx) { m_idx = idx; }
	int GetIdx() { return m_idx; }
	TYPE GetType() { return m_type; }
private:
	int m_idx;
	TYPE m_type;
};

//@brief  サイズタグ
class SizeTextTag : public TextTag
{
public:
	SizeTextTag() : TextTag(SIZE) {}
	void SetSize(const int& size) { m_size = size; }
	int GetSize() { return m_size; }
private:
	int m_size;
};

//@brief  カラータグ
class ColorTextTag : public TextTag
{
public:
	ColorTextTag() : TextTag(COLOR) {}
	void SetColor(const D3DXCOLOR& color) { 
		m_fillColor = color; 
		m_edgeColor = color;
	}
	void SetFillColor(const D3DXCOLOR& color) { m_fillColor = color; }
	void SetEdgeColor(const D3DXCOLOR& color) { m_edgeColor = color; }
	D3DXCOLOR GetFillColor() { return m_fillColor; }
	D3DXCOLOR GetEdgeColor() { return m_edgeColor; }
private:
	D3DXCOLOR m_fillColor;
	D3DXCOLOR m_edgeColor;
};

//@brief  フォントカラータグ
class FontColorTextTag : public TextTag
{
public:
	FontColorTextTag() : TextTag(FONT_COLOR) {}
	void SetColor(const D3DXCOLOR& color) { m_color = color; }
	D3DXCOLOR GetColor() { return m_color; }
private:
	D3DXCOLOR m_color;
};

//@brief  エッジカラータグ
class EdgeColorTextTag : public TextTag
{
public:
	EdgeColorTextTag() : TextTag(EDGE_COLOR) {}
	void SetColor(const D3DXCOLOR& color) { m_color = color; }
	D3DXCOLOR GetColor() { return m_color; }
private:
	D3DXCOLOR m_color;
};

//@brief スペースタグ
class SpaceTextTag : public TextTag
{
public:
	SpaceTextTag() : TextTag(SPACE) {}
	void SetSpace(const float& space) { m_space = space; }
	float GetSpace() { return m_space; }
private:
	float m_space;
};

//@brief アイコンタグ
class IconTextTag : public TextTag
{
public:
	IconTextTag() : TextTag(ICON)/*, m_width(0), m_height(0)*/ {}

	void SetName(const std::string& name) { strcpy(&m_name[0], name.c_str()); }
	std::string GetName() const { 
		std::string convert = m_name;
		return convert;
	}

	void SetSize(int w, int h) { m_width = w; m_height = h; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

private:
	char m_name[MAX_PATH];
	int m_width;
	int m_height;
};


#endif // !_TEXT_H_
