#include <list>
#include <unordered_map>
#include <iostream>
using namespace std;

class LRUCache {
public:
    list<int> dll; 
    unordered_map<int, int> keyToVal;
    unordered_map<int, list<int>::iterator> keyToPos; 
    int capacity;

    LRUCache(int capacity) {
        this->capacity = capacity;
    }
    
    void access(int key) {
        dll.erase(keyToPos[key]);
        dll.push_front(key); 
        keyToPos[key] = dll.begin();
    }

    int get(int key) {
        if(keyToVal.find(key) != keyToVal.end()) {
            access(key); 
            return keyToVal[key]; 
        } else {
            return -1;
        } 
    }
    
    void put(int key, int value) {
        if(keyToVal.find(key) != keyToVal.end()) {
            access(key); 
            keyToVal[key] = value;
        } else {
            if(keyToVal.size() == capacity) {
                int lruKey = dll.back();
                dll.pop_back(); 
                keyToPos.erase(lruKey); 
                keyToVal.erase(lruKey);
            }

            dll.push_front(key); 
            keyToVal[key] = value;
            keyToPos[key] = dll.begin(); 
        }
    }
};

int main() {
    LRUCache* cache = new LRUCache(2);
    cache->put(1, 1);
    cache->put(2, 2);
    cout << cache->get(1) << endl; // returns 1
    cache->put(3, 3);
    cout << cache->get(2) << endl; // returns -1 (not found)
    cache->put(4, 4);
    cout << cache->get(1) << endl; // returns -1 (not found)
    cout << cache->get(3) << endl; // returns 3
    cout << cache->get(4) << endl; // returns 4
    delete cache;
    return 0;
}
