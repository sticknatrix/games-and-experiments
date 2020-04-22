/*
  Man erzeugt ein 256-Farben-Graustufen-BMP und loescht den Header raus.
  Der Graustufenheader muss eine lineare Graustufenpalette enthalten.
  Der Header, das sind die ersten paar Bytes, sodass unten 64000 Byte uebrigbleiben.
  Mit dem Hex-Editor kann man es meist gut erkennen, wo er aufhoert.

  Das Bitmap muss die Aufloesung 320x200 haben.

  An der Kommandozeile gibt man ein: Programmname [Eingangsrohdatei ohne Header]
   [Ausgabedatei].

   Man kann versuchen, die Ausgabedatei mit RAR oder ZIP zu komprimieren und sieht:
   es ist genauso gut wie JPEG.

   Dann kopiert man den alten Header vor die Ausgabedatei und das Bitmap ist fertig.

   Mit SCHWELLE kann man die Toleranzschwelle fuer den Flood-Fill festlegen.

   Ganz einfach: Flood-Fill, bis der tiefste erfasste Wert mit dem Hoechsten eine
   frei waehlbare Diskrepanz erreicht.
   Den vom Flood-Fill erfassten Bereich fuellt man mit dessen arithmetischen Mittel-
   Wert.

   MPEG ueberlappt Folgebilder zu einem 3D-Array und tut da dasselbe.
   Auf die Bewegungssuche ist geschissen. Sie braucht nur einen Haufen Rechenleistung.

   Sie kochen alle nur mit Wasser.


*/



#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCHWELLE schwelle
#define SHIFT 3
#define TOINIT 257

unsigned char bild[64000];
unsigned char maske[64000];
unsigned char ausgabe[64000];

int iter_buf[64000][4];
int iter_count;

unsigned int x_sized, y_sized, x_start,y_start;

float mvalue, mvalue_sum;
float hellst, dunkelst;
long int rec_count, old_rec_count;
signed long int abbruch;
long int highest_depth;
int dumped;

  int distance=0;

long int schwelle;


void dumpnum(int x, int y, unsigned char wert )
{
 int stelle=0;
 int x_add,y_add;
 int digindex, nullflag;

 char font[250]={
                 0,1,1,1,0,
                 1,0,0,0,1,
                 1,0,0,0,1,
                 1,0,0,0,1,
                 0,1,1,1,0,

                 0,1,1,1,0,
                 1,0,0,1,0,
                 0,0,0,1,0,
                 0,0,0,1,0,
                 0,0,0,1,0,

                 0,1,1,1,0,
                 1,0,0,0,1,
                 0,0,0,1,0,
                 0,0,1,0,0,
                 0,1,1,1,1,

                 0,1,1,1,0,
                 1,0,0,0,1,
                 0,0,1,1,0,
                 1,0,0,0,1,
                 0,1,1,1,0,

                 0,1,0,0,1,
                 1,0,0,0,1,
                 0,1,1,1,1,
                 0,0,0,0,1,
                 0,0,0,0,1,

                 1,1,1,1,0,
                 1,0,0,0,0,
                 1,1,1,1,0,
                 0,0,0,0,1,
                 1,1,1,1,0,

                 0,1,1,1,1,
                 1,0,0,0,0,
                 1,1,1,1,0,
                 1,0,0,0,1,
                 0,1,1,1,0,

                 1,1,1,1,1,
                 0,0,0,1,0,
                 0,1,1,1,0,
                 0,0,1,0,0,
                 0,1,0,0,0,

                 0,1,1,1,0,
                 1,0,0,0,1,
                 0,1,1,1,0,
                 1,0,0,0,1,
                 0,1,1,1,0,

                 0,1,1,1,0,
                 1,0,0,0,1,
                 0,1,1,1,0,
                 0,0,1,0,0,
                 1,1,0,0,0  };

                 printf("drin...\n");
  stelle=3;nullflag=0;
  wert= (wert+10)/64;
  while ( stelle > 0 )
  {
   if ( stelle==3 )
     {
      digindex= (( wert/100)%10)*25;
      if ( digindex!=0 ) nullflag=1;
      }
   if ( stelle==2 )
    {
      digindex= (( wert/10)%10)*25;
     if ( digindex!=0 ) nullflag=1;
    }
   if ( stelle==1 )
    {
      digindex= (( wert/1)%10)*25;
      if ( digindex!=0 ) nullflag=1;
    }

     if ( nullflag==1 || stelle==1 )
     {
      y_add=0;
      while ( y_add < 5 )
      {
       x_add=0;
       while ( x_add < 5 )
       {
        if ( x+x_add < 320 && y+y_add < 200 )
          if( font[digindex+x_add+(4-y_add)*5]==1 )
            ausgabe[x+x_add+ (y+y_add)*320]=127;
        x_add++;
       }
       y_add++;
      }

      stelle--;
      x+=6;
      }

      if ( nullflag==0 ) stelle--;


  }

 printf("dumnum() verlassen...\n");

}


void backsort(void)
{
 int count1, count2;
 int buf[4];

 count2=0;

 while ( count2 < iter_count )
 {

   count1=iter_count-1;
  while ( count1 > count2 )
  {

  if ( iter_buf[count1][2] < iter_buf[count1-1][2] )
   {

     buf[0]=iter_buf[count1-1][0];
     buf[1]=iter_buf[count1-1][1];
     buf[2]=iter_buf[count1-1][2];
     buf[3]=iter_buf[count1-1][3];

     iter_buf[count1-1][0]=iter_buf[count1][0];
     iter_buf[count1-1][1]=iter_buf[count1][1];
     iter_buf[count1-1][2]=iter_buf[count1][2];
     iter_buf[count1-1][3]=iter_buf[count1][3];

     iter_buf[count1][0]=buf[0];
     iter_buf[count1][1]=buf[1];
     iter_buf[count1][2]=buf[2];
     iter_buf[count1][3]=buf[3];

   }

   count1--;

  }

  count2++;

 }

 /* Kettendumpen debug

 count2=0;
 while( count2 < iter_count )
 {
  printf("Position %d X: %d Y: %d Aktivierung %d Zaehlerwert %d\n",
         count2, iter_buf[count2][0], iter_buf[count2][1], iter_buf[count2][3],
          iter_buf[count2][2] );

  count2++;
 }

 bis hier */

}


void insert(int x, int y, int depth)
{
  float buf;

 if ( hellst==TOINIT ) hellst=bild[x+y*320];
 if ( dunkelst==TOINIT ) dunkelst=bild[x+y*320];
 
  if ( bild[x+y*320] > hellst )
    /* if ( abs( (float)bild[x+y*320]-dunkelst ) < SCHWELLE  ) */
        hellst=bild[x+y*320];
     /*    else return; */
     
  if ( bild[x+y*320] < dunkelst )
    /* if ( abs( (float)hellst-bild[x+y*320] ) < SCHWELLE  ) */
        dunkelst=bild[x+y*320];
     /*    else return; */

     if ( rec_count > 0 )
     {
      buf= mvalue_sum/rec_count;


         /*
      if ( abs ( hellst-buf ) > SCHWELLE ) { return; }
      if ( abs ( dunkelst-buf ) > SCHWELLE ) { return; }

         */

      if ( rec_count > 1200 ){ausgabe[x+y*320]< 65 ? (ausgabe[x+y*320]=64 ):(ausgabe[x+y*320]=0 ); maske[x+y*320]=1;return ; }


       if ( abs ( bild[x+y*320] - buf ) > SCHWELLE ) { ausgabe[x+y*320]< 65 ?(ausgabe[x+y*320]=64) : (ausgabe[x+y*320]=0) ;maske[x+y*320]=1; return; }


     }


  mvalue_sum+=bild[x+y*320];
  rec_count++;

   maske[x+y*320]=1;
   


 iter_buf[iter_count][0]=x;
 iter_buf[iter_count][1]=y;
 iter_buf[iter_count][2]=depth;
 iter_buf[iter_count][3]=0;
 iter_count++;

}






void dinsert(int x, int y, int depth )
{
  float buf;

 if ( hellst==TOINIT ) hellst=bild[x+y*320];
 if ( dunkelst==TOINIT ) dunkelst=bild[x+y*320];
 
  if ( bild[x+y*320] > hellst )
    /* if ( abs( (float)bild[x+y*320]-dunkelst ) < SCHWELLE  ) */
        hellst=bild[x+y*320];
     /*    else return; */
     
  if ( bild[x+y*320] < dunkelst )
    /* if ( abs( (float)hellst-bild[x+y*320] ) < SCHWELLE  ) */
        dunkelst=bild[x+y*320];
     /*    else return; */

       if ( rec_count==old_rec_count/3 && dumped==0 ) dumped=1,  dumpnum(x-3 > 0 ? x - 3 : 0,y-3 > 0 ? y-3 : 0, mvalue);

     if ( rec_count > 0 )
     {
      buf= mvalue_sum/rec_count;


         /*
      if ( abs ( hellst-buf ) > SCHWELLE ) { return; }
      if ( abs ( dunkelst-buf ) > SCHWELLE ) { return; }

         */

      if ( rec_count > 1200 ){
          return;    }

       if ( abs ( bild[x+y*320] - buf ) > SCHWELLE )
       {

          return; }


     }

  rec_count++;



   mvalue_sum+=bild[x+y*320];
   maske[x+y*320]=1;



 iter_buf[iter_count][0]=x;
 iter_buf[iter_count][1]=y;
 iter_buf[iter_count][2]=depth;
 iter_buf[iter_count][3]=0;
 iter_count++;

}







int flood(int x, int y, int depth )
{
  int buf2; int buf3;

  if ( iter_count==0 ) insert ( x, y, depth );


   buf2=0; while ( iter_buf[buf2][3]==1 ) buf2++;

 x=iter_buf[buf2][0];
 y=iter_buf[buf2][1];
 depth=iter_buf[buf2][2];
 iter_buf[buf2][3]=1;


 /* printf("%d %d \n", x,y ); */



    if ( x < 319 ) if ( maske[x+1+y*320]==0 )
    {
     insert( x+1, y , iter_buf[0][2]+1 );

     }
    if ( x > 0 )  if ( maske[x-1+y*320]==0 )
    {
     insert( x-1, y , iter_buf[0][2]+1 );
  
    }
    if ( y < 199 ) if ( maske[x+(y+1)*320]==0 )
    {
     insert( x, y+1 , iter_buf[0][2]+1 ); y_sized++;

    }
    if ( y > 0 ) if ( maske[x+(y-1)*320]==0 )
    {
     insert( x, y-1 , iter_buf[0][2]+1 ); y_sized++;

    }


    /*
    
    if ( x < 319 && y < 199 ) if ( maske[x+1+(y+1)*320]==0 )
    {
     insert( x+1, y +1, depth+1 );

     }
    if ( x > 0 && y > 0)  if ( maske[x-1+(y-1)*320]==0 )
    {
     insert( x-1, y-1 , depth+1 );
  
    }
    if ( y < 199 && x > 0) if ( maske[(x-1)+(y+1)*320]==0 )
    {
     insert( x-1, y+1 , depth+1 );

    }
    if ( y > 0 && x < 319 ) if ( maske[(x+1)+(y-1)*320]==0 )
    {
     insert( x+1, y-1 , depth+1 );

    }

     */



       abbruch= -1;
   buf3=0; while ( buf3 < iter_count ) { if ( iter_buf[buf3][3]==0 ) abbruch=1; buf3++; }


}


int dither(int x, int y, int depth )
{
  int buf2; int buf3;

  if ( iter_count==0 ) dinsert ( x, y, depth );


   buf2=0; while ( iter_buf[buf2][3]==1 ) buf2++;

 x=iter_buf[buf2][0];
 y=iter_buf[buf2][1];
 depth=iter_buf[buf2][2];
 iter_buf[buf2][3]=1;


 /* printf("%d %d \n", x,y ); */



    if ( x < 319 ) if ( maske[x+1+y*320]==0 )
    {
     dinsert( x+1, y , depth+1 );

     }
    if ( x > 0 )  if ( maske[x-1+y*320]==0 )
    {
     dinsert( x-1, y , depth+1 );
  
    }
    if ( y < 199 ) if ( maske[x+(y+1)*320]==0 )
    {
     dinsert( x, y+1 , depth+1 ); y_sized++;

    }
    if ( y > 0 ) if ( maske[x+(y-1)*320]==0 )
    {
     dinsert( x, y-1 , depth+1 ); y_sized++;

    }


    /*
    if ( x < 319 && y < 199 ) if ( maske[x+1+(y+1)*320]==0 )
    {
     dinsert( x+1, y+1 , depth+1 );

     }
    if ( x > 0 && y > 0)  if ( maske[x-1+(y-1)*320]==0 )
    {
     dinsert( x-1, y-1 , depth+1 );
  
    }
    if ( y < 199 && x > 0) if ( maske[(x-1)+(y+1)*320]==0 )
    {
     dinsert( x-1, y+1 , depth+1 );

    }
    if ( y > 0 && x < 319 ) if ( maske[(x+1)+(y-1)*320]==0 )
    {
     dinsert( x+1, y-1 , depth+1 );

    }


      */


       abbruch= -1;
   buf3=0; while ( buf3 < iter_count ) { if ( iter_buf[buf3][3]==0 ) abbruch=1; buf3++; }


}


void reserflood( int x, int y)
{

  if ( hellst==TOINIT ) hellst=bild[x+y*320];
 if ( dunkelst==TOINIT ) dunkelst=bild[x+y*320];
 
  if ( bild[x+y*320] > hellst )
        hellst=bild[x+y*320];
     
  if ( bild[x+y*320] < dunkelst )
        dunkelst=bild[x+y*320];


    if ( abs(hellst-dunkelst) > SCHWELLE  )
     {
      ausgabe[x+y*320]=0;
      return;

      }
    
    rec_count++;
    mvalue_sum+=bild[x+y*320];
    maske[x+y*320]=1;


    if ( rec_count> 0 ) mvalue=mvalue_sum/rec_count;



    if ( x < 319 ) if ( maske[x+1+y*320]==0 )
    {
     x_sized++; reserflood( x+1, y  );

     }
    if ( x > 0 )  if ( maske[x-1+y*320]==0 )
    {
      x_sized++; reserflood( x-1, y  );
  
    }
    if ( y < 199 ) if ( maske[x+(y+1)*320]==0 )
    {
      y_sized++; reserflood( x, y+1  );

    }
    if ( y > 0 ) if ( maske[x+(y-1)*320]==0 )
    {
      y_sized++; reserflood( x, y-1 );

    }


}

enum {RIGHT, LEFT, UP, DOWN }sn_direc;

void snake(int x, int y)
{

 int count;

 while (1)
 {
  if ( x+1 < 320 )
   if ( sn_direc==RIGHT && maske[x+1+y*320]==1 )
   {
    x_sized++;

    for(count=x_sized; count > 0; count-- )
    {
      if ( x+count < 320 ) { if ( maske[x+count+y*320]!=1 ){x_sized--; return; } }
      else {x_sized--; return; }

    }
    x+=x_sized;
    sn_direc=UP;
   }
   else break;


  if ( y-1 > 0 )
   if ( sn_direc==UP && maske[x+(y-1)*320]==1 )
   {
    y_sized++;

    for(count=y_sized; count > 0; count-- )
    {
      if ( y-count > 0 ) { if ( maske[x+(y-count)*320]!=1 ){y_sized--; return; }}
      else {y_sized--; return; }


    }
    y-=y_sized;


    sn_direc=LEFT;
   }
   else break;

   
  if ( x-1 > 0 )
   if ( sn_direc==LEFT && maske[x-1+y*320]==1 )
   {
    x_sized++;


    for(count=x_sized; count > 0; count-- )
    {
      if ( x-count > 0 ) { if ( maske[x-count+y*320]!=1 ) {x_sized--; return; }}
      else {x_sized--; return; }

    }
    x-=x_sized;
    sn_direc=DOWN;
   }
   else break;

   
  if ( y+1 < 200 )
   if ( sn_direc==DOWN && maske[x+(y+1)*320]==1 )
   {
    y_sized++;

    for(count=y_sized; count > 0; count-- )
    {
     if ( y+count < 200 ) { if ( maske[x+(y+count)*320]!=1 ){ y_sized--; return; } }
      else {y_sized--; return; }

    }
    y+=y_sized;
    sn_direc=RIGHT;
   }
   else break;


 }



}


int main(int argc, char*argv[])
{

  FILE *input, *output;
  int n=0, n2=0;
   int olx_sized, oly_sized, act_xs, act_ys;

  
  
  input=fopen(argv[1],"rb");
  
  if ( argc != 4 )
  {
   printf("Falsche Anzahl Argumente!\n");
   return 1;
  
  }

  schwelle=atoi(argv[3] );

  if ( input == NULL )
  {
   printf("Datei nicht gefunden!\n");
   return 1;
  }


  output=fopen(argv[2],"wb");
  if ( output==NULL )
  {
   printf("E/A-Fehler.\n");
   return 1;
  }

 fread( bild, sizeof(unsigned char), 64000, input);
 fclose(input);


  n=0;
 while(n<64000) { maske[n]=0; ausgabe[n]=255; n++; }
 
  n=0;
 

 n2=0;
 while ( n2 < 64000)
 {
  n2=0;
  rec_count=0;
  abbruch=0;
  highest_depth=1;

  mvalue_sum=0;
  hellst=TOINIT, dunkelst=TOINIT;

  while( maske[n2]!=0 && n2 < 64000) n2++;
  if ( n2== 64000 ) break;

   printf("Flute die Koordinaten %d %d\n", n2%320, n2/320 ); /*debug*/

    x_sized=0, y_sized=0;
    rec_count=0;
    mvalue_sum=0;
    x_start=n2%320, y_start=n2/320;

       iter_count=0;

   do{  flood(n2%320, n2/320 , 1 ); backsort(); }  while ( abbruch >= 0 ) ;

  if ( rec_count > 0 ) mvalue=mvalue_sum/rec_count;

    printf("Der Wert lautet: %f\n", mvalue );

     y_start=0;
     x_start=0;

     olx_sized=0, oly_sized=0;

     while( y_start < 200 )
     {
      x_start=0;
      while ( x_start < 320 )
      {
        x_sized=0, y_sized=0;
       sn_direc=RIGHT;
       if ( maske[x_start+y_start*320]== 1 ) snake(x_start,y_start);

       if ( x_sized > olx_sized && y_sized > oly_sized ) olx_sized=x_sized, oly_sized=y_sized, act_xs=x_start,
                                                         act_ys=y_start ;

       /* if ( x_sized>15 && y_sized > 15 ) break; */
       x_start++;
      }
      /* if ( x_sized>15 && y_sized > 15 ) break; */
      y_start++;
     }

   if ( olx_sized > 5 && oly_sized > 5 )
     dumpnum(act_xs-3, act_ys-2, mvalue), dumped=1;

  n=0;
  while(n < 64000 )
  {
   if ( maske[n]==1 ) maske[n]=2;
   n++;
  }
  
  n2++;
 }  

 fwrite ( ausgabe, sizeof(unsigned char), 64000, output );
 fflush(output );

  fclose(output);

}
