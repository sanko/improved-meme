#ifndef ROCKEN_INCLUDE_ROCKEN__H_
#define ROCKEN_INCLUDE_ROCKEN__H_
#if defined(_WIN32)
#define __export __declspec(dllexport)
#elif defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#define __export __attribute__((visibility("default")))
#else
#define __export
#endif

__export int add(int a, int b);

#endif // ROCKEN_INCLUDE_ROCKEN__H_
