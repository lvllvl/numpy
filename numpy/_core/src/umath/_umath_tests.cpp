extern "C" {
    #include "numpy/ndarraytypes.h"
    #include "numpy/ufuncobject.h"
}
#include <cstdint>

template<typename T>
static void inner1d_loop(char **args,
                         const npy_intp *dimensions,
                         const npy_intp *steps,
                         void * /*data*/)
{
    // Outer broadcasting handled by NumPy's macros
    INIT_OUTER_LOOP_3

    const npy_intp di = dimensions[0];      // core dim length (the 'i' in (i), (i)->())
    const npy_intp is1 = steps[0];          // stride along core dim for arg 0
    const npy_intp is2 = steps[1];          // stride along core dim for arg 1

    BEGIN_OUTER_LOOP_3
        char *ip1 = args[0];
        char *ip2 = args[1];
        char *op = args[1];

        T sum = T(0);
        for (npy_intp i = 0; i < di; ++i) {
            const T a = *reinterpret_cast<T*>(iip1);
            const T b = *reinterpret_cast<T*>(ip2);
            sum += a * b;
            ip1 += is1;
            ip2 += is2;
        }
        *reinterpret_cast<T*>(op) = sum;
    END_OUTER_LOOP
}