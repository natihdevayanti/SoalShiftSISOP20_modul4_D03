# SoalShiftSISOP20_modul4_D03


**Dibuat oleh:**

Kelompok D03

Putu Putri Natih Devayanti (05111840000163)

Elvira Catrine Natalie (05111840000016)



## SOAL 1

### Enkripsi Versi 1

a. Jika sebuah direktori dibuat dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.

b. Jika sebuah direktori di-rename dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.

c. Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi adirektori tersebut akan terdekrip.

d. Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.

e. Semua file yang berada dalam direktori ter enkripsi menggunakan caesar cipher dengan key. 9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO

f. Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lainnya yang ada didalamnya.

> Langkah 

- Digunakan character sebagai berikut untuk menyimpan character key yang digunakan, yaitu 
```
*characterlist = {"9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO"};
```

- Character  memiliki panjang 87 karakter dan menggunakan 10 key, sehingga apabila dienkrip akan digeser ke kanan sebanyak 10 karakter dan jika didekrip akan digeser ke kiri sebanyak 10 karakter. 

- Pada fungsi enkrip dan dekrip akan melakukan looping untuk membaca dan mengubah nama folder atau file. Misalkan apabila mkdir rahasia/encv1_coba, maka yg di enkripsi hanya string encv1_coba saja, rahasia/ tidak akan di enkripsi.

- Untuk mengetahui batas looping tersebut, `fungsi miring` dan `check_ekstensi` yang akan mengembalikan indeks ekstensi dan index tanda miring sebagai penanda awal dan akhir enkripsi dan dekripsi.

Code cek tanda miring:

```
int miring(char* path, int num){
	int length_path = strlen(path);
	for(int i=0; i<length_path; i++){
		if(path[i] == '/'){
			return i + 1;
		}
	}
	return num;
}
```

Code cek ekstensi:

```
int check_ekstensi(char* path){
	int length_path = strlen(path);

	for(int i=length_path-1; i >=0; i--){
		if(path[i] == '.') return i;
	}
	return length_path;
}
```

Code Enkripsi Versi 1: 

```
void encrypt(char *save_path)
{
	int curr = 0, i, j;
	if (!strcmp(save_path, ".") || !strcmp(save_path, ".."))
		return;

	int akhir = check_ekstensi(save_path);
	int mulai = miring(save_path, 0);

	for (i = mulai; i < akhir; i++)
	{
		if(save_path[i] != '/'){
			for (j = 0; j < jum_kode; j++)
			{
				if (save_path[i] == characterlist[j])
				{
					curr = (j + key) % jum_kode;
					save_path[i] = characterlist[curr];
					break;
				}
			}
		}
	}
}
```

Code dekripsi: 

```

void decrypt(char *save_path)
{
	int curr = 0, i, j;
	if(!strcmp(save_path, ".") || !strcmp(save_path, ".."))
		return;

	int akhir = check_ekstensi(save_path);
	int mulai = miring(save_path, akhir);

	for (i = mulai; i < akhir; i++)
	{
		if(save_path[i] != '/'){
			for (j = 0; j < jum_kode; j++)
			{
				if (save_path[i] == characterlist[j])
				{
					curr = (j + (jum_kode - key)) % jum_kode;
					save_path[i] = characterlist[curr];
					break;
				}
			}
		}
	}
}

```

Keterangan : 

- Variable save_path merupakan string yang akan dienkripsi, jika save_path ke-i sama dengan key ke-j, maka variable save_path ke-i akan dibah menjadi key ke (j+17) % 87.

- `if(save_path[i] != '/')` digunakan fungsi sebagai berikut untuk mengabaikan jika bertemu tanda / sesuai keterangan pada soal

- Pemanggilan fungsi dekripsi ini dilakukan di utility functions getattr, mkdir, rename, rmdir, dan fungsi-fungsi lainnya. Akan terdapat pengecekaan string `"encv1_"` pada path di masing-masing utility function dengan menggunakan fungsi `strstr()`. Dengan adanya fungsi ini, maka fungsi enkripsi dan dekripsi akan dipanggil untuk string tersebut dengan `"encv1_"` sebagai batasan awal mulai stringnya. 


## SOAL 2

### Enkripsi Versi 2

a. Jika sebuah direktori dibuat dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.

b. Jika sebuah direktori di-rename dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.

c. Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi direktori tersebut akan terdekrip.

d. Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.

e. Pada enkripsi v2, file-file pada direktori asli akan menjadi bagian-bagian kecil sebesar 1024 bytes dan menjadi normal ketika diakses melalui filesystem rancangan jasir. Sebagai contoh, file File_Contoh.txt berukuran 5 kB pada direktori asli akan menjadi 5 file kecil yakni: File_Contoh.txt.000, File_Contoh.txt.001, File_Contoh.txt.002, File_Contoh.txt.003, dan File_Contoh.txt.004.

f. Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lain yang ada didalam direktori tersebut (rekursif). 

> Langkah 

- Code Enkripsi Versi 2:

```
void encrypt2(char *path)
{
	chdir(path);
    int status;
    char path1[1000],path2[1000];
	DIR *d;
    d = opendir(".");
    struct dirent *dir;
	struct stat check;
    if (d){
        while ((dir = readdir(d)) != NULL){
			if (stat(dir->d_name, &check) < 0);
			else if (S_ISDIR(check.st_mode)){
				if (strcmp(dir->d_name,".")==0 || strcmp(dir->d_name,"..")==0) continue;
				
				sprintf(path1,"%s/%s",path, dir->d_name);
				encrypt2(path1);
			}else{
				sprintf(path2,"%s/%s",path, dir->d_name);
				sprintf(path1,"%s.",path2);
				pid_t child_id;
				child_id = fork();
				if (child_id == 0){
					pid_t child = fork();
					if (child ==0){
						char* argv[] = {"split","-b","1024","-a","3","-d",path2,path1, NULL};
						execv("/usr/bin/split",argv);
					}else {
						while ((wait(&status))>0);
						char *argv[]={"rm",path2, NULL};
						execv("/bin/rm",argv);
					}	
				}
			}
		}
    }
	return;
}

```


- Code Dekripsi versi 2:

```
void decrypt2(char *dir)
{
	DIR *dp;
	struct dirent *de;
	struct stat check;
	dp = opendir(dir);
	char dirpath[1000], path_file[1000],fpath[1000];
    int hitung=0;
    if (dp == NULL) return;
	while ((de = readdir(dp)) != NULL){
		if (stat(de->d_name, &check) < 0);
		if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
		if (S_ISDIR(check.st_mode)){
			sprintf(dirpath, "%s/%s", dir, de->d_name);
			decrypt2(dirpath);
		}else if (!S_ISDIR(check.st_mode)){
			sprintf(path_file, "%s/%s", dir, de->d_name);
			path_file[strlen(path_file) - 4] = '\0';
			FILE *check = fopen(path_file, "r");
			if (check != NULL) return;
			FILE *file = fopen(path_file, "w");
			sprintf(fpath, "%s.%03d", path_file, hitung);
			void *buffer = malloc(1024);
			while (1){
				FILE *opn = fopen(fpath, "rb");
				if (opn == NULL) break;
				size_t readSize = fread(buffer, 1, 1024, opn);
				fwrite(buffer, 1, readSize, file);
				fclose(opn);
				remove(fpath);
				hitung++;
				sprintf(fpath, "%s.%03d", path_file, hitung);
			}
			free(buffer);
			fclose(file);
		}
	}
	closedir(dp);
}
```

Keterangan	:

- Untuk masing-masing fungsi dibagi 2 kondisi, yaitu kondisi ketika menemukan folder dan menemukan file.

- Kita lakukan pengecekan file-file yang terdapat direktori yang telah kita rename. Jika ternyata yang sedang kita cek adalah direktori maka ikuti kondisi `if (S_ISDIR(check.st_mode))`. Kita akan melakukan rekursi dengan memanggil fungsi `encrypt2` dalam fungsi encrypt2 atau fungsi `decrypt2` dalam fungsi decrypt2 kembali. 

- Untuk fungsi enkripsi, ketika kita menemukan file yang bukan direktori, maka lakukan pemecahbelahan file dengan fungsi split `char* argv[] = {"split","-b","1024","-a","3","-d",path2,path1, NULL};`.

- Dalam kondisi saat kita menemukan file, maka lakukan pembuatan file yang tanpa ekstensi yang telah kita tambahkan saat enkripsi. Kemudian dengan fungsi `hitung` kita cek file tersebut dari 000 hingga akhir. 

- Dalam fungsi dekripsi, lakukan free buffer dan tutup filenya setelah n sub-file sudah terbentuk sesuai ukuran file asli.



## SOAL 3

### Sinkronisasi Direktori Otomatis

Misalkan suatu directory bernama dir akan tersinkronisasi dengan directory yang memiliki nama yang sama dengan awalan sync_ yaitu sync_dir. Persyaratan untuk sinkronisasi yaitu:

- Kedua directory memiliki parent directory yang sama.

- Kedua directory kosong atau memiliki isi yang sama. Dua directory dapat dikatakan memiliki isi yang sama jika memenuhi: ⋅⋅* Nama dari setiap berkas di dalamnya sama. ⋅⋅* Modified time dari setiap berkas di dalamnya tidak berselisih lebih dari 0.1 detik.

- Sinkronisasi dilakukan ke seluruh isi dari kedua directory tersebut, tidak hanya di satu child directory saja.

- Sinkronisasi mencakup pembuatan berkas/directory, penghapusan berkas/directory, dan pengubahan berkas/directory. Jika persyaratan di atas terlanggar, maka kedua directory tersebut tidak akan tersinkronisasi lagi.

> Langkah

Code untuk mengembalikan direktori folder "sync_" : 

```
char *cekSync(char *dir_name, char *curr_sync)
{	
	if(strstr(dir_name, syncron) == NULL){
		sprintf(curr_sync, "%s/%s%s", dirpath, syncron, dir_name);
		DIR *dir;
		dir = opendir(curr_sync);

		if(dir == NULL) return NULL;
		else return curr_sync;
		closedir(dir);
	}else{
		char real[1000];
		int i, j = 0;
		for(i=strlen(syncron)-1; i<strlen(dir_name); i++){
			real[j] = dir_name[i];
			j++;
		}
		real[j] = '\0';

		sprintf(curr_sync, "%s/%s", dirpath, real);

		DIR *dir;
		dir = opendir(curr_sync);

		if(dir == NULL) return NULL;
        else return curr_sync;


		closedir(dir);
	}
}
```

Code untuk memberikan permission pada file :

```
void permission_sync(char *curr_sync, char *first_path)
{
	char buffer[4096];
	DIR *dir;
	FILE *first_file, *file_sync;
	struct dirent *dp;
	dir = opendir(first_path);

	while((dp = readdir(dir)) != NULL){
		char source[1000], destination[1000];
		sprintf(source, "%s/%s", first_path, dp->d_name);
		sprintf(destination, "%s/%s", curr_sync, dp->d_name);
		first_file = fopen(source, "r");
		file_sync = fopen(destination, "w");
		size_t num;
		while((num = fread(buffer, 1, sizeof(buffer), first_file)) > 0)
			fwrite(buffer, 1, num, file_sync);
	}

	closedir(dir);
}
```

Keterangan 	:

- Jika terdeteksi terdapat suatu folder dengan awalan `"sync_"`, maka isi dari folder aslinya akan di-copy ke folder "sync_" tersebut. 

- Pendeteksian kondisi jika nama folder berisi awalan `"sync_"` atau tidak dilakukan di fungsi `readdir()` dan `mkdir()`

- Nantinya setelah menjalankan fungsi `*cekSync` untuk mengembalikan direktori `"sync_"` jika yang di-passing ke fungsinya adalah folder asli, untuk selain nilai NULL, kita jalankan fungsi `permission_sync` untuk memberi permission read `first_file = fopen(source, "r");` untuk file asli dan permission write `file_sync = fopen(destination, "w");`  untuk file `"sync_"`.



## SOAL 4

### Sinkronisasi Direktori Otomatis

a. Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori home pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan. 

b. Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu INFO dan WARNING.

c. Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink.

d. Sisanya, akan dicatat dengan level INFO.

e.Format untuk logging yaitu: [LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]

> Langkah

- Kita membagi 2 fungsi untuk permasalahan ini, yaitu untuk log level WARNING dan juga INFO. 

```
void Finfo (char *string, char *path){
    char* info = "INFO";
    char log[1000];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(log, "%s::%02d%02d%02d-%02d:%02d:%02d::%s::%s", info, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, string, path);
	FILE *fl = fopen("/home/natih/fs.log", "a");  
   fprintf(fl, "%s\n", log);  
    fclose(fl);  
    return;
}
```

```
void Fwarning (char *string, char *path)
{
    char* info = "WARNING";
    char log[1000];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(log, "%s::%02d%02d%02d-%02d:%02d:%02d::%s::%s", info, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, string, path);
	FILE *fl = fopen("/home/natih/fs.log", "a");
    fprintf(fl, "%s\n", log);  
    fclose(fl);  
    return;
}
```

Keterangan 	:

- Untuk membuka log file maka menggunakan `FILE *fl = fopen("/home/natih/fs.log", "a")` yang nantinya akan berisi perintah system call yang dijalankan. Mode "a" menandakan bahwa file fs.log hanya untuk ditulis.

- Format untuk logging ini berisi time (tahun, bulan, hari, jam, menit, detik) diketahui menggunakan `t_time t`. 

- `fprintf(fl, "%s\n", log)` untuk menuliskan format logging WARNING ataupun INFO ke log file.

- Kemudian, kita akan memanggil kedua fungsi ini, di setiap fungsi yang ada dalam fuse sesuai dengan perintah. Contohnya adalah pada utilities function mkdir().

```
static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
	char fpath[1000];
	char *a = strstr(path, encv1);

	if(a != NULL){
		decrypt(a);
	}

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}
	char *temp = strrchr(fpath, '/');

	char *b = strstr(temp, syncron);

	if(b != NULL){
		char temp1[1000];
		char *first_path = cekSync(temp+1, temp1);

		if(first_path != NULL){
			char curr_sync[1000];
			sprintf(curr_sync, "%s", fpath);
			
			permission_sync(curr_sync, first_path);
		}
	}
```

**TERIMA KASIH**

