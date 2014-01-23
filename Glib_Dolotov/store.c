#include "store.h"

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

char* addStrs(char* a, char*b){
  char* final;
  int finlen;
  if(strlen(a)>strlen(b)){
    final = (char*)calloc(strlen(a)+2,sizeof(char));
    finlen=strlen(a)+2;
  }
  else{
    final = (char*)calloc(strlen(b)+2,sizeof(char));
    finlen=strlen(b)+2;
  }
  int a_count = strlen(a);
  int b_count = strlen(b);
  int i = 1;
  int temp = 0;
  while(a_count>0 || b_count>0){
    if(b_count>0){
      temp+=digit(b[b_count-i]);
      b_count--;
    }
    if(a_count>0){
      temp+=digit(a[a_count-i]);
      a_count--;
    }
    final[finlen-i]=rune(temp%10);
    i++;
    temp = temp/10;
  }
  final[0] = rune(temp);
  if (final[0]=='0')
    final++;
  return final;
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

char* multByTwo(char* val){
  char* final = (char*)calloc(strlen(val)+2,sizeof(char));
  int temp, i,j;
  temp = 0;
  for(i=strlen(val)-1;i>=0;i--){
    temp+=digit(val[i])*2;
    final[i+1] = rune(temp%10);
    temp=temp/10;
  }
  final[0] = rune(temp);
  if (final[0]=='0')
    final++;
  return final;
}

//count how many binary digits it would take to represent
//a value having x decimal digits
int binaryDigits(int digits){
  return ((int)(log2(10)*digits))+1;
}


//represent value (represented as string) as a string of binary
char* decToBin(char* val){
  int b,i;
  b = binaryDigits(strlen(val));
  //printf("%d\n",b);
  while(!(b%32==0))
    b++;
  char* final = (char*) calloc(b+1,sizeof(char));
  for(i=0;i<b;i++){
    if (digit(val[strlen(val)-1])%2 == 0)
      final[i] = '0';
    else{
      final[i] = '1';
      val[strlen(val)-1]=rune(val[strlen(val)-1]-1);
      //printf("%s\n",val); 
    }
    //printf("%s ",final);
    divByTwo(val);
    //printf("%s\n",val);
  }
  printf("Storing binary:\n%s\n",final);
  return final;
}

unsigned int binToDec(char* val, int start){
  unsigned int result = 0;
  int i;
  for(i=0;i<32;i++)
    if (val[start+i]=='1')
      result += (int) pow(2,i);
  return result;
}

//take the binary and break it up into unsigned ints
//which would store the value bitwise
unsigned int* binaryStore(char* bin){
  unsigned int* final = 
    (unsigned int*) calloc(strlen(bin)/32+1,sizeof(unsigned int));
  //first element of the unsigned int array will be the number of
  //unsigned ints (sets of 32 bits) that compose the value
  final[0]=strlen(bin)/32;
  int i;
  for(i=1; i<strlen(bin)/32+1;i++){
    final[i]=binToDec(bin,(i-1)*32);
    //printf("%u\n",final[i]);
  }
  //printf("%d\n", final[0]);
  return final;
}

unsigned int* decimalStore(char* dec){
  char* bin = decToBin(dec);
  //printf("%s\n",bin);
  return binaryStore(bin);
}

unsigned int* stdinStore(){
  //get value and store it as a string of binary:
  char* buff = (char*) calloc(1000,sizeof(char));
  fgets(buff, 1000, stdin);
  //remove the new-line byte
  buff[strlen(buff)-1]=0;
  return decimalStore(buff);
}

char* uintToBinary(unsigned int val){
  char* final = (char*)calloc(32,sizeof(char));
  int i;
  for(i=0;i<32;i++){
    if(val%2==0)
      final[i]='0';
    else{
      final[i]='1';
      val--;
    }
    val=val/2;
  }
  return final;
}

char* unpackToBinary(unsigned int* store){
  int k = store[0];
  int i,j;
  char* temp;
  char* final = (char*)calloc(32*k,sizeof(char));
  for(i=1;i<=k;i++){
    temp = uintToBinary(store[i]);
    for(j=0;j<32;j++){
      final[j+32*(i-1)] = temp[j];
    }
  }
  return final;
}


//char* binToDec(char* bin){
//  char* final = (char*)calloc((int)(strlen(bin)-1)/log2(10)+1,sizeof(char));
//  int i;
// for(i=
//}


void main(){
  //unsigned int* a = stdinStore();
  //char* bin = unpackToBinary(a);
  //char* inverse = binToDec(bin);
  //printf("Translated and untranslated:\n%s\n",inverse);

  char* buff1 = (char*) calloc(1000,sizeof(char));
  fgets(buff1, 1000, stdin);
  buff1[strlen(buff1)-1]=0;

  char* buff2 = (char*) calloc(1000,sizeof(char));
  fgets(buff2, 1000, stdin);
  buff2[strlen(buff2)-1]=0;

  printf("sum:\n%s\n",addStrs(buff1,buff2));
}

