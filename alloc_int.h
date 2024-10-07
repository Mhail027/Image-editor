// Copyright Necula Mihail 313CAa 2023-2024

#include <stdio.h>
#include <stdlib.h>

#ifndef alloc_int
#define alloc_int

void free_matrix(int **a, int n);

int *malloc_vector(int n);
int *calloc_vector(int n);
int **malloc_matrix(int n, int m);

#endif

