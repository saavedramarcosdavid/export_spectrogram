# import necessary libraries
from librosa import load
from spectrogram.melspec import Melspec
from spectrogram.melspec_export import Melspec_export
from spectrogram.melspec_export_options import *

print("Bienvenidos a la generacion del futuro")

# parameters for the creation of Melspec
n_mels = 16
n_fft = 128
noverlap = 64

signal, fs = load('nine.wav', sr = 5000)

# creating a Melspec instance and spectrogram processing
melspec = Melspec(n_fft, n_mels, noverlap)
melspec.report()
melspec.calculate_params(len(signal),fs, report=True)
#melspec.process_melspectrogram(signal,fs, report = True)
#melspec.plot(f"Spectrogram shape: {melspec.shape}")

# export options
options = ProjectOptions()
options.project_type = ProjectType.C # puede ser ARDUINO tambien
options.data_type = DataType.FLOAT # not implemented yet
options.files = ProjectFiles.ALL
options.example_data = signal
options.example_type = 'float' # it cannot be other than "float". Not implemented
options.baudrate = 9600 # only if project_type is Arduino

# creation of the exporter and execution of the export to code c
output_folder = "outputs"
project_name = "test"
ms_export = Melspec_export(melspec, options)
ms_export.export(output_folder, project_name)

print("End")