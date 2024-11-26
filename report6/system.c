#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("사용법: %s <명령어> [옵션들]\n", argv[0]);
        exit(1);
    }

    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0) {
        if (strcmp(argv[1], "ls") == 0) {
            setenv("LS_COLORS", "di=1;34:fi=0;32:*.c=0;37:*.txt=0;37", 1);
            execvp("ls", (char *[]){ "ls", "--color=auto", "-la", NULL });
        } else {
            execvp(argv[1], argv + 1);
        }
        perror("execvp 실패:");
        exit(1);
    } else if (pid > 0) {
        wait(&status);
        return status;
    } else {
        perror("fork 실패");
        exit(1);
    }
}
