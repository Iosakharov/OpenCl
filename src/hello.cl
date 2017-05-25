typedef uint data_t;
#define getKey(a) (a)
#define getValue(a) (0)
#define makeData(k,v) (k)

__kernel void hello(__global char* string)
{
	string[0] = 'H';
	string[1] = 'e';
	string[2] = 'l';
	string[3] = 'l';
	string[4] = 'o';
	string[5] = ',';
	string[6] = 'S';
	string[7] = 'i';
	string[8] = 'r';
	string[9] = '!';
	string[10] = '\0';
}


__kernel void sum(__global int* message)
{
	int gid = get_global_id(0);

	message[gid] += gid;
}

__kernel void VAdd(__global const float* a,__global const float* b,__global float* c)
	{
		int gid = get_global_id(0);
		c[gid] = a[gid] + b[gid];
	}

	// One thread per record
	__kernel void ParallelSelection(__global const data_t * in,__global data_t * out)
	{
	  int i = get_global_id(0); // current thread
	  int n = get_global_size(0); // input size
	  data_t iData = in[i];
	  uint iKey = getKey(iData);
	  // Compute position of in[i] in output
	  int pos = 0;
	  for (int j=0;j<n;j++)
	  {
	    uint jKey = getKey(in[j]); // broadcasted
	    bool smaller = (jKey < iKey) || (jKey == iKey && j < i); // in[j] < in[i] ?
	    pos += (smaller)?1:0;
	  }
	  out[pos] = iData;
	}

__kernel void ParallelSelection_Local(__global const data_t * in,__global data_t * out,__local data_t * aux)
	{
		int i = get_local_id(0);
		int wg = get_local_size(0);

		int offset = get_group_id(0) * wg;
		in += offset; out += offset;

		data_t iData = in[i];
		aux[i] = iData;
		barrier(CLK_LOCAL_MEM_FENCE);

		uint iKey = getKey(iData);
		int pos = 0;
		for (int j=0; j<wg; j++)
		{
			uint jKey = getKey(aux[j]);
			bool smaller = (jKey < iKey) || (jKey == iKey && j < i); // in[j] < in[i] ?
	    pos += (smaller)?1:0;
		}

	out[pos] = iData;
	}
/*
__kernel void ParallelMerge_local(__global const data_t in, __global data_t out,__local data_t * aux)
	{
		int i = get_local_id(0);
		int wg = get_local_size(0);

		int offset = get_group_id(0) * wg;
		in += offset; out += offset;

		aux[i] = in[i];
		barrier(CLK_LOCAL_MEM_FENCE);

		for (int legth=1; length<wg; length<<=1)
		{

			data_t iData = aux[i];
			uint iKey = getKey(iData);
			int ii = i & (length-1);

		}
	}
	*/

__kernel void ParallelBitonic_Local(__global const data_t * in,__global data_t * out, __local data_t * aux)
{
	int i = get_local_id(0);
	int wg = get_local_size(0);

	int offset = get_group_id(0) * wg;
	in += offset; out += offset;

	aux[i] = in [i];
	barrier(CLK_LOCAL_MEM_FENCE);

	for (int length=1; length<wg; length<<=1)
	{
		bool direction = ((i & (length<<1)) != 0);

		for (int inc=length; inc>0; inc>>=1)
		{
			int j = i ^ inc;
			data_t iData = aux[i];
			uint iKey = getKey(iData);
			data_t jData = aux[j];
			uint jKey = getKey(jData);
			bool smaller = (jKey < iKey) || (jKey == iKey && j < i);
			bool swap = smaller ^ (j < i) ^ direction;
			barrier(CLK_LOCAL_MEM_FENCE);
			aux[i] = (swap)?jData:iData;
			barrier(CLK_LOCAL_MEM_FENCE);
		}
	}

	out[i] = aux[i];
}

__kernel void ParallelBitonic_A(__global const data_t * in,__global data_t * out, int inc, int dir)
{
  int i = get_global_id(0); // thread index
  int j = i ^ inc; // sibling to compare

  // Load values at I and J
  data_t iData = in[i];
  uint iKey = getKey(iData);
  data_t jData = in[j];
  uint jKey = getKey(jData);

  // Compare
  bool smaller = (jKey < iKey) || ( jKey == iKey && j < i );
 	bool swap = smaller ^ (j < i) ^ ((dir & i) != 0) ;

  out[i] = (swap)?jData:iData;
}

__kernel void Copy(__global const data_t * in,__global data_t * out)
{
  int i = get_global_id(0); // current thread
  out[i] = in[i]; // copy
}
