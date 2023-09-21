#ifndef ROCKEN_INCLUDE_ROCKEN__H_
#define ROCKEN_INCLUDE_ROCKEN__H_

// Define EXPORT for any platform
#if defined _WIN32 || defined __CYGWIN__
#ifdef WIN_EXPORT
#ifdef __GNUC__
#define EXPORT __attribute__((dllexport))
#else
#define EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define EXPORT __attribute__((dllimport))
#else
#define EXPORT __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define NOT_EXPORT
#else
#if __GNUC__ >= 4
#define EXPORT __attribute__((visibility("default")))
#define NOT_EXPORT __attribute__((visibility("hidden")))
#else
#define EXPORT
#define NOT_EXPORT
#endif
#endif

EXPORT int add(int a, int b);

#endif // ROCKEN_INCLUDE_ROCKEN__H_
