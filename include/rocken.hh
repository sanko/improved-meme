#ifndef ROCKEN_INCLUDE_ROCKEN__H_
#define ROCKEN_INCLUDE_ROCKEN__H_

#if defined(_WIN32)
#define __export __declspec(dllexport)
#elif defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#define __export __attribute__((visibility("default")))
#else
#define __export
#endif

#ifndef _MSVC_VER
#include <string>
#endif
namespace Rocken
{
#ifdef _MSVC_VER
// I need to sort out Unicode on Windows...
#else
std::u32string to_utf32(std::string_view s);
std::string to_utf8(std::u32string_view s);
#endif
};     // namespace Rocken
#endif // ROCKEN_INCLUDE_ROCKEN__H_
