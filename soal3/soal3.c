#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

int main(){
    int i=0;
    for(i=0;i<5;i++){
        pid_t child_id;
        int status;

        child_id = fork();

        if (child_id < 0) {
            exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
        }

        if (child_id == 0) {
            // this is child

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

            char format_name[50];
            sprintf(format_name,"%d-%02d-%02d_%02d:%02d:%02d\n", year, month, day, hours, minutes, seconds);
            //printf("%s", format_name);    
            char *argv[] = {"mkdir", "-p", format_name, NULL};
            execv("/bin/mkdir", argv);
    
        } else {
            // this is parent
            sleep(5);
        }
    }    
    return 0;
}
