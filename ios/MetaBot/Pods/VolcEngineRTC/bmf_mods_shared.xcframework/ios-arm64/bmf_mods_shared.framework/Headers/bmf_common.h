#ifndef _BMF_MODS_COMMON_H_
#define _BMF_MODS_COMMON_H_
#ifdef _WIN32
    #define BMF_MODS_EXPORT __declspec(dllexport)
#elif __APPLE__
    #define BMF_MODS_EXPORT __attribute__ ((visibility("default")))
#elif __ANDROID__
    #define BMF_MODS_EXPORT __attribute__ ((visibility("default")))
#elif __linux__
    #define BMF_MODS_EXPORT __attribute__ ((visibility("default")))
#endif
#endif