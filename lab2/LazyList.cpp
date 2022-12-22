#include <iostream>
#include <stdlib.h>
#include <climits>
#include <mutex>

class lazyNode {

public:
    int value;
    lazyNode* next;
    bool mark;

// private:
    std::mutex nodeLock;

public:
    lazyNode(int key){
        value = key;
        next = NULL;
        mark = false;
    }

    // ~lazyNode() {pthread_mutex_unlock(&nodeLock);}
    ~lazyNode() {nodeLock.unlock();}
    // void lock(){
    //     pthread_mutex_lock(&nodeLock);
    // }

    // void unlock(){
    //     pthread_mutex_unlock(&nodeLock);
    // }
};


class LazyList {
private:
    lazyNode* head;
    
    bool validate(lazyNode* prev, lazyNode* curr){
        lazyNode* tmpNode = head;
        while (tmpNode->value <= prev->value){
            if (tmpNode == prev){
                //validate is modified to return true iff both nodes are not marked
                return !prev->mark && !curr->mark && prev->next == curr;
            }
            tmpNode = tmpNode->next;
        }
        return false;
    }

public:
    LazyList() {   
        head = new lazyNode(INT_MIN);
        head->next = new lazyNode(INT_MAX);
    }

    bool add(int key) {
        // Starting with head traversing the list one node at a time by taking locks
        while(true){
            lazyNode* prev = head;
            lazyNode* curr = prev->next;
            while(curr->value < key){
                prev = curr;
                curr = prev->next;
            }
            
            {// lock both nodes before validating them
                std::unique_lock<std::mutex>guardPrev(prev->nodeLock,std::defer_lock);
                std::unique_lock<std::mutex>guardCurr(curr->nodeLock,std::defer_lock);
                std::lock(guardCurr, guardPrev);

                if (validate(prev, curr)){
                    if (curr->value == key){
                        // guardPrev.unlock();
                        // guardCurr.unlock();
                        return false;
                    }
                    // Create new node and add it at the current location
                    lazyNode* newNode = new lazyNode(key);
                    newNode->next = curr;
                    prev->next = newNode;
                    // guardPrev.unlock();
                    // guardCurr.unlock();
                    return true;
                }
                // guardPrev.unlock();
                // guardCurr.unlock();
            }
        }
    }

    bool rmv(int key){
        // Starting at the head and traversing the list one node at a time by taking locks along the way
        while(true){
            lazyNode* prev = head;
            lazyNode* curr = prev->next;
            while(curr->value < key){
                prev = curr;
                curr = prev->next;
            }
            // Give the pointer of next node to previous node and return true if the key is present
            // lock both nodes before validating them
            {
                std::unique_lock<std::mutex>guardPrev(prev->nodeLock,std::defer_lock);
                std::unique_lock<std::mutex>guardCurr(curr->nodeLock,std::defer_lock);
                std::lock(guardCurr, guardPrev);

                if (validate(prev, curr)){
                    if (curr->value == key){
                        curr->mark = true;
                        prev->next = curr->next;
                        // guardPrev.unlock();
                        // guardCurr.unlock();
                        // free(curr);
                        return true;
                    }
                    // Return false if key is not found
                    // guardPrev.unlock();
                    // guardCurr.unlock();
                    return false;
                }
                // guardPrev.unlock();
                // guardCurr.unlock();
            }
        }
    }

    bool ctn(int key){
        // Starting at the head and traversing the list one node at a time
        lazyNode* curr = head;
        while(curr->value < key){
            curr = curr->next;
        }
        return (!curr->mark) && (curr->value == key);
    }

    void printState()
    {
        head->nodeLock.lock();
        lazyNode* prev = head;
        lazyNode* curr = head->next;
        curr->nodeLock.lock();
        std::cout << "The state of the lazy set after successful operation is: ";
        while(curr->value < INT_MAX){
            std::cout << ", value: "<< curr->value << " mark: " << std::boolalpha << curr->mark <<" ";
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