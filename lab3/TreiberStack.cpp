#include <iostream>
#include <atomic>
#include <stdlib.h>
#include <climits>

using std::atomic;

class ScopedMutex {
public:
    ScopedMutex(pthread_mutex_t *m) : theMutex_(m) {pthread_mutex_lock(theMutex_);}
    ~ScopedMutex() { pthread_mutex_unlock(theMutex_); }
private:
    pthread_mutex_t *theMutex_;
};

enum listOps {PUSH, POP, SIZE};

typedef struct setOperation{
  listOps method;
  int value;
  int expOutput;
} setOperation;

class tNode {

public:
    int value;
    tNode* next;

public:
    tNode(int key){
        value = key;
        next = NULL;
    }
};

class tStack{
private:
    atomic<tNode*> top;
    atomic<int> stackSize;
    pthread_mutex_t linearMutex; 

public:
    std::vector<setOperation> operationSequence;

    tStack(){
        top.store(NULL);
        stackSize.store(0);
    }

    void addOperationSequence(listOps opr, int key, int returnVal){
        ScopedMutex coarseLock(&linearMutex);
        setOperation currOperation;
        currOperation.value = key;
        currOperation.method = opr;
        currOperation.expOutput = returnVal;
        // switch (opr){
        //     case PUSH:
        //         currOperation.expOutput = 't';
        //         break;
        //     default:
        //         currOperation.expOutput = returnVal;
        // }
        operationSequence.push_back(currOperation);
    }

    void push(int key){
        tNode* pushNode = new tNode(key);
        while(true){
            tNode* topNode = top.load();
            pushNode->next = topNode;
            if (top.compare_exchange_strong(topNode, pushNode)){
                addOperationSequence(listOps::PUSH, key, 't');
                stackSize++;
                //logic to atomically remove the node
                return;
            }
        }
    }

    int pop(){
        while(true){
            tNode* popNode = top.load();
            if (popNode == NULL){
                addOperationSequence(listOps::POP, 0, -1);
                return -1;
            }
            if (top.compare_exchange_strong(popNode, popNode->next)){
                addOperationSequence(listOps::POP, 0, popNode->value);
                stackSize--;
                //logic to atomically remove the node
                return popNode->value;
            }
        }
    }

    int size(){
        int tmpSize = stackSize.load();
        addOperationSequence(listOps::SIZE, 0, tmpSize);
        return tmpSize;
    }

    void printState(){
        tNode* printNode = top.load();
        std::cout << "The state of the fine set after successful operation is: ";
        while(printNode != NULL){
            std::cout << printNode->value <<" ";
            printNode = printNode->next;
        }
        std::cout << "." << std::endl;
    }
};