//OpenCL test



/***
 *     ██████╗ ██████╗ ███████╗███╗   ██╗     ██████╗██╗
 *    ██╔═══██╗██╔══██╗██╔════╝████╗  ██║    ██╔════╝██║
 *    ██║   ██║██████╔╝█████╗  ██╔██╗ ██║    ██║     ██║
 *    ██║   ██║██╔═══╝ ██╔══╝  ██║╚██╗██║    ██║     ██║
 *    ╚██████╔╝██║     ███████╗██║ ╚████║    ╚██████╗███████╗
 *     ╚═════╝ ╚═╝     ╚══════╝╚═╝  ╚═══╝     ╚═════╝╚══════╝
 *
 */


#include "CL/cl.hpp"
#pragma comment( lib, "opencl.lib" )
#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
//#include <stdafx.h>
#include "helper.h"

#define MEM_SIZE (128)
#define LOC_SIZE (16)
#define MEM_SIZE_SORT (1000000)
#define MAX_SOURCE_SIZE (0x100000)

typedef cl_uint data_t;
inline void setKey(data_t & a,cl_uint key) { a = key; }
inline void setValue(data_t & a,cl_uint value) { }
inline cl_uint getKey(const data_t & a) { return a; }
inline cl_uint getValue(const data_t & a) { return 0; }

inline data_t makeData(cl_uint key,cl_uint value) { data_t a; setKey(a,key); setValue(a,value); return a; }

/*
* size_t iSize;
* char *info;
* uint numb;
* cl_platform_id boom;
*/



//using namespace std;

int main(int argc, char* argv[])
{
/***
*	vector<cl::Platform> platforms;
*	cl::Platform::get(&platforms);
*	for (unsigned int iPlatform = 0; iPlatform < platforms.size(); iPlatform++)
*	{
*		clGetPlatformIDs(NULL, &boom, &numb);
*
*		std::cout << platforms.size() << "\n" << numb << boom;
*		std::vector<cl::Device> devices;
*		platforms[iPlatform].getDevices(CL_DEVICE_TYPE_ALL, &devices);
*
*
*	}
*
*/

size_t global_work_size[1] = {MEM_SIZE};
size_t local_work_size[1] = {LOC_SIZE};
cl_device_id device_id = NULL;
cl_context context = NULL;
cl_command_queue command_queue = NULL;
cl_mem memobj = NULL;
cl_mem str = NULL;
cl_mem memobj2 = NULL;
cl_mem cmDevSrcA = NULL;
cl_mem cmDevSrcB = NULL;
cl_mem cmDevDst = NULL;
cl_mem inBuffer = NULL;
cl_mem outBuffer = NULL;
cl_mem ABuffer[2];
cl_mem inc_mem = NULL;
cl_mem length_mem = NULL;
cl_program program = NULL;
cl_kernel kernel = NULL;
cl_kernel kernel2 = NULL;
cl_kernel kernel3 = NULL;
cl_kernel kernel4 = NULL;
cl_kernel kernel5 = NULL;
cl_kernel kernel6 = NULL;
cl_kernel kernel7 = NULL;
cl_kernel kernel8 = NULL;
cl_platform_id platform_id = NULL;
cl_uint ret_num_devices;
cl_uint ret_num_platforms;
cl_int ret;
cl_int* mem = (cl_int *)malloc (sizeof(cl_int) * MEM_SIZE);
cl_float* srcA = (cl_float *)malloc (sizeof(cl_float) * MEM_SIZE);
cl_float* srcB = (cl_float *)malloc (sizeof(cl_float) * MEM_SIZE);
cl_float* dst = (cl_float *)malloc (sizeof(cl_float) * MEM_SIZE);
cl_uint* in = (cl_uint *)malloc (sizeof(cl_uint) * MEM_SIZE);
cl_uint* out = (cl_uint *)malloc (sizeof(cl_uint) * MEM_SIZE);
cl_uint* outSort = (cl_uint *)malloc (sizeof(cl_uint) * MEM_SIZE);
cl_uint* outLocalSort = (cl_uint *)malloc (sizeof(cl_uint) * MEM_SIZE);
cl_uint* outBitLocalSort = (cl_uint *)malloc (sizeof(cl_uint) * MEM_SIZE);
cl_uint* ASort = (cl_uint *)malloc (sizeof(cl_uint) * MEM_SIZE);
cl_uint* ASort2 = (cl_uint *)malloc (sizeof(cl_uint) * MEM_SIZE);
cl_uint* inc_cl = (cl_uint *)malloc (1);
cl_uint* len_cl = (cl_uint *)malloc (1);


bool* direction = (bool *) malloc (sizeof(bool) * 32);
double* s_time = (double *)malloc (sizeof(double) * 16);
char string[MEM_SIZE];


FILE *fp;
char fileName[] = "./src/hello.cl";
char *source_str;
size_t source_size;

fp = fopen(fileName, "r");
if (!fp) {
	fprintf(stderr, "Failed to load kernel\n");
	return 0;
}

source_str = (char*)malloc(MAX_SOURCE_SIZE);
source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
fclose(fp);
s_time[1] = getRealTime();
std::cout << " time is = " << s_time[1];

for (int i=0; i<MEM_SIZE; i++)
{
		srcA[i] = i*i/(i+i+1.0);
		std::cout << "*" << srcA[i];
		srcB[i] = i;
		std::cout << "  |  " << srcB[i];
		in[i] = ASort[i] = (1+rand()%100);
		std::cout << "  |  " << in[i] << "* \n";
}
disp_inf(srcB,srcA,MEM_SIZE);


/***
 *    ██████╗ ██╗            ██╗       ██████╗ ███████╗██╗   ██╗
 *    ██╔══██╗██║            ██║       ██╔══██╗██╔════╝██║   ██║
 *    ██████╔╝██║         ████████╗    ██║  ██║█████╗  ██║   ██║
 *    ██╔═══╝ ██║         ██╔═██╔═╝    ██║  ██║██╔══╝  ╚██╗ ██╔╝
 *    ██║     ███████╗    ██████║      ██████╔╝███████╗ ╚████╔╝
 *    ╚═╝     ╚══════╝    ╚═════╝      ╚═════╝ ╚══════╝  ╚═══╝
 *
 */

ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
std::cout << "---PLATFORM ID = " << ret << '\n';
ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
std::cout << "---DEVICE ID = " << ret << '\n';

/***
 *     ██████╗ ██████╗ ███╗   ██╗████████╗███████╗██╗  ██╗████████╗
 *    ██╔════╝██╔═══██╗████╗  ██║╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝
 *    ██║     ██║   ██║██╔██╗ ██║   ██║   █████╗   ╚███╔╝    ██║
 *    ██║     ██║   ██║██║╚██╗██║   ██║   ██╔══╝   ██╔██╗    ██║
 *    ╚██████╗╚██████╔╝██║ ╚████║   ██║   ███████╗██╔╝ ██╗   ██║
 *     ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝
 *
 */


	context = clCreateContext(NULL, 1, &device_id , NULL, NULL, &ret);
  std::cout << "---CONTEXT = " << ret << '\n';

/***
*     ██████╗ ██╗   ██╗███████╗██╗   ██╗███████╗
*    ██╔═══██╗██║   ██║██╔════╝██║   ██║██╔════╝
*    ██║   ██║██║   ██║█████╗  ██║   ██║█████╗
*    ██║▄▄ ██║██║   ██║██╔══╝  ██║   ██║██╔══╝
*    ╚██████╔╝╚██████╔╝███████╗╚██████╔╝███████╗
*     ╚══▀▀═╝  ╚═════╝ ╚══════╝ ╚═════╝ ╚══════╝
*
*/

	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
  std::cout << "---QUEUE = " << ret << '\n';

/***
 *    ██████╗ ██╗   ██╗███████╗███████╗███████╗██████╗
 *    ██╔══██╗██║   ██║██╔════╝██╔════╝██╔════╝██╔══██╗
 *    ██████╔╝██║   ██║█████╗  █████╗  █████╗  ██████╔╝
 *    ██╔══██╗██║   ██║██╔══╝  ██╔══╝  ██╔══╝  ██╔══██╗
 *    ██████╔╝╚██████╔╝██║     ██║     ███████╗██║  ██║
 *    ╚═════╝  ╚═════╝ ╚═╝     ╚═╝     ╚══════╝╚═╝  ╚═╝
 *
 */

/*
	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(char) * MEM_SIZE, NULL, &ret);
	std::cout << "---BUFFER 1 = " << ret << '\n';

	str = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(char), NULL, &ret);
	std::cout << "---BUFFER S = " << ret << '\n';

	memobj2 = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int) * MEM_SIZE, NULL, &ret);
	std::cout << "---BUFFER 2 = " << ret << '\n';

	cmDevSrcA = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float) * MEM_SIZE, NULL, &ret);
	std::cout << "---BUFFER A = " << ret << '\n';

	cmDevSrcB = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float) * MEM_SIZE, NULL, &ret);
	std::cout << "---BUFFER B = " << ret << '\n';

	cmDevDst = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float) * MEM_SIZE, NULL, &ret);
	std::cout << "---BUFFER R = " << ret << '\n';

	inBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(data_t) * MEM_SIZE, NULL, &ret);
	std::cout << "---BUFFER IN = " << ret << '\n';

	outBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(data_t) * MEM_SIZE, NULL, &ret);
	std::cout << "---BUFFER OUT = " << ret << '\n';*/

	ABuffer[0] = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(data_t) * MEM_SIZE, NULL, &ret);
	std::cout << "---BUFFER A0 = " << ret << '\n';

	ABuffer[1] = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(data_t) * MEM_SIZE, NULL, &ret);
	std::cout << "---BUFFER A1 = " << ret << '\n';

	inc_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint), NULL, &ret);
	std::cout << "---BUFFER A1 = " << ret << '\n';

	length_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint), NULL, &ret);
	std::cout << "---BUFFER A1 = " << ret << '\n';






/***
 *    ████████╗ ██████╗     ██████╗ ██╗   ██╗███████╗███████╗███████╗██████╗
 *    ╚══██╔══╝██╔═══██╗    ██╔══██╗██║   ██║██╔════╝██╔════╝██╔════╝██╔══██╗
 *       ██║   ██║   ██║    ██████╔╝██║   ██║█████╗  █████╗  █████╗  ██████╔╝
 *       ██║   ██║   ██║    ██╔══██╗██║   ██║██╔══╝  ██╔══╝  ██╔══╝  ██╔══██╗
 *       ██║   ╚██████╔╝    ██████╔╝╚██████╔╝██║     ██║     ███████╗██║  ██║
 *       ╚═╝    ╚═════╝     ╚═════╝  ╚═════╝ ╚═╝     ╚═╝     ╚══════╝╚═╝  ╚═╝
 *
 */
/*
	ret = clEnqueueWriteBuffer(command_queue, memobj2, CL_TRUE, 0, sizeof(cl_int) * MEM_SIZE, mem, 0, NULL, NULL);
	std::cout << "---COPY BUFFER 2 = " << ret << '\n';

	ret = clEnqueueWriteBuffer(command_queue, cmDevSrcA, CL_TRUE, 0, sizeof(cl_float) * MEM_SIZE, srcA, 0, NULL, NULL);
	std::cout << "---COPY BUFFER A = " << ret << '\n';

	ret = clEnqueueWriteBuffer(command_queue, cmDevSrcB, CL_TRUE, 0, sizeof(cl_float) * MEM_SIZE, srcB, 0, NULL, NULL);
	std::cout << "---COPY BUFFER B = " << ret << '\n';

	ret = clEnqueueWriteBuffer(command_queue, inBuffer, CL_TRUE, 0, sizeof(cl_uint) * MEM_SIZE, in, 0, NULL, NULL);
	std::cout << "---COPY BUFFER IN = " << ret << '\n';*/

	ret = clEnqueueWriteBuffer(command_queue, ABuffer[0], CL_TRUE, 0, sizeof(cl_uint) * MEM_SIZE, in, 0, NULL, NULL);
	std::cout << "---COPY BUFFER A/BIT = " << ret << '\n';

	ret = clEnqueueReadBuffer (command_queue, ABuffer[0], CL_TRUE, 0, sizeof(cl_uint) * MEM_SIZE, ASort, 0, NULL, NULL);
	std::cout << "---READ = " << ret << '\n';

	ret = disp_inf(ASort, MEM_SIZE);
	std::cout << "---READ 2 = " << ret << '\n';




/***
 *    ██████╗ ██████╗  ██████╗  ██████╗ ██████╗  █████╗ ███╗   ███╗
 *    ██╔══██╗██╔══██╗██╔═══██╗██╔════╝ ██╔══██╗██╔══██╗████╗ ████║
 *    ██████╔╝██████╔╝██║   ██║██║  ███╗██████╔╝███████║██╔████╔██║
 *    ██╔═══╝ ██╔══██╗██║   ██║██║   ██║██╔══██╗██╔══██║██║╚██╔╝██║
 *    ██║     ██║  ██║╚██████╔╝╚██████╔╝██║  ██║██║  ██║██║ ╚═╝ ██║
 *    ╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝
 *
 */

	program = clCreateProgramWithSource (context, 1,(const char **)&source_str, (const size_t*)&source_size, &ret);
  std::cout << "---PROGRAMM<source = " << ret << '\n';

	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
  std::cout << "---PROGRAMM = " << ret << '\n';

/***
 *    ██╗  ██╗███████╗██████╗ ███╗   ██╗███████╗██╗
 *    ██║ ██╔╝██╔════╝██╔══██╗████╗  ██║██╔════╝██║
 *    █████╔╝ █████╗  ██████╔╝██╔██╗ ██║█████╗  ██║
 *    ██╔═██╗ ██╔══╝  ██╔══██╗██║╚██╗██║██╔══╝  ██║
 *    ██║  ██╗███████╗██║  ██║██║ ╚████║███████╗███████╗
 *    ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚══════╝╚══════╝
 *
 */


/*
	kernel = clCreateKernel (program, "hello", &ret);
	std::cout << "---KERNEL 1 = " << ret << '\n';

	kernel2 = clCreateKernel (program, "sum", &ret);
	std::cout << "---KERNEL 2 = " << ret << '\n';

	kernel3 = clCreateKernel (program, "VAdd", &ret);
	std::cout << "---KERNEL 3 = " << ret << '\n';

	kernel4 = clCreateKernel (program, "Copy", &ret);
  std::cout << "---KERNEL 4 = " << ret << '\n';

	kernel5 = clCreateKernel (program, "ParallelSelection", &ret);
	std::cout << "---KERNEL 5 = " << ret << '\n';

	kernel6 = clCreateKernel (program, "ParallelSelection_Local", &ret);
	std::cout << "---KERNEL 6 = " << ret << '\n';

	kernel7 = clCreateKernel (program, "ParallelBitonic_Local", &ret);
	std::cout << "---KERNEL 7 = " << ret << '\n';
	*/

	kernel8 = clCreateKernel (program, "ParallelBitonic_A", &ret);
	std::cout << "---KERNEL 8 = " << ret << '\n';

/***
 *     █████╗ ██████╗  ██████╗ ██╗   ██╗███╗   ███╗███████╗███╗   ██╗████████╗███████╗
 *    ██╔══██╗██╔══██╗██╔════╝ ██║   ██║████╗ ████║██╔════╝████╗  ██║╚══██╔══╝██╔════╝
 *    ███████║██████╔╝██║  ███╗██║   ██║██╔████╔██║█████╗  ██╔██╗ ██║   ██║   ███████╗
 *    ██╔══██║██╔══██╗██║   ██║██║   ██║██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   ╚════██║
 *    ██║  ██║██║  ██║╚██████╔╝╚██████╔╝██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   ███████║
 *    ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝  ╚═════╝ ╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚══════╝
 *
 */


/*
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&memobj);
	std::cout << "---SET ARGUMENTS 1 = " << ret << '\n';

	ret = clSetKernelArg(kernel3, 0, sizeof(cl_mem), (void*)&cmDevSrcA);
	std::cout << "---SET ARGUMENTS A = " << ret << '\n';
	ret = clSetKernelArg(kernel3, 1, sizeof(cl_mem), (void*)&cmDevSrcB);
	std::cout << "---SET ARGUMENTS B = " << ret << '\n';
	ret = clSetKernelArg(kernel3, 2, sizeof(cl_mem), (void*)&cmDevDst);
	std::cout << "---SET ARGUMENTS D = " << ret << '\n';
//	ret = clSetKernelArg(kernel3, 3, sizeof(cl_int), (void*)&iNum);
//	std::cout << "---SET ARGUMENTS I = " << ret << '\n';

	ret = clSetKernelArg(kernel2, 0, sizeof(cl_mem), (void*)&memobj2);
	std::cout << "---SET ARGUMENTS 2 = " << ret << '\n';

	ret = clSetKernelArg(kernel4, 0, sizeof(cl_mem), (void*)&inBuffer);
	std::cout << "---SET ARGUMENTS 4 = " << ret << '\n';
	ret = clSetKernelArg(kernel4, 1, sizeof(cl_mem), (void*)&outBuffer);
	std::cout << "---SET ARGUMENTS 4 = " << ret << '\n';

	ret = clSetKernelArg(kernel5, 0, sizeof(cl_mem), (void*)&inBuffer);
	std::cout << "---SET ARGUMENTS 5 = " << ret << '\n';
	ret = clSetKernelArg(kernel5, 1, sizeof(cl_mem), (void*)&outBuffer);
	std::cout << "---SET ARGUMENTS 5 = " << ret << '\n';

	ret = clSetKernelArg(kernel6, 0, sizeof(cl_mem), (void*)&inBuffer);
	std::cout << "---SET ARGUMENTS 6 = " << ret << '\n';
	ret = clSetKernelArg(kernel6, 1, sizeof(cl_mem), (void*)&outBuffer);
	std::cout << "---SET ARGUMENTS 6 = " << ret << '\n';
	ret = clSetKernelArg(kernel6, 2, sizeof(data_t)*LOC_SIZE, NULL);
	std::cout << "---SET ARGUMENTS 6 = " << ret << '\n';

	ret = clSetKernelArg(kernel7, 0, sizeof(cl_mem), (void*)&inBuffer);
	std::cout << "---SET ARGUMENTS 7 = " << ret << '\n';
	ret = clSetKernelArg(kernel7, 1, sizeof(cl_mem), (void*)&outBuffer);
	std::cout << "---SET ARGUMENTS 7 = " << ret << '\n';
	ret = clSetKernelArg(kernel7, 2, sizeof(data_t)*LOC_SIZE, NULL);
	std::cout << "---SET ARGUMENTS 7 = " << ret << '\n';
*/


/***
 *    ██████╗ ██╗████████╗ ██████╗ ███╗   ██╗██╗ ██████╗    ██╗ █████╗
 *    ██╔══██╗██║╚══██╔══╝██╔═══██╗████╗  ██║██║██╔════╝   ██╔╝██╔══██╗
 *    ██████╔╝██║   ██║   ██║   ██║██╔██╗ ██║██║██║       ██╔╝ ███████║
 *    ██╔══██╗██║   ██║   ██║   ██║██║╚██╗██║██║██║      ██╔╝  ██╔══██║
 *    ██████╔╝██║   ██║   ╚██████╔╝██║ ╚████║██║╚██████╗██╔╝   ██║  ██║
 *    ╚═════╝ ╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═══╝╚═╝ ╚═════╝╚═╝    ╚═╝  ╚═╝
 *
 */
int current = 0;
for (int length=1;length<MEM_SIZE;length<<=1) for (int inc=length;inc>0;inc>>=1)
{
	int len_cl = length << 1;//length<<1 ;
	//inc_cl = 1;//inc;


std::cout << "Set" << '\n';
	ret = clSetKernelArg(kernel8, 0, sizeof(cl_mem), (void*)&ABuffer[current]);
std::cout << ret << '\n';
	ret = clSetKernelArg(kernel8, 1, sizeof(cl_mem), (void*)&ABuffer[1-current]);
std::cout << ret << '\n';
	ret = clSetKernelArg(kernel8, 2, sizeof(cl_uint), (void*)&inc);
std::cout << ret << '\n';
	ret = clSetKernelArg(kernel8, 3, sizeof(cl_uint), (void*)&len_cl);
	std::cout << ret << '\n';


std::cout << "Run" << '\n';
	ret = clEnqueueNDRangeKernel(command_queue, kernel8, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
std::cout << ret << '\n';
std::cout << "---SOLVE = " << ret << '\n';
	ret = clEnqueueBarrier (command_queue);
std::cout << ret << '\n';

	ret = clEnqueueReadBuffer (command_queue, ABuffer[1-current], CL_TRUE, 0, sizeof(cl_uint) * MEM_SIZE, ASort2, 0, NULL, NULL);
	std::cout << "---READ = " << ret << '\n';

	ret = disp_inf(ASort,ASort2, MEM_SIZE);
	std::cout << "---READ 2 = " << ret << '\n';
//ret = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);
/*
ret = clEnqueueReadBuffer (command_queue, ABuffer[1], CL_FALSE, 0, sizeof(cl_uint), ASort2, 0, NULL, NULL);
std::cout << "---READ = " << ret << '\n';
ret = clEnqueueBarrier (command_queue);
std::cout << "---Barrier = " << ret << '\n';

for (int i = 0; i < MEM_SIZE; i++)
std::cout << in[i] << "  "<< ASort2[i] << '\n';
puts(string);*/
current = 1-current;
getchar();

}

/***
 *    ███████╗████████╗ █████╗ ██████╗ ████████╗
 *    ██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝
 *    ███████╗   ██║   ███████║██████╔╝   ██║
 *    ╚════██║   ██║   ██╔══██║██╔══██╗   ██║
 *    ███████║   ██║   ██║  ██║██║  ██║   ██║
 *    ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝
 *
 */
/*
	ret = clEnqueueTask(command_queue, kernel, 0, NULL, NULL);
	std::cout << "---SOLVE = " << ret << '\n';
	ret = clEnqueueNDRangeKernel(command_queue, kernel2, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
	std::cout << "---SOLVE 2 = " << ret << '\n';
	ret = clEnqueueNDRangeKernel(command_queue, kernel3, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
	std::cout << "---SOLVE 3 = " << ret << '\n';
	ret = clEnqueueNDRangeKernel(command_queue, kernel4, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
		std::cout << "---SOLVE 4 = " << ret << '\n';
	ret = clEnqueueReadBuffer (command_queue, outBuffer, CL_TRUE, 0, MEM_SIZE * sizeof(cl_uint), out, 0, NULL, NULL);
	std::cout << "---READ 4 = " << ret << '\n';
	s_time [2] = getRealTime();
	ret = clEnqueueNDRangeKernel(command_queue, kernel5, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
	std::cout << "---SOLVE 5 = " << ret << '\n';
	s_time [3] = getRealTime();
	ret = clEnqueueReadBuffer (command_queue, outBuffer, CL_TRUE, 0, MEM_SIZE * sizeof(cl_uint), outSort, 0, NULL, NULL);
	std::cout << "---READ 5 = " << ret << '\n';
	s_time [4] = getRealTime();
	ret = clEnqueueNDRangeKernel(command_queue, kernel6, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	std::cout << "---SOLVE 6 = " << ret << '\n';
	s_time [5] = getRealTime();
	ret = clEnqueueReadBuffer (command_queue, outBuffer, CL_TRUE, 0, MEM_SIZE * sizeof(cl_uint), outLocalSort, 0, NULL, NULL);
	std::cout << "---READ 6 = " << ret << '\n';

	clEnqueueWriteBuffer(cqCommandQue,cmDevSrcA, CL_FALSE, 0, sizeof(cl_float) * szGlobalWorkSize, srcA, 0, NULL, NULL);
	clEnqueueWriteBuffer(cqCommandQue,cmDevSrcB, CL_FALSE, 0, sizeof(cl_float) * szGlobalWorkSize, srcB, 0, NULL, NULL);
*/
	//clEnqueueNDRangeKernel(cqCommandQue,ckKernel,1,NULL,&szGlobalWorkSize, NULL, 0, NULL, NULL);


/***
 *    ██████╗ ███████╗ █████╗ ██████╗     ██████╗ ██╗   ██╗███████╗███████╗███████╗██████╗
 *    ██╔══██╗██╔════╝██╔══██╗██╔══██╗    ██╔══██╗██║   ██║██╔════╝██╔════╝██╔════╝██╔══██╗
 *    ██████╔╝█████╗  ███████║██║  ██║    ██████╔╝██║   ██║█████╗  █████╗  █████╗  ██████╔╝
 *    ██╔══██╗██╔══╝  ██╔══██║██║  ██║    ██╔══██╗██║   ██║██╔══╝  ██╔══╝  ██╔══╝  ██╔══██╗
 *    ██║  ██║███████╗██║  ██║██████╔╝    ██████╔╝╚██████╔╝██║     ██║     ███████╗██║  ██║
 *    ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝     ╚═════╝  ╚═════╝ ╚═╝     ╚═╝     ╚══════╝╚═╝  ╚═╝
 *
 */
/*
	ret = clEnqueueReadBuffer (command_queue, memobj, CL_TRUE, 0, MEM_SIZE * sizeof(char), string, 0, NULL, NULL);
	std::cout << "---READ = " << ret << '\n';
	ret = clEnqueueReadBuffer (command_queue, memobj2, CL_TRUE, 0, MEM_SIZE * sizeof(float), mem, 0, NULL, NULL);
	std::cout << "---READ 2 = " << ret << '\n';

	ret = clEnqueueReadBuffer (command_queue, cmDevDst, CL_TRUE, 0, MEM_SIZE * sizeof(float), dst, 0, NULL, NULL);
	std::cout << "---READ 3 = " << ret << '\n';
	ret = clEnqueueReadBuffer (command_queue, outBuffer, CL_TRUE, 0, MEM_SIZE * sizeof(cl_uint), outBitLocalSort, 0, NULL, NULL);
	std::cout << "---READ 7 = " << ret << '\n';
*/
/***
 *    ██████╗ ██╗███████╗██████╗ ██╗      █████╗ ██╗   ██╗
 *    ██╔══██╗██║██╔════╝██╔══██╗██║     ██╔══██╗╚██╗ ██╔╝
 *    ██║  ██║██║███████╗██████╔╝██║     ███████║ ╚████╔╝
 *    ██║  ██║██║╚════██║██╔═══╝ ██║     ██╔══██║  ╚██╔╝
 *    ██████╔╝██║███████║██║     ███████╗██║  ██║   ██║
 *    ╚═════╝ ╚═╝╚══════╝╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝
 *
 */

 for (int j=1; j< 32; j<<=1)
 {for (int i=0; i< 32; i++)
 {
	 if (i==0) std::cout << '\n' << "=========="<< j <<"========="<< '\n';
	 int d = i ^ j;
		direction[i] = ((i & (j<<1)) != 0);
	 std::cout << i << "=" << d << '-';
	 if (i==31) std::cout << '\n';
	 	/*for (int length=1;length<32;length<<=1)
	 	*{
		 *	direction= ((i & (length<<1)) != 0);
		 *	//std::cout << direction << "=";
	 	*}
		*/
 }
 for (int i=0; i< 32; i++) std::cout << direction[i] << '=';
}

 getchar();
	//puts(string);
	//for (int i=0; i<MEM_SIZE; i++)
	/*std::cout <<
	 //mem[i] << "  " << dst[i] << "  " <<
	 out[i] << "  " << outSort[i] << "  " << outLocalSort[i] << "  " << outBitLocalSort[i] << '\n';*/
	//puts("123456789");
	//std::cout << "123456" << string;
	std::cout << " time start = " << s_time[1] << "\n";
	std::cout << " time end = " << getRealTime() << "\n";
	std::cout << " time is = " << getRealTime() - s_time[1] << "\n";




/***
 *     ██████╗██╗     ███████╗ █████╗ ██████╗
 *    ██╔════╝██║     ██╔════╝██╔══██╗██╔══██╗
 *    ██║     ██║     █████╗  ███████║██████╔╝
 *    ██║     ██║     ██╔══╝  ██╔══██║██╔══██╗
 *    ╚██████╗███████╗███████╗██║  ██║██║  ██║
 *     ╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝
 *
 */
 std::cout << '\n' <<"CLEAR SYS" << '\n';
 ret = clFlush(command_queue);
 std::cout << "---1 = " << ret << '\n';
 ret = clFinish(command_queue);
 std::cout << "---2 = " << ret << '\n';
 ret = clReleaseKernel(kernel);
 std::cout << "---3 = " << ret << '\n';
 ret = clReleaseKernel(kernel2);
 std::cout << "---3 = " << ret << '\n';
 ret = clReleaseKernel(kernel3);
 std::cout << "---3 = " << ret << '\n';
 ret = clReleaseKernel(kernel4);
 std::cout << "---3 = " << ret << '\n';
 ret = clReleaseKernel(kernel5);
 std::cout << "---3 = " << ret << '\n';
 ret = clReleaseKernel(kernel6);
 std::cout << "---3 = " << ret << '\n';
 ret = clReleaseKernel(kernel7);
 std::cout << "---3 = " << ret << '\n';
 ret = clReleaseKernel(kernel8);
 std::cout << "---3 = " << ret << '\n';
 ret = clReleaseProgram(program);
 std::cout << "---4 = " << ret << '\n';
 ret = clReleaseMemObject(memobj);
 std::cout << "---5 = " << ret << '\n';
 ret = clReleaseMemObject(memobj2);
 std::cout << "---5 = " << ret << '\n';
 ret = clReleaseMemObject(cmDevSrcA);
 std::cout << "---5 = " << ret << '\n';
 ret = clReleaseMemObject(cmDevSrcB);
 std::cout << "---5 = " << ret << '\n';
 ret = clReleaseMemObject(cmDevDst);
 std::cout << "---5 = " << ret << '\n';
 ret = clReleaseMemObject(inBuffer);
 std::cout << "---5 = " << ret << '\n';
 ret = clReleaseMemObject(outBuffer);
 std::cout << "---5 = " << ret << '\n';
 ret = clReleaseMemObject(ABuffer[0]);
 std::cout << "---5 = " << ret << '\n';
 ret = clReleaseMemObject(ABuffer[1]);
 std::cout << "---5 = " << ret << '\n';
 ret = clReleaseMemObject(length_mem);
 std::cout << "---5 = " << ret << '\n';
 ret = clReleaseMemObject(inc_mem);
 std::cout << "---5 = " << ret << '\n';
 ret = clReleaseCommandQueue(command_queue);
 std::cout << "---6 = " << ret << '\n';
 ret = clReleaseContext(context);
 std::cout << "---7 = " << ret << '\n';

	free(source_str);
	return 0;
}
