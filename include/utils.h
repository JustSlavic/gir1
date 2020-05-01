#ifndef GIR1_UTIL
#define GIR1_UTIL

#include <string>


std::string read_whole_file(const char *filename);

std::string get_current_path();

bool check_gl_errors();


#endif // GIR1_UTIL
