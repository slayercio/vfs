#pragma once

#include <vfs/vfs_lib.hpp>
#include <mutex>
namespace vfs
{
    class Mutexed
    {
    public:
        // If lock is unavailable waits
        VFS_API void lock() const;

        VFS_API void unlock() const;

        // If lock is unavailable returns
        VFS_API bool try_lock() const;

    private:
        mutable std::mutex m_Mutex;
    };
}
