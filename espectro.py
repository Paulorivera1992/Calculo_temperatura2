from time import time
from seabreeze.spectrometers import Spectrometer, list_devices
import numpy as np
import matplotlib.pyplot as plt
from time import sleep
import Funciones 

def open_spect():
  try:
      spec = Spectrometer.from_first_available()
      print("conectando")
      
      return spec
      
  except: #excepcion en caso de que el espectrometro no este conectada
      #logger.error("Error conectando el espectrometro") 
      print("no se pudo conectar")
      return 0 

def save_spect(spec):
  try:
      wavelengths = spec.wavelengths() #lee longitudes de onda wavelengths in (nm)
      intensities = spec.intensities() #lee intensidades measured intensity array in (a.u.)

      #f = open (ruta_longitud,'wb')
      #np.savetxt(f, np.array(wavelengths))
      #f.close()
      #f = open (ruta_intensidad,'wb')
      #np.savetxt(f, intensities)
      #f.close()
      #return True
      #print("todo bien")
      return spec
      
  except: #excepcion en caso de que el espectrometro no este conectada
      #logger.error("Error conectando el espectrometro") 
      #return False  
      #print("error de lectura")
      return 0
      
 
#start_time = time()
#asd=open_spect()  
#tiempo_espectrometro= time() - start_time 
#print(tiempo_espectrometro)
logger=1
nombre_archivo_intensidad='/home/ubuntu/calculo_temperatura/espectro_llama/espectro.txt'
nombre_archivo_wavelengths='/home/ubuntu/calculo_temperatura/espectro_llama/wavelength.txt'
nombre_archivo_calibracion='/home/ubuntu/calculo_temperatura/espectro_llama/calib.txt'
asd=Funciones.open_spect(logger)
for i in range(5000):
  asd=Funciones.obtener_espectro(asd,nombre_archivo_intensidad,nombre_archivo_wavelengths,logger)
  print(asd)
  #if(asd!=0):
    #start_time = time()
   # asd=save_spect(asd)
    #tiempo_espectrometro= time() - start_time
  #else:
   # start_time = time()
    #asd=open_spect()  
    #tiempo_espectrometro= time() - start_time 
    #print(tiempo_espectrometro)
  #spec = Spectrometer.from_serial_number("S05507")
  #spec = Spectrometer.from_first_available()
  #spec = Spectrometer.from_serial_number()
  
  #spec.integration_time_micros(20000)


  #print(tiempo_espectrometro)
  #sleep(5)

 # f = open ('longitud'+str(i)+'.txt','wb')
 # np.savetxt(f, np.array(wavelengths))
 # f.close()
 # f = open ('intensidad'+str(i)+'.txt','wb')
 # np.savetxt(f, intensities)
 # f.close()

  #plt.plot(wavelengths,intensities)
  #plt.xlabel('wavelengths')
  
  #plt.ylabel('intensities')
  #plt.title('Lab DLS')
#plt.show()