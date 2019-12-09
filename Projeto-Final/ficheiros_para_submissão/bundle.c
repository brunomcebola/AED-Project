/*******************************************************************************
 * 2019-2020 AED - grupo 65
 * Last modified: 2019-12-09
 *
 * NAME
 *      bundle.c
 *
 * DESCRIPTION
 *      Implements a function to check the memory allocation 
 *
 * COMMENTS
 *      none
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "bundle.h"


/*******************************************************************************
* Function name: freeSolver()
*
* Arguments: num - number of pointers
*            ... - any number of pointers
*
* Return: none
*
* Side-effects: Forces the program to terminate (with exit(0)) if there is an
*               error with the memory managment (opening/creating files or
*               allocating memory)
*
* Description: checks if pointers are set to NULL (memory error)
*
*******************************************************************************/
void checkNull(int num, ...) {
    va_list valist;
    int i;
    va_start(valist, num);

    for (i = 0; i < num; i++) {
      if(va_arg(valist, void*) == NULL){
          exit(0);
      }
   }

   va_end(valist);
}
