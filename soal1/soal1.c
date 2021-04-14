#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

// 9 Apr 2021 22:22:00 GMT+07
#define BIRTHDAY_UNIX 1617981720

void removeFile(char*);
void removeFolder_t(char*);
void unzipFile(char*);
void downloadZipFromDrive(char*, char*);
void renameFolder(char*, char*);
void constructDirectDownloadLink(char*, char*);
void downloadAndExtractThenRemove(char*, char*, char*, char*);
void renameFolder(char*, char*);
void happyBirthdayStevany();
void zipAndRemoveStevanyStuff();
void downloadStevanyStuff();

int main() {
    pid_t pid, sid;

    pid = fork();

    // Child spawn failed
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

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

    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    // close(STDERR_FILENO);

    while(1) {
        unsigned long timestamp = time(NULL);
        if (timestamp >= BIRTHDAY_UNIX) {
            printf("\nHappy Birthday, Stevany!\n");
            happyBirthdayStevany();
        }
        else {
            sleep(1);
        }
    }
    return 0;
}

void happyBirthdayStevany() {
    pid_t child_id;

    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE);
    }

    int status;

    if (child_id == 0) {
        printf("Downloading stevany stuff, please wait...\n");
        downloadStevanyStuff();
    }
    else {
        while(wait(&status) > 0);
        printf("Cleaning up, ehe...\n");

        pid_t child2_id;

        child2_id = fork();
        if (child2_id < 0) {
            exit(EXIT_FAILURE);
        }

        int status2;

        if (child2_id == 0) {
            zipAndRemoveStevanyStuff();
        }
        else {
            while(wait(&status2) > 0);
            printf("Well done.\nPress ENTER to close\n");
            exit(EXIT_SUCCESS);
        }
    }
}

void downloadStevanyStuff() {
    pid_t child_id;

    child_id = fork();

    if (child_id == 0) {
        downloadAndExtractThenRemove("1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD", "Photo_for_Stevany.zip", "FOTO", "Pyoto");
    }
    else {
        pid_t child2_id;
        child2_id = fork();

        if (child2_id == 0) {
            downloadAndExtractThenRemove("1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J", "Music_for_Stevany.zip", "MUSIK", "Musyik");
        }
        else {
            downloadAndExtractThenRemove("1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp", "Film_for_Stevany.zip", "FILM", "Fylm");
        }
    }
}

void constructDirectDownloadLink(char *download_link, char *drive_id) {
    sprintf(download_link, "https://drive.google.com/uc?id=%s&export=download", drive_id);
}

void downloadZipFromDrive(char *drive_id, char *name) {
    char download_link[1024];
    constructDirectDownloadLink(download_link, drive_id);
    char *argv[] = {"wget", "--no-check-certificate", "-q", download_link, "-O", name, NULL};
    execv("/usr/bin/wget", argv);
}

void unzipFile(char *name) {
    char *argv[] = {"unzip", "-q", name, NULL};
    execv("/usr/bin/unzip", argv);
}

void removeFile(char *name) {
    char *argv[] = {"rm", name, NULL};
    execv("/bin/rm", argv);
}

void zipAndRemoveStevanyStuff() {
    char *argv[] = {"zip", "-qrm", "Lopyu_Stevany.zip", "Fylm", "Musyik", "Pyoto", NULL};
    execv("/usr/bin/zip", argv);
}

void removeFolder_t(char *name) {
    pid_t child_id;
    child_id = fork();
    if (child_id < 0) {
        exit(EXIT_FAILURE);
    }
    if (child_id == 0) {
        char *argv[] = {"rm", "-r", name, NULL};
        execv("/bin/rm", argv);
    }
}

void downloadAndExtractThenRemove(char *drive_id, char *name, char *yyy, char *xxx) {
    pid_t child_id;

    // Spawn child process and save its PID to child_id
    child_id = fork();

    // Fail to spawn child process
    if (child_id < 0) {
        exit(EXIT_FAILURE);
    }

    int status;

    if (child_id == 0) {
        downloadZipFromDrive(drive_id, name);
    }
    else {
        while(wait(&status) > 0);

        child_id = fork();

        if (child_id < 0) {
            exit(EXIT_FAILURE);
        }

        if (child_id == 0) {
            unzipFile(name);
        }
        else {
            while(wait(&status) > 0);

            child_id = fork();

            if (child_id == 0) {
                renameFolder(yyy, xxx);
            }
            else {
                while(wait(&status) > 0);
                removeFile(name);
            }
        }
    }
}

void renameFolder(char *from, char *to) {
    char *argv[] = {"mv", from, to, NULL};
    execv("/bin/mv", argv);
}
