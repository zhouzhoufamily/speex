#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "speex/speex_callbacks.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef FIXED_DEBUG
extern long long spx_mips;
#endif

#define FRAME_SIZE 160
#include <math.h>

// wav header struct  
struct wave_header {

	char riff[4];
	unsigned long fileLength;
	char wavTag[4];
	char fmt[4];
	unsigned long size;
	unsigned short formatTag;
	unsigned short channel;
	unsigned long sampleRate;
	unsigned long bytePerSec;
	unsigned short blockAlign;
	unsigned short bitPerSample;
	char data[4];
	unsigned long dataSize;

};


void *createWaveHeader(int fileLength, short channel, int sampleRate, short bitPerSample)
{

	struct wave_header *header = malloc(sizeof(struct wave_header));

	if (header == NULL) {
		return  NULL;
	}

	// RIFF  
	header->riff[0] = 'R';
	header->riff[1] = 'I';
	header->riff[2] = 'F';
	header->riff[3] = 'F';

	// file length  
	header->fileLength = fileLength + (44 - 8);

	// WAVE  
	header->wavTag[0] = 'W';
	header->wavTag[1] = 'A';
	header->wavTag[2] = 'V';
	header->wavTag[3] = 'E';

	// fmt  
	header->fmt[0] = 'f';
	header->fmt[1] = 'm';
	header->fmt[2] = 't';
	header->fmt[3] = ' ';

	header->size = 16;
	header->formatTag = 1;
	header->channel = channel;
	header->sampleRate = sampleRate;
	header->bitPerSample = bitPerSample;
	header->blockAlign = (short)(header->channel * header->bitPerSample / 8);
	header->bytePerSec = header->blockAlign * header->sampleRate;

	// data  
	header->data[0] = 'd';
	header->data[1] = 'a';
	header->data[2] = 't';
	header->data[3] = 'a';

	// data size  
	header->dataSize = fileLength;

	return header;
}

int encFun(char *inFile, char *outFile)
{
	FILE *fin, *fout, *fbits = NULL;//file streans
	short in[FRAME_SIZE];
	float input[FRAME_SIZE];
	char cbits[200];
	int nbBits;
	void *state;//encode state
	SpeexBits bits;
	int i, tmp;
	int bitCount = 0;//count bit bytes

	state = speex_encoder_init(speex_lib_get_mode(SPEEX_MODEID_NB));

	tmp = 8;
	speex_encoder_ctl(state, SPEEX_SET_QUALITY, &tmp);

	fin = fopen(inFile, "rb");
	fout = fopen(outFile, "wb+");

	speex_bits_init(&bits);
	while (!feof(fin))
	{
		fread(in, sizeof(short), FRAME_SIZE, fin);
		if (feof(fin))
			break;

		for (size_t i = 0; i < FRAME_SIZE; i++)
			input[i] = in[i];

		speex_bits_reset(&bits);

		speex_encode(state, input, &bits);
		nbBits = speex_bits_write(&bits, cbits, 200);
		bitCount += bits.nbBits;

		speex_bits_reset(&bits);

		fwrite(&nbBits, sizeof(int), 1, fout);
		fwrite(cbits, 1, nbBits, fout);
	}
	fprintf(stderr, "Total encoded size: %d bits\n", bitCount);
	speex_encoder_destroy(state);
	speex_bits_destroy(&bits);

	fclose(fin);
	fclose(fout);

	return 1;
}

int decFun(char *inFile, char *outFile)
{
	FILE *fin, *fout, *fbits = NULL;//file streans
	short out[FRAME_SIZE];
	float output[FRAME_SIZE];
	char cbits[200];
	int nbBits;
	void *state;//decode state
	SpeexBits bits;
	int i, tmp;
	int bitCount = 0;//count bit bytes

	state = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_NB));

	tmp = 1;
	speex_decoder_ctl(state, SPEEX_SET_ENH, &tmp);

	fin = fopen(inFile, "rb");
	fout = fopen(outFile, "wb+");

	//write wav header
	char *header = createWaveHeader(0, 1, 8000, 16);
	fwrite(header, 1, 44, fout);

	speex_bits_init(&bits);
	while (!feof(fin))
	{
		fread(&nbBits, sizeof(int), 1, fin);
		if (feof(fin))
			break;

		fread(cbits, 1, nbBits, fin);

		speex_bits_read_from(&bits, cbits, nbBits);

		speex_decode(state, &bits, output);

		for (size_t i = 0; i < FRAME_SIZE; i++)
			out[i] = output[i];

		bitCount += bits.nbBits;

		fwrite(out, sizeof(short), FRAME_SIZE, fout);
	}
	fprintf(stderr, "Total encoded size: %d bits\n", bitCount);
	speex_decoder_destroy(state);
	speex_bits_destroy(&bits);

	//write bit bytes.
	fseek(fout, 40, SEEK_SET);
	fwrite(&bitCount, 1, 4, fout);

	fseek(fout, 4, SEEK_SET);
	bitCount += 36;
	fwrite(&bitCount, 1, 4, fout);

	fclose(fin);
	fclose(fout);

	return 1;
}

int main(int argc, char **argv)
{
	if (argc != 5 && argc != 4)
	{
		fprintf(stderr, "Usage: encode [-e or -d] [in file] [out file] [bits file]\nargc = %d", argc);
		exit(1);
	}

	if (strcmp("-e", argv[1]) == 0)
	{
		encFun(argv[2], argv[3]);
	}
	else if (strcmp("-d", argv[1]) == 0)
	{
		decFun(argv[2], argv[3]);
	}
	else if (strcmp("-ed", argv[1]) == 0)
	{
		encFun(argv[2], argv[3]);
		decFun(argv[3], argv[4]);
	}
	else
	{
		fprintf(stderr, "Usage: encode [-e or -d] [in file] [out file] [bits file]\nargc = %d", argc);
		exit(1);
	}

   return 0;
}
