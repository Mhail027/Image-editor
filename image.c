//  Copyright Necula Mihail 313CAa 2023-2024

typedef struct image {
	int loaded;
	char magic_word[3];

	// a = the matrix of pixels
	int **a;

	/*	n = the number of lines of the matrix
		n = the number of pixels which the image has per a coloumn

		m = the number of pixels which the image has per a line

		coefficient = how many values are behind a pixel
			P1, P2, P4, P5 -> coefficient = 1
			P3, P6 -> coefficient = 3;

		m * coefficient = how many values has the matrix per a line
		m * coefficient = the number of columns of the matrix */
	int n, m, coefficient;

	// The pixels can have any value from the interval [0, max].
	int max;

	// x1, x2, y1, y2 represent the coordinates of selected area from image
	int x1, x2, y1, y2;
} image;

/*	Free the dynamically alocated space of vector v,
	which has n elemets of type struct image. */
void free_image(struct image *v, int n)
{
	for (int i = 0; i < n; ++i)
		if (v[i].loaded)
			free_matrix(v[i].a, v[i].n);
	free(v);
}

/*  Alocate dynamically space for a vector, which has n elements of type
	struct image. Every element has the atributes initialized with 0. */
struct image *calloc_image(int n)
{
	image *v = (struct image *)malloc(n * sizeof(struct image));
	if (!v) {
		fprintf(stderr, "calloc_image() failed");
		return NULL;
	}

	for (int i = 0; i < n; ++i) {
		v[i].loaded = 0;
		v[i].magic_word[0] = '\0';
		v[i].n = 0;
		v[i].m = 0;
		v[i].max = 0;
		v[i].a = NULL;
		v[i].x1 = 0;
		v[i].x2 = 0;
		v[i].y1 = 0;
		v[i].y2 = 0;
	}
	return v;
}

/*  Copy the elemets of src in a new variable.
	After the copy is done, is returned the address of the new variable. */
struct image *duplicate_image(struct image *src)
{
	struct image *dst = (struct image *)malloc(sizeof(struct image));
	*dst = *src;
	dst->a = duplicate_matrix(src->a, src->n, src->m * src->coefficient);
	return dst;
}

/*	Return the pixels's matrix of an ascii image (P1, P2 or P3).
	The matrix has n lines, m columns and is taken from fin. */
int **load_ascii(int n, int m, FILE *fin)
{
	// a = pixels's matrix
	int **a = malloc_matrix(n, m);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			fscanf(fin, "%d", &a[i][j]);
	return a;
}

/*	Return the pixels's matrix of a P4 image.
	The matrix has n lines, m columns and is taken from fin. */
int **load_binary1(int n, int m, FILE *fin)
{
	//  a = pixels's matrix
	int **a = malloc_matrix(n, m);
	/*  The coding style checker does not let us to use uint8_t,
		so we'll work with another type which has 8 bits: unsigned char. */
	unsigned char buffer;
	/*  i = current line
		j = current column */
	int i = 0, j = 0;

	/*  Reads the file byte by byte, using the variable buffer.
		Every bit of buffer is put in pixels's matrix */
	while (i != n - 1 && j != m - 1) {
		fread(&buffer, sizeof(unsigned char), 1, fin);
		for (int move = 7; move >= 0; move--) {
			// Check if we exited from the matrix.
			if (i == n - 1 && j == m - 1)
				break;
			/*  Put the bit from the pposition 8 - move of buffer
				in pixels's matrix */
			a[i][j] = (buffer >> move) & 1;
			// We move on the next position from the matrix.
			if (j + 1 == m - 1) {
				++i;
				j = 0;
			} else {
				++j;
			}
		}
	}

	return a;
}

/*	Return the pixels's matrix of a P5/P6 image.
	The matrix has n lines, m columns and is taken from fin. */
int **load_binary2(int n, int m, FILE *fin)
{
	// a = pixels's matrix
	int **a = malloc_matrix(n, m);
	/*  The coding style checker does not let us to use uint8_t,
		so we'll work with another type which has 8 bits: unsigned char. */
	unsigned char buffer;
	//  The '\n' character separte us of matrix.
	fread(&buffer,  sizeof(unsigned char), 1, fin);

	/*  Reads the file byte by byte, using the variable buffer.
		Every byte is put in pixels's matrix */
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) {
			fread(&buffer,  sizeof(unsigned char), 1, fin);
			a[i][j] = buffer;
		}

	return a;
}

/*  Load, in the structure given, the characteristics of the image
	which is saved in the file with the name of token.
	If this is not possible, the atribute loaded receive the value 0
	and the mission is abandoned. */
void i_want_to_load(struct image *img, char *token)
{
	FILE *fin = fopen(token, "r");

	if (fin) {
		if (img->loaded)
			free_matrix(img->a, img->n);

		fgets(img->magic_word, 3, fin);
		fscanf(fin, "%d%d%d", &img->m, &img->n, &img->max);
		if (img->magic_word[1] == '3' || img->magic_word[1] == '6')
			img->coefficient = 3;
		else
			img->coefficient = 1;

		switch (img->magic_word[1] - '0') {
		case 1:
		case 2:
		case 3:
			img->a = load_ascii(img->n, img->m * img->coefficient, fin);
			break;
		case 4:
			img->a = load_binary1(img->n, img->m, fin);
			break;
		case 5:
		case 6:
			img->a = load_binary2(img->n, img->m * img->coefficient, fin);
			break;
		}

		img->x1 = 0;
		img->y1 = 0;
		img->x2 = img->m;
		img->y2 = img->n;
	}

	/* Are 2 cases in which the image can not be loaded:
		1. the file with the name of token does not exist;
		2. we do not have enough memory to save the informations from file. */
	if (!fin || !img->a) {
		if (img->a)
			free_matrix(img->a, img->n);
		if (fin)
			fclose(fin);
		img->loaded = 0;
		printf("Failed to load %s\n", token);
	} else {
		fclose(fin);
		img->loaded = 1;
		printf("Loaded %s\n", token);
	}
}

/*  Change the coordinates of selected area from image
	which is saved in given struct. */
void select_area(struct image *img, int x1, int x2, int y1, int y2)
{
	img->x1 = x1;
	img->x2 = x2;
	img->y1 = y1;
	img->y2 = y2;
}

/*  Verify if the selection which we want to have it is possible.
	If yes, save the selection in the given struct.
	If no, does not make the change. */
void i_want_to_select(struct image *img)
{
	if (!img->loaded) {
		printf("No image loaded\n");
		return;
	}

	char *token = strtok(NULL, " ");
	if (strcmp(token, "ALL") == 0) {
		select_area(img, 0, img->m, 0, img->n);
		printf("Selected ALL\n");
	} else {
		int x1 = convert_string_to_int(token);
		token = strtok(NULL, " ");
		int y1 = convert_string_to_int(token);
		token = strtok(NULL, " ");
		int x2 = convert_string_to_int(token);
		token = strtok(NULL, " ");
		int y2 = convert_string_to_int(token);

		if (x1 > x2)
			swap(&x1, &x2);
		if (y1 > y2)
			swap(&y1, &y2);

		// The coordinates must be from the image and not from outside of it.
		if (x1 < 0 || x2 > img->m || y1 < 0 || y2 > img->n) {
			printf("Invalid set of coordinates\n");
			return;
		}
		/* We can not have a selected area which has the height or the width
			equals to 0. */
		if (x1 == x2 || y1 == y2) {
			printf("Invalid set of coordinates\n");
			return;
		}

		select_area(img, x1, x2, y1, y2);
		printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
	}
}

// Crop the image from the given struct as to remain just the selected area.
void crop(struct image *img)
{
	int new_n = img->y2 - img->y1;
	int new_m = img->x2 - img->x1;
	int **tmp = malloc_matrix(new_n, new_m * img->coefficient);
	int start_line = img->y1;
	int final_line = img->y2;
	int start_column = img->x1 * img->coefficient;
	int final_column = img->x2 * img->coefficient;
	for (int i = 0; start_line + i < final_line; ++i)
		for (int j = 0; start_column + j < final_column; ++j)
			tmp[i][j] = img->a[start_line + i][start_column + j];

	free_matrix(img->a, img->n);
	img->n = new_n;
	img->m = new_m;
	img->a = tmp;
	select_area(img, 0, img->m, 0, img->n);
}

/*  Verify if the crop operation can be done.
	If yes, make the operation.
	If no, abandon the mission. */
void i_want_to_crop(struct image *img)
{
	if (!img->loaded) {
		printf("No image loaded\n");
		return;
	}

	crop(img);
	printf("Image cropped\n");
}

/*  Print on the screen the histogram of the image loaded in given struct.
	The histogram has y bins, and every bin can have maximum x stars. */
void print_histogram(struct image *img, int x, int y)
{
	/*  Initially, we want:
		v[i] = the number of appearences of elements from the bin i + 1. */
	int *v =  calloc_vector(y);
	for (int i = 0; i < img->n; ++i)
		for (int j = 0; j < img->m * img->coefficient; ++j)
			v[img->a[i][j] / (256 / y)]++;

	//  Find the maximum frequency.
	int max_freq = find_the_maximum(v, y);

	/*  Now, we want:
		stars[i] = the number of stars for the bin i + 1. */
	int *stars = calloc_vector(y);
	for (int i = 0; i < y; ++i) {
		stars[i] = 1.0 * v[i] / max_freq * x;
		clamp(stars[i], 0, x);
	}

	//  Print the histogram
	for (int i = 0; i < y; ++i) {
		printf("%d\t|\t", stars[i]);
		for (int cont = 1; cont <= stars[i]; cont++)
			printf("*");
		printf("\n");
	}

	free(v);
	free(stars);
}

/*  Verify if the image from the given struct can have an histogram.
	If yes, make it ,with y bins and maximum x stars per bin, and print it.
	If no, abandon the mission. */
void i_want_the_histogram(struct image *img, int x, int y)
{
	if (!img->loaded) {
		printf("No image loaded\n");
		return;
	}

	if (img->magic_word[1] == '3' || img->magic_word[1] == '6') {
		printf("Black and white image needed\n");
		return;
	}

	print_histogram(img, x, y);
}

// Equalize the given image. The changes are done in place.
void equalize(struct image *img)
{
	/*  We want:
		v[i] = the number of appearences of the value i in pixels's matrix. */
	int *v = calloc_vector(256);
	for (int i = 0 ; i < img->n; ++i)
		for (int j = 0; j < img->m; ++j)
			v[img->a[i][j]]++;

	/*  We want:
		sum[i] = v[0] + v[1] + v[2] + ... + v[i]. */
	int *sum = calloc_vector(256);
	sum[0] = v[0];
	for (int i = 1; i < 256; ++i)
		sum[i] = sum[i - 1] + v[i];

	// Equalize the image.
	double area = img->n * img->m;
	for (int i = 0 ; i < img->n; ++i) {
		for (int j = 0; j < img->m; ++j) {
			img->a[i][j] = my_round(255 / area * sum[img->a[i][j]]);
			img->a[i][j] = clamp(img->a[i][j], 0, img->max);
		}
	}

	free(v);
	free(sum);
}

/*  Verify if the equalize operation can be done for the given image.
	If yes, makes the operation.
	If no, abandons the mission. */
void i_want_to_equalize(struct image *img)
{
	if (!img->loaded) {
		printf("No image loaded\n");
		return;
	}
	if (img->magic_word[1] == '3' || img->magic_word[1] == '6') {
		printf("Black and white image needed\n");
		return;
	}

	equalize(img);
	printf("Equalize done\n");
}

//  Build in a the matrix for the efect of sharpen.
void build_sharpen_matrix(double a[3][3])
{
	a[0][0] = 0;
	a[0][1] = -1;
	a[0][2] = 0;
	a[1][0] = -1;
	a[1][1] = 5;
	a[1][2] = -1;
	a[2][0] = 0;
	a[2][1] = -1;
	a[2][2] = 0;
}

//  Build in a the matrix for the efect of edge.
void build_edge_matrix(double a[3][3])
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (i == 1 && j == 1)
				a[i][j] = 8;
			else
				a[i][j] = -1;
}

//  Build in a the matrix for the efect of blur.
void build_blur_matrix(double a[3][3])
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			a[i][j] = 1.0 / 9;
}

//  Build in a the matrix for the efect of gaussian blur.
void build_gaussian_blur_matrix(double a[3][3])
{
	a[0][0] = 1.0 / 16;
	a[0][1] = 2.0 / 16;
	a[0][2] = 1.0 / 16;
	a[1][0] = 2.0 / 16;
	a[1][1] = 4.0 / 16;
	a[1][2] = 2.0 / 16;
	a[2][0] = 1.0 / 16;
	a[2][1] = 2.0 / 16;
	a[2][2] = 1.0 / 16;
}

// Build in a the matrix for the given effect.
void build_the_efect(double apply[3][3], char *effect)
{
	if (strcmp(effect, "SHARPEN") == 0)
		build_sharpen_matrix(apply);
	if (strcmp(effect, "EDGE") == 0)
		build_edge_matrix(apply);
	if (strcmp(effect, "BLUR") == 0)
		build_blur_matrix(apply);
	if (strcmp(effect, "GAUSSIAN_BLUR") == 0)
		build_gaussian_blur_matrix(apply);
}

/*  Apply on the selected area of the image from the given struct the effect
	coresponding to the matrix apply. */
void i_apply(struct image *img, double apply[3][3])
{
	int **new_a = malloc_matrix(img->n, img->m * img->coefficient);
	int coefficient = img->coefficient;
	for (int i = 0; i < img->n; ++i)
		for (int j = 0; j < img->m * coefficient; ++j) {
			/*  Firstly, we copy the current value in the new matrix
				without to modify it. */
			new_a[i][j] = img->a[i][j];

			/*  If all 4 conditions from above are met, we must apply the
				selected efect on the current value */
			//  condition 1 = We are not on the first or last line
			int c1 = 0 < i && i < img->n - 1;
			//  condition 2 = We are not on the first or last column
			int c2 = coefficient - 1 < j && j < (img->m - 1) * coefficient;
			//  condition 3 = We are on a line which is in the selected area
			int c3 = img->y1 <= i && i < img->y2;
			//  condition 4 = We are on a column which is in the selected area
			int c4 = img->x1 * coefficient <= j && j < img->x2 * coefficient;

			if (c1 && c2 && c3 && c4) {
				double val = 0;
				for (int x = -1; x < 2; ++x)
					for (int y = -1; y < 2; y += 1)
						val += img->a[i + x][j + y * 3] * apply[x + 1][y + 1];
				new_a[i][j] = my_round(val);
				new_a[i][j] = clamp(new_a[i][j], 0, img->max);
			}
		}

	free_matrix(img->a, img->n);
	img->a = new_a;
}

/*	Verify if the apply operation with the given efect
	can be done for the image received.
	If yes, make the operation.
	If not, abandon the mission. */
void i_want_to_apply(struct image *img, char *effect)
{
	if (!img->loaded) {
		printf("No image loaded\n");
		return;
	}

	if (img->magic_word[1] == '2' || img->magic_word[1] == '5') {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	double apply[3][3];
	// If apply[1][1] remains 0, the parameter is not valid.
	apply[1][1] = 0;
	build_the_efect(apply, effect);
	if (apply[1][1] == 0) {
		printf("APPLY parameter invalid\n");
		return;
	}

	i_apply(img, apply);
	printf("APPLY %s done\n", effect);
}

// Rotates plus 90 degrees the image from the given structure.
void rotate_plus_90(struct image *img)
{
		swap(&img->n, &img->m);
		int **new_a = malloc_matrix(img->n, img->m * img->coefficient);
		int coef = img->coefficient;

		/* Firstly, we want the transpose,
			taking in consideration that a pixel represent an element.
			P1, P2, P4, P5 -> 1 element = 1 value
			P3, P6 -> 1 element = 3 values */
		for (int i = 0; i < img->n; ++i) {
			for (int j = 0; j < img->m; ++j) {
				for (int  k = 0; k < coef; ++k)
					new_a[i][j * coef + k] = img->a[j][i * coef + k];
			}
		}

		/*  After, we want to reverse the elements of every line,
			taking in consideration that a pixel represent an element.
			P1, P2, P4, P5 -> 1 element = 1 value
			P3, P6 -> 1 element = 3 values */
		for (int i = 0; i < img->n; ++i) {
			// In v we'll save temporarily the reversed line i.
			int *v =  malloc_vector(img->m * img->coefficient);
			for (int j = 0; j < img->m; ++j)
				for (int k = 0; k < coef; ++k)
					v[j * coef + k] = new_a[i][(img->m - j - 1) * coef + k];
			free(new_a[i]);
			new_a[i] = v;
		}

		free_matrix(img->a, img->m);
		img->a = new_a;
		swap(&img->x2, &img->y2);
}

/*  Rotates with that angle the selected area of the image
	from the given structure. */
void rotate(struct image *img, int angle)
{
	while (angle) {
		if (img->x2 - img->x1 == img->m && img->y2 - img->y1 == img->n) {
			rotate_plus_90(img);
		} else {
			struct image *tmp = duplicate_image(img);
			crop(tmp);
			rotate_plus_90(tmp);

			int coef = img->coefficient;
			for (int i = img->y1; i < img->y2; ++i)
				for (int j = img->x1 * coef; j < img->x2 * coef; ++j)
					img->a[i][j] = tmp->a[i - img->y1][j - img->x1 * coef];

			free_image(tmp, 1);
		}

		angle -= 90;
	}
}

/*  Verify if the rotate operation with that angle is possible
	for the given image.
	If yes, make the operation.
	If no, abandon the mission. */
void i_want_to_rotate(struct image *img, int angle)
{
	if (!img->loaded) {
		printf("No image loaded\n");
		return;
	}

	if (img->x2 - img->x1 != img->m && img->y2 - img->y1 != img->n)
		if (img->x2 - img->x1 != img->y2 - img->y1) {
			printf("The selection must be square\n");
			return;
		}

	int angle_dup = angle;
	if (angle <  0)
		angle += 360;
	if (angle < 0 || angle > 360 || angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}

	rotate(img, angle);
	printf("Rotated %d\n", angle_dup);
}

/*  Save, in the file fout, an image.
	The informations about the image are taken from the given struct.
	The image is saved using the ascii format(P1, P2 or P3). */
void save_ascii(struct image *img, FILE *fout)
{
	fprintf(fout, "%s\n%d %d\n%d\n", img->magic_word, img->m, img->n, img->max);

	for (int i = 0; i < img->n; ++i) {
		for (int j = 0; j < img->m * img->coefficient - 1; ++j)
			fprintf(fout, "%d ", img->a[i][j]);
		fprintf(fout, "%d\n", img->a[i][img->m * img->coefficient - 1]);
	}
}

/*  Save, in the file fout, an image.
	The informations about the image are taken from the given struct.
	The image is saved using the P4 format */
void save_binary1(struct image *img, FILE *fout)
{
	fprintf(fout, "%s\n%d %d\n%d\n", img->magic_word, img->m, img->n, img->max);

	int counter = 0, val = 0;
	for (int i = 0; i < img->n; ++i) {
		for (int j = 0; j < img->m * img->coefficient; ++j) {
			if (counter == 8) {
				val = convert_base_2_to_base_10(val);
				fprintf(fout, "%c", val);
				counter = 0;
				val = 0;
			} else {
				val = val * 10 + img->a[i][j];
				++counter;
			}
		}
	}

	if (counter != 0) {
		while (counter == 8) {
			val = val * 10;
			++counter;
		}
		val = convert_base_2_to_base_10(val);
		fprintf(fout, "%c", val);
	}
}

/*  Save, in the file fout, an image.
	The informations about the image are taken from the given struct.
	The image is saved using the P5/P6 format. */
void save_binary2(struct image *img, FILE *fout)
{
	fprintf(fout, "%s\n%d %d\n%d\n", img->magic_word, img->m, img->n, img->max);

	for (int i = 0; i < img->n; ++i)
		for (int j = 0; j < img->m * img->coefficient ; ++j)
			fprintf(fout, "%c", img->a[i][j]);
}

/*  If an image is loded, is saved in a file with the name of given string,
	taking in the consideration the format in which we want to save
	(ascii or bynary).
	format == 'a' -> ascii
	format == 'b' -> binary */
void i_want_to_save(struct image *img, char *filename, char format)
{
	if (!img->loaded) {
		printf("No image loaded\n");
		return;
	}

	FILE *fout = fopen(filename, "w");
	if (format == 'b') {
		if (img->magic_word[1] <= '3')
			img->magic_word[1] = img->magic_word[1] + 3;

		if (img->magic_word[1] != '1' && img->magic_word[1] != '4')
			save_binary2(img, fout);
		else
			save_binary1(img, fout);
	} else {
		if (img->magic_word[1] > '3')
			img->magic_word[1] = img->magic_word[1] - 3;

		save_ascii(img, fout);
	}

	fclose(fout);
	printf("Saved %s\n", filename);
}

// Realize the EXIT command.
void finish(struct image *img)
{
	if (!img->loaded)
		printf("No image loaded\n");
	free_image(img, 1);
}

// Read the current command from stdin and save it in the given parameter.
void take_command(char command[201])
{
	fgets(command, 201, stdin);

	if (command[strlen(command) - 1] == '\n')
		command[strlen(command) - 1] = '\0';
	while (strlen(command) > 0 && command[strlen(command) - 1] == ' ')
		command[strlen(command) - 1] = '\0';
}

/*  Return 0 if the command received is not valid.
	Otherwise, the value returned is 1. */
int verify_command(char *command)
{
	if (!strcmp(command, "SELECT ALL"))
		return 1;
	if (!strcmp(command, "EQUALIZE"))
		return 1;
	if (!strcmp(command, "CROP"))
		return 1;
	if (!strcmp(command, "APPLY"))
		return 0;

	char command_dup[201];
	strcpy(command_dup, command);
	char *token = strtok(command_dup, " ");

	if (!strcmp(token, "APPLY"))
		return 1;
	if (!strcmp(token, "LOAD")) {
		token = strtok(NULL, " ");
		if (!token)
			return 0;
		token = strtok(NULL, " ");
		if (token)
			return 0;
		return 1;
	}
	if (!strcmp(token, "SELECT")) {
		token = strtok(NULL, " ");
		if (!token || !string_is_int(token))
			return 0;
		token = strtok(NULL, " ");
		if (!token || !string_is_int(token))
			return 0;
		token = strtok(NULL, " ");
		if (!token || !string_is_int(token))
			return 0;
		token = strtok(NULL, " ");
		if (!token || !string_is_int(token))
			return 0;
		token = strtok(NULL, " ");
		if (token)
			return 0;
		return 1;
	}
	if (!strcmp(token, "HISTOGRAM")) {
		token = strtok(NULL, " ");
		if (!token || !string_is_int(token))
			return 0;
		token = strtok(NULL, " ");
		if (!token || !string_is_int(token))
			return 0;
		token = strtok(NULL, " ");
		if (token)
			return 0;
		return 1;
	}
	if (!strcmp(token, "ROTATE")) {
		token = strtok(NULL, " ");
		if (!token || !string_is_int(token))
			return 0;
		token = strtok(NULL, " ");
		if (token)
			return 0;
		return 1;
	}
	if (!strcmp(token, "SAVE")) {
		token = strtok(NULL, " ");
		if (!token)
			return 0;
		token = strtok(NULL, " ");
		if (!token)
			return 1;
		if (strcmp(token, "ascii"))
			return 0;
		token = strtok(NULL, " ");
		if (token)
			return 0;
		return 1;
	}
	return 0;
}

/*  Realize the given command if it is possible.
	If can not be done, print on the screen the met problem.
	Img is a pointer to the current/loaded image.*/
void do_command(char command[201], struct image *img)
{
	char *token = strtok(command, " ");

	/*  It is easier to compare 2 characters than 2 strings.
		So, we'll ascociate every command with a letter.
		The third letter is first to be different in all the possible commands.
		So, we will do the asociation with that letter.*/
	switch (token[2]) {
	// LOAD - A
	case 'A':
		token = strtok(NULL, " ");
		i_want_to_load(img, token);
		break;
	// SELECT - L
	case 'L':
		i_want_to_select(img);
		break;
	// CROP - O
	case 'O':
		i_want_to_crop(img);
		break;
	// APPLY - P
	case 'P':
		token = strtok(NULL, " ");
		i_want_to_apply(img, token);
		break;
	// EQUALIZE - U
	case 'U':
		i_want_to_equalize(img);
		break;
	// HISTOGRAM - S
	case 'S':
		token = strtok(NULL, " ");
		int x = convert_string_to_int(token);
		token = strtok(NULL, " ");
		int y = convert_string_to_int(token);
		i_want_the_histogram(img, x, y);
		break;
	// ROTATE - T
	case 'T':
		token = strtok(NULL, " ");
		int angle = convert_string_to_int(token);
		i_want_to_rotate(img, angle);
		break;
	// SAVE - V
	case 'V':
		token = strtok(NULL, " ");
		char type;
		if (token[strlen(token) + 1] == '\0')
			type = 'b';
		else
			type = 'a';
		i_want_to_save(img, token, type);
		break;
	}
}
