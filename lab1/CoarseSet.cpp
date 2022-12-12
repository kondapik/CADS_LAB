#include <iostream>
#include <stdlib.h>
#include <mutex>

using namespace std;
// Class to make add and remove function acquire lock on the whole state before any modifications
class ScopedMutex {
public:
    ScopedMutex(pthread_mutex_t *m) : theMutex_(m) {pthread_mutex_lock(theMutex_);}
    ~ScopedMutex() { pthread_mutex_unlock(theMutex_); }
private:
    pthread_mutex_t *theMutex_;
};


struct listElem {
    int value;
    listElem *next;
};

class CoarseSet {
private:
    listElem *head;
    pthread_mutex_t modifyMutex; 

public:
    //initializing head to NULL
    CoarseSet() {   
        head = NULL;
    }

    bool add(int elem) {
        ScopedMutex coarseLock(&modifyMutex);
        struct listElem *lastElement = head;
        if (head != NULL){
            while(lastElement->next != NULL) {
                if(lastElement->value==elem) {
                    return false;
                }
                lastElement = lastElement->next;
            }
        }
        struct listElem *newElement = new listElem;
        newElement->value = elem;
        newElement->next = NULL;
        if(head == NULL) {
            head = newElement;
        } else {
            lastElement->next = newElement;
        }
        return true;
    }

    bool rmv(int elem) {
        ScopedMutex coarseLock(&modifyMutex);
        struct listElem *currElem = head, *prev;

        if (head != NULL && head->value == elem) {
            head = head->next; 
            free(currElem); 
            return true;
        }else{
            while (currElem != NULL && currElem->value != elem) {
                prev = currElem;
                currElem = currElem->next;
            }

            if (currElem == NULL) return false;

            prev->next = currElem->next;
            free(currElem); 
            return true;
        }
    }

    bool ctn(int elem)
    {
        ScopedMutex coarseLock(&modifyMutex);
        struct listElem* currElem = head; 
        while (currElem != NULL) {
            if (currElem->value == elem) return true;
            currElem = currElem->next;
        }
        return false;
    }

    void printState()
    {
        if (head != NULL){
            cout << "The state of the set after successful operation is: ";
            struct listElem* currElem = head;
            while (currElem != NULL) {
                std::cout << currElem->value <<" ";
                currElem = currElem->next;
            }   
            std::cout << endl;
        }else{
            cout << "The set is empty." << endl;
        }
    }
};