#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

void ConstructTimeNow(char*);
void ConstructDownloadLink(char* ,char*, char*);

int main(){
    pid_t child_id;
    int status;
    char format_name[50];
    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child

        pid_t pid, sid;        // Variabel untuk menyimpan PID
            
        pid = fork();     // Menyimpan PID dari Child Process

        /* Keluar saat fork gagal
        * (nilai variabel pid < 0) */
        if (pid < 0) {
            exit(EXIT_FAILURE);
        }

        /* Keluar saat fork berhasil
        * (nilai variabel pid adalah PID dari child process) */
        if (pid > 0) {
            exit(EXIT_SUCCESS);
        }

        umask(0);

        sid = setsid();
        if (sid < 0) {
            exit(EXIT_FAILURE);
        }

        if ((chdir("/")) < 0) {
            exit(EXIT_FAILURE);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        while (1) {
            // Tulis program kalian di sini
                
            ConstructTimeNow(format_name);   
            char *argv[] = {"mkdir", "-p", format_name, NULL};
            execv("/bin/mkdir", argv);
            sleep(10);
        }
    } else {
        // this is parent
        while ((wait(&status)) > 0);
        int j=0;
        char path[1000];
            
        for(j=0;j<10;j++){
            pid_t child_id;
            int status1;

            child_id = fork();

            if (child_id < 0) {
                exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
            }

            if (child_id == 0) {
                // this is child
                while ((wait(&status)) > 0);
                char size[5],download_link[1000],name[50];
                ConstructDownloadLink(size, download_link, name);
                sprintf(path, "%s/%s", format_name,name);
                char *argv[] = {"wget", "--no-check-certificate", "-q", download_link, "-O", path, NULL};
                execv("/bin/wget", argv);
            } else {
                // this is parent
                
            }
        }
            
    }
 
    return 0;
}

void ConstructTimeNow(char *format_name){
    int hours, minutes, seconds, day, month, year;
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    hours = local->tm_hour;          // get hours since midnight (0-23)
    minutes = local->tm_min;         // get minutes passed after the hour (0-59)
    seconds = local->tm_sec;         // get seconds passed after minute (0-59)
            
    day = local->tm_mday;            // get day of month (1 to 31)
    month = local->tm_mon + 1;       // get month of year (0 to 11)
    year = local->tm_year + 1900;    // get year since 1900
    sprintf(format_name,"%d-%02d-%02d_%02d:%02d:%02d\n", year, month, day, hours, minutes, seconds);
}
void ConstructDownloadLink(char *size ,char *link,char *name){
    sprintf(size,"%d",(int)time(NULL)%1000+50); 
    ConstructTimeNow(name);
    sprintf(link,"https://picsum.photos/id/2/%s/%s",size,size);
}
