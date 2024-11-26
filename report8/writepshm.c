#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define SHMSIZE 1024
#define SHMNAME "/my_shm"

int main() {
    int shmd;
    void *shmaddr;

    // 공유 메모리 객체 열기
    if ((shmd = shm_open(SHMNAME, O_CREAT | O_RDWR, 0666)) == -1) {
        perror("shm_open failed");
        exit(1);
    }

    // 공유 메모리 크기 설정
    if (ftruncate(shmd, SHMSIZE) != 0) {
        perror("ftruncate failed");
        exit(1);
    }

    // 공유 메모리 매핑
    if ((shmaddr = mmap(0, SHMSIZE, PROT_WRITE, MAP_SHARED, shmd, 0)) == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    // 공유 메모리 잠금
    if (mlock(shmaddr, SHMSIZE) != 0) {
        perror("mlock failed");
        exit(1);
    }

    // 데이터 쓰기
    strcpy((char *)shmaddr, "Hello, world");

    // 공유 메모리 매핑 해제
    if (munmap(shmaddr, SHMSIZE) == -1) {
        perror("munmap failed");
        exit(1);
    }

    // 공유 메모리 객체 닫기
    close(shmd);

    return 0;
}
