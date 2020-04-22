/* detect pitch and generate a program that plays a tune.
    In reality, the results aren't as good as there are sometimes resonant fifths an octave higher that are louder
	 than the root note. And the beep() function also is slow.
	 
	 8 bit unsigned .snd 44100hz input*/


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char signal[3000000];
float signalb[16000];

void interpolate(long int start, long int count)
{
 signed long int buf, count2, off=0, end=0;

 end=4000/count;

while (off<end)
{
  buf=0;
  count2=count;
 while (count2 > 0){ 
   buf+=signal[start+count2+off*count]; count2--; }

   buf/=count;
   if ( buf > 255) buf=255;
   if ( buf < 0 ) buf=0;

  while ( count2 < count)
 {
  signalb[count2+off*count]=(unsigned char)buf;
 count2++;
 }
 off++;
 

}

}

float sintab[128];

void sininit(void)
{
 float value;
 value=-6.4;

 while ( value < 6.4 )
 {
  sintab[(int)((value+6.4)*10.0)]=sin(value);
  value+=0.1;
 }

}


inline float SIN(float value)
{
 signed int sign;
 signed int index;

 if ( value < 0 ) sign=-1;
 else sign=+1;
  index= ((int)( value*10 )%64);
  index+=63;

 return sintab[index];
}

float filterin[16000];
float ausschlag[12][5];
float tone,octave;


void interpolateOTT(long int count)
{
 float buf;
 signed long int count2, off=0, end=0;

 end=4000/count;

while (off<end)
{
  buf=0;
  count2=count;
 while (count2 > 0){ 
   buf+=filterin[count2+off*count]; count2--; }

   buf/=count;
   /*
   if ( buf > 255) buf=255;
   if ( buf < 0 ) buf=0;*/

  while ( count2 < count)
 {
  filterin[count2+off*count]=buf;
 count2++;
 }
 off++;
 

}

}



unsigned short int output[3000000];

int main(int argc, char*argv[])
{
  printf("#include <windows.h>\n int main(void){");

  FILE *input, *input2;
  unsigned int n=0, n2=0, n3=0, randoff=0, runs=0, filesize;
  float steigung, frequenz, frequenz_gen[1500], 
  frequenz_ausgeb[1500],
  stoplen, maxsteigung;
float compfrequenz, smallest,
      biggest, loudest[1500],
	  buf, lowersum;

unsigned int pos;

float most_silent;

  
  if ( argc != 2 )
  {
   printf("Falsche Anzahl Argumente!\n");
   return 1;
  
  }

  sininit();
  input=fopen(argv[1],"rb");


  if ( input == NULL )
  {
   printf("Datei nicht gefunden!\n");
   return 1;
  }
  

 fread( signal, sizeof(unsigned char), 3000000, input);
   fseek(input, 0 , SEEK_END);
 filesize = ftell(input);
  if ( filesize > 3000000) { printf("Eingabedatei zu gross.\n");
   return 1;
  }
 fclose(input);


  n=0;
  while ( n < filesize) output[n]=32767,n++;


  n=0;
  while ( n < filesize )
  {
  	runs=0;
  	
#define BANDS 1
 
 pos=0;
 
   n3=0;
   while ( n3 < 1000) loudest[n3]=0.0, n3++;
 n2=0, n3=0;
 unsigned int ton=0, oktave=0;
 
 float CUTOFF;
 
 CUTOFF=4000.0;
 unsigned int first=0;
 first=0;
 
   compfrequenz=200;
    tone=1.0;
    octave=1.0;
    ton=0, oktave=0;
	while ( compfrequenz < CUTOFF)
   {
   	
   	
   	
   	 runs++;
     stoplen=44100/compfrequenz;
     frequenz= 3.14 /(44100/compfrequenz) ;
    
   n2=0;
   while ( n2 < 16000 )
   {
    filterin[n2]=0.0;n2++;
    
   }

#define OVERSKIPP  (44100.0/(compfrequenz*4)) 

   interpolate(n,OVERSKIPP);  
   
   
   float signalc[32000];
   
                       
   
    
    n3=OVERSKIPP;
     while ( n3 < 4000)
    {
     steigung= signalb[n3]-signalb[n3-(int)(OVERSKIPP)];
  	
     if ( fabs(steigung) > 0.0)
	 {	        
	 float nfloat;
	 nfloat=0.0;
     n2=0;
     while (  n2 < 4000  ) 
     {
     buf=  SIN((float)n2*frequenz)*steigung;
       filterin[ (int)(n2+n3)  ]+=buf; 
       
	   n2+=OVERSKIPP;
       
	  }
    }
     n3+=OVERSKIPP;
    }

    n2=0;
    smallest=0.0;
    biggest=0.0;
   
     while ( n2< 8000 )
    {
	  if ( filterin[n2]/(compfrequenz) < smallest) smallest=filterin[n2]/(compfrequenz); 
      if ( filterin[n2]/(compfrequenz) > biggest ) biggest=filterin[n2]/(compfrequenz); 
     n2++;
    }
    
 
   smallest+=100000.0, biggest+=100000.0;
    steigung=fabs(biggest-smallest);
   
   frequenz_gen[pos]=3.14/(44100/compfrequenz), frequenz_ausgeb[pos]=compfrequenz, loudest[pos]=steigung;
   pos++;
   
   
   lowersum+=steigung;
   
   
   compfrequenz*=1.02;
  }


n3=1;

while ( n3 < 1000)
{
 n2=1000;
 while (n2 > n3)
 {
  if ( loudest[n2] > loudest[n2-1] ) 
    {
    	n3=0;
   	
    	float fbuf, fgbuf, lobuf;
  
 	 fbuf=frequenz_gen[n2-1], fgbuf=frequenz_ausgeb[n2-1], lobuf=loudest[n2-1];
	 frequenz_gen[n2-1]=frequenz_gen[n2], frequenz_ausgeb[n2-1]=frequenz_ausgeb[n2],loudest[n2-1]=loudest[n2],
	 frequenz_gen[n2]=fbuf, frequenz_ausgeb[n2]=fgbuf, loudest[n2]=lobuf; 
   }
  n2--;
  }
  
  n3++;
 }

  n3=0;
  while ( n3 < BANDS)
  {
  
  
   {
   
    n2=0;
    
    randoff=rand()%4000;
    n2=0;
	while ( n2 < 4000 ){ 
    output[n2+n]+=(signed int)(SIN((float)n2*frequenz_gen[n3]+randoff)*loudest[n3]*10.0 ); n2++;}

     printf("Beep(%d,100); ", (int)frequenz_ausgeb[n3]/2, (int)(loudest[n3]*10.0));
    }
  
   n3++; 
  }  
  
  n+=4000;
  
    

    

  }

printf("}");

  input2=fopen(".\\mp3.raw", "wb");
  fwrite( output, sizeof(unsigned char), filesize*2, input2);
  fclose(input2);

}
