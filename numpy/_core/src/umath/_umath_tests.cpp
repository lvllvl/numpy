/*
*****************************************************************************
**                                  INCLUDES                               **
*****************************************************************************
*/
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define NPY_NO_DEPRECATED_API NPY_API_VERSION
#if defined(NPY_INTERNAL_BUILD)
#undef NPY_INTERNAL_BUILD
#endif
// for add_INT32_negative_indexed
#define NPY_TARGET_VERSION NPY_2_1_API_VERSION
#include "numpy/arrayobject.h"
#include "numpy/ufuncobject.h"
#include "numpy/ndarrayobject.h"
#include "numpy/npy_math.h"



#include "npy_config.h"
#include "npy_cpu_features.h"
#include "npy_cpu_dispatch.h"
#include "numpy/npy_cpu.h"
#include "npy_import.h"
#include "numpy/dtype_api.h"


/*
*****************************************************************************
**                                  BASICS                                 **
*****************************************************************************
*/

// TODO: how to test this and make sure: 
    // it works - in general
    // it works like the previous macros
struct OuterLoop1 {

    npy_intp dN;
    npy_intp N_;
    npy_intp s0;

    OuterLoop1(npy_intp*& dimensions, npy_intp*& steps) {
        dN = *dimensions;
        dimensions++;
        N_ = 0;
        s0 = *steps;
        steps++;
    }
};

struct Outerloop2 : OuterLoop1 {
    npy_intp s1;

    Outerloop2(npy_intp*& dimensions, npy_intp*& steps)
        : OuterLoop1(dimensions, steps) // Run INIT_OUTER_LOOP_1
    {
        s1 = *steps;
        steps++;     // advance steps
    }
};

struct Outerloop3 : Outerloop2 {
    npy_intp s2;

    Outerloop3(npy_intp*& dimensions, npy_intp*& steps)
        : Outerloop2(dimensions, steps)
    {
        s2 = *steps;
        steps++;
    }
};

struct Outerloop4 : Outerloop3 {
    npy_intp s3;

    Outerloop4(npy_intp*& dimensions, npy_intp*& steps)
        : Outerloop3(dimensions, steps)
    {
        s3 = *steps;
        steps++;
    }
};




#define BEGIN_OUTER_LOOP_2      \
    for (N_ = 0; N_ < dN; N_++, args[0] += s0, args[1] += s1) {

#define BEGIN_OUTER_LOOP_3      \
    for (N_ = 0; N_ < dN; N_++, args[0] += s0, args[1] += s1, args[2] += s2) {

#define BEGIN_OUTER_LOOP_4      \
    for (N_ = 0; N_ < dN; N_++, args[0] += s0, args[1] += s1, args[2] += s2, args[3] += s3) {

#define END_OUTER_LOOP  }



/*
*****************************************************************************
**                                  UFUNC LOOPS                            **
*****************************************************************************
*/

