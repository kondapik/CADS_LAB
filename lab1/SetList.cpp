#include <iostream>
#include <stdlib.h>

using namespace std;

struct listElem {
    int value;
    listElem *next;
};

class SetList {
private:
    listElem *head;
public:
    //initializing head to NULL
    SetList() {   
        head = NULL;
    }

    bool add(int elem) {
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