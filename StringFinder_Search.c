#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ssize_t my_getline(char **lineptr, size_t *n, FILE *stream) {
    if (lineptr == NULL || n == NULL || stream == NULL) {
        return -1;
    }

    size_t pos = 0;
    int c;

    if (*lineptr == NULL) {
        *n = 128;
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            return -1;
        }
    }

    while ((c = fgetc(stream)) != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n * 2;
            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *lineptr = new_ptr;
            *n = new_size;
        }

        (*lineptr)[pos++] = (char)c;
        if (c == '\n') {
            break;
        }
    }

    if (pos == 0 && c == EOF) {
        return -1;
    }

    (*lineptr)[pos] = '\0';
    return pos;
}

int search_in_file(const char *filename, const char *search_term) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return -1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int found = 0;

    while ((read = my_getline(&line, &len, file)) != -1) {
        if (strstr(line, search_term) != NULL) {
            found = 1;
            break;
        }
    }

    free(line);
    fclose(file);

    return found;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <search_term>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    const char *search_term = argv[2];

    int result = search_in_file(filename, search_term);
    if (result == 1) {
        printf("Found\n");
    } else {
        printf("Not Found\n");
    }

    return 0;
}
