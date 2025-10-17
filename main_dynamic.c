#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    void *handle;
    int (*add)(int, int);
    int (*subtract)(int, int);
    int (*multiply)(int, int);
    double (*divide)(int, int);

    // 동적 라이브러리 로드
    handle = dlopen("./libcalc.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "라이브러리 로드 실패: %s\n", dlerror());
        exit(1);
    }

    // 함수 심볼 가져오기
    add = dlsym(handle, "add");
    subtract = dlsym(handle, "subtract");
    multiply = dlsym(handle, "multiply");
    divide = dlsym(handle, "divide");

    // dlerror 확인
    char *error;
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "심볼 가져오기 실패: %s\n", error);
        dlclose(handle);
        exit(1);
    }

    // 함수 사용
    int x = 10, y = 5;
    printf("x + y = %d\n", add(x, y));
    printf("x - y = %d\n", subtract(x, y));
    printf("x * y = %d\n", multiply(x, y));
    printf("x / y = %.2f\n", divide(x, y));

    // 라이브러리 닫기
    dlclose(handle);
    return 0;
}
