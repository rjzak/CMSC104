/**
 * File: the_rectangle.c
 * Author: Richard Zak
 * Email: rzak1@umbc.edu
 * Date: 29 October 2021
 *
 * This file is a companion to Lecture 16: Functions, Part 3.
 * The user is asked for dimensions for a rectangle, and
 * we draw it. It demonstrates the use
 * of functions, loops, and input validation.
 */

#include<stdio.h>

int get_value(char* label);
void draw_line(int width_size, int is_hollow);

int main() {
	int height, width, hCounter;
	height = get_value("height");
	width = get_value("width");
	printf("\n"); // an extra new line, for asthetics
	
	draw_line(width, 0);
	for(hCounter = 0; hCounter < height - 2; hCounter++) {
		draw_line(width, 1);
	}
	draw_line(width, 0);

	return 0;
}

/**
 * get_value() - get's the requested integer value from the user
 * Inputs: the string name of the value
 * Output: returns the integer from the user
 * Conditions: the return value will be greater than zero
 */
int get_value(char* label) {
	int value;
	printf("Enter the %s: ", label);
	scanf("%d", &value);
	while(value <= 0) {
		printf("The %s must be greater than zero!\n", label);
		printf("Enter the %s: ", label);
		scanf("%d", &value);
	}
	return value;
}

/**
 * draw_line() - draws a line with the given width, solid or hollow
 * Inputs: width_size is the width, is_hollow == 0 for solid
 * Outputs: None
 */
void draw_line(int width_size, int is_hollow) {
	int i;
	for(i = 0; i < width_size; i++) {
		if (i == 0 || i == width_size - 1 || is_hollow == 0) {
			printf("*");
		} else {
			printf(" ");
		}
		printf(" "); // to make it wider
	}
	printf("\n");
}
