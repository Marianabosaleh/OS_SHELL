#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <codec> <message>\n", argv[0]);
        return 1;
    }

    char* codec_name = argv[1];
    char* message = argv[2];

    // Load the shared library for the selected codec
    char lib_name[20];
    snprintf(lib_name, sizeof(lib_name), "lib%s.so", codec_name);
    void* handle = dlopen(lib_name, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    // Get the encode function for the selected codec
    char encode_func_name[20];
    snprintf(encode_func_name, sizeof(encode_func_name), "%s_encode", codec_name);
    char* (*encode_func)(const char*);
    encode_func = dlsym(handle, encode_func_name);
    if (!encode_func) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    // Encode the message using the selected codec
    char* encoded_message = encode_func(message);
    printf("%s\n", encoded_message);

    // Clean up
    free(encoded_message);
    dlclose(handle);
    return 0;
}
