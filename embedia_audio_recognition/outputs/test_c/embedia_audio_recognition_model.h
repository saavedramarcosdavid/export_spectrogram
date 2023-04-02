
/* EmbedIA model - Autogenerado */
#ifndef EMBEDIA_AUDIO_RECOGNITION_MODEL_H
#define EMBEDIA_AUDIO_RECOGNITION_MODEL_H

#include "embedia.h"
#define EMBEDIA_AUDIO_RECOGNITION_MODEL_CHANNELS 1
#define EMBEDIA_AUDIO_RECOGNITION_MODEL_HEIGHT 12
#define EMBEDIA_AUDIO_RECOGNITION_MODEL_WIDTH 16
void model_init();

int model_predict(data_t input, flatten_data_t * results);

#endif
