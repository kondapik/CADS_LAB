#include <set>
#include <iterator>
#include <iostream>
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

class SetLib {
  public:
  bool add(int elem, bool out) {
    // ScopedMutex coarseLock(&modifyMutex);
    if (state.find(elem) == state.end()) {
      state.insert(elem);
      return out;
    } else {
      return !out;
    }
  }
	
  bool rmv(int elem, bool out) {
    // ScopedMutex coarseLock(&modifyMutex);
    if (state.find(elem) != state.end()) {
      state.erase(elem);
      return out;
    } else {
      return !out;
    }
  }
	
  bool ctn(int elem, bool out) {
    if (state.find(elem) != state.end()) {
      return out;
    } else {
      return !out;
    }
  }
	
  //Add your own member functions if needed
  void printState() {
    ScopedMutex coarseLock(&modifyMutex);
    set<int>::iterator stateIt ;
    if (state.size() != 0){
      cout << "The state of the set after successful operation is: ";
      for (stateIt = state.begin() ; stateIt != state.end() ; stateIt++ ) 
      {
          cout << *stateIt<<" ";
      }
      cout << endl;
    }else{
      cout << "The set is empty." << endl;
    }
  }

  private:
  set<int> state;
  pthread_mutex_t modifyMutex; 
};



