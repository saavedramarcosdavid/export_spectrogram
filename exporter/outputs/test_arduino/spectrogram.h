// File spectrogram.h
#ifndef _SPECTROGRAM_H
#define _SPECTROGRAM_H

#include "fft.h"

// Constantes autogeneradas
#ifndef N_FFT
#define CONVERT_TO_DB 0
#define N_FFT 128
#define N_MELS 16
#define FRAME_LENGTH 2322
#define SAMPLE_RATE 5000
#define BLOCKS 36
#define N_FFT_TABLE 64
#define NOVERLAP 64
#define STEP 64
#define LEN_NFFT_NMELS 4
#define SPEC_SIZE 576
#define TS_US 200
#endif

void create_spectrogram(float *data, float *result);

#endif
	