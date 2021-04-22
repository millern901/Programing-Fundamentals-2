//
// Created by Nicholas Miller on 2/4/18.
//

#ifndef PA1_PA1_H
#define PA1_PA1_H

const int N = 29;

// Creates a structure that holds a 2D array.
struct MagicSquare {
    int arr[N][N];
};

// This method creates a magic square based on a know algorithm.
MagicSquare squareOne(int n);

// This method creates a magic square based on a pattern I noticed.
MagicSquare squareTwo(int n);

// This method transposes the magic square.
MagicSquare transpose(int n, MagicSquare mS);

// This method rotates the matrix 90 degrees to the right.
MagicSquare rotate(int n, MagicSquare mS);

// This method prints the sums of all rows, columns, and diagonals.
void printSums(int n, MagicSquare mS);

// This method prints the magic square to the screen.
void printMagicSquare(int n, MagicSquare mS);

#endif //PA1_PA1_H
