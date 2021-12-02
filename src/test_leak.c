#include "stdio.h"
#include "stdlib.h"

typedef enum { ZERO, ONE, ONE_RING } Init;

void print(int* A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", A[i*n + j]);
        }
        printf("\n");
    }
}

void init(int* A, int n, Init init) {
    switch(init) {
        case ZERO:
            for (int i = 0; i < n*n; i++)
                A[i] = 0;
            break;

        case ONE:
            for (int i = 0; i < n*n; i++)
                A[i] = 1;
            break;

        case ONE_RING:
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (i == 0 || i == n-1 || j == 0 || j == n-1)
                        A[i*n + j] = 1;
                    else
                        A[i*n + j] = 0;
                }
            }
            break;
    }
}




/*
    =============================
    ===== MATRIX OPERATIONS =====
*/

int* add(int* A, int* B, int n) {
    int* C = malloc(sizeof(*C) * n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i*n + j] = A[i*n + j] + B[i*n + j];
        }
    }

    return C;
}




int* recurAdd(int* A, int* B, int n, int times) {
    // returns A + B*times
    if (times == 1)
        return add(A, B, n);

    int* AB = recurAdd(A, B, n, times-1);
    int* C = add(AB, B, n);
    free(AB);
    return C;
}


int main(int argc, char **argv) {
    int n = 5;
    int *A = malloc(sizeof(*A) * n * n);
    int *B = malloc(sizeof(*B) * n * n);

    init(A, n, ONE_RING);
    init(B, n, ONE_RING);

    int *C = add(A, B, n);

    print(C, n);

    free(A); free(B); free(C);
}