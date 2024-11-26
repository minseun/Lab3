/* count – 한 파일 내의 문자 수를 센다 */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>  // 추가: printf() 사용을 위한 헤더 파일

#define BUFSIZE 512 /* 읽혀질 덩어리의 크기 */

int main() {
    char buffer[BUFSIZE];
    int filedes;
    ssize_t nread;
    long total = 0;

    /* “anotherfile”을 읽기 전용으로 개방 */
    if ((filedes = open("anotherfile", O_RDONLY)) == -1) {
        printf("error in opening anotherfile\n");
        exit(1);
    }

    /* EOF까지 반복하라. EOF는 복귀값 0에 의해 표시된다 */
    while ((nread = read(filedes, buffer, BUFSIZE)) > 0)
        total += nread; /* total을 증가시킨다 */

    printf("total chars in anotherfile: %ld\n", total);
    exit(0);
}
