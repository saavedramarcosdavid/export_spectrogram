#include <stdio.h>
#include "embedia.h"
#include "example_data.h"
#include "spectrogram.h"
#include "embedia_audio_recognition_model.h"

// Structure with input data for the inference function
data_t input = {  
                  EMBEDIA_AUDIO_RECOGNITION_MODEL_CHANNELS, 
                  EMBEDIA_AUDIO_RECOGNITION_MODEL_WIDTH, 
                  EMBEDIA_AUDIO_RECOGNITION_MODEL_HEIGHT, 
                  example_data
};

// Structure with inference output results
flatten_data_t results;

int main(void){

  // model initialization
  model_init();
  
  // Process signal, get spectrogram 
  float spectrogram[SPEC_SIZE];
  create_spectrogram(example_data,spectrogram);
  input.data = spectrogram;   

  // model inference
  int prediction = model_predict(input, &results);    
    
  printf("Prediction class id: %d\n", prediction); 

  return 0;
}