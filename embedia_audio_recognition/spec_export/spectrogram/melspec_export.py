import numpy as np
import os
import shutil

from spec_export.spectrogram.melspec import Melspec
from spec_export.spectrogram.melspec_export_options import *

class Melspec_export:
	'''
		Class that receives an instance of the Melspec
		class and transforms it into c code.
	'''
	def __init__(self, melspec_instance, options):
		'''
			Class constructor.

			Parameters
			----------
			melspec_instance: 
				Melspec class instance to transform.
		'''
		self.melspec_instance = melspec_instance
		self.options = options
		self.files_path = "spec_export/files"

	def _save_to_file(self, filename, content):
		'''
			Save content to a file named filename

			Parameters
			----------
			filename: 
				Name of the file where the data will be saved.
			content: 
				Content of the file to save.
		'''
		file = open(filename, 'w', encoding='utf-8') #guardar
		file.write(content)
		file.close() 

	def _read_from_file(self, filename):
		'''
			Read and return the content of a file.

			Parameters
			----------
			filename: 
				Name of the file to be read.
		'''
		file = open(filename, 'r', encoding='utf-8') #guardar
		content = file.readlines()
		file.close()  
		return content

	def export(self, output_folder, project_name):
		'''
			Function to export the spectrogram library 
			using melspec_instance and loaded options.
			If melspec_instance.ready () returns negative, 
			export () ends in error since the instance 
			must contain data.

			Parameters
			----------
			output_folder:
				Name of the destination folder.
			project_name:
				Project's name.
		'''
		assert self.melspec_instance.ready(), "melspec_instance must have data"

		# create output folder if doesnt exists
		if output_folder[-1] != '/':
			output_folder+='/'
		if not os.path.exists(output_folder):
			os.mkdir(output_folder)
		# Arduino require a project folder with same name as main file
		# if self.options.project_type == ProjectType.ARDUINO:
		output_folder+=project_name+'/'
		if not os.path.exists(output_folder):
			os.mkdir(output_folder)
			 
		# absolute path for copying files
		dst_folder = os.path.abspath(output_folder)
		src_folder = os.path.abspath(self.files_path)

		# extension of files to copy/create
		if self.options.project_type == ProjectType.C:
			(h_ext, c_ext)  = ('.h', '.c')
		else:
			(h_ext, c_ext)  = ('.h', '.cpp')
			
				
		# copy library files       
		if ProjectFiles.LIBRARY in self.options.files:
			
			# fft files
			shutil.copy(os.path.join(src_folder,'fft.h'), os.path.join(dst_folder,'fft'+h_ext))
			shutil.copy(os.path.join(src_folder,'fft.c'), os.path.join(dst_folder,'fft'+c_ext))

			# spectrogram files
			header_spec = self.melspec_header_c(self.options.convert_to_db)
			self._save_to_file(os.path.join(dst_folder,'spectrogram'+h_ext), header_spec)
			shutil.copy(os.path.join(src_folder,'spectrogram.c'), os.path.join(dst_folder,'spectrogram'+c_ext))
		
		# create main file with an example
		if ProjectFiles.MAIN in self.options.files:

			# create example file from data
			example_data_c_code = self._vector_to_c(self.options.example_data, 'example_data', self.options.example_type)
			self._save_to_file(os.path.join(dst_folder,'example_data'+h_ext), example_data_c_code)

			# create c code from main 
			c_main = self.melspec_main_c()
			
			# change project extension for arduino
			main_name = "main"
			if self.options.project_type == ProjectType.ARDUINO:
				c_ext ='.ino'
				main_name = project_name
			self._save_to_file(output_folder+main_name+c_ext, c_main)

	def melspec_header_c(self, convert_to_db = False):
		'''
			Function that generates a string with the header 
			and constants necessary for the C program.

			Return
			------
				h:
					String header for C program.
		'''
		if convert_to_db: 
			convert_to_db = 1
		else:
			convert_to_db = 0

		h = f'''// File spectrogram.h
#ifndef _SPECTROGRAM_H
#define _SPECTROGRAM_H

#include "fft.h"

// Constantes autogeneradas
#ifndef N_FFT
#define CONVERT_TO_DB {convert_to_db}
#define N_FFT {self.melspec_instance.n_fft}
#define N_MELS {self.melspec_instance.n_mels}
#define FRAME_LENGTH {self.melspec_instance.input_length}
#define SAMPLE_RATE {self.melspec_instance.input_fs}
#define BLOCKS {self.melspec_instance.n_blocks}
#define N_FFT_TABLE {int(self.melspec_instance.n_fft/2)}
#define NOVERLAP {self.melspec_instance.noverlap}
#define STEP {self.melspec_instance.step}
#define LEN_NFFT_NMELS {(self.melspec_instance.n_fft//2)//self.melspec_instance.n_mels}
#define SPEC_SIZE {self.melspec_instance.shape[0]*self.melspec_instance.shape[1]}
#define TS_US {int(1/self.melspec_instance.input_fs*1000*1000)}
#endif

void create_spectrogram(float *data, float *result);

#endif
	'''
		return h

	def _vector_to_c(self, input_data, name_vector, variable_type):
		'''
			It receives a vector and transforms it into a vector in c code.

			Parameters
			----------
			input_data: 
				Data input to convert.
			name_vector:
				Name that the vector of c will have.
			variable_type:
				Type of variable to use.

			Returns
			-------
			vector_c: String
				Generated string.
		'''
		vector_c = f'{variable_type} {name_vector}[] = {{\n\t'
		s=''
		for i in input_data:
			i_string = "%.7f, " % (i)
			s+=i_string
		s = s[:-2]
		vector_c += s + '\n};'
		
		return vector_c

	def _melspec_main_c_generic(self):
		'''
			Class method to build indistinct code between the different ProjectTypes.

			Returns
			-------
			code: String
				Generic code.
		'''
		code = '''
	// Process signal, get spectrogram
	float spectrogram[SPEC_SIZE];
	create_spectrogram(example_data, spectrogram);
	printVector(spectrogram, SPEC_SIZE);
	'''
		return code

	def _melspec_main_c_printVector(self):
		'''
			It generates a string that contains the printVector 
			function according to the type of project.

			Returns
			-------
			code:
				String with c code of the printVector function.
		'''
		

		if self.options.project_type == ProjectType.C:
			nt = "  "#repr('\n\t')[1:-1]
			nn = "    "#repr('\n\n')[1:-1]

			code = f'''
void printVector(float * vector, int N){{
	printf("vector = np.array([{nt}");
	printf("%.5f", vector[0]);
	for(int i=1;i<N;i++){{
		printf(", %.5f", vector[i]);
	}}
	printf("]){nn}");

}}
'''
			
		elif self.options.project_type == ProjectType.ARDUINO:
			ln = ''#'ln'
			code = f'''
void printVector(float * vector, int N){{
	int i;
	Serial.print{ln}("vector = np.array([");
	Serial.print("    ");
	Serial.print(vector[0]);
	for(i=1;i<N;i++){{
		Serial.print(", ");
		Serial.print(vector[i]);
	}}
	Serial.print{ln}("])");
	Serial.print{ln}(" ");
}}
'''
		return code

	def melspec_main_c(self):
		'''
			Function that generates and returns a string with c, c ++ 
			or arduino code depending on the ProjectType option.

			Returns
			code:
				String with c code of the main test program.
		'''
		code = ''

		if self.options.project_type == ProjectType.C:
			code += '#include <stdio.h>\n'

		code += '#include "spectrogram.h"\n'
		code += '#include "example_data.h"\n\n'
		code += 'void printVector(float *, int);\n\n'

		if self.options.project_type == ProjectType.C:
			code += 'int main(int argc, char* argv[]){\n'
		elif self.options.project_type == ProjectType.ARDUINO:
			code += 'void setup(){\n\tSerial.begin('+str(self.options.baudrate)+');\n'

		code += self._melspec_main_c_generic()

		if self.options.project_type == ProjectType.C:
			code += "\n\treturn 0;\n}\n"
			
		elif self.options.project_type == ProjectType.ARDUINO:
			code += "\n}\n\nvoid loop(){}\n"
			
		code += self._melspec_main_c_printVector()

		return code