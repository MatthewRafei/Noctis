#include "diagnostic.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>

// TODO(malac0da): Replace all error messages with diagnostic reporting

char *file_to_str(const char *fp, struct CompilerContext *context)
{
    FILE *file = fopen(fp, "rb");

    if (!file) {
        context->source.file = fp;
        context->source.line = 0;
        context->source.col = 0;
        report_error(INTERNAL, "Failed to open file.", context);
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        context->source.file = fp;
        context->source.line = 0;
        context->source.col = 0;
        report_error(INTERNAL, "Failed to seek file.", context);
        (void) fclose(file);
        return NULL;
    }

    if (ftell(file) < 0) {
        context->source.file = fp;
        context->source.line = 0;
        context->source.col = 0;
        report_error(INTERNAL, "Failed to determine file length.", context);
        (void) fclose(file);
        return NULL;
    }

    size_t length = (size_t) ftell(file);

    // Some file-systems might return -1
    if (length == (size_t) -1) {
        context->source.file = fp;
        context->source.line = 0;
        context->source.col = 0;
        report_error(INTERNAL, "Failed to determine file length.", context);
        (void) fclose(file);
        return NULL;
    }

    (void) fseek(file, 0, SEEK_SET);

    // No need to cast char* as malloc's return type is already void *
    char *buffer = malloc(length + 1);
    if (!buffer) {
        report_error(INTERNAL, "Failed to allocate memory\n", context);
        (void) fclose(file);
        return NULL;
    }
    //Free buffer if fread fails
    size_t read_size = fread(buffer, 1, length, file);
    if (read_size != length) {
        report_error(INTERNAL, "Failed to read file!\n", context);
        free(buffer);
        (void) fclose(file);
        return NULL;
    }

    buffer[length] = '\0';
    (void) fclose(file);
    return buffer;
}
