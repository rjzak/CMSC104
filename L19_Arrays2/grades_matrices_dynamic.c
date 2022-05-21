/**
 * File: grades_matrices.c
 * Author: Richard Zak
 * Email: rzak1@umbc.edu
 * Date: 21 May 2022
 *
 * This file is a companion to Lecture 19: Arrays, Part 2.
 * Grade values are collected from the user, and stats are calculated.
 * Shows a use for two dimensional arrays, structs, #defines, constant arrays,
 * and how to use dynamic memory for arrays which increase as needed.
 *
 * Added is an example showing how to read from a text file directly, and how
 * to experiment with different data types by changing just two lines of code.
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

/*
** Here, we can play with the data type we'll use.
** Some thoughts:
**   * Always used unsigned, should never be negative
**   * Always be an integer type, otherwise some code will break
**   * Always update the former to make sure the values are read correctly
**   * Try: char, short, int, long.
*/
typedef unsigned short GRADE_TYPE;
#define GRADE_TYPE_FORMATTER "%hu"

const char GRADE_LETTERS[GRADES] = {'F', 'D', 'C', 'B', 'A'};
const char PLUS_MINUS[PLUS_MINUS_SIZE] = {'+', ' ', '-'};

typedef struct {
    float mean; /* average of all scores */
    GRADE_TYPE median;     /* when sorted, which grade is in the middle? */
    GRADE_TYPE mode;       /* which grade was the most common? */
    unsigned int mode_count; /* what was the frequency for mode? */
    GRADE_TYPE max;        /* max score */
    GRADE_TYPE min;        /* minimum score */
    unsigned int counts;   /* how many grades? */
} ClassStatistics;

typedef struct {
    GRADE_TYPE* array;      /* the array itself */
    size_t size;            /* the size of the array */
    size_t currentPosition; /* the location for the next insertion */
} GrowingArray;

// Functions for GrowingArray
GrowingArray* GrowingArray_Create();
GrowingArray* GrowingArray_CreateWithCapacity(const size_t initialSize);
void GrowingArray_InsertValue(GrowingArray* gr, const GRADE_TYPE value);
void GrowingArray_Sort(GrowingArray* gr);
void GrowingArray_Destroy(GrowingArray* gr);
void swap(GRADE_TYPE *p, GRADE_TYPE *q);

// Functions for handling grades
void PrintInstructions();
void GetScores(GrowingArray* scores);
void GetScoresFromFile(const char filePath[], GrowingArray* scores);
ClassStatistics CalcStats(GrowingArray* scores, GRADE_TYPE gradesCount[GRADES][PLUS_MINUS_SIZE]);
void PrintStats(const ClassStatistics stats, const GRADE_TYPE gradesCount[GRADES][PLUS_MINUS_SIZE]);

int main(int argc, char* argv[]) {
    /* Counts are never negative, so we use unsigned integers */
    GRADE_TYPE gradesCount[GRADES][PLUS_MINUS_SIZE] = {0};
    
    GrowingArray* scores = GrowingArray_CreateWithCapacity(10);

    PrintInstructions();
    if (argc == 1) {
        GetScores(scores);
    } else if (argc == 2) {
        GetScoresFromFile(argv[1], scores);
    } else {
        printf("Unexpected arguments. Provide path to a file to read from a file.\n");
        return EXIT_FAILURE;
    }
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
    GRADE_TYPE value;
    int matched;
    while(1) {
        printf("Enter next value: ");
        matched = scanf(GRADE_TYPE_FORMATTER, &value);
        if (matched == 0 || value < 0) {
            break;
        }
        while(value > MAX) {
            printf("Values must not be greater than %d.\n", MAX);
            printf("Enter next value: ");
            matched = scanf(GRADE_TYPE_FORMATTER, &value);
            if (matched == 0) {
                return; // bail out!
            }
        }
        GrowingArray_InsertValue(scores, value);
    }
}

/******************************************************************
** GetScoresFromFile - fills an unsigned integer array with valid values that
**                     are read from the indicated text file.  
**                     Assures the values are between zero and MAX.
** Inputs:  filePath - path to the text file to read
**            scores - array to fill
** Outputs: size - size of the array
** Notes: MAX must be #defined in this file
********************************************************************/
void GetScoresFromFile(const char filePath[], GrowingArray* scores) {
    FILE *fp = fopen(filePath, "r+");
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s for reading: %s\n", filePath, strerror(errno));
        errno = 0;
        return;
    }
    
    GRADE_TYPE value, matched;
    while(1) {
        matched = fscanf(fp, GRADE_TYPE_FORMATTER, &value);
        if (matched == 0 || matched == EOF) {
            break;
        }
        if (value > MAX)
            continue;
        GrowingArray_InsertValue(scores, value);
    }
    
    fclose(fp);
    fp = NULL;
}

/******************************************************************
** CalcStats - Returns statistical information about the grades
** Inputs:  scores array, size of scores array, and grades matrix
** Outputs: statistics struct
** Notes: Letter grades, GRADES, PLUS_MINUS_SIZE, MINUS, REGULAR, PLUS
**        must be #defined in this file
********************************************************************/
ClassStatistics CalcStats(GrowingArray* scores, GRADE_TYPE gradesCount[GRADES][PLUS_MINUS_SIZE]) {
    GRADE_TYPE i, total = 0;
    GRADE_TYPE counter[MAX+1] = {0};
    ClassStatistics stats;
    stats.max = 0;
    stats.min = 0;
    stats.mean = 0;
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
        counter[scores->array[i]]++; // count the frequency for the grades to calculate the mode
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
            default: printf("Invalid score " GRADE_TYPE_FORMATTER " found at index %d.\n", scores->array[i], i);
        } // end switch
    } // end for
    if (scores->currentPosition > 0)
        stats.mean = (float) total / (float) scores->currentPosition;
    stats.counts = scores->currentPosition;
    stats.mode = 0;
    stats.mode_count = 0;
    for(i = 0; i <= MAX; i++) {
        if (counter[i] > stats.mode_count) {
            stats.mode = i;
            stats.mode_count = counter[i];
        }
    }
    
    GrowingArray_Sort(scores);
    stats.median = scores->array[((scores->currentPosition+1) / 2) -1];
    
    return stats;
}

/*********************************************************
** swap: swaps two locations in an unsigned integer array
** Inputs: p & q are pointers to specific positions in the array
** Outputs: None
** Note: modifies the values at the represented locations
*********************************************************/
void swap(GRADE_TYPE *p, GRADE_TYPE *q) {
   GRADE_TYPE temp;
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
void PrintStats(const ClassStatistics stats, const GRADE_TYPE gradesCount[GRADES][PLUS_MINUS_SIZE]) {
    GRADE_TYPE i, j;
    if (stats.counts == 0) {
        printf("No grades entered.\n");
        return;
    }
    printf("The class average is %.2f, median is " GRADE_TYPE_FORMATTER ", mode is " GRADE_TYPE_FORMATTER " with %hu occurrences.\n", stats.mean, stats.median, stats.mode, stats.mode_count);
    printf("Max: " GRADE_TYPE_FORMATTER ", Min: " GRADE_TYPE_FORMATTER "\n", stats.max, stats.min);
    printf("Of the %hu grades, there were:\n", stats.counts);

    for(i = GRADES-1; i >= 0; i--) {
        if (i == F) { /* F is a special case, no plus or minus */
            printf("           %2d %c\n", gradesCount[i][REGULAR], GRADE_LETTERS[i]);
            break;
        }
        for(j = 0; j < PLUS_MINUS_SIZE; j++) {
            printf("           %2d %c%c\n", gradesCount[i][j], GRADE_LETTERS[i], PLUS_MINUS[j]);
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
    array->array = (GRADE_TYPE*) calloc(initialSize, sizeof(GRADE_TYPE));
    array->size = initialSize;
    return array;
}

/*********************************************************************************
 ** GrowingArray_InsertValue - Insert the value into the array, resizing if needed
 ** Inputs: Pointer to the array, value to be added
 ** Outputs: None
 *********************************************************************************/
void GrowingArray_InsertValue(GrowingArray* gr, const GRADE_TYPE value) {
    if (gr->currentPosition == gr->size-1) {
        size_t new_size = gr->size * 2;
        int i;
        GRADE_TYPE* temp = (GRADE_TYPE*) calloc(new_size, sizeof(GRADE_TYPE));
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
   GRADE_TYPE i, j;

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
