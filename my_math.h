// Copyright Necula Mihail 313CAa 2023-2024

#include <stdio.h>
#include <string.h>

#ifndef my_math
#define my_math

void swap(int *x, int *y);
int power(int a, int x);
int my_round(double x);
int number_of_digits(int x);
int clamp(int x, int minimum, int maximum);

int string_is_int(char *string);
int convert_string_to_int(char *string);
int convert_base_2_to_base_10(int x);

int find_the_maximum(int *v, int n);
int **duplicate_matrix(int **src, int n, int m);

#endif

