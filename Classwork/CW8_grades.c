/**************************************
** File:  grades.c
** Author: <myName>
** Date:  <todaysDate>
** Section: 02-LEC (1068)
** E-mail: <myEmailAddress>
**
** This file contains the main program for Classwork 8.
** The program randomly generates letter grades (ABCDF),
** counts how many of each there are,
** and prints the results to the screen.
**************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define TOTAL_GRADES 36

int main() {
   // Letter grade counter variables
   // TODO: Initialize each variable appropriately.
   int numA;
   int numB;
   int numC;
   int numD;
   int numF;

   // Variables used in grades loop
   int gradeCounter;
   int totalGrades = TOTAL_GRADES;

   // Variable used to simulate each letter grade
   char letterGrade;

   // Seed the random number generator
   srandom(time(0));

   // Counter-controlled loop to simulate class letter grades
   for (gradeCounter = 0; gradeCounter < totalGrades; gradeCounter++) {
      // Randomly generate a letter grade from ABCDF
      letterGrade = "ABCDF" [random() % 5];

      // TODO: Create a switch statement on letterGrade
      // to update num variables using += assignment operator.
      // NOTE: Don't forget the default case for invalid letterGrade.
   }

   // Print out the final results
   printf("Out of %d grades, here is the class grade breakdown:\n", totalGrades);
   printf("A: %d\n", numA);
   printf("B: %d\n", numB);
   printf("C: %d\n", numC);
   printf("D: %d\n", numD);
   printf("F: %d\n", numF);

   return 0;
}
