//
// Created by Nicholas Miller on 2/4/18.
//

#include <iostream>
#include <string.h>
#include "pa1.h"

int main() {

    // Gets input from the user.
    std::cout << "Enter the size of a magic square: ";
    int n;
    std::cin >> n;
    std::cout << std::endl;

    // This determines if the input is valid. If the input is invalid the program terminates.
    if (n >= 3 && n <= 15 && n % 2 != 0) {

        // Initializes the MagicSquare structure.
        MagicSquare mS1;

        // This loops through all ten squares.
        for (int i = 1; i <= 10; i++) {

            // Since three only has 8 possible solutions a built in this statement
            // to break when the program reached three's last magic square.
            if (i >= 9 && n == 3) break;

            std::cout << "Magic Square #" << i << " is:\n\n";

            // Using a formulation of different methods I create ten different magic squares.
            switch (i) {
                case 1:
                    mS1 = squareOne(n);
                    break;
                case 2:
                    mS1 = transpose(n, squareOne(n));
                    break;
                case 3:
                    mS1 = rotate(n, squareOne(n));
                    break;
                case 4:
                    mS1 = transpose(n, rotate(n, squareOne(n)));
                    break;
                case 5:
                    mS1 = rotate(n, rotate(n, squareOne(n)));
                    break;
                case 6:
                    mS1 = transpose(n, rotate(n, rotate(n, squareOne(n))));
                    break;
                case 7:
                    mS1 = rotate(n, rotate(n, rotate(n, squareOne(n))));
                    break;
                case 8:
                    mS1 = transpose(n, rotate(n, rotate(n, rotate(n, squareOne(n)))));
                    break;
                case 9:
                    mS1 = squareTwo(n);
                    break;
                case 10:
                    mS1 = transpose(n, squareTwo(n));
                    break;
            }

            printMagicSquare(n, mS1);
            printSums(n, mS1);
            std::cout << "\n";
        }
    }
    return 0;
}

// This method creates a magic square based on a know algorithm.
MagicSquare squareOne(int n) {
    MagicSquare mS;

    // Sets all entries to zero
    memset(mS.arr, 0, sizeof(mS));

    // Starting position for 1
    int i = n / 2;
    int j = n - 1;

    for (int num = 1; num <= n * n;) {
        if (i == -1 && j == n) {
            i = 0;
            j = n - 2;
        } else {
            if (i < 0)
                i = n - 1;

            if (j == n)
                j = 0;
        }
        if (mS.arr[i][j]) {
            i++;
            j -= 2;
            continue;
        } else
            mS.arr[i][j] = num++;

        i--;
        j++;
    }

    return mS;
}

// This method creates a magic square based on a pattern I noticed.
MagicSquare squareTwo(int n) {

    MagicSquare mS;

    // Sets all entries to zero
    memset(mS.arr, 0, sizeof(mS));

    // Starting position for 1
    int i = (n / 2) + 1;
    int j = (n / 2);

    for (int num = 1; num <= n * n;) {
        if (i == n && j == n) {
            i = 1;
            j = n - 1;
        } else {
            if (i == n)
                i = 0;
            if (j == n)
                j = 0;
        }

        if (mS.arr[i][j]) {
            i++;
            j--;
            continue;
        } else
            mS.arr[i][j] = num++;

        i++;
        j++;
    }

    return mS;
}

// This method transposes the magic square.
MagicSquare transpose(int n, MagicSquare mS) {
    MagicSquare mS2 = mS;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Swaps rows and columns
            mS2.arr[j][i] = mS.arr[i][j];
        }
    }
    return mS2;
}

// This method rotates the matrix 90 degrees to the right.
MagicSquare rotate(int n, MagicSquare mS) {
    MagicSquare mS2 = mS;
    int x = 0;
    int y = 0;

    for (int j = 0; j < n; j++) {
        for (int i = n - 1; i >= 0; i--) {
            mS2.arr[x][y] = mS.arr[i][j];
            y++;
        }
        y = 0;
        x++;
    }
    return mS2;
}

// This method prints the sums of all rows, columns, and diagonals.
void printSums(int n, MagicSquare mS) {

    int M = n * (n * n + 1) / 2;
    int sum;

    // Checks all horizontal lines
    std::cout << "Checking sums of all rows:      ";
    for (int i = 0; i < n; i++) {
        sum = 0;
        for (int j = 0; j < n; j++) {
            sum += mS.arr[i][j];
        }
        std::cout << sum << " ";
    }

    std::cout << std::endl;

    // Checks all vertical lines
    std::cout << "Checking sums of all columns:   ";
    for (int j = 0; j < n; j++) {
        sum = 0;
        for (int i = 0; i < n; i++) {
            sum += mS.arr[i][j];
        }
        std::cout << sum << " ";
    }

    std::cout << std::endl;

    // Check diagonal one
    std::cout << "Checking sums of all diagonals: ";
    sum = 0;
    for (int i = 0, j = 0; i < n; i++, j++) {
        sum += mS.arr[i][j];
    }
    std::cout << sum << " ";

    //Check diagonal two
    sum = 0;
    for (int i = 0, j = n - 1; i < n; i++, j--) {
        sum += mS.arr[i][j];
    }
    std::cout << sum << " ";
    std::cout << std::endl;
}

// This method prints the magic square to the screen.
void printMagicSquare(int n, MagicSquare mS) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << mS.arr[i][j] << "\t";
            if (j == n - 1)
                std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}
