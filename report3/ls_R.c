#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

void ls_R(const char *name) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name))) {
        perror("opendir");
        return;
    }

    printf("%s:\n", name);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }
        printf("%s  ", entry->d_name);
    }
    printf("\n\n");

    rewinddir(dir);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);

        struct stat entry_stat;
        if (stat(path, &entry_stat) == 0 && S_ISDIR(entry_stat.st_mode)) {
            ls_R(path);     // 디렉토리면 재귀 호출 사용
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *target_directory;

    if (argc > 1) {
        target_directory = argv[1];
    } else {
        target_directory = "."; 
    }

    ls_R(target_directory);
    return 0;
}
