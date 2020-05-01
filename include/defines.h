#ifndef GIR1_DEFINES
#define GIR1_DEFINES

#include <stdio.h>
#include <stdlib.h>

#include <utils.h>


#define ASSERT(x) \
    if (!(x)) { \
        fprintf(stderr, "Failed at %s:%d\n", __FILE__, __LINE__); \
        std::exit(1); \
    } enum {}

#define GL_CALL(call) \
    call; \
    ASSERT(check_gl_errors());


#endif // GIR1_DEFINES
