#include "FontData.h"

using namespace v3d;
using namespace v3d::resources;

CFontData::CFontData(const std::string& font)
    : m_fontType(EFontType::eUnknownFont)
    , m_font(font)
    , m_fontSize(32U)
    , m_mapSize(k_fontMapSize, k_fontMapSize)
{
}

CFontData::~CFontData()
{
}

CFontData::EFontType CFontData::getFontType() const
{
    return m_fontType;
}

void CFontData::setFontType(EFontType type)
{
    m_fontType = type;
}

const CFontData::SCharDesc* CFontData::getCharInfo(const s32 charCode) const
{
    std::map<s32, SCharDesc>::const_iterator it = m_charInfo.find(charCode);
    if (it == m_charInfo.end())
    {
        return nullptr;
    };

    return &it->second;
}

const std::string& CFontData::getFontName() const
{
    return m_font;
}

void CFontData::setFontSize(const u32 size)
{
    m_fontSize = size;
}

const u32 CFontData::getFontSize() const
{
    return m_fontSize;
}