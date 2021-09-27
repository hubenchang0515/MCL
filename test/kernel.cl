__kernel void vecAdd(__global float* X, __global float* Y)
{
	/* 获取当前线程的全局id */
	const int idx = get_global_id(0);

	X[idx] = X[idx] + Y[idx];
}