# soal-shift-sisop-modul-2-A05-2021

## Soal 1
#### A. Deskripsi Singkat Soal
Pada hari ulang tahun Stevany, Steven ingin memberikan Stevany sebuah file _zip_ yang berisikan hal-hal yang disukai Stevany.
Steven ingin isi _zip_-nya menjadi rapi dengan membuat folder untuk masing-masing file sesuai extensi.
* (a) Steven ingin nama foldernya adalah Musyik untuk mp3, Fylm untuk mp4, dan Pyoto untuk jpg.
* (b) Steven mendownload foto, musik, dan film dari link yang terlampir di poin D di bawah.
* (c) Steven tidak ingin isi folder yang dibuatnya berisikan zip, sehingga Anda perlu meng-_extract_-nya setelah mendownloadnya.
* (d) Lalu, Anda memindahkannya ke dalam folder yang telah dibuat (hanya file yang dimasukkan).
* (e) Untuk memudahkan Steven, ia ingin semua hal di atas berjalan otomatis saat 6 jam sebelum ulang tahun Stevany.
* (f) Setelah itu, pada saat ulang tahun Stevany, semua folder akan di-_zip_ dengan nama *Lopyu_Stevany.zip* dan semua folder akan di delete sehingga hanya menyisakan *Lopyu_Stevany.zip*

#### B. Note
* Ulang Tahun Stevany: 09 April 2021 Pukul 22.22 WIB

#### C. Batasan
* Semua dijalankan oleh satu script di latar belakang
* Tidak boleh menggunakan fungsi `system`, `mkdir`, dan `rename`
* Tidak boleh menggunakan fitur CRON
* Gunakan fungsi `fork` dan `exec`

#### D. Link Download File
* Foto : https://drive.google.com/file/d/1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD/view
* Musik : https://drive.google.com/file/d/1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J/view
* Film : https://drive.google.com/file/d/1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp/view

#### E. Solusi
Ulang tahun Stevany adalah 09 April 2021 22.22.00 WIB. Ulang tahun Stevany dalam Unix Timestamp adalah 1617981720.
<figure>
  <img src="https://user-images.githubusercontent.com/8071604/114817777-cfd6d780-9de4-11eb-9782-0b9977ad38b6.png" alt="Epoch Ulang Tahun Stevany">
  <figcaption>Sumber: epochconverter.com</figcaption>
</figure>

6 jam sebelum ulang tahun Stevany dalam Unix Timestamp adalah 1617960120.

<figure>
  <img src="https://user-images.githubusercontent.com/8071604/114818253-aff3e380-9de5-11eb-9a9d-a4b59b2b687d.png" alt="Epoch 6 Jam Sebelum Ulang Tahun Stevany">
  <figcaption>Sumber: epochconverter.com</figcaption>
</figure>

Unix Timestamp digunakan untuk mengecek apakah waktu ulang tahun telah tercapai dengan menggunakan loop _while_. Berikut ini _pseudocode_-nya
```
state = 0
while True
  timestamp = get_current_timestamp
  if timestamp >= birthday_unix_timestamp && state == 1
    zip_birthday_asset()
    break
  else if timestamp >= six_hour_before_birthday_unix_timestamp && state == 0
    state = 1
    download_birthday_asset()
  else {
    sleep 1
  }
}
```

```
NOTE: Setiap kali proses akan melakukan perintah `exec`, sebuah process child harus di spawn.
```
##### E.1 Download asset dari Google Drive
Pada dasarnya, setiap _shared folder_ di Google Drive mempunyai sebuah _id_. Command `wget` hanya dapat digunakan untuk _direct download link_ saja. Oleh karena itu, perlu dibuat sebuah format _url_ yang digunakan sebagai url _direct download_-nya. _Direct download link_ yang digunakan adalah

```
https://drive.google.com/uc?id=[id drive]&export=download
```
Keterangan:
* URL: https://drive.google.com/uc
* GET REQUEST:
  * id: ID google drive
  * export: value `download` untuk melakukan direct download

Perintah yang digunakan untuk melakukan download pada google drive dengan menggunakan `wget` adalah
```
wget --no-check-certificate -q [download link] -O [file name].zip
```
Keterangan:
* wget : perintah download
* --no-check-certificate : skip pengecekan certificate SSL/TLS
* -q : silence, tidak melakukan print log `wget` di terminal
* [download link] : Link direct download
* -O : Object file
* [file-name] : Nama zip file

Proses download asset dari Google Drive menggunakan child process karena dalam praktiknya di kode bahasa C, fungsi `exec` digunakan dan semua statement di bawah fungsi `exec` tidak akan tereksekusi. Berikut ini _pseudocode_-nya

```
child_id = fork()
if child_id < 0
  exit
if child_id == 0
  argv = {"wget", "--no-check-certificate", "-q", download_link, "-O", file_name, NULL}
  execv("/usr/bin/wget", argv)
```

##### E.2 Proses ekstrak file asset (zip) yang telah di download
Perintah `unzip` digunakan untuk melakukan ekstraksi file asset.
```
unzip -q [nama file].zip
```
Keterangan:
* unzip : nama perintah
* -q : parameter silence
* [nama file] : nama file yang telah didownload pada poin E.1

Perintah `unzip` perlu dilakukan oleh proses terpisah (child process) karena adanya penggunaan fungsi `exec`.

Setelah isi dari file _zip_ diekstrak, file zip harus dihapus dengan menggunakan command `rm`.

```
rm [nama file].zip
```
Keterangan:
* rm : perintah untuk menghapus file/folder
* [nama file] : nama file asset (zip) yang akan dihapus

Perintah `rm` juga perlu dilakukan oleh proses terpisah (child process) karena adanya penggunaan fungsi `exec`.

##### E.3 Proses Pemilahan
Setelah diekstrak, struktur file yang terjadi adalah sebagai berikut
- FILM
- MUSIK
- FOTO
- soal1.c
- soal1 (binary file)

Sebelumnya, folder dengan nama kesukaan Stevany harus dibuat dengan menggunakan command `mkdir`.
```
mkdir Fylm
mkdir Musyik
mkdir Pyoto
```

Selanjutnya, proses pemilahan dilakukan dengan cara:
1. Lakukan scan di dalam directory FILM dan pindahkan semua file berektensi .mp4 ke dalam folder Fylm
2. Lakukan scan di dalam directory MUSIK dan pindahkan semua file berektensi .mp3 ke dalam folder Musyik
3. Lakukan scan di dalam directory FOTO dan pindahkan semua file berektensi .jpg ke dalam folder Pyoto

Pemilahan dilakukan dengan menggunakan `#include <dirent.h>` dengan menyeleksi ektensi dari setiap file yang ada.

Di akhir operasi, folder FOTO tidak kosong.
Di deskripsi soal, tertera bahwa file berektensi .jpg saja yang masuk ke folder Pyoto.
Oleh karena itu, file yang tidak berektensi .jpg tidak dipindahkan ke folder Pyoto.

##### E.4 (Ulang Tahun Stevany tiba) Proses zip folder Fylm, Musyik, dan Pyoto
Proses zipping dilakukan dengan menggunakan command `zip`.

```
zip -qrm Lopyu_Stevany.zip Fylm Musyik Pyoto
```
Keterangan:
* zip: perintah zip
* -q : parameter silence
* -r : parameter zip folder secara recursive (seluruh isi dalam folder juga di zip)
* -m : parameter delete, hapus file setelah di zip
* Lopyu_Stevany.zip : nama file zip
* Fylm, Musyik, Pyoto : nama folder-folder yang akan di zip

#### F. Kesulitan
Kesulitan yang dihadapi oleh praktikan selama mengerjakan soal nomor 1 adalah
1. Proses debugging
Proses debugging ketika program tidak berjalan sesuai dengan keinginan lumayan kompleks karena di dalam proses programmnya banyak terjadi banyak hal: zip file, wget dari google drive, banyak fungsi `fork()` dan `wait()` terpanggil.

#### G. Screenshot

![image](https://user-images.githubusercontent.com/8071604/115100379-654da500-9f66-11eb-8979-055e618e3b60.png)
![image](https://user-images.githubusercontent.com/8071604/115100385-77c7de80-9f66-11eb-94db-c588bf4190ef.png)
![image](https://user-images.githubusercontent.com/8071604/115100392-82827380-9f66-11eb-842f-2d22f8bdc522.png)
![image](https://user-images.githubusercontent.com/8071604/115100399-87472780-9f66-11eb-930e-6f2ff1a988e8.png)
![image](https://user-images.githubusercontent.com/8071604/115100402-8e6e3580-9f66-11eb-9811-00bbc883c9bd.png)

## Soal 2
* ### setup program `soal2.c`
  * menggunakan Daemon
  * membuat beberapa macro definition `#define Directive` untuk memudahkan
  ```c
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
  ```
* ### 2a
  >extract `pets.zip` ke `/home/[user]/modul2/petshop` dan hapus file/folder yang bukan gambar pets
  Proses pengerjaan soal 2 bagian a ini dapat secara garis besar adalah sebagai berikut
  * buatlah sebuah fungsi `extractFiles()` yang menggunakan `fork()` untuk membuat proses lain
  * proses `child`-nya digunakan untuk membuat folder baru di `/home/[user]/modul2/petshop` dengan `execv`. Jelasnya sebagai berikut
    ```c
      char *argv[] = {"mkdir", "-p", PATH, NULL};
      execv("/bin/mkdir", argv);
    ```
  * proses `parent`-nya melalukan wait hingga folder sudah dibuat, kemudian melakukan extraction file `pets.zip` ke folder yang baru dibuat
    ```c
      while(wait(&status) > 0);
      char *argv[] = {"unzip", "-d", PATH, "/home/krisna/Documents/Code/Modul 2/SoalShift2/pets.zip", NULL};
      execv("/bin/unzip", argv);
    ```
  setelah itu, penghapusan file/folder tidak penting dapat dilakukan dengan membuat fungsi `deleteFiles()` dimana terdapat `fork()` juga yang memanggil `rm` melalui `execv()` 
* ### 2b
  >buatlah folder-folder berdasakan jenis tiap pets
  Pertama-tama, tiap filename dari tiap gambar harus di proses untuk mendapatkan apa saja jenis pets yang ada
  * bacalah nama file yang ada pada direktori
  ```c
    DIR *d;
    struct dirent *dir;
    d = opendir("/home/krisna/modul2/petshop/.");
  ```
  * copy tiap char
  * berhenti ketika bertemu `;`
  * simpan sebagai string yang berisi jenis pets
  ```c
    void cutAtChar(char *str, char c) {
      if (!str) return;

      while (*str != '0' && *str != c) str++;

      *str = '\0';
      return;
    }
  ```
  Kemudian, tiap string hasil `string processing` sebelumnya disimpan ke dalam `Unique Binary Search Tree`. Hal ini berguna untuk
  * menyimpan jenis pets
  * memastikan tidak ada jenis pets yang duplikat
  Terakhir, gunakan `fork()` dan `execv()` untuk membuat folder jenis pets berdasarkan data yang ada. `execv()` akan memanggil `/bin/mkdir/`
* ### 2c
  >pindahkan file foto pets ke folder berdasarkan kategorinya
  * cari file .jpg yang perlu dipindahkan
  ```c
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
  ```
  * buat fungsi `__moveFiles(char *fileName)` untuk membantu memindahan file. Fungsi tersebut akan memanggil fungsi `processFileName(char *fileName)` yang akan memproses nama file sehingga dapat ditentukan akan kemana foto pets tersebut akan dipindahkan. Fungsi tersebut juga akan bermanfaat untuk mengerjakan soal **2e**.
  ```c
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
  ```
  * melalui `__moveFiles`, akan dipanggil fungsi `__moveFilesHelper` yang akan memindahkan file. Fungsi ini menerima nama file yang akan dipindahkan dan tujuan pemindahan. Fungsi ini juga akan memanggil `__createDuplicate` yang akan digunakan untuk menyelesaikan soal **2d**
  ```c
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
  ```
* ### soal 2d
  >jika terdapat dua pets dalam satu foto, duplikasikan file tersebut dan kategorikan filenya sesuai jenis pets
  Seperti yang disebutkan sebelumnya, soal ini menggunakan fungsi `__createDuplicate`.
  * Perbedaan utama dengan pengerjaan **2c** adalah penggunaan `cp` untuk men-copy file dan bukan `mv`.
  * Fungsi ini dieksekusi sebelum `__moveFilesHelper` selesai memindahkan file.
  * string `secondPetType` dan `secondPetName` dihasilkan dari fungsi `processFileName`.
  * Terdapat variabel boolean `need_dup` yang akan di set sebagai `true` apabila `processFileName` menemukan adanya `_` pada nama file yang sesuai deskripsi soal mengindikasikan ada dua pets pada satu file foto.
  ```c
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
  ```
* ### soal 2e
  >pada tiap folder kategori pets, buatlah `keterangan.txt` yang berisi nama dan umur pets sesuai format yang diberikan
  
  Pembuatan file `keterangan.txt` terjadi pada fungsi `processFileName`
  ```c
    FILE *ket;
    ket = fopen(destKet, "a");
    fprintf(ket, "nama : %s\n", petName);
    fprintf(ket, "umur : %s\n\n", petAge);
    fclose(ket);
  ```
  * menggunakan fungsi `fopen` dengan mode `a` untuk menandakan append
  * variable string `destKet` merupakan hasil `strcat` jenis pet dengan "keterangan.txt"
  * varaible string `petName` dan `petAge` diperoleh dari string processing
  * untuk file dengan dua pets, proses serupa, hanya saja tipe, nama, dan umur pets dibaca mulai dari '_'

* ### hasil run program `soal2.c`
  * direktori `/home/[user]/modul2/petshop` 
  
  ![image](https://user-images.githubusercontent.com/43901559/115546124-d971ab00-a2ce-11eb-834c-e39bac5fb44a.png)
  * tampilan salah satu kategori pets - **dog**
  
  ![image](https://user-images.githubusercontent.com/43901559/115546313-1178ee00-a2cf-11eb-829a-cbc7f41f925c.png)
  * tampilan di folder **cat**. perhatikan bahwa terdapat file yang sama seperti di folder **dog**, hanya nama filenya adalah pets kategori **cat**
  
  ![image](https://user-images.githubusercontent.com/43901559/115546366-25245480-a2cf-11eb-98fb-e31fe95b66cd.png)
  * tampilan file `keterangan.txt` pada folder **chicken**
  
  ![image](https://user-images.githubusercontent.com/43901559/115546535-5a30a700-a2cf-11eb-8532-fbf3920cd66c.png)

## Soal 3
* Soal 3 dikerjakan menggunakan daemon.
* Tampilan pada while(1) daemon:
```c
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
```
* ### 3a
  Soal 3a diminta untuk membuat directory setiap 40 detik dengan format nama directory esuai timestamp [YYYY-mm-dd_HH:ii:ss].
    1. Pada langkah pertama kami membuat fungsi `ConstructTimeNow` yang digunakan untuk mendapatkan format waktu saat ini kemudian disimpan pada variable `folder_name`. Berikut isi dari fungsi `ConstructTimeNow`:
        ```c
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
        ```
    2. Setelah mendapatkan nama directory, kami membuat child proses yang digunakan untuk membuat directory dengan fungsi `__makeFolder` yang terdapat pada fungsi `phrase_3a`:
        ```c
        void phrase_3a(char *folder_name){
          __makeFolder(folder_name);
        }
       
        void __makeFolder(char *folder_name) {
          char *argv[] = {"mkdir", "-p", folder_name, NULL};
          execv("/bin/mkdir", argv);
        }
        ```
    3. Kemudian pada akhir while(1) kami sisipkan `sleep(40)` supaya program menunggu 40 detik untuk membuat directory yang baru.

* ### 3b
  Soal 3b diminta untuk mengisi directory yang sudah dibuat pada soal 3a dengan 10 gambar dimana 1 gambar diunduh setiap 5 detik dengan tiap gambar diberi format nama timestamp [YYYY-mm-dd_HH:ii:ss] serta ukuran gambar (n%1000) + 50 pixel dimana n adalah detik Epoch Unix.
    1. Langkah pertama tentusaja adalah membuat child proses yang bertujuan untuk mengunduh gambar, `phrase_3b` menyimpan seluruh source code untuk menyelesaikan soal 3b ini. 
    2. Pada `phrase_3b` terdapat fungsi `downloadImage` yang berguna untuk mendownload image
        ```c
        void phrase_3b(char *folder_name) {
          for (int j = 0; j < 10; j++) {
            downloadImage(folder_name);
            sleep(5);
          }
        }
        ```
        dan pada akhir for kami menyisipkan `sleep(5)` untuk menunggu selama 5 detik sebelum looping berikutnya.
    3. Pada `downloadImage` kami membuat child proses yang berguna untuk mengeksekusi `wget` dalam pengunduhan gambar, kami juga melakukan passing argumen `folder_name` yang menyimpan nama directory untuk menyimpan gambar.
        ```c
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
        ```
    4. Fungsi `ConstructDownloadLink` bertujuan untuk membuat download link pada variable `download_link` dan ukuran gambar yang diunduh bisa sesuai format yaitu (n%1000) + 50 pixel dimana n adalah detik Epoch Unix pada variable `size` serta nama dari file yang di download sesaui format yaitu timestamp [YYYY-mm-dd_HH:ii:ss] yang dimasukkan pada variable `name`
        ```c
        void ConstructDownloadLink(char *size ,char *link,char *name){
            sprintf(size,"%d",(int)time(NULL)%1000+50); 
            ConstructTimeNow(name);
            sprintf(link,"https://picsum.photos/%s",size);
        }
        ```
    5. Setelah link download terbentuk maka program akan menjalankan `wget`
        ```c
        char *argv[] = {"wget", "--no-check-certificate", "-q", download_link, "-O", path, NULL};
        execv("/usr/bin/wget", argv);
        ```

* ### 3c
  Soal 3c meminta setelah directory diisi penuh oleh 10 gambar ditambahkan juga file status.txt yang isinya adalah string "Download Success" yang di enkripsi menggunakan caesar chiper dengan shift 5 dan directory tersebut di zip dan directory sebelumnya dihapus.
    1. Solusi untuk memecahkan problem ini ada pada fungsi `phrase_3c` yang mana isinya adalah fungsi untuk membuat `status.txt` dan men zip lalu menghapus directory
        ```c
        void phrase_3c(char *folder_name) {
            __makestatus(folder_name);
            __zipping(folder_name);
        }
        ```
    2. Pada `__makestatus` terjadi passing argumen yaitu `folder_name` yang mana adalah nama directory yang 
