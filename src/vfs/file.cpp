#include <vfs/exceptions.hpp>
#include <vfs/file.hpp>
#include <fstream>
#include <iostream>

namespace vfs
{
    const bool VfsFile::IsReadable() const
    {
        return m_Flags & static_cast<uint8_t>(FileMode::READ);
    }

    const bool VfsFile::IsWritable() const
    {
        return m_Flags & static_cast<uint8_t>(FileMode::WRITE);
    }

    const bool VfsFile::IsAppendable() const
    {
        return m_Flags & static_cast<uint8_t>(FileMode::APPEND);
    }

    const size_t VfsFile::Size() const
    {
        return m_Size;
    }

    const size_t VfsFile::Position() const
    {
        return m_BufferPos;
    }

    const vfs::UUID VfsFile::UniqueId() const
    {
        return m_uniqueId;
    }

    const std::string VfsFile::Path() const
    {
        return m_Path;
    }

    void VfsFile::Write(const std::vector<uint8_t>& data)
    {
        if(!IsWritable())
        {
            throw vfs::OperationNotPermitted();
        }

        if(m_Size - m_BufferPos < data.size())
        {
            std::size_t bytesNeeded = data.size() - (m_Size - m_BufferPos);

            m_Data.resize(bytesNeeded);
        }

        for(const auto& b: data)
        {
            m_Data.insert(m_Data.begin() + m_BufferPos++, b);
        }
    }

    void VfsFile::Write(const std::uint8_t* data, const std::size_t size)
    {
        Write(std::vector<uint8_t>{data, data + size});
    }

    void VfsFile::Read(std::vector<uint8_t>& buffer)
    {
        if(!IsReadable())
        {
            throw vfs::OperationNotPermitted();
        }

        if(m_Size - m_BufferPos < buffer.capacity())
        {
            throw vfs::IndexOutOfBounds();
        }

        for(size_t i = 0ull; i < buffer.capacity(); i++)
        {
            buffer.push_back(m_Data[m_BufferPos++]);
        }
    }

    void VfsFile::Read(std::uint8_t* buffer, const std::size_t size)
    {
        if(!IsReadable())
        {
            throw vfs::OperationNotPermitted();
        }

        if(m_Size - m_BufferPos < size)
        {
            throw vfs::IndexOutOfBounds();
        }

        for(size_t i = 0ull; i < size; i++)
        {
            buffer[i] = m_Data[m_BufferPos++];
        }
    }

    void VfsFile::Save() const
    {
        Save(m_ReferencePath);
    }

    void VfsFile::Save(const std::filesystem::path path) const
    {
        if(path.empty())
        {
            throw vfs::InvalidPath();
        }

        std::ofstream stream;
        stream.open(path, std::ios::out | std::ios::binary);
        stream.write(reinterpret_cast<const char*>(m_Data.data()), m_Data.size());
        stream.flush();
    }

    VfsFile::VfsFile(const std::string path, std::filesystem::path file, FileMode mode)
        : m_ReferencePath(file), m_Path(path), m_BufferPos(0), m_Flags(static_cast<uint8_t>(mode))
    {
        std::cout << std::filesystem::absolute(file).string() << std::endl;
        std::ifstream stream(file, std::ios::binary | std::ios::ate);

        m_Size = std::filesystem::file_size(file);

        stream.seekg(std::ios::beg);
        m_Data.resize(m_Size);

        stream.read(reinterpret_cast<char*>(m_Data.data()), m_Size);
    }

    VfsFile::VfsFile(const std::string path, const std::vector<std::uint8_t>& data, FileMode mode)
        : m_Path(path), m_BufferPos(0), m_Flags(static_cast<uint8_t>(mode)), m_Data(data)
    {
        m_Size = data.size();
    }
}
