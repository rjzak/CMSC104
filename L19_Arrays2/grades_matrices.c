/**
 * File: grades_matrices.c
 * Author: Richard Zak
 * Email: rzak1@umbc.edu
 * Date: 23 November 2021
 *
 * This file is a companion to Lecture 19: Arrays, Part 2.
 * Grade values are collected from the user, and stats are calculated.
 * Shows a use for two dimensional arrays, structs, #defines, constant arrays.
 */


#include<stdio.h>
#include<stdbool.h>

#define CLASS_SIZE 39
#define GRADES 5
#define PLUS_MINUS_SIZE 3
#define A 4
#define B 3
#define C 2
#define D 1
#define F 0
#define PLUS 0
#define REGULAR 1
#define MINUS 2
#define MAX 100

const char GRADE_LETTERS[GRADES] = {'F', 'D', 'C', 'B', 'A'};
const char PLUS_MINUS[PLUS_MINUS_SIZE] = {'+', ' ', '-'};

typedef struct {
    float mean; /* average of all scores */
    unsigned int median; /* when sorted, which grade is in the middle? */
    unsigned int max;    /* max score */
    unsigned int min;    /* minimum score */
} ClassStatistics;

void PrintInstructions();
void GetScores(unsigned int scores[], const unsigned int size);
ClassStatistics CalcStats(unsigned int scores[], const unsigned int size, unsigned int gradesCount[GRADES][PLUS_MINUS_SIZE]);
void swap(unsigned int *p, unsigned int *q);
void sort(unsigned int scores[], const unsigned int size);
void PrintStats(const ClassStatistics stats, const unsigned int gradesCount[GRADES][PLUS_MINUS_SIZE]);

int main() {
    /* Grades are never negative, so we'll use unsigned integers */
    unsigned int scores[CLASS_SIZE] = {0};
    unsigned int gradesCount[GRADES][PLUS_MINUS_SIZE] = {0};

    PrintInstructions();
    GetScores(scores, CLASS_SIZE);
    ClassStatistics stats = CalcStats(scores, CLASS_SIZE, gradesCount);
    PrintStats(stats, gradesCount);

    return 0;
}

/*********************************************************
** PrintInstructions - prints the user instructions
** Inputs:  None
** Outputs:  None
** Notes: CLASS_SIZE must be #defined in this file
**********************************************************/
void PrintInstructions() {
    printf("This program calculates the average score\n");
    printf("for a class of %d students. It also reports the\n", CLASS_SIZE);
    printf("number of As, B's, C's, D's, and F's. You will\n");
    printf("be asked to enter the individual scores.\n");
}

/******************************************************************
** GetScores - fills an unsigned integer array with valid values that
**                are entered by the user.  
**                Assures the values are between zero and MAX.
** Inputs:  scores - array to fill
** Outputs: size - size of the array
** Notes: MAX must be #defined in this file
********************************************************************/
void GetScores(unsigned int scores[], const unsigned int size) {
    int i;
    for(i = 0; i < size; i++) {
        printf("Enter next value: ");
        scanf("%u", &scores[i]); // %u for unsigned ints!
        while( scores[i] > MAX) {
            printf("Values must not be greater than %d.\n", MAX);
            printf("Enter next value: ");
            scanf("%u", &scores[i]);
        }
    }
}

/******************************************************************
** CalcStats - Returns statistical information about the grades
** Inputs:  scores array, size of scores array, and grades matrix
** Outputs: statistics struct
** Notes: Letter grades, GRADES, PLUS_MINUS_SIZE, MINUS, REGULAR, PLUS
**        must be #defined in this file
********************************************************************/
ClassStatistics CalcStats(unsigned int scores[], const unsigned int size, unsigned int gradesCount[GRADES][PLUS_MINUS_SIZE]) {
    unsigned int i, total = 0;
    ClassStatistics stats;
    for(i = 0; i < size; i++) {
        total += scores[i];
        if (i == 0) { // We can assume the first element in the array is the largest and smallest
            stats.max = scores[i];
            stats.min = scores[i];
        } else {
            if (stats.max < scores[i]) {
                stats.max = scores[i];
            }
            if (stats.min > scores[i]) {
                stats.min = scores[i];
            }
        }
        switch(scores[i]/10) {
            case 10: gradesCount[A][PLUS]++; break;
            
            /* There has to be a better way to handle plus/minus, and have
               a common way to have the cut-off values specified for all */
            case 9:
                if (scores[i] >= 97) {
                    gradesCount[A][PLUS]++;
                } else if (scores[i] > 92) {
                    gradesCount[A][REGULAR]++;
                } else {
                    gradesCount[A][MINUS]++;
                }
                break;
            case 8:
                if (scores[i] >= 87) {
                    gradesCount[B][PLUS]++;
                } else if (scores[i] > 82) {
                    gradesCount[B][REGULAR]++;
                } else {
                    gradesCount[B][MINUS]++;
                }
                break;
            case 7:
                if (scores[i] > 77) {
                    gradesCount[C][PLUS]++;
                } else if (scores[i] > 72) {
                    gradesCount[C][REGULAR]++;
                } else {
                    gradesCount[C][MINUS]++;
                }
                break;
            case 6:
                if (scores[i] > 67) {
                    gradesCount[D][PLUS]++;
                } else if (scores[i] > 62) {
                    gradesCount[D][REGULAR]++;
                } else {
                    gradesCount[D][MINUS]++;
                }
                break;
            
            /* F is a special case, no plus or minus */
            case 5: case 4: case 3: case 2:
            case 1: case 0: gradesCount[F][REGULAR]++; break;
            default: printf("Invalid score %d found at index %d.\n", scores[i], i);
        } // end switch
    } // end for
    stats.mean = (float) total / (float) size;
    
    sort(scores, size);
    stats.median = scores[(size+1) / 2 -1];
    
    return stats;
}

/*********************************************************
** swap: swaps two locations in an unsigned integer array
** Inputs: p & q are pointers to specific positions in the array
** Outputs: None
** Note: modifies the values at the represented locations
*********************************************************/
void swap(unsigned int *p, unsigned int *q) {
   unsigned int temp;
   temp = *p;
   *p = *q;
   *q = temp;
}

/*********************************************************
** sort: sorts an array from low to high
** Inputs: unsigned integer array, size
** Outputs: None
** Note: modifies the contents of the array
**********************************************************/
void sort(unsigned int scores[], const unsigned int size) { 
   unsigned int i, j;

   for(i = 0; i < size-1; i++) {
      for(j = 0; j < size-i-1; j++) {
         if (scores[j] > scores[j+1])
            swap(&scores[j], &scores[j+1]);
      }
   }
}

/******************************************************************
** PrintResults - prints the class average and grade distribution
** Inputs:  stats is the struct with several measured data points
**          gradesCount matrix which has the totalled grades with
**                      plus, minus, and regular values
** Outputs:  None
** Notes:: A, B, C, D, F must be #defined in this file
*******************************************************************/
void PrintStats(const ClassStatistics stats, const unsigned int gradesCount[GRADES][PLUS_MINUS_SIZE]) {
    unsigned int i, j;
    bool first = true;
    printf("The class average is %.2f, median is %d\n", stats.mean, stats.median);
    printf("Max: %d, Min: %d\n", stats.max, stats.min);

    for(i = GRADES-1; i >= 0; i--) {
        if (i == F) { /* F is a special case, no plus or minus */
            printf("           %2d %c\n", gradesCount[i][REGULAR], GRADE_LETTERS[i]);
            break;
        }
        for(j = 0; j < PLUS_MINUS_SIZE; j++) {
            if (first) { // Easy way to have cleaner output for the user.
                printf("There were ");
                first = false;
            } else {
                printf("           ");
            }
            printf("%2d %c%c\n", gradesCount[i][j], GRADE_LETTERS[i], PLUS_MINUS[j]);
        }
    }
}

// vim: softtabstop=4 shiftwidth=4 expandtab
