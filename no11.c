#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
 * 실습 11. 공유 메모리(mmap) 기반 파일 복사
 */

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: shm_copy src dest\n");
        exit(1);
    }

    int src = open(argv[1], O_RDONLY);
    int dst = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);

    struct stat st;
    fstat(src, &st);
    ftruncate(dst, st.st_size);

    void *src_map = mmap(NULL, st.st_size, PROT_READ,  MAP_SHARED, src, 0);
    void *dst_map = mmap(NULL, st.st_size, PROT_WRITE, MAP_SHARED, dst, 0);

    memcpy(dst_map, src_map, st.st_size);

    munmap(src_map, st.st_size);
    munmap(dst_map, st.st_size);
    close(src);
    close(dst);

    printf("Copy completed.\n");
    return 0;
}
