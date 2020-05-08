#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void validate_dirp(DIR *dirp, char * directory_path) {
    if (dirp == NULL) {
         printf ("Cannot open directory '%s'\n", directory_path);
         exit(EXIT_FAILURE);
     }
}

int validate_dent(struct dirent* dent, char * directory_path) {
    if (dent == NULL) {
         //printf ("Cannot open directory '%s'\n", directory_path);
         return -1;
     }
     return 0;
}

int get_dent_size(char * directory_path) {
    DIR    *dirp;
    struct dirent* dent;
    int    dent_size = 0;

    dirp = opendir(directory_path);

    do {
        dent = readdir(dirp);
        dent_size++;
    } while (dent);
    return dent_size;
}

int get_max_line_length_in_directory(char * directory_path) {
    DIR    *dirp;
    struct dirent* dent;
    struct stat info;
    int    file_count = 0;
    int    dent_size  = get_dent_size(directory_path);
    int    file_path_size = 500;
    char   files[dent_size][file_path_size];
    memset(files, 0, dent_size * file_path_size);

    dirp = opendir(directory_path);
    validate_dirp(dirp, directory_path);
    
    do {
        dent = readdir(dirp);
        if(validate_dent(dent, directory_path) == -1) 
            continue;

        if(dent){
           if (stat(directory_path, &info) == -1)
               continue;
        }

        printf("> %s \n", dent->d_name);
        for(int i = 0; i < sizeof(dent->d_name); i++)
            files[file_count][i] = dent->d_name[i];

        file_count++;
    } while (dent);
    closedir(dirp);

    for(int i = 0; i < file_count; i++) {
        printf("-- %s \n", files[i]);
    }
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
