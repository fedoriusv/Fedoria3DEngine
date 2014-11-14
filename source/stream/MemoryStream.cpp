#include "MemoryStream.h"

using namespace v3d;
using namespace v3d::stream;

MemoryStream::MemoryStream()
    : m_stream(nullptr)
    , m_length(0)
    , m_allocated(0)
    , m_pos(0)
{
}

MemoryStream::MemoryStream(const void* data, const u32 size)
    : m_stream((u8*)data)
    , m_length(0)
    , m_allocated(size)
    , m_pos(0)
{
}

MemoryStream::~MemoryStream()
{
    if (m_allocated)
    {
        delete[] m_stream;
        m_stream = nullptr;
    }
}

void MemoryStream::close()
{
}

u32 MemoryStream::read(void* buffer, const u32 size, const u32 count)
{
    ASSERT(m_pos + size * count <= m_length);

    memcpy(buffer, m_stream + m_pos, size * count);
    m_pos += size * count;

    return m_pos;
}

u32 MemoryStream::read(u8& value)
{
    ASSERT(m_length - m_pos >= sizeof(u8));

    value = m_stream[m_pos++];

    return m_pos;
}

u32 MemoryStream::read(s8& value)
{
    ASSERT(m_length - m_pos >= sizeof(s8));

    value = m_stream[m_pos++];

    return m_pos;
}

u32 MemoryStream::read(u16& value)
{
    ASSERT(m_length - m_pos >= sizeof(u16));

    value = (m_stream[m_pos++] & 0xFF) << 8;
    value |= m_stream[m_pos++] & 0xFF;

    return m_pos;
}

u32 MemoryStream::read(s16& value)
{
    ASSERT(m_length - m_pos >= sizeof(s16));

    value = (m_stream[m_pos++] & 0xFF) << 8;
    value |= m_stream[m_pos++] & 0xFF;

    return m_pos;
}

u32 MemoryStream::read(u32& value)
{
    ASSERT(m_length - m_pos >= sizeof(u32));

    value = (m_stream[m_pos++] & 0xFF) << 24;
    value |= (m_stream[m_pos++] & 0xFF) << 16;
    value |= (m_stream[m_pos++] & 0xFF) << 8;
    value |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MemoryStream::read(s32& value)
{
    ASSERT(m_length - m_pos >= sizeof(s32));

    value = (m_stream[m_pos++] & 0xFF) << 24;
    value |= (m_stream[m_pos++] & 0xFF) << 16;
    value |= (m_stream[m_pos++] & 0xFF) << 8;
    value |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MemoryStream::read(u64& value)
{
    ASSERT(m_length - m_pos >= sizeof(u64));

    //value = (m_stream[m_pos++] & 0xFF) << 56;
    //value |= (m_stream[m_pos++] & 0xFF) << 48;
    //value |= (m_stream[m_pos++] & 0xFF) << 40;
    //value |= (m_stream[m_pos++] & 0xFF) << 32;

    value |= (m_stream[m_pos++] & 0xFF) << 24;
    value |= (m_stream[m_pos++] & 0xFF) << 16;
    value |= (m_stream[m_pos++] & 0xFF) << 8;
    value |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MemoryStream::read(s64& value)
{
    ASSERT(m_length - m_pos >= sizeof(s64));

    //value = (m_stream[m_pos++] & 0xFF) << 56;
    //value |= (m_stream[m_pos++] & 0xFF) << 48;
    //value |= (m_stream[m_pos++] & 0xFF) << 40;
    //value |= (m_stream[m_pos++] & 0xFF) << 32;

    value |= (m_stream[m_pos++] & 0xFF) << 24;
    value |= (m_stream[m_pos++] & 0xFF) << 16;
    value |= (m_stream[m_pos++] & 0xFF) << 8;
    value |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MemoryStream::read(f32& value)
{
    ASSERT(m_length - m_pos >= sizeof(f32));

    s32& ival = *((s32*)&value);

    ival = (m_stream[m_pos++] & 0xFF) << 24;
    ival |= (m_stream[m_pos++] & 0xFF) << 16;
    ival |= (m_stream[m_pos++] & 0xFF) << 8;
    ival |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MemoryStream::read(f64& value)
{
    ASSERT(m_length - m_pos >= sizeof(f64));

    s32& ival = *((s32*)&value);

    ival = (m_stream[m_pos++] & 0xFF) << 24;
    ival |= (m_stream[m_pos++] & 0xFF) << 16;
    ival |= (m_stream[m_pos++] & 0xFF) << 8;
    ival |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MemoryStream::read(f80& value)
{
    ASSERT(m_length - m_pos >= sizeof(f80));

    s32& ival = *((s32*)&value);

    ival = (m_stream[m_pos++] & 0xFF) << 24;
    ival |= (m_stream[m_pos++] & 0xFF) << 16;
    ival |= (m_stream[m_pos++] & 0xFF) << 8;
    ival |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MemoryStream::read(bool& value)
{
    ASSERT(m_length - m_pos >= sizeof(bool));
    value = m_stream[m_pos++] != 0;

    return m_pos;
}

u32 MemoryStream::read(std::string& value)
{
    u32 size = 0;

    read(size);
    ASSERT(m_length - m_pos >= size);

    value.resize(size);
    memcpy(&value[0], &m_stream[m_pos++], size);

    return m_pos;
}

u32 MemoryStream::write(void* buffer, const u32 size, const u32 count)
{
    if (checkSize(size))
    {
        memcpy(m_stream + m_pos, buffer, size * count);
        m_pos += size * count;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const u8 value)
{
    if (checkSize(sizeof(u8)))
    {
        m_stream[m_pos++] = value;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const s8 value)
{
    if (checkSize(sizeof(s8)))
    {
        m_stream[m_pos++] = value;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const u16 value)
{
    if (checkSize(sizeof(u16)))
    {
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const s16 value)
{
    if (checkSize(sizeof(s16)))
    {
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const u32 value)
{
    if (checkSize(sizeof(u32)))
    {
        m_stream[m_pos++] = (value >> 24) & 0xFF;
        m_stream[m_pos++] = (value >> 16) & 0xFF;
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const s32 value)
{
    if (checkSize(sizeof(s32)))
    {
        m_stream[m_pos++] = (value >> 24) & 0xFF;
        m_stream[m_pos++] = (value >> 16) & 0xFF;
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const u64 value)
{
    if (checkSize(sizeof(u64)))
    {
        //m_stream[m_pos++] = (value >> 56) & 0xFF;
        //m_stream[m_pos++] = (value >> 48) & 0xFF;
        //m_stream[m_pos++] = (value >> 40) & 0xFF;
        //m_stream[m_pos++] = (value >> 32) & 0xFF;

        m_stream[m_pos++] = (value >> 24) & 0xFF;
        m_stream[m_pos++] = (value >> 16) & 0xFF;
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const s64 value)
{
    if (checkSize(sizeof(s64)))
    {
        //m_stream[m_pos++] = (value >> 56) & 0xFF;
        //m_stream[m_pos++] = (value >> 48) & 0xFF;
        //m_stream[m_pos++] = (value >> 40) & 0xFF;
        //m_stream[m_pos++] = (value >> 32) & 0xFF;

        m_stream[m_pos++] = (value >> 24) & 0xFF;
        m_stream[m_pos++] = (value >> 16) & 0xFF;
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const f32 value)
{
    if (checkSize(sizeof(f32)))
    {
        s32& ival = *((s32*)&value);

        m_stream[m_pos++] = (ival >> 24) & 0xFF;
        m_stream[m_pos++] = (ival >> 16) & 0xFF;
        m_stream[m_pos++] = (ival >> 8) & 0xFF;
        m_stream[m_pos++] = ival & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const f64 value)
{
    if (checkSize(sizeof(f64)))
    {
        s32& ival = *((s32*)&value);

        m_stream[m_pos++] = (ival >> 24) & 0xFF;
        m_stream[m_pos++] = (ival >> 16) & 0xFF;
        m_stream[m_pos++] = (ival >> 8) & 0xFF;
        m_stream[m_pos++] = ival & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const f80 value)
{
    if (checkSize(sizeof(f80)))
    {
        s32& ival = *((s32*)&value);

        m_stream[m_pos++] = (ival >> 24) & 0xFF;
        m_stream[m_pos++] = (ival >> 16) & 0xFF;
        m_stream[m_pos++] = (ival >> 8) & 0xFF;
        m_stream[m_pos++] = ival & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const bool value)
{
    if (checkSize(sizeof(bool)))
    {
        m_stream[m_pos++] = value;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MemoryStream::write(const std::string& value)
{
    if (value.empty())
    {
        return m_pos;
    }

    if (checkSize(sizeof(u32)))
    {
        MemoryStream::write(static_cast<u32>(value.size()));
    }

    if (checkSize(value.size()))
    {
        memcpy(m_stream + m_pos, &value[0], value.size());
        m_pos += value.size();

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

void MemoryStream::seekBeg(const u32 offset)
{
    ASSERT(offset <= m_length);
    m_pos = offset;
}

void MemoryStream::seekEnd(const u32 offset)
{
    ASSERT(offset <= m_length);
    m_pos = m_length + offset;
}

void MemoryStream::seekCur(const u32 offset)
{
    ASSERT(offset <= m_length);
    m_pos += offset;
}

u32 MemoryStream::tell()
{
    return m_pos;
}

u32 MemoryStream::size()
{
    return m_length;
}

u8* MemoryStream::getData() const
{
    return m_stream;
}

void MemoryStream::clear()
{
    if (m_stream)
    {
        delete[] m_stream;
        m_stream = nullptr;
    }

    MemoryStream::seekBeg(0);
    m_length = 0;
}

void MemoryStream::allocate(u32 size)
{
    if (m_stream)
    {
        delete[] m_stream;
        m_stream = nullptr;
    }

    m_allocated = size;
    m_stream = new u8[m_allocated];

    MemoryStream::seekBeg(0);
}

bool MemoryStream::checkSize(u32 size)
{
   /* ASSERT(m_stream);
    if (m_allocated == 0)
    {
        return false;
    }*/

    if (m_pos + size > m_allocated)
    {
        u8* oldStream = m_stream;

        s32 newAllocated = 2 * (m_pos + size);
        m_stream = new u8[newAllocated];
        memcpy(m_stream, oldStream, m_allocated);

        m_allocated = newAllocated;

        delete[] oldStream;
        oldStream = nullptr;
    }

    return true;
}
