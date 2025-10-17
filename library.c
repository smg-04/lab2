/*
*6장기본함수
*파일이름: library.c
*/

#include<stdio.h>
#include<math.h>
#include<ctype.h>

int main(void) {
    int i=4;
    char c='A';
    printf("sqrt(%d)=%f\n", i,sqrt(i));
    printf("tolower(%c)=%c\n",c,tolower(c));
}