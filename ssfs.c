#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/xattr.h>
#include <sys/wait.h>
#include <pthread.h>
#include <pwd.h>
#include <grp.h>

char name_dir[200] = "/home/natih/Documents";
char characterlist[90] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
int key=10;
int jum_kode=87;
char encv1[20] = "/encv1_";
//char encv2[20] = "encv2_";

void decrypt(char *save_path)
{

  int length_path,i,x;
if(strcmp(save_path, ".") == 0 || strcmp(save_path, "..") == 0) return;
  for(x=0; save_path[x] != '\0'; x++)
  {
	length_path = save_path[x];
    if(length_path == '/') continue;
    for(i=0; i<jum_kode; i++)
    {
      if(length_path == characterlist[i]) break;
	}
      i-=key;
	if(i<0) i+=jum_kode;
	
	save_path[x]=characterlist[i];
   }
}

void encrypt(char *save_path)
{
   int length_path,i,x;
if(strcmp(save_path, ".") == 0 || strcmp(save_path, "..") == 0) return;
  for(x=0; save_path[x] != '\0'; x++)
  {
	length_path = save_path[x];
    if(length_path == '/') continue;
     for(i=0; i<jum_kode; i++)
    {
      if(length_path == characterlist[i]) break;
	}
      i+=key;
	if(i>jum_kode) i-=jum_kode;
	
	save_path[x]=characterlist[i];
   }
}

int case1 (char path[1000], int condition) {

    DIR *dp;
    struct dirent *de;
    dp = opendir(path);
    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {

        struct stat st;
        memset(&st, 0, sizeof(st));

        char name[100],temp1[100], temp2[100],directory[100];
        strcpy(name, de->d_name);
        sprintf(temp1, "%s/%s", path, name);
	//untuk folder
        if (de->d_type == 4 && strcmp(".", de->d_name) != 0 && strcmp("..", de->d_name) != 0 ){
            sprintf(directory, "%s/%s", path, name);

            if (condition == 1) {
                encrypt(name);
                case1(directory, 1);
            }
            else {
                decrypt(name);
                case1(directory, 0);
            }
            sprintf(temp2, "%s/%s", path, name);

            int res = rename(temp1, temp2);
	        if (res == -1) return -errno;
        }
        //untuk file
        if (de->d_type == 8) {

            char *ekstensi = strrchr(name, '.');
		//jika tak ada ekstensi
            if (ekstensi == NULL) {
                if (condition == 1) 
		encrypt(name);
                else decrypt(name);

                sprintf(temp2, "%s/%s", path, name);
            }
            else {
		//jika ada ekstensi
                char noekstensi[ekstensi - name];
                strncpy(noekstensi, name, ekstensi - name);
                noekstensi[ekstensi - name] = '\0';
                if (condition == 1) encrypt(noekstensi);
                else decrypt(noekstensi);
                
                sprintf(noekstensi, "%s%s", noekstensi, ekstensi);
                sprintf(temp2, "%s/%s", path, noekstensi);
            }
		//melakukan rename
            int res = rename(temp1, temp2);
	        if (res == -1) return -errno;
        }
    }

    closedir(dp);
    return 0;

}

static  int  xmp_getattr(const char *path, struct stat *stbuf)

{

int res;
char name[1000];
char fpath[1000];
char path1[1000];
sprintf(name, "%s", path);
strcpy (path1, name);

char length[1000];
	
	int i, j;

	for (i = 0; i < strlen(length); i++) {
		for (j = 0; j < jum_kode; j++) {
			if(length[i] == characterlist[j]) {
                if(j - 10 < 0) j=+jum_kode;
				length[i] = characterlist[(j-10)];
				break;
 			}
		}
	}
sprintf(fpath,"%s%s",name_dir,path1);

res = lstat(fpath, stbuf);

if (res == -1)

return -errno;

return 0;

}

  

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,

off_t offset, struct fuse_file_info *fi)

{

char fpath[1000];
char name[1000];
char test[1000];
sprintf(name, "%s", path);
//encrypt(name);

if(strcmp(path,"/") == 0)

{
path=name_dir;
sprintf(fpath,"%s",path);
}

//panggil encrypt
else {
encrypt(name);
sprintf(fpath, "%s%s",name_dir,test);
}
int res = 0;

DIR *dp;

struct dirent *de;

(void) offset;

(void) fi;

dp = opendir(fpath);

if (dp == NULL) return -errno;
  
/* 
while ((de = readdir(dp)) != NULL) {

struct stat st;

memset(&st, 0, sizeof(st));

st.st_ino = de->d_ino;

st.st_mode = de->d_type << 12;

res = (filler(buf, de->d_name, &st, 0));

if(res!=0) break;

}
*/

	while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        //res = (filler(buf, de->d_name, &st, 0));
        //if(res!=0) break;
        char enkripsi[1000];
strcpy(enkripsi, de->d_name);
	char nama_file[1000];
	sprintf(nama_file, "%s%s", fpath, enkripsi);
if(strcmp(enkripsi, ".") != 0 && strcmp(enkripsi, "..") != 0) encrypt(enkripsi);
res = (filler(buf, de->d_name, &st, 0));
        if(res!=0) break;
   }
closedir(dp);

return 0;

}

  
static int xmp_read(const char *path, char *buf, size_t size, off_t offset,

struct fuse_file_info *fi)

{

char fpath[1000];

if(strcmp(path,"/") == 0)

{

path=name_dir;

sprintf(fpath,"%s",path);

}

else sprintf(fpath, "%s%s",name_dir,path);

int res = 0;

int fd = 0 ;

  

(void) fi;

fd = open(fpath, O_RDONLY);

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
    char name[1000];
    //	sprintf(d_name1, "%s", path);
    sprintf(fpath, "%s%s", name_dir, name);
    res = mkdir(fpath, 0750); 
    if(res == -1)
        return -errno;
    return 0;
}
static int xmp_rename(const char *from, const char *to)
{
    
    int res;
     char go_to[1000];
    char go_from[1000];
   // char name[1000];
    //char compare1[1000];
   // char res1[1000];
    //res = rename(from, to);
  //  sprintf(name, "encv1_%s", to);
sprintf(go_to, "%s%s", name_dir, to);
    sprintf(go_from, "%s%s", name_dir, from);
   
     if (strncmp("/encv1_", to, 7) == 0){ 
        case1(go_from, 1);
}
    if (strncmp("/encv1_", from, 7) == 0){
        case1(go_from, 0);
} 
       res = rename(go_to, go_from);
        if(res == -1)
        return -errno;
    return 0;
}
 

static struct fuse_operations xmp_oper = {

.getattr = xmp_getattr,

.mkdir = xmp_mkdir,

.readdir = xmp_readdir,

.read = xmp_read,

.rename = xmp_rename,

};

  

int  main(int  argc, char *argv[])

{

umask(0);

return fuse_main(argc, argv, &xmp_oper, NULL);

}
