#include<bits/stdc++.h> 
#include<thread> 

using namespace std; 

void cellMulti(vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<int>>& C, int row, int col) {
    C[row][col] = 0;
    int n = A[0].size();

    for(int i=0; i<n; i++) {
        C[row][col] += A[row][i]*B[i][col];
    }
}

void multiply(vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<int>>& C) { 
    int n1 = A.size(); 
    int n2 = B[0].size(); 
    vector<thread> threadArr; 

    for(int i=0; i<n1; i++) {
        for(int j=0; j<n2; j++) {
            // threadArr.emplace_back(cellMulti, ref(A), ref(B), ref(C), i, j);
            threadArr.emplace_back([&A, &B, &C, i, j]()-> void {cellMulti(A, B, C, i, j);});
        }
    }

    for(auto& thread : threadArr) {
        thread.join();
    }
}

void print(vector<vector<int>>& mat) { 
    int n1 = mat.size(); 
    int n2 = mat[0].size();

    for(int i=0; i<n1; i++) {
        for(int j=0; j<n2; j++) {
            cout<<mat[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

int main() { 
    vector<vector<int>> A = {{1, 2}, {3, 4}}; 
    vector<vector<int>> B = {{1, 4}, {8, 2}}; 
    vector<vector<int>> C; 
    C.assign(2, vector<int>(2, 0)); 

    multiply(A, B, C);
    print(C);
    return 0; 
}