#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHM_SIZE 1024

int main(int argc, char *argv[]) {
    int shmid;
    pid_t pid;
    char *shared_txt;
    void *shared_memory = (void *)0;
    
    sem_t *semaphore = sem_open("/file_sem", O_CREAT, 0666, 0);
    if (semaphore == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    // 공유 메모리 생성
    key_t key = ftok(".", 'a');
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }
    
    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }
    shared_txt = (char *)shared_memory;

    pid = fork();
    if (pid == 0) {
        // 자식 프로세스
        FILE *f = fopen(argv[1], "r");
        if (f == NULL) {
            perror("Failed to open file for reading");
            exit(1);
        }
        
        fscanf(f, "%[^\n]s", shared_txt);
        fclose(f);
        printf("[child process] -> read %s\n", argv[1]);
        
        sem_post(semaphore);
        exit(0);
    } else if (pid > 0) {
        // 부모 프로세스
        sem_wait(semaphore);

        FILE *f = fopen(argv[2], "w");
        if (f == NULL) {
            perror("Failed to open file for writing");
            exit(1);
        }
        
        fprintf(f, "%s", shared_txt);
        fclose(f);
        printf("[parent process] -> copy to %s\n", argv[2]);

        wait(NULL);
    }

    sem_close(semaphore);
    sem_unlink("/file_sem");

    return 0;
}
