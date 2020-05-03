#ifndef GIR1_DEFINES_H
#define GIR1_DEFINES_H

#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <utils.h>


#define ASSERT(x) \
    if (!(x)) { \
        fprintf(stderr, "Failed at %s:%d\n", __FILE__, __LINE__); \
        std::exit(1); \
    } void SEMICOLON__()

#define GL_CALL(call) \
    call; \
    ASSERT(check_gl_errors())

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)


typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float    f32;
typedef double   f64;

#endif // GIR1_DEFINES_H
