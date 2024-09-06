#include "vfs/exceptions.hpp"
#include "vfs/file.hpp"
#include "vfs/uuid.hpp"
#include <algorithm>
#include <filesystem>
#include <vfs/manager.hpp>

namespace vfs
{
    using file_ref = VfsManager::file_ref;
    namespace fs = std::filesystem;

    VfsManager::VfsManager() = default;
    VfsManager::~VfsManager() = default;

    file_ref VfsManager::LoadFrom(std::filesystem::path file, FileMode mode, std::string_view prefix)
    {
        if(!fs::exists(file))
        {
            throw vfs::InvalidPath();
        }

        auto file_cp = file;

        auto path = std::format("{}:{}", prefix, file_cp.replace_extension("").filename().string());
        auto ref = std::make_shared<vfs::VfsFile>(path, file, mode);

        m_PathToUUID[path] = ref->UniqueId();
        m_UniqueIdToFileRef[ref->UniqueId()] = ref;
        m_Files.push_back(ref);

        return ref;
    }

    file_ref VfsManager::GetByPath(const std::string path)
    {
        if(m_PathToUUID.find(path) == m_PathToUUID.end())
        {
            return nullptr;
        }

        auto uuid = m_PathToUUID[path];

        if(m_UniqueIdToFileRef.find(uuid) == m_UniqueIdToFileRef.end())
        {
            return nullptr;
        }

        return m_UniqueIdToFileRef[uuid];
    }

    file_ref VfsManager::GetByUUID(const vfs::UUID& uuid)
    {
        if(m_UniqueIdToFileRef.find(uuid) == m_UniqueIdToFileRef.end())
        {
            return nullptr;
        }

        return m_UniqueIdToFileRef[uuid];
    }

    void VfsManager::Release(const vfs::UUID& uuid)
    {
        auto iter = std::find_if(m_Files.begin(), m_Files.end(), [&](file_ref file) {
            return file->UniqueId() == uuid;
        });

        if(iter == m_Files.end())
        {
            throw vfs::InvalidUUID();
        }

        file_ref file = *iter;

        if(file->UniqueId() == uuid)
        {
            m_PathToUUID.erase(file->Path());
            m_UniqueIdToFileRef.erase(file->UniqueId());
        }

        m_Files.erase(iter);
    }

    void VfsManager::Release(const std::string path)
    {
        auto iter = std::find_if(m_Files.begin(), m_Files.end(), [&](file_ref file) {
            return file->Path() == path;
        });

        if(iter == m_Files.end())
        {
            throw vfs::InvalidUUID();
        }

        file_ref file = *iter;

        if(file->Path() == path)
        {
            m_PathToUUID.erase(file->Path());
            m_UniqueIdToFileRef.erase(file->UniqueId());
        }

        m_Files.erase(iter);
    }
}
