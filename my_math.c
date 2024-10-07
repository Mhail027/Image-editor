// Copyright Necula Mihail 313CAa 2023-2024

#define MIN -2147483648

//  Swap between them the values at wich point x and y.
void swap(int *x, int *y)
{
	int tmp = *x;
	*x = *y;
	*y = tmp;
}

//  Return the number a raised to the power x.
int power(int a, int x)
{
	int result = 1;
	for (int i = 1; i <= x; ++i)
		result *= a;
	return result;
}

// Return the nearest integer of the given double number.
int my_round(double x)
{
	if (x - (int)x <= 0.5)
		return (int)x;
	return (int)x + 1;
}

// Return the number of digits of x.
int number_of_digits(int x)
{
	int counter = 0;
	while (x) {
		++counter;
		x /= 10;
	}
	return x;
}

/*	Return the nummber from the interval [minimum, maximum]
	which is the closest to x. */
int clamp(int x, int minimum, int maximum)
{
	if (x < minimum)
		return minimum;
	if (x > maximum)
		return maximum;
	return x;
}

/*  Return 1 if the given string is a integer number.
	Return 0 if it is not. */
int string_is_int(char *string)
{
	size_t i = 0;
	if (string[0] == '-')
		i++;

	for (; i < strlen(string); ++i)
		if (string[i] < '0' || string[i] > '9')
			return 0;

	return 1;
}

//  Transform the string in a int variable and return it.
int convert_string_to_int(char *string)
{
	int sign = 1, number = 0;
	size_t i = 0;
	if (string[0] == '-') {
		sign = -1;
		i++;
	}

	for (; i < strlen(string); ++i)
		number = number * 10 + string[i] - '0';
	number *= sign;

	return number;
}

/*  Transform the given number from base 2 to base 10
	and return the result of coonversion. */
int convert_base_2_to_base_10(int x)
{
	int result = 0, digits = number_of_digits(x);
	for (int i = 0; i <= digits; ++i) {
		int digit = x % 10;
		x /= 10;
		result += digit * power(2, i);
	}
	return result;
}

// Return the maximum value from vector v, which has n elements.
int find_the_maximum(int *v, int n)
{
	int max = MIN;
	for (int i = 0; i < n; ++i)
		if (v[i] > max)
			max = v[i];
	return max;
}

/*  Copy the elements of src matrix, which has n lines and m columns,
	in a new matrix.
	After the copy is done, is returned the address of the new matrix. */
int **duplicate_matrix(int **src, int n, int m)
{
	int **new_matrix = malloc_matrix(n, m);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			new_matrix[i][j] = src[i][j];
	return new_matrix;
}

