#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int digit(char c); //WORKING
char rune(int c); //WORKING
char* divByTwo(char* val); // WORKING
//char* multByTwo(char* val);
int binaryDigits(int digits); //WORKING
char* decToBin(char* val); //WORKING
unsigned int binToDec(char* val, int start); //WORKING
unsigned int* binaryStore(char* bin); //WORKING
unsigned int* decimalStore(char* dec); //WORKING
unsigned int* stdinStore(); //WORKING

//reverse functions for testing & debugging purposes
char* uintToBinary(unsigned int val); //WORKING
char* unpackToBinary(unsigned int* store); //WORKING
//char* binToDec(char* bin);
