#ifndef _V3D_SHADER_PROGRAM_GL_H_
#define _V3D_SHADER_PROGRAM_GL_H_

#include "renderer/ShaderProgram.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for shader program management. GL render only.
    */
    class CShaderProgramGL : public CShaderProgram
    {
    public:

        CShaderProgramGL();
        ~CShaderProgramGL();

        bool                create()    override;
        void                destroy()   override;
        void                bind()      override;
        void                unbind()    override;

        u32                 getShaderProgramID()   const;

    private:

        bool                useProgram(u32 shaderProgram);

        bool                init(const std::vector<u32>& shaders);
        bool                link();
        bool                validate();

        void                getShaderIDArray(std::vector<u32>& shaders);

        bool                applyUniform       (CShaderUniform* uniform)                    override;
        void                applyUniformInt    (s32 location, s32 value)                    override;
        void                applyUniformFloat  (s32 location, f32 value)                    override;
        void                applyUniformVector2(s32 location, const core::Vector2D& vector) override;
        void                applyUniformVector3(s32 location, const core::Vector3D& vector) override;
        void                applyUniformVector4(s32 location, const core::Vector4D& vector) override;
        void                applyUniformMatrix3(s32 location, const core::Matrix3D& matrix) override;
        void                applyUniformMatrix4(s32 location, const core::Matrix4D& matrix) override;

        u32                 m_shaderProgramID;

        static u32          s_currentShader;

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_SHADER_PROGRAM_GL_H_
