#include <stdio.h>
#include "spectrogram.h"
#include "example_data.h"

void printVector(float *, int);

int main(int argc, char* argv[]){

	// Process signal, get spectrogram
	float spectrogram[SPEC_SIZE];
	create_spectrogram(example_data, spectrogram);
	printVector(spectrogram, SPEC_SIZE);
	
	return 0;
}

void printVector(float * vector, int N){
	printf("vector = np.array([  ");
	printf("%.5f", vector[0]);
	for(int i=1;i<N;i++){
		printf(", %.5f", vector[i]);
	}
	printf("])    ");

}
