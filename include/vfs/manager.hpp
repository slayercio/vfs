#pragma once

#include "uuid.hpp"
#include <filesystem>
#include <vfs/vfs_lib.hpp>
#include <vfs/file.hpp>
#include <vfs/uuid.hpp>
#include <memory>
#include <unordered_map>

namespace vfs
{
    class VfsManager
    {
    public:
        using file_ref = std::shared_ptr<vfs::VfsFile>;

    public:
        VFS_API VfsManager();
        VFS_API ~VfsManager();
        [[nodiscard]] VFS_API file_ref LoadFrom(std::filesystem::path file, FileMode mode, std::string_view prefix = "memory");

    public:
        [[nodiscard]] VFS_API file_ref GetByUUID(const vfs::UUID& uuid);
        [[nodiscard]] VFS_API file_ref GetByPath(const std::string path);

    public:
        VFS_API void Release(const vfs::UUID& uuid);
        VFS_API void Release(const std::string path);

    private:
        std::unordered_map<std::string, vfs::UUID>  m_PathToUUID;
        std::unordered_map<vfs::UUID, file_ref, std::hash<vfs::UUID>, vfs::uuid_equals>     m_UniqueIdToFileRef;
        std::vector<file_ref>                       m_Files;
    };
}
