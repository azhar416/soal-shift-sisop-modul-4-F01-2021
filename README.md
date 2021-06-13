# soal-shift-sisop-modul-4-F01-2021

## SOAL 1
Di suatu jurusan, terdapat admin lab baru yang super duper gabut, ia bernama Sin. Sin baru menjadi admin di lab tersebut selama 1 bulan. Selama sebulan tersebut ia bertemu orang-orang hebat di lab tersebut, salah satunya yaitu Sei. Sei dan Sin akhirnya berteman baik. Karena belakangan ini sedang ramai tentang kasus keamanan data, mereka berniat membuat filesystem dengan metode encode yang mutakhir. Berikut adalah filesystem rancangan Sin dan Sei :

### A Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

### B Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

### C Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.


untuk persoalan A dan B diminta untuk melakukan encoding directory dan file dengan atBash Cipher.
sementara persoalan C diminta untuk melakukan decoding directory dan file dengan atBash Cipher.

untuk ketiga permasalahan tersebut, dapat menggunakan algoritma atBash Cipher. tetapi kita harus mengambil path dan nama filenya terlebih dahulu dan cek apakah directory tersebut berawalan `AtoZ_`, jika iya maka directory tersebut akan di encode dengan atBash Cipher dan jika tidak maka directory tersebut akan langsung di copy ke path directory mount.

```C++
char fpath[1000];
bzero(fpath, 1000);
int i, cekCipher = 0;

char* t;
if (strcmp(path, "/") != 0) {
    t = strstr(path, "/AtoZ_");
    if (t) {
        cekCipher = 1;
        t++;
    }
}

if(strcmp(path, "/") == 0)
{
    path = dirpath;
    sprintf(fpath,"%s",path);
} 
else if (cekCipher) 
{
    char befciper[1024];
    bzero(befciper, 1024);
    strncpy(befciper, path, strlen(path) - strlen(t));

    char x[1024]; strcpy(x, t);
    char* token;
    char* s = x;
    
    char temp[1024];
    i = 0;
    while ((token = strtok_r(s, "/", &s))) 
    {
        bzero(temp, 1024);
        if(i == 0) 
        {
            strcpy(temp, token);
            strcat(befciper, temp);
            
            i = 1;
            continue;
        }

        char str[1024];
        strcpy(str, token);
        int i;
        for(i = 0; i < strlen(str); i++) 
        {
            if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = 'Z' - (str[i] - 'A');
            }
            else if (str[i] >= 'a' && str[i] <= 'z') {
                str[i] = 'z' - (str[i] - 'a');
            }
        }
        char* res = str;

        strcpy(temp, res);
        strcat(befciper, "/");
        strcat(befciper, temp);
    }
    sprintf(fpath, "%s%s", dirpath, befciper); 
}
else sprintf(fpath, "%s%s",dirpath,path);
```

untuk algoritma atBash Cipher
```C++
.
.
.
char str[1024];
strcpy(str, token);
int i;
for(i = 0; i < strlen(str); i++) 
{
    if (str[i] >= 'A' && str[i] <= 'Z') {
    str[i] = 'Z' - (str[i] - 'A');
    }
    else if (str[i] >= 'a' && str[i] <= 'z') {
        str[i] = 'z' - (str[i] - 'a');
    }
}
char* res = str;
.
.
.
```

### D Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]

untuk persoalan D, kita diminta untuk membuat log setiap pembuatan directory dan juga rename directory dengan awalan `AtoZ_`.

untuk log pada fungsi `mkdir`
```C++
char* hadeuh = strchr(path, '/');
if (strstr(hadeuh, "AtoZ_"))
{
    char path_log[1024];

    FILE* file = fopen("inilog.log", "a");
    char iyedah[10000];
    sprintf(iyedah, "MKDIR : %s", path_log);
    fprintf(file, "%s\n", iyedah);

    fclose(file);
}
```

untuk log pada fungsi `rename`
```C++
char* hadeuh = strchr(to, '/');
if (strstr(hadeuh, "AtoZ_"))
{
    char ke[1024];
    char dari[1024];
    sprintf(dari, "%s%s", dirpath, from);
    sprintf(ke, "%s%s", dirpath, to);

    FILE* file = fopen("inilog.log", "a");
    char iyedah[10000];
    sprintf(iyedah, "RENAME : %s ==> %s", dari, ke);
    fprintf(file, "%s\n", iyedah);

    fclose(file);
}
```

### E. Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

untuk persoalan E, jika di dalam directory terdapat directory lagi maka harus di cek. dalam kata lain pengecekan directory yg harus di encode dan decode dilakukan secara recursive.

untuk implementasinya dapat langsung di cek pada source code
[Code](https://github.com/azhar416/soal-shift-sisop-modul-4-F01-2021/blob/main/SINSEIFS.c)

## 4. Untuk memudahkan dalam memonitor kegiatan pada filesystem mereka Sin dan Sei membuat sebuah log system dengan spesifikasi sebagai berikut.
### A. Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.
### B. Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.
### C. Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.
### D. Sisanya, akan dicatat pada level INFO.
### E. Format untuk logging yaitu: [Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]

untuk menyelesaikan persoalan nomer 4 ini, kami menggunakan fungsi untuk membuat log tersebut. 

```C++
void systemlog(char* level, char* cmd, int desctotal, const char* desc[])
{
    FILE* file = fopen(sysLog, "a");

    time_t now;
    time(&now);

    struct tm* t = localtime(&now);
    fprintf(file, "%s::%s::%02d%02d%04d-%02d:%02d:%02d", level, cmd, t->tm_mday, t->tm_mon+1, t->tm_year+1900, t->tm_hour, t->tm_min, t->tm_sec);
    for (int i = 0; i < desctotal; i++)
    {
        fprintf(file, "::%s", desc[i]);
    }
    fprintf(file, "\n");
    fclose(file);
}
```

untuk cara memanggilnya
```C++
    const char *desc[] = {path};
    systemlog("WARNING","RMDIR", 1, desc);
    return 0;

    const char* desc[] = {path};
    systemlog("INFO", "READDIR", 1, desc);
    return 0;

    const char* desc[] = {path}; 
    systemlog("INFO", "READ", 1, desc);
    return res;
    
    const char* desc[] = {path}; 
    systemlog("INFO", "GETATTR", 1, desc);
    return 0;

    const char* desc[] = {path}; 
    systemlog("INFO", "MKDIR", 1, desc);
    return 0;

    const char* desc[] = {from, to}; 
    systemlog("INFO", "RENAME", 2, desc);
    return 0;

    const char *desc[] = {path};
    systemlog("WARNING","UNLINK", 1, desc);
    return 0;
```    
fungsi tersebut dipanggil sebelum fungsi `xmp_` melakukan return value.