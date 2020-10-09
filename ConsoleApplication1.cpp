// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "mpi.h"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>


using namespace std;
using namespace utils;

int main(int argc, char** argv) {

    int* nums = nullptr;
    int numsSize = 0;

    if (argc == 4) {
        createFileOfNums(argv[1], atoi(argv[2]), -10000, 10000);
    }
    else if (argc != 3) {
        cout << "Not all arguments specified!" << endl;
        return (1);
    }
    else
    {
        string inFile(argv[1]);
        string outFile(argv[2]);
        MPI_Init(&argc, &argv);

        int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (rank == 0) {
            tie(nums, numsSize) = loadNums(inFile);
        }
        MPI_Bcast(&numsSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        double wtime = MPI_Wtime();

        if (size == 1) {
            quicksort(nums, numsSize);
        }
        else {
            quicksortMPI(nums, numsSize, rank, size);
        }

        MPI_Barrier(MPI_COMM_WORLD);
        wtime = MPI_Wtime() - wtime;

        if (rank == 0) {
            char buf[1000];
            int res = -1;
            res = snprintf(buf, sizeof(buf), "%d Process: %d Size, %f Time\n", size, numsSize, wtime);
            string str = "error!";
            if (res >= 0 && res < sizeof(buf))
                str = buf;
            cout << str << endl;
            writeLogTime(str);
            printAnswer(nums, numsSize, outFile);
        }
        MPI_Finalize();
    }
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
