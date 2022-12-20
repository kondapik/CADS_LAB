#include <bits/stdc++.h>
#include "LazyList.cpp"
#include "OptimisticList.cpp"
#include "FineList.cpp"

enum listOps {add, rmv, ctn};

// typedef struct setOperation{
//   listOps method;
//   int value;
//   bool expOutput;
// } setOperation;
#define PRINT_FINAL false
typedef struct setTestCase{
  listOps method;
  int value;
} setTestCase;

// std::vector<setOperation> operationSequence;
std::vector<setTestCase> allTestCases;

LazyList lzySet;
OptimisticList optSet;
FineList finSet;

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

void createTestCases(int ctnPercent, int maxNumber){
    if(PRINT_FINAL) {std::cout << "Creating random test cases" << std::endl;}
    int ctnLimit = 500*ctnPercent;
    int addLimit = 0.9*500*(1-ctnPercent);

    for(int sNo = 0; sNo < 500; sNo++){
        setTestCase currTestCase;
        currTestCase.value = (rand() % maxNumber);
        if (sNo < ctnLimit){
            currTestCase.method = ctn;
        }else if(sNo < ctnLimit + addLimit){
            currTestCase.method = add;
        }else {
            currTestCase.method = rmv;
        }
        allTestCases.push_back(currTestCase);
    }

    std::shuffle(allTestCases.begin(), allTestCases.end(), std::random_device());
}

void lazyThread(){
    // std::cout << "in Lazy Thread" << std::endl;
    for(int sNo = 0; sNo < 500; sNo++){
        switch (allTestCases[sNo].method){
            case add:
                lzySet.add(allTestCases[sNo].value);
                break;
            case rmv:
                lzySet.rmv(allTestCases[sNo].value);
                break;
            default:
                lzySet.ctn(allTestCases[sNo].value);
        }
    }
}

void optimisticThread(){
    // std::cout << "in Optimistic Thread" << std::endl;
    for(int sNo = 0; sNo < 500; sNo++){
        switch (allTestCases[sNo].method){
            case add:
                optSet.add(allTestCases[sNo].value);
                break;
            case rmv:
                optSet.rmv(allTestCases[sNo].value);
                break;
            default:
                optSet.ctn(allTestCases[sNo].value);
        }
    }
}

void fineThread(){
    // std::cout << "in Fine Thread" << std::endl;
    for(int sNo = 0; sNo < 500; sNo++){
        switch (allTestCases[sNo].method){
            case add:
                finSet.add(allTestCases[sNo].value);
                break;
            case rmv:
                finSet.rmv(allTestCases[sNo].value);
                break;
            default:
                finSet.ctn(allTestCases[sNo].value);
        }
    }
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

    srand(time(NULL));

    createTestCases(ctnPercent/100, maxNumber);

    std::thread *threadHandles = new std::thread[maxThreads];

    if(PRINT_FINAL) {std::cout << "Testing Lazy Set: " << std::endl;}
    //* Running test case on lazy set
    //Starting Timer
    auto start_time = std::chrono::system_clock::now();

    for (int threadNo=0; threadNo<maxThreads; ++threadNo){
        threadHandles[threadNo] = std::thread(lazyThread);
    }
    
    for (int threadNo=0; threadNo<maxThreads; ++threadNo){
        threadHandles[threadNo].join();
    }
    std::chrono::duration<double> duration = (std::chrono::system_clock::now() - start_time);

    std::cout << "Lazy Set Test --- No_of_Threads: " << maxThreads << " cntPercent: " << ctnPercent << " duration: " << duration.count() << std::endl;
    if(PRINT_FINAL) {lzySet.printState();}


    if(PRINT_FINAL) {std::cout << "Testing Optimistic Set: " << std::endl;}
    //* Running test case on optimistic set
    // Starting Timer
    start_time = std::chrono::system_clock::now();

    for (int threadNo=0; threadNo<maxThreads; ++threadNo){
        threadHandles[threadNo] = std::thread(optimisticThread);
    }
    
    for (int threadNo=0; threadNo<maxThreads; ++threadNo){
        threadHandles[threadNo].join();
    }
    duration = (std::chrono::system_clock::now() - start_time);

    std::cout << "Optimistic Set Test --- No_of_Threads: " << maxThreads << " cntPercent: " << ctnPercent << " duration: " << duration.count() << std::endl;
    if(PRINT_FINAL) {optSet.printState();}


    if(PRINT_FINAL) {std::cout << "Testing Fine Set: " << std::endl;}
    //* Running test case on fine set
    // Starting Timer
    start_time = std::chrono::system_clock::now();

    for (int threadNo=0; threadNo<maxThreads; ++threadNo){
        threadHandles[threadNo] = std::thread(fineThread);
    }
    
    for (int threadNo=0; threadNo<maxThreads; ++threadNo){
        threadHandles[threadNo].join();
    }
    duration = (std::chrono::system_clock::now() - start_time);

    std::cout << "Fine Set Test --- No_of_Threads: " << maxThreads << " cntPercent: " << ctnPercent << " duration: " << duration.count() << std::endl;
    if(PRINT_FINAL) {finSet.printState();}

    std::cout << std::endl << std::endl;
}