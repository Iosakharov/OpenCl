#ifndef helper_h
#define helper_h

#include <stdio.h>
#include <math.h>
#include <CL/cl.h>
#include <iostream>


double getRealTime();
int disp_inf(const cl_uint* text, int size);
int disp_inf (const cl_uint* text, const cl_uint* text2, int size);
int disp_inf (const cl_float* text, const cl_float* text2, int size);

#endif
