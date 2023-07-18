#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char* codecA_encode(const char* message) {
    size_t len = strlen(message);
    char* result = malloc(len + 1);
    for (size_t i = 0; i < len; i++) {
        if (islower(message[i])) {
            result[i] = toupper(message[i]);
        } else if (isupper(message[i])) {
            result[i] = tolower(message[i]);
        } else {
            result[i] = message[i];
        }
    }
    result[len] = '\0';
    return result;
}

char* codecA_decode(const char* message) {
    return codecA_encode(message);
}
