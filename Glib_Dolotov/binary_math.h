#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

//returns +1 if a>b, 0 if a=b, -1 if a>b
int compare(unsigned int* a, unsigned int* b);

//returns sum of a and b
unsigned int* sum(unsigned int* a, unsigned int* b);

//returns |a-b|
unsigned int* difference(unsigned int* a, unsigned int* b);

//returns a*b
unsigned int* product(unsigned int* a, unsigned int* b);

//returns a/b
unsigned int* quotient(unsigned int* a, unsigned int* b);

//returns a%b
unsigned int* remainder(unsigned int* a, unsigned int* b);

//returns a raised to the b power
unsigned int* power(unsigned int* a, unsigned int* b);
