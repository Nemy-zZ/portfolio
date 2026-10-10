//------------------------------------------------------------
// @file			text.h
// @brief		テキスト
// @detail	フォントを描画します
//------------------------------------------------------------
#include "text.h"
#include <regex>

const TextUI::FontData TextUI::loadFonts[TextUI::FontType::MAX] = {
    {TextUI::FontType::ZENKAKU_GOTHIC, "data\\FONT\\ZenKakuGothicNew-Bold.ttf", "Zen Kaku Gothic New"},
};

//=============================================================
// フォントの読み込み
//=============================================================
void TextUI::InitLoadFont()
{
    std::string failedFont = "";
    int failedCounter = 0;
    for (int i = 0; i < FontType::MAX; i++)
    {
        if (!(AddFontResourceEx(loadFonts[i].path.c_str(), FR_PRIVATE, NULL) > 0))
        { // 失敗
            Log::SendLog(loadFonts[i].name + " のフォント読み込みに失敗しました！", Log::TYPE_ERROR);
        }
    }
}

//=============================================================
// 初期化
//=============================================================
void TextUI::Init()
{
    // スプライトを作成する
    if (FAILED(D3DXCreateSprite(Manager::GetInstance()->GetDevice(), &m_sprite)))
    { // 失敗時
        Log::SendLog("フォント表示用スプライトの生成に失敗しました", Log::TYPE_ERROR);
    }

    // 初期設定
    m_fontName = loadFonts[0].name;
    m_fontSize = 100;
    m_outlineSize = 0;
    m_outlineColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_fillColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_align = ALIGN::LEFT;
    m_alpha = 1.0f;
}

//=============================================================
// 終了
//=============================================================
void TextUI::Uninit()
{
    // テキストデータの破棄
    for (int i = 0; i < (int)m_textInfos.size(); i++)
    {
        // フォントテクスチャの破棄
        if (m_textInfos[i].texture != nullptr)
        {
            m_textInfos[i].texture->Release();
            m_textInfos[i].texture = nullptr;
        }
    }
    m_textInfos.clear();

    // スプライトの破棄
    if (m_sprite != nullptr)
    {
        m_sprite->Release();
        m_sprite = nullptr;
    }
}

//=============================================================
// UI描画
//=============================================================
void TextUI::DrawUI()
{
    // スケールと回転のマトリックス
    D3DXMATRIX mtx, mtxScale, mtxRot;
    D3DXMatrixIdentity(&mtx);
    D3DXMatrixScaling(&mtxScale, transform->GetWorldScale().x, transform->GetWorldScale().y, transform->GetWorldScale().z);
    mtxRot = transform->GetRotationMatrix();
    D3DXMatrixMultiply(&mtx, &mtxScale, &mtxRot);

    // スプライトにマトリックスを適用する
    m_sprite->SetTransform(&mtx);
    m_sprite->Begin(D3DXSPRITE_ALPHABLEND);

    for (auto itr = m_textInfos.begin(); itr != m_textInfos.end(); itr++)
    {
        D3DXVECTOR3 pos = {
            transform->GetWorldPosition().x + (*itr).pos.x,
            transform->GetWorldPosition().y - (*itr).pos.y,
            0.0f
        };

        D3DXMATRIX matScale, matTrans, matWorld;
        D3DXMatrixScaling(&matScale, itr->scale.x, itr->scale.y, 1.0f);
        D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);
        D3DXMatrixMultiply(&matWorld, &matScale, &matTrans);

        m_sprite->SetTransform(&matWorld);
        m_sprite->Draw(itr->texture, 0, 0, 0, D3DXCOLOR(1, 1, 1, m_alpha));
    }
    m_sprite->End();
}

//=============================================================
// UTF-8かを判別する
//=============================================================
bool IsUtf8(const std::string& str)
{
    // BOM (Byte Order Mark) のチェック
    // UTF-8 BOM: EF BB BF
    if (str.length() >= 3 &&
        (unsigned char)str[0] == 0xEF &&
        (unsigned char)str[1] == 0xBB &&
        (unsigned char)str[2] == 0xBF)
    {
        return true; // BOMがあればUTF-8とみなす
    }

    // UTF-8バイトシーケンスの基本的な妥当性チェック
    size_t i = 0;
    while (i < str.length())
    {
        unsigned char c = (unsigned char)str[i];

        if (c <= 0x7F) // 1バイト文字 (ASCII)
        {
            i++;
            continue;
        }
        else if ((c & 0xE0) == 0xC0) // 2バイト文字: 110xxxxx 10xxxxxx
        {
            if (i + 1 >= str.length() || ((unsigned char)str[i + 1] & 0xC0) != 0x80)
                return false; // 不正な継続バイト
            i += 2;
        }
        else if ((c & 0xF0) == 0xE0) // 3バイト文字: 1110xxxx 10xxxxxx 10xxxxxx
        {
            if (i + 2 >= str.length() ||
                ((unsigned char)str[i + 1] & 0xC0) != 0x80 ||
                ((unsigned char)str[i + 2] & 0xC0) != 0x80)
                return false; // 不正な継続バイト
            i += 3;
        }
        else if ((c & 0xF8) == 0xF0) // 4バイト文字: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        {
            if (i + 3 >= str.length() ||
                ((unsigned char)str[i + 1] & 0xC0) != 0x80 ||
                ((unsigned char)str[i + 2] & 0xC0) != 0x80 ||
                ((unsigned char)str[i + 3] & 0xC0) != 0x80)
                return false; // 不正な継続バイト
            i += 4;
        }
        else
        {
            return false; // 不正な開始バイト
        }
    }

    return true; // 上記のチェックを通過すればUTF-8
}

//=============================================================
// テキストの設定
//=============================================================
void TextUI::SetText(const std::string& text)
{
    if (m_oldText != text)
    {
        // 変換前のテキストを保存しておく
        m_oldText = text;

        if (IsUtf8(text))
        {
            m_text = text;
        }
        else
        {
            int wideCharLen = MultiByteToWideChar(CP_ACP, 0, text.c_str(), -1, NULL, 0);
            if (wideCharLen == 0)
            {
                Log::SendLog("SetTextでANSIテキストをワイド文字に変換できませんでした", Log::TYPE_ERROR);
                m_text = "";
                return;
            }

            std::vector<wchar_t> wTextBuf(wideCharLen);
            if (MultiByteToWideChar(CP_ACP, 0, text.c_str(), -1, wTextBuf.data(), wideCharLen) == 0)
            {
                Log::SendLog("SetTextでANSIテキストをワイド文字に変換できませんでした", Log::TYPE_ERROR);
                m_text = "";
                return;
            }
            std::wstring wText(wTextBuf.data());

            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wText.c_str(), -1, NULL, 0, NULL, NULL);
            if (utf8Len == 0)
            {
                Log::SendLog("SetTextでワイド文字をUTF-8に変換できませんでした", Log::TYPE_ERROR);
                m_text = "";
                return;
            }

            std::vector<char> utf8TextBuf(utf8Len);
            if (WideCharToMultiByte(CP_UTF8, 0, wText.c_str(), -1, utf8TextBuf.data(), utf8Len, NULL, NULL) == 0)
            {
                Log::SendLog("SetTextでワイド文字をUTF-8に変換できませんでした", Log::TYPE_ERROR);
                m_text = "";
                return;
            }

            // 変換されたUTF-8文字列
            m_text = utf8TextBuf.data();
        }

        // テキストの更新
        UpdateText();
    }
}

//=============================================================
// 色の設定
//=============================================================
void TextUI::SetColor(const D3DXCOLOR& color)
{
    if (m_fillColor != color || m_outlineColor != color)
    {
        m_fillColor = color;
        m_outlineColor = color;

        // テキストの更新
        UpdateText();
    }
}

//=============================================================
// フォントサイズの設定
//=============================================================
void TextUI::SetFontSize(const int& size)
{
    if (m_fontSize != size)
    {
        m_fontSize = size;

        // テキストの更新
        UpdateText();
    }
}

//=============================================================
// フォント名の設定
//=============================================================
void TextUI::SetFont(const FontType& fontType)
{
    if (m_fontName != loadFonts[fontType].name)
    {
        m_fontName = loadFonts[fontType].name;

        // テキストの更新
        UpdateText();
    }
}

//=============================================================
// 整列の設定
//=============================================================
void TextUI::SetAlign(const ALIGN& align)
{
    if (m_align != align)
    {
        m_align = align;

        // テキストの更新
        UpdateText();
    }
}

//=============================================================
// 透明度の設定
//=============================================================
void TextUI::SetAlpha(const float& fAlpha)
{
    m_alpha = fAlpha;
}

//=============================================================
// アウトラインサイズの設定
//=============================================================
void TextUI::SetOutlineSize(const int& size)
{
    if (m_outlineSize != size)
    {
        m_outlineSize = size;

        // テキストの更新
        UpdateText();
    }
}

//=============================================================
// アウトラインの色設定
//=============================================================
void TextUI::SetOutlineColor(const D3DXCOLOR& color)
{
    if (m_outlineColor != color)
    {
        m_outlineColor = color;

        // テキストの更新
        UpdateText();
    }
}

//=============================================================
// フォントの色設定
//=============================================================
void TextUI::SetFontColor(const D3DXCOLOR& color)
{
    if (m_fillColor != color)
    {
        m_fillColor = color;

        // テキストの更新
        UpdateText();
    }
}

//=============================================================
// テキストの更新
//=============================================================
void TextUI::UpdateText()
{
    // 今までのテクスチャを破棄する
    for (int i = 0; i < (int)m_textInfos.size(); i++)
    {
        // フォントテクスチャの破棄
        if (m_textInfos[i].texture != nullptr)
        {
            m_textInfos[i].texture->Release();
            m_textInfos[i].texture = nullptr;
        }
    }
    m_textInfos.clear();

    // タグを処理した後の文字列をUTF-16 (wchar_t) に変換する
    int wideCharLen = MultiByteToWideChar(CP_UTF8, 0, m_text.c_str(), -1, NULL, 0);
    if (wideCharLen == 0)
    {
        Log::SendLog("処理されたテキストをワイド文字に変換できませんでした", Log::TYPE_ERROR);
        return;
    }
    std::vector<wchar_t> wTextBuf(wideCharLen);
    if (MultiByteToWideChar(CP_UTF8, 0, m_text.c_str(), -1, wTextBuf.data(), wideCharLen) == 0)
    {
        Log::SendLog("処理されたテキストをワイド文字に変換できませんでした", Log::TYPE_ERROR);
        return;
    }
    std::wstring showWText(wTextBuf.data());

    // 表示テキスト (タグを処理するための一時的なコピー)
    //std::wstring processedText = showWText;

    // テキストタグ
    std::vector<TextTag*> vecTextTag;

    // サイズタグ
    std::wsmatch match;
    auto it = showWText.cbegin();
    while (std::regex_search(it, showWText.cend(), match, std::regex(R"(<size=(\d+)>)")))
    {
        SizeTextTag* pSizeTag = new SizeTextTag();
        pSizeTag->SetSize(_wtoi(match.str(1).c_str()));

        pSizeTag->SetIdx(static_cast<int>(match.position(0)));
        showWText.erase(match.position(0), match.length());
        vecTextTag.push_back(pSizeTag);

        // タグの位置調整
        for (int i = 0; i < (int)vecTextTag.size(); i++)
        {
            if (vecTextTag[i]->GetIdx() > match.position(0))
            {
                vecTextTag[i]->SetIdx(vecTextTag[i]->GetIdx() - static_cast<int>(match.length()));
            }
        }
        it = showWText.cbegin(); // 削除後にイテレータを再設定
    }

    // サイズタグ（リセット）
    it = showWText.cbegin();
    while (std::regex_search(it, showWText.cend(), match, std::regex(R"(<size=reset>)")))
    {
        SizeTextTag* pSizeTag = new SizeTextTag();
        pSizeTag->SetSize(m_fontSize);

        pSizeTag->SetIdx(static_cast<int>(match.position(0)));
        showWText.erase(match.position(0), match.length());
        vecTextTag.push_back(pSizeTag);

        // タグの位置調整
        for (int i = 0; i < (int)vecTextTag.size(); i++)
        {
            if (vecTextTag[i]->GetIdx() > match.position(0))
            {
                vecTextTag[i]->SetIdx(vecTextTag[i]->GetIdx() - static_cast<int>(match.length()));
            }
        }
        it = showWText.cbegin(); // 削除後にイテレータを再設定
    }

    // カラータグ
    it = showWText.cbegin();
    while (std::regex_search(it, showWText.cend(), match, std::regex(R"(<color=(\d+),\s?(\d+),\s?(\d+)>)")))
    {
        ColorTextTag* pColorTag = new ColorTextTag();
        pColorTag->SetColor(D3DCOLOR_RGBA(_wtoi(match.str(1).c_str()), _wtoi(match.str(2).c_str()), _wtoi(match.str(3).c_str()), 255));

        pColorTag->SetIdx(static_cast<int>(match.position(0)));
        showWText.erase(match.position(0), match.length());
        vecTextTag.push_back(pColorTag);

        // タグの位置調整
        for (int i = 0; i < (int)vecTextTag.size(); i++)
        {
            if (vecTextTag[i]->GetIdx() > match.position(0))
            {
                vecTextTag[i]->SetIdx(vecTextTag[i]->GetIdx() - static_cast<int>(match.length()));
            }
        }
        it = showWText.cbegin();
    }

    // カラータグ（リセット）
    it = showWText.cbegin();
    while (std::regex_search(it, showWText.cend(), match, std::regex(R"(<color=reset>)")))
    {
        ColorTextTag* pColorTag = new ColorTextTag();
        pColorTag->SetFillColor(m_fillColor);
        pColorTag->SetEdgeColor(m_outlineColor);

        pColorTag->SetIdx(static_cast<int>(match.position(0)));
        showWText.erase(match.position(0), match.length());
        vecTextTag.push_back(pColorTag);

        // タグの位置調整
        for (int i = 0; i < (int)vecTextTag.size(); i++)
        {
            if (vecTextTag[i]->GetIdx() > match.position(0))
            {
                vecTextTag[i]->SetIdx(vecTextTag[i]->GetIdx() - static_cast<int>(match.length()));
            }
        }
        it = showWText.cbegin();
    }

    // フォントカラータグ
    it = showWText.cbegin();
    while (std::regex_search(it, showWText.cend(), match, std::regex(R"(<font-color=(\d+),\s?(\d+),\s?(\d+)>)")))
    {
        FontColorTextTag* pFontColorTag = new FontColorTextTag();
        pFontColorTag->SetColor(D3DCOLOR_RGBA(_wtoi(match.str(1).c_str()), _wtoi(match.str(2).c_str()), _wtoi(match.str(3).c_str()), 255));

        pFontColorTag->SetIdx(static_cast<int>(match.position(0)));
        showWText.erase(match.position(0), match.length());
        vecTextTag.push_back(pFontColorTag);

        // タグの位置調整
        for (int i = 0; i < (int)vecTextTag.size(); i++)
        {
            if (vecTextTag[i]->GetIdx() > match.position(0))
            {
                vecTextTag[i]->SetIdx(vecTextTag[i]->GetIdx() - static_cast<int>(match.length()));
            }
        }
        it = showWText.cbegin();
    }

    // フォントカラータグ（リセット）
    it = showWText.cbegin();
    while (std::regex_search(it, showWText.cend(), match, std::regex(R"(<font-color=reset>)")))
    {
        FontColorTextTag* pFontColorTag = new FontColorTextTag();
        pFontColorTag->SetColor(m_fillColor);

        pFontColorTag->SetIdx(static_cast<int>(match.position(0)));
        showWText.erase(match.position(0), match.length());
        vecTextTag.push_back(pFontColorTag);

        // タグの位置調整
        for (int i = 0; i < (int)vecTextTag.size(); i++)
        {
            if (vecTextTag[i]->GetIdx() > match.position(0))
            {
                vecTextTag[i]->SetIdx(vecTextTag[i]->GetIdx() - static_cast<int>(match.length()));
            }
        }
        it = showWText.cbegin();
    }

    // エッジカラータグ
    it = showWText.cbegin();
    while (std::regex_search(it, showWText.cend(), match, std::regex(R"(<edge-color=(\d+),\s?(\d+),\s?(\d+)>)")))
    {
        EdgeColorTextTag* pEdgeColorTag = new EdgeColorTextTag();
        pEdgeColorTag->SetColor(D3DCOLOR_RGBA(_wtoi(match.str(1).c_str()), _wtoi(match.str(2).c_str()), _wtoi(match.str(3).c_str()), 255));

        pEdgeColorTag->SetIdx(static_cast<int>(match.position(0)));
        showWText.erase(match.position(0), match.length());
        vecTextTag.push_back(pEdgeColorTag);

        // タグの位置調整
        for (int i = 0; i < (int)vecTextTag.size(); i++)
        {
            if (vecTextTag[i]->GetIdx() > match.position(0))
            {
                vecTextTag[i]->SetIdx(vecTextTag[i]->GetIdx() - static_cast<int>(match.length()));
            }
        }
        it = showWText.cbegin();
    }

    // エッジカラータグ（リセット）
    it = showWText.cbegin();
    while (std::regex_search(it, showWText.cend(), match, std::regex(R"(<edge-color=reset>)")))
    {
        EdgeColorTextTag* pEdgeColorTag = new EdgeColorTextTag();
        pEdgeColorTag->SetColor(m_outlineColor);

        pEdgeColorTag->SetIdx(static_cast<int>(match.position(0)));
        showWText.erase(match.position(0), match.length());
        vecTextTag.push_back(pEdgeColorTag);

        // タグの位置調整
        for (int i = 0; i < (int)vecTextTag.size(); i++)
        {
            if (vecTextTag[i]->GetIdx() > match.position(0))
            {
                vecTextTag[i]->SetIdx(vecTextTag[i]->GetIdx() - static_cast<int>(match.length()));
            }
        }
        it = showWText.cbegin();
    }

    // スペースタグ
    it = showWText.cbegin();
    while (std::regex_search(it, showWText.cend(), match, std::regex(R"(<space=(\d+)>)")))
    {
        SpaceTextTag* pSpaceTag = new SpaceTextTag();
        pSpaceTag->SetSpace((float)_wtoi(match.str(1).c_str()));

        pSpaceTag->SetIdx(static_cast<int>(match.position(0)));
        showWText.erase(match.position(0), match.length());
        vecTextTag.push_back(pSpaceTag);

        // タグの位置調整
        for (int i = 0; i < (int)vecTextTag.size(); i++)
        {
            if (vecTextTag[i]->GetIdx() > match.position(0))
            {
                vecTextTag[i]->SetIdx(vecTextTag[i]->GetIdx() - static_cast<int>(match.length()));
            }
        }
        it = showWText.cbegin();
    }

    // アイコンタグ
    it = showWText.cbegin();
    std::wregex iconRegex(LR"(<icon=([a-zA-Z0-9_]+)(?:,w=(\d+),h=(\d+))?>)");
    while (std::regex_search(it, showWText.cend(), match, iconRegex))
    {
        IconTextTag* pIconTag = new IconTextTag();

        // 名前
        std::wstring iconNameW = match.str(1);
        std::string iconName(iconNameW.begin(), iconNameW.end());  // UTF-8変換
        pIconTag->SetName(iconName);

        // サイズ
        if (match.size() == 4 && match[2].matched && match[3].matched)
        {
            int w = _wtoi(match.str(2).c_str());
            int h = _wtoi(match.str(3).c_str());
            pIconTag->SetSize(w, h);
        }

        pIconTag->SetIdx(static_cast<int>(match.position(0)));
        showWText.erase(match.position(0), match.length());
        vecTextTag.push_back(pIconTag);

        // 位置調整
        for (int i = 0; i < (int)vecTextTag.size(); i++)
        {
            if (vecTextTag[i]->GetIdx() > match.position(0))
            {
                vecTextTag[i]->SetIdx(vecTextTag[i]->GetIdx() - static_cast<int>(match.length()));
            }
        }

        it = showWText.cbegin();
    }


    int nTextLength = static_cast<int>(showWText.length());
    float fTextWidth = 0.0f;
    float fTextHeight = 0.0f;
    int fontSize = m_fontSize;
    D3DXCOLOR fillColor = m_fillColor;
    D3DXCOLOR edgeColor = m_outlineColor;
    int lineCounter = 0;
    std::vector<float> lineHeights;
    std::vector<float> lineLengths;

    for (int i = 0; i < nTextLength; i++)
    {
        TextInfo textInfo;
        RECT strInfo = { 0, 0, 0, 0 }; // strInfo を初期化

        // テキストタグ
        for (int n = 0; n < (int)vecTextTag.size(); n++)
        {
            if (vecTextTag[n]->GetIdx() == i)
            {
                if (vecTextTag[n]->GetType() == TextTag::TYPE::SIZE)
                {
                    SizeTextTag* sizeTag = (SizeTextTag*)vecTextTag[n];
                    fontSize = sizeTag->GetSize();
                }
                else if (vecTextTag[n]->GetType() == TextTag::TYPE::COLOR)
                {
                    ColorTextTag* colorTag = (ColorTextTag*)vecTextTag[n];
                    fillColor = colorTag->GetFillColor();
                    edgeColor = colorTag->GetEdgeColor();
                }
                else if (vecTextTag[n]->GetType() == TextTag::TYPE::FONT_COLOR)
                {
                    FontColorTextTag* fontColorTag = (FontColorTextTag*)vecTextTag[n];
                    fillColor = fontColorTag->GetColor();
                }
                else if (vecTextTag[n]->GetType() == TextTag::TYPE::EDGE_COLOR)
                {
                    EdgeColorTextTag* edgeColorTag = (EdgeColorTextTag*)vecTextTag[n];
                    edgeColor = edgeColorTag->GetColor();
                }
                else if (vecTextTag[n]->GetType() == TextTag::TYPE::SPACE)
                {
                    SpaceTextTag* spaceTag = (SpaceTextTag*)vecTextTag[n];

                    // 現在の行の長さを記録する
                    lineLengths.push_back(fTextWidth);
                    lineHeights.push_back(fTextHeight);

                    lineCounter++;
                    fTextWidth = 0.0f; // 次の行に移るので幅をリセット
                    fTextHeight = spaceTag->GetSpace();
                }
                else if (vecTextTag[n]->GetType() == TextTag::TYPE::ICON)
                {
                    IconTextTag* iconTag = (IconTextTag*)vecTextTag[n];
                    TextInfo textIconInfo;
                    textIconInfo.texture = nullptr;

                    // アイコンのファイルパス構築
                    std::string filePath = "data/TEXTURE/ICON/ICON_" + iconTag->GetName() + ".png";

                    if (FAILED(D3DXCreateTextureFromFile(Manager::GetInstance()->GetDevice(), filePath.c_str(), &textIconInfo.texture)))
                    {
                        Log::SendLog(filePath + " の読み込みに失敗", Log::TYPE_ERROR);
                        continue;
                    }

                    D3DSURFACE_DESC desc;
                    textIconInfo.texture->GetLevelDesc(0, &desc);

                    float targetW = iconTag->GetWidth() > 0 ? (float)iconTag->GetWidth() : (float)desc.Width;
                    float targetH = iconTag->GetHeight() > 0 ? (float)iconTag->GetHeight() : (float)desc.Height;

                    textIconInfo.scale.x = targetW / (float)desc.Width;
                    textIconInfo.scale.y = targetH / (float)desc.Height;

                    textIconInfo.pos = { fTextWidth, targetH, 0.0f };

                    if (fTextHeight < targetH)
                        fTextHeight = targetH;

                    textIconInfo.line = lineCounter;
                    fTextWidth += targetW;
                    m_textInfos.push_back(textIconInfo);

                    if (i == nTextLength - 1)
                    {
                        lineLengths.push_back(fTextWidth);
                        lineHeights.push_back(fTextHeight);
                    }

                    continue;
                }
            }
        }

        // 現在の文字
        wchar_t currentChar[2] = { showWText[i], L'\0' };

        // 改行文字の処理
        if (currentChar[0] == L'\n')
        {
            // 現在の行の長さを記録する
            lineLengths.push_back(fTextWidth);
            lineHeights.push_back(fTextHeight);

            lineCounter++;
            fTextWidth = 0.0f; // 次の行に移るので幅をリセット
            fTextHeight = 0.0f;
            continue;
        }

        // フォントテクスチャの生成
        if (textInfo.texture == nullptr)
        {
            textInfo.texture = CreateFontTexture(
                m_fontName.c_str(),
                DEFAULT_CHARSET,
                currentChar,
                fontSize,
                0,
                m_outlineSize,
                edgeColor,
                fillColor,
                3,
                &strInfo
            );
        }

        if (textInfo.texture == nullptr)
        {
            Log::SendLog("フォントテクスチャの生成に失敗しました", Log::TYPE_ERROR);
            continue;
        }

        // 描画位置を決める
        textInfo.pos = { fTextWidth, (float)strInfo.bottom - (float)strInfo.top, 0.0f };

        // 現在の行の最大高さを更新
        if (fTextHeight < (float)strInfo.bottom - (float)strInfo.top)
        {
            fTextHeight = (float)strInfo.bottom - (float)strInfo.top;
        }

        textInfo.line = lineCounter;
        fTextWidth += strInfo.right; // 現在の行の幅を加算
        m_textInfos.push_back(textInfo);

        // 最後の処理
        if (i == nTextLength - 1)
        {
            lineLengths.push_back(fTextWidth);
            lineHeights.push_back(fTextHeight);
        }
    }

    for (int i = 0; i < (int)m_textInfos.size(); i++)
    {
        if (m_align == ALIGN::CENTER)
        { // 中央揃え
            m_textInfos[i].pos.x -= lineLengths[m_textInfos[i].line] * 0.5f;
        }
        else if (m_align == ALIGN::RIGHT)
        { // 右揃え
            m_textInfos[i].pos.x -= lineLengths[m_textInfos[i].line];
        }

        // 高さ
        float height = 0.0f;
        for (int n = 0; n < m_textInfos[i].line + 1; n++)
        {
            height += lineHeights[n];
        }
        m_textInfos[i].pos.y -= height;
    }

    // テキストタグを破棄する
    for (int i = 0; i < (int)vecTextTag.size(); i++)
    {
        if (vecTextTag[i] != nullptr)
        {
            delete vecTextTag[i];
            vecTextTag[i] = nullptr;
        }
    }
    vecTextTag.clear(); // vectorもクリア
}

//=============================================================
// フォントテクスチャの生成（1文字）
//=============================================================
IDirect3DTexture9* TextUI::CreateFontTexture(
    const char* faceName,
    unsigned char charSet,
    const wchar_t* wstr,
    unsigned fontHeight,
    unsigned weight,
    int penSize,
    D3DXCOLOR edgeColor,
    D3DXCOLOR fillColor,
    int quality,
    RECT* info)
{
    penSize *= quality;
    IDirect3DTexture9* tex = 0;

    // ハンドルとデバイスの取得
    HWND hWnd = Main::GetInstance().GetHWND();
    LPDIRECT3DDEVICE9 pDevice = Manager::GetInstance()->GetDevice();

    // デバイスコンテキスト作成
    HDC hDC = GetDC(hWnd);
    HDC memDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    // フォント作成
    LOGFONTA lf = {};
    lf.lfHeight = fontHeight * quality;
    lf.lfCharSet = charSet;
    lf.lfWeight = weight;
    lf.lfQuality = ANTIALIASED_QUALITY;
    memcpy(lf.lfFaceName, faceName, strlen(faceName));
    HFONT hFont = CreateFontIndirectA(&lf);
    HFONT oldFont = (HFONT)SelectObject(memDC, hFont);

    // 描画するBMPを作成
    TEXTMETRICA tm;
    GLYPHMETRICS gm;
    MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };

    UINT code = wstr[0];
    GetTextMetricsA(memDC, &tm);
    GetGlyphOutlineW(memDC, code, GGO_METRICS, &gm, 0, 0, &mat);

    RECT charRegion =
    {
            gm.gmptGlyphOrigin.x - penSize / 2,
            tm.tmAscent - gm.gmptGlyphOrigin.y - penSize / 2,
            gm.gmptGlyphOrigin.x + (LONG)gm.gmBlackBoxX + penSize / 2,
            tm.tmAscent - gm.gmptGlyphOrigin.y + (LONG)gm.gmBlackBoxY + penSize / 2
    };

    RECT charWH = { 0, 0, ((LONG)gm.gmBlackBoxX + penSize + quality - 1) / quality * quality, ((LONG)gm.gmBlackBoxY + penSize + quality - 1) / quality * quality };
    int bmpW = charWH.right;
    int bmpH = charWH.bottom;
    BITMAPINFO bmpInfo = {};
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = bmpW;
    bmpInfo.bmiHeader.biHeight = -bmpH;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 24;
    unsigned char* p = 0;
    HBITMAP hBitMap = CreateDIBSection(0, &bmpInfo, DIB_RGB_COLORS, (void**)&p, 0, 0);
    HBITMAP oldBMP = (HBITMAP)SelectObject(memDC, hBitMap);

    // BMP背景を青色で初期化
    HBRUSH bgBrush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 255));
    FillRect(memDC, &charWH, bgBrush);
    DeleteObject(bgBrush);

    // パスを描く
    // パス色は緑、塗は赤
    HPEN   hPen = (HPEN)CreatePen(PS_SOLID, penSize, RGB(0, 255, 0));
    HBRUSH hBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
    HPEN   oldPen = (HPEN)SelectObject(memDC, hPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, hBrush);

    SetBkMode(memDC, TRANSPARENT);
    BeginPath(memDC);
    TextOutW(memDC, -charRegion.left, -charRegion.top, wstr, 1);
    EndPath(memDC);
    StrokeAndFillPath(memDC);

    SelectObject(memDC, oldPen);
    SelectObject(memDC, oldBrush);
    SelectObject(memDC, oldFont);
    SelectObject(memDC, oldBMP);
    DeleteObject(hBrush);
    DeleteObject(hPen);
    DeleteObject(hFont);

    // DirectXのテクスチャにBMPの色分けを使いフォントを穿つ
    int texW = charWH.right / quality;
    int texH = charWH.bottom / quality;
    int q2 = quality * quality;
    if (FAILED(pDevice->CreateTexture(texW, texH, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex, 0)))
    {
        return nullptr;
    }
    D3DLOCKED_RECT lockR;
    if (SUCCEEDED(tex->LockRect(0, &lockR, 0, 0)))
    {
        char* d = (char*)lockR.pBits;
        unsigned BMPPitch = (charWH.right * 3 + 3) / 4 * 4;
        for (int y = 0; y < texH; y++) 
        {
            for (int x = 0; x < texW; x++) 
            {
                unsigned& v = *((unsigned*)d + x + y * texW);   // テクスチャのピクセル位置
                unsigned alph = 0;
                unsigned edge = 0;
                unsigned fill = 0;
                // quality倍率分点を平均化
                for (int i = 0; i < quality; i++) 
                {
                    for (int j = 0; j < quality; j++)
                    {
                        alph += p[(y * quality + i) * BMPPitch + (x * quality + j) * 3 + 0];
                        edge += p[(y * quality + i) * BMPPitch + (x * quality + j) * 3 + 1];
                        fill += p[(y * quality + i) * BMPPitch + (x * quality + j) * 3 + 2];
                    }
                }
                alph /= q2;
                edge /= q2;
                fill /= q2;

                // 色の形式変換
                int colEdge[4] = { (int)(edgeColor.r * 255), (int)(edgeColor.g * 255), (int)(edgeColor.b * 255), (int)(edgeColor.a * 255) };
                int colFill[4] = { (int)(fillColor.r * 255), (int)(fillColor.g * 255), (int)(fillColor.b * 255), (int)(fillColor.a * 255) };

                // 不透明の場合はブレンド色を採用
                unsigned a = 0xff - alph;
                if (a < 0xff) 
                {
                    // 半透明
                    unsigned r = colEdge[0];
                    unsigned g = colEdge[1];
                    unsigned b = colEdge[2];
                    a = (a * colEdge[3]) >> 8;
                    v = a << 24 | r << 16 | g << 8 | b;
                }
                else 
                {
                    // 不透明
                    unsigned r = ((colEdge[0] * edge) >> 8) + ((colFill[0] * fill) >> 8);
                    unsigned g = ((colEdge[1] * edge) >> 8) + ((colFill[1] * fill) >> 8);
                    unsigned b = ((colEdge[2] * edge) >> 8) + ((colFill[2] * fill) >> 8);
                    a = ((colEdge[3] * edge) >> 8) + ((colFill[3] * fill) >> 8);
                    v = a << 24 | r << 16 | g << 8 | b;
                }
            }
        }
        tex->UnlockRect(0);
    }

    DeleteObject(hBitMap);
    DeleteDC(memDC);

    if (info) 
    {
        info->left = charRegion.left / quality;
        info->top = charRegion.top / quality;
        info->right = (gm.gmCellIncX + penSize) / quality;
        info->bottom = (tm.tmHeight + penSize) / quality;
    }
    return tex;
}


//=============================================================
// コンストラクタ
//=============================================================
TypingTextUI::TypingTextUI()
{
    m_typingSpeed = 30;
    m_typingCounter = 0;
    m_typingNum = 0;
}

//=============================================================
// 更新
//=============================================================
void TypingTextUI::Update()
{
    m_typingCounter--;
    if (m_typingCounter <= 0 &&
        m_typingNum <= (int)m_textInfos.size())
    {
        m_typingNum++;

        // リセット
        m_typingCounter = m_typingSpeed;
    }
}

//=============================================================
// 描画
//=============================================================
void TypingTextUI::DrawUI()
{
    m_sprite->Begin(D3DXSPRITE_ALPHABLEND);

    for (int i = 0; i < (int)m_textInfos.size(); i++)
    {
        if (i < m_typingNum)
        {
            D3DXVECTOR3 pos = {
                transform->GetWorldPosition().x + m_textInfos[i].pos.x,
                transform->GetWorldPosition().y - m_textInfos[i].pos.y,
                0.0f
            };            
            m_sprite->Draw(m_textInfos[i].texture, 0, 0, &pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha));
        }
    }

    m_sprite->End();
}