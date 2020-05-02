# SoalShiftSISOP20_modul4_D03


**Dibuat oleh:**

Kelompok D03

Putu Putri Natih Devayanti (05111840000163)

Elvira Catrine Natalie (05111840000016)

##SOAL 1 - ENKRIPSI DAN DEKRIPSI VERSI 1##

Sebuah direktori akan menjadi direktori terenkripsi menggunakan metode enkripsi v1 dengan awalan encv1_

Dengan caesar chiper char characterlist[90] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";

```
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
```
```
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
```
