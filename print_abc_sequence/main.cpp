/* 
FR 
-> Multithreading programming to print sequence ABCABCABC.....
-> 3 threads for printig each character. 
*/

#include<bits/stdc++.h> 
#include<condition_variable> 
#include<mutex> 
#include<thread> 
using namespace std; 

class SequencePrinter {
    private:
        condition_variable cv;
        mutex mtx;
        int turn = 0; 

    public:
        void printA() { 
            for(int i=0; i<3; i++) {
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [this]()->bool{return turn == 0;});
                cout<<"A";
                turn = 1;
                cv.notify_all(); 
            }

        }

        void printB() { 
            for(int i=0; i<3; i++) {
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [this]()->bool{return turn == 1;});
                cout<<"B";
                turn = 2;
                cv.notify_all(); 
            }
        }

        void printC() {
            for(int i=0; i<3; i++) {
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [this]()->bool{return turn == 2;});
                cout<<"C";
                cout<<endl;
                turn = 0;
                cv.notify_all(); 
            }
        }
};


int main() {
    SequencePrinter sequencePrinter; 
    
    thread aThread(&SequencePrinter::printA, &sequencePrinter); 
    thread bThread(&SequencePrinter::printB, &sequencePrinter); 
    thread cThread(&SequencePrinter::printC, &sequencePrinter); 

    aThread.join();
    bThread.join();
    cThread.join();

    return 0;
}