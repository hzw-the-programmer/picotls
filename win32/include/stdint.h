#ifndef _STDINT
#define _STDINT

typedef int int32_t;
#if defined(_MSC_VER) && _MSC_VER <= 1200
typedef __int64 int64_t;
#else
typedef long long int64_t;
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1200
typedef unsigned __int64 uint64_t;
#else
typedef unsigned long long uint64_t;
#endif

typedef int intptr_t;

#endif // _STDINT
