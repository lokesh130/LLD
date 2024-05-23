#include<bits/stdc++.h>
using namespace std; 

class LFUCache {
public:
    unordered_map<int, list<int>> keyToDll; 
    unordered_map<int, pair<int, int>> keyToValFreq; 
    unordered_map<int, list<int>::iterator> keyToPos; 
    int capacity; 
    int minFreq; 

    LFUCache(int capacity) {
        this->capacity = capacity;
        this->minFreq = 0;
    }
    
    void access(int key) {
        int currFreq = keyToValFreq[key].second;
        keyToDll[currFreq].erase(keyToPos[key]);
        int newFreq = currFreq + 1; 
        keyToDll[newFreq].push_front(key); 
        keyToValFreq[key] = {keyToValFreq[key].first, newFreq};
        keyToPos[key] = keyToDll[newFreq].begin();
        if(keyToDll[minFreq].empty()) {
            minFreq++;
        }
    }

    int get(int key) {
        if(keyToValFreq.find(key) != keyToValFreq.end()) {
            access(key); 
            return keyToValFreq[key].first;
        } else {
            return -1;
        }
    }
    
    void put(int key, int value) {
        if(keyToValFreq.find(key) != keyToValFreq.end()) {
            access(key); 
            keyToValFreq[key] = {value, keyToValFreq[key].second};
        } else {
            if(keyToValFreq.size() == capacity) {
                int lfuKey = keyToDll[minFreq].back();
                keyToDll[minFreq].pop_back();
                keyToValFreq.erase(lfuKey); 
                keyToPos.erase(lfuKey);
            }

            keyToDll[1].push_front(key); 
            keyToValFreq[key] = {value, 1};
            keyToPos[key] = keyToDll[1].begin();
            minFreq = 1;
        }
    }
};

int main() {
    LFUCache cache(2);

    cache.put(1, 1);
    cache.put(2, 2);
    cout << "Get 1: " << cache.get(1) << endl; // returns 1
    cache.put(3, 3); // evicts key 2
    cout << "Get 2: " << cache.get(2) << endl; // returns -1 (not found)
    cout << "Get 3: " << cache.get(3) << endl; // returns 3
    cache.put(4, 4); // evicts key 1
    cout << "Get 1: " << cache.get(1) << endl; // returns -1 (not found)
    cout << "Get 3: " << cache.get(3) << endl; // returns 3
    cout << "Get 4: " << cache.get(4) << endl; // returns 4

    return 0;
}