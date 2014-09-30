#ifndef _V3D_FONT_DATA_H_
#define _V3D_FONT_DATA_H_

#include "stream/Resource.h"
#include "renderer/Texture.h"

namespace v3d
{
namespace scene
{
    class CFontManager;
    class CText;
}
namespace resources
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    const u32 k_fontMapSize = 256U;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CFontData : public stream::CResource
    {
    public:

        enum class EFontType
        {
            eUnknownFont,
            eBitmapFont,
            eVectorFont
        };

        struct SCharDesc
        {
            u32                 _srcX;
            u32                 _srcY;
            u32                 _width;
            u32                 _height;
            s32                 _advX;
            s32                 _advY;
            s32                 _offX;
            s32                 _offY;
            u32                 _page;

            std::vector<s32>    _kerningPairs;

            SCharDesc()
                : _srcX(0)
                , _srcY(0)
                , _width(0)
                , _height(0)
                , _advX(0)
                , _advY(0)
                , _offX(0)
                , _offY(0)
                , _page(0)
            {}
        };

        CFontData(const std::string& font);
        virtual                             ~CFontData();

        EFontType                           getFontType() const;
        const std::string&                  getFontName() const;

        const SCharDesc*                    getCharInfo(const s32 charCode) const;

        void                                setFontSize(const u32 size);
        const u32                           getFontSize() const;

    protected:

        friend                              scene::CFontManager;
        friend                              scene::CText;

        void                                setFontType(EFontType type);

        virtual bool                        addCharsToMap(const std::string& text) = 0;
        virtual bool                        loadFont(const std::string& resource)  = 0;

        EFontType                           m_fontType;

        std::map<s32, SCharDesc>            m_charInfo;
        std::vector<renderer::TexturePtr>   m_charTexture;
        Dimension2D                         m_mapSize;

        std::string                         m_font;
        u32                                 m_fontSize;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CFontData>         FontDataPtr;
    typedef std::map<std::string, FontDataPtr> FontDataList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_FONT_DATA_H_
