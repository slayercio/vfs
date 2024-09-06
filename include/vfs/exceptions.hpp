#pragma once
#include <exception>

namespace vfs
{
    class OperationNotPermitted final : public std::exception
    {
    public:
        OperationNotPermitted()
            : std::exception("operation not permitted") {};
    };

    class IndexOutOfBounds final : public std::exception
    {
    public:
        IndexOutOfBounds()
            : std::exception("index out of bounds") {};
    };

    class InvalidPath final : public std::exception
    {
    public:
        InvalidPath()
            : std::exception("invalid path") {}
    };

    class InvalidUUID final : public std::exception
    {
    public:
        InvalidUUID()
            : std::exception("invalid uuid") {}
    };
}
