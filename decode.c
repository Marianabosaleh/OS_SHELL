#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

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

    // Get the decode function for the selected codec
    char decode_func_name[20];
    snprintf(decode_func_name, sizeof(decode_func_name), "%s_decode", codec_name);
    char* (*decode_func)(const char*);
    decode_func = dlsym(handle, decode_func_name);
    if (!decode_func) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    // Decode the message using the selected codec
    char* decoded_message = decode_func(message);
    printf("%s\n", decoded_message);

    // Clean up and exit
    dlclose(handle);
    free(decoded_message);
    return 0;
}
