#ifndef _V3D_RENDER_STATE_H_
#define _V3D_RENDER_STATE_H_

#include "common.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EWinding
    {
        eWindingCW,
        eWindingCCW
    };

    enum ECompareFunc
    {
        eCmpLess,
        eCmpLequal,
        eCmpEqual,
        eCmpGequal,
        eCmpGreater,
        eCmpNotequal,
        eCmpAlways,
        eCmpNever,

        eCompareCount
    };

    enum EPolygonMode
    {
        ePolyModeFill,
        ePolyModeLine,
        ePolyModePoint,

        eModeCount
    };

    enum ECullFace
    {
        eFaceFront,
        eFaceBack,
        eFaceFrontAndBack,

        eCullfaceCount
    };

    enum EBlendFactor
    {
        eBlendZero,
        eBlendOne,
        eBlendSrcColor,
        eBlendInvSrcColor,
        eBlendDstColor,
        eBlendInvDstColor,
        eBlendSrcAlpha,
        eBlendInvSrcAlpha,
        eBlendDstAlpha,
        eBlendInvDstAlpha,
        eBlendSrcAplhaSaturated,

        eBlendCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderState
    {
    public:

        CRenderState();
        virtual                   ~CRenderState();

        ECullFace                 getCullface()       const;
        bool                      isCulling()        const;
        bool                      isBlend()          const;
        EWinding                  getWinding()        const;
        EPolygonMode              getPolygonMode()    const;
        EBlendFactor              getBlendFactorSrc() const;
        EBlendFactor              getBlendFactorDst() const;
        bool                      getDepthWrite()     const;
        bool                      getDepthTest()      const;
        ECompareFunc              getDepthfunc()      const;
                              
        void                      setCullface(ECullFace mode);
        void                      setCulling(bool enable);
        void                      setBlend(bool enable);
        void                      setWinding(EWinding type);
        void                      setPolygonMode(EPolygonMode type);
        void                      setBlendFactors(EBlendFactor dst, EBlendFactor src);
        void                      setDepthWrite(bool enable);
        void                      setDepthTest(bool enable);
        void                      setDepthFunc(ECompareFunc func);

        virtual void              bind() = 0;

        bool                      parse(const tinyxml2::XMLElement* root);

        static EPolygonMode       getPolygonModeByName(const std::string& name);
        static EBlendFactor       getBlendFactorByName(const std::string& name);
        static ECullFace          getCullFaceByName(const std::string& name);
        static ECompareFunc       getCompareFuncByName(const std::string& name);

    protected:

        bool                      m_culling;
        ECullFace                 m_cullface;
        EWinding                  m_winding;
        EPolygonMode              m_polygonMode;

        EBlendFactor              m_blendSrc;
        EBlendFactor              m_blendDst;
        bool                      m_blend;

        bool                      m_depthWrite;
        bool                      m_depthTest;
        ECompareFunc              m_depthFunc;

    private:

        static const std::string  s_renderPolygonMode[EPolygonMode::eModeCount];
        static const std::string  s_blendFactor[EBlendFactor::eBlendCount];
        static const std::string  s_cullface[ECullFace::eCullfaceCount];
        static const std::string  s_comparefunc[ECompareFunc::eCompareCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderState> RenderStatePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_STATE_H_
