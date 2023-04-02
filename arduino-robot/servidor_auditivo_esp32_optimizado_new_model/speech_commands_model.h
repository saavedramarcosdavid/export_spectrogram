/* EmbedIA model - Autogenerado */
#ifndef SPEECH_COMMANDS_MODEL_H
#define SPEECH_COMMANDS_MODEL_H

#include "embedia.h"
#define SPEECH_COMMANDS_MODEL_CHANNELS 1
#define SPEECH_COMMANDS_MODEL_HEIGHT 19
#define SPEECH_COMMANDS_MODEL_WIDTH 32
  
void model_init();

int model_predict(data_t input, flatten_data_t * results);

  
#endif
