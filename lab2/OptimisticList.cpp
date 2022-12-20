#include <iostream>
#include <stdlib.h>
#include <climits>
#include <mutex>

class optNode {

public:
    int value;
    optNode* next;

private:
    pthread_mutex_t nodeLock;

public:
    optNode(int key){
        value = key;
        next = NULL;
    }

    // ~optNode() {pthread_mutex_unlock(&nodeLock);}

    void lock(){
        pthread_mutex_lock(&nodeLock);
    }

    void unlock(){
        pthread_mutex_unlock(&nodeLock);
    }
};


class OptimisticList {
private:
    optNode* head;
    
    bool validate(optNode* prev, optNode* curr){
        optNode* tmpNode = head;
        while (tmpNode->value <= prev->value){
            if (tmpNode == prev){
                return prev->next == curr;
            }
            tmpNode = tmpNode->next;
        }
        return false;
    }

public:
    OptimisticList() {   
        head = new optNode(INT_MIN);
        head->next = new optNode(INT_MAX);
    }

    bool add(int key) {
        // Starting with head traversing the list one node at a time by taking locks
        while(true){
            optNode* prev = head;
            optNode* curr = prev->next;
            while(curr->value < key){
                prev = curr;
                curr = prev->next;
            }

            // lock both nodes before validating them
            curr->lock();
            prev->lock();
            if (validate(prev, curr)){
                if (curr->value == key){
                    curr->unlock();
                    prev->unlock();
                    return false;
                }
                // Create new node and add it at the current location
                optNode* newNode = new optNode(key);
                newNode->next = curr;
                prev->next = newNode;
                curr->unlock();
                prev->unlock();
                return true;
            }
            curr->unlock();
            prev->unlock();
        }
    }

    bool rmv(int key){
        // Starting at the head and traversing the list one node at a time by taking locks along the way
        while(true){
            optNode* prev = head;
            optNode* curr = prev->next;
            while(curr->value < key){
                prev = curr;
                curr = prev->next;
            }
            // Give the pointer of next node to previous node and return true if the key is present
            // lock both nodes before validating them
            curr->lock();
            prev->lock();
            if (validate(prev, curr)){
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
            curr->unlock();
            prev->unlock();
        }
    }

    bool ctn(int key){
        // Starting at the head and traversing the list one node at a time by taking locks along the way
        while(true){
            optNode* prev = head;
            optNode* curr = head->next;
            while(curr->value < key){
                prev = curr;
                curr = prev->next;
            }
            // lock both nodes before validating them
            curr->lock();
            prev->lock();
            if (validate(prev, curr)){
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
            curr->unlock();
            prev->unlock();
        }
    }

    void printState()
    {
        head->lock();
        optNode* prev = head;
        optNode* curr = head->next;
        curr->lock();
        std::cout << "The state of the optimistic set after successful operation is: ";
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