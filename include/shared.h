/**
 * @file shared.h
 * @brief Internal Header: DO NOT USE
 * @warning None of this is part of the API and is not intended for public usage
 * @note This module may not be documented fully because it is not meant for
 * public usage
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
#endif // __GNUC__
#endif // _WIN32

// CLIB2_SHARED_DEPREC(MSG): Deprecated with message MSG
#ifndef _WIN32
#define CLIB2_SHARED_DEPREC(MSG) __declspec(deprecated(MSG))
#else
#if defined(__GNUC__) || defined(__clang__)
#define CLIB2_SHARED_DEPREC(MSG) __attribute__((deprecated(MSG)))
#else
#define CLIB2_SHARED_DEPREC(MSG) /* No special treatment */
#endif                           // __GNUC__ || __clang__
#endif                           // _WIN32

#endif // CLIB2_SHARED_H
