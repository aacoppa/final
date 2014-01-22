#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int digit(char c);

char rune(int c);

char* divByTwo(char* val);

char* multByTwo(char* val);

int binaryDigits(int digits);

char* decToBin(char* val);

unsigned int binToDec(char* val, int start);

unsigned int* binaryStore(char* bin);

unsigned int* decimalStore(char* dec);

unsigned int* stdinStore();

//reverse functions for testing & debugging purposes
char* uintToBinary(unsigned int val);

char* unpackToBinary(unsigned int* store);

char* binToDec(char* bin);
