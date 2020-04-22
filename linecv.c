/* Nehmt ein .im8-Sun-Rasterimage mit Graustufenpalette und der Auflösung 320x200 und übergebt
  den Dateinamen als Parameter, nachdem ihr den Header gelöscht habt, den ihr dann wieder dranhängt, wenn ihr möchtet .*/


#include <stdio.h>

/* Ziv Lempel muss Redundanzangabe kodiert in Ausgabestrom schreiben */



int main(int argc, char *argv[])
{
 FILE *input, *output;
  unsigned char bild[64000], lined[64000], umask[64000];
  int bild2[64000];

 if ( argc!=3) return 1;
 input=fopen(argv[1],"rb");
   if ( input==NULL) return 1;
 fread(bild , sizeof(unsigned char),64000, input );



float x=0, y=0;
  
  y=0;
  while ( y < 200)
  {
   x=0;
   while ( x < 320)
   {
   	 bild2[(int)x+(int)y*320]=bild[(int)x+(int)y*320];
   	 
   	  x++;
	   }	
  	y++;
  }
  
  int n=0;
  while ( n < 2)
  {
  int x=0, y=0;
  
  y=0;
  while ( y < 200)
  {
   x=0;
   while ( x < 320)
   {
   	 bild2[x+y*320]+=bild2[x+1+y*320];
   	 bild2[x+y*320]+=bild2[x+(y+1)*320];
   	 bild2[x+y*320]+=bild2[x+1+(y+1)*320];
   	
   	 bild2[x+y*320]/=4;
   	 
   	  x++;
	   }	
  	y++;
  }
  
   n++;
 }
  
  {
  	int x=0,y=0;
  y=0;
  while ( y < 200)
  {
  	x=0;
  	while ( x < 320)
  	{
  		int c;
  		lined[x+y*320]=255;
  		lined[x+1+y*320]=255;
  		lined[x+(y+1)*320]=255;
  		lined[x+1+(y+1)*320]=255;
  		
  		
  	  c=bild2[x+y*320]; //(bild2[x+y*320]+bild2[x+1+y*320]+bild2[x+(y+1)*320]+bild2[x+1+(y+1)*320])/4;
  	   umask[x+y*320]=255-c, umask[x+1+y*320]=255-c, umask[x+(y+1)*320]=255-c, umask[x+1+(y+1)*320]=255-c;	
  	
  	    umask[x+y*320]=bild[x+y*320]+umask[x+y*320] <=255 ? bild[x+y*320]+umask[x+y*320] : 255;
	    umask[x+1+y*320]=bild[x+1+y*320]+umask[x+1+y*320] <=255 ? bild[x+1+y*320]+umask[x+1+y*320] : 255;
	    umask[x+1+(y+1)*320]=bild[x+1+(y+1)*320]+umask[x+1+(y+1)*320] <=255 ? bild[x+1+(y+1)*320]+umask[x+1+(y+1)*320] : 255;
	    umask[x+(y+1)*320]=bild[x+(y+1)*320]+umask[x+(y+1)*320] <=255 ? bild[x+(y+1)*320]+umask[x+(y+1)*320] : 255;
		  
		  if ( umask[x+y*320]< 255) umask[x+y*320]=0;
		  if ( umask[x+1+y*320]< 255) umask[x+1+y*320]=0;
		  if ( umask[x+1+(y+1)*320]< 255) umask[x+1+(y+1)*320]=0;
		  if ( umask[x+(y+1)*320]< 255) umask[x+(y+1)*320]=0;
		  
		  
	 	x+=2;
	  }
  	y+=2;
  }
  
  }

  y=0;
  while ( y < 200)
  {
   x=0;
   double oldlen;
    oldlen=0.0;
	double longslopex, longslopey;		  	 
		  	 
   while( x < 320)
   {
   	
   	 double angle;
   	 
   	 double xslope,yslope;
   	 double xsloper,ysloper;
   	 double len=0;
	     
   	if ( umask[(int)x+(int)y*320]==0)
   	{
   		
   	 oldlen=0.0;
   	 
   	 angle=-6.15;
   	 
   	 xslope=1.0, yslope=0.0;
   	 
   	 while ( angle< 6.14)
   	 {
   	    xsloper= +(xslope)*cos(angle)-(yslope)*sin(angle);
        ysloper= (xslope)*sin(angle)+(yslope)*cos(angle);
	
	
	   /*
	    double buf;
	    if ( xsloper ==0.0) xsloper=0.01;
	    ysloper=ysloper/xsloper;
	    xsloper=1.0; */
	    
	    if ( fabs(xsloper) > fabs(ysloper) ) { ysloper/=xsloper; xsloper/=xsloper; }
	    else
		if ( fabs(ysloper) >= fabs(xsloper))
	    { xsloper/=ysloper; ysloper/=ysloper; }
	    
	     len=0;
   	 	while ( len < 100 )
 		{
		
		if (x+xsloper*len >= 0 && x+xsloper*len < 319 &&
		    y+ysloper*len >= 0 && y+ysloper*len < 199 )
		    {
		if ( umask[(int)(x+xsloper*len)+(int)(y+ysloper*len)*320]==0 /*||
		     umask[(int)(x+1+xsloper*len)+(int)(y+1+ysloper*len)*320]==0 ||
			 umask[(int)(x+xsloper*len)+(int)(y+ysloper*len)*320]==0 ||
			 umask[(int)(x+1+xsloper*len)+(int)(y+ysloper*len)*320]==0 */ ) len++;
   		  else break;
   		   } else break;
   		
   	      }
   	    angle+=0.03;
	   
	   if ( len >= oldlen) oldlen=len, longslopex=xsloper, longslopey=ysloper;
       }
      
          xsloper=longslopex, ysloper=longslopey;
         len=0;
	   
	   if ( oldlen >=1)
	   {
	   //printf("Eintritt...\n");
		while ( len < oldlen )
		{
		
		if (x+xsloper*len >= 0 && x+xsloper*len < 320 &&
		    y+ysloper*len >= 0 && y+ysloper*len < 200 )
		    {
		//if ( umask[(int)(x+xsloper*len)+(int)(y+ysloper*len)*320]==0 )
		    {	
		     if ( oldlen >= atoi(argv[2]))lined[(int)(x+xsloper*len)+(int)(y+ysloper*len)*320]=0;
		     
			 umask[(int)(+x+xsloper*len)+(int)(y+ysloper*len)*320]=255;
			 
			 
			 len++;
   		    }
		  //else break;
   		   } else break;
   		   
   		   
   	      }
       
          if ( oldlen >= atoi(argv[2]))printf("%d %d %d %d %d\n",(int)x,(int)y,(int)(xsloper*100),(int)(ysloper*100),(int)oldlen);
   	      
   	     }
       
       
       }
   	
   	x++;
   }
  	
  	y++;
  	
  }
  printf("0 0 0 0 0\n");  
  
  output=fopen(".\\umasked.raw","wb");
  
  /*
  y=0;
  while ( y < 200)
  {
   x=0;
   while ( x < 320)
   {
   	if ( lined[(int)x+(int)y*320]!=0) lined[(int)x+(int)y*320]=bild2[(int)x+(int)y*320];
   	x++;
   }
  	y++;
  }
  */
  
  
  fwrite( lined , sizeof(unsigned char),64000,output);
  fclose(output);
 


}
