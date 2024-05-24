#include<bits/stdc++.h> 
#include<thread> 
#include<shared_mutex> 
#include<mutex> 

using namespace std; 



class HashMap { 
    private:
        unordered_map<string, string> mp;
        shared_mutex mtx; 

    public: 
        HashMap() {

        }

        void put(string key, string value) {
            unique_lock<shared_mutex> lock(mtx);
            mp[key] = value; 
            cout<<"Successfully put "<<key<<endl;
        }

        string get(string key) {
            shared_lock<shared_mutex> lock(mtx);
            if(mp.find(key) == mp.end()) {
                cout<<"Key not found"<<endl;
                return "";
            } else {
                cout<<"Successfully returned value for key "<<key<<endl;
                return mp[key];
            }
        }
};



int main() { 
    HashMap hashMap; 

    thread putThread1(&HashMap::put, &hashMap, "key1", "value1");
    thread putThread2(&HashMap::put, &hashMap, "key2", "value2");
    thread putThread3(&HashMap::put, &hashMap, "key3", "value3");
    thread putThread4(&HashMap::put, &hashMap, "key4", "value4");

    thread readThread1(&HashMap::get, &hashMap, "key1");
    thread readThread2(&HashMap::get, &hashMap, "key2");
    thread readThread3(&HashMap::get, &hashMap, "key3");


    putThread1.join(); 
    putThread2.join(); 
    putThread3.join(); 
    putThread4.join(); 

    readThread1.join(); 
    readThread2.join(); 
    readThread3.join();
    return 0;
}