//binary_math.h already includes libc h-files
#include "binary_math.h"

int compare(unsigned int* a, unsigned int* b){
}

unsigned int* sum(unsigned int* a, unsigned int* b){
  int a_count = a[0];
  int b_count = b[0];
  unsigned int* final;
  //assume that there will be a carry-over into another unsigned int rep.
  //first value can always be adjusted to less if that's how it turns out.
  if(a[0]>b[0]){
    final = (unsigned int*)calloc(a[0]+2,sizeof(unsigned int));
    final[0]=a[0]+1;
  }
  else{
    final = (unsigned int*)calloc(b[0]+2,sizeof(unsigned int));
    final[0]=b[0]+1;
  }


  int i = 1;
  long long temp;
  long long carry = 0;
  while(a_count>0 || b_count>0){
    if(a_count>0 && b_count>0){
      temp = a[i];
      temp+= b[i];
      temp+= carry;
      if (temp > (long long)UINT_MAX){
	temp-=(long long)UINT_MAX;
	carry=1;
	final[i]=(unsigned int)temp;
      }
    }
    else if (a_count<1){
      temp = b[i];
      temp+= carry;
      if (temp > (long long)UINT_MAX){
	temp-=(long long)UINT_MAX;
	carry=1;
	final[i]=(unsigned int)temp;
      }
    }
    else if (b_count<1){
      temp = a[i];
      temp+= carry;
      if (temp > (long long)UINT_MAX){
	temp-=(long long)UINT_MAX;
	carry=1;
	final[i]=(unsigned int)temp;
      }
    }
    i++;
    a_count--;
    b_count--;
  }
  if(carry==1)
    final[i]=carry;
  else
    final[0]=final[0]-1;
  return final;
}

unsigned int* difference(unsigned int* a, unsigned int* b){
}

unsigned int* product(unsigned int* a, unsigned int* b){
}

unsigned int* quotient(unsigned int* a, unsigned int* b){
}

unsigned int* remain(unsigned int* a, unsigned int* b){
}

unsigned int* power(unsigned int* a, unsigned int* b){
}
