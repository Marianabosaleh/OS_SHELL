#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *src_file, *dst_file;
    char *src_filename, *dst_filename;
    int force = 0, verbose = 0, status = 0;

    // Check that the user has provided at least two file names
    if (argc < 3) {
        printf("Usage: copy [-f] [-v] source_file destination_file\n");
        return 1;
    }

    // Parse command-line arguments
    int i;
    for (i = 1; i < argc - 2; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            force = 1;
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else {
            printf("Error: unrecognized flag %s\n", argv[i]);
            return 1;
        }
    }

    src_filename = argv[argc - 2];
    dst_filename = argv[argc - 1];  

    // Open source file for reading
    src_file = fopen(src_filename, "rb");
    if (src_file == NULL) {
        printf("Error: could not open source file %s\n", src_filename);
        return 1;
    }

    // Check if destination file already exists
    dst_file = fopen(dst_filename, "rb");
    if (dst_file != NULL) {
        fclose(dst_file);
        if (force) {
            if (remove(dst_filename) != 0) {
                printf("Error: could not remove target file %s\n", dst_filename);
                fclose(src_file);
                return 1;
            }
        } else {
            printf("target file exist\n");
            fclose(src_file);
            return 1;
        }
    }

    // Open destination file for writing
    dst_file = fopen(dst_filename, "wb");
    if (dst_file == NULL) {
        printf("Error: could not create destination file %s\n", dst_filename);
        fclose(src_file);
        return 1;
    }

    // Copy contents of source file to destination file
    int ch;
    while ((ch = fgetc(src_file)) != EOF) {
        if (fputc(ch, dst_file) == EOF) {
            printf("Error: could not write to destination file %s\n", dst_filename);
            fclose(src_file);
            fclose(dst_file);
            remove(dst_filename);
            return 1;
        }
    }

    // Close files and print result
    fclose(src_file);
    fclose(dst_file);
    if (verbose) {
        printf("success\n");
    }
    return status;
}
