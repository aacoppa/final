#include "binary_math.h"
#include "store.h"

void main(){
  unsigned char* a,b;
  a = stdinStore();
  b = stdinStore();
  int i;
  for(i=0;i<=a[0];i++)
    printf("%u ",a[i]);
  printf("\n");
  for(i=0;i<=b[0];i++)
    printf("%u ",b[i]);
  printf("\n");
}
