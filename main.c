#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

void validate_dirp(DIR *dirp, char * directory_path) {
    if (dirp == NULL) {
         printf ("Cannot open directory '%s'\n", directory_path);
         exit(EXIT_FAILURE);
     }
}

void validate_dent(struct dirent* dent, char * directory_path) {
    if (dent == NULL) {
         printf ("Cannot open directory '%s'\n", directory_path);
         exit(EXIT_FAILURE);
     }
}

int get_max_line_length_in_directory(char * directory_path) {
    DIR    *dirp;
    struct dirent* dent;
    struct stat info;

    dirp = opendir(directory_path);
    validate_dirp(dirp, directory_path);
    do {
        dent = readdir(dirp);
        validate_dent(dent, directory_path); 
        if(dent){
           if (stat(directory_path, &info) == -1) {
               perror("stat");
               exit(EXIT_FAILURE);
           } 
        }
        switch (info.st_mode & S_IFMT) {
            case S_IFBLK:  printf("block device\n");            break;
            case S_IFCHR:  printf("character device\n");        break;
            case S_IFDIR:  printf("dir ");                      break;
            case S_IFIFO:  printf("FIFO/pipe\n");               break;
            case S_IFLNK:  printf("lnk ");                      break;
            case S_IFREG:  printf("reg ");                      break;
            case S_IFSOCK: printf("socket\n");                  break;
            default:       printf("unknown?\n");                break;
        }
        printf("%s \n", dent->d_name);
    } while (dent);
    closedir(dirp);
    // if directory, iterate the files in the dir and recursively apply this method
    // if file, apply get_max_line_length_in_file
    return 0;
}

int get_max_line_length_in_file(char * file_path) {
    FILE *  fp;
    char *  line = NULL;
    size_t  len  = 0;
    ssize_t read;
    fp = fopen(file_path, "r");
    if(fp == NULL)
        exit(EXIT_FAILURE);
    while((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }
    fclose(fp);
    if(line)
        free(line);
    return 0;
}

int main(void) {
    get_max_line_length_in_directory("/Users/kei.sugano/code/huubhr/backend");
    //get_max_line_length_in_file("tmp/test");
    return 0;
}
