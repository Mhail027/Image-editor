Copyright Necula Mihail 313CAa 2023-2024

We save the informations about the current image in a variable named img,
which is a pointer of type struct image. The struct image has the
following attributes:
	loaded - which is 1 if we have an image loaded or 0 if we do not have
	magic_word - in which we save the type of image 
	a - in which save the address to pixels's matrix
	n - which represents the number of pixels per column and also the number
	of lines of previous matrix
	m - which represents the number of pixels per line
	coefficient - in which we save the number of values from behind a pixel
	max - which represent the maximum value which can exist in pixels's matrix
	x1, x2, y1, y2 - in which we save the coordinates of area which we select
	from image; initially, the entire image is selected

After we receive a command, we have 2 cases:
	I. the command is valid,
	II. the command is not valid.
We determine the case in which we are using the function verify_command,from
the own library image.c. That function returns 1 if the command is valid and
0 if it is not. If it is not valid, we go to the next command after we print
on the screen the appropriate message.If it is valid, the things starts to
become interesting.

Using the function do_command, from the same library, we do the operation
corresponding to the valid command that we received, if it is possible for the
image with which we work. In any case, we'll print on the screen the command
situation. After we finish with the valid instruction, we go to the next
command.

The program ends when we receive the command EXIT.

