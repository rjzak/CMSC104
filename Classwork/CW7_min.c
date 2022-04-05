/**************************************
** File:    min.c
** Author:   <myName>
** Date:    <todaysDate>
** Section:  02-LEC (1474)
** E-mail:   <myEmailAddress>
**
** This file contains an example program for Classwork 7.
** The program reports the smallest integer entered by the user,
** and users can quit by typing anything that's not an integer,
** ideally the word 'quit' without the quotes around it.
**************************************/

#include <stdio.h>

// Include the following so we can use the INT_MAX constant
#include <limits.h>

int main() {
   // Variable declarations
   int min = INT_MAX;       /* smallest integer entered by the user; everything is <= INT_MAX */
   int num;                 /* user's number entry */
   int count = 0;           /* how many numbers user has entered */
   int stop = 0;            /* don't stop */
   int matched;             /* used like flag to see if user entered 'quit' */

   // Loop for user input checking
   while (!stop) {
      // Prompting user for a number to enter
      printf("Enter a number (type 'quit' to end): ") ;
      matched = scanf("%d", &num) ;

      /* If user typed 'quit', set stop to 1 so we can break out of loop;
      *  else increase count and check if this number is now smallest  
      */
      if (matched <= 0) {
         stop = 1 ;
      } else {
         count = count + 1 ;
         if (num < min) min = num;
      }
   } // end of user input checking loop

   /* If user entered at least one number, let user know how many numbers were entered and what was the lowest;
   *  else let user know no numbers were entered.
   */
   if ( count > 0 ) {
      printf("The smallest number out of %d numbers entered was: %d\n", count, min) ; 
   } else {
      printf("No numbers were entered.\n") ;
   }

   return 0 ;
}