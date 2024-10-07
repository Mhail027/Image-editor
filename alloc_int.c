//  Copyright Necula Mihail 313CAa 2023-2024

//	Free the dynamically alocated space of matrix a, which has n lines.
void free_matrix(int **a, int n)
{
	for (int i = 0; i < n; ++i)
		free(a[i]);
	free(a);
}

//  Alocate dynamically space for a vector, which has n elements.
int *malloc_vector(int n)
{
	int *v = (int *)malloc(n * sizeof(int));
	if (!v) {
		fprintf(stderr, "malloc_vector() failed\n");
		return 0;
	}

	return v;
}

/*  Alocate dynamically space for a vector, which has n elements,
	and fill it with zeroes. */
int *calloc_vector(int n)
{
	int *v = (int *)calloc(n, sizeof(int));
	if (!v) {
		fprintf(stderr, "calloc_vector() failed\n");
		return 0;
	}

	return v;
}

//  Alocate dynamically space for a matrix, which has n lines and m columns.
int **malloc_matrix(int n, int m)
{
	int **a = (int **)malloc(n * sizeof(int *));
	if (!a) {
		fprintf(stderr, "malloc_matrix() failed\n");
		return NULL;
	}

	for (int i = 0; i < n; ++i) {
		a[i] = (int *)malloc(m * sizeof(int));
		if (!a) {
			fprintf(stderr, "malloc_matrix failed()\n");
			free_matrix(a, i);
			return NULL;
		}
	}

	return a;
}
