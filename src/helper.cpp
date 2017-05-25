#include <sys/time.h>
#include "helper.h"

double getRealTime()
{
  struct timeval tv;
  gettimeofday(&tv,0);
  return (double)tv.tv_sec + 1.0e-6*(double)tv.tv_usec;
}

int disp_inf (const cl_uint* text, const cl_uint* text2, int size)
  {
    for (int i = 0; i < size; i++)

    {std::cout << text[i] << "= ="<< text2[i] << "\n";
    if(i%2==1) std::cout << "\n";}
    return 0;
  }
  int disp_inf (const cl_float* text, const cl_float* text2, int size)
    {
      for (int i = 0; i < size; i++)
    {
      std::cout << text[i] << "= ="<< text2[i] << "\n";
      if(i%2==1) std::cout << "\n";
    }
      return 0;
    }
int disp_inf(const cl_uint* text, int size)
  {
    for (int i = 0; i < size; i++)
    std::cout << text[i] << "\n";

    return 0;
  }
