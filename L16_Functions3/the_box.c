/**
 * File: the_box.c
 * Author: Richard Zak
 * Email: rzak1@umbc.edu
 * Date: 29 October 2021
 *
 * This file is a companion to Lecture 16: Functions, Part 3.
 * The user is asked for dimensions for a box, and the volume
 * and surface area are calculated. It demonstrates the use
 * of functions, loops, and input validation.
 */

#include<stdio.h>

int get_value(char* label);

int main() {
	int height, width, depth, volume;
	int surface1, surface2, surface3, surface_area;
	height = get_value("height");
	width = get_value("width");
	depth = get_value("depth");

	volume = height * width * depth;
	surface1 = height * width;
	surface2 = width * depth;
	surface3 = depth * height;
	surface_area = 2 * (surface1 + surface2 + surface3);
	
	printf("Height: %d, Width: %d, Depth: %d\n", height,width, depth);
	printf("Volume: %d\n", volume);
	printf("Surface area: %d\n", surface_area);
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
