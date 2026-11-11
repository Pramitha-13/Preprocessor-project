#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void remove_comments(FILE *src, FILE *dest);

void handle_include(FILE *src, FILE *dest);

void handle_macro(FILE *src, FILE *dest);

#endif
