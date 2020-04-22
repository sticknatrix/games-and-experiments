/*

    Reconstruct the twisted image created by your encoder.

*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


unsigned char bild[128000];
unsigned char maske[64000];
unsigned char ausgabe[64000];


float mvalue, mvalue_sum;


FILE *encoded;


unsigned long get_code()
{
 static int count=0;
 static unsigned char to_convert[3];
 unsigned int inbufbuffer[2];

 if ( count==0)
 {
  to_convert[0]=fgetc(encoded);
  to_convert[1]=fgetc(encoded);
  to_convert[2]=fgetc(encoded);
 
  inbufbuffer[count]   = to_convert[0];
  inbufbuffer[count]  += (to_convert[1]%16)*256;
  inbufbuffer[count+1] = to_convert[1]/16;
  inbufbuffer[count+1]+= to_convert[2]*16;
 }

 count++;
 (count)&=1;
	
	if ( count==1) return inbufbuffer[0];
                   else return inbufbuffer[1];
	
	
}




int main(int argc, char*argv[])
{

  FILE *input, *output;
  signed long int n=0, n2=0,copy_dest, copy_source;
  int x_add, y_add;
  
  unsigned char kachel_upper,kachel_lower;
  unsigned int kachelnum=0;

  double abweich=0.0, old_abweich=1000000.0, pixdiff ;

  input=fopen(argv[1],"rb");
  
  if ( argc != 2 )
  {
   printf("Falsche Anzahl Argumente!\n");
   return 1;
  
  }


  if ( input == NULL )
  {
   printf("Datei nicht gefunden!\n");
   return 1;
  }

 fread( bild, sizeof(unsigned char), 128000, input);
 fclose(input);

 encoded=fopen(".\\encoded","rb");

  n=0;
 while(n<64000) { maske[n]=0; ausgabe[n]=255; n++; }
 
 n2=0;
 copy_source=0;
 while ( copy_source!= 128001 )
 {
 
   /*	
 	kachel_upper=fgetc(encoded);
    kachel_lower=fgetc(encoded);
    */
   //scanf("%c%c", &kachel_upper,&kachel_lower );
   kachelnum=get_code(); //(unsigned int)kachel_upper*256+kachel_lower;
   copy_dest=0;
   
    if ( kachelnum==4095 ) { break; }
 
   while ( kachelnum > 0)
   {
   copy_dest+=5;
   if ( copy_dest%320 == 0 ) copy_dest+=320*4;
   	
   	kachelnum--;
   }
   
     x_add=0, y_add=0;
     while ( y_add < 5 )
     {
      x_add=0;
      while ( x_add < 5 )
      {
        ausgabe[ (copy_source+x_add) %320+ (copy_source/320+y_add)*320  ]= bild[ (copy_dest+x_add)%320+ (copy_dest/320+y_add)*320  ] ;
         x_add++;
        }
       y_add++;
      }

   copy_source+=5;
   if ( copy_source%320 == 0 ) copy_source+=320*4;
 }  

  output=fopen("outback.raw","wb");
  fwrite(ausgabe,sizeof(unsigned char),64000, output);
  fclose(output);
  fclose(encoded);


}
