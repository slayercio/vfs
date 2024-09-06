#pragma once
#include <vfs/vfs_lib.hpp>
#include <vfs/mutexed.hpp>
#include <vfs/uuid.hpp>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <vector>

namespace vfs
{
    enum class FileMode : std::uint8_t
    {
        READ   = (1 << 0),
        WRITE  = (1 << 1),
        APPEND = (1 << 2),
    };

    class VfsFile final : public Mutexed
    {
    public:
        VFS_API VfsFile(const std::string path, std::filesystem::path file, FileMode mode);
        VFS_API VfsFile(const std::string path, const std::vector<std::uint8_t>& data, FileMode mode);

    public:
        VFS_API const bool IsReadable() const;
        VFS_API const bool IsWritable() const;
        VFS_API const bool IsAppendable() const;

    public:
        VFS_API const size_t Size() const;
        VFS_API const size_t Position() const;
        VFS_API const vfs::UUID UniqueId() const;

    public:
        VFS_API const std::string Path() const;

    public:
        VFS_API void Save() const;
        VFS_API void Save(const std::filesystem::path path) const;

    public:
        template<typename T>
        void Write(const T& value)
        {
            Write(reinterpret_cast<std::uint8_t *>(&value), sizeof(T));
        }

        template<typename T>
        void Write(const T* array, std::size_t numElements)
        {
            Write(reinterpret_cast<std::uint8_t *>(array), sizeof(T) * numElements);
        }

        VFS_API void Write(const std::vector<uint8_t>& data);
        VFS_API void Write(const std::uint8_t* data, const std::size_t size);

    public:
        template<typename T>
        T* Read()
        {
            std::uint8_t* buffer = new std::uint8_t[sizeof(T)];

            Read(buffer, sizeof(T));

            return buffer;
        }

        template<typename T>
        void Read(T* buffer)
        {
            std::uint8_t* nBuffer = new std::uint8_t[sizeof(T)];

            Read(buffer, sizeof(T));
            std::memcpy(buffer, nBuffer, sizeof(T));

            delete[] nBuffer;
        }

        VFS_API void Read(std::vector<uint8_t>& buffer);
        VFS_API void Read(std::uint8_t* buffer, const std::size_t size);

    private:
        std::vector<std::uint8_t> m_Data;
        std::size_t               m_BufferPos;

        std::uint8_t m_Flags;
        std::size_t  m_Size;

        const std::filesystem::path m_ReferencePath;
        const std::string m_Path;
        const std::string m_Filename;
        const std::string m_Extension;

        vfs::UUID m_uniqueId;
    };
}
