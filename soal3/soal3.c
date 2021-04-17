#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include <ctype.h>

void ConstructTimeNow(char*);
void ConstructDownloadLink(char* ,char*, char*);
void encrypt(char* , char*, int);
void phrase_3a(char*);
void __makeFolder(char *);
void phrase_3b();
void downloadImage(char*);
void phrase_3c(char*);
int min(int a, int b) { return (a > b ? a : b); };
void __killNow();
void __makestatus(char*);
void __zipping(char*);
void __smoothKill(int);

char format_name[50],temp[50];
int main(int argc, char** argv) {
    pid_t pid, sid, p1, p2;        // Variabel untuk menyimpan PID

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
    int pidParrent = (int)getpid();
    if(strcmp(argv[1],"-z")==0)__killNow();
    else if(strcmp(argv[1],"-x")==0)__smoothKill(pidParrent);
    while (1) {
      char folder_name[50];
      ConstructTimeNow(folder_name);
      p1 = fork();
      if(p1==0)phrase_3a(folder_name);
      p2 = fork();
      if(p2==0){
        phrase_3b(folder_name);
        phrase_3c(folder_name);
      }
      sleep(40);
    }
}

void __makeFolder(char *folder_name) {
  char *argv[] = {"mkdir", "-p", folder_name, NULL};
  execv("/bin/mkdir", argv);
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

void phrase_3b(char *folder_name) {
  for (int j = 0; j < 10; j++) {
    //printf("DEBUG: donwlot gambar ke %d\n", j);
    downloadImage(folder_name);
    sleep(5);
  }
  //phrase_3c(folder_name);
}

void downloadImage(char *folder_name) {
  pid_t child_id2;

  child_id2 = fork();

  if (child_id2 < 0) {
      exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  int status;

  if (child_id2 == 0) {
      // this is child
      char size[5],download_link[1000],name[50];
      ConstructDownloadLink(size, download_link, name);
      char path[512];
      sprintf(path, "%s/%s.jpg", folder_name,name);
      char *argv[] = {"wget", "--no-check-certificate", "-q", download_link, "-O", path, NULL};
      execv("/usr/bin/wget", argv);
      
  }
}

void ConstructDownloadLink(char *size ,char *link,char *name){
    sprintf(size,"%d",(int)time(NULL)%1000+50); 
    ConstructTimeNow(name);
    sprintf(link,"https://picsum.photos/%s",size);
}

void phrase_3c(char *folder_name) {
    __makestatus(folder_name);
    __zipping(folder_name);
}

void __makestatus(char *folder_name){
    char dir[512];
    sprintf(dir, "%s/status.txt", folder_name);
    FILE *fptr = fopen(dir, "w");
    if (fptr == NULL) {
      exit(EXIT_FAILURE);
    }


    char text_to_write[50] = "Download Success";

    encrypt(text_to_write, text_to_write, strlen(text_to_write));
    //printf("DEBUG: Menulis status.txt (Enkripsi: %s)\n", text_to_write);

    fprintf(fptr, "%s", text_to_write);

    fclose(fptr);
}

void encrypt(char *text, char *encrypted, int len)
{
  len = min(strlen(text), len);

  // traverse text
  for (int i = 0; i < len; i++)
  {
    // apply transformation to each character
    // Encrypt Uppercase letters
    if (isupper(text[i]))
      encrypted[i] = ((text[i]+5-65)%26 +65); 
    else
      // Encrypt Lowercase letters
      encrypted[i] = ((text[i]+5-97)%26 +97);
  }
  encrypted[len] = '\0';
}

void __zipping(char *folder_name){
    char zippo[50];
    sprintf(zippo, "%s.zip", folder_name);
    char *argv[] = {"zip", "-qrm", zippo, folder_name, NULL};
    execv("/bin/zip", argv);
}

void __killNow(){
  char dir[512];
  sprintf(dir, "Killer.sh");
  FILE *fptr = fopen(dir, "w");
  if (fptr == NULL) {
    exit(EXIT_FAILURE);
  }

  char text_to_write[50] = "#!/bin/bash\n\nkillall soal3\nrm Killer.sh";
  fprintf(fptr, "%s", text_to_write);
  fclose(fptr);
}

void __smoothKill(int pid){
    char dir[512];
    sprintf(dir, "Killer.sh");
    FILE *fptr = fopen(dir, "w");
    if (fptr == NULL) {
    exit(EXIT_FAILURE);
    }

    char text_to_write[50];
    sprintf(text_to_write, "#!/bin/bash\n\nkill %d\nrm Killer.sh",pid);
    fprintf(fptr, "%s", text_to_write);
    fclose(fptr);
}

void phrase_3a(char *folder_name){
  __makeFolder(folder_name);
}
