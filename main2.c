/*
*6장기본함수
파일이름:main.c
*/

#include<stdio.h>

int main(int argc, char *argv[]) {
    int i;
    for(i=0; i<argc; i++)printf("%s",argv[i]);
}