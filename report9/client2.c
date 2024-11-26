#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include "chat.h"

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

key_t key;
int msgid;

// 송신
void *send_messages(void *arg) {
    struct message msg;
    char buffer[MAX_TEXT];

    while (1) {
        fgets(buffer, MAX_TEXT, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        msg.msg_type = 2; 
        strncpy(msg.msg_text, buffer, MAX_TEXT);

        // 메시지 큐에 전송
        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1)
            error_exit("msgsnd");

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("채팅 종료 요청.\n");
            break;
        }
    }
    pthread_exit(NULL);
}

// 수신
void *receive_messages(void *arg) {
    struct message msg;

    while (1) {
		// 메시지 큐에서 메시지 수신
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1)
            error_exit("msgrcv");

        printf("\n[Client1 메시지]: %s\n", msg.msg_text);
        if (strncmp(msg.msg_text, "exit", 4) == 0) {
            printf("상대방이 채팅을 종료했습니다.\n");
            break;
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t send_thread, receive_thread;

    key = ftok("progfile", 65);
    if (key == -1)
        error_exit("ftok");

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1)
        error_exit("msgget");

    printf("Client2: 채팅 시작 (종료하려면 'exit' 입력)\n");

    pthread_create(&send_thread, NULL, send_messages, NULL);
    pthread_create(&receive_thread, NULL, receive_messages, NULL);

    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    printf("채팅 종료.\n");
    return 0;
}
