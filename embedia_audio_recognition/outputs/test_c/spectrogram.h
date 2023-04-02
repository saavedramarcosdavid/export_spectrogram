// File spectrogram.h
#ifndef _SPECTROGRAM_H
#define _SPECTROGRAM_H

#include "fft.h"

// Constantes autogeneradas
#ifndef N_FFT
#define CONVERT_TO_DB 0
#define N_FFT 512
#define N_MELS 16
#define FRAME_LENGTH 5000
#define SAMPLE_RATE 5000
#define BLOCKS 12
#define N_FFT_TABLE 256
#define NOVERLAP 128
#define STEP 384
#define LEN_NFFT_NMELS 16
#define SPEC_SIZE 192
#define TS_US 200
#endif

void create_spectrogram(float *data, float *result);

#endif
	