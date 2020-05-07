#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

int get_maximum_line_length_in_file(char * file_path) {
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
    get_maximum_line_length_in_file("tmp/test");
    return 0;
}
