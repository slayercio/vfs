#include <vfs/mutexed.hpp>

namespace vfs
{
    void Mutexed::lock() const
    {
        m_Mutex.lock();
    }

    void Mutexed::unlock() const
    {
        m_Mutex.unlock();
    }

    bool Mutexed::try_lock() const
    {
        return m_Mutex.try_lock();
    }
}
