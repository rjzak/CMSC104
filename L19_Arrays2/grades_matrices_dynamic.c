/**
 * File: grades_matrices.c
 * Author: Richard Zak
 * Email: rzak1@umbc.edu
 * Date: 31 March 2022
 *
 * This file is a companion to Lecture 19: Arrays, Part 2.
 * Grade values are collected from the user, and stats are calculated.
 * Shows a use for two dimensional arrays, structs, #defines, constant arrays,
 * and how to use dynamic memory for arrays which increase as needed.
 */


#include<stdio.h>
#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>

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

typedef struct {
    unsigned int* array;    /* the array itself */
                            /* Grades are never negative, so we use unsigned integers */
    size_t size;            /* the size of the array */
    size_t currentPosition; /* the location for the next insertion */
} GrowingArray;

// Functions for GrowingArray
GrowingArray* GrowingArray_Create();
GrowingArray* GrowingArray_CreateWithCapacity(const size_t initialSize);
void GrowingArray_InsertValue(GrowingArray* gr, const unsigned int value);
void GrowingArray_Sort(GrowingArray* gr);
void GrowingArray_Destroy(GrowingArray* gr);
void swap(unsigned int *p, unsigned int *q);

// Functions for handling grades
void PrintInstructions();
void GetScores(GrowingArray* scores);
ClassStatistics CalcStats(GrowingArray* scores, unsigned int gradesCount[GRADES][PLUS_MINUS_SIZE]);
void PrintStats(const ClassStatistics stats, const unsigned int gradesCount[GRADES][PLUS_MINUS_SIZE]);

int main() {
    /* Counts are never negative, so we use unsigned integers */
    unsigned int gradesCount[GRADES][PLUS_MINUS_SIZE] = {0};
    
    GrowingArray* scores = GrowingArray_CreateWithCapacity(10);

    PrintInstructions();
    GetScores(scores);
    ClassStatistics stats = CalcStats(scores, gradesCount);
    PrintStats(stats, gradesCount);
    
    // Dynamic memory must be freed when no longer needed
    GrowingArray_Destroy(scores);

    return EXIT_SUCCESS;
}

/*********************************************************
** PrintInstructions - prints the user instructions
** Inputs:  None
** Outputs:  None
** Notes: CLASS_SIZE must be #defined in this file
**********************************************************/
void PrintInstructions() {
    printf("This program calculates the average score\n");
    printf("given a list of grades. It also reports the\n");
    printf("number of As, B's, C's, D's, and F's. You will\n");
    printf("be asked to enter the individual scores. A negative number exits.\n");
}

/******************************************************************
** GetScores - fills an unsigned integer array with valid values that
**                are entered by the user.  
**                Assures the values are between zero and MAX.
** Inputs:  scores - array to fill
** Outputs: size - size of the array
** Notes: MAX must be #defined in this file
********************************************************************/
void GetScores(GrowingArray* scores) {
    int value;
    while(1) {
        printf("Enter next value: ");
        scanf("%d", &value);
        if (value < 0) {
            break;
        }
        while(value > MAX) {
            printf("Values must not be greater than %d.\n", MAX);
            printf("Enter next value: ");
            scanf("%u", &value);
        }
        GrowingArray_InsertValue(scores, value);
    }
}

/******************************************************************
** CalcStats - Returns statistical information about the grades
** Inputs:  scores array, size of scores array, and grades matrix
** Outputs: statistics struct
** Notes: Letter grades, GRADES, PLUS_MINUS_SIZE, MINUS, REGULAR, PLUS
**        must be #defined in this file
********************************************************************/
ClassStatistics CalcStats(GrowingArray* scores, unsigned int gradesCount[GRADES][PLUS_MINUS_SIZE]) {
    unsigned int i, total = 0;
    ClassStatistics stats;
    for(i = 0; i < scores->currentPosition; i++) {
        total += scores->array[i];
        if (i == 0) { // We can assume the first element in the array is the largest and smallest
            stats.max = scores->array[i];
            stats.min = scores->array[i];
        } else {
            if (stats.max < scores->array[i]) {
                stats.max = scores->array[i];
            }
            if (stats.min > scores->array[i]) {
                stats.min = scores->array[i];
            }
        }
        switch(scores->array[i]/10) {
            case 10: gradesCount[A][PLUS]++; break;
            
            /* There has to be a better way to handle plus/minus, and have
               a common way to have the cut-off values specified for all */
            case 9:
                if (scores->array[i] >= 97) {
                    gradesCount[A][PLUS]++;
                } else if (scores->array[i] > 92) {
                    gradesCount[A][REGULAR]++;
                } else {
                    gradesCount[A][MINUS]++;
                }
                break;
            case 8:
                if (scores->array[i] >= 87) {
                    gradesCount[B][PLUS]++;
                } else if (scores->array[i] > 82) {
                    gradesCount[B][REGULAR]++;
                } else {
                    gradesCount[B][MINUS]++;
                }
                break;
            case 7:
                if (scores->array[i] > 77) {
                    gradesCount[C][PLUS]++;
                } else if (scores->array[i] > 72) {
                    gradesCount[C][REGULAR]++;
                } else {
                    gradesCount[C][MINUS]++;
                }
                break;
            case 6:
                if (scores->array[i] > 67) {
                    gradesCount[D][PLUS]++;
                } else if (scores->array[i] > 62) {
                    gradesCount[D][REGULAR]++;
                } else {
                    gradesCount[D][MINUS]++;
                }
                break;
            
            /* F is a special case, no plus or minus */
            case 5: case 4: case 3: case 2:
            case 1: case 0: gradesCount[F][REGULAR]++; break;
            default: printf("Invalid score %d found at index %d.\n", scores->array[i], i);
        } // end switch
    } // end for
    stats.mean = (float) total / (float) scores->currentPosition;
    
    GrowingArray_Sort(scores);
    stats.median = scores->array[(scores->currentPosition+1) / 2 -1];
    
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

/************************************************************************
 ** GrowingArray_Create - Creates an empty array with no intial capacity
 ** Input:  None
 ** Outputs: Pointer to the GrowingArray struct set to zero
 ** Note: Check that the pointer isn't null!
 ************************************************************************/
GrowingArray* GrowingArray_Create() {
    GrowingArray* array = (GrowingArray*) calloc(1, sizeof(GrowingArray));
    return array;
}

/*****************************************************************************************
 ** GrowingArray_CreateWithCapacity - Creates an empty array with initial space for values
 *****************************************************************************************/
GrowingArray* GrowingArray_CreateWithCapacity(const size_t initialSize) {
    GrowingArray* array = GrowingArray_Create();
    array->array = (unsigned int*) calloc(initialSize, sizeof(unsigned int));
    array->size = initialSize;
    return array;
}

/*********************************************************************************
 ** GrowingArray_InsertValue - Insert the value into the array, resizing if needed
 ** Inputs: Pointer to the array, value to be added
 ** Outputs: None
 *********************************************************************************/
void GrowingArray_InsertValue(GrowingArray* gr, const unsigned int value) {
    if (gr->currentPosition == gr->size-1) {
        size_t new_size = gr->size * 2;
        int i;
        unsigned int* temp = (unsigned int*) calloc(new_size, sizeof(unsigned int));
        for(i = 0; i < gr->size; i++) {
            temp[i] = gr->array[i];
        }
        free(gr->array);
        gr->array = temp;
        gr->size = new_size;
    }
    gr->array[gr->currentPosition++] = value;
}

/*********************************************************
** GrowingArray_Sort - sorts an array from low to high
** Inputs: Pointer to array
** Outputs: None
** Note: modifies the contents of the array in-memory
**********************************************************/
void GrowingArray_Sort(GrowingArray* gr) {
   unsigned int i, j;

   for(i = 0; i < gr->currentPosition; i++) {
      for(j = 0; j < gr->currentPosition-i; j++) {
         if (gr->array[j] > gr->array[j+1])
            swap(&gr->array[j], &gr->array[j+1]);
      }
   }
}

/*************************************************
 ** GrowingArray_Destroy - Free all the memory used by the array
 ** Input: Pointer to the array
 ** Output: None
 *************************************************/
void GrowingArray_Destroy(GrowingArray* gr) {
    free(gr->array);
    gr->array = NULL;
    free(gr);
    gr = NULL;
}

// vim: softtabstop=4 shiftwidth=4 expandtab
