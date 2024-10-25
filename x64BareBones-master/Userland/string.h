// SPDX-License-Identifier: GPL-2.0
/*
 * string_functions.h
 * Header for Linux string and memory handling functions.
 *
 * Copyright (C) 1991, 1992 Linus Torvalds
 */

#ifndef STRING_FUNCTIONS_H
#define STRING_FUNCTIONS_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int strncasecmp(const char *s1, const char *s2, size_t len);
int strcasecmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
ssize_t sized_strscpy(char *dest, const char *src, size_t count);
char *stpcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t count);
size_t strlcat(char *dest, const char *src, size_t count);
int strcmp(const char *cs, const char *ct);
int strncmp(const char *cs, const char *ct, size_t count);
char *strchr(const char *s, int c);
char *strchrnul(const char *s, int c);
char *strnchrnul(const char *s, size_t count, int c);
char *strrchr(const char *s, int c);
char *strnchr(const char *s, size_t count, int c);
size_t strlen(const char *s);
size_t strnlen(const char *s, size_t count);
size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *s, const char *reject);
char *strpbrk(const char *cs, const char *ct);
char *strsep(char **s, const char *ct);
void *memset(void *s, int c, size_t count);
void *memset16(uint16_t *s, uint16_t v, size_t count);
void *memset32(uint32_t *s, uint32_t v, size_t count);
void *memset64(uint64_t *s, uint64_t v, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t count);

#ifdef __cplusplus
}
#endif

#endif /* STRING_FUNCTIONS_H */
