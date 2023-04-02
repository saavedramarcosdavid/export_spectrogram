import os
import shutil

from mix_export.mix.mix_exporter_options import *

class Mix_export:
	'''
		Class Mix_export.
	'''
	def __init__(self, options):
		'''
			Class constructor.

			Parameters
			----------
			options: 
				options for export (not implement yet)
		'''
		self.options = options
		self.files_path = "mix_export/files"

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
			Function to export the main program in charge of the execution of 
			an inference using a spectrogram of an audio that will be preprocessed

			Parameters
			----------
			output_folder:
				Name of the destination folder.
			project_name:
				Project's name.
		'''
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

		# extension of files to copy/create and copy main.c
		if self.options.project_type == ProjectType.C:
			(h_ext, c_ext)  = ('.h', '.c')
			shutil.copy(os.path.join(src_folder,'main_c.c'), os.path.join(dst_folder,'main'+c_ext))
		else:
			if self.options.project_type == ProjectType.ARDUINO:
				shutil.copy(os.path.join(src_folder,'main_arduino.c'), os.path.join(dst_folder,project_name+".ino"))
			else:
				(h_ext, c_ext)  = ('.h', '.cpp')
				shutil.copy(os.path.join(src_folder,'main_c.c'), os.path.join(dst_folder,'main'+c_ext))