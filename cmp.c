#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    FILE *file1, *file2;
    char *file1_name, *file2_name;
    int equal = 0; // initialize to true
    int ch1, ch2;
    // Check that the user has provided two file names
    if (argc != 3 && argc != 4) {
        printf("Usage: cmp file1.txt file2.txt [-i] [-v]\n");
        return 1;
    }

    // Get the file names from the command-line arguments
    file1_name = argv[1];
    file2_name = argv[2];

    // Open the files for reading
    file1 = fopen(file1_name, "r");
    if (file1 == NULL) {
        printf("Error: could not open file %s\n", file1_name);
        return 1;
    }

    file2 = fopen(file2_name, "r");
    if (file2 == NULL) {
        printf("Error: could not open file %s\n", file2_name);
        fclose(file1);
        return 1;
    }

    // Get file size for both files
    fseek(file1, 0, SEEK_END);
    long int file1_size = ftell(file1);
    fseek(file1, 0, SEEK_SET);

    fseek(file2, 0, SEEK_END);
    long int file2_size = ftell(file2);
    fseek(file2, 0, SEEK_SET);

    if (file1_size != file2_size) {
        equal = 1;
    }
    while ((ch1 = fgetc(file1)) != EOF && (ch2 = fgetc(file2)) != EOF) {
    if (argc == 4 && strcmp(argv[3], "-i") == 0) {
        if (tolower(ch1) != tolower(ch2)) {
            equal = 1;
            break;
        }
    } else {
        if (ch1 != ch2) {
            equal = 1;
            break;
        }
    }
}
 

    // Close the files when you're done with them
    fclose(file1);
    fclose(file2);

    // Print the result, depending on whether the -v flag was provided
    if (argc == 4 && strcmp(argv[3], "-v") == 0) {
        if (equal) {
            printf("distinct\n");
        } else {
            printf("equal\n");
        }
    }
    if (argc == 4 && strcmp(argv[3], "-i") == 0) {
        if (equal) {
            printf("distinct\n");
        } else {
            printf("equal\n");
        }
    }

    // Print the integer result when no flags are provided
    if (argc == 3) {
        printf("%d\n", equal);
    }

    // Return an integer value depending on whether the files are equal
    return equal ? 1 : 0;
}
