#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h> // IWYU pragma: keep
#include <stdint.h> // IWYU pragma: keep

// ##########################################

bool file_read(uint8_t **file_buffer, uint64_t *file_size, const char *source_filename);
bool file_write(uint8_t **file_buffer, uint64_t *file_size, const char *destination_filename);

// ##########################################
// clang-format off

#define STR_(x) #x
#define STR(x) STR_(x)

#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)

#define ARRAY_LEN(x) (sizeof((x)) / sizeof((x)[0]))

#define DEBUG_BREAK() __builtin_trap()
#define ASSERT(x, ...) if (!(x)) {DEBUG_BREAK();}

#define UNUSED(x) (void)(x)

// clang-format on
// ##########################################

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // COMMON_H

#ifdef COMMON_IMPL

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>  // IWYU pragma: keep
#include <stdlib.h> // IWYU pragma: keep

bool file_read(uint8_t **file_buffer, uint64_t *file_size, const char *source_filename)
{
    if (!source_filename || !file_buffer) { return false; }

    // FILE_OPEN
    FILE *f = fopen(source_filename, "rb");
    if (!f) { return false; }

    fseek(f, 0, SEEK_END);
    *file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // MEMORY_ALLOCATION
    *file_buffer = (uint8_t *)malloc(*file_size + 1);

    if (!(*file_buffer)) { return false; }

    *(*file_buffer + *file_size) = '\0';

    fread(*file_buffer, sizeof(uint8_t), *file_size, f);

    fclose(f);
    return true;
}

bool file_write(uint8_t **file_buffer, uint64_t *file_size, const char *destination_filename)
{
    if (!destination_filename || !file_buffer) { return false; }

    // FILE_OPEN
    FILE *f = fopen(destination_filename, "wb");
    if (!f) { return false; }

    fwrite(*file_buffer, sizeof(uint8_t), *file_size, f);

    fclose(f);
    return true;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // COMMON_IMPL