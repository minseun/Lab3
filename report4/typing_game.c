#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 256
#define MAX_SENTENCES 7

const char *sentences[MAX_SENTENCES] = {
    "This is a typing game program.",
    "Welcome to the typing practice program!",
    "We will practice typing the national anthem.",
    "Until East sea dries up and Mountain Baekdu wears away",
    "God protect and help us.",
    "Spectacular rivers and mountains with rose of sharon across the country",
    "Great korean people, Preserve our nation as a korean way."
};

int main() {
    char user_input[MAX_LENGTH];
    int errors = 0;
    int total_characters = 0;
    time_t start_time, end_time;

    printf("[ 타자 연습 프로그램 ]\n");
    printf("문장을 입력하고 Enter를 누르세요.\n\n");

    time(&start_time);

    for (int i = 0; i < MAX_SENTENCES; i++) {
        printf("문장 %d: %s\n", i + 1, sentences[i]);
        printf("입력: ");
        fgets(user_input, MAX_LENGTH, stdin);

        size_t len = strlen(user_input);
        if (len > 0 && user_input[len - 1] == '\n') {
            user_input[len - 1] = '\0';
        }

        for (size_t j = 0; j < strlen(sentences[i]); j++) {
            if (j >= strlen(user_input) || user_input[j] != sentences[i][j]) {
                errors++;
            }
        }

        total_characters += strlen(sentences[i]);
    }

    time(&end_time);

    double time_taken = difftime(end_time, start_time);
    double wpm = (total_characters / 5.0) / (time_taken / 60.0);

    printf("\n=== 결과 ===\n");
    printf("총 잘못된 입력 횟수: %d\n", errors);
    printf("평균 분당 타자수: %.2f\n", wpm);
    printf("총 소요 시간: %.2f초\n", time_taken);

    return 0;
}
