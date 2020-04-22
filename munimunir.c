
#include <stdio.h>
#include <time.h>
#include <math.h>

#define TZAHL 71
#define ELEK 0
#define PROT 1
#define NEUT 2

#define SCHEITEL 3.0
   #define LADUNG 17.0  // war 17.0
   #define MAXKRAFT 4.0 // war 4.0  // war beides 57.0
   #define TRAEGSCHWELLE 1.0 // war 1.0 // war 1.0
   #define TIMESLICE 50.0 // war nach oben 150.0
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


*/

#define SIN(val) sintab[(int)(val*10) %64 +63]

/*
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

#define COS(val) costab[(int)(val*10) %64 +63]




unsigned char wheel[400]={
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
        0,0,0,1,1,1,0,0,0,0,1,0,0,1,1,0,0,0,0,0,
        0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,
        0,0,0,1,1,0,1,1,0,0,1,0,0,1,0,1,1,0,0,0,
        0,0,1,1,0,0,1,1,0,0,1,0,1,0,0,0,1,0,0,0,
        0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,0,0,0,
        0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
        0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,
        0,0,1,1,1,0,0,1,1,0,1,0,1,0,0,1,1,0,0,0,
        0,0,0,1,1,0,1,1,0,0,1,0,0,1,0,1,1,0,0,0,
        0,0,0,0,1,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,
        0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,0,0,
        0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
        0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0             };



unsigned char forks[400]={
         0,0,0,0,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,
		 0,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,
		 0,0,0,0,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,
		 0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		 };


unsigned char screen[160][90];




void findwheelchange(float *x_vec, float *y_vec, float *x_vec2, float *y_vec2, double xstartsamp, double xendsamp, double ystartsamp, double yendsamp, float speed )
{
  
 double x_rot, y_rot;
   float distance;
   
  distance= sqrt((xstartsamp-xendsamp)*(xstartsamp-xendsamp)+(ystartsamp-yendsamp)*(ystartsamp-yendsamp));
   
    if ( distance==0.0) distance=0.001;
   
   
   y_rot=xendsamp-xstartsamp;
   x_rot=yendsamp-ystartsamp;
   
   
   *x_vec=*x_vec+x_rot/distance*speed;
   *y_vec=*y_vec+y_rot/distance*speed;

   *x_vec2=*x_vec2- x_rot/distance*speed;
   *y_vec2=*y_vec2- y_rot/distance*speed;

   //printf("%f %f",*x_vec, *y_vec), getch();

   //printf("%f %f",x_rot/distance*speed,y_rot/distance*speed), getch();

}


void put_sprite(int x_set, int y_set, double xstartsamp, double xendsamp, double ystartsamp, double yendsamp, unsigned char *pattern)
{
  // man muesste nur Quadranten durchprobieren; den Gradwert kriegt man mit atan() direkt?	
  float angle;
  unsigned char shape_copy[1024]={
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};	
 
 double x_rot, y_rot;
 
 int x,y;
 
 
  double sampangl, distance, nearest, nearestdist;
  
  nearest=1000000;
  
  
  distance= sqrt((xstartsamp-xendsamp)*(xstartsamp-xendsamp)+(ystartsamp-yendsamp)*(ystartsamp-yendsamp));
   
   //printf("%f", distance), getch();
   
   angle=-6.4;
   
   while ( angle < 6.4)
   {
   	
		 x_rot=  +((xendsamp-xstartsamp))*cos(angle)-((yendsamp-ystartsamp))*sin(angle);
          y_rot=  ((xendsamp-xstartsamp))*sin(angle)+((yendsamp-ystartsamp))*cos(angle);
           
          
          x_rot+=xstartsamp;
          y_rot+=ystartsamp;
                   
          
          
          
          (nearestdist= ( (xstartsamp-x_rot)*(xstartsamp-x_rot)+((yendsamp+distance)-y_rot)*((ystartsamp+distance)-y_rot)));
          if (  nearestdist < nearest) nearest=nearestdist, sampangl=angle;//, printf("%f %f %f\a\n",xstartsamp, xendsamp, sampangl), getch();
   	  
   	angle+=0.1;
   }
   
   /*
   printf("X1: %f X2: %f Y1: %fY2: %f\n",xstartsamp,xendsamp,ystartsamp,yendsamp);
   printf("%f ", sampangl), getch();
   */
   
   angle=sampangl; //-3.14/4;
   
 
 y=0;
 
 while ( y < 20)
 {
 	x=0;
 	while ( x < 20)
 	{
 	    if ( pattern[x+y*20]!=0)
		 {
		  
		  
		  x_rot= +(x-8.0)*COS(angle)-(y-8.0)*SIN(angle);
          y_rot= (x-8.0)*SIN(angle)+(y-8.0)*COS(angle);
          
          
          x_rot+=10.0, y_rot+=10.0;
          if ( !( x_rot < 0 || x_rot > 19 || y_rot < 0 || y_rot > 19 ) )
	      if ( pattern[x+y*20]==3) shape_copy[(int)(x_rot)+((int)(y_rot)*20)]=177;else 
		  shape_copy[(int)(x_rot)+((int)(y_rot)*20)]=pattern[x+y*20]==1 ? 176 : 219;	
		 }
		 
		 	
 		x++;
	 }
	 y++;
 }


 y=0;
 while ( y < 20)
 {
  x=0;
  while ( x < 20)
  {
   if ( x_set+x >= 0 &&
        x_set+x < 160 &&
        
        y_set+y >= 0 &&
        y_set+y < 88   
         )
   {
    if ( shape_copy[x+y*20]!=0)screen[x+x_set][y+y_set]=shape_copy[x+y*20];
   }
   x++;
  }
  y++;
 }
  return;
}





unsigned char floormap[50]=
{ 10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,1,2,3,4,5,6,7,8,7,6,5,4,3,2,1,0,0,0,0,0,0,1,2,3,2,1,0,0,0,0,2,4,8,10,12};
    
 

int main(void)
{
  long int runs=0;
  struct
{
 float x,y,z;
 float xv,yv,zv;
 float xv_old, yv_old ;
   int elec_or_prot;
  unsigned char symbol;
  unsigned char ID;
} teilchen[TZAHL];

int n=0, n2=0;

signed int scroll=0;

int x,y;

float x_buf, y_buf, power, sign, distance;


teilchen[0].x=35.0, teilchen[0].y=5,
teilchen[0].xv=0.0, teilchen[0].yv=0;
teilchen[0].xv_old=0.0, teilchen[0].yv_old=0.0;
teilchen[0].elec_or_prot=PROT,
teilchen[0].symbol='R';


teilchen[1].x=35.0, teilchen[1].y=8,
teilchen[1].xv=0.0, teilchen[1].yv=0.0;
teilchen[1].xv_old=0.0, teilchen[1].yv_old=0.0;
teilchen[1].elec_or_prot=ELEK,
teilchen[1].symbol='R';


teilchen[2].x=35.0, teilchen[2].y=11,
teilchen[2].xv=0.0, teilchen[2].yv=0.0;
teilchen[2].xv_old=0.0, teilchen[2].yv_old=0.0;
teilchen[2].elec_or_prot=PROT,
teilchen[2].symbol='R';



teilchen[3+0].x=38.0, teilchen[3+0].y=5,
teilchen[3+0].xv=0.0, teilchen[3+0].yv=0;
teilchen[3+0].xv_old=0.0, teilchen[3+0].yv_old=0.0;
teilchen[3+0].elec_or_prot=ELEK,
teilchen[3+0].symbol='R';


teilchen[3+1].x=38.0, teilchen[3+1].y=8,
teilchen[3+1].xv=0.0, teilchen[3+1].yv=0.0;
teilchen[3+1].xv_old=0.0, teilchen[3+1].yv_old=0.0;
teilchen[3+1].elec_or_prot=PROT,
teilchen[3+1].symbol='R';


teilchen[3+2].x=38.0, teilchen[3+2].y=11,
teilchen[3+2].xv=0.0, teilchen[3+2].yv=0.0;
teilchen[3+2].xv_old=0.0, teilchen[3+2].yv_old=0.0;
teilchen[3+2].elec_or_prot=ELEK,
teilchen[3+2].symbol='R';




teilchen[6+0].x=41.0, teilchen[6+0].y=5,
teilchen[6+0].xv=0.0, teilchen[6+0].yv=0;
teilchen[6+0].xv_old=0.0, teilchen[6+0].yv_old=0.0;
teilchen[6+0].elec_or_prot=PROT,
teilchen[6+0].symbol='R';


teilchen[6+1].x=41.0, teilchen[6+1].y=8,
teilchen[6+1].xv=0.0, teilchen[6+1].yv=0.0;
teilchen[6+1].xv_old=0.0, teilchen[6+1].yv_old=0.0;
teilchen[6+1].elec_or_prot=ELEK,
teilchen[6+1].symbol='R';


teilchen[6+2].x=41.0, teilchen[6+2].y=11,
teilchen[6+2].xv=0.0, teilchen[6+2].yv=0.0;
teilchen[6+2].xv_old=0.0, teilchen[6+2].yv_old=0.0;
teilchen[6+2].elec_or_prot=PROT,
teilchen[6+2].symbol='R';




teilchen[9].x=38.0, teilchen[9].y=2,
teilchen[9].xv=0.0, teilchen[9].yv=0.0;
teilchen[9].xv_old=0.0, teilchen[9].yv_old=0.0;
teilchen[9].elec_or_prot=PROT,
teilchen[9].symbol='R';


teilchen[10].x=32.0, teilchen[10].y=8,
teilchen[10].xv=0.0, teilchen[10].yv=0.0;
teilchen[10].xv_old=0.0, teilchen[10].yv_old=0.0;
teilchen[10].elec_or_prot=PROT,
teilchen[10].symbol='R';

teilchen[11].x=44.0, teilchen[11].y=8,
teilchen[11].xv=0.0, teilchen[11].yv=0.0;
teilchen[11].xv_old=0.0, teilchen[11].yv_old=0.0;
teilchen[11].elec_or_prot=PROT,
teilchen[11].symbol='R';

teilchen[20].x=38.0, teilchen[20].y=14,
teilchen[20].xv=0.0, teilchen[20].yv=0.0;
teilchen[20].xv_old=0.0, teilchen[20].yv_old=0.0;
teilchen[20].elec_or_prot=PROT,
teilchen[20].symbol='R';



teilchen[12].x=38.0, teilchen[12].y=8,
teilchen[12].xv=0.0, teilchen[12].yv=0.0;
teilchen[12].xv_old=0.0, teilchen[12].yv_old=0.0;
teilchen[12].elec_or_prot=PROT,
teilchen[12].symbol='S';

teilchen[13].x=38.0, teilchen[13].y=5,
teilchen[13].xv=0.0, teilchen[13].yv=0.0;
teilchen[13].xv_old=0.0, teilchen[13].yv_old=0.0;
teilchen[13].elec_or_prot=ELEK,
teilchen[13].symbol='S';

teilchen[14].x=35.0, teilchen[14].y=5,
teilchen[14].xv=0.0, teilchen[14].yv=0.0;
teilchen[14].xv_old=0.0, teilchen[14].yv_old=0.0;
teilchen[14].elec_or_prot=PROT,
teilchen[14].symbol='S';

teilchen[15].x=38.0, teilchen[15].y=2,
teilchen[15].xv=0.0, teilchen[15].yv=0.0;
teilchen[15].xv_old=0.0, teilchen[15].yv_old=0.0;
teilchen[15].elec_or_prot=PROT,
teilchen[15].symbol='S';

teilchen[16].x=35.0, teilchen[16].y=2,
teilchen[16].xv=0.0, teilchen[16].yv=0.0;
teilchen[16].xv_old=0.0, teilchen[16].yv_old=0.0;
teilchen[16].elec_or_prot=ELEK,
teilchen[16].symbol='S';


teilchen[17].x=38.0, teilchen[17].y=-1,
teilchen[17].xv=0.0, teilchen[17].yv=0.0;
teilchen[17].xv_old=0.0, teilchen[17].yv_old=0.0;
teilchen[17].elec_or_prot=ELEK,
teilchen[17].symbol='S';

teilchen[18].x=35.0, teilchen[18].y=-1,
teilchen[18].xv=0.0, teilchen[18].yv=0.0;
teilchen[18].xv_old=0.0, teilchen[18].yv_old=0.0;
teilchen[18].elec_or_prot=PROT,
teilchen[18].symbol='S';


teilchen[19].x=1141.0, teilchen[19].y=2,
teilchen[19].xv=0.0, teilchen[19].yv=0.0;
teilchen[19].xv_old=0.0, teilchen[19].yv_old=0.0;
teilchen[19].elec_or_prot=PROT,
teilchen[19].symbol='D'; // der 'Drive'


sininit();
cosinit();

float touch=0,notouch=0;

n=21;
while ( n < 71)
{
 teilchen[n].y=50-floormap[n], teilchen[n].x=(n-21)*5;
 teilchen[n].symbol='F';	
 n++;	
}

clock_t delay;

delay=0;

 while(1)
 {
 
  n=0;
 while ( n < TZAHL-50)
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
    ( teilchen[n].y - teilchen[n2].y) );
  
    distance=power;

   x_buf=(teilchen[n].x-teilchen[n2].x);
   y_buf=(teilchen[n].y-teilchen[n2].y);
 
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   
   power=power-SCHEITEL;
  
  if ( power!= 0)
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
  else power=0.0;


   // if ( n2 > 19) power*=50;
  if (!(n2 > 20))
  {
    if ( (n==19||n2==19) && ( n==15|| n2==15))
    {
     x_buf*=-1.0, y_buf*=-1.0;
    }
    else
    if ( (n==19||n2==19) && (teilchen[n2].symbol=='S' || teilchen[n].symbol=='S' ) )
    {
      x_buf=0, y_buf=0;	
	}
	else if ( n==19 || n2==19)
	{
	 power*=0.25;
	}
	else
    if ( (n==12 && n2==4) || (n==4 && n2==12) ) power*=1.0;
	else if( teilchen[n2].symbol=='F'|| teilchen[n].symbol=='F');
    else
    if ( (teilchen[n].symbol=='R' && teilchen[n2].symbol=='S')||
	     (teilchen[n].symbol=='S' && teilchen[n2].symbol=='R' ) ) x_buf=0, y_buf=0;
     else
    if ( ((teilchen[n].elec_or_prot==ELEK && teilchen[n2].elec_or_prot==PROT)||
         (teilchen[n].elec_or_prot==PROT && teilchen[n2].elec_or_prot==ELEK))&& (teilchen[n].symbol==teilchen[n2].symbol) 

) x_buf*=-1.0, y_buf*=-1.0;

   else
   if ( ((teilchen[n].elec_or_prot==NEUT && teilchen[n2].elec_or_prot==PROT)||
        (teilchen[n2].elec_or_prot==NEUT && teilchen[n].elec_or_prot==PROT) )&& sign > 0 ) x_buf=0, y_buf=0;
   
   else
   if ( (teilchen[n].elec_or_prot==NEUT || teilchen[n2].elec_or_prot==NEUT) && sign > 0 ) x_buf=0, y_buf=0;
   
   else
   if ( (teilchen[n].elec_or_prot==NEUT && teilchen[n2].elec_or_prot==ELEK)||
        (teilchen[n2].elec_or_prot==NEUT && teilchen[n].elec_or_prot==ELEK) ) x_buf=0, y_buf=0;
     else
     if ( (teilchen[n2].elec_or_prot== NEUT || teilchen[n].elec_or_prot==NEUT) ) x_buf=0, y_buf=0;
   
    }
   
   
          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
        
        if ( n2 < 21)
        {
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
    }  else x_buf=0, y_buf=0 ;
       
     }
     else
     {
      
        #define TRAEGSCHWELLEB 4.0
      
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLEB )
      { 
        power= (distance-TRAEGSCHWELLE)/ distance;
         x_buf*=power*2; y_buf*=power*2;
		 if ( n<12|| n==20)notouch++;
		 //printf("Beruehrt Boden\a");
         
       }  else { x_buf=0, y_buf=0;}
     	
	 }

    if ( n==4 || n==12) x_buf*=0.2, y_buf*=0.2;
    teilchen[n].xv+=x_buf, teilchen[n].yv+=y_buf;
    
    n2++;
  }

  n++;

 }

if ( notouch==14) touch=0, notouch=0; else touch=1;


n=0;
while ( n < TZAHL-50)
{
 teilchen[n].yv+=0.0009;	
 n++;	
}

 n=0;
while (n < TZAHL-50)
{
 teilchen[n].x+=teilchen[n].xv/TIMESLICE,
 teilchen[n].y+=teilchen[n].yv/TIMESLICE;
 teilchen[n].xv*=VERLANGSFAC, teilchen[n].yv*=VERLANGSFAC;
 teilchen[n].xv=(teilchen[n].xv_old*1.0+teilchen[n].xv)/2.0;
 teilchen[n].yv=(teilchen[n].yv_old*1.0+teilchen[n].yv)/2.0;
 teilchen[n].xv_old=teilchen[n].xv;
 teilchen[n].yv_old=teilchen[n].yv;
 n++;
}


if ( (runs++)%200==0)
{
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

scroll=teilchen[18].x-35;
if ( scroll > 89) scroll=89;
if ( scroll < 0) scroll=0;

//system("cls\n");
//printf("%f", teilchen[18].x), getch();

 y=0;
while ( y < 90)
{
 x=0;
 while ( x*5 < 155)
{
 if ( x+scroll<160)screen[x*5][50-floormap[x+scroll/5]]=219;
 if ( x+1+scroll<160)screen[1+x*5][50-floormap[x+scroll/5]]='*';
 if ( x+2+scroll<160)screen[2+x*5][50-floormap[x+scroll/5]]=219;
 if ( x+3+scroll<160)screen[3+x*5][50-floormap[x+scroll/5]]='*';
 if ( x+3+scroll<160)screen[4+x*5][50-floormap[x+scroll/5]]=219;

 x++;
}
 y++;
}


n=0;
//while ( n < TZAHL-50)
//{
 // if ( (teilchen[n].x-scroll) > 0 && (teilchen[n].x-scroll) < 158 && (teilchen[n].y) > 0 && (teilchen[n].y) < 88)
// screen[(int)(teilchen[n].x-scroll)][(int)(teilchen[n].y)]=teilchen[n].symbol;

 float zerobuf;
  zerobuf=teilchen[0].y-teilchen[1].y; if ( zerobuf==0) zerobuf=0.001;
 put_sprite(teilchen[4].x-scroll-10 , teilchen[4].y-10, teilchen[0].x,teilchen[4].x,teilchen[0].y,teilchen[4].y, wheel );
  zerobuf=teilchen[12].y-teilchen[17].y; if ( zerobuf==0) zerobuf=0.001;
 put_sprite(teilchen[12].x-scroll-10 , teilchen[12].y-10, teilchen[17].x,teilchen[12].x,teilchen[17].y,teilchen[12].y, forks );
 if ( teilchen[4].x-scroll > 0) screen[(int)teilchen[4].x-scroll][(int)teilchen[4].y]='X';
 //if ( kbhit()) printf("Teil4: %f %f Teil12:%f %f", teilchen[4].x, teilchen[4].y, teilchen[12].x, teilchen[12].y),getch(),getch();
//n++;
//}

system("cls\n");
  y=0;
 while ( y < 88)
 {
  x=0;
  while ( x < 160)
 {
  printf("%c%c%c%c",screen[x][y], screen[x+1][y], screen[x+2][y],screen[x+3][y] );
  x+=4;
 }
 printf("\n");
 y++;
 }
 

 

  while ( delay > clock());
  delay=clock()+100; 

   }
  unsigned char c;
  if ( kbhit()){ 
  
     c=getch();
    if ( c=='o'){
	 findwheelchange(&teilchen[0].xv, &teilchen[0].yv,
	                &teilchen[6].xv, &teilchen[6].yv,teilchen[12].x, teilchen[0].x, teilchen[12].x, teilchen[0].y,1.5); }
	
	             // teilchen[19].xv+=1, teilchen[15].xv-=1;
    else if (c=='p') {                               findwheelchange(&teilchen[0].xv, &teilchen[0].yv,
                                                      &teilchen[6].xv, &teilchen[6].yv,
	                                                  teilchen[12].x, teilchen[0].x, teilchen[12].x, teilchen[0].y,-1.5); }
   }
}

}

