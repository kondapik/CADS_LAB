#include <ctime>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>
#include "LazyList.cpp"
#include "OptimisticList.cpp"
#include "FineList.cpp"


int helpText(char *program)
{
    std::cout << "Usage: " << program << " T I V" << std::endl;
    std::cout << std::endl;
    std::cout << "Required Inputs: " << std::endl;
    std::cout << "  T\tNumber of Threads. (a natural number)" << std::endl;
    std::cout << "  I\tPercentage of 'ctn' operations in generated test case. (a natural number.)" << std::endl;
    std::cout << "  V\tLargest value of input value for operations used in test case. (a natural number.)" << std::endl;
    std::cout << "Optional Command: " << std::endl;
    std::cout << "  -h\tDisplay this information." << std::endl;
    std::cout << std::endl;
    exit(1);
}

int main(int argc, char *argv[]){

    // SetList mainTestSet;
    int maxThreads, ctnPercent, maxNumber;
    if (argc == 1 || "-h" == std::string(argv[1]) || argc < 4 || argc > 5)
    {
        helpText(argv[0]);
    }
    try
    {
      maxThreads = std::stoi(argv[1]);
    } catch (...)
    {
      helpText(argv[0]);
    }

    if (maxThreads < 1)
    {
      helpText(argv[0]);
    }

    try
    {
      ctnPercent = std::stoi(argv[2]);
    } catch (...)
    {
      helpText(argv[0]);
    }

    try
    {
      maxNumber = std::stoi(argv[3]);
    } catch (...)
    {
      helpText(argv[0]);
    }
    
}