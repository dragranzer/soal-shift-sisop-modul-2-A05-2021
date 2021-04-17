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

#define MAX 256

#define PATH "/home/krisna/modul2/petshop"
#define DEL_DIR_1 "/home/krisna/modul2/petshop/apex_cheats"
#define DEL_DIR_2 "/home/krisna/modul2/petshop/musics"
#define DEL_DIR_3 "/home/krisna/modul2/petshop/unimportant_files"

#define CHANGE_TO_ROOT if (chdir("/") < 0) exit(EXIT_FAILURE);
#define CHECK_FORK_SUCCESS(x) if (x < 0) exit(EXIT_FAILURE);

char jenis[15][10];
int i = 0;

char dest[20];
char petName[10];
char petAge[5];

bool need_dup = false;

char secondPetType[10];
char secondPetName[10];
char secondPetAge[5];

FILE *fp;

typedef struct bstnode_t {
    char str[10];
    struct bstnode_t \
        *left, *right;
} BSTNode;
typedef struct bst_t {
    BSTNode *_root;
    unsigned int _size;
} BST;

BSTNode* __bst__createNode(char *str) {
    BSTNode *newNode = (BSTNode*) malloc(sizeof(BSTNode));
    strcpy(newNode->str, str);
    newNode->left = newNode->right = NULL;
    return newNode;
}
BSTNode* __bst__insert(BSTNode *root, char *str) {
    if (root == NULL) 
        return __bst__createNode(str);

    if (strcmp(str, root->str) < 0)
        root->left = __bst__insert(root->left, str);
    if (strcmp(str, root->str) > 0)
        root->right = __bst__insert(root->right, str);
    
    return root;
}
BSTNode* __bst__search(BSTNode *root, char *str) {
    while (root != NULL) {
        if (strcmp(str, root->str) < 0)
            root = root->left;
        else if (strcmp(str, root->str) > 0)
            root = root->right;
        else
            return root;
    }
    return root;
}
void __bst__inorder(BSTNode *root) {
    if (root) {
        __bst__inorder(root->left);
        fprintf(fp, "%s\n", root->str);
        __bst__inorder(root->right);
    }
}

void bst_init(BST *bst) {
    bst->_root = NULL;
    bst->_size = 0u;
}
bool bst_isEmpty(BST *bst) {
    return bst->_root == NULL;
}
bool bst_find(BST *bst, char *str) {
    BSTNode *temp = __bst__search(bst->_root, str);
    if (temp == NULL)
        return false;
    
    if (strcmp(temp->str, str) == 0)
        return true;
    else
        return false;
}
void bst_insert(BST *bst, char* str) {
    if (!bst_find(bst, str)) {
        bst->_root = __bst__insert(bst->_root, str);
        bst->_size++;
    }
}

void bst_inorder(BST *bst) {
    fp = fopen("/home/krisna/modul2/petshop/jenis.txt", "w+");
    __bst__inorder(bst->_root);
    fclose(fp);
}

char fname[] = "jenis.txt";
char temp[] = "temp.txt";
char removed[MAX];

void removeLine() {
    int line, ctr = 0;
    char ch;

    FILE *fp1, *fp2;

    // buat copy ke file temp
    char str[MAX];

    fp1 = fopen(fname, "r");
    // can't open file
    if (!fp1) {
        printf("file cannot be opened\n");
        return;
    }

    fp2 = fopen(temp, "w");
    if (!fp2) {
        printf("unable to make temp\n");
        fclose(fp1);
        return;
    }

    line = 0;
    line++;

    while (!feof(fp1)) {
        strcpy(str, "\0");
        fgets(str, MAX, fp1);

        if (!feof(fp1)) {
            ctr++;

            if (ctr != line) {
                fprintf(fp2, "%s", str);
            }
            else {
                int len = strlen(str);
                str[len-1] = '\0';

                strcpy(removed, str);
                // printf("folder: %s", removed);
                // printf("size: %d\n\n", strlen(removed));
            }
        }
    }
    fclose(fp1);
    fclose(fp2);
    remove(fname);
}

void cutAtChar(char *str, char c) {
    if (!str) return;

    while (*str != '0' && *str != c) str++;

    *str = '\0';
    return;
}

void processFileName(char *str) {
    int i = 0;
    int j = 0;
    int k = 0;

    while (str[i] != ';') {
        dest[k] = str[i];
        i++;
        k++;
    }
    dest[k] = '/';
    k++;
    i++;

    while(str[i] != ';') {
        petName[j] = str[i];
        dest[k] = str[i];
        j++;
        i++;
        k++;
    }

    dest[k] = '\0';
    petName[j] = '\0';

    i++;

    int l = 0;
    while(str[i] != '\0' && str[i] != '_' && str[i] != '.') {
        petAge[l] = str[i];
        i++;
        l++;
    }

    petAge[l] = '\0';

    FILE *ket;
    ket = fopen("keterangan.txt", "a");
    fprintf(ket, "nama : %s\n", petName);
    fprintf(ket, "umur : %s\n\n", petAge);
    fclose(ket);

// check pet kedua
    if (str[i] == '_') {
        need_dup = true;

        i++;
        int _j = 0, _k = 0, _l = 0;

        while (str[i] != ';') {
            secondPetType[_l] = str[i];
            i++;
            _l++;
        }
        i++;

        secondPetType[_l] = '\0';

        while (str[i] != ';') {
            secondPetName[_j] = str[i];
            _j++;
            i++;
        }
        secondPetName[_j] = '\0';
        i++;
        while (str[i] != '.') {
            secondPetAge[_k] = str[i];
            _k++;
            i++;
        }
        secondPetAge[_k] = '\0';

        FILE *ket;
        ket = fopen("keterangan.txt", "a");
        fprintf(ket, "nama : %s\n", secondPetName);
        fprintf(ket, "umur : %s\n\n", secondPetAge);
        fclose(ket);
    }
}

void deleteFiles();
void extractFiles();
void organizeFiles();
void checkFiles();
void makeSomeFolders();
void moveFiles();

void __makeSomeFolders();
void __makeSomeFoldersHelper();
void __moveFiles(char *fileName);
void __moveFilesHelper(char *fileName, char *folderDest, char *name);
void __createDuplicate(char *fileName);

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
        moveFiles();
    }
    else {
        while(wait(&status) > 0);
        char *argv[] = {"rm", "-f", "/home/krisna/modul2/petshop/jenis.txt", NULL};
        execv("/bin/rm", argv);
    }
}

void moveFiles() {
    pid_t child;
    child = fork();
    CHECK_FORK_SUCCESS(child)

    int status;

    if (child == 0) {
        checkFiles();
    }
    else {
        while(wait(&status) > 0);

        if (chdir("home/krisna/modul2/petshop/") < 0) exit(EXIT_FAILURE);


        DIR *d;
        struct dirent *dir;
        d = opendir(".");

        if (d) {
            while ((dir = readdir(d)) != NULL) {
                char *ret = strstr(dir->d_name, ".jpg");
                if (ret) {
                    __moveFiles(dir->d_name);
                }
            }
            closedir(d);
        }
        exit(EXIT_SUCCESS);
    }
}

void __moveFiles(char *fileName) {
    pid_t child;
    child = fork();
    CHECK_FORK_SUCCESS(child)

    int status;

    if (child == 0) {
        processFileName(fileName);
        __moveFilesHelper(fileName, dest, petName);
    }
    else {
        while(wait(&status) > 0);

        exit(EXIT_SUCCESS);
    }
}
void __moveFilesHelper(char *fileName, char *folderDest, char *pet) {
    pid_t child;
    child = fork();
    CHECK_FORK_SUCCESS(child)

    int status;

    if (child == 0) {
        if (need_dup) {
            need_dup = false;
            __createDuplicate(fileName);
        }
    }
    else {
        while(wait(&status) > 0);

        strcat(folderDest, ".jpg");

        char *argv[] = {"mv", "-f", fileName, folderDest, NULL};
        execv("/bin/mv", argv);
    }
}

void __createDuplicate(char *fileName) {
    pid_t child;
    child = fork();
    CHECK_FORK_SUCCESS(child)

    int status;

    if (child == 0) {

    }
    else {
        while(wait(&status) > 0);

        char d_dest[MAX];
        strcpy(d_dest, secondPetType);
        strcat(d_dest, "/");
        strcat(d_dest, secondPetName);
        strcat(d_dest, ".jpg");

        char *argv[] = {"cp", fileName, d_dest, NULL};
        execv("/bin/cp", argv);
    }
}

void checkFiles() {
    pid_t child;
    child = fork();
    CHECK_FORK_SUCCESS(child)

    int status;

    if (child == 0) {
        deleteFiles();
    }
    else {
        while(wait(&status) > 0);

        BST myBST;
        bst_init(&myBST);

        DIR *d;
        struct dirent *dir;
        d = opendir("/home/krisna/modul2/petshop/.");
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                    
                }
                else {
                    char temp[100];
                    strcpy(temp, dir->d_name);
                    
                    cutAtChar(temp, ';');
                    bst_insert(&myBST, temp);
                }
            }
            closedir(d);
        }
        bst_inorder(&myBST);

        makeSomeFolders();
        exit(EXIT_SUCCESS);
    }
}

void makeSomeFolders() {
    int N = 15;
    int i=0;

    int status;
    
    for (int i = 0; i < N; i++) {
        pid_t child;
        child = fork();
        CHECK_FORK_SUCCESS(child)
        
        if (child == 0) {
            __makeSomeFolders();
            __makeSomeFoldersHelper();
        }
        else {
            while(wait(&status) > 0);
        }
    }
}

void __makeSomeFolders() {
    pid_t child;
    child = fork();
    CHECK_FORK_SUCCESS(child)

    if (chdir("home/krisna/modul2/petshop/") < 0) exit(EXIT_FAILURE);

    int status;

    if (child == 0) {
        removeLine();
        int len = strlen(removed);

        if (len != 0) {
            char *argv[] = {"mkdir", "-p", removed, NULL};
            execv("/bin/mkdir", argv);
        } 
    }
    else {
        while(wait(&status) > 0);
    }
}
void __makeSomeFoldersHelper() {
    pid_t child;
    child = fork();
    CHECK_FORK_SUCCESS(child)

    int status;

    if (child == 0) {
        char *argv[] = {"mv", temp, fname, NULL};
        execv("/bin/mv", argv);    
    }
    else {
        while(wait(&status) > 0);
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
