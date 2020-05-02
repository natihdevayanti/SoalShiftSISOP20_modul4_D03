#define FUSE_USE_VERSION 28

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <wait.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

static const char *dirpath = "/home/natih/Documents";
char *characterlist = {"9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO"};
int key = 10;
char encv1[10] = "encv1_";
char encv2[10] = "encv2_";
char *syncron = "sync_";
int jum_kode = 87;


//fungsi info
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
//fungsi warning 
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

int miring(char* path, int num){
	int length_path = strlen(path);
	for(int i=0; i<length_path; i++){
		if(path[i] == '/'){
			return i + 1;
		}
	}
	return num;
}

int check_ekstensi(char* path){
	int length_path = strlen(path);

	for(int i=length_path-1; i >=0; i--){
		if(path[i] == '.') return i;
	}
	return length_path;
}

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

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, encv1);
	if(a != NULL) decrypt(a);
	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}
  
	res = lstat(fpath, stbuf);
  Finfo ("LS",fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
					   off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
  
	char *a = strstr(path, encv1);
	if(a != NULL) decrypt(a);

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}
	
	Finfo ("CD", fpath);
	int res = 0;
	DIR *dp;
	struct dirent *de;

	(void)offset;
	(void)fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;
	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if(de->d_type == DT_DIR){
			char temp1[1000];
			char *curr_sync = cekSync(de->d_name, temp1);
			
			if(curr_sync != NULL){
				char temp2[1000];
				sprintf(temp2, "%s/", fpath);
				char *first_path = strcat(temp2, de->d_name);

				permission_sync(curr_sync, first_path);
	
			}
		}

		if(a != NULL) encrypt(de->d_name);
		res = (filler(buf, de->d_name, &st, 0));

		if (res != 0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
					struct fuse_file_info *fi)
{
  
	char fpath[1000];
	char *a = strstr(path, encv1);
	if(a != NULL)
		decrypt(a);

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}
 
	int res = 0;
	int fd = 0;

	(void)fi;

	fd = open(fpath, O_RDONLY);
	  Finfo ("CAT", fpath);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

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

	
	res = mkdir(fpath, mode);
Finfo ("MKDIR", fpath);
	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;
	char fpath[1000];
	char *a = strstr(path, encv1);
	if(a != NULL)
		decrypt(a);

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else sprintf(fpath, "%s%s", dirpath, path);
	

	res = rmdir(fpath);
Fwarning ("RMDIR", fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;
	char fpath[1000], path2[1000];
	char *a = strstr(to, encv1);
	if(a != NULL)
		decrypt(a);

	sprintf(fpath, "%s%s", dirpath, from);
	sprintf(path2, "%s%s", dirpath, to);

	

	res = rename(fpath, path2);

	if (res == -1)
		return -errno;
    Finfo ("RENAME", fpath);
    if (strstr(to, encv2) != NULL){
		encrypt2(path2);
	}

	if (strstr(from, encv2) != NULL && strstr(to, encv2) == NULL){
		decrypt2(path2);
	}

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
	char fpath[1000];
	char *a = strstr(path, encv1);
	if (a != NULL)
		decrypt(a);

	if (strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}

	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
					 off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
	char fpath[1000];
	char *a = strstr(path, encv1);
	if(a != NULL) decrypt(a);

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}
	(void)fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

	return res;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	(void)fi;

	int res;
	char fpath[1000];

	char *a = strstr(path, encv1);
	if(a != NULL)
		decrypt(a);

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}
    
	res = creat(fpath, mode);
    Finfo ("CREATE", fpath);
	if (res == -1)
		return -errno;

	close(res);

	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	char fpath[1000];

	char *a = strstr(path, encv1);
	if (a != NULL)
		decrypt(a);

	if (strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}

	struct timeval tv[2];

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_access(const char *path, int mask)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, encv1);
	if(a != NULL)
		decrypt(a);

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}

	res = access(fpath, mask);
	if (res == -1)
		return -errno;

	return 0;
}
static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
	char fpath[1000];
	char *a = strstr(path, encv1);
	if(a != NULL)
		decrypt(a);

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}


static int xmp_unlink(const char *path){
    int res;
    char fpath[1000];
    char *a = strstr(path, encv1);
	if(a != NULL)
		decrypt(a);

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	} else {
		sprintf(fpath, "%s%s", dirpath, path);
	}

  
    res = unlink(fpath);
  Fwarning ("REMOVE", fpath);
    if(res == -1){
        return -errno;
    }

    return 0;
}


static int xmp_readlink(const char *path, char *buf, size_t size)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, encv1);
	if (a != NULL)
		decrypt(a);

	if (strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}

	res = readlink(fpath, buf, size - 1);
	
	if (res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}


static int xmp_mknod(const char *path, mode_t mode, dev_t rdev) {
	int res;
	char fpath[1000];

	char *a = strstr(path, encv1);
	if(a != NULL)
		decrypt(a);

	if(strcmp(path, "/") == 0){
		path = dirpath;
		sprintf(fpath, "%s", path);
	}else{
		sprintf(fpath, "%s%s", dirpath, path);
	}
	// 'mknod(path, mode, rdev)' more portable
	if (S_ISREG(mode)) {
		res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(path, mode);
	else
		res = mknod(path, mode, rdev);

	if (res == -1)
		return -errno;

	return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr = xmp_getattr,
	.readdir = xmp_readdir,
	.read = xmp_read,
	.mkdir = xmp_mkdir,
	.rmdir = xmp_rmdir,
	.rename = xmp_rename,
	.truncate = xmp_truncate,
	.write = xmp_write,
	.create = xmp_create,
	.utimens = xmp_utimens,
	.access = xmp_access,
	.open = xmp_open,
    .unlink = xmp_unlink,
    .readlink = xmp_readlink,
	.mknod = xmp_mknod,
	
};
int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
