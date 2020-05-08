#define _GNU_SOURCE
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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

bool starts_with(char * target, char elem) {
    return target[0] == elem;
}

// is there duplication here???
int get_max_length_in_file(char * file_path) {
    FILE* file = fopen(file_path, "r"); /* should check the result */
    char line[256];
    int  max_size = 0;

    while (fgets(line, sizeof(line), file)) {
        int curr_size = strlen(line);
        if(max_size < curr_size) {
            max_size = curr_size;
        }
    }

    fclose(file);
    return max_size;
}

int max_line_lengths[1024] = {};
int max_line_length_index    = 0;

int ends_with(char * s, char * t)
{
    while (*s == *t) {
        if (!(*s)) {
            return 1;
        }
        s++;
        t++;
    }
    return 0;
}

int get_max_line_length_in_directory(char * directory_path) {
    if(is_dir(directory_path) == -1) {
        if(!(ends_with(directory_path, ".groovy") == 0 || ends_with(directory_path, ".hs"))) {
            return -1; 
        }
        int max_length = get_max_length_in_file(directory_path);
        if(max_length > 0) {
            max_line_lengths[max_line_length_index] = max_length;
            max_line_length_index++;
        }
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
        
        if(starts_with(dent->d_name, '.') == true) { 
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
        get_max_line_length_in_directory(files[i]);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    printf("dir: %s\n", argv[1]);
    get_max_line_length_in_directory(argv[1]);
    int sum      = 0;
    int max_line = 0;
    for(int i = 0; i < max_line_length_index; i++) {
        sum += max_line_lengths[i];
        if(max_line < max_line_lengths[i]) {
            max_line = max_line_lengths[i];
        }
    }
    float average = sum/sizeof(max_line_lengths);
    printf("max line: %d\n", max_line);
    return 0;
}
