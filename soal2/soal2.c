#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

#define PATH "/home/krisna/modul2/petshop"
#define DEL_DIR_1 "/home/krisna/modul2/petshop/apex_cheats"
#define DEL_DIR_2 "/home/krisna/modul2/petshop/musics"
#define DEL_DIR_3 "/home/krisna/modul2/petshop/unimportant_files"

#define CHANGE_TO_ROOT if (chdir("/") < 0) exit(EXIT_FAILURE);
#define CHECK_FORK_SUCCESS(x) if (x < 0) exit(EXIT_FAILURE);

void deleteFiles();
void extractFiles();
void organizeFiles();

int main() {
    pid_t pid, sid;

    pid = fork();

    // Child spawn failed
    CHECK_FORK_SUCCESS(pid)

    // Terminate parent process
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    // Reserve SID, prevent orphan process
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    CHANGE_TO_ROOT

    while (1) {
        pid_t c;
        c = fork();

        int status;

        if (c == 0) {
            organizeFiles();
        }
        else {
            // printf("hai\n");
            break;
        }
    }

    return 0;
}

void organizeFiles() {
    pid_t child;
    child = fork();
    CHECK_FORK_SUCCESS(child)

    int status;

    if (child == 0) {
        deleteFiles();
    }
    else {
        while(wait(&status) > 0);
        char *argv[] = {"echo", "hadeh", NULL};
        execv("/bin/echo", argv);
    }
}

void deleteFiles() {
    pid_t child;
    child = fork();
    CHECK_FORK_SUCCESS(child)

    int status;

    if (child == 0) {
        extractFiles();
    }
    else {
        while(wait(&status) > 0);
        char *argv[] = {"rm", "-rf", DEL_DIR_1, DEL_DIR_2, DEL_DIR_3, NULL};
        execv("/bin/rm", argv);
    }
}

void extractFiles() {
    pid_t child;
    child = fork();
    CHECK_FORK_SUCCESS(child)

    int status;

    if (child == 0) {
        char *argv[] = {"mkdir", "-p", PATH, NULL};
        execv("/bin/mkdir", argv);
    }
    else {
        while(wait(&status) > 0);
        char *argv[] = {"unzip", "-d", PATH, "/home/krisna/Documents/Code/Modul 2/SoalShift2/pets.zip", NULL};
        execv("/bin/unzip", argv);
    }
}
