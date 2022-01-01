#pragma once
#define STRING_BUFFER_SIZE 1024

#ifndef __MINGW32__
    #if __has_include(<filesystem>)
        #include <filesystem>
        #define std_filesystem std::filesystem
    #else
        #include <experimental/filesystem>
        #define std_filesystem std::experimental::filesystem
    #endif
#else
    #error "Compiling against MinGW is not supported!"
#endif