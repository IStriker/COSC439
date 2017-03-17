#include <stdio.h>
#include <stdlib.h> /* for exit() */



void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
