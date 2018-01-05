#ifndef Const_h
#define Const_h

#include <cmath>
#include <cstdlib>

typedef unsigned long long uint64;
typedef unsigned int uint32;

namespace Const {
    const double eps = 1e-6;
    const double pi = M_PI;
    const uint64 hash_p = 9999991;
    
    inline uint32 Rand_uint() {
#ifdef _Win32
        return (rand() << 15) | rand();
#else
        return rand();
#endif
    }
    
    inline uint64 Rand_uint64() {
#ifdef _WIN32
        return (((((1ll * rand() << 15) | rand()) << 15) | rand()) << 15) | rand();
#else
        return (1ll * rand() << 31) | rand();
#endif
    }
    
    // \in [0, 1]
    inline double Rand_double() {
#ifdef _WIN32
        return 1.0 * Rand_uint() / ((1ll << 31) - 1);
#else
        return 1.0 * rand() / RAND_MAX;
#endif
    }
}


#endif // Const_h
