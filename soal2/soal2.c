#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <wait.h>

#define PATH "/home/krisna/modul2/petshop"
#define CHECK_FILE "/home/krisna/modul2/petshop/cat;atlas;0.6.jpg"

int main() {
    int N=2;
    int i=0;

    int status;

    for(i=0;i<N;i++)
    {
        pid_t child_id=fork();
        if (child_id < 0) {
            exit(EXIT_FAILURE);
        }

        if (chdir("/") < 0) {
            exit(EXIT_FAILURE);
        }

        if (child_id == 0) {
            DIR* dir = opendir(PATH);
            
            if (dir) {
                closedir(dir);

                char *argv[] = {"rm", "-rf", "/home/krisna/modul2/petshop/apex_cheats", NULL};
                execv("/bin/rm", argv);
            }
            else if (ENOENT == errno) {
                char *argv[] = {"mkdir", "-p", PATH, NULL};
                execv("/bin/mkdir", argv);
            }
        }
        else {
            while ((wait(&status)) > 0);

            if (access(CHECK_FILE, F_OK) == 0) {
                printf("ada\n");
            }
            else {
                printf("ga ada\n");
                char *argv[] = {"unzip", "-d", PATH, "/home/krisna/Documents/Code/Modul 2/SoalShift2/pets.zip", NULL};
                execv("/bin/unzip", argv);
            }

            
        }

    }
    return 0;
}
