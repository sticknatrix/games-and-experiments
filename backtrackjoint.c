/* The joint is deformed in a way the backtracking artificial intelligence prediction
  eventually detects how the ball that is falling down will hit the goal mark after it has
  touched the swinging arm.
  
  If you defined some positions with motion capturing and gave them a better evaluation in
   the backtracking in sequence, you could perhaps automatically combine recorded animation
   patterns in interaction with intelligent adaption to the environment, including physics.
   
   And if the computer had some sort of 'theory of mind', it could perhaps trick you with
   rote-saved gestures and mimics and also combine that with the interaction with the
   physical environment to achieve certain goals, again predefined states with a good
   evaluation.
   
   */
   



#include <stdio.h>
#include <math.h>

#define TZAHL 19
#define ELEK 0
#define PROT 1
#define NEUT 2

#define SCHEITEL 6.0
   #define LADUNG 17.0  // war 17.0
   #define MAXKRAFT 4.0 // war 4.0  // war beides 57.0
   #define TRAEGSCHWELLE 0.25 // war 1.0 // war 1.0
   #define TIMESLICE 50.0 // war nach oben 150.0
   #define VERLANGSFAC 1.00 // war 0.9
    
int lowest_depth;

int goalx=15, goaly=40;

typedef
   struct
{
 float x,y,z;
 float xv,yv,zv;
 float xv_old, yv_old ;
   int elec_or_prot;
  unsigned char symbol;
  unsigned char ID;
} teilchensize;

teilchensize teilchenb[TZAHL];



int backtrackjoint(float radius, float onegrow, float radius2, float twogrow, int depth)
{

 int n=0, n2=0;

 unsigned int ignore=0;


unsigned char screen[100][25];
int x,y;

teilchensize teilchenbuf[TZAHL];

float x_buf, y_buf, power, sign, distance;



if ( depth > 8) return 100;

/*

teilchenb[0].x=35.0, teilchenb[0].y=5,
teilchenb[0].xv=0.0, teilchenb[0].yv=0;
teilchenb[0].xv_old=0.0, teilchenb[0].yv_old=0.0;
teilchenb[0].elec_or_prot=PROT,
teilchenb[0].symbol='A';


teilchenb[1].x=35.0, teilchenb[1].y=11,
teilchenb[1].xv=0.0, teilchenb[1].yv=0.0;
teilchenb[1].xv_old=0.0, teilchenb[1].yv_old=0.0;
teilchenb[1].elec_or_prot=ELEK,
teilchenb[1].symbol='A';


teilchenb[2].x=35.0, teilchenb[2].y=17,
teilchenb[2].xv=0.0, teilchenb[2].yv=0.0;
teilchenb[2].xv_old=0.0, teilchenb[2].yv_old=0.0;
teilchenb[2].elec_or_prot=PROT,
teilchenb[2].symbol='A';



teilchenb[3+0].x=41.0, teilchenb[3+0].y=5,
teilchenb[3+0].xv=0.0, teilchenb[3+0].yv=0;
teilchenb[3+0].xv_old=0.0, teilchenb[3+0].yv_old=0.0;
teilchenb[3+0].elec_or_prot=ELEK,
teilchenb[3+0].symbol='A';


teilchenb[3+1].x=41.0, teilchenb[3+1].y=11,
teilchenb[3+1].xv=0.0, teilchenb[3+1].yv=0.0;
teilchenb[3+1].xv_old=0.0, teilchenb[3+1].yv_old=0.0;
teilchenb[3+1].elec_or_prot=PROT,
teilchenb[3+1].symbol='A';


teilchenb[3+2].x=41.0, teilchenb[3+2].y=17,
teilchenb[3+2].xv=0.0, teilchenb[3+2].yv=0.0;
teilchenb[3+2].xv_old=0.0, teilchenb[3+2].yv_old=0.0;
teilchenb[3+2].elec_or_prot=ELEK,
teilchenb[3+2].symbol='A';




teilchenb[6+0].x=47.0, teilchenb[6+0].y=5,
teilchenb[6+0].xv=0.0, teilchenb[6+0].yv=0;
teilchenb[6+0].xv_old=0.0, teilchenb[6+0].yv_old=0.0;
teilchenb[6+0].elec_or_prot=PROT,
teilchenb[6+0].symbol='A';


teilchenb[6+1].x=47.0, teilchenb[6+1].y=11,
teilchenb[6+1].xv=0.0, teilchenb[6+1].yv=0.0;
teilchenb[6+1].xv_old=0.0, teilchenb[6+1].yv_old=0.0;
teilchenb[6+1].elec_or_prot=ELEK,
teilchenb[6+1].symbol='A';


teilchenb[6+2].x=47.0, teilchenb[6+2].y=17,
teilchenb[6+2].xv=0.0, teilchenb[6+2].yv=0.0;
teilchenb[6+2].xv_old=0.0, teilchenb[6+2].yv_old=0.0;
teilchenb[6+2].elec_or_prot=PROT,
teilchenb[6+2].symbol='A';




// war bei y=9 am 13.06.2017 und xv=-20
teilchenb[9+0].x=35.0, teilchenb[9+0].y=23,
teilchenb[9+0].xv=0.0, teilchenb[9+0].yv=0;
teilchenb[9+0].xv_old=0.0, teilchenb[9+0].yv_old=0.0;
teilchenb[9+0].elec_or_prot=ELEK,
teilchenb[9+0].symbol='A';


teilchenb[9+1].x=41.0, teilchenb[9+1].y=23,
teilchenb[9+1].xv=0.0, teilchenb[9+1].yv=0.0;
teilchenb[9+1].xv_old=0.0, teilchenb[9+1].yv_old=0.0;
teilchenb[9+1].elec_or_prot=PROT,
teilchenb[9+1].symbol='A';


teilchenb[9+2].x=47.0, teilchenb[9+2].y=23,
teilchenb[9+2].xv=0.0, teilchenb[9+2].yv=0.0;
teilchenb[9+2].xv_old=0.0, teilchenb[9+2].yv_old=0.0;
teilchenb[9+2].elec_or_prot=ELEK,
teilchenb[9+2].symbol='A';



teilchenb[9+3+0].x=35.0, teilchenb[9+3+0].y=29,
teilchenb[9+3+0].xv=0.0, teilchenb[9+3+0].yv=0;
teilchenb[9+3+0].xv_old=0.0, teilchenb[9+3+0].yv_old=0.0;
teilchenb[9+3+0].elec_or_prot=PROT,
teilchenb[9+3+0].symbol='A';


teilchenb[9+3+1].x=41.0, teilchenb[9+3+1].y=29,
teilchenb[9+3+1].xv=0.0, teilchenb[9+3+1].yv=0.0;
teilchenb[9+3+1].xv_old=0.0, teilchenb[9+3+1].yv_old=0.0;
teilchenb[9+3+1].elec_or_prot=ELEK,
teilchenb[9+3+1].symbol='A';


teilchenb[9+3+2].x=47.0, teilchenb[9+3+2].y=29,
teilchenb[9+3+2].xv=0.0, teilchenb[9+3+2].yv=0.0;
teilchenb[9+3+2].xv_old=0.0, teilchenb[9+3+2].yv_old=0.0;
teilchenb[9+3+2].elec_or_prot=PROT,
teilchenb[9+3+2].symbol='A';



teilchenb[9+6+0].x=35.0, teilchenb[9+6+0].y=35,
teilchenb[9+6+0].xv=0.0, teilchenb[9+6+0].yv=0;
teilchenb[9+6+0].xv_old=0.0, teilchenb[9+6+0].yv_old=0.0;
teilchenb[9+6+0].elec_or_prot=ELEK,
teilchenb[9+6+0].symbol='A';


teilchenb[9+6+1].x=41.0, teilchenb[9+6+1].y=35,
teilchenb[9+6+1].xv=0.0, teilchenb[9+6+1].yv=0.0;
teilchenb[9+6+1].xv_old=0.0, teilchenb[9+6+1].yv_old=0.0;
teilchenb[9+6+1].elec_or_prot=PROT,
teilchenb[9+6+1].symbol='A';


teilchenb[9+6+2].x=47.0, teilchenb[9+6+2].y=35,
teilchenb[9+6+2].xv=0.0, teilchenb[9+6+2].yv=0.0;
teilchenb[9+6+2].xv_old=0.0, teilchenb[9+6+2].yv_old=0.0;
teilchenb[9+6+2].elec_or_prot=ELEK,
teilchenb[9+6+2].symbol='A';



teilchenb[9+6+3].x=17.0, teilchenb[9+6+3].y=33,
teilchenb[9+6+3].xv=5.0, teilchenb[9+6+3].yv=0.0;
teilchenb[9+6+3].xv_old=0.0, teilchenb[9+6+3].yv_old=0.0;
teilchenb[9+6+3].elec_or_prot=ELEK,
teilchenb[9+6+3].symbol='B';

*/

 float radiusM, radius2M;
 int runs=0;

 while(runs < 10)
 {
 	radiusM=0.0, radius2M=0.0;
   radius2=(6.0-radius)+6.0;
  n=0;
 while ( n < TZAHL)
{


  n2=0;
  while ( n2 < TZAHL)
  {
   if ( n2==n || fabs(teilchenb[n].x-teilchenb[n2].x) > 15 || fabs(teilchenb[n].y - teilchenb[n2].y) > 15 ) { n2++; continue;}

  x_buf=0, y_buf=0;
  

  power=
   sqrt ( 
    ( teilchenb[n].x - teilchenb[n2].x )  *
    ( teilchenb[n].x  - teilchenb[n2].x )+
       ( teilchenb[n].y - teilchenb[n2].y) *
    ( teilchenb[n].y - teilchenb[n2].y) );
  
    distance=power;

   x_buf=(teilchenb[n].x-teilchenb[n2].x);
   y_buf=(teilchenb[n].y-teilchenb[n2].y);


 
  if ( n==9||n2==9)
 {
   if ( power-radius < 0.0 ) sign= -1.0; else sign= 1.0;
   
    power=power-radius;
  
}
else if ( n==11||n2==11)
{
	
   if ( power-radius2 < 0.0 ) sign= -1.0; else sign= 1.0;
   
    power=power-radius2;
  
	
}
  else
 {
   if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   
    power=power-SCHEITEL;
  
}
  
  
  if ( power!= 0)
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
  else power=0.0;

    if ( ((teilchenb[n].elec_or_prot==ELEK && teilchenb[n2].elec_or_prot==PROT)||
         (teilchenb[n].elec_or_prot==PROT && teilchenb[n2].elec_or_prot==ELEK))&& (teilchenb[n].symbol==teilchenb[n2].symbol) 

) x_buf*=-1.0, y_buf*=-1.0;



/////////// GELENK-GEGENKRAFT ////////////////

radiusM=0.0, radius2M=0.0;

if ( teilchenb[n2].elec_or_prot==ELEK && n==9 ) radiusM=-0.01, radius2M=+0.01;
if ( teilchenb[n2].elec_or_prot==ELEK && n==11 ) radius2M=-0.01, radiusM=+0.01;

if ( n==9 && sqrt ( x_buf*x_buf+y_buf*y_buf ) < radiusM && teilchenb[n2].elec_or_prot==PROT) radiusM=-0.01, radius2M=+0.01;
 else if ( n==9 && teilchenb[n2].elec_or_prot==PROT) radiusM+=0.01, radius2M-=0.01; 

if ( n==11 && sqrt ( x_buf*x_buf+y_buf*y_buf ) < radiusM && teilchenb[n2].elec_or_prot==PROT) radiusM=+0.01, radius2M=-0.01;
 else if ( n==11 && teilchenb[n2].elec_or_prot==PROT)  radiusM=-0.01, radius2M=+0.01;

////////// GELENK-GEGENKRAFT ////////////



   if ( ((teilchenb[n].elec_or_prot==NEUT && teilchenb[n2].elec_or_prot==PROT)||
        (teilchenb[n2].elec_or_prot==NEUT && teilchenb[n].elec_or_prot==PROT) )&& sign > 0 ) x_buf=0, y_buf=0;
   
   
   if ( (teilchenb[n].elec_or_prot==NEUT || teilchenb[n2].elec_or_prot==NEUT) && sign > 0 ) x_buf=0, y_buf=0;
   
   
   if ( (teilchenb[n].elec_or_prot==NEUT && teilchenb[n2].elec_or_prot==ELEK)||
        (teilchenb[n2].elec_or_prot==NEUT && teilchenb[n].elec_or_prot==ELEK) ) x_buf=0, y_buf=0;
     // N wie Prot bedeutet: laengerer Stossvorgang?!
     
     if ( (teilchenb[n2].elec_or_prot== NEUT || teilchenb[n].elec_or_prot==NEUT) && ignore==1) x_buf=0, y_buf=0;
   
          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
    }  else x_buf=0, y_buf=0 ;


 
    teilchenb[n].xv+=x_buf, teilchenb[n].yv+=y_buf;
    
    n2++;
  }

  n++;

 }

n=0;
while ( n < TZAHL)
{
 teilchenb[n].yv+=0.02;
 n++;	
}

 n=0;
while (n < TZAHL)
{
 if ( n==3) n++;
 teilchenb[n].x+=teilchenb[n].xv/TIMESLICE,
 teilchenb[n].y+=teilchenb[n].yv/TIMESLICE;
 teilchenb[n].xv*=VERLANGSFAC, teilchenb[n].yv*=VERLANGSFAC;
 teilchenb[n].xv=(teilchenb[n].xv_old*1.0+teilchenb[n].xv)/2.0;
 teilchenb[n].yv=(teilchenb[n].yv_old*1.0+teilchenb[n].yv)/2.0;
 teilchenb[n].xv_old=teilchenb[n].xv;
 teilchenb[n].yv_old=teilchenb[n].yv;
 n++;
}

 if ( radius > 3.0 && radius < 9.0)
radius+=radiusM, radius2+=radius2M;

 if ( radius > 3.0 && radius < 9.0)
 radius+=onegrow, radius2+=twogrow;
runs++;

 if (  fabs(teilchenb[18].x-goalx ) < 1 && fabs(teilchenb[18].y-goaly) < 1 ) { if ( lowest_depth > depth)lowest_depth=depth; return lowest_depth; }
}

 
 memcpy(teilchenbuf, teilchenb, sizeof(teilchenb));
 backtrackjoint(radius,+0.05, radius2,-0.05, depth+1);
 memcpy(teilchenb, teilchenbuf, sizeof(teilchenb));
 backtrackjoint(radius,-0.05, radius2,+0.05, depth+1);
 memcpy(teilchenb, teilchenbuf, sizeof(teilchenb)); 
 
 return lowest_depth;
	
}

 
 
 
 //////MAINBEGINNTHIER//////////

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

unsigned int ignore=0;
float radiusM, radius2M;

float radius=6.0, radius2=6.0;

float rad1add=0.0, rad2add=0.0;

unsigned char screen[100][25];
int x,y;

float x_buf, y_buf, power, sign, distance;


int lowdep1, lowdep2;


teilchen[0].x=35.0, teilchen[0].y=5,
teilchen[0].xv=0.0, teilchen[0].yv=0;
teilchen[0].xv_old=0.0, teilchen[0].yv_old=0.0;
teilchen[0].elec_or_prot=PROT,
teilchen[0].symbol='A';


teilchen[1].x=35.0, teilchen[1].y=11,
teilchen[1].xv=0.0, teilchen[1].yv=0.0;
teilchen[1].xv_old=0.0, teilchen[1].yv_old=0.0;
teilchen[1].elec_or_prot=ELEK,
teilchen[1].symbol='A';


teilchen[2].x=35.0, teilchen[2].y=17,
teilchen[2].xv=0.0, teilchen[2].yv=0.0;
teilchen[2].xv_old=0.0, teilchen[2].yv_old=0.0;
teilchen[2].elec_or_prot=PROT,
teilchen[2].symbol='A';



teilchen[3+0].x=41.0, teilchen[3+0].y=5,
teilchen[3+0].xv=0.0, teilchen[3+0].yv=0;
teilchen[3+0].xv_old=0.0, teilchen[3+0].yv_old=0.0;
teilchen[3+0].elec_or_prot=ELEK,
teilchen[3+0].symbol='A';


teilchen[3+1].x=41.0, teilchen[3+1].y=11,
teilchen[3+1].xv=0.0, teilchen[3+1].yv=0.0;
teilchen[3+1].xv_old=0.0, teilchen[3+1].yv_old=0.0;
teilchen[3+1].elec_or_prot=PROT,
teilchen[3+1].symbol='A';


teilchen[3+2].x=41.0, teilchen[3+2].y=17,
teilchen[3+2].xv=0.0, teilchen[3+2].yv=0.0;
teilchen[3+2].xv_old=0.0, teilchen[3+2].yv_old=0.0;
teilchen[3+2].elec_or_prot=ELEK,
teilchen[3+2].symbol='A';




teilchen[6+0].x=47.0, teilchen[6+0].y=5,
teilchen[6+0].xv=0.0, teilchen[6+0].yv=0;
teilchen[6+0].xv_old=0.0, teilchen[6+0].yv_old=0.0;
teilchen[6+0].elec_or_prot=PROT,
teilchen[6+0].symbol='A';


teilchen[6+1].x=47.0, teilchen[6+1].y=11,
teilchen[6+1].xv=0.0, teilchen[6+1].yv=0.0;
teilchen[6+1].xv_old=0.0, teilchen[6+1].yv_old=0.0;
teilchen[6+1].elec_or_prot=ELEK,
teilchen[6+1].symbol='A';


teilchen[6+2].x=47.0, teilchen[6+2].y=17,
teilchen[6+2].xv=0.0, teilchen[6+2].yv=0.0;
teilchen[6+2].xv_old=0.0, teilchen[6+2].yv_old=0.0;
teilchen[6+2].elec_or_prot=PROT,
teilchen[6+2].symbol='A';




// war bei y=9 am 13.06.2017 und xv=-20
teilchen[9+0].x=35.0, teilchen[9+0].y=23,
teilchen[9+0].xv=0.0, teilchen[9+0].yv=0;
teilchen[9+0].xv_old=0.0, teilchen[9+0].yv_old=0.0;
teilchen[9+0].elec_or_prot=ELEK,
teilchen[9+0].symbol='A';


teilchen[9+1].x=41.0, teilchen[9+1].y=23,
teilchen[9+1].xv=0.0, teilchen[9+1].yv=0.0;
teilchen[9+1].xv_old=0.0, teilchen[9+1].yv_old=0.0;
teilchen[9+1].elec_or_prot=PROT,
teilchen[9+1].symbol='A';


teilchen[9+2].x=47.0, teilchen[9+2].y=23,
teilchen[9+2].xv=0.0, teilchen[9+2].yv=0.0;
teilchen[9+2].xv_old=0.0, teilchen[9+2].yv_old=0.0;
teilchen[9+2].elec_or_prot=ELEK,
teilchen[9+2].symbol='A';



teilchen[9+3+0].x=35.0, teilchen[9+3+0].y=29,
teilchen[9+3+0].xv=0.0, teilchen[9+3+0].yv=0;
teilchen[9+3+0].xv_old=0.0, teilchen[9+3+0].yv_old=0.0;
teilchen[9+3+0].elec_or_prot=PROT,
teilchen[9+3+0].symbol='A';


teilchen[9+3+1].x=41.0, teilchen[9+3+1].y=29,
teilchen[9+3+1].xv=0.0, teilchen[9+3+1].yv=0.0;
teilchen[9+3+1].xv_old=0.0, teilchen[9+3+1].yv_old=0.0;
teilchen[9+3+1].elec_or_prot=ELEK,
teilchen[9+3+1].symbol='A';


teilchen[9+3+2].x=47.0, teilchen[9+3+2].y=29,
teilchen[9+3+2].xv=0.0, teilchen[9+3+2].yv=0.0;
teilchen[9+3+2].xv_old=0.0, teilchen[9+3+2].yv_old=0.0;
teilchen[9+3+2].elec_or_prot=PROT,
teilchen[9+3+2].symbol='A';



teilchen[9+6+0].x=35.0, teilchen[9+6+0].y=35,
teilchen[9+6+0].xv=0.0, teilchen[9+6+0].yv=0;
teilchen[9+6+0].xv_old=0.0, teilchen[9+6+0].yv_old=0.0;
teilchen[9+6+0].elec_or_prot=ELEK,
teilchen[9+6+0].symbol='A';


teilchen[9+6+1].x=41.0, teilchen[9+6+1].y=35,
teilchen[9+6+1].xv=0.0, teilchen[9+6+1].yv=0.0;
teilchen[9+6+1].xv_old=0.0, teilchen[9+6+1].yv_old=0.0;
teilchen[9+6+1].elec_or_prot=PROT,
teilchen[9+6+1].symbol='A';


teilchen[9+6+2].x=47.0, teilchen[9+6+2].y=35,
teilchen[9+6+2].xv=0.0, teilchen[9+6+2].yv=0.0;
teilchen[9+6+2].xv_old=0.0, teilchen[9+6+2].yv_old=0.0;
teilchen[9+6+2].elec_or_prot=ELEK,
teilchen[9+6+2].symbol='A';


teilchen[9+6+3].x=17.0, teilchen[9+6+3].y=33,
teilchen[9+6+3].xv=15.0, teilchen[9+6+3].yv=0.0;
teilchen[9+6+3].xv_old=0.0, teilchen[9+6+3].yv_old=0.0;
teilchen[9+6+3].elec_or_prot=ELEK,
teilchen[9+6+3].symbol='B';

//while (!kbhit()) rand();

 while(1)
 {
 	
 	
if ( runs %600==0)
{
 teilchen[9+6+3].x=12.0+rand()%10, teilchen[9+6+3].y=8+rand()%10,
 teilchen[9+6+3].xv=3.0, teilchen[9+6+3].yv=0.0;
 teilchen[9+6+3].xv_old=0.0, teilchen[9+6+3].yv_old=0.0;
 teilchen[9+6+3].elec_or_prot=ELEK,
 teilchen[9+6+3].symbol='B';

 goalx=rand()%5 + 15, goaly= rand()%10 + 30;
}

 	
 	
 	
   radius2=(6.0-radius)+6.0;
  n=0;
 while ( n < TZAHL)
{
 radiusM=0.0, radius2M=0.0;

  n2=0;
  while ( n2 < TZAHL)
  {
   if ( n2==n || fabs(teilchen[n].x-teilchen[n2].x) > 15 || fabs(teilchen[n].y - teilchen[n2].y) > 15) { n2++; continue;}

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


 
  if ( n==9||n2==9)
 {
   if ( power-radius < 0.0 ) sign= -1.0; else sign= 1.0;
   
    power=power-radius;
  
}
else if ( n==11||n2==11)
{
	
   if ( power-radius2 < 0.0 ) sign= -1.0; else sign= 1.0;
   
    power=power-radius2;
  
	
}
  else
 {
   if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   
    power=power-SCHEITEL;
  
}
  
  
  if ( power!= 0)
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
  else power=0.0;

    if ( ((teilchen[n].elec_or_prot==ELEK && teilchen[n2].elec_or_prot==PROT)||
         (teilchen[n].elec_or_prot==PROT && teilchen[n2].elec_or_prot==ELEK))&& (teilchen[n].symbol==teilchen[n2].symbol) 

) x_buf*=-1.0, y_buf*=-1.0;




/////////// GELENK-GEGENKRAFT ////////////////

radiusM=0.0, radius2M=0.0;

if ( teilchen[n2].elec_or_prot==ELEK && n==9 ) radiusM=-0.01, radius2M=+0.01;
if ( teilchen[n2].elec_or_prot==ELEK && n==11 ) radius2M=-0.01, radiusM=+0.01;

if ( n==9 && sqrt ( x_buf*x_buf+y_buf*y_buf ) < radiusM && teilchen[n2].elec_or_prot==PROT) radiusM=-0.01, radius2M=+0.01;
 else if ( n==9 && teilchen[n2].elec_or_prot==PROT) radiusM+=0.01, radius2M-=0.01; 

if ( n==11 && sqrt ( x_buf*x_buf+y_buf*y_buf ) < radiusM && teilchen[n2].elec_or_prot==PROT) radiusM=+0.01, radius2M=-0.01;
 else if ( n==11 && teilchen[n2].elec_or_prot==PROT)  radiusM=-0.01, radius2M=+0.01;

////////// GELENK-GEGENKRAFT ////////////



   if ( ((teilchen[n].elec_or_prot==NEUT && teilchen[n2].elec_or_prot==PROT)||
        (teilchen[n2].elec_or_prot==NEUT && teilchen[n].elec_or_prot==PROT) )&& sign > 0 ) x_buf=0, y_buf=0;
   
   
   if ( (teilchen[n].elec_or_prot==NEUT || teilchen[n2].elec_or_prot==NEUT) && sign > 0 ) x_buf=0, y_buf=0;
   
   
   if ( (teilchen[n].elec_or_prot==NEUT && teilchen[n2].elec_or_prot==ELEK)||
        (teilchen[n2].elec_or_prot==NEUT && teilchen[n].elec_or_prot==ELEK) ) x_buf=0, y_buf=0;
     // N wie Prot bedeutet: laengerer Stossvorgang?!
     
     if ( (teilchen[n2].elec_or_prot== NEUT || teilchen[n].elec_or_prot==NEUT) && ignore==1) x_buf=0, y_buf=0;
   
          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
    }  else x_buf=0, y_buf=0 ;


 
    teilchen[n].xv+=x_buf, teilchen[n].yv+=y_buf;
    
    n2++;
  }

  n++;

 }


n=0;
while ( n < TZAHL)
{
 teilchen[n].yv+=0.02;
 n++;	
}


 n=0;
while (n < TZAHL)
{
 if ( n==3) n++;
 teilchen[n].x+=teilchen[n].xv/TIMESLICE,
 teilchen[n].y+=teilchen[n].yv/TIMESLICE;
 teilchen[n].xv*=VERLANGSFAC, teilchen[n].yv*=VERLANGSFAC;
 teilchen[n].xv=(teilchen[n].xv_old*1.0+teilchen[n].xv)/2.0;
 teilchen[n].yv=(teilchen[n].yv_old*1.0+teilchen[n].yv)/2.0;
 teilchen[n].xv_old=teilchen[n].xv;
 teilchen[n].yv_old=teilchen[n].yv;
 
 n++;
}
 if ( radius > 3.0 && radius < 9.0) 
radius+=radiusM, radius2+=radius2M;

if ( (runs)%10==0)
{
 x=0;
while ( x < 100)
{
 y=0;
 while ( y < 25)
{
 screen[x][y]=' ';
 y++;
}
x++;
}

float mx, my;
 mx=0.0, my=0.0;
 
 
n=0;
while ( n < TZAHL)
{
  mx+=teilchen[n].x, my+=teilchen[n].y;
  if ( (teilchen[n].x+0.1)/3 > 0 && (teilchen[n].x+0.1)/3 < 99 && (teilchen[n].y+0.1)/3 > 0 && (teilchen[n].y+0.1)/3 < 22)
 screen[(int)(teilchen[n].x+0.1)/3][(int)(teilchen[n].y+0.1)/3]=219; // teilchen[n].symbol;
n++;

}

screen[goalx/3][goaly/3]='X';

system("cls\n");
  y=0;
 while ( y < 23)
 {
  x=0;
  while ( x < 78)
 {
  printf("%c",screen[x][y] );
  x++;
 }
 printf("\n");
 y++;
 }
//if ( kbhit()) { y=getch(); if (y=='+' )radius+=0.1; else if ( y=='-' && radius > 0.1) radius-=0.1; }

lowest_depth=100;

 memcpy(teilchenb, teilchen, sizeof( teilchenb) );

 lowdep1=backtrackjoint(radius,+0.05, radius2,-0.05,0);
 //printf("%d",lowdep1), getch();
 lowest_depth=100;

 memcpy(teilchenb, teilchen, sizeof( teilchenb ) );

 lowdep2=backtrackjoint(radius,-0.05, radius2,+0.05,0);  
 //printf("%d",lowdep2), getch();
   //printf("%d", lowdep2), getch();   
     
//printf("%f %f", mx/18.0, my/18,0);


     if ( lowdep1!=100 || lowdep2!=100)
     {
  if ( lowdep1 < lowdep2 ) rad1add=0.05, rad2add=-0.05 ;
      else rad1add=-0.05, rad2add=0.05;
    } else rad1add=0.0, rad2add=0.0;

//printf("%f %f", radius, radius2);

}
  
  if ( radius > 3.0 && radius < 9.0)
  radius+=rad1add, radius2+=rad2add;
 
 if (  fabs(teilchen[18].x-goalx ) < 1 && fabs(teilchen[18].y-goaly) < 1 ) printf("\a");
  
  runs++;


}

}

