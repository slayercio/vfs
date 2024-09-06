#pragma once

#if defined(_MSC_VER)
#   define VFS_EXPORT __declspec(dllexport)
#   define VFS_IMPORT __declspec(dllimport)
#elif defined(_GNUC_)
#   define VFS_EXPORT __attribute__((visibility("normal")))
#   define VFS_IMPORT
#else
#   error "Unknown import/export semantics"
#endif

#if defined(_BUILD_VFS)
#   define VFS_API VFS_EXPORT
#else
#   define VFS_API VFS_IMPORT
#endif
