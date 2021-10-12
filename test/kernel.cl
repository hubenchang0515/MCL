__kernel void add(__global float* X, __global float* Y, __global float* Z)
{
	/* 获取当前线程的全局id */
	const int idx = get_global_id(0);

	Z[idx] = X[idx] + Y[idx];
}

__kernel void sub(__global float* X, __global float* Y, __global float* Z)
{
	/* 获取当前线程的全局id */
	const int idx = get_global_id(0);

	Z[idx] = X[idx] - Y[idx];
}

__kernel void mul(__global float* X, __global float* Y, __global float* Z)
{
	/* 获取当前线程的全局id */
	const int idx = get_global_id(0);

	Z[idx] = X[idx] * Y[idx];
}

__kernel void div(__global float* X, __global float* Y, __global float* Z)
{
	/* 获取当前线程的全局id */
	const int idx = get_global_id(0);

	Z[idx] = X[idx] / Y[idx];
}