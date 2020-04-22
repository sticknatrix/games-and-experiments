/*
  How it works correctly:
  The light source emits round sort of ping pong particles with a randomized
  movement vector. Randomized may be not as efficient, but what does it matter in a test program.
  When the particle that represents the light (in reality, light is not a particle, but it should look the same) hits an object,
  there are two cases: either it is mirrored and bounces off, that's the case when the object is flat, or it is scattered randomly, that's the case
  when the object is rough. You can mix both. The object that was hit by the particle gets increased its brightness. The brightness
  can reach the brightness of the light source as a maximum limit. You can also divide it down to the object's own color value.
  Glass or water are more complicated.
  Only white reflects all light. Store the reflection info in the struct of the particle.
  Radially out in the infinity, the entire light of the source
  is emitted into space, but a small area can always only catch the amount according to its size.
  Each time a particle hits an object, the corresponding pixels in a pixel cloud get brightened by a fraction of its own color and the
  color stored in the particle data structure.
  It's important to reset the values and start the raytracing again after a defined time has passed.
  I turned of the physics animation in this source code, because i only wanted to observe the raytracing behaviour.
  Why use a pixel cloud? Because it has no geometrical limits, sums up everything and is easy to write.
  You can have several clouds with several scaling resolutions and you can select them during projection according to the distance from the camera view.
  You don't even need the full resolution for shading.
  Let the graphic processors do this independently from the CPU. I don't know how, but that's how everybody does it nowadays.
  For small light sources that are hidden from small objects, you can pick the downscaled pixel cloud when they are far
  away from the player. And you can let the particles entirely disappear after a certain run-length.
  That saves time.
  The problem are bigger light sources light the sun and big parts of the covering terrain. A good thing is that all rays
  are nearly parallel so you can place that light source to a closer position and you can also eliminate all angles
  that don't hit the terrain anyway. The problem is that it looks bad when you choose to shade with a lower resolution because
  when the player is coming closer to the object that throws the shadow, the borders of the shadow will flicker.
  So put the parts of the terrain that need a higher resolution in a separate data structure.
  The good thing is that you can simply add all separate light impact together. Simplified but it should look good.
  The dumb thing with the pixel cloud might be that you cannot check for the collision as quickly as in an array.
  Make up some optimizations. Sorting the cloud might be not so good, but you could transfer parts in an array or completely
  ignore the shadows of distant small objects. You could also speed up the particles when there are many direct lines free.
  */
  
  
  


#include <stdio.h>
#include <math.h>

#define TZAHL 54
#define ELEK 0
#define PROT 1
#define NEUT 2

#define SCHEITEL 3.0
   #define LADUNG 17.0  // war 17.0
   #define MAXKRAFT 4.0 // war beides 57.0
   #define TRAEGSCHWELLE 1.0 // war 1.0
   #define TIMESLICE 150.0 // war nach oben 150.0
   #define VERLANGSFAC 1.0 // war 0.9
    
 
 float sintab[128];
float costab[128];

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

void cosinit(void)
{
 float value;
 value=-6.4;

 while ( value < 6.4 )
 {
  costab[(int)((value+6.4)*10.0)]=cos(value);
  value+=0.1;
 }

}

/*
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


inline float COS(float value)
{
 signed int sign;
 signed int index;

 if ( value < 0 ) sign=-1;
 else sign=+1;
  index= ((int)( value*10 )%64);
  index+=63;

 return  costab[index];
}

*/
#define SIN(val) sintab[(int)(val*10) %64 +63]
#define COS(val) costab[(int)(val*10) %64 +63]


int main(void)
{
  long int runs=0;
  struct
{
 float x,y,z;
 float xv,yv,zv;
 float xv_old, yv_old, zv_old ;
   int elec_or_prot;
  unsigned char symbol;
  unsigned char ID;
  long int tracecount;
} teilchen[TZAHL+1];

int n=0, n2=0;

	float xshift=-30, yshift=0, zshift=-40, dirflag=0;

unsigned char screen[160][90];
unsigned int screen_z[160][90];
float pixbuf[100000][4]; unsigned int pbc=0;
int x,y;
int zc;

int moved=0;

float x_buf, y_buf, z_buf, power, sign, distance;
 float y_angl=-0.6;

 sininit();
 cosinit();
zc=0;
while ( zc< 54) { if ( zc%18 < 9)teilchen[zc].zv=5.0 /* <--5.0 */; else teilchen[zc].zv=0.0; teilchen[zc].zv_old=0.0; zc++; }
zc=0;
while ( zc < 3)
{

#define AB1 30.0
#define AB2 35.0
teilchen[zc*18+0].x=35.0, teilchen[zc*18+0].y=5, teilchen[zc*18+0].z=AB1+3*zc;
teilchen[zc*18+0].xv=0.0, teilchen[zc*18+0].yv=0;
teilchen[zc*18+0].xv_old=0.0, teilchen[zc*18+0].yv_old=0.0;
teilchen[zc*18+0].elec_or_prot=PROT^(zc&1),
teilchen[zc*18+0].symbol='A';


teilchen[zc*18+1].x=38.0, teilchen[zc*18+1].y=5, teilchen[zc*18+1].z=AB1+3*zc;
teilchen[zc*18+1].xv=0.0, teilchen[zc*18+1].yv=0.0;
teilchen[zc*18+1].xv_old=0.0, teilchen[zc*18+1].yv_old=0.0;
teilchen[zc*18+1].elec_or_prot=ELEK^(zc&1),
teilchen[zc*18+1].symbol='A';


teilchen[zc*18+2].x=41.0, teilchen[zc*18+2].y=5, teilchen[zc*18+2].z=AB1+3*zc;
teilchen[zc*18+2].xv=0.0, teilchen[zc*18+2].yv=0.0;
teilchen[zc*18+2].xv_old=0.0, teilchen[zc*18+2].yv_old=0.0;
teilchen[zc*18+2].elec_or_prot=PROT^(zc&1),
teilchen[zc*18+2].symbol='A';



teilchen[zc*18+3+0].x=35.0, teilchen[zc*18+3+0].y=8, teilchen[zc*18+3+0].z=AB1+3*zc;
teilchen[zc*18+3+0].xv=0.0, teilchen[zc*18+3+0].yv=0;
teilchen[zc*18+3+0].xv_old=0.0, teilchen[zc*18+3+0].yv_old=0.0;
teilchen[zc*18+3+0].elec_or_prot=ELEK^(zc&1),
teilchen[zc*18+3+0].symbol='A';


teilchen[zc*18+3+1].x=38.0, teilchen[zc*18+3+1].y=8,  teilchen[zc*18+3+1].z=AB1+3*zc;
teilchen[zc*18+3+1].xv=0.0, teilchen[zc*18+3+1].yv=0.0;
teilchen[zc*18+3+1].xv_old=0.0, teilchen[zc*18+3+1].yv_old=0.0;
teilchen[zc*18+3+1].elec_or_prot=PROT^(zc&1),
teilchen[zc*18+3+1].symbol='A';


teilchen[zc*18+3+2].x=41.0, teilchen[zc*18+3+2].y=8, teilchen[zc*18+3+2].z=AB1+3*zc;
teilchen[zc*18+3+2].xv=0.0, teilchen[zc*18+3+2].yv=0.0;
teilchen[zc*18+3+2].xv_old=0.0, teilchen[zc*18+3+2].yv_old=0.0;
teilchen[zc*18+3+2].elec_or_prot=ELEK^(zc&1),
teilchen[zc*18+3+2].symbol='A';




teilchen[zc*18+6+0].x=35.0, teilchen[zc*18+6+0].y=11,  teilchen[zc*18+6+0].z=AB1+3*zc;
teilchen[zc*18+6+0].xv=0.0, teilchen[zc*18+6+0].yv=0;
teilchen[zc*18+6+0].xv_old=0.0, teilchen[zc*18+6+0].yv_old=0.0;
teilchen[zc*18+6+0].elec_or_prot=PROT^(zc&1),
teilchen[zc*18+6+0].symbol='A';


teilchen[zc*18+6+1].x=38.0, teilchen[zc*18+6+1].y=11,  teilchen[zc*18+6+1].z=AB1+3*zc,
teilchen[zc*18+6+1].xv=0.0, teilchen[zc*18+6+1].yv=0.0;
teilchen[zc*18+6+1].xv_old=0.0, teilchen[zc*18+6+1].yv_old=0.0;
teilchen[zc*18+6+1].elec_or_prot=ELEK^(zc&1),
teilchen[zc*18+6+1].symbol='A';


teilchen[zc*18+6+2].x=41.0, teilchen[zc*18+6+2].y=11, teilchen[zc*18+6+2].z=AB1+3*zc,
teilchen[zc*18+6+2].xv=0.0, teilchen[zc*18+6+2].yv=0.0;
teilchen[zc*18+6+2].xv_old=0.0, teilchen[zc*18+6+2].yv_old=0.0;
teilchen[zc*18+6+2].elec_or_prot=PROT^(zc&1),
teilchen[zc*18+6+2].symbol='A';





teilchen[zc*18+9+0].x=75.0, teilchen[zc*18+9+0].y=9,  teilchen[zc*18+9+0].z=AB2+3*zc;
teilchen[zc*18+9+0].xv=-20.0, teilchen[zc*18+9+0].yv=0;
teilchen[zc*18+9+0].xv_old=0.0, teilchen[zc*18+9+0].yv_old=0.0;
teilchen[zc*18+9+0].elec_or_prot=PROT^(zc&1),
teilchen[zc*18+9+0].symbol='D';


teilchen[zc*18+9+1].x=78.0, teilchen[zc*18+9+1].y=9,  teilchen[zc*18+9+1].z=AB2+3*zc;
teilchen[zc*18+9+1].xv=-20.0, teilchen[zc*18+9+1].yv=0.0;
teilchen[zc*18+9+1].xv_old=0.0, teilchen[zc*18+9+1].yv_old=0.0;
teilchen[zc*18+9+1].elec_or_prot=ELEK^(zc&1),
teilchen[zc*18+9+1].symbol='D';


teilchen[zc*18+9+2].x=81.0, teilchen[zc*18+9+2].y=9,  teilchen[zc*18+9+2].z=AB2+3*zc;
teilchen[zc*18+9+2].xv=-20.0, teilchen[zc*18+9+2].yv=0.0;
teilchen[zc*18+9+2].xv_old=0.0, teilchen[zc*18+9+2].yv_old=0.0;
teilchen[zc*18+9+2].elec_or_prot=PROT^(zc&1),
teilchen[zc*18+9+2].symbol='D';



teilchen[zc*18+9+3+0].x=75.0, teilchen[zc*18+9+3+0].y=12, teilchen[zc*18+9+3+0].z=AB2+3*zc;
teilchen[zc*18+9+3+0].xv=-20.0, teilchen[zc*18+9+3+0].yv=0;
teilchen[zc*18+9+3+0].xv_old=0.0, teilchen[zc*18+9+3+0].yv_old=0.0;
teilchen[zc*18+9+3+0].elec_or_prot=ELEK^(zc&1),
teilchen[zc*18+9+3+0].symbol='D';


teilchen[zc*18+9+3+1].x=78.0, teilchen[zc*18+9+3+1].y=12, teilchen[zc*18+9+3+1].z=AB2+3*zc;
teilchen[zc*18+9+3+1].xv=-20.0, teilchen[zc*18+9+3+1].yv=0.0;
teilchen[zc*18+9+3+1].xv_old=0.0, teilchen[zc*18+9+3+1].yv_old=0.0;
teilchen[zc*18+9+3+1].elec_or_prot=PROT^(zc&1),
teilchen[zc*18+9+3+1].symbol='D';


teilchen[zc*18+9+3+2].x=81.0, teilchen[zc*18+9+3+2].y=12,  teilchen[zc*18+9+3+2].z=AB2+3*zc;
teilchen[zc*18+9+3+2].xv=-20.0, teilchen[zc*18+9+3+2].yv=0.0;
teilchen[zc*18+9+3+2].xv_old=0.0, teilchen[zc*18+9+3+2].yv_old=0.0;
teilchen[zc*18+9+3+2].elec_or_prot=ELEK^(zc&1),
teilchen[zc*18+9+3+2].symbol='D';



teilchen[zc*18+9+6+0].x=75.0, teilchen[zc*18+9+6+0].y=15, teilchen[zc*18+9+6+0].z=AB2+3*zc;
teilchen[zc*18+9+6+0].xv=-20.0, teilchen[zc*18+9+6+0].yv=0;
teilchen[zc*18+9+6+0].xv_old=0.0, teilchen[zc*18+9+6+0].yv_old=0.0;
teilchen[zc*18+9+6+0].elec_or_prot=PROT^(zc&1),
teilchen[zc*18+9+6+0].symbol='D';


teilchen[zc*18+9+6+1].x=78.0, teilchen[zc*18+9+6+1].y=15, teilchen[zc*18+9+6+1].z=AB2+3*zc;
teilchen[zc*18+9+6+1].xv=-20.0, teilchen[zc*18+9+6+1].yv=0.0;
teilchen[zc*18+9+6+1].xv_old=0.0, teilchen[zc*18+9+6+1].yv_old=0.0;
teilchen[zc*18+9+6+1].elec_or_prot=ELEK^(zc&1),
teilchen[zc*18+9+6+1].symbol='D';


teilchen[zc*18+9+6+2].x=81.0, teilchen[zc*18+9+6+2].y=15,teilchen[zc*18+9+6+2].z=AB2+3*zc,
teilchen[zc*18+9+6+2].xv=-20.0, teilchen[zc*18+9+6+2].yv=0.0;
teilchen[zc*18+9+6+2].xv_old=0.0, teilchen[zc*18+9+6+2].yv_old=0.0;
teilchen[zc*18+9+6+2].elec_or_prot=PROT^(zc&1),
teilchen[zc*18+9+6+2].symbol='D';

zc++;
}

x=0;
while ( x < 160)
{
 y=0;
 while ( y < 90)
{
 screen[x][y]=' ';
 y++;
}
x++;
}


int ylampe=0;
int pbc_old=0;
 while(1)
 {
 	ylampe=-10;
 	long int bruns=0;
 	//bruns=0;
 pbc=0;
  teilchen[TZAHL].x=10, teilchen[TZAHL].y=ylampe+0, teilchen[TZAHL].z=20;
   teilchen[TZAHL].xv=-10+(rand()%200)*0.1,teilchen[TZAHL].yv=-10+(rand()%200)*0.1,teilchen[TZAHL].zv=-10+(rand()%200)*0.1;
 
 int reflect=0;
 reflect=0;
 while ( bruns < 400000)
 {
    if ( pbc > 99999) pbc=99999;
    if ( bruns%2500==0||reflect==1)
    {
    	reflect=0;
 	teilchen[TZAHL].x=10, teilchen[TZAHL].y=ylampe+0, teilchen[TZAHL].z=20;
 teilchen[TZAHL].xv=-10+(rand()%200)*0.1,teilchen[TZAHL].yv=-10+(rand()%200)*0.1,teilchen[TZAHL].zv=-10+(rand()%200)*0.1;
  }
  bruns++;
 	 n=TZAHL;
 
 // printf("%d\n",n); 
 while ( n < TZAHL+1)
{


  n2=0;
  while ( n2 < TZAHL)
  {
   if ( n2==n) { n2++; continue;}

  x_buf=0, y_buf=0;
  

  power=
   sqrt ( 
    ( teilchen[n].x - teilchen[n2].x )  *
    ( teilchen[n].x  - teilchen[n2].x )+
       ( teilchen[n].y - teilchen[n2].y) *
    ( teilchen[n].y - teilchen[n2].y) +
       ( teilchen[n].z - teilchen[n2].z) *
    ( teilchen[n].z - teilchen[n2].z)
);
  
    distance=power;

   x_buf=(teilchen[n].x-teilchen[n2].x);
   y_buf=(teilchen[n].y-teilchen[n2].y);
   z_buf=(teilchen[n].z-teilchen[n2].z);
 
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;

  
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;

    if ( ((teilchen[n].elec_or_prot==ELEK && teilchen[n2].elec_or_prot==PROT)||
         (teilchen[n].elec_or_prot==PROT && teilchen[n2].elec_or_prot==ELEK))&& (teilchen[n].symbol==teilchen[n2].symbol) 

) x_buf*=-1.0, y_buf*=-1.0, z_buf*=-1.0;
   

          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
          z_buf*=power/distance;

        } else x_buf=0, y_buf=0, z_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf+ z_buf*z_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power, z_buf*=power;
      
      pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,pixbuf[pbc][3]=219,(reflect==0 ? (pbc++,reflect=1): (reflect=1));
      
    }  else x_buf=0, y_buf=0, z_buf=0 ;


   
    teilchen[n].xv+=x_buf, teilchen[n].yv+=y_buf, teilchen[n].zv+=z_buf;
    
    n2++;
  }

  n++;

 }
 
 
 
 
 
n=TZAHL;  // linke Bande
    while ( n < TZAHL+1)
 {
	       
       x_buf=0, y_buf=0, z_buf=0;
  

  power=
   sqrt ( 
    ( teilchen[n].x - 0 )  *
    ( teilchen[n].x - 0 )+
      0 );
  
    distance=power;

   y_buf=0;
   z_buf=0;
   x_buf=(teilchen[n].x-0);
 
 
   if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
  



    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;


          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
          z_buf*=power/distance;

        } else x_buf=0, y_buf=0, z_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf+z_buf*z_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power, z_buf*=power;
    pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,pixbuf[pbc][3]=219,(reflect==0 ?(pbc++,reflect=1): (reflect=1));
       //if ( pbc < 100000)pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,
         //    pixbuf[pbc][3]=176; pbc++;
 
	}  else x_buf=0, y_buf=0, z_buf=0 ;

   
    teilchen[n].xv+=x_buf, teilchen[n].yv+=y_buf, teilchen[n].zv+=z_buf;
    n++;
  } 
 
n=TZAHL;  // rechte Bande
    while ( n < TZAHL+1)
 {
	       
       x_buf=0, y_buf=0, z_buf=0;
  

  power=
   sqrt ( 
    ( teilchen[n].x - 90 )  *
    ( teilchen[n].x - 90 )+
      0 );
  
    distance=power;

   y_buf=0;
   z_buf=0;
   x_buf=(teilchen[n].x-90);
 
 
   if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
  



    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;


          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
          z_buf*=power/distance;

        } else x_buf=0, y_buf=0, z_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf+z_buf*z_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power, z_buf*=power;
           pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,pixbuf[pbc][3]=219,(reflect==0 ?(pbc++,reflect=1): (reflect=1));
       //if ( pbc < 100000)pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,
         //    pixbuf[pbc][3]=177; pbc++;
 
    }  else x_buf=0, y_buf=0, z_buf=0 ;

   
    teilchen[n].xv+=x_buf, teilchen[n].yv+=y_buf, teilchen[n].zv+=z_buf;
    n++;
  } 
 
 

n=TZAHL;  // Boden
    while ( n < TZAHL+1)
 {
	       
       x_buf=0, y_buf=0, z_buf=0;
  

  power=
   sqrt ( 
    ( teilchen[n].y - 30 )  *
    ( teilchen[n].y - 30 )+
      0 );
  
    distance=power;

   y_buf=(teilchen[n].y-30);
   z_buf=0;
   x_buf=0;
 
 
   if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
  



    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;


          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
          z_buf*=power/distance;

        } else x_buf=0, y_buf=0, z_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf+z_buf*z_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power, z_buf*=power;
      pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,pixbuf[pbc][3]=219,(reflect==0 ?(pbc++,reflect=1): (reflect=1));
      // Reibung
       //teilchen[n].xv*=0.99,teilchen[n].yv*=0.99,teilchen[n].zv*=0.99;
      
           //if ( pbc < 100000) pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,
            // pixbuf[pbc][3]=219; pbc++;
 
    }  else x_buf=0, y_buf=0, z_buf=0 ;

   
    teilchen[n].xv+=x_buf, teilchen[n].yv+=y_buf, teilchen[n].zv+=z_buf;
    n++;
  } 
 

n=TZAHL;  // Decke
    while ( n < TZAHL+1)
 {
	       
       x_buf=0, y_buf=0, z_buf=0;
  

  power=
   sqrt ( 
    ( teilchen[n].y + 20 )  *
    ( teilchen[n].y + 20 )+
      0 );
  
    distance=power;

   y_buf=(teilchen[n].y+20);
   z_buf=0;
   x_buf=0;
 
 
   if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
  



    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;


          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
          z_buf*=power/distance;

        } else x_buf=0, y_buf=0, z_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf+z_buf*z_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power, z_buf*=power;
      pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,pixbuf[pbc][3]=219,(reflect==0 ?(pbc++,reflect=1): (reflect=1));
     // if ( pbc < 100000) pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,
       //      pixbuf[pbc][3]=176; pbc++;
 
    }  else x_buf=0, y_buf=0, z_buf=0 ;

   
    teilchen[n].xv+=x_buf, teilchen[n].yv+=y_buf, teilchen[n].zv+=z_buf;
    n++;
  } 
 
 
 
 
 
 
n=TZAHL;  // Hinten
    while ( n < TZAHL+1)
 {
	       
       x_buf=0, y_buf=0, z_buf=0;
  

  power=
   sqrt ( 
    ( teilchen[n].z - 80 )  *
    ( teilchen[n].z - 80 )+
      0 );
  
    distance=power;

   y_buf=0;
   z_buf=(teilchen[n].z-80);
   x_buf=0;
 
 
   if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
  



    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;


          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
          z_buf*=power/distance;

        } else x_buf=0, y_buf=0, z_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf+z_buf*z_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power, z_buf*=power;
      pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,pixbuf[pbc][3]=219,(reflect==0 ?(pbc++,reflect=1): (reflect=1));
     // if ( pbc < 100000) pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,
      //       pixbuf[pbc][3]=177; pbc++;
      
    }  else x_buf=0, y_buf=0, z_buf=0 ;

   
    teilchen[n].xv+=x_buf, teilchen[n].yv+=y_buf, teilchen[n].zv+=z_buf;
    n++;
  } 
 

n=TZAHL;  // Vorne
    while ( n < TZAHL+1)
 {
	       
       x_buf=0, y_buf=0, z_buf=0;
  

  power=
   sqrt ( 
    ( teilchen[n].z + 0 )  *
    ( teilchen[n].z + 0 )+
      0 );
  
    distance=power;

   y_buf=0;
   z_buf=(teilchen[n].z+0);
   x_buf=0;
 
 
   if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;
  



    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;


          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;
          z_buf*=power/distance;

        } else x_buf=0, y_buf=0, z_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf+z_buf*z_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power, z_buf*=power;
      pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,pixbuf[pbc][3]=219,(reflect==0 ?(pbc,reflect=1): (reflect=1));
      //Sichtbarmachdebug
      
	  //if ( pbc < 100000)pixbuf[pbc][0]=teilchen[n].x,pixbuf[pbc][1]=teilchen[n].y,pixbuf[pbc][2]=teilchen[n].z,
        //     pixbuf[pbc][3]=219; pbc++;
 
    }  else x_buf=0, y_buf=0, z_buf=0 ;

   
    teilchen[n].xv+=x_buf, teilchen[n].yv+=y_buf, teilchen[n].zv+=z_buf;
    n++;
  } 
 
 
 n=TZAHL;
 while ( n < TZAHL) { teilchen[n].yv+=0.009; n++; } // Schwerkraft

 n=TZAHL;
while (n < TZAHL+1)
{
teilchen[n].x+=teilchen[n].xv/TIMESLICE,
 teilchen[n].y+=teilchen[n].yv/TIMESLICE;
 teilchen[n].z+=teilchen[n].zv/TIMESLICE;
 teilchen[n].xv*=VERLANGSFAC, teilchen[n].yv*=VERLANGSFAC, teilchen[n].zv*=VERLANGSFAC;
 //teilchen[n].xv=(teilchen[n].xv_old+teilchen[n].xv)/2.0;
 //teilchen[n].yv=(teilchen[n].yv_old+teilchen[n].yv)/2.0;
 //teilchen[n].zv=(teilchen[n].zv_old+teilchen[n].zv)/2.0;
  
 teilchen[n].xv_old=teilchen[n].xv;
 teilchen[n].yv_old=teilchen[n].yv;
 teilchen[n].zv_old=teilchen[n].zv;
 n++;
}


}


if ( (runs++)%1==0)
{
 x=0;
while ( x < 160)
{
 y=0;
 while ( y < 90)
{
 if ( screen[x][y]!=219||moved==1)screen[x][y]=' ';
 screen_z[x][y]=100000;
 y++;
}
x++;
}
moved=0;

	if ( dirflag==1 && yshift > 0) yshift--;
	else dirflag=0;
	//if ( dirflag==0 && yshift < 30) yshift++;
	//else dirflag=1;
	
n=0;
//pbc=0;
while ( n < TZAHL)
{
	
	#define CAMX (xshift)
	#define CAMY (yshift)
	#define CAMZ (zshift)
	#define SCAL 120.0 // war 60
	
	unsigned char symbol;
	switch ((int)n/9 )
	{
		case 0: symbol=219; break;
		case 1: symbol=176; break;
		case 2: symbol=177; break;
		case 3: symbol=219; break;
		case 4: symbol=176; break;
		case 5: symbol=177; break;
	}
	
 
 
 #define FILL(X,Y,Z)\
  pixbuf[pbc][0]=teilchen[n].x+(X),pixbuf[pbc][1]=teilchen[n].y+(Y),pixbuf[pbc][2]=teilchen[n].z+(Z), pixbuf[pbc][3]=symbol, pbc++;
/*
if( screen_z[(int)((teilchen[n].x-CAMX)*SCAL/(teilchen[n].z+Z))+ X +80][(int)((teilchen[n].y-CAMY)*SCAL/(teilchen[n].z+Z))+40 + Y ]>teilchen[n].z+Z)\
 screen[(int)((teilchen[n].x-CAMX)*SCAL/(teilchen[n].z+Z))+ X +80][(int)((teilchen[n].y-CAMY)*SCAL/(teilchen[n].z+Z))+ Y +40]=symbol,\
 screen_z[(int)((teilchen[n].x-CAMX)*SCAL/(teilchen[n].z+Z))+ X +80][(int)((teilchen[n].y-CAMY)*SCAL/(teilchen[n].z+Z))+ Y +40]=teilchen[n].z+Z;
 */
 
 unsigned char ball[6][5][5]={
                             {
							  {0,0,0,0,0},
                              {0,0,0,0,0},
						      {0,0,1,0,0},
						      {0,0,0,0,0},
						      {0,0,0,0,0} },
							  
							  {
							
							  {0,0,0,0,0},
                              {0,0,1,0,0},
						      {0,1,1,1,0},
						      {0,0,1,0,0},
						      {0,0,0,0,0} },
							  
							  {
							  
							  {0,0,1,0,0},
                              {0,1,1,1,0},
						      {1,1,1,1,1},
						      {0,1,1,1,0},
							  {0,0,1,0,0} },
							 
                              {
							  {0,0,1,0,0},
                              {0,1,1,1,0},
						      {1,1,1,1,1},
						      {0,1,1,1,0},
							  {0,0,1,0,0} },
							  
							  {
							  {0,0,0,0,0},
                              {0,0,1,0,0},
						      {0,1,1,1,0},
						      {0,0,1,0,0},
							  {0,0,0,0,0} },
							
							{
							  {0,0,0,0,0},
                              {0,0,1,0,0},
						      {0,1,1,1,0},
						      {0,0,1,0,0},
							  {0,0,0,0,0} },
							  
							  
							  };
	
	int z=0;
	z=0;
	while ( z < 5)
	{
	 y=0;
	 while ( y < 5)
	 {
      x=0;
	  while ( x < 6)
	  {
	  	if ( ball[x][y][z]==1 ) FILL ( x-2,y-2, z+2) 	
	  	x++;
		  }	

     y++;
	 }
	 
	  z++;
     }
		 /*                 
  FILL ( 0,0) FILL (-1,0) FILL (-2,0) FILL ( 1,0) FILL (2,0)
FILL(0,-1) FILL(-1,-1) FILL(-2,-1) FILL ( 1,-1) FILL(2,-1)
FILL(0,1) FILL(-1,1) FILL(-2,1) FILL ( 1,1) FILL(2,1)
FILL(0,-2) FILL(-1,-2) FILL(-2,-2) FILL ( 1,-2) FILL(2,-2)
FILL(0,2) FILL(-1,2) FILL(-2,2) FILL ( 1,2) FILL(2,2)
FILL(0,3) FILL(-1,3) FILL(-2,3) FILL (1,3) FILL (2,3) */

 
/*
 if( screen_z[(int)(teilchen[n].x/3)][(int)(teilchen[n].y/3)] > teilchen[n].z )
if ( teilchen[n].x/3> 0 && teilchen[n].x/3 < 159 && teilchen[n].y/3 > 0 && teilchen[n].y/3 < 88)
   screen[(int)(teilchen[n].x/3)][(int)(teilchen[n].y/3)]=teilchen[n].symbol+n/18,
   screen_z[(int)(teilchen[n].x/3)][(int)(teilchen[n].y/3)]=teilchen[n].z;
*/
n++;
}


/*
n=100;
while ( n < 180) { pixbuf[pbc][0]=0; pixbuf[pbc][1]=30; pixbuf[pbc][2]=n, pixbuf[pbc][3]=176; n++; pbc++; }
n=100;
while ( n < 180) { pixbuf[pbc][0]=90; pixbuf[pbc][1]=30; pixbuf[pbc][2]=n, pixbuf[pbc][3]=176; n++; pbc++; }

n=100;
while ( n < 180) { pixbuf[pbc][0]=0; pixbuf[pbc][1]=-20; pixbuf[pbc][2]=n, pixbuf[pbc][3]=176; n++; pbc++; }
n=100;
while ( n < 180) { pixbuf[pbc][0]=90; pixbuf[pbc][1]=-20; pixbuf[pbc][2]=n, pixbuf[pbc][3]=176; n++; pbc++; }



n=0;
while ( n < 90) { pixbuf[pbc][0]=n; pixbuf[pbc][1]=-20; pixbuf[pbc][2]=180, pixbuf[pbc][3]=176; n++; pbc++; }
n=0;
while ( n < 90) { pixbuf[pbc][0]=n; pixbuf[pbc][1]=30; pixbuf[pbc][2]=180, pixbuf[pbc][3]=176; n++; pbc++; }

n=0;

while ( n < 50) { pixbuf[pbc][0]=0; pixbuf[pbc][1]=n-20; pixbuf[pbc][2]=180, pixbuf[pbc][3]=176; n++; pbc++; }
n=0;
while ( n < 50) { pixbuf[pbc][0]=90; pixbuf[pbc][1]=n-20; pixbuf[pbc][2]=180, pixbuf[pbc][3]=176; n++; pbc++; }

n=0;
while ( n < 90) { pixbuf[pbc][0]=n; pixbuf[pbc][1]=-20; pixbuf[pbc][2]=100, pixbuf[pbc][3]=176; n++; pbc++; }
n=0;
while ( n < 90) { pixbuf[pbc][0]=n; pixbuf[pbc][1]=30; pixbuf[pbc][2]=100, pixbuf[pbc][3]=176; n++; pbc++; }

n=0;

while ( n < 50) { pixbuf[pbc][0]=0; pixbuf[pbc][1]=n-20; pixbuf[pbc][2]=100, pixbuf[pbc][3]=176; n++; pbc++; }
n=0;
while ( n < 50) { pixbuf[pbc][0]=90; pixbuf[pbc][1]=n-20; pixbuf[pbc][2]=100, pixbuf[pbc][3]=176; n++; pbc++; }

*/


n=5;
while ( n < 75) { pixbuf[pbc][0]=5; pixbuf[pbc][1]=25; pixbuf[pbc][2]=n, pixbuf[pbc][3]=176; n++; pbc++; }
n=5;
while ( n < 75) { pixbuf[pbc][0]=85; pixbuf[pbc][1]=25; pixbuf[pbc][2]=n, pixbuf[pbc][3]=176; n++; pbc++; }

n=5;
while ( n < 75) { pixbuf[pbc][0]=5; pixbuf[pbc][1]=-15; pixbuf[pbc][2]=n, pixbuf[pbc][3]=176; n++; pbc++; }
n=5;
while ( n < 75) { pixbuf[pbc][0]=85; pixbuf[pbc][1]=-15; pixbuf[pbc][2]=n, pixbuf[pbc][3]=176; n++; pbc++; }



n=5;
while ( n < 85) { pixbuf[pbc][0]=n; pixbuf[pbc][1]=-15; pixbuf[pbc][2]=75, pixbuf[pbc][3]=176; n++; pbc++; }
n=5;
while ( n < 85) { pixbuf[pbc][0]=n; pixbuf[pbc][1]=25; pixbuf[pbc][2]=75, pixbuf[pbc][3]=176; n++; pbc++; }

n=0;

while ( n < 40) { pixbuf[pbc][0]=5; pixbuf[pbc][1]=n-15; pixbuf[pbc][2]=75, pixbuf[pbc][3]=176; n++; pbc++; }
n=0;
while ( n < 40) { pixbuf[pbc][0]=85; pixbuf[pbc][1]=n-15; pixbuf[pbc][2]=75, pixbuf[pbc][3]=176; n++; pbc++; }

n=5;
while ( n < 85) { pixbuf[pbc][0]=n; pixbuf[pbc][1]=-15; pixbuf[pbc][2]=5, pixbuf[pbc][3]=176; n++; pbc++; }
n=5;
while ( n < 85) { pixbuf[pbc][0]=n; pixbuf[pbc][1]=25; pixbuf[pbc][2]=5, pixbuf[pbc][3]=176; n++; pbc++; }

n=0;

while ( n < 40) { pixbuf[pbc][0]=5; pixbuf[pbc][1]=n-15; pixbuf[pbc][2]=5, pixbuf[pbc][3]=176; n++; pbc++; }
n=0;
while ( n < 40) { pixbuf[pbc][0]=85; pixbuf[pbc][1]=n-15; pixbuf[pbc][2]=5, pixbuf[pbc][3]=176; n++; pbc++; }


//while ( n < 90) { pixbuf[n][0]=n; pixbuf[n][1]=30; pixbuf[n][2]=80, pixbuf[n][3]=176; n++; pbc++; }


n=0;
while ( n < pbc)
{
  float x_turned, y_turned, z_turned;	
	  x_turned= +(pixbuf[(int)n][0]-xshift)*COS(y_angl)+(pixbuf[(int)n][2]-zshift)*SIN(y_angl);
       z_turned= -(pixbuf[(int)n][0]-xshift)*SIN(y_angl)+(pixbuf[(int)n][2]-zshift)*COS(y_angl);

         pixbuf[(int)n][0]=x_turned+xshift;
        pixbuf[(int)n][2]=z_turned+zshift;
 n++;
}


n=0;
while ( n < pbc && n < 100000)
{
	if ( pixbuf[n][2]-zshift > 0)
	if ( (pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-zshift) +80> 4 && (pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-zshift) +80 < 156 && (pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-zshift) +40 > 4 && (pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-zshift) +40 < 85)
	if( screen_z[(int)(((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-zshift)))+80][(int)(((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-zshift)))+40  ]>pixbuf[n][2])\
 screen[(int)(((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-zshift)))+80][(int)(((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-zshift))) +40]=pixbuf[n][3],\
 screen_z[(int)(((pixbuf[n][0]-CAMX)*SCAL/(pixbuf[n][2]-zshift)))+80][(int)(((pixbuf[n][1]-CAMY)*SCAL/(pixbuf[n][2]-zshift))) +40]=pixbuf[n][2];
 n++;
}

system("cls\n");
  y=0;
 while ( y < 88)
 {
  x=0;
  while ( x < 158)
 {
  printf("%c%c%c%c%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y],screen[x+4][y],screen[x+5][y],screen[x+6][y],screen[x+7][y]);
  x+=8;
 }
 printf("\n");
 y++;
 }

zc=0;
float left, right, smallestr;
left=0, right=0; smallestr=
10000;
while ( zc < 54) {
                   if ( teilchen[zc].z < smallestr) smallestr=teilchen[zc].z;
                 
                   if ( zc%18 < 9) left+=teilchen[zc].z; else right+=teilchen[zc].z; zc++;
                  }
                  
                  printf(" %f %f %f ", left/27, right/27, smallestr);

 printf("%d",runs); // debug
if ( kbhit()) { unsigned char c; c=getch(),moved=1;

if(c=='u' ) xshift-= -2*SIN(-y_angl), zshift-= -2*COS(-y_angl);
   if(c=='j' ) xshift+= -2*SIN(-y_angl), zshift+= -2*COS(-y_angl);
   if(c=='h'  ) y_angl+=0.04;
   if(c=='k' ) y_angl-=0.04;
   if(c=='e') yshift-=2.0; // war 2.0
   if(c=='d') yshift+=2.0; 
if ( c=='p' ) getch();

}
}  

}

}
