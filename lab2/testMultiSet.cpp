#include <bits/stdc++.h>
#include "FineMultiSet.cpp"
#include <iostream>
#include <stdlib.h>
#include <mutex>

#define PRINT_FINAL false
#define THREAD_COUNT 16
#define OPERATIONS 100

using namespace std;

enum listOps {add, rmv, ctn};

typedef struct setOperation{
  listOps method;
  int value;
  int expOutput;
} setOperation;

typedef struct setTestCase{
  listOps method;
  int value;
} setTestCase;

std::vector<setOperation> operationSequence;
std::vector<setTestCase> allTestCases;
pthread_mutex_t threadLock;

void createTestCases(int testCasesNo){
    std::cout << "Creating random test cases" << std::endl;
    for(int sNo = 0; sNo < testCasesNo; sNo++){
        setTestCase currTestCase;
        currTestCase.value = (rand() % 1024);
        currTestCase.method = static_cast<listOps>(rand() % 3); // hardcoded for 3 methods
        allTestCases.push_back(currTestCase);
    }
}

FineMultiSet testSet;

void workerThread(){
    for(int sNo = 0; sNo < OPERATIONS; sNo++){
        setOperation currOperation;
        currOperation.value = allTestCases[sNo].value;
        currOperation.method = allTestCases[sNo].method; // hardcoded for 3 methods
        switch (currOperation.method){
            case add:
                currOperation.expOutput = testSet.add(currOperation.value);
                break;
            case rmv:
                currOperation.expOutput = testSet.rmv(currOperation.value);
                break;
            default:
                currOperation.expOutput = testSet.ctn(currOperation.value);
        }
        pthread_mutex_lock(&threadLock);
        operationSequence.push_back(currOperation);
        pthread_mutex_unlock(&threadLock);
    }
}

int main(int argc, char *argv[]){

    FineMultiSet mainTestSet;
    
    srand(time(NULL));

    createTestCases(OPERATIONS);
    
    thread *threadHandles = new thread[THREAD_COUNT];

    std::cout << "Running test cases on all threads" << std::endl;

    for (int threadNo=0; threadNo<THREAD_COUNT; ++threadNo){
        threadHandles[threadNo] = thread(workerThread);
    }
    
    for (int threadNo=0; threadNo<THREAD_COUNT; ++threadNo){
        threadHandles[threadNo].join();
    }

    std::cout << "Monitoring generated sequence" << std::endl;
    for(int sNo = 0; sNo < OPERATIONS*THREAD_COUNT; sNo++){
        int returnVal;
        switch (operationSequence[sNo].method){
            case add:
                returnVal = mainTestSet.add(operationSequence[sNo].value);
                cout << "(add," << operationSequence[sNo].value << "," << boolalpha << operationSequence[sNo].expOutput << ")" << endl;
                break;
            case rmv:
                returnVal = mainTestSet.rmv(operationSequence[sNo].value);
                cout << "(rmv," << operationSequence[sNo].value << "," << boolalpha << operationSequence[sNo].expOutput << ")" << endl;
                break;
            default:
                returnVal = mainTestSet.ctn(operationSequence[sNo].value);
                cout << "(ctn," << operationSequence[sNo].value << "," << operationSequence[sNo].expOutput << ")" << endl;
        }

        if (returnVal != operationSequence[sNo].expOutput){
            cout << "The above operation is not allowed!" << endl;
            break;
        }else if(sNo == OPERATIONS*THREAD_COUNT - 1){
            cout << "All Operations are executed successfully" << endl;
        }
    }
    mainTestSet.printState();

    return 0;
}


