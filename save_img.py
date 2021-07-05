from pypylon import pylon
import platform
import time
from time import time
from time import sleep

def configurar(camara):
  if(camara!=0):
      # exposure time
      camara.ExposureTimeAbs.SetValue(3000.0)# En microsegundos forma generica="camera.ExposureTime.SetValue(3500.0)";
                 
      # acquisition frame rate 
      camara.AcquisitionFrameRateEnable.SetValue(True)
      camara.AcquisitionFrameRateAbs.SetValue(30.0) #Fotogramas por segundo. forma generica="cam.AcquisitionFrameRate.SetValue(100.00000);"
          
      # gain
      camara.GainAuto.SetValue("Off")#("Off", "Once", "Continuous").
      camara.GainRaw.SetValue(10); #Ganancia en dB en el rango 0 a 240 para 12 bits y 360 para 8 bit. forma generica="cam.Gain.SetValue(100)"
          
      #configuracion de parametros de tamano
      camara.Width.SetValue(1920)    #en pixeles
      camara.Height.SetValue(1200)   #en pixeles
      camara.OffsetX=0 #debe ser multiplo de 2
      camara.OffsetY=0  #debe ser multiplo de 2

def guardar_imagen(camara,ruta):
  try:
      #inicia captura de imagenes
      camara.StartGrabbing()
      #result = camara.GrabOne(2000, pylon.TimeoutHandling_ThrowException)#timeout 2000 ms
      result=camara.RetrieveResult(2000, pylon.TimeoutHandling_ThrowException)
  
      #codigo para guardar la imagen
      img = pylon.PylonImage()        
      img.AttachGrabResultBuffer(result)     
      img.Save(pylon.ImageFileFormat_Tiff, ruta)
      img.Release()
      print("foto obtenida") 
      camara.StopGrabbing()
      return camara
  except:
      sleep(1)
      print("error adquiriendo la imagen")
      return 0
  

def conectar_camara():
  tlf = pylon.TlFactory.GetInstance()
  try:
      start_time = time()
      cam = pylon.InstantCamera(tlf.CreateFirstDevice())
      tiempo_camara= time() - start_time
      print(tiempo_camara)
      try:
          cam.Open()
          #configurar(cam)
          #guardar_imagen(cam,ruta)
          return cam
      except:
          print("error abriendo la camara")
          return 0
          #cam.StopGrabbing()
          #cam.Close()
     # finally:
          #cam.StopGrabbing()
      #    return 0
      #    cam.Close()          
  except: 
      print("error de conexion") 
      return 0  

def obtener_imagen(camara,ruta):
    if(camara!=0):
        start_time = time()
        camara=guardar_imagen(camara,filename)
        tiempo_camara= time() - start_time
        print(tiempo_camara)
        sleep(0.01)
        return camara
    else:
        camara=conectar_camara() 
        configurar(camara)
        return camara
      

filename = "imagenes_llama/saved_pypylon_img_1.tiff"

#while True:
camara=conectar_camara()
print(camara)
configurar(camara)
for i in range(300):
    camara=obtener_imagen(camara,filename)
    

