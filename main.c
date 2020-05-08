#define _GNU_SOURCE
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int is_dot_file(char * file);
char buf[PATH_MAX + 1];

int validate_dirp(DIR *dirp, char * directory_path) {
    if (dirp == NULL) {
         printf ("Cannot open directory '%s'\n", directory_path);
         return -1;
     }
     return 0;
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

int is_dir(char * file_path) {
    DIR* dir = opendir(file_path);
    if (dir) {
        closedir(dir);
        return 0;
    } 
    return -1;
}

int get_max_line_length_in_directory(char * directory_path) {
    // if file, process as file and return 0 here.
    if(is_dir(directory_path) == -1) {
        printf("file: %s \n", directory_path);
        return 0;
    }

    DIR    *dirp;
    struct dirent* dent;
    struct stat info;
    int    file_count     = 0;
    int    dent_size      = get_dent_size(directory_path);
    int    file_path_size = 500;
    char   files[dent_size][file_path_size];
    memset(files, 0, dent_size * file_path_size);

    printf("> %s\n", directory_path);
    dirp = opendir(directory_path);
    if(validate_dirp(dirp, directory_path) == -1) {
        return -1;
    }
    
    do {
        dent = readdir(dirp);
        if(validate_dent(dent, directory_path) == -1) 
            continue;

        if(dent){
           if (stat(directory_path, &info) == -1)
               continue;
        }
        
        if(is_dot_file(dent->d_name) == 0) { 
            continue;
        }

        int directory_path_size = 0;
        for(int i = 0; i < strlen(directory_path); i++) {
            files[file_count][i] = directory_path[i];
            directory_path_size++;
        }

        files[file_count][directory_path_size] = '/';
        for(int i = 0; i < strlen(dent->d_name); i++)
            files[file_count][i + directory_path_size + 1] = dent->d_name[i];

        file_count++;
    } while (dent);
    closedir(dirp);

    for(int i = 0; i < file_count; i++) {
        if(is_dot_file(files[i]) != 0) {
            get_max_line_length_in_directory(files[i]);
        }
    }
    // if directory, iterate the files in the dir and recursively apply this method
    // if file, apply get_max_line_length_in_file
    return 0;
}

int is_dot_file(char * file) {
    int result;
    result = strcmp(file, ".");
    if(result == 0) 
        return result;
    result = strcmp(file, "..");
    return result;
    
} 

int get_max_line_length_in_file(char * file_path) {
    FILE *  fp;
    ssize_t read;
    char *  line = NULL;
    size_t  len  = 0;

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
