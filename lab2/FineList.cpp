#include <iostream>
#include <stdlib.h>
#include <climits>
#include <mutex>

class fineNode {

public:
    int value;
    fineNode* next;

private:
    pthread_mutex_t nodeLock;

public:
    fineNode(int key){
        value = key;
        next = NULL;
    }

    // ~fineNode() {pthread_mutex_unlock(&nodeLock);}

    void lock(){
        pthread_mutex_lock(&nodeLock);
    }

    void unlock(){
        pthread_mutex_unlock(&nodeLock);
    }
};


class FineList {
private:
    fineNode* head;

public:
    FineList() {   
        head = new fineNode(INT_MIN);
        head->next = new fineNode(INT_MAX);
    }

    bool add(int key) {
        // Starting with head traversing the list one node at a time by taking locks
        head->lock();
        fineNode* prev = head;
        fineNode* curr = head->next;
        curr->lock();
        while(curr->value < key){
            prev->unlock();
            prev = curr;
            curr = prev->next;
            curr->lock();
        }
        // Return false if the key is already present
        if (curr->value == key){
            return false;
        }
        // Create new node and add it at the current location
        fineNode* newNode = new fineNode(key);
        newNode->next = curr;
        prev->next = newNode;
        curr->unlock();
        prev->unlock();
        return true;
    }

    bool rmv(int key){
        // Starting at the head and traversing the list one node at a time by taking locks along the way
        head->lock();
        fineNode* prev = head;
        fineNode* curr = head->next;
        curr->lock();
        while(curr->value < key){
            prev->unlock();
            prev = curr;
            curr = prev->next;
            curr->lock();
        }
        // Give the pointer of next node to previous node and return true if the key is present
        if (curr->value == key){
            prev->next = curr->next;
            curr->unlock();
            prev->unlock();
            free(curr);
            return true;
        }
        // Return false if key is not found
        curr->unlock();
        prev->unlock();
        return false;
    }

    bool ctn(int key){
        // Starting at the head and traversing the list one node at a time by taking locks along the way
        head->lock();
        fineNode* prev = head;
        fineNode* curr = head->next;
        curr->lock();
        while(curr->value < key){
            prev->unlock();
            prev = curr;
            curr = prev->next;
            curr->lock();
        }
        // return true if the key is present
        if (curr->value == key){
            curr->unlock();
            prev->unlock();
            return true;
        }
        // Return false if key is not found
        curr->unlock();
        prev->unlock();
        return false;
    }

    void printState()
    {
        head->lock();
        fineNode* prev = head;
        fineNode* curr = head->next;
        curr->lock();
        std::cout << "The state of the fine set after successful operation is: ";
        while(curr->value < INT_MAX){
            std::cout << curr->value <<" ";
            prev->unlock();
            prev = curr;
            curr = prev->next;
            curr->lock();
        }
        std::cout << "." << std::endl;
        curr->unlock();
        prev->unlock();
    }
};