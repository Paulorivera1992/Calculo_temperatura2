#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>       /* round() */
#include <time.h> 
#include "tiffio.h"

double max(double a, double b, double c);
double countingSort (double *numbers, int size, double *B, double *C,int perc);
void *TF(char *filename, double *temper, double *radg);
void leer_matriz_T(double **T, int numero_muestras);
void calculo_temperatura(double *R,double *G,double *B,int num_pixel, double *Temperatura , double *radgV2);
double Radp_estimado(double **T,double R[3],int num_muestras);
void leer_vector(double *R, int num_medidas, char *ruta);
double trapezoidal(double *inten, double *longi, double *calib, int min, int max);
double calculo_radt(char *filename1, char *filename2, char *filename3);

///////////////////funcion principal////////////////////////////////////////
int main(int argc, char* argv[])
{
  //archivos iniciales
  time_t t;
  struct tm *tm;
  char datos_bufet[300];
  FILE* bufet_file;
  bufet_file = fopen(argv[5], "at");
  char tempe_direct[100];
  char tempe_spect[100];
  char rad_direct[100];
  char rad_spect[100];
  char rad1[100];
  char radt[100];
  char soot[100];
  double temperatura[2];
  double radg[3];
  double Radt=0;
  double sot=0; //hollin
  
  //obtencion de temperatura y radg
  TF(argv[1],temperatura,radg);
  Radt=calculo_radt(argv[2],argv[3],argv[4]);
  
  //transformo mediciones en cadenas de texto
  gcvt(temperatura[0], 15, tempe_direct); 
  gcvt(temperatura[1], 15, tempe_spect); 
  gcvt(radg[0], 15, rad_direct); 
  gcvt(radg[1], 15, rad_spect); 
  gcvt(radg[2], 15, rad1); 
  gcvt(Radt, 15, radt);
  gcvt(sot, 15, soot); 
  
  //marca de tiempos
  t=time(NULL);
  tm=localtime(&t);
  strftime(datos_bufet, 100, "%d/%m/%Y %H:%M:%S", tm);
  
  //creacion de string a escribir en archivo con fomato tiempo valor
  strcat(datos_bufet," ");
  strcat(datos_bufet,tempe_direct);
  strcat(datos_bufet," ");
  strcat(datos_bufet,tempe_spect);
  strcat(datos_bufet," ");
  strcat(datos_bufet,rad_direct);
  strcat(datos_bufet," ");
  strcat(datos_bufet,rad_spect);
  strcat(datos_bufet," ");
  strcat(datos_bufet,rad1);
  strcat(datos_bufet," ");
  strcat(datos_bufet,radt);
  strcat(datos_bufet," ");
  strcat(datos_bufet,soot);
  strcat(datos_bufet,"\n");
  
  //escritura de datos
  fputs(datos_bufet,bufet_file);

  //cerramos los archivos
  fclose(bufet_file); 

  return 0;
}

//////////////////////calculo de vector V//////////////////////////////////
double max(double a, double b, double c) {
   return ((a > b)? (a > c ? a : c) : (b > c ? b : c));
}

////////////////////calculo de percentiles///////////////////////////
double countingSortMain (double *numbers, double k, int size, double *B, double *C,int perc) {
    int i, j, indexB = 0;
    B = (double*) malloc(sizeof(double) * (size + 1));
    C = (double*) calloc((k + 1), sizeof(double));
    for (i = 0; i < size; i++) {
        C[(int)numbers[i]] = C[(int)numbers[i]] + 1;
    }
    for (i=0; i <= k; ++i) {
        for(j=0; j < C[i]; ++j) {
            B[indexB] = i;
            indexB++;
        }
    }
    int x= perc*(size)/100;
    double percentil=(B[x-1]);
    free(B);
    free(C);
    return percentil;
}

double countingSort (double *numbers, int size, double *B, double *C,int perc) {
    int i; 
    double k=-1.0;
    for (i = 0; i < size; i++) {
        if (numbers[i] > k) {
            k = numbers[i];
        }
    }
    return countingSortMain(numbers, k, size, B, C, perc);
}

//////////////////////calculo de temperatura /////////////////////////////////
void *TF(char *filename,double *temp, double *radg){
  int ancho, alto;
	int numero_pixeles;
  double* matriz_R;
  double* matriz_G;
  double* matriz_B;
  double* value;
  
  ////////////////////cargar imagen .tiff en matriz de colores RGB//////////////////////////////////
  TIFF* tiff; 
  TIFF * tif = TIFFOpen (filename, "r");
  if (tif) {
	  uint32 * raster;
	  TIFFGetField (tif, TIFFTAG_IMAGEWIDTH, &ancho);
	  TIFFGetField (tif, TIFFTAG_IMAGELENGTH, &alto);
	  numero_pixeles = ancho * alto;      
    matriz_R=malloc(numero_pixeles*sizeof(double));
    matriz_G=malloc(numero_pixeles*sizeof(double));
    matriz_B=malloc(numero_pixeles*sizeof(double));
    value=malloc(numero_pixeles*sizeof(double));
    
	  raster = (uint32*) _TIFFmalloc(numero_pixeles* sizeof (uint32));
	  if (raster != NULL) {
	    if (TIFFReadRGBAImage (tif, ancho, alto, raster, 0)) {
         for(int i=0; i<numero_pixeles; i++){
           matriz_B[i] = (double)TIFFGetB(raster[i]);
           matriz_G[i] = (double)TIFFGetG(raster[i]);
           matriz_R[i] = (double)TIFFGetR(raster[i]);
           value[i] = max(matriz_R[i], matriz_G[i], matriz_B[i]);
         }
	    }
	    _TIFFfree (raster);
	  }
	TIFFClose (tif);
  }
  
    
  /////////////////calculo de percentiles/////////
  double delta_min;
  double delta_max;
  double* b1=malloc(numero_pixeles*sizeof(double));
  double* c1=malloc(numero_pixeles*sizeof(double));
  delta_min=countingSort(value,numero_pixeles, b1, c1,96);
  delta_max=countingSort(value,numero_pixeles, b1, c1,100);
  
  ////aplicación de mascara////////////////////////
  int n=0;
  for(int i=0;i<numero_pixeles;i++){
    if(value[i]<delta_max && value[i]>delta_min){
      matriz_B[i]=matriz_B[i]/255;
      matriz_G[i]=matriz_G[i]/255;
      matriz_R[i]=matriz_R[i]/255;
      n=n+1;
      }
    else{
      matriz_B[i]=0;
      matriz_G[i]=0;
      matriz_R[i]=0;
    }    
  }
  
 
  //////////calculo temperatura//////////////////////7
  calculo_temperatura(matriz_R,matriz_G, matriz_B,numero_pixeles,temp,radg);
    
  free(b1);
  free(c1);
  free(value);
  free(matriz_R);
  free(matriz_G);
  free(matriz_B);
}

/////////////////calculo de temperaturas///////////////////////////////////
void calculo_temperatura(double *R,double *G,double *B,int num_pixel, double *Temperatu, double *radgV2){
  double lambda_B=473.5*pow(10,-9);
  double lambda_G=540*pow(10,-9);
  double lambda_R=615*pow(10,-9);
  double pl1=580*pow(10,-9);
  double pl2=620*pow(10,-9);
  int i1 = 813; //espectro en 580 nm
  int i2 = 989; // espectro en 620 nm
  double sigma=5.67*pow(10,-8);      // constante de Boltzmann
  double Ap=4.8*pow(10,-6)*4.8*pow(10,-6); //area del pixel de la camara
  int epsilon=1;          // emisividad de llama
  int t_amb=25+273;       // temperatura ambiente
  double **T = NULL;
  int num_muestras=1339;
  T=(double **)malloc(num_muestras*sizeof(double *)); 
  // se reserva memoria para cada fila
 	for (int i = 0; i < num_muestras; i = i + 1) {
    	T[i]= (double*)malloc(3*sizeof(double));
  	}
  leer_matriz_T(T, num_muestras);
  double suma_tf_rec_spect=0; 
  double suma_tf_direct=0;
  double suma_Radp_direct=0;
  double suma_Radp_rec_spect=0;
  double suma_Radp2=0;
  double maxTf=3000;         // limite superior de Temperatura
  double minTf=1000;         // limite inferior de Temperatura
  double c2=1.4385*pow(10,-2); 
  double C=0.0214;
  int numero_no_zero=0;
  int numero_no_zero1=0;
  double rho[3];
  double Temperatura[2];
  double suma=0;
  for(int i=0;i<num_pixel;i++) {
    rho[0]=(double)R[i];
    rho[1]=(double)0.7677*G[i];
    rho[2]=(double)0.3197*B[i];
    if(rho[0] >= 0.03 && rho[0] < 0.99 && rho[1] >= 0.01 && rho[1] < 0.99){
      numero_no_zero=numero_no_zero+1;
      numero_no_zero1=numero_no_zero1+1;
      /***************************temperatura direct*****************************/
      double SS=0.3653*(pow((rho[0]/(rho[1]/0.7677)),2))-1.669*(rho[0]/(rho[1]/0.7677)) + 3.392;   // factor de forma obtenido experimental
      double num=c2*(1/(lambda_G)-1/(lambda_R));               //numerador de la ecuacion
      double denom=log(rho[0]/(rho[1]/0.7677)) + 6*log(lambda_R/lambda_G) + log(SS); //denominador de la ecuacion
      double temp_direct=num/denom; //temperatura calculada
      //test sobrepaso limites
      if(isnan(temp_direct)){//si es un nand
        suma_tf_direct=suma_tf_direct;
        numero_no_zero=numero_no_zero-1;      
      }
      else if (temp_direct>maxTf) {// nivel superior
        suma_tf_direct=suma_tf_direct + maxTf;
        }
      else if(temp_direct<minTf){ //nivel inferior
        suma_tf_direct=suma_tf_direct + minTf;
        }
      else {//minTf < Tf < maxTf valor correcto
        suma_tf_direct=suma_tf_direct + temp_direct;
        suma_Radp_direct=suma_Radp_direct+Ap*epsilon*sigma*(pow(temp_direct,4) - pow(t_amb,4));//calculo de radiacion de imagen
        } 
     /*********************temperatura recuperación espectral****************/  
      double pE1 = T[i1-1][0]*rho[0]+ T[i1-1][1]*rho[1]+ T[i1-1][2]*rho[2]; //faltan los valores de T
      double pE2 = T[i2-1][0]*rho[0]+ T[i2-1][1]*rho[1]+ T[i2-1][2]*rho[2]; //faltan los valores de T
      num=(c2*(pl1-pl2)/(pl1*pl2));//numerador de la ecuacion
      denom=(log((pE1*pow(pl1,5))/(pE2*pow(pl2,5))));//denominador de la ecuacion
      double temp_re=num/denom; //temperatura calculada
      //test sobrepaso limites
      if(isnan(temp_re)){//si es un nand
        suma_tf_rec_spect=suma_tf_rec_spect;
        numero_no_zero1=numero_no_zero1-1;   
      }
      else if (temp_re>maxTf) {// nivel superior
        suma_tf_rec_spect=suma_tf_rec_spect + maxTf;  
      }
      else if(temp_re<minTf){ //nivel inferior
        suma_tf_rec_spect=suma_tf_rec_spect + minTf;
      }    
      else {//minTf < Tf < maxTf valor correcto
        suma_tf_rec_spect=suma_tf_rec_spect + temp_re;
        suma_Radp_rec_spect=suma_Radp_rec_spect+Ap*epsilon*sigma*(pow(temp_re,4) - pow(t_amb,4));//calculo de radiacion de imagen
        suma_Radp2=suma_Radp2+Radp_estimado(T,rho,num_muestras);
        
        
      }  
      
    } 
    /************************************************************************/
    else{
       suma_tf_direct=suma_tf_direct;
       suma_tf_rec_spect=suma_tf_rec_spect;
    }
  }
  Temperatu[0]= suma_tf_direct/numero_no_zero;
  Temperatu[1]= suma_tf_rec_spect/numero_no_zero1;  
  if(numero_no_zero==0){
    Temperatu[0]= 0;
    Temperatu[1]= 0; 
  }
  radgV2[0]=suma_Radp_direct;
  radgV2[1]=suma_Radp_rec_spect;
  radgV2[2]=C*suma_Radp2;
  for (int i = 0; i < num_muestras; i = i + 1) {
    	free(T[i]);
  	}
  free(T);  
  //printf("la suma del espectro estimado es radp2 es: %0.15f \n",suma_Radp2);
  //printf("la temperatura es: %0.15f \n",suma_tf_rec_spect);
}

//////////carga elementos de la matriz T/////////////////////////////
void leer_matriz_T(double **T, int numero_muestras){ 
  FILE* fichero;
  fichero = fopen("T.txt", "r");
  for(int a=1; !feof(fichero); a++){
      if(a>numero_muestras) break;
      fscanf(fichero,"%lf %lf %lf",&T[a-1][0],&T[a-1][1],&T[a-1][2]);
    }
  fclose(fichero);  
}

//////calculo radp con espectro estimado//////////////////////////77
double Radp_estimado(double **T,double R[3],int num_muestras){
  double Espectro_E[num_muestras];
  double Radp=0;
  //double sum=0;
  for(int i=0;i<num_muestras;i++){
    Espectro_E[i]=T[i][0]*R[0]+ T[i][1]*R[1]+ T[i][2]*R[2];
    //sum=sum+Espectro_E[i];
  }
  for(int j=1;j<num_muestras;j++){
    Radp=Radp+0.5*(Espectro_E[j]+Espectro_E[j-1]);
  }
  //printf("la suma de E es: %0.15f \n",sum);
  return Radp;
}

//////////carga elementos de vector/////////////////////////////
void leer_vector(double *R, int num_medidas,char *ruta){ 
  FILE* fichero;
  int a;
  fichero = fopen(ruta, "r");
  for(a=1; !feof(fichero); a++){
      if(a>num_medidas) break;
      fscanf(fichero,"%lf",&R[a-1]);
    }
  fclose(fichero);
}

double trapezoidal(double *inten, double *longi, double *calib, int min, int max){
  double area=0;
  for(int j=min;j<max;j++){
   area=area+0.5*((longi[j]-longi[j-1])*((calib[j]*inten[j])+(calib[j-1]*inten[j-1])));
  }
  return area;
}

double calculo_radt(char *filename1, char *filename2, char *filename3){
  int numero_datos=1024;
  int i_min=1;
  int i_max=1024;
  double *intensidades=(double *)malloc(numero_datos*sizeof(double)); 
  double *longitud=(double *)malloc(numero_datos*sizeof(double)); 
  double *calibracion=(double *)malloc(numero_datos*sizeof(double));
  leer_vector(intensidades,numero_datos,filename1);
  leer_vector(longitud,numero_datos,filename2);
  leer_vector(calibracion,numero_datos,filename3);
  double radt=0.001*trapezoidal(intensidades,longitud,calibracion,i_min,i_max);
  if(radt<=0){
    radt=0;
  }
  //printf("el radt es: %0.10f \n", radt);
  return radt; 
}