#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>

void fileRead(char *fileName, uint64_t buffer, size_t length)
{
    FILE *ptr;
    ptr = fopen(fileName, "rb");
    fread(buffer, length, 1, ptr);
}