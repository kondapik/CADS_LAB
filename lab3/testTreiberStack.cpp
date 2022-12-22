#include <bits/stdc++.h>
#include "TreiberStack.cpp"
#include <iostream>
#include <stdlib.h>
#include <mutex>

#define PRINT_FINAL false
#define THREAD_COUNT 16
#define OPERATIONS 100
#define DEBUG false

using namespace std;

// enum listOps {PUSH, POP, SIZE};

// typedef struct setOperation{
//   listOps method;
//   int value;
//   int expOutput;
// } setOperation;

typedef struct setTestCase{
  listOps method;
  int value;
} setTestCase;

std::vector<setTestCase> allTestCases;
pthread_mutex_t threadLock;

void createTestCases(int testCasesNo){
    std::cout << "Creating random test cases" << std::endl;
    for(int sNo = 0; sNo < testCasesNo; sNo++){
        setTestCase currTestCase;
        currTestCase.value = (rand() % 256);
        currTestCase.method = static_cast<listOps>(rand() % 3); // hardcoded for 3 methods
        allTestCases.push_back(currTestCase);
    }
}

tStack testSet;

void workerThread(){
    for(int sNo = 0; sNo < OPERATIONS; sNo++){
        switch (allTestCases[sNo].method){
            case PUSH:
                testSet.push(allTestCases[sNo].value);
                break;
            case POP:
                testSet.pop();
                break;
            default:
                testSet.size();
        }
    }
}

int main(int argc, char *argv[]){

    tStack mainTestSet;
    
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
            case PUSH:
                returnVal = 't';
                mainTestSet.push(testSet.operationSequence[sNo].value);
                cout << "(push," << testSet.operationSequence[sNo].value << ")" << endl;
                break;
            case POP:
                returnVal = mainTestSet.pop();
                cout << "(pop," << ((testSet.operationSequence[sNo].expOutput > 0) ? to_string(testSet.operationSequence[sNo].expOutput) : "*") << ")" << endl;
                break;
            default:
                returnVal = mainTestSet.size();
                cout << "(size," << testSet.operationSequence[sNo].expOutput << ")" << endl;
        }

        if (returnVal != testSet.operationSequence[sNo].expOutput){
            printf("The above operation is not allowed!\n");
            printf("The expected value is %d and the returned value is %d\n", testSet.operationSequence[sNo].expOutput, returnVal);
            if (DEBUG) {throw std::invalid_argument( "received wrong result" );}
            break;
        }else if(sNo == OPERATIONS*THREAD_COUNT - 1){
            cout << "All Operations are executed successfully" << endl;
        }
    }
    mainTestSet.printState();

    return 0;
}


