/*
Exercise 01 — Size inventory
Difficulty: EASY

Goal:
Build the byte-size table for this machine yourself, rather than trusting one.

Requirements:
- Print the size of: bool, char, short, int, unsigned int, long, long long,
  float, double, size_t, int*, char*, double*.
- Do not use std::vector or any container.

Questions before coding:
1. Will sizeof(int*) and sizeof(double*) be the same? Why?
2. Is any work done at run time to compute these values? no, it is always preprocessed before compilation
R: only python is made like that
3. Memory cost. For EACH of these cases, pick the smallest type that is still
   correct, then compute how much memory 100 million of them would occupy:
     (a) integers known to be between 0 and 200
     (b) integers that may reach 3 billion
     (c) real numbers needing about 6 significant digits
     (d) real numbers needing about 14 significant digits
   Then answer: how much memory does each choice cost, in MB? Your GPU has
   12 GB -- how many such arrays fit at once?
   Rule to derive from this: choose the smallest type that is still correct.
int, unsigned int, long long 

Why this matters for CUDA:
Every stride and offset a kernel computes is a multiple of one of these numbers.
*/

#include <iostream>

using namespace std;

int main() {
    
    int vidas;
    
    cout << "Digite o número de vidas: " << "\n";
    cin >> vidas ;
    cout << vidas << "\n";
    return 0;
}
