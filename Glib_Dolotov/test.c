#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
void main(){
  int a =(int)pow(2,32)-1;
  unsigned long long ull = a;
  ull*=a;
  printf("%llu %d %u\n",ull,a,a);

  printf("%d %u %llu\n",UINT_MAX, UINT_MAX,(unsigned long long)UINT_MAX);
  printf("%d\n", sizeof(unsigned long));
}
