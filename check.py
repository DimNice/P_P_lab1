import numpy as np
import filecmp

out = open("C:/Users/admin/Desktop/result_python.txt", 'w+')

for k in range(10, 101, 10):
    M = open("C:/Users/admin/Desktop/M.txt", "r")
    M1 = open("C:/Users/admin/Desktop/M1.txt", "r")
    matrix1 = np.ones((k, k), dtype=int)
    for i in range(0, k):
        for j in range(0, k):
            matrix1[i][j] = int(M.readline())
    #print(matrix1)
    matrix2 = np.ones((k, k), dtype=int)
    for i in range(0, k):
        for j in range(0, k):
            matrix2[i][j] = int(M1.readline())
    #print(matrix2)
    mul = matrix1.dot(matrix2)
    #print(mul)
    for i in range(0, k):
        for j in range(0, k):
            out.write(str(mul[i][j]) + ' ')
        out.write('\n')
    M.close()
    M1.close()
out.close()
if filecmp.cmp('C:/Users/admin/Desktop/result_python.txt', 'C:/Users/admin/Desktop/result_C++.txt', shallow=False):
    print('Равны, всё правильно :)')
else:
    print('Неравны, всё неправильно, переделывай! >:(')

