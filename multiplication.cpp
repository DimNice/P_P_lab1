#include <iostream>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <omp.h>
using namespace std;
int main()
{
    int row1, row2, col1, col2;
    double** a, ** b, ** c;
    system("chcp 1251");
    system("cls");
    fstream fs("C:/Users/admin/Desktop/M.txt");
    fstream fs1("C:/Users/admin/Desktop/M1.txt");
    /*std::ofstream in1;
    in1.open("C:/Users/admin/Desktop/M1.txt");
    if (in1.is_open())
    {
        for (int i = 0; i < 1000000; i++)
        {
            in1 << rand() % 110 - 55 << "\n";
        }
    }
    cout << "Pervuy gotov";
    std::ofstream in;
    in.open("C:/Users/admin/Desktop/M.txt");
    if (in.is_open())
    {
        for (int i = 0; i < 1000000; i++)
        {
            in << rand() % 100 - 50 << "\n";
        }
    }
    cout << "Vtoroy gotov";
    */
    std::ofstream out;
    std::ofstream out_python;
    out.open("C:/Users/admin/Desktop/cute_result.txt");
    out_python.open("C:/Users/admin/Desktop/result_C++.txt");
    for (int ka = 100; ka < 1001; ka += 100)
    {
        fstream fs("C:/Users/admin/Desktop/M.txt");
        fstream fs1("C:/Users/admin/Desktop/M1.txt");
        row1 = row2 = col1 = col2=ka;
        a = new double* [row1];
        for (int i = 0; i < row1; i++)
        {
            a[i] = new double[col1];
            for (int j = 0; j < col1; j++)
            {
                fs >> a[i][j];
                //cout << a[i][j]<< endl;
            }
        }
        b = new double* [row2];
        for (int i = 0; i < row2; i++)
        {
            b[i] = new double[col2];
            for (int j = 0; j < col2; j++)
            {
                
                fs1 >> b[i][j];
                //cout << b[i][j] << endl;
            }
        }
        c = new double* [row1];
        omp_set_dynamic(0);
        omp_set_num_threads(4);

        
        double start = omp_get_wtime();
#pragma omp parallel for
        for (int i = 0; i < row1; i++)
        {
            int num = omp_get_num_threads();
            cout << "\nnumber_of_threads = ";
            cout << num << endl;
            c[i] = new double[col2];
            for (int j = 0; j < col2; j++)
            {
                c[i][j] = 0;
                for (int k = 0; k < col1; k++)
                    c[i][j] += a[i][k] * b[k][j];
            }
        }
        double end = omp_get_wtime();
        double time = end - start;
        out << "\nTime = ";
        out << time << " tiks" << endl;
        out << "for" << endl;
        out << "Matrix {";
        out << ka << "x" << ka << '}' << endl;
        out << "_________________________" << endl;
        cout << "\nTime = ";
        cout << time << " tik";
        if (out_python.is_open())
        {
            for (int i = 0; i < row1; i++)
            {
                for (int j = 0; j < col2; j++)
                    out_python << c[i][j] << " ";
                out_python << std::endl;
            }
        }
        delete[] a;
        delete[] b;
        delete[] c;
    }
    return 0;
}
