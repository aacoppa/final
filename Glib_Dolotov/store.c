#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//converts char (digit by digit) into integer value
int digit(char c){
  if (c=='0')
    return 0;
  if (c=='1')
    return 1;
  if (c=='2')
    return 2;
  if (c=='3')
    return 3;
  if (c=='4')
    return 4;
  if (c=='5')
    return 5;
  if (c=='6')
    return 6;
  if (c=='7')
    return 7;
  if (c=='8')
    return 8;
  if (c=='9')
    return 9;
}

//does the inverse of digit()
char rune(int c){
  if (c==0)
    return '0';
  if (c==1)
    return '1';
  if (c==2)
    return '2';
  if (c==3)
    return '3';
  if (c==4)
    return '4';
  if (c==5)
    return '5';
  if (c==6)
    return '6';
  if (c==7)
    return '7';
  if (c==8)
    return '8';
  if (c==9)
    return '9';
}

//divides string-represented even value by two
char* divByTwo(char* val){
  int temp,i;
  temp = 0;
  for(i = 0; i < strlen(val);i++){
    temp = (temp%2)*10 + digit(val[i]);
    val[i] = rune(temp/2);
  }
  return val;
}


//count how many binary digits it would take to represent
//a value having x decimal digits
int binaryDigits(int digits){
  return (((int)log2(10))*digits)+1;
}


//represent value (represented as string) as a string of binary
char* decToBin(char* val){
  int b,i;
  b = binaryDigits(strlen(val));
  printf("%d\n",b);
  char* final = (char*) calloc(b+1,sizeof(char));
  for(i=0;i<b;i++){
    if (digit(val[strlen(val)-1])%2 == 0)
      final[i] = '0';
    else{
      final[i] = '1';
      val[strlen(val)-1]=rune(val[strlen(val)-1]-1);
      printf("%s\n",val); 
    }
    printf("%s ",final);
    divByTwo(val);
    printf("%s\n",val);
  }
  return final;
}

void main(){
  /*
  char* buff = (char*) calloc(1000,sizeof(char));
  fgets(buff, 1000, stdin);

  //test fgets: SUCCESS
  // printf("%s\n",buff);

  char* bin = decToBin(buff);
  printf("%s\n%s\n",buff,bin);
  */
  //test divByTwo: SUCCESS
  char x[] = "22222";
  decToBin(x);
  printf("%s\n",x);
}
