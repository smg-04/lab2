#include <stdio.h>
#include "calc.h"

int main() {
    int x=10, y=5;
    printf("x+y=%d\n", add(x,y));
    printf("x-y=%d\n", subtract(x,y));
    printf("x*y=%d\n", multiply(x,y));
    printf("x/y=%.2f\n", divide(x,y));
    return 0;
}
