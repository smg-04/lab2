/*
*6장기본함수
*파일이름:envp.c
*/

#include<stdio.h>

int main (int argc, char *argv[], char *envp[]) {
    int i;
    for(i=0;envp[i]; i++)
        printf("%s\n",envp[i]);
}