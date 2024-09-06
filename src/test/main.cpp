#include "vfs/file.hpp"
#include <filesystem>
#include <iostream>
#include <vfs/manager.hpp>

void print_file(auto file)
{
    std::cout << "uuid: " << file->UniqueId().value() << std::endl;
    std::cout << "size: " << file->Size() << std::endl;
    std::cout << "path: " << file->Path() << std::endl;
}

int main()
{
    vfs::VfsManager manager;
    auto file = manager.LoadFrom(std::filesystem::relative("test.txt"), vfs::FileMode::READ);
    if(file)
    {
        print_file(file);
    }

    auto ref = manager.GetByPath("memory:test");
    if(ref)
    {
        print_file(ref);
    }

    auto uuidRef = manager.GetByUUID(ref->UniqueId());
    if(uuidRef)
    {
        print_file(uuidRef);
    }
}
