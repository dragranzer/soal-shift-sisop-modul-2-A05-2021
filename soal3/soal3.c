#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

void ConstructTimeNow(char*);
void ConstructTimeNow2(char*);
void ConstructDownloadLink(char* ,char*, char*);
char format_name[50];
int main() {
    pid_t child_id;
    int status;
    
    child_id = fork();

    if (child_id < 0) {
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }
    
    if (child_id == 0) {
        // this is child
        while(1){
            pid_t child_id1;

            child_id1 = fork();

            if (child_id1 < 0) {
                exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
            }
            
            if (child_id1 == 0) {
                // this is child
                ConstructTimeNow(format_name);
                char *argv[] = {"mkdir", "-p", format_name, NULL};
                execv("/bin/mkdir", argv);
            } else {
                // this is parent

                sleep(40);
            }
        }
        
    } else {
        // this is parent
        sleep(2);
        int j=0;
        char path[1000];
        ConstructTimeNow2(format_name);
        for(j=0;j<10;j++){
            pid_t child_id2;

            child_id2 = fork();

            if (child_id2 < 0) {
                exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
            }

            if (child_id2 == 0) {
                // this is child
                char size[5],download_link[1000],name[50];
                ConstructDownloadLink(size, download_link, name);
                sprintf(path, "%s/%s", format_name,name);
                printf("%s\n",format_name);
                char *argv[] = {"wget", "--no-check-certificate", "-q", download_link, "-O", path, NULL};
                execv("/bin/wget", argv);
                
            } else {
                // this is parent
                sleep(5);
            }
        }
        

    }
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
    sprintf(format_name,"%d-%02d-%02d_%02d:%02d:%02d", year, month, day, hours, minutes, seconds);
}
void ConstructDownloadLink(char *size ,char *link,char *name){
    sprintf(size,"%d",(int)time(NULL)%1000+50); 
    ConstructTimeNow(name);
    sprintf(link,"https://picsum.photos/id/2/%s/%s",size,size);
}

void ConstructTimeNow2(char *format_name){
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
    sprintf(format_name,"%d-%02d-%02d_%02d:%02d:%02d", year, month, day, hours, minutes, seconds-2);
}
