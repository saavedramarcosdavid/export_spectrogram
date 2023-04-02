// file spectrogram.h
#ifndef _SPECTROGRAM_H
#define _SPECTROGRAM_H

#include "fft.h"
#include <stdlib.h>

#define CONVERT_TO_DB 0

// Constantes autogeneradas: 
#ifndef N_FFT
#define N_MELS 32
#define N_FFT 256
#define N_FMIN 0
#define N_FMAX 0
#define FRAME_LENGTH 5000
#define SAMPLE_RATE 5000
#define BLOCKS 19
#define N_FFT_TABLE 128
#define NOVERLAP 0
#define STEP 256
#define LEN_NFFT_NMELS 4
#define SPEC_SIZE 608
#define DELAY_US 200
#endif

void create_spectrogram(float *data, float *result);

#endif