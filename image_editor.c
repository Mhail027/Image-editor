// Copyright Necula Mihail 313CAa 2023-2024

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alloc_int.c"
#include "my_math.c"
#include "image.c"

int main(void)
{
	struct image *img = calloc_image(1);
	char command[201];
	take_command(command);

	while (strcmp(command, "EXIT")) {
		if (verify_command(command)) {
			do_command(command, img);
		} else {
			if (!img->loaded)
				printf("No image loaded\n");
			else
				printf("Invalid command\n");
		}

		take_command(command);
	}

	finish(img);
	return 0;
}

