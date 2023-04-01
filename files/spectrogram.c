#include "spectrogram.h"

void create_spectrogram(float *data, float *result){
    register int i,j,k;
    float aux;
    
    float data_re[N_FFT];
    float data_im[N_FFT];
    
    for(i=0;i<BLOCKS;i++){
        // Copiar los valores a la entrada de la fft
        const unsigned int start = i*STEP;
        for(j=0;j<N_FFT;j++){
            data_re[j] = data[start+j];
            data_im[j] = 0;
        }

        // Realizar fft
        fft(data_re,data_im,N_FFT);

        // Calcular el modulo de la fft
        for(j=0;j<N_FFT;j++){
            const float aux_re = data_re[j];
            const float aux_im = data_im[j];
            data_re[j] = sqrt(aux_re*aux_re + aux_im*aux_im);
        }

        // Procesamiento de N_MELS
        const unsigned int start2 = i*N_MELS;
        for(j=0;j<N_MELS;j++){
            const unsigned int start3 = j*LEN_NFFT_NMELS;
            aux = 0;
            for(k=0;k<LEN_NFFT_NMELS;k++){
                aux += data_re[start3+k];
            }
            aux /= LEN_NFFT_NMELS;
            #if CONVERT_TO_DB
                result[start2+j] = 10*log10(aux);
            #else
                result[start2+j] = aux;
            #endif
        }
    }
}
