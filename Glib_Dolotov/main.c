#include "binary_math.h"
#include "store.h"

void main(){
  unsigned int* a;
  unsigned int* b;
  a = stdinStore();
  b = stdinStore();
  int i;
  for(i=0;i<=a[0];i++)
    printf("%u ",a[i]);
  printf("\n");
  for(i=0;i<=b[0];i++)
    printf("%u ",b[i]);
  printf("\n");

  printf("%d\n",compare(a,b));

  unsigned int* c;
  c = sum(a,b);
  for(i=0;i<=b[0];i++)
    printf("%u ",c[i]);
  printf("\n");

}
