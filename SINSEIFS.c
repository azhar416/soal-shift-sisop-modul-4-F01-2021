#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
 
static const char *dirpath = "/home/azhar416/Downloads";
static const char *sysLog = "/home/azhar416/SinSeiFS.log";

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

static int xmp_rmdir(const char* path)
{
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

    int res = rmdir(fpath);
    if (res < 0)
        return -errno;

    const char *desc[] = {path};
    systemlog("WARNING","RMDIR", 1, desc);
    return 0;
}

/* XMP Field */
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
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
 
    int res = 0;
    DIR *dp;
    struct dirent *de;
 
    (void) offset;
    (void) fi;
 
    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;
 
    while ((de = readdir(dp)) != NULL) 
    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
 
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) 
        {
            res = (filler(buf, de->d_name, &st, 0));
        }
        else if (cekCipher) 
        {
            if (de->d_type & DT_DIR) {
                char temp[1024];
                bzero(temp, 1024);
                strcpy(temp, de->d_name);

                char str[1024];
                strcpy(str, temp);
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
                char* isi = str;
                
                strcpy(temp, isi);
                res = (filler(buf, temp, &st, 0));
            }
            else 
            {
                char* dot;
                dot = strchr(de->d_name, '.');
                
                char namaFile[1024];
                bzero(namaFile, 1024);
                if (dot) {
                    strncpy(namaFile, de->d_name, strlen(de->d_name) - strlen(dot));
                    
                    char str[1024];
                    strcpy(str, namaFile);
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
                    char* isi2 = str;

                    strcpy(namaFile, isi2);
                    strcat(namaFile, dot);
                }
                else 
                {
                    strcpy(namaFile, de->d_name);

                    char str[1024];
                    strcpy(str, namaFile);
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
                    char* isi3 = str;

                    strcpy(namaFile, isi3);
                }
                res = (filler(buf, namaFile, &st, 0));
            }
        }
        else 
        {
            res = (filler(buf, de->d_name, &st, 0));
        }
 
        if(res!=0) break;
    }
 
    closedir(dp);

    const char* desc[] = {path};
    systemlog("INFO", "READDIR", 1, desc);
    return 0;
}
 
static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    bzero(fpath, 1000);
    int i, cekCipher = 0;
 
    char* t;
    if (strcmp(path, "/") != 0) {
 
        t = strstr(path, "/AtoZ_");
        if (t) 
        {
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

            char tipe[1024];
            bzero(tipe, 1024);
            strcpy(tipe, befciper);
            strcat(tipe, "/"); strcat(befciper, "/");
            strcat(tipe, token);
 
            if (strlen(tipe) == strlen(path)) 
            {
                DIR *dp = opendir(fpath);
                if (dp == NULL) 
                {
                    char* dot;
                    dot = strchr(token, '.');
 
                    char namaFile[1024];
                    bzero(namaFile, 1024);
                    if (dot) 
                    {
                        strncpy(namaFile, token, strlen(token) - strlen(dot));
                        if (cekCipher)
                        {
                            char str[1024];
                            strcpy(str, namaFile);
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
                            char* isi4 = str;
                            strcpy(namaFile, isi4);
                        }
                            
                        strcat(namaFile, dot);
                    }
                    else 
                    {
                        strcpy(namaFile, token);
                        if (cekCipher){
                            char str[1024];
                            strcpy(str, namaFile);
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
                            char* isi5 = str;
                            strcpy(namaFile, isi5);
                        }
                    }
                    strcat(befciper, namaFile);
                }
                else 
                {
                    char namaFolder[1024];
                    bzero(namaFolder, 1024);
                    strcpy(namaFolder, token);
                    if (cekCipher)
                    {
                        char str[1024];
                            strcpy(str, namaFolder);
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
                            char* isi5 = str;
                            strcpy(namaFolder, isi5);
                    }
                    strcat(befciper, namaFolder);
                }
            }
            else 
            {
                char namaFolder[1024];
                bzero(namaFolder, 1024);
                strcpy(namaFolder, token);
                if (cekCipher){
                    char str[1024];
                    strcpy(str, namaFolder);
                    int i;
                    for(i = 0; i < strlen(str); i++) {
                        if (str[i] >= 'A' && str[i] <= 'Z') {
                            str[i] = 'Z' - (str[i] - 'A');
                        }
                        else if (str[i] >= 'a' && str[i] <= 'z') {
                            str[i] = 'z' - (str[i] - 'a');
                        }
                    }
                    char* isi6 = str;
                    strcpy(namaFolder, isi6);
                }
                strcat(befciper, namaFolder);
            }
 
 
 
        }
        sprintf(fpath, "%s%s", dirpath, befciper); 
    }
    else sprintf(fpath, "%s%s",dirpath,path);
    
    int fd;
    int res;
 
    (void) fi;
    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;
 
    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;
 
    close(fd);

    const char* desc[] = {path}; 
    systemlog("INFO", "READ", 1, desc);
    return res;
}
 
static int xmp_getattr(const char *path, struct stat *stbuf)
{
    // printf("PET : %s\n", path);
    char fpath[1000];
    bzero(fpath, 1000);
    int i, cekCipher = 0;
 
    char* t;
    if (strcmp(path, "/") != 0) 
    {
 
        t = strstr(path, "/AtoZ_");
        if (t) 
        {
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
 
            char tipe[1024];
            bzero(tipe, 1024);
            strcpy(tipe, befciper);
            strcat(tipe, "/"); strcat(befciper, "/");
            strcat(tipe, token);
 
            if (strlen(tipe) == strlen(path)) 
            {
                char pathFolder[1024];
                sprintf(pathFolder, "%s%s", dirpath, befciper);
                DIR *dp = opendir(pathFolder);
                if (dp == NULL) 
                {
                    char* dot;
                    dot = strchr(token, '.');
 
                    char namaFile[1024];
                    bzero(namaFile, 1024);
                    if (dot) 
                    {
                        strncpy(namaFile, token, strlen(token) - strlen(dot));
                        char str[1024];
                        strcpy(str, namaFile);
                        int i;
                        for(i = 0; i < strlen(str); i++) {
                            if (str[i] >= 'A' && str[i] <= 'Z') {
                                str[i] = 'Z' - (str[i] - 'A');
                            }
                            else if (str[i] >= 'a' && str[i] <= 'z') {
                                str[i] = 'z' - (str[i] - 'a');
                            }
                        }
                        char* isi6 = str;
                        strcpy(namaFile, isi6);
                        strcat(namaFile, dot);
                    }
                    else 
                    {
                        strcpy(namaFile, token);
                        char str[1024];
                        strcpy(str, namaFile);
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
                        char* isi7 = str;
                        strcpy(namaFile, isi7);
                    }
                    strcat(befciper, namaFile);
                }
                else 
                {
                    char namaFolder[1024];
                    bzero(namaFolder, 1024);
                    strcpy(namaFolder, token);

                    char str[1024];
                    strcpy(str, namaFolder);
                    int i;
                    for(i = 0; i < strlen(str); i++) 
                    {
                        if (str[i] >= 'A' && str[i] <= 'Z')
                        {
                                str[i] = 'Z' - (str[i] - 'A');                            }
                        else if (str[i] >= 'a' && str[i] <= 'z') {
                            str[i] = 'z' - (str[i] - 'a');
                        }
                    }
                    char* isi8 = str;
                    strcpy(namaFolder, isi8);
                    strcat(befciper, namaFolder);
                }
            }
            else 
            {
                char namaFolder[1024];
                bzero(namaFolder, 1024);
                strcpy(namaFolder, token);
                
                char str[1024];
                strcpy(str, namaFolder);
                int i;
                for(i = 0; i < strlen(str); i++) 
                {
                    if (str[i] >= 'A' && str[i] <= 'Z')
                    {
                        str[i] = 'Z' - (str[i] - 'A');           
                    }
                    else if (str[i] >= 'a' && str[i] <= 'z') {
                        str[i] = 'z' - (str[i] - 'a');
                    }
                }
                char* isi9 = str;
                strcpy(namaFolder, isi9);
                strcat(befciper, namaFolder);
            }
 
        }
        sprintf(fpath, "%s%s", dirpath, befciper); 
    }
    else { 
        
        sprintf(fpath, "%s%s",dirpath,path);
    }
 
    int res;
 
    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;
 
    const char* desc[] = {path}; 
    systemlog("INFO", "GETATTR", 1, desc);
    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
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

    char fpath[1024];
    bzero(fpath, 1024);
    int i, cek_ciper = 0;

    char* t;
    if (strcmp(path, "/") != 0)
    {
        t = strstr(path, "/AtoZ_");
        if (t)
        {
            cek_ciper = 1;
            t++;
        }
    }

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else if (cek_ciper)
    {
        char befciper[1024];
        bzero(befciper, 1024);
        strncpy(befciper, path, strlen(path) - strlen(t));

        char x[1024];
        strcpy(x, t);
        char* token;
        char* s = x;

        char tempo[1024];
        i = 0;

        for(; token = strtok_r(s, "/", &s); i++)
        {
            bzero(tempo, 1024);
            if (i == 0)
            {
                strcpy(tempo, token);
                strcat(befciper, tempo);
                i = 1;
                continue;
            }

            char tipe[1024];
            bzero(tipe, 1024);
            strcat(befciper, "/");
            strcpy(tipe, befciper);
            strcat(tipe, token);

            if (strlen(tipe) == strlen(path))
            {
                char pathFolder[1024];
                sprintf(pathFolder, "%s%s%s", dirpath, befciper, token);

                DIR* dp = opendir(pathFolder);
                if (!dp)
                {
                    char* dot = strchr(token, ".");

                    char namaFile[1024];
                    bzero(namaFile, 1024);
                    if (dot)
                    {
                        strncpy(namaFile, token, strlen(token) - strlen(dot));
                        char str[1024];
                        strcpy(str, namaFile);
                        int i;
                        for (i = 0; i < strlen(str); i++)
                        {
                            if (str[i] >= 'A' && str[i] <= 'Z') {
                                str[i] = 'Z' - (str[i] - 'A') ;
                            }
                            else if (str[i] >= 'a' && str[i] <= 'z') {
                                str[i] = 'z' - (str[i] - 'a') ;
                            }
                        }
                        char* isi = str;
                        strcpy(namaFile, isi);
                        strcat(namaFile, dot);
                    }
                    else
                    {
                        strcpy(namaFile, token);
                        char str[1024];
                        strcpy(str, namaFile);
                        int i;
                        for (i = 0; i < strlen(str); i++)
                        {
                            if (str[i] >= 'A' && str[i] <= 'Z') {
                                str[i] = 'Z' - (str[i] - 'A') ;
                            }
                            else if (str[i] >= 'a' && str[i] <= 'z') {
                                str[i] = 'z' - (str[i] - 'a') ;
                            }
                        }
                        char* isi = str;
                        strcpy(namaFile, isi);
                    }
                    strcat(befciper, namaFile);
                }
                else
                {
                    char namaFolder[1024];
                    bzero(namaFolder, 1024);
                    strcpy(namaFolder, token);
                    char str[1024];
                        strcpy(str, namaFolder);
                        int i;
                        for (i = 0; i < strlen(str); i++)
                        {
                            if (str[i] >= 'A' && str[i] <= 'Z') {
                                str[i] = 'Z' - (str[i] - 'A') ;
                            }
                            else if (str[i] >= 'a' && str[i] <= 'z') {
                                str[i] = 'z' - (str[i] - 'a') ;
                            }
                        }
                        char* isi = str;
                    strcpy(namaFolder, isi);
                    strcat(befciper, namaFolder);
                }
            }
            else
            {
                char namaFolder[1024];
                bzero(namaFolder, 1024);
                strcpy(namaFolder, token);
                char str[1024];
                    strcpy(str, namaFolder);
                    int i;
                    for (i = 0; i < strlen(str); i++)
                    {
                        if (str[i] >= 'A' && str[i] <= 'Z') {
                            str[i] = 'Z' - (str[i] - 'A') ;
                        }
                        else if (str[i] >= 'a' && str[i] <= 'z') {
                            str[i] = 'z' - (str[i] - 'a') ;
                        }
                    }
                    char* isi = str;
                strcpy(namaFolder, isi);
                strcat(befciper, namaFolder);
            }
        }
        sprintf(fpath, "%s%s", dirpath, befciper);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }
    int res = mkdir(fpath, mode);
    if (res < 0)
        return -errno;
    
    const char* desc[] = {path}; 
    systemlog("INFO", "MKDIR", 1, desc);
    return 0;
}

static int xmp_rename(const char *from, const char *to)
{
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
    char f_from[1024], f_to[1024];
    bzero(f_from, 1024);
    bzero(f_to, 1024);
    int i, cek_ciper = 0;

    char* t;
    if (strcmp(from, "/") != 0)
    {
        t = strstr(from, "/AtoZ_");
        if (t)
        {
            cek_ciper = 1;
            t++;
        }
    }

    if (strcmp(from, "/") == 0)
    {
        from = dirpath;
        sprintf(f_from, "%s", from);
    }
    else if (cek_ciper)
    {
        char befciper[1024];
        bzero(befciper, 1024);
        strncpy(befciper, from, strlen(from) - strlen(t));

        char x[1024];
        strcpy(x, t);
        char* token;
        char* s = x;

        char tempo[1024];
        i = 0;

        for(; token = strtok_r(s, "/", &s); i++)
        {
            bzero(tempo, 1024);
            if (i == 0)
            {
                strcpy(tempo, token);
                strcat(befciper, tempo);
                i = 1;
                continue;
            }

            char tipe[1024];
            bzero(tipe, 1024);
            strcat(befciper, "/");
            strcpy(tipe, befciper);
            strcat(tipe, token);

            if (strlen(tipe) == strlen(from))
            {
                char pathFolder[1024];
                sprintf(pathFolder, "%s%s%s", dirpath, befciper, token);

                DIR* dp = opendir(pathFolder);
                if (!dp)
                {
                    char* dot = strchr(token, ".");

                    char namaFile[1024];
                    bzero(namaFile, 1024);
                    if (dot)
                    {
                        strncpy(namaFile, token, strlen(token) - strlen(dot));
                        char str[1024];
                        strcpy(str, namaFile);
                        int i;
                        for (i = 0; i < strlen(str); i++)
                        {
                            if (str[i] >= 'A' && str[i] <= 'Z') {
                                str[i] = 'Z' - (str[i] - 'A') ;
                            }
                            else if (str[i] >= 'a' && str[i] <= 'z') {
                                str[i] = 'z' - (str[i] - 'a') ;
                            }
                        }
                        char* isi = str;
                        strcpy(namaFile, isi);
                        strcat(namaFile, dot);
                    }
                    else
                    {
                        strcpy(namaFile, token);
                        char str[1024];
                        strcpy(str, namaFile);
                        int i;
                        for (i = 0; i < strlen(str); i++)
                        {
                            if (str[i] >= 'A' && str[i] <= 'Z') {
                                str[i] = 'Z' - (str[i] - 'A') ;
                            }
                            else if (str[i] >= 'a' && str[i] <= 'z') {
                                str[i] = 'z' - (str[i] - 'a') ;
                            }
                        }
                        char* isi = str;
                        strcpy(namaFile, isi);
                    }
                    strcat(befciper, namaFile);
                }
                else
                {
                    char namaFolder[1024];
                    bzero(namaFolder, 1024);
                    strcpy(namaFolder, token);
                    char str[1024];
                        strcpy(str, namaFolder);
                        int i;
                        for (i = 0; i < strlen(str); i++)
                        {
                            if (str[i] >= 'A' && str[i] <= 'Z') {
                                str[i] = 'Z' - (str[i] - 'A') ;
                            }
                            else if (str[i] >= 'a' && str[i] <= 'z') {
                                str[i] = 'z' - (str[i] - 'a') ;
                            }
                        }
                        char* isi = str;
                    strcpy(namaFolder, isi);
                    strcat(befciper, namaFolder);
                }
            }
            else
            {
                char namaFolder[1024];
                bzero(namaFolder, 1024);
                strcpy(namaFolder, token);
                char str[1024];
                    strcpy(str, namaFolder);
                    int i;
                    for (i = 0; i < strlen(str); i++)
                    {
                        if (str[i] >= 'A' && str[i] <= 'Z') {
                            str[i] = 'Z' - (str[i] - 'A') ;
                        }
                        else if (str[i] >= 'a' && str[i] <= 'z') {
                            str[i] = 'z' - (str[i] - 'a') ;
                        }
                    }
                    char* isi = str;
                strcpy(namaFolder, isi);
                strcat(befciper, namaFolder);
            }
        }
        sprintf(f_from, "%s%s", dirpath, befciper);
    }
    else
    {
        sprintf(f_from, "%s%s", dirpath, from);
    }

    cek_ciper = 0;

    if (strcmp(to, "/") != 0)
    {
        t = strstr(to, "/AtoZ_");
        if (t)
        {
            cek_ciper = 1;
            t++;
        }
    }

    if (strcmp(to, "/") == 0)
    {
        to = dirpath;
        sprintf(f_to, "%s", to);
    }
    else if (cek_ciper)
    {
        char befciper[1024];
        bzero(befciper, 1024);
        strncpy(befciper, to, strlen(to) - strlen(t));

        char x[1024];
        strcpy(x, t);
        char* token;
        char* s = x;

        char tempo[1024];
        i = 0;

        for(; token = strtok_r(s, "/", &s); i++)
        {
            bzero(tempo, 1024);
            if (i == 0)
            {
                strcpy(tempo, token);
                strcat(befciper, tempo);
                i = 1;
                continue;
            }

            char tipe[1024];
            bzero(tipe, 1024);
            strcat(befciper, "/");
            strcpy(tipe, befciper);
            strcat(tipe, token);

            if (strlen(tipe) == strlen(to))
            {
                char pathFolder[1024];
                sprintf(pathFolder, "%s%s%s", dirpath, befciper, token);

                DIR* dp = opendir(pathFolder);
                if (!dp)
                {
                    char* dot = strchr(token, ".");

                    char namaFile[1024];
                    bzero(namaFile, 1024);
                    if (dot)
                    {
                        strncpy(namaFile, token, strlen(token) - strlen(dot));
                        char str[1024];
                        strcpy(str, namaFile);
                        int i;
                        for (i = 0; i < strlen(str); i++)
                        {
                            if (str[i] >= 'A' && str[i] <= 'Z') {
                                str[i] = 'Z' - (str[i] - 'A') ;
                            }
                            else if (str[i] >= 'a' && str[i] <= 'z') {
                                str[i] = 'z' - (str[i] - 'a') ;
                            }
                        }
                        char* isi = str;
                        strcpy(namaFile, isi);
                        strcat(namaFile, dot);
                    }
                    else
                    {
                        strcpy(namaFile, token);
                        char str[1024];
                        strcpy(str, namaFile);
                        int i;
                        for (i = 0; i < strlen(str); i++)
                        {
                            if (str[i] >= 'A' && str[i] <= 'Z') {
                                str[i] = 'Z' - (str[i] - 'A') ;
                            }
                            else if (str[i] >= 'a' && str[i] <= 'z') {
                                str[i] = 'z' - (str[i] - 'a') ;
                            }
                        }
                        char* isi = str;
                        strcpy(namaFile, isi);
                    }
                    strcat(befciper, namaFile);
                }
                else
                {
                    char namaFolder[1024];
                    bzero(namaFolder, 1024);
                    strcpy(namaFolder, token);
                    char str[1024];
                        strcpy(str, namaFolder);
                        int i;
                        for (i = 0; i < strlen(str); i++)
                        {
                            if (str[i] >= 'A' && str[i] <= 'Z') {
                                str[i] = 'Z' - (str[i] - 'A') ;
                            }
                            else if (str[i] >= 'a' && str[i] <= 'z') {
                                str[i] = 'z' - (str[i] - 'a') ;
                            }
                        }
                        char* isi = str;
                    strcpy(namaFolder, isi);
                    strcat(befciper, namaFolder);
                }
            }
            else
            {
                char namaFolder[1024];
                bzero(namaFolder, 1024);
                strcpy(namaFolder, token);
                char str[1024];
                    strcpy(str, namaFolder);
                    int i;
                    for (i = 0; i < strlen(str); i++)
                    {
                        if (str[i] >= 'A' && str[i] <= 'Z') {
                            str[i] = 'Z' - (str[i] - 'A') ;
                        }
                        else if (str[i] >= 'a' && str[i] <= 'z') {
                            str[i] = 'z' - (str[i] - 'a') ;
                        }
                    }
                    char* isi = str;
                strcpy(namaFolder, isi);
                strcat(befciper, namaFolder);
            }
        }
        sprintf(f_to, "%s%s", dirpath, befciper);
    }
    else
    {
        sprintf(f_to, "%s%s", dirpath, to);
    }

    int res = rename(f_from, f_to);
    printf("f_from : %s\n", f_from);
    printf("f_to : %s\n", f_to);

    if (res < 0)
        return -errno;

    const char* desc[] = {from, to}; 
    systemlog("INFO", "RENAME", 2, desc);
    return 0;
}

static int xmp_unlink(const char* path)
{
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

    int res = unlink(fpath);
    if (res < 0)
        return -errno;
    
    const char *desc[] = {path};
    systemlog("WARNING","UNLINK", 1, desc);
    return 0;
}
 
static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .mkdir = xmp_mkdir,
    .rename = xmp_rename,
    .rmdir = xmp_rmdir,
    .unlink = xmp_unlink,
};

/* End XMP Field */
 
int  main(int  argc, char *argv[])
{
    
    umask(0);
 
    return fuse_main(argc, argv, &xmp_oper, NULL);
}

// gcc -Wall `pkg-config fuse --cflags` SINSEIFS.c -o tes `pkg-config fuse --libs`
