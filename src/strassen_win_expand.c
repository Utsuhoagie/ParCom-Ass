#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>




#define A(i,j) A[i*n + j]
#define B(i,j) B[i*n + j]
#define C(i,j) C[i*n + j]
#define M(i,j) M[i*n + j]
#define N(i,j) N[i*n + j]


#define A1(i,j) A1[i*n + j]
#define A2(i,j) A2[i*n + j]
#define A3(i,j) A3[i*n + j]
#define A4(i,j) A4[i*n + j]
#define B1(i,j) B1[i*n + j]
#define B2(i,j) B2[i*n + j]
#define B3(i,j) B3[i*n + j]
#define B4(i,j) B4[i*n + j]

#define M1(i,j) M1[i*n + j]
#define M2(i,j) M2[i*n + j]
#define M3(i,j) M3[i*n + j]
#define M4(i,j) M4[i*n + j]
#define M5(i,j) M5[i*n + j]
#define M6(i,j) M6[i*n + j]
#define M7(i,j) M7[i*n + j]


typedef enum { ZERO, ONE_RING, ONE, CHECKER, INCR, DEC_RESET_INCR, RANDOM } Init;

// ----- Helpers ----------------

int closestHighPow(int x) {
    int pow = 2;
    while (pow < x)
        pow *= 2;

    return pow;
}

void printMat(double* M, int n) {
    int i,j,k;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%2.0lf ", M[i*n + j]);
        }
        printf("\n");
    }

    printf("\n");
}

void printArr(double* M, int n) {
    int i;

    for (i = 0; i < n*n; i++) {
        printf("%2.0lf ", M[i]);
    }

    printf("\n\n");
}


double* initMat(double* M, int n, Init init) {
    double data;
    int i,j,k;

    if (init == ZERO) {
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                M[i*n + j] = 0;
    }
    else if (init == INCR) {
        data = 0;

        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                M[i*n + j] = data;
                data++;
            }
        }
    }
    else if (init == DEC_RESET_INCR) {
        for (i = 0; i < n; i++) {
            data = 0.0;
            for (j = 0; j < n; j++) {
                data += 1.0 / (double)n;
                M[i*n + j] = data;
            }
        }
    }

    else if (init == ONE_RING) {
        for (i = 0; i < n; i++) {
            M[i] = 1;
            M[(n-1)*n + i] = 1;
        }
        for (i = 0; i < n; i++) {
            M[i*n] = 1;
            M[i*n + (n-1)] = 1;
        }
    }
    else if (init == ONE) {
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                M[i*n + j] = 1;
    }


    int expanded_size = closestHighPow(n);
    double* expanded_M = malloc(expanded_size * expanded_size * sizeof(double));

    for (i = 0; i < expanded_size; i++) {
        for (j = 0; j < expanded_size; j++) {
            if (i < n && j < n)
                expanded_M[i*expanded_size + j] = M[i*n + j];
            else 
                expanded_M[i*expanded_size + j] = 0;
        }
    }

    return expanded_M;
}

double* trim(double* M, int n, int expanded_n) {
    double* trimmed = malloc(n * n * sizeof(double));
    int i,j,k;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            trimmed[i*n + j] = M[i*expanded_n + j];
        }
    }

    return trimmed;
}

// ----- Operations ---------------

double* add(double* A, double* B, int n) {
    double* C = malloc(n * n * sizeof(double));
    int i,j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            C[i*n + j] = A[i*n + j] + B[i*n + j];

    return C;
}

double* sub(double* A, double* B, int n) {
    double* C = malloc(n * n * sizeof(double));
    int i,j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            C[i*n + j] = A[i*n + j] - B[i*n + j];

    return C;
}

// ONLY FOR SMALL MATRICES  ( maybe 4x4 )
double* mul(double* A, double* B, int n) {
    double* C = malloc(n * n * sizeof(double));
    int i,j,k;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            for (k = 0; k < n; k++)
                C[i*n + j] += A[i*n + k] * B[k*n + j];

    return C;
}

// ----- Matrix multiplication -------------------------------

// TODO: CHANGE n TO GENERAL NUMBERS
void split(double* M, double** pM11, double** pM12, double** pM21, double** pM22, int n) {
    int i, j, k;

    for (i = 0; i < n/2; i++)
        for (j = 0; j < n/2; j++)
            (*pM11)[i*(n/2) + j] = M[i*n + j];

    k = 0;
    for (i = 0; i < n/2; i++)
        for (j = n/2; j < n; j++) {
            (*pM12)[k] = M[i*n + j];
            k++;
        }

    k = 0;
    for (i = n/2; i < n; i++)
        for (j = 0; j < n/2; j++) {
            (*pM21)[k] = M[i*n + j];
            k++;
        }

    k = 0;
    for (i = n/2; i < n; i++)
        for (j = n/2; j < n; j++) {
            (*pM22)[k] = M[i*n + j];
            k++;
        }
}

double* merge(double* C11, double* C12, double* C21, double* C22, int n) {
    double* C = malloc(n * n * 4 * sizeof(double));
    int big_n = n*2;
    int i,j,k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            C[i*n + j] = 0;
        }
    }

    // printMat(C11, n);
    // printMat(C12, n);
    // printMat(C21, n);
    // printMat(C22, n);


    //printf("\tStarting C combine!!\n");
    bool isLeft;
    bool isUpper;
    for (i = 0; i < big_n; i++) {
        for (j = 0; j < big_n; j++) {
            isLeft = j < (big_n / 2);
            isUpper = i < (big_n / 2);

            if (isUpper) {  
                if (isLeft) {
                    C[i*big_n + j] = C11[i*n + j];
                }
                else {
                    C[i*big_n + j] = C12[i*n + j%n];
                }
            }
            else {
                if (isLeft) {
                    C[i*big_n + j] = C21[(i%n)*n + j];
                }
                else {
                    C[i*big_n + j] = C22[(i%n)*n + j%n];
                }
            }
        }
    }
    //printf("\tFinished C combine!!\n");

    return C;
}

double* strassen(double* A, double* B, int n) {
    
    double* C = malloc(n * n * sizeof(double));

    // Matrices after splitting into 4
    double *A11, *A12, *A21, *A22;
    double *B11, *B12, *B21, *B22;
    double *C11, *C12, *C21, *C22;

    A11 = malloc(n * n * sizeof(double) / 4);
    A12 = malloc(n * n * sizeof(double) / 4);
    A21 = malloc(n * n * sizeof(double) / 4);
    A22 = malloc(n * n * sizeof(double) / 4);

    B11 = malloc(n * n * sizeof(double) / 4);
    B12 = malloc(n * n * sizeof(double) / 4);
    B21 = malloc(n * n * sizeof(double) / 4);
    B22 = malloc(n * n * sizeof(double) / 4);

    C11 = malloc(n * n * sizeof(double) / 4);
    C12 = malloc(n * n * sizeof(double) / 4);
    C21 = malloc(n * n * sizeof(double) / 4);
    C22 = malloc(n * n * sizeof(double) / 4);

    // Indices
    int i,j,k;

    printf("--- A matrix ---\n");
    printMat(A, n);
    printf("--- B matrix ---\n");
    printMat(B, n);
    printf("--- C matrix ---\n");
    printMat(C, n);

    split(A, &A11, &A12, &A21, &A22, n);
    split(B, &B11, &B12, &B21, &B22, n);
    split(C, &C11, &C12, &C21, &C22, n);
    
    printf(" -- Finished partition! Starting M calc ------------\n\n");

    // M matrices for calculating
    double *M1, *M2, *M3, *M4, *M5, *M6, *M7;

    /* 
        M1 = (A11 + A22) * (B11 + B22)
        M2 = (A21 + A22) * B11
        M3 = A11 * (B12 - B22)
        M4 = A22 * (B21 - B11)
        M5 = (A11 + A12) * B22
        M6 = (A21 - A11) * (B11 + B12)
        M7 = (A12 - A22) * (B21 + B22)
    */
    double *M11, *M12, *M21, *M32, *M42, *M51, *M61, *M62, *M71, *M72;

    if (n > 4) {
        M1 = strassen(add(A11, A22, n/2), add(B11, B22, n/2), n/2);
        M2 = strassen(add(A21, A22, n/2), B11               , n/2);
        M3 = strassen(A11               , sub(B12, B22, n/2), n/2);
        M4 = strassen(A22               , sub(B21, B11, n/2), n/2);
        M5 = strassen(add(A11, A12, n/2), B22               , n/2);
        M6 = strassen(sub(A21, A11, n/2), add(B11, B12, n/2), n/2);
        M7 = strassen(sub(A12, A22, n/2), add(B21, B22, n/2), n/2);
    }
    else {      
        M1 = mul(add(A11, A22, n/2), add(B11, B22, n/2), n/2);
        M2 = mul(add(A21, A22, n/2), B11, n/2);
        M3 = mul(A11, sub(B12, B22, n/2), n/2);
        M4 = mul(A22, sub(B21, B11, n/2), n/2);
        M5 = mul(add(A11, A12, n/2), B22, n/2);
        M6 = mul(sub(A21, A11, n/2), add(B11, B12, n/2), n/2);
        M7 = mul(sub(A12, A22, n/2), add(B21, B22, n/2), n/2);
    }

    printf(" -- Finished M calc! Combining to form C matrix --------------\n\n");

    C11 = add(sub(add(M1, M4, n/2), M5, n/2), M7, n/2);
    C12 = add(M3, M5, n/2);
    C21 = add(M2, M4, n/2);
    C22 = add(add(sub(M1, M2, n/2), M3, n/2), M6, n/2);

    /*
        C11 = M1 + M4 - M5 + M7
        C12 = M3 + M5
        C21 = M2 + M4
        C22 = M1 - M2 + M3 + M6
    */
    
    C = merge(C11, C12, C21, C22, n/2);

    printf("--- Finished combining C! --------------\n\n");

    return C;
}



/*  =======================================================================
    ======================================================================= */

int main(int argc, char** argv) {

// ------------------------------------------
    // Declarations
    int n = 3, expanded_n = n;

    // Matrices
    double* A = malloc(n * n * sizeof(double));
    double* B = malloc(n * n * sizeof(double));
    double* C = malloc(n * n * sizeof(double));
    A = initMat(A, n, ONE_RING);
    B = initMat(B, n, ONE_RING);
    //C = initMat(C, n, ZERO);

    expanded_n = closestHighPow(n);

    // Transpose if needed
    // B = transpose(B, n);


// ---------------------------------------
// clock_t startTime, endTime, totalTime;
// startTime = clock();

        // --- Print only ----
        // printMat(A, n);
        // printMat(B, n);
        // printMat(C, n);

        // --- Other ops -----
        // C = add(A, B, n);
        // printMat(C, n);


        // --- Normal mul ----
        // C = mul(A, B, expanded_n);
        // C = trim(C, n, expanded_n);
        // printMat(C, n);


        // --- Strassen mul ---
        C = strassen(A, B, expanded_n);
        C = trim(C, n, expanded_n);

        printf("-------------------------------------\n");
        printf("--------- After Strassen: -----------\n");
        printMat(C, n);



// startTime = clock() - startTime;
// totalTime = endTime - startTime;
// printf("\nTotal time = %lf", (double)startTime / CLOCKS_PER_SEC);
// ----------------------------------------

    free(A); free(B); free(C);

}