#include <iostream>
#include <stdlib.h>
#include <climits>
#include <mutex>

class fineNode {

public:
    int value;
    fineNode* next;
    int count;

// private:
    std::mutex nodeLock;

public:
    fineNode(int key){
        value = key;
        next = NULL;
        count = 1;
    }

    // ~fineNode() {pthread_mutex_unlock(&nodeLock);}

    // void lock(){
    //     pthread_mutex_lock(&nodeLock);
    // }

    // void unlock(){
    //     pthread_mutex_unlock(&nodeLock);
    // }
};


class FineMultiSet {
private:
    fineNode* head;

public:
    FineMultiSet() {   
        head = new fineNode(INT_MIN);
        head->next = new fineNode(INT_MAX);
    }

    bool add(int key) {
        // Starting with head traversing the list one node at a time by taking locks
        head->nodeLock.lock();
        fineNode* prev = head;
        fineNode* curr = head->next;
        curr->nodeLock.lock();
        while(curr->value < key){
            prev->nodeLock.unlock();
            prev = curr;
            curr = prev->next;
            curr->nodeLock.lock();
        }
        // Return false if the key is already present
        if (curr->value == key){
            curr->count++;
            curr->nodeLock.unlock();
            prev->nodeLock.unlock();
            return true;
        }
        // Create new node and add it at the current location
        fineNode* newNode = new fineNode(key);
        newNode->next = curr;
        prev->next = newNode;
        curr->nodeLock.unlock();
        prev->nodeLock.unlock();
        return true;
    }

    bool rmv(int key){
        // Starting at the head and traversing the list one node at a time by taking locks along the way
        head->nodeLock.lock();
        fineNode* prev = head;
        fineNode* curr = head->next;
        curr->nodeLock.lock();
        while(curr->value < key){
            prev->nodeLock.unlock();
            prev = curr;
            curr = prev->next;
            curr->nodeLock.lock();
        }
        // Give the pointer of next node to previous node and return true if the key is present
        if (curr->value == key){
            if (curr->count > 1){
                curr->count--;
                curr->nodeLock.unlock();
                prev->nodeLock.unlock();
            }else{
                prev->next = curr->next;
                curr->nodeLock.unlock();
                prev->nodeLock.unlock();
                free(curr);
            }
            return true;
        }
        // Return false if key is not found
        curr->nodeLock.unlock();
        prev->nodeLock.unlock();
        return false;
    }

    int ctn(int key){
        // Starting at the head and traversing the list one node at a time by taking locks along the way
        head->nodeLock.lock();
        fineNode* prev = head;
        fineNode* curr = head->next;
        curr->nodeLock.lock();
        while(curr->value < key){
            prev->nodeLock.unlock();
            prev = curr;
            curr = prev->next;
            curr->nodeLock.lock();
        }
        // return true if the key is present
        if (curr->value == key){
            curr->nodeLock.unlock();
            prev->nodeLock.unlock();
            return curr->count;
        }
        // Return false if key is not found
        curr->nodeLock.unlock();
        prev->nodeLock.unlock();
        return 0;
    }

    void printState()
    {
        head->nodeLock.lock();
        fineNode* prev = head;
        fineNode* curr = head->next;
        curr->nodeLock.lock();
        std::cout << "The state of the fine multi set after successful operation is: ";
        while(curr->value < INT_MAX){
            for (int i = 0; i < curr->count; i++){
                std::cout << curr->value <<" ";
            }
            prev->nodeLock.unlock();
            prev = curr;
            curr = prev->next;
            curr->nodeLock.lock();
        }
        std::cout << "." << std::endl;
        curr->nodeLock.unlock();
        prev->nodeLock.unlock();
    }
};