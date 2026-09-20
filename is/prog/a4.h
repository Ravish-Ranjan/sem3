#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* additive_cipher(const char* data, int key);
char* multiplicative_cipher(const char* data, int key);
char* additive_decipher(const char* data, int key);
char* multiplicative_decipher(const char* data, int key);