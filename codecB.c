#include <stdlib.h>
#include <string.h>

char* codecB_encode(const char* message) {
    size_t len = strlen(message);
    char* encoded = malloc(len + 1);

    for (size_t i = 0; i < len; i++) {
        if ((message[i] >= 'a' && message[i] <= 'z') || (message[i] >= 'A' && message[i] <= 'Z')) {
            encoded[i] = message[i] + 3;
        } else {
            encoded[i] = message[i];
        }
    }

    encoded[len] = '\0';
    return encoded;
}

char* codecB_decode(const char* message) {
    size_t len = strlen(message);
    char* decoded = malloc(len + 1);

    for (size_t i = 0; i < len; i++) {
        if ((message[i] >= 'a' && message[i] <= 'z') || (message[i] >= 'A' && message[i] <= 'Z')) {
            decoded[i] = message[i] - 3;
        } else {
            decoded[i] = message[i];
        }
    }

    decoded[len] = '\0';
    return decoded;
}
