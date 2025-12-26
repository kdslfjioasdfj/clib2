/**
 * @file shared.h
 * @brief Internal header for library visibility management
 * @date 2025-12-26
 */

#ifndef CLIB2_SHARED_H
#define CLIB2_SHARED_H

#ifdef _WIN32
// On Windows, if building the library, use __declspec(dllexport), else
// __declspec(dllimport)
#ifdef CLIB2_CONFIG_BUILD
#define CLIB2_SHARED_PUBLIC __declspec(dllexport)
#else
#define CLIB2_SHARED_PUBLIC __declspec(dllimport)
#endif
#else
// On Linux/macOS (Clang/GCC-based), use visibility="default" to export symbols
#ifdef __GNUC__
#define CLIB2_SHARED_PUBLIC __attribute__((visibility("default")))
#else
// For other compilers, no special visibility handling (static libraries, etc.)
#define CLIB2_SHARED_PUBLIC
#endif
#endif

#endif // CLIB2_SHARED_H
