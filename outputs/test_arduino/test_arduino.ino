#include "spectrogram.h"
#include "example_data.h"

void printVector(float *, int);

void setup(){
	Serial.begin(9600);

	// Process signal, get spectrogram
	float spectrogram[SPEC_SIZE];
	create_spectrogram(example_data, spectrogram);
	printVector(spectrogram, SPEC_SIZE);
	
}

void loop(){}

void printVector(float * vector, int N){
	int i;
	Serial.print("vector = np.array([");
	Serial.print("    ");
	Serial.print(vector[0]);
	for(i=1;i<N;i++){
		Serial.print(", ");
		Serial.print(vector[i]);
	}
	Serial.print("])");
	Serial.print(" ");
}
