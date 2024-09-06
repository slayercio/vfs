#pragma once

#include <functional>
#include <vfs/vfs_lib.hpp>
#include <random>
#include <string>
namespace vfs
{
    static std::string GenerateUuid()
    {
        std::random_device dev;
        std::mt19937       rng(dev());

        std::uniform_int_distribution<int> dist(0, 15);

        constexpr const char v[] = "0123456789abcdef";
        const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

        std::string uuid;
        for(int i = 0; i < 16; i++)
        {
            if(dash[i]) uuid += '-';
            uuid += v[dist(rng)];
            uuid += v[dist(rng)];
        }

        return uuid;
    }

    class UUID
    {
    public:
        static UUID from_string(std::string str)
        {
            return UUID(str);
        }
    public:
        UUID()
            : m_Value(GenerateUuid()) {}

        UUID(const UUID& other)
            : m_Value(other.m_Value) {}

        UUID(std::string value)
            : m_Value(value) {}

        UUID operator=(const UUID& other)
        {
            if(this != &other)
            {
                m_Value = other.m_Value;
            }

            return *this;
        }

        bool operator==(const std::string value) const
        {
            return m_Value.compare(value) == 0;
        }

        bool operator==(const UUID& other) const
        {
            if(m_Value.size() != other.m_Value.size())
            {
                return false;
            }

            for(size_t i = 0; i < m_Value.size(); i++)
            {
                if(m_Value[i] != other.m_Value[i])
                {
                    return false;
                }
            }

            return true;
        }

        bool operator!=(const UUID& other) const
        {
            return !(operator==(other));
        }

        const std::string value() const
        {
            return m_Value;
        }



    private:
        std::string m_Value;
    };

    struct uuid_equals
    {
        bool operator()(const vfs::UUID& lhs, const vfs::UUID& rhs) const
        {
            return lhs.value().compare(rhs.value()) == 0;
        }
    };
}

namespace std
{
    template<>
    struct std::hash<vfs::UUID>
    {
        std::size_t operator()(const vfs::UUID& uuid) const
        {
            return std::hash<std::string>()(uuid.value());
        }
    };
}
