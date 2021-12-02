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

    print(A, n);
}




/*
    =============================
    ===== MATRIX OPERATIONS =====
*/

int mul2(int* A) {
    return 0;
}




// int* matrix(int) {

// }




int main(int argc, char **argv) {
    int n = 5;
    int *A = malloc(sizeof(*A) * n * n);

    //init(A, n, ONE_RING);

    free(A);
}