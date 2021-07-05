from opcua import Client
from opcua.client.ua_client import UaClient
from opcua import ua
import numpy as np 
from datetime import datetime
import time
from os import system

def leer_valor(server,nodo):
  client = Client("opc.tcp://192.168.0.101:4840")
  try:
      client.connect()#conecta el servidor
      var=client.get_node("ns=1;s="+nodo)#se obtiene el primer nodo
      variable2=var.get_children()
      tamano=np.size(variable2)
      if tamano>0:
        tiempo=variable2[0].get_value()#tiempo
        #date_time = tiempo.strftime('%d/%m/%Y %H:%M:%S')
        tfd=variable2[1].get_value()#tf_direct
        tfrs=variable2[2].get_value()#tf_rec_spect
        rgtfd=variable2[3].get_value()#rg_tf_direct
        rgtfrs=variable2[4].get_value()#rg_tf_rec_spect
        rgrs=variable2[5].get_value()#rg_rec_spect
        rt=variable2[6].get_value()#rt
        sp=variable2[7].get_value()#soot propensity
        ec=variable2[8].get_value()#status camara
        ee=variable2[9].get_value()#status espectrometro
        avg=variable2[10].get_value()#avg soot propensity
        median=variable2[11].get_value()#median soot propensity
        std=variable2[12].get_value()#std soot propensity
        mode=variable2[13].get_value()#mode soot propensity
        p50=variable2[14].get_value()#percentil 50 soot propensity
        m2=variable2[15].get_value()#segundo momento soot propensity
        m3=variable2[16].get_value()#tercer momento soot propensity
        m4=variable2[17].get_value()#cuarto momento soot propensity
        s1=variable2[18].get_value()#primer coeficiente de asimetria
        s2=variable2[19].get_value()#segundo coeficiente de asimetria
        s3=variable2[20].get_value()#coeficiente asimetria
        k1=variable2[21].get_value()#coeficiente de kurtosis
        
        system("clear")
        print("\n\n\n Tiempo:                 ",tiempo,"\n tf_direct:              ",tfd,"\n tf_rec_spect:           ",tfrs,"\n Rg_tf_direc:            ",rgtfd)
        print(" Rg_tf_rec_spec:         ",rgtfrs,"\n RG_rec_spect:           ",rgrs,"\n Rt:                     ",rt,"\n hollin:                 ",sp)
        print(" Estado camara:          ",ec,"\n Estado Espectrometro:   ",ee)
        print(" Avg Soot Propensity:    ",avg,"\n Median Soot Propensity: ",median)
        print(" Std Soot Propensity:    ",std,"\n Mode Soot Propensity:   ",mode)
        print(" P50 Soot Propensity:    ",p50,"\n M2 Soot Propensity:     ",m2)
        print(" M3 Soot Propensity:     ",m3,"\n M4 Soot Propensity:     ",m4)
        print(" S1 Soot Propensity:     ",s1,"\n S2 Soot Propensity:     ",s2)
        print(" S3 Soot Propensity:     ",s3,"\n K Soot Propensity:      ",k1,"\n")
      else:
        system("clear")
        print("No se encuentra el nodo en el servidor opc")
      
      client.disconnect()
  
  except: 
      system("clear")
      print("No se puede conectar con el servidor opc")

while(True):
  leer_valor("opc.tcp://192.168.1.107:4840","sensor 10")     
  time.sleep(1) 
  