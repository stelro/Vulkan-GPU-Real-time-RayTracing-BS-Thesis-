/* =======================================================================
   $File: core.hh
   $Date: 3/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#ifndef PROJECT_CORE_HH
#define PROJECT_CORE_HH

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <type_traits>
#include <string_view>

#include "vulkan/vulkan.h"
#if defined(_WIN32)
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#endif

// Custom define for better code readability
#define VK_FLAGS_NONE 0
// Default fence timeout in nanoseconds
#define DEFAULT_FENCE_TIMEOUT 100000000000
// fatal error used for ost_assertions


namespace ost {


    /*
     * Types aliases, for code portability
     */

    using u8                    = uint8_t;
    using u16                   = uint16_t;
    using u32                   = uint32_t;
    using u64                   = uint64_t;
    using i8                    = int8_t;
    using i16                   = int16_t;
    using i32                   = int32_t;
    using i64                   = int64_t;
    using uptr                  = uintptr_t;
    using usize                 = size_t;

    /*
     * I am avoiding exceptions, especially when low level code is involved,
     * so assertions and static_assertions is heavily used all over the project.
     */

    #define OST_ASSERT_LOG(condition, msg) do \
        { if (!(condition)) { printf("File: %s\nLine: %d\nMessage: %s\n--ERROR END--\n", __FILE__,__LINE__, msg); exit(1); } \
        } while(0);

    #define OST_ASSERT(condition) do \
        { if (!(condition)) { printf("\nAssertion Failed -- > \n File: %s\nLine: %d\n--END--\n", __FILE__,__LINE__); exit(1); } \
        } while(0);

    /*
     * Custom defines to save writing
     */

    #define OST_DISABLE_COPY(Class) \
            Class(const Class &) = delete; \
            Class &operator =(const Class &) = delete;

    #define OST_DISABLE_MOVE(Class) \
            Class(const Class &&) = delete; \
            Class &operator =(const Class &&) = delete;

    #ifdef _WIN32
        #define OST_FINAL final
            #define OST_LIBRARY_EXPORT __declspec(dllexport)
            #define OST_LIBRARY_IMPORT __declspec(dllimport)
            #define OST_FORCE_INLINE __forceinline
            #define OST_RESTRICT __restrict
            #define OST_ATTRIBUTE_USED
    #else
        #define OST_FINAL final
        #define OST_LIBRARY_EXPORT __attribute__((visibility("default")))
        #define OST_LIBRARY_IMPORT
        #define OST_FORCE_INLINE __attribute__((always_inline)) inline
        #define OST_RESTRICT __restrict__
        #define OST_ATTRIBUTE_USED __attribute__((used))
    #endif

    OST_FORCE_INLINE
    constexpr size_t mb_to_byte(size_t size) {
        return (size << 10 << 10);
    }

    OST_FORCE_INLINE
    constexpr size_t gb_to_byte(size_t size) {
        return (size << 10 << 10 << 10);
    }

    OST_FORCE_INLINE
    constexpr size_t byte_to_mb(size_t size) {
        return (size >> 10 >> 10);
    }

    OST_FORCE_INLINE
    constexpr size_t byte_to_gb(size_t size) {
        return (size >> 10 >> 10 >> 10);
    }

    /// Vulkan helper macros
    #define VK_CHECK_RESULT(f)																				\
    {																										\
        VkResult res = (f);																					\
        if (res != VK_SUCCESS)																				\
        {																									\
            printf("Fatal : VkResult failed! in %s, at line: %d",  __FILE__, __LINE__); \
            exit(1);																		\
        }																									\
    }

    // Exit fatal with message
    OST_FORCE_INLINE
    void exitFatal(std::string_view message, u32 exitCode) {
    #if defined(_WIN32)
            MessageBox(nullptr, message, nullptr, MB_OK | MB_ICONERROR);
    #endif
        printf("%s", message.data());
        exit(exitCode);
    }

    OST_FORCE_INLINE
    void exitFatal(std::string_view message, VkResult resultCode) {
        exitFatal(message, static_cast<u32>(resultCode));
    }



}

#endif //PROJECT_CORE_HH
