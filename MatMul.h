#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#define BLOCK_SIZE 16


__global__ void gpu_matrix_mult(int* a, int* b, int* c, int m, int n, int k)
{
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	int sum = 0;
	if (col < k && row < m)
	{
		for (int i = 0; i < n; i++)
		{
			sum += a[row * n + i] * b[i * k + col];
		}
		c[row * k + col] = sum;
	}
}


__global__ void gpu_square_matrix_mult(int* d_a, int* d_b, int* d_result, int n)
{
	__shared__ int tile_a[BLOCK_SIZE][BLOCK_SIZE];
	__shared__ int tile_b[BLOCK_SIZE][BLOCK_SIZE];

	int row = blockIdx.y * BLOCK_SIZE + threadIdx.y;
	int col = blockIdx.x * BLOCK_SIZE + threadIdx.x;
	int tmp = 0;
	int idx;

	for (int sub = 0; sub < gridDim.x; ++sub)
	{
		idx = row * n + sub * BLOCK_SIZE + threadIdx.x;
		if (idx >= n * n)
		{
			// n may not divisible by BLOCK_SIZE
			tile_a[threadIdx.y][threadIdx.x] = 0;
		}
		else
		{
			tile_a[threadIdx.y][threadIdx.x] = d_a[idx];
		}

		idx = (sub * BLOCK_SIZE + threadIdx.y) * n + col;
		if (idx >= n * n)
		{
			tile_b[threadIdx.y][threadIdx.x] = 0;
		}
		else
		{
			tile_b[threadIdx.y][threadIdx.x] = d_b[idx];
		}
		__syncthreads();

		for (int k = 0; k < BLOCK_SIZE; ++k)
		{
			tmp += tile_a[threadIdx.y][k] * tile_b[k][threadIdx.x];
		}
		__syncthreads();
	}
	if (row < n && col < n)
	{
		d_result[row * n + col] = tmp;
	}
}


__global__ void gpu_matrix_transpose(int* mat_in, int* mat_out, unsigned int rows, unsigned int cols)
{
	unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int idy = blockIdx.y * blockDim.y + threadIdx.y;

	if (idx < cols && idy < rows)
	{
		unsigned int pos = idy * cols + idx;
		unsigned int trans_pos = idx * rows + idy;
		mat_out[trans_pos] = mat_in[pos];
	}
}



void printFileA(int* matrix, int N)
{
	std::ofstream fout;
	fout.open("matrix/" + std::to_string(N) + "/" + (std::string)"A.txt");

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++) 
			fout << matrix[i * N + j] << ' ';
		fout << '\n';
	}
	fout.close();
}
void printFileB(int* matrix, int N)
{
	std::ofstream fout;
	fout.open("matrix/" + std::to_string(N) + "/" + (std::string)"B.txt");

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++) 
			fout << matrix[i * N + j] << ' ';
		fout << '\n';
	}
	fout.close();
}
void printFileC(int* matrix, int N)
{
	std::ofstream fout;
	fout.open("matrix/" + std::to_string(N) + "/" + (std::string)"C.txt");

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++) 
			fout << matrix[i * N + j] << ' ';
		fout << '\n';
	}
	fout.close();
}

int main(int argc, char const* argv[])
{
	for (int i = 500; i < 3000; i += 250)
	{
		int m, n, k;
		m = n = k = i;
		srand(3333);

		// allocate memory in host RAM, h_cc is used to store CPU result
		int* h_a, * h_b, * h_c;
		cudaMallocHost((void**)&h_a, sizeof(int) * m * n);
		cudaMallocHost((void**)&h_b, sizeof(int) * n * k);
		cudaMallocHost((void**)&h_c, sizeof(int) * m * k);

		// random initialize matrix A
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				h_a[i * n + j] = rand() % 1024;
			}
		}
		printFileA(h_a, n);
		// random initialize matrix B
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < k; ++j) {
				h_b[i * k + j] = rand() % 1024;
			}
		}
		printFileB(h_b, n);

		float gpu_elapsed_time_ms;

		// some events to count the execution time
		cudaEvent_t start, stop;
		cudaEventCreate(&start);
		cudaEventCreate(&stop);

		// start to count execution time of GPU version
		cudaEventRecord(start, 0);
		// Allocate memory space on the device
		int* d_a, * d_b, * d_c;
		cudaMalloc((void**)&d_a, sizeof(int) * m * n);
		cudaMalloc((void**)&d_b, sizeof(int) * n * k);
		cudaMalloc((void**)&d_c, sizeof(int) * m * k);

		// copy matrix A and B from host to device memory
		cudaMemcpy(d_a, h_a, sizeof(int) * m * n, cudaMemcpyHostToDevice);
		cudaMemcpy(d_b, h_b, sizeof(int) * n * k, cudaMemcpyHostToDevice);

		unsigned int grid_rows = (m + BLOCK_SIZE - 1) / BLOCK_SIZE;
		unsigned int grid_cols = (k + BLOCK_SIZE - 1) / BLOCK_SIZE;
		dim3 dimGrid(grid_cols, grid_rows);
		dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);

		// Launch kernel
		
		gpu_square_matrix_mult << <dimGrid, dimBlock >> > (d_a, d_b, d_c, n);
		// Transefr results from device to host
		cudaMemcpy(h_c, d_c, sizeof(int) * m * k, cudaMemcpyDeviceToHost);
		cudaThreadSynchronize();
		// time counting terminate
		cudaEventRecord(stop, 0);
		cudaEventSynchronize(stop);

		// compute time elapse on GPU computing
		cudaEventElapsedTime(&gpu_elapsed_time_ms, start, stop);
		printf("Matrix multiplication time size %dx%d on GPU: %f s.\n\n", m, n, gpu_elapsed_time_ms / 1000);

		

		printFileC(h_c, n);

		
		// free memory
		cudaFree(d_a);
		cudaFree(d_b);
		cudaFree(d_c);
		cudaFreeHost(h_a);
		cudaFreeHost(h_b);
		cudaFreeHost(h_c);
	}
	return 0;
}
