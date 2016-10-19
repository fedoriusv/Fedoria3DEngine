#pragma once

#include "common.h"
#include "utils/Thread.h"
#include "utils/Semaphore.h"
#include "renderer/Renderer.h"
#include "stream/MemoryStream.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ERenderCommand
    {
        eCommandInitialize = 0,

        eCommandUpdateTexure1D,
        eCommandUpdateTexure2D,
        eCommandUpdateTexure3D,
        eCommandUpdateTexureCube,
        eCommandReadTexture,
        eCommandReadTextureCube,
        eCommandFillTexure1D,
        eCommandFillTexure2D,
        eCommandFillTexure3D,

        eCommandDestroy,

        eCommandTerminate,
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderThread;

    class RenderStreamCommand
    {
    public:

        RenderStreamCommand(ERenderCommand cmd);
        ~RenderStreamCommand();

        template <class T>
        const T             readValue(u32 count = 1) const;
        void*               readValue(u32 size, u32 count = 1) const;

        template <class T>
        void                writeValue(const T& val, u32 count = 1);
        void                writeValue(const void* data, u32 size, u32 count = 1);

        void*               getStreamData() const;
        u32                 getStreamSize() const;

        ERenderCommand      getCommand() const;

    private:

        stream::MemoryStream m_commandStream;
        ERenderCommand       m_command;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class RenderThread final
    {
    public:

        explicit RenderThread(const renderer::RendererPtr renderer);
        ~RenderThread();

        void                                pushCommand(const RenderStreamCommand& command);
        void                                submit();
        void                                wait();

        void                                init();
                                            
        void                                beginFrame();
        void                                endFrame();
        void                                presentFrame();
                                            
        void                                draw();

        //

        void                                destroy();

        void                                terminate();

    private:

        const RenderStreamCommand           popCommand();

        void                                threadWorker(void* data);
        void                                runCommand(const RenderStreamCommand& command);

        std::atomic_bool                    m_isRunning;

        std::queue<RenderStreamCommand>     m_commandBufferQueue;

        utils::Thread                       m_thread;
        utils::Semaphore                    m_waitSemophore;

        renderer::RendererWPtr              m_renderer;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    template<class T>
    inline const T RenderStreamCommand::readValue(u32 count) const
    {
        T value;
        m_commandStream.read(&value, sizeof(T), count);

        return value;
    }

    template<class T>
    inline void RenderStreamCommand::writeValue(const T& val, u32 count)
    {
        const void* data = reinterpret_cast<const void*>(&val);
        m_commandStream.write(data, sizeof(T), count);
    }

} //namespace renderer
} //namespace v3d
