import Funciones 
import subprocess
from time import time
from pypylon import pylon
from opcua import Client
from opcua.client.ua_client import UaClient
from opcua import ua
import numpy as np 
from datetime import datetime
import socket
import logging
from scipy import misc
import math
import abel
from scipy.stats import mode
from scipy.stats import moment
from scipy.stats import skew
from scipy.stats import kurtosis
from skimage.transform import resize
from skimage.transform import rescale
from seabreeze.spectrometers import Spectrometer

nombre_archivo_imagen='/home/ubuntu/calculo_temperatura/imagenes_llama/Llama.tiff'
nombre_archivo_intensidad='/home/ubuntu/calculo_temperatura/espectro_llama/espectro.txt'
nombre_archivo_wavelengths='/home/ubuntu/calculo_temperatura/espectro_llama/wavelength.txt'
nombre_archivo_calibracion='/home/ubuntu/calculo_temperatura/espectro_llama/calib.txt'
nombre_archivo_configuracion='/home/ubuntu/calculo_temperatura/configuracion.txt'
nombre_archivo_buffet='/home/ubuntu/calculo_temperatura/archivos_buffet/Bufet.txt'
nombre_archivo_buffet_soot='/home/ubuntu/calculo_temperatura/archivos_buffet/Bufet_soot.txt'

completed = subprocess.run(['./calculo_Tf',nombre_archivo_imagen,nombre_archivo_intensidad,nombre_archivo_wavelengths,nombre_archivo_calibracion,nombre_archivo_buffet], check=True, cwd="/home/ubuntu/calculo_temperatura/") 