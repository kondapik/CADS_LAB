#include "SetList.cpp"
#include <ctime>
#include <stdlib.h>

using namespace std;

int main(){
    SetList testSet;

    srand(time(NULL));

    for(int sNo = 0; sNo < 100; sNo++){
        int rndValue = (rand() % 30);
        bool rndResult = (rand()%2 == 1)?true:false;
        int rndOperation = (rand() % 3);

        bool returnVal;

        switch (rndOperation){
            case 0:
                returnVal = testSet.add(rndValue);
                cout << "(add," << rndValue << "," << boolalpha << rndResult << ")" << endl;
                break;
            case 1:
                returnVal = testSet.rmv(rndValue);
                cout << "(rmv," << rndValue << "," << boolalpha << rndResult << ")" << endl;
                break;
            default:
                returnVal = testSet.ctn(rndValue);
                cout << "(ctn," << rndValue << "," << boolalpha << rndResult << ")" << endl;
        }

        if (returnVal != rndResult){
            cout << "The above operation is not allowed!" << endl;
            break;
        }else if(sNo == 99){
            cout << "All Operations are executed successfully" << endl;
        }
    }
    testSet.printState();

    return 0;
}