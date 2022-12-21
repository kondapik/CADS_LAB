#include <iostream>
#include <atomic>
#include <stdlib.h>
#include <climits>

using std::atomic;

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

public:
    tStack(){
        top.store(NULL);
        stackSize.store(0);
    }

    void push(int key){
        tNode* pushNode = new tNode(key);
        while(true){
            tNode* topNode = top.load();
            pushNode->next = topNode;
            if (top.compare_exchange_strong(topNode, pushNode)){
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
                return -1;
            }
            if (top.compare_exchange_strong(popNode, popNode->next)){
                stackSize--;
                //logic to atomically remove the node
                return popNode->value;
            }
        }
    }

    int size(){
        return stackSize.load();
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