#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h> // IWYU pragma: keep
#include <stdint.h> // IWYU pragma: keep

// ##########################################

bool bard_file_read(uint8_t **file_buffer, uint64_t *file_size, const char *source_filename);
bool bard_file_write(uint8_t **file_buffer, uint64_t *file_size, const char *destination_filename);

// ##########################################
// clang-format off

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)

#define ARRAYLEN(x) (sizeof((x)) / sizeof((x)[0]))

#define DEBUG_BREAK() __builtin_trap()
#define ASSERT(x) do { if (!(x)) { DEBUG_BREAK(); } } while (0)

#define UNUSED(x) (void)(x)

// #####################
#define VEC_HEADER(type) // put inside struct
#define vec_reserve(vecptr, newcapacity)
#define vec_push(vecptr, ...)
#define vec_reset(vecptr)
#define vec_free(vecptr)
#define vec_print_stats(vecptr)
// #####################

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

#undef VEC_HEADER
#undef vec_print_stats
#undef vec_reserve
#undef vec_push
#undef vec_reset
#undef vec_free
// clang-format off
#define VEC_HEADER(type) typeof_unqual(type) *items; uint64_t count; uint64_t capacity
#define vec_print_stats(vecptr) printf("%s: count = %llu, capacity = %llu\n", STRINGIFY(vecptr), (vecptr)->count, (vecptr)->capacity)
#define vec_size__(vecptr) (sizeof(*((vecptr)->items)))
#define vec_type__(vecptr) typeof_unqual(*((vecptr)->items))

#define vec_reserve(vecptr, newcapacity)\
do {\
    if ((vecptr)->capacity <= (newcapacity))\
    {\
        if (!((vecptr)->items))\
        {\
            (vecptr)->items = (vec_type__(vecptr) *)realloc(0, vec_size__(vecptr) * 256);\
            (vecptr)->capacity = (newcapacity);\
        }\
        else\
        {\
            (vecptr)->items = (vec_type__(vecptr) *)realloc((vecptr)->items, (vecptr)->capacity * 2 * vec_size__(vecptr));\
            (vecptr)->capacity = (newcapacity);\
        }\
    }\
} while(0)

#define vec_push(vecptr, ...)\
do {\
    if (!((vecptr)->items))\
    {\
        (vecptr)->items = (vec_type__(vecptr) *)realloc(0, vec_size__(vecptr) * 256);\
        (vecptr)->capacity = 256;\
        (vecptr)->count = 0;\
    }\
    if ((vecptr)->count + 1 >= (vecptr)->capacity)\
    {\
        (vecptr)->items = (vec_type__(vecptr) *)realloc((vecptr)->items, (vecptr)->capacity * 2 * vec_size__(vecptr));\
        (vecptr)->capacity *= 2;\
    }\
    (vecptr)->items[(vecptr)->count] = (__VA_ARGS__);\
    (vecptr)->count += 1;\
} while(0)

#define vec_reset(vecptr) do { (vecptr)->count = 0; } while(0)
#define vec_free(vecptr) do { (vecptr)->capacity = 0; free((vecptr)->items); } while(0)
// clang-format on

bool bard_file_read(uint8_t **file_buffer, uint64_t *file_size, const char *source_filename)
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

bool bard_file_write(uint8_t **file_buffer, uint64_t *file_size, const char *destination_filename)
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