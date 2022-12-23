#include <bits/stdc++.h>
#include "FineMultiSet.cpp"
#include <iostream>
#include <stdlib.h>
#include <mutex>

#define PRINT_FINAL false
#define THREAD_COUNT 16
#define OPERATIONS 100

using namespace std;

typedef struct setTestCase{
  listOps method;
  int value;
} setTestCase;


std::vector<setTestCase> allTestCases;

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
        switch (allTestCases[sNo].method){
            case add:
                testSet.add(allTestCases[sNo].value);
                break;
            case rmv:
                testSet.rmv(allTestCases[sNo].value);
                break;
            default:
                testSet.ctn(allTestCases[sNo].value);
        }
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
        switch (testSet.operationSequence[sNo].method){
            case add:
                returnVal = mainTestSet.add(testSet.operationSequence[sNo].value);
                cout << "(add," << testSet.operationSequence[sNo].value << "," << boolalpha << testSet.operationSequence[sNo].expOutput << ")" << endl;
                break;
            case rmv:
                returnVal = mainTestSet.rmv(testSet.operationSequence[sNo].value);
                cout << "(rmv," << testSet.operationSequence[sNo].value << "," << boolalpha << testSet.operationSequence[sNo].expOutput << ")" << endl;
                break;
            default:
                returnVal = mainTestSet.ctn(testSet.operationSequence[sNo].value);
                cout << "(ctn," << testSet.operationSequence[sNo].value << "," << testSet.operationSequence[sNo].expOutput << ")" << endl;
        }

        if (returnVal != testSet.operationSequence[sNo].expOutput){
            cout << "The above operation is not allowed!" << endl;
            printf("The expected value is %d and the returned value is %d\n", testSet.operationSequence[sNo].expOutput, returnVal);
            break;
        }else if(sNo == OPERATIONS*THREAD_COUNT - 1){
            cout << "All Operations are executed successfully" << endl;
        }
    }
    mainTestSet.printState();

    return 0;
}


