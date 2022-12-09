#include <iostream>
#include <chrono>
#include <mpi.h>
#include <stdio.h>
#include <string>
#include <windows.h>
#include <vector>
#include <fstream>
using namespace std;

void print(vector<int>& v) {
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << endl;
    }
}

int main(int argc, char* argv[])
{
    int cr = 100;
    vector<int> matrix1(cr*cr);
    vector<int> matrix2(cr * cr);
    for (int i = 0; i < cr * cr; i++) {
        matrix1[i] = (rand() % 9 +1 );
        matrix2[i] = (rand() % 9 +1);
    }
    std::ofstream data("C:/Users/admin/Desktop/M.txt");
    for (int i = 0; i < cr * cr; i++) {
        data << matrix1[i] << " " << matrix2[i] << " ";
    }
    data.close();

    int SIZE = 100;
    setlocale(LC_ALL, "ru");
    MPI_Init(&argc, &argv);
    int my_rank, proc_num;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int dimension = 0;
    vector<int> matrix_1(SIZE * SIZE);
    vector<int> matrix_2_transponse(SIZE * SIZE);
    vector<int> matrix_4(SIZE * SIZE);
    if (my_rank == 0)
    {
        vector<int> matrix_2(SIZE * SIZE);
        ifstream data("C:/Users/admin/Desktop/M.txt");
        int temp;
        bool changer = true;
        int i1 = 0;
        int i2 = 0;
        while (data >> temp) {
            if (changer) {
                matrix_1[i1] = temp;
                i1++;
            }
            else {
                matrix_2[i2] = temp;
                i2++;
            }
            changer = !changer;
        }
        for (int i = 0; i < SIZE; ++i)
        {
            for (int j = 0; j < SIZE; ++j)
            {
                matrix_2_transponse[i * SIZE + j] = matrix_2[j * SIZE + i];
            }
        }
        dimension = SIZE;
    }
    MPI_Bcast(&dimension, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (my_rank)
    {
        vector<int> matrix_2_transponse(SIZE * SIZE);
    }
    int local_size = dimension / proc_num;
    vector<int> local_a_part(local_size * dimension);  
    vector<int> local_result(local_size * dimension);
    int* matrix_1_pointer = &matrix_1[0];
    int* local_a_part_pointer = &local_a_part[0];
    int* local_result_pointer = &local_result[0];
    int* matrix_4_pointer = &matrix_4[0];
    int* matrix_2_transponse_pointer = &matrix_2_transponse[0];
    MPI_Scatter(matrix_1_pointer + my_rank * dimension * local_size, dimension * local_size, MPI_INT, local_a_part_pointer, dimension * local_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrix_2_transponse_pointer, dimension * dimension, MPI_INT, 0, MPI_COMM_WORLD);
    double begin = MPI_Wtime();
    for (int i = 0; i < local_size; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            local_result[i * dimension + j] = 0;
            for (int k = 0; k < dimension; ++k)
            {
                local_result[i * dimension + j] += local_a_part[i * dimension + k] * matrix_2_transponse[j * dimension + k];
            }
        }
    }
    double end = MPI_Wtime();
    MPI_Gather(local_result_pointer, local_size * dimension, MPI_INT, matrix_4_pointer, dimension * local_size, MPI_INT, 0, MPI_COMM_WORLD);
    if (my_rank == 0) {
        ofstream matrix("C:/Users/admin/Desktop/Result_C++.txt");
        for (long int i = 0; i < SIZE * SIZE; ++i) {
            matrix << matrix_4[i] << " ";
        }
        matrix.close();
        cout << "Matrix { " << SIZE << " on " << SIZE<<" }"<< endl;
        cout << "Time = " <<(end - begin)<< "sec" << endl;
    }
    MPI_Finalize();
    return 0;
}
