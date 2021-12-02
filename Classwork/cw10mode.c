/*****************************************
** File:  mode.c
** Author: <myName>
** Date:  <todaysDate>
** Section: 02-LEC (10600)
** E-mail: <myEmailAddress>
**
** This file contains the main program for Classwork 10.
** The program asks the user for an array of numbers and
** tells them the average and mode.
*****************************************/

#include <stdio.h>

#define MAX_SCORE 20
#define MAX_SIZE 1000

int main() {
   int i = 0;       // used as index into array
   int n = 0;       // number of items in array
   int r;           // used to ensure user enters an integer
   float average;   // average of numbers entered by user
   float num;       // total number of items in array

   int A[MAX_SIZE]; // will hold the scores

   n = 0;
   i = 0;

   // read in the scores into array A[]
   while(1) {
      r = scanf("%d", &A[i] );

      // end of input?
      if ( r <= 0 ) {
          break ;
      }

      i = i + 1;

      // exceeded array size?
      if (i >= MAX_SIZE) {
         break;
      }
   }

   // Make n the number of items stored in A[]
   n = i;

   // compute the average

   int sum = 0;

   for (i = 0 ; i < n ; i++) {
      sum = sum + A[i];
   }

   num = n;               // num is float
   average = sum / num;

   printf("The average score is: %f\n", average);

   // *****
   // ***** TODO
   // *****

   // Declare a new count[] array.

   // Initialize each element of count[] to 0. (Use a for loop.)

   // Iterate through the elements of the A[] array and update count[] in
   // each iteration.

   // Iterate through the count[] array and print out the number of times
   // that each score appeared.

   // Iterate through the elements of the count[] array to find the maximum
   // count and the score with the maximum count.

   // Print out the score that has the highest count and the number of
   // times that score appeared.

   return 0;
}