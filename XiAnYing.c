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
    return res;
}
 
static int xmp_getattr(const char *path, struct stat *stbuf)
{
    
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
 
    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
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
    
    return 0;
}
 
static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .mkdir = xmp_mkdir,
};

/* End XMP Field */
 
int  main(int  argc, char *argv[])
{
    
    umask(0);
 
    return fuse_main(argc, argv, &xmp_oper, NULL);
}