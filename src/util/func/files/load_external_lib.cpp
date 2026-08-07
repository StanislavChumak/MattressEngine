#include "util/func/files/load_external_lib.hpp"

#include <cstdio>

namespace mtrs::util
{

LibHandle load_library(const std::string& path)
{
#if defined(_WIN32)
    return LoadLibraryA(path.c_str());
#else
    return dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
}

void free_library(LibHandle handle)
{
    if (!handle) return;
#if defined(_WIN32)
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
}

void* get_symbol(LibHandle handle, const std::string& name)
{
#if defined(_WIN32)
    return reinterpret_cast<void*>(GetProcAddress(handle, name.c_str()));
#else
    return dlsym(handle, name.c_str());
#endif
}

const char* get_last_error()
{
#if defined(_WIN32)
    static char buf[256];
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(),
                   0, buf, sizeof(buf), nullptr);
    return buf;
#else
    return dlerror();
#endif
}

}