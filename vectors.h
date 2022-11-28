#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;


int max(vector<double> A) {
    int m = 0;
    for (int i=0; i<A.size(); i++) {
        if (A[m]<A[i]) {m = i;}
    }
    return m;
}

double sigmoid(double x) {
    return 1.0/(1.0+exp(-x));
}

double activationFunction(double x) {
    return sigmoid(x);
    if (x>1) {return 1;}
    if (x>0) {return x;}
    return 0;
}
double derivedActivationFunction(double x) {
    return sigmoid(x)*(1-sigmoid(x));
    if (x>1) {return 0;}
    if (x>0) {return 1;}
    return 0;
}

void displayVector(vector<double> A) {
    for (int i=0; i<A.size(); i++) {
        printf("%.3f, ", A[i]);
    }
    printf("\n");
}

void displayVectorMatrix(vector<vector<double> > A) {
    for (int i = 0; i<A.size(); i++) {
        displayVector(A[i]);
    }
}

void activateVector(vector<double> A, vector<double> *B) {
    if (A.size()!= B->size()) {
        cout << "Wrong size!\n";
        return;
    }
    for (int i = 0; i<A.size(); i++) {
        B->at(i) = activationFunction(A[i]);
    }
}

void addVector(vector<double> A, vector<double> B, vector<double> *C) { //add A to B
    if (A.size()!= B.size() || A.size()!= C->size()) {
        cout << "Wrong size!\n";
        return;
    }
    for (int i = 0; i<A.size(); i++) {
        C->at(i) = A[i] + B[i];
    }
}

void addVectorMatrix(vector<vector<double> > A, vector<vector<double> > B, vector<vector<double> > *C) { //add A to B
    if (A.size()!= B.size() || A.size()!= C->size()) {
        cout << "Wrong size!\n";
        return;
    }
    if (A[0].size()!= B[0].size() || A[0].size()!= C->at(0).size()) {
        cout << "Wrong size!\n";
        return;
    }
    for (int i = 0; i<A.size(); i++) {
        addVector(A[i], B[i], &C->at(i));
    }
}



void multiplyVector(vector<double> A, vector<vector<double> > B, vector<double> *C) { //matrix multiply A by B to get C
    if (A.size() != B[0].size() || B.size() != C->size()) {
        cout << "Wrong size!\n";
        return;
    }
    for (int i=0; i<C->size(); i++) {
        C->at(i) = 0;
        for (int j=0; j<A.size(); j++) {
            C->at(i)+= A[j]*B[i][j];
        }
    }
}

void multiplyVector(double a, vector<double> A, vector<double> *B) {
    if (A.size() != B->size()) {
        cout << "Wrong size!\n";
        return;
    }
    for (int i=0; i<B->size(); i++) {
        B->at(i) = a * A[i];
    }
}

void multiplyVectorMatrix(double a, vector<vector<double> > A, vector<vector<double> > *B) {
    if (A.size() != B->size() || A[0].size() != B->at(0).size()) {
        cout << "Wrong size!\n";
        return;
    }
    for (int i=0; i<A.size(); i++) {
        multiplyVector(a, A[i], &B->at(i));
    }
}

void test(){
    vector<vector<double> > matrix_1(3, vector<double>(3)), matrix_2(3, vector<double>(3));

    matrix_1[0][0] = 3;
    matrix_1[1][2] = 5;
    matrix_1[2][1] = -2;

    matrix_2[0][0] = 7;
    matrix_2[1][2] = 5;
    matrix_2[2][1] = -8;
    matrix_2[2][2] = 15;

    multiplyVectorMatrix(-5, matrix_1, &matrix_1);
    displayVectorMatrix(matrix_1);

}
