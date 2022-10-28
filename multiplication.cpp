#include <iostream>
#include <cstdio>
#include <ctime>
#include <fstream>
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
   
    for (int ka = 10; ka < 101; ka += 10)
    {
        fstream fs("C:/Users/admin/Desktop/M.txt");
        fstream fs1("C:/Users/admin/Desktop/M1.txt");
        int time = clock();
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
        for (int i = 0; i < row1; i++)
        {
            c[i] = new double[col2];
            for (int j = 0; j < col2; j++)
            {
                c[i][j] = 0;
                for (int k = 0; k < col1; k++)
                    c[i][j] += a[i][k] * b[k][j];
            }
        }
        if (out_python.is_open())
        {
            
            for (int i = 0; i < row1; i++)
            {
                for (int j = 0; j < col2; j++)
                    out_python << c[i][j] << " ";
                out_python << std::endl;
            }
        }
        cout << "\nTime = ";
        cout << time << " tik";
        out << "Matrix {";
        out << ka << "x" << ka << '}' << endl;
        out << "\nTime = ";
        out << time << " tiks" << endl;
        out << "_________________________" << endl;
    }
    return 0;
}


