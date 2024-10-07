//  Copyright Necula Mihail 313CAa 2023-2024

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alloc.c"
#include "my_math.c"

#ifndef image
#define image

typedef struct image;

void free_image(struct image *v, int n);
struct image *calloc_image(int n);
struct image *duplicate_image(struct image *src);

int **load_ascii(int n, int m, FILE *f);
int **load_binary1(int n, int m, FILE *f);
int **load_binary2(int n, int m, FILE *f);
void i_want_to_load(struct image *img, char *token);

void select_area(struct image *img, int x1, int x2, int y1, int y2);
void i_want_to_select(struct image *img);

void crop(struct image *img);
void i_want_to_crop(struct image *img);

void print_histogram(struct image *img, int x, int y);
void i_want_the_histogram(struct image *img);

void equalize(struct image *img);
void i_want_to_equalize(struct image *img);

void build_sharpen_matrix(double a[3][3]);
void build_edge_matrix(double a[3][3]);
void build_blur_matrix(double a[3][3]);
void build_gaussian_blur_matrix(double a[3][3]);
void build_the_efect(double apply[3][3], char *effect);
void i_apply(struct image *img, double apply[3][3]);
void i_want_to_apply(struct image *img, char *token);

void rotate_plus_90(struct image *img);
void rotate(struct image *img, int angle);
void i_want_to_rotate(struct image *img, int angle);

void save_ascii(struct image *img, FILE *f);
void save_binary1(struct image *img, FILE *f);
void save_binary2(struct image *img, FILE *f);
void i_want_to_save(struct image *img, char *token, char type);

void finish(struct image *img);

void take_command(char command[201]);
int verify_command(char *command);
void do_command(char command[201], struct image *img);

#endif

