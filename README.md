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
* ### 2a
  >extract `pets.zip` ke `/home/[user]/modul2/petshop` dan hapus file/folder yang bukan gambar pets
  Proses pengerjaan soal 2 bagian a ini dapat secara garis besar adalah sebagai berikut
  * buatlah sebuah fungsi `extractFiles()` yang menggunakan `fork()` untuk membuat proses lain
  * proses `child`-nya digunakan untuk membuat folder baru di `/home/[user]/modul2/petshop` dengan `execv`. Jelasnya sebagai berikut
    <pre>
      char *argv[] = {"mkdir", "-p", PATH, NULL};
      execv("/bin/mkdir", argv);
    </pre>
  * proses `parent`-nya melalukan wait hingga folder sudah dibuat, kemudian melakukan extraction file `pets.zip` ke folder yang baru dibuat
    <pre>
      while(wait(&status) > 0);
      char *argv[] = {"unzip", "-d", PATH, "/home/krisna/Documents/Code/Modul 2/SoalShift2/pets.zip", NULL};
      execv("/bin/unzip", argv);
    </pre>
  setelah itu, penghapusan file/folder tidak penting dapat dilakukan dengan membuat fungsi `deleteFiles()` dimana terdapat `fork()` juga yang memanggil `rm` melalui `execv()` 
* ### 2b
  >buatlah folder untuk mengkategorikan jenis

## Soal 3
