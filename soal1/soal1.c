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

#define DEBUG_ON

// 9 Apr 2021 22:22:00 GMT+07
#define BIRTHDAY_UNIX 1617981720
#define SIX_HOUR_BEFORE_BD_UNIX 1617960120

typedef struct GOOGLE_DRIVE {
  char id[128];
  char folder_name[32];
} GOOGLE_DRIVE;

GOOGLE_DRIVE gdrive[3] = {
  {"1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD", "FOTO"},
  {"1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J", "MUSIK"},
  {"1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp", "FILM"}
};

void prepareGift();
void downloadGD();
void __downloadGD(int);
void prepareGiftFolders();
void __prepareGiftFolder(char*);
void sortingFiles();
void filteredCopyFiles(char*, char*, char*);
void deleteUnusedFiles();
void __deleteFolder(char*);
void zipGift();
void getExtension(char*, char*, int);
bool isNull(char*);

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

  while(1) {
      unsigned long timestamp = time(NULL);
      if (timestamp >= SIX_HOUR_BEFORE_BD_UNIX) {
        pid_t c2;

        c2 = fork();

        if (c2 < 0) {
          exit(EXIT_FAILURE);
        }

        int s2;

        if (c2 == 0) {
          prepareGift();
        }
        else {
          while(wait(&c2) > 0);
          while(1) {
            timestamp = time(NULL);
            if (timestamp >= BIRTHDAY_UNIX) {
              zipGift();
              break;
            }
            else {
              sleep(1);
            }
          }
          printf("HAPPY BIRTHDAY!\n");
        }
        break;
      }
      else {
          sleep(1);
      }
  }
  return 0;
}

void prepareGift() {
  pid_t child;
  
  child = fork();

  if (child < 0) {
    exit(EXIT_FAILURE);
  }

  int status;

  if (child == 0) {
    downloadGD();
  }
  else {
    while(wait(&status) > 0);
    prepareGiftFolders();
  }
}

void downloadGD() {
  for (int i = 0; i < 3; i++) {
    pid_t child;

    child = fork();

    if (child < 0) {
      exit(EXIT_FAILURE);
    }

    int status;

    if (child == 0) {
      __downloadGD(i);
    }
    else {
      while(wait(&status) > 0);
    }
  }
}

void __downloadGD(int i) {
  pid_t child;

  child = fork();

  if (child < 0) {
    exit(EXIT_FAILURE);
  }

  int status;

  if (child == 0) {
    #if defined DEBUG_ON
      printf("Downloading %s...\n", gdrive[i].folder_name);
    #endif
    char download_link[1024];
    sprintf(download_link, "https://drive.google.com/uc?id=%s&export=download", gdrive[i].id);
    char *argv[] = {"wget", "--no-check-certificate", "-q", download_link, "-O", "temp.zip", NULL};
    execv("/usr/bin/wget", argv);
  }
  else {
    while(wait(&status) > 0);
    #if defined DEBUG_ON
      printf("Unzipping %s...\n", gdrive[i].folder_name);
    #endif
    char *argv[] = {"unzip", "-q", "temp.zip", NULL};
    execv("/usr/bin/unzip", argv);
  }
}

void prepareGiftFolders() {
  pid_t child;

  child = fork();

  if (child < 0) {
    exit(EXIT_FAILURE);
  }

  int status;

  if (child == 0) {
    #if defined DEBUG_ON
      printf("Preparing Stevany's favourite folders...\n");
    #endif
    __prepareGiftFolder("Pyoto");
    __prepareGiftFolder("Musyik");
    __prepareGiftFolder("Fylm");
  }
  else {
    while(wait(&status) > 0);
    sortingFiles();
  }
}

void __prepareGiftFolder(char *name) {
  pid_t child;

  child = fork();

  if (child < 0) {
    exit(EXIT_FAILURE);
  }

  int status;

  if (child == 0) {
    #if defined DEBUG_ON
      printf("Creating folder %s...\n", name);
    #endif
    char *argv[] = {"mkdir", name, NULL};
    execv("/bin/mkdir", argv);
  }
  else {
    while(wait(&status) > 0);
  }
}

void sortingFiles() {
  pid_t child;

  child = fork();

  if (child < 0) {
    exit(EXIT_FAILURE);
  }

  int status;

  if (child == 0) {
    typedef struct FILTER_COPY {
      char source[128];
      char dest[128];
      char ext[64];
    } FILTER_COPY;
    
    static FILTER_COPY FC[3] = {
      {"FOTO", "Pyoto", "jpg"},
      {"FILM", "Fylm", "mp4"},
      {"MUSIK", "Musyik", "mp3"}
    };

    for (int i = 0; i < 3; i++) {
      pid_t c2;

      c2 = fork();

      if (c2 < 0) {
        exit(EXIT_FAILURE);
      }

      int s2;

      if (c2 == 0) {
        filteredCopyFiles(FC[i].source, FC[i].dest, FC[i].ext);
      }
      else {
        while(wait(&s2) > 0);
      }
    }
  }
  else {
    while(wait(&status) > 0);
    deleteUnusedFiles();
  }
}

void filteredCopyFiles(char *source_folder, char *dest_folder, char* ext) {
  char path[1024];

  struct dirent *dp;
  DIR *dir = opendir(source_folder);

  if (!dir) {
    return;
  }

  while((dp = readdir(dir)) != NULL) {
    if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
      // If it is a file
      if (dp->d_type == DT_REG) {
        char dp_ext[16];
        getExtension(dp->d_name, dp_ext, 16);
        printf("%s has extension %s.\n", dp->d_name, dp_ext);
        if (!isNull(dp_ext) && strcmp(dp_ext, ext) == 0) {
          sprintf(path, "%s/%s", source_folder, dp->d_name);

          pid_t child;

          child = fork();

          if (child < 0) {
            exit(EXIT_FAILURE);
          }

          int status;

          if (child == 0) {
            char *argv[] = {"cp", path, dest_folder, NULL};
            execv("/bin/cp", argv);
          }
          else {
            while(wait(&status) > 0);
          }
        }
      }
    }
  }
}

void deleteUnusedFiles() {
  pid_t c1;

  c1 = fork();

  if (c1 < 0) {
    exit(EXIT_FAILURE);
  }

  int status;

  if (c1 == 0) {
    char *folderToDel[] = {"FOTO", "MUSIK", "FILM"};
    for (int i = 0; i < 3; i++) {
      pid_t c2;

      c2 = fork();

      if (c2 < 0) {
        exit(EXIT_FAILURE);
      }

      int s2;

      if (c2 == 0) {
        __deleteFolder(folderToDel[i]);
      }
      else {
        while(wait(&s2) > 0);
      }
    }
  }
  else {
    while(wait(&status) > 0);
  }
}

void __deleteFolder(char *name) {
  #if defined DEBUG_ON
    printf("Deleting folder %s...\n", name);
  #endif
  char *argv[] = {"rm", "-r", name, NULL};
  execv("/bin/rm", argv);
}

void zipGift() {
  #if defined DEBUG_ON
    printf("Zipping folders...\n");
  #endif
  char *argv[] = {"zip", "-qrm", "Lopyu_Stevany.zip", "Musyik", "Pyoto", "Fylm", NULL};
  execv("/usr/bin/zip", argv);
}

void getExtension(char *source, char *dest, int dest_size) {
  // Caution: Make sure the string is valid, otherwise there would be endless loop
  // A string is valid if it has null (\0) characters at the end
  int index = 0;
  bool start_recording = false;
  for (int i = 0; source[i] != '\0'; i++) {
    if (source[i] == '.') {
      start_recording = true;
    }
    else {
      if (start_recording) {
        dest[index++] = source[i];
      }
    }
  }
  dest[index] = '\0';
  // Validator
  if (dest_size < index) {
    printf("Warning: dest overflow.");
  }
}

bool isNull(char *s) {
  if (s[0] == '\0') return true;
  return false;
}
