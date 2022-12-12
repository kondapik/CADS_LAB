#include "SetList.cpp"
#include <ctime>
#include <stdlib.h>
#include <vector>

using namespace std;

typedef struct setOperation{
  int method;
  int value;
  bool expOutput;
} setOperation;

vector<setOperation> operationSequence;

void createOperationSequence(int testCasesNo){
    SetList testSet;
    cout << "Creating random test sequence" << endl;

    for(int sNo = 0; sNo < testCasesNo; sNo++){
        setOperation currOperation;
        currOperation.value = (rand() % 30);
        currOperation.method = (rand() % 3); // hardcoded for 3 methods
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

int main(){
    SetList testSet;
    createOperationSequence(100);
    srand(time(NULL));

    for(int sNo = 0; sNo < 100; sNo++){
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
        }else if(sNo == 99){
            cout << "All Operations are executed successfully" << endl;
        }
    }
    testSet.printState();

    return 0;
}