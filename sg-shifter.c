// This work is licensed under the terms of the MIT license.  
// To obtain a copy, see <https://opensource.org/licenses/MIT>.

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XORBy 0x64 // that's how steins;gate resources are "encrypted" in the android port

#define PNG_MAGIC '\x89'
#define MP4_MAGIC '\x00'
#define TXT_MAGIC '\x23' // all script files begin with a hasthag (# / 0x23)
#define SDT_PNG_MAGIC (PNG_MAGIC ^ XORBy)
#define SDT_MP4_MAGIC (MP4_MAGIC ^ XORBy)
#define SRC_MAGIC (TXT_MAGIC ^ XORBy)

void shift(size_t n, size_t length, char* fmem, FILE* fout) {
    for (size_t i = 0; i < n; i++) {
        fmem[i] ^= XORBy; // XOR (^) the first n bytes by XORBy
    }
    fwrite(fmem, sizeof(*fmem), length, fout); // write the resulting array to the out file
}

const char* getOutExtension(const char firstByte) {
    switch (firstByte) { // determine input file's type
        case SDT_PNG_MAGIC:
            return ".png";
        case SDT_MP4_MAGIC:
            return ".mp4";
        case PNG_MAGIC:
        case MP4_MAGIC:
            return ".sdt";
        case SRC_MAGIC:
            return ".txt";
        case TXT_MAGIC:
            return ".src";
        default:
            return ".out";
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("usage: sg-shifter <in-file>\n");
        return 0;
    }

    FILE *fin  = fopen(argv[1], "rb");
    if (!fin) {
        printf("could not open the input file!\n");
        return 1;
    }

    fseek(fin, 0, SEEK_END); // set the stream indicator to the end of the file
    size_t length = ftell(fin); // set length to stream indicator's position
    if (length < 1) {
        printf("input file is empty\n");
        fclose(fin);
        return 1;
    }
    rewind(fin); // put the indicator back to the beginning of the file
    
    char *fmem = malloc(length * sizeof(*fmem));
    if (!fmem) {
        printf("memory allocation failed!\n");
        fclose(fin);
        return 1;
    }
    fread(fmem, sizeof(*fmem), length, fin);

    const char firstByte = fmem[0];
    const char *outExtension = getOutExtension(firstByte);

    char* dot = strrchr(argv[1], '.');
    if (dot != NULL)
        *dot = '\0';

    size_t len = strlen(argv[1]) + strlen(outExtension) + 1;
    char* out = malloc(len * sizeof(*out));
    if (!out) {
        printf("memory allocation failed!\n");
        fclose(fin);
        return 1;
    }

    strcpy(out, argv[1]);
    strcat(out, outExtension);

    FILE* fout = fopen(out, "wb");
    if (!fout) {
        printf("could not open the output file for write!\n");
        fclose(fin);
        return 1;
    }

    switch (firstByte) {
        case PNG_MAGIC:
        case MP4_MAGIC:
        case SDT_MP4_MAGIC:
        case SDT_PNG_MAGIC:
            shift(100, length, fmem, fout);
            break;
        case SRC_MAGIC:
        case TXT_MAGIC:
            shift(length, length, fmem, fout);
            break;
        default:
            printf("unknown file type, shifting all characters\n");
            shift(length, length, fmem, fout);
    }

    free(fmem);
    fclose(fout);
    fclose(fin);
    printf("wrote to %s\n", out);
    free(out);
    return 0;
}