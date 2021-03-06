#include "FileStream.h"
#include "utils/Logger.h"
#include <sstream>
#include <fstream>
#include <ostream>
#include <istream>
#include <sys/stat.h>

using namespace v3d;
using namespace v3d::stream;
using namespace v3d::core;

FileStream::FileStream()
    : m_fileHandler(nullptr)
    , m_fileSize(0)
    , m_isOpen(false)
    , m_mappedMemory(nullptr)
    , m_mapped(false)
{
}

FileStream::FileStream(const std::string& file, EOpenMode openMode)
    : m_fileHandler(nullptr)
    , m_fileSize(0)
    , m_isOpen(false)
    , m_mappedMemory(nullptr)
    , m_mapped(false)
{
    open(file, openMode);

    if (!m_isOpen)
    {
        LOG_ERROR("Can not read file: %s", file.c_str());
    }
}


FileStream::~FileStream()
{
    close();
}

bool FileStream::open(const std::string& file, EOpenMode openMode)
{
    if (m_isOpen)
    {
        return true;
    }

    const char* mode = 0;

    if ((openMode & e_in) == openMode)
    {
        mode = "rb";
    }
    else if ((openMode & e_out) == openMode)
    {
        mode = "wb";
    }
    else if ((openMode & e_app) == openMode)
    {
        mode = "ab";
    }
    else if ((openMode & (e_in | e_out)) == openMode)
    {
        mode = "r+b";
    }
    else if ((openMode & (e_out | e_app)) == openMode)
    {
        mode = "ab";
    }
    else if (openMode & e_trunc)
    {
        mode = "wb+";
    }
    else if (openMode & e_create)
    {
        mode = "wb+";
    }
    else
    {
        mode = "rb";
    }

    m_fileName = file;
    m_fileHandler = fopen(m_fileName.c_str(), mode);
 
    m_isOpen = m_fileHandler != nullptr;

    return m_isOpen;
}

void FileStream::close()
{
    if (m_fileHandler)
    {
        fclose(m_fileHandler);
        m_fileHandler = nullptr;
    }
    m_fileSize = 0;
    m_fileName = "";
    m_isOpen = false;
}


bool FileStream::isOpen() const
{
    return m_isOpen;
}

u32 FileStream::read(void* buffer, const u32 size, const u32 count)
{
    ASSERT(m_fileHandler, "File Handler nullptr");
    const u32 ret = (u32)fread(buffer, size, count, m_fileHandler);
    return ret;
}

u32 FileStream::read(u8& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(s8& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(u16& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(s16& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(u32& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(s32& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(u64& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(s64& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(f32& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(f64& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(f80& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(bool& value)
{
    const u32 ret = FileStream::read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(std::string& value)
{
    ASSERT(m_fileHandler, "File Handler nullptr");

    FileStream::seekEnd(0);
    m_fileSize = FileStream::tell();
    rewind(m_fileHandler);

    value.clear();
    value.resize(m_fileSize);
    const u32 ret = FileStream::read((void*)value.data(), sizeof(c8), m_fileSize);

    return ret;
}

u32 FileStream::write(const void* buffer, const u32 size, const u32 count)
{
    ASSERT(m_fileHandler, "File Handler nullptr");
    const u32 ret = (u32)fwrite(buffer, size, count, m_fileHandler);
    return ret;
}

u32 FileStream::write(const u8 value)
{
    const u32 ret = FileStream::write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const s8 value)
{
    const u32 ret = FileStream::write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const u16 value)
{
    const u32 ret = FileStream::write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const s16 value)
{
    const u32 ret = FileStream::write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const u32 value)
{
    const u32 ret = FileStream::write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const s32 value)
{
    const u32 ret = FileStream::write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const u64 value)
{
    const u32 ret = FileStream::write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const s64 value)
{
    const u32 ret = FileStream::write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const f32 value)
{
    const u32 ret = FileStream::write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const f64 value)
{
    const u32 ret = (u32)write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const f80 value)
{
    const u32 ret = FileStream::write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const bool value)
{
    const u32 ret = FileStream::write(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::write(const std::string value)
{
    const u32 strLen = (u32)value.length();
    u32 ret = FileStream::write(&strLen, sizeof(u32), 1);
    ret += FileStream::write(value.c_str(), sizeof(c8), strLen);
    return ret;
}

void FileStream::seekBeg(const u32 offset)
{
    ASSERT(m_fileHandler, "File Handler nullptr");
    fseek(m_fileHandler, offset, SEEK_SET);
}

void FileStream::seekEnd(const u32 offset)
{
    ASSERT(m_fileHandler, "File Handler nullptr");
    fseek(m_fileHandler, offset, SEEK_END);
}

void FileStream::seekCur(const u32 offset)
{
    ASSERT(m_fileHandler, "File Handler nullptr");
    fseek(m_fileHandler, offset, SEEK_CUR);
}

u32 FileStream::tell()
{
    ASSERT(m_fileHandler, "File Handler nullptr");
    u32 tell = ftell(m_fileHandler);
    return tell;
}

u32 FileStream::size()
{
    ASSERT(m_fileHandler, "File Handler nullptr");
    if (m_fileSize == 0)
    {
        u32 currentPos = tell();
        fseek(m_fileHandler, 0, SEEK_END);
        m_fileSize = tell();
        fseek(m_fileHandler, currentPos, SEEK_SET);
    }
    return m_fileSize;
}

u8* FileStream::map(const u32 size)
{
    ASSERT(size > 0 && tell() + size <= FileStream::size(), "Invalid file size");
    u8* address = 0;

     m_mappedMemory = new u8[size];
     address = m_mappedMemory;

    read(address, size);

    ASSERT(!m_mapped, "Memory not mapped");
    m_mapped = true;

    return address;
}

void FileStream::unmap()
{
    if (m_mappedMemory)
    {
        delete[] m_mappedMemory;
        m_mappedMemory = nullptr;
    }
    ASSERT(m_mapped, "Memory not mapped");
    m_mapped = false;
}

const std::string& FileStream::getName() const
{
    return m_fileName;
}

bool FileStream::isFileExist(const std::string& file)
{
    struct stat status;
    int ret = stat(file.c_str(), &status);
    return (ret == 0);
}

bool FileStream::isDirectory(const std::string& path)
{
    struct stat status;
    int ret = stat(path.c_str(), &status);
    if (ret != -1)
    {
        ret = ((status.st_mode & S_IFDIR) != 0) ? 1 : 0;
    }
    return (ret != 0);
}

bool FileStream::remove(const std::string& file)
{
    return std::remove(file.c_str()) == 0;
}
