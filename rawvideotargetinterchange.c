/*
Take a raw 320x200 greyscale bitmap as a keyframe or pattern table and this one will give out index numbers to the most similiar
5x5 pixel blocks in that image. Reconstruct it with the other tool. Take a look at the quality and file size and wonder.
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


unsigned char bild[64000];
unsigned char bild2[128000];


FILE *encoded;


void put_code(unsigned long int code)
{
 static int count=0;
 static unsigned char buf_1, buf_2,buf_3;

 if( count==0 )
 {
  buf_1=(unsigned char)  (code%256);
  buf_2=(unsigned char) ((code/256))%16;
 }
 else
 if( count==1 )
 {
  buf_2+=(unsigned char) (code%16)*16;
  buf_3=(unsigned char) (code/16);

  fputc( buf_1, encoded);
  fputc(buf_2, encoded);
  fputc( buf_3, encoded);

 }


 count++;
 (count)&=1;
	
	
	
}


float mvalue, mvalue_sum;

int main(int argc, char*argv[])
{

  FILE *input, *input2;
  signed int n=0, n2=0,copy_dest, copy_source;
  int x_add, y_add;
  int blocks=0;
  unsigned int kachel=0;
  unsigned int kachelfreeze;

  double abweich=0.0, old_abweich=1000000.0, pixdiff ;

  
  if ( argc != 3 )
  {
   printf("Falsche Anzahl Argumente!\n");
   return 1;
  
  }


  input=fopen(argv[1],"rb");
  input2=fopen(argv[2],"rb");
  encoded=fopen(".\\encoded","wb");

  if ( input == NULL )
  {
   printf("Datei nicht gefunden!\n");
   return 1;
  }

 fread( bild2, sizeof(unsigned char), 128000, input2);

 fclose(input2);


fread( bild, sizeof(unsigned char), 64000, input);
 fclose(input);

  n=0;

 n2=0;
 
 while ( n2 < 64000)
 {
   old_abweich=100000000.0 ;


  kachel=0;
   n=0;
  while ( n < 64000 )
  {

    x_add=0, y_add=0;

     abweich=0;

      while ( y_add < 5 )
      {
       x_add=0;
       while ( x_add < 5 )
       {

        abweich+= fabs( bild[ (n2+x_add) %320+ (n2/320+y_add)*320  ]- bild2[ (n+x_add) %320+ (n/320+y_add)*320  ] );
          if ( abweich > old_abweich) break;
        x_add++;
       }
       if ( abweich > old_abweich) break;
       y_add++;

       }

    if ( abweich < old_abweich )
    {
     old_abweich=abweich;
     copy_source=n2;
     copy_dest=n;
     kachelfreeze=kachel;
     }

     n+=5;
     
     kachel++;
     if ( n%320 ==0 && n > 0 ) n+= (320*4);

       
    }

    printf("%d\n",kachelfreeze);    
    put_code(kachelfreeze);  
	   
   n2+=5;
   if ( n2%320 ==0 && n2 > 0 ) n2+= ( 320*4 );

 }   
 
 
 put_code(0xfff), put_code(0xfff);
 
 printf("128001\n");
 
 fclose(encoded);
}
