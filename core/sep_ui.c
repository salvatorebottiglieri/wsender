#include <string.h>
#include <stdio.h>

#include "sep_log.h"

#define EQUAL_SYMBOL 61



/**
 * Prints a progress bar to the console.
 * The progress bar is represented by a string of equal signs (=) and spaces.
 * The equal signs represent the current progress and the spaces represent the remaining progress.
 * The progress bar is printed to the console followed by a percentage representation of the progress.
 * If the current progress is equal to the total progress, the progress bar is printed on a new line.
 * Otherwise, the progress bar is printed on the same line as the previous progress bar.
 * @param current The current progress
 * @param total The total progress
 */
void progress_bar(int current, int total){
    if (current > total){s_log(ERROR, "Current is greater than total"); return;}
    
    int ratio = current * 100 / total;
    char parts[100];
    memset(parts, EQUAL_SYMBOL, 99);
    parts[ratio-1] = '>';
    for (int i = ratio; i < 100; i++){
        parts[i] = ' ';
    }
    if (current == total){printf("[%s] %d%%\n", parts, ratio);}
    else {printf("[%s] %d%%\r", parts, ratio);}
    
}