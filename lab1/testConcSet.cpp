#include "SetList.cpp"
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <thread>

using namespace std;

int helpText(char *program)
{
    std::cout << "Usage: " << program << "T N" << std::endl;
    std::cout << std::endl;
    std::cout << "Required Inputs: " << std::endl;
    std::cout << "  T\tNumber of Threads. (a natural number)" << std::endl;
    std::cout << "  N\tNumber of Operations. (a natural number.)" << std::endl;
    std::cout << "Optional Command: " << std::endl;
    std::cout << "  -h\tDisplay this information." << std::endl;
    std::cout << std::endl;
    exit(1);
}

typedef struct setOperation{
  int method;
  int value;
  bool expOutput;
} setOperation;

typedef struct setTestCase{
  int method;
  int value;
} setTestCase;

SetList testSet;

vector<setOperation> operationSequence;
vector<setTestCase> allTestCases;

void createTestCases(int testCasesNo){
    cout << "Creating random test cases" << endl;
    for(int sNo = 0; sNo < testCasesNo; sNo++){
        setTestCase currTestCase;
        currTestCase.value = (rand() % 30);
        currTestCase.method = (rand() % 3); // hardcoded for 3 methods
        allTestCases.push_back(currTestCase);
    }
}

void workerThread(int maxNumber){
    for(int sNo = 0; sNo < maxNumber; sNo++){
        setOperation currOperation;
        currOperation.value = allTestCases[sNo].value;
        currOperation.method = allTestCases[sNo].method; // hardcoded for 3 methods
        switch (currOperation.method){
            case 0:
                currOperation.expOutput = testSet.add(currOperation.value);
                break;
            case 1:
                currOperation.expOutput = testSet.rmv(currOperation.value);
                break;
            default:
                currOperation.expOutput = testSet.ctn(currOperation.value);
        }
        operationSequence.push_back(currOperation);
    }
}

int main(int argc, char *argv[]){

    SetList mainTestSet;
    int maxThreads, maxNumber;
    if (argc == 1 || "-h" == std::string(argv[1]) || argc < 3 || argc > 4)
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
      maxNumber = std::stoi(argv[2]);
    } catch (...)
    {
      helpText(argv[0]);
    }
    createTestCases(maxNumber);
    
    thread *threadHandles = new thread[maxThreads];

    for (int threadNo=0; threadNo<maxThreads; ++threadNo){
        threadHandles[threadNo] = thread(workerThread, maxNumber);
    }
    
    for (int threadNo=0; threadNo<maxThreads; ++threadNo){
        threadHandles[threadNo].join();
    }

    for(int sNo = 0; sNo < maxNumber*maxThreads; sNo++){
        bool returnVal;
        switch (operationSequence[sNo].method){
            case 0:
                returnVal = testSet.add(operationSequence[sNo].value);
                cout << "(add," << operationSequence[sNo].value << "," << boolalpha << operationSequence[sNo].expOutput << ")" << endl;
                break;
            case 1:
                returnVal = testSet.rmv(operationSequence[sNo].value);
                cout << "(rmv," << operationSequence[sNo].value << "," << boolalpha << operationSequence[sNo].expOutput << ")" << endl;
                break;
            default:
                returnVal = testSet.ctn(operationSequence[sNo].value);
                cout << "(ctn," << operationSequence[sNo].value << "," << boolalpha << operationSequence[sNo].expOutput << ")" << endl;
        }

        if (returnVal != operationSequence[sNo].expOutput){
            cout << "The above operation is not allowed!" << endl;
            break;
        }else if(sNo == maxNumber - 1){
            cout << "All Operations are executed successfully" << endl;
        }
    }
    testSet.printState();

    return 0;
}