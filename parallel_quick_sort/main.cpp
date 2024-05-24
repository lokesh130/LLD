/*
FR:
-> Use multithreading to do quick-sort parallely. 
*/



#include<bits/stdc++.h> 
#include<thread>
using namespace std; 


int findPivot(vector<int>& arr, int l, int r) {

    int& pivot = arr[r];
    int i = -1;

    for(int j=0; j<r; j++) {
        if(arr[j] <= pivot) {
            i++; 
            swap(arr[i], arr[j]);
        }
    }

    swap(pivot, arr[i+1]);

    return i+1;
}

void quickSort(vector<int>& arr, int l, int r) {
    if(l < r) {
        int pivotInd = findPivot(arr, l, r);
        thread leftThread([&]()->void {quickSort(arr, l, pivotInd-1);});
        thread rightThread([&]()->void {quickSort(arr, pivotInd+1, r);});
        leftThread.join(); 
        rightThread.join();
        // quickSort(arr, l, pivotInd-1);
        // quickSort(arr, pivotInd+1, r);
    }
}

void printArr(vector<int>& arr) {
    for(int& ele : arr) {
        cout<<ele<<" ";
    }
    cout<<endl;
}

int main() {
    vector<int> arr = {6, 3, 8, 10, 3, 67}; 
    quickSort(arr, 0, arr.size()-1); 
    printArr(arr);

    return 0;
}