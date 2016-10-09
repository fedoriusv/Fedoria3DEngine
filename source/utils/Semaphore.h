#pragma once

#include "common.h"

namespace v3d
{
namespace utils
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class Semaphore
    {
    public:

        explicit Semaphore(u32 countThreads);
        ~Semaphore();

        void wait();
        bool tryWait();
        void post();

    private:

        u32                     m_countThreads;

        std::condition_variable m_cv;
        std::mutex              m_mutex;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace utils
} //namespace v3d