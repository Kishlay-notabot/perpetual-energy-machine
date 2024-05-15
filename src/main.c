#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "SAM/src/reciter.h"
#include "SAM/src/sam.h"
#include "SAM/src/debug.h"

// taken from SAM/src/main.c

int debug = 0;

// Approximations of some Windows functions to ease portability
#if defined __GNU_LIBRARY__ || defined __GLIBC__
static int min(int l, int r) { return l < r ? l : r; }
static void strcat_s(char * dest, int size, char * str) {
    unsigned int dlen = strlen(dest);
    if (dlen >= size-1) return;
    strncat(dest+dlen, str, size - dlen - 1);
}
void fopen_s(FILE ** f, const char * filename, const char * mode) {
    *f = fopen(filename,mode);
}
#endif

void WriteWav(char* filename, char* buffer, int bufferlength) {
	unsigned int filesize;
	unsigned int fmtlength = 16;
	unsigned short int format=1; //PCM
	unsigned short int channels=1;
	unsigned int samplerate = 22050;
	unsigned short int blockalign = 1;
	unsigned short int bitspersample=8;

	FILE *file;
	fopen_s(&file, filename, "wb");
	if (file == NULL) return;
	//RIFF header
	fwrite("RIFF", 4, 1,file);
	filesize=bufferlength + 12 + 16 + 8 - 8;
	fwrite(&filesize, 4, 1, file);
	fwrite("WAVE", 4, 1, file);

	//format chunk
	fwrite("fmt ", 4, 1, file);
	fwrite(&fmtlength, 4, 1, file);
	fwrite(&format, 2, 1, file);
	fwrite(&channels, 2, 1, file);
	fwrite(&samplerate, 4, 1, file);
	fwrite(&samplerate, 4, 1, file); // bytes/second
	fwrite(&blockalign, 2, 1, file);
	fwrite(&bitspersample, 2, 1, file);

	//data chunk
	fwrite("data", 4, 1, file);
	fwrite(&bufferlength, 4, 1, file);
	fwrite(buffer, bufferlength, 1, file);

	fclose(file);
}

int main() {
    char* wavfilename = "lure.wav";
    char input[256];

    strcat_s((char*)input, sizeof(input), "I really really really like C++");
    strcat_s((char*)input, sizeof(input), " ");
    strcat_s((char*)input, 256, "[");
	if (!TextToPhonemes(input)) return 1;

    SetInput(input);
    if (!SAMMain()){
		return 1;
	}

    WriteWav(wavfilename, GetBuffer(), GetBufferLength()/50);
}
