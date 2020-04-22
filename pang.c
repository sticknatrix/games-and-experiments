#include <stdio.h>
#include <math.h>
#include <time.h>


#define BIG  0
#define SMALL 1

int ballnum=4;
 
 struct
 {
  float x,y;
  float xvec, yvec;
  float xv_old, yv_old;
  int type;
 } ball[20];      /* der Ball */
 
 
 struct
 {
  float pos;
  int shoot, wait;
  int score;
 
 }player[2];  /* Spieler und Computer */

 
void split(void)
{
	int n=0,n2=0,n3=0;
    signed int y_buf[2]={-1000,0};

 n2=0;
 while ( n2 < ballnum)
 {
 
  while ( !(fabs(ball[n].x-player[0].pos) < 2) && n < ballnum ) n++;
   
   if ( n < ballnum)
   if ( ball[n].y > y_buf[0]) y_buf[0]=ball[n].y, y_buf[1]=n;
 
  n2++;
 }
  
  if ( y_buf[0]!= -1000)
  {
  	
	n=y_buf[1];
  	
	if ( ball[n].type==BIG)
	{
	      
	 n2=ballnum;
  	 ballnum++;
  	  ball[n2].xvec=ball[n2].xv_old=ball[n].xvec+2;
  	  ball[n2].yvec=ball[n2].yv_old=ball[n].yvec;
  	  ball[n2].x=ball[n].x+1, ball[n2].y=ball[n].y;
  	  ball[n2].type=SMALL;
  	  ball[n].type=SMALL;
  	  ball[n].xvec-=2;
  	  ball[n].x-=1;
  	  
    }
    else
    if ( ball[n].type==SMALL)
    {
     ballnum--;
     if ( ballnum!=0)
     { 
      ball[n].x=ball[ballnum].x;
      ball[n].y=ball[ballnum].y;
      ball[n].xvec=ball[n].xv_old=ball[ballnum].xvec;
      ball[n].yvec=ball[n].yv_old=ball[ballnum].yvec;
	  ball[n].type=ball[ballnum].type;	
      }
	}
  	
  }
  	
	
}


int main(void)
{
 float xbuf,ybuf;
 
 long int runs=0;
 

  ball[0].x=20, ball[0].y=5, ball[0].xvec=ball[0].xv_old=3, ball[0].yvec=ball[0].yv_old=0; ball[0].type=BIG;
 ball[1].x=30, ball[1].y=5, ball[1].xvec=ball[1].xv_old=3, ball[1].yvec=ball[1].yv_old=0; ball[1].type=SMALL;

 ball[2].x=10, ball[2].y=5, ball[2].xvec=ball[2].xv_old=-4, ball[2].yvec=ball[2].yv_old=0; ball[2].type=BIG;
 
 ball[3].x=60, ball[3].y=3, ball[3].xvec=ball[3].xv_old=-3, ball[3].yvec=ball[3].yv_old=0; ball[3].type=BIG;
 
 
 float distance, power;
 int x_buf, y_buf;
 int x,y;
 int sign;
 int n, n2;
 
 char screen[80][25];
 
 player[0].score=0, player[0].pos=40;

 while ( 1 )
 {
  n=0;
  while ( n < ballnum)
  {
   n2=0;
   while ( n2 < ballnum)
   {
   	if ( n==n2) {n2++; continue;}
   	
   xbuf=0, ybuf=0;
  

  power=
   sqrt ( 
    ( ball[n].x - ball[n2].x )  *
    ( ball[n].x - ball[n2].x )+
       ( ball[n].y - ball[n2].y) *
    ( ball[n].y - ball[n2].y) );
  
    distance=power;

   x_buf=(ball[n].x - ball[n2].x);
   y_buf=(ball[n].y - ball[n2].y);
 
 
 #define SCHEITEL_A 2.5
 #define SCHEITEL_B 1.5
 #define SCHEITEL_C 0.5
   #define LADUNG 20
   #define LADUNG_B 10
   #define MAXKRAFT 1
   #define TRAEGSCHWELLE 0.5
   #define TIMESLICE 40.0
    
 
 if ( ball[n].type==BIG && ball[n2].type==BIG)
 {
   if ( power-SCHEITEL_A < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL_A;
  
  }
  
  
 if ( (ball[n].type==SMALL && ball[n2].type==BIG)||
      (ball[n].type==BIG && ball[n2].type==SMALL) )
 {
   if ( power-SCHEITEL_B < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL_B;
  
  }
  
  
 if ( ball[n].type==SMALL && ball[n2].type==SMALL )
 {
   if ( power-SCHEITEL_C < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL_C;
  }


    if ( ball[n2].type==BIG)
    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;
    else if ( ball[n2].type==SMALL)
     power= ( LADUNG_B / (( power * power)+MAXKRAFT) ) *sign  ;


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

   
    ball[n].xvec+=x_buf, ball[n].yvec+=y_buf;
    n2++;
    }
   n++;
  }
  
  n=0;
  while ( n < ballnum)
  {
  
    ball[n].yvec+=0.3;       // Schwerkraft
   n++;
  }



     // Boden

   n=0;
    while ( n < ballnum)
 {
	       
       xbuf=0, ybuf=0;
  

  power=
   sqrt ( 
    ( ball[n].y - 20 )  *
    ( ball[n].y - 20 )+
      0 );
  
    distance=power;

   x_buf=0;
   y_buf=(ball[n].y-20);
 
 
   if ( power-SCHEITEL_C < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL_C;
  



    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;


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

   
    ball[n].xvec+=x_buf, ball[n].yvec+=y_buf;
    n++;
  }



n=0;  // linke Bande
    while ( n < ballnum)
 {
	       
       xbuf=0, ybuf=0;
  

  power=
   sqrt ( 
    ( ball[n].x - 0 )  *
    ( ball[n].x - 0 )+
      0 );
  
    distance=power;

   y_buf=0;
   x_buf=(ball[n].x-0);
 
 
   if ( power-SCHEITEL_C < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL_C;
  



    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;


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

   
    ball[n].xvec+=x_buf, ball[n].yvec+=y_buf;
    n++;
  }


n=0; // rechte Bande
    while ( n < ballnum)
 {
	       
       xbuf=0, ybuf=0;
  

  power=
   sqrt ( 
    ( ball[n].x - 80 )  *
    ( ball[n].x - 80 )+
      0 );
  
    distance=power;

   y_buf=0;
   x_buf=(ball[n].x-80);
 
 
   if ( power-SCHEITEL_C < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL_C;
  



    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;


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

   
    ball[n].xvec+=x_buf, ball[n].yvec+=y_buf;
    n++;
  }

  n=0;
  
  while ( n < ballnum)
  { 
   ball[n].x+=ball[n].xvec/TIMESLICE; /* Ball bewegen */
   ball[n].y+=ball[n].yvec/TIMESLICE;

   ball[n].xvec=(ball[n].xv_old+ball[n].xvec)/2;
   ball[n].yvec=(ball[n].yv_old+ball[n].yvec)/2;
 
   ball[n].xv_old=ball[n].xvec;
   ball[n].yv_old=ball[n].yvec;
   n++;
  }
  


  n=0;
  
  while ( n < ballnum)
  { 
   ball[n].x+=ball[n].xvec/TIMESLICE; /* Ball bewegen */
   ball[n].y+=ball[n].yvec/TIMESLICE;

   ball[n].xvec=(ball[n].xv_old+ball[n].xvec)/2;
   ball[n].yvec=(ball[n].yv_old+ball[n].yvec)/2;
 
   ball[n].xv_old=ball[n].xvec;
   ball[n].yv_old=ball[n].yvec;
   n++;
  }
  


  n=0;
  
  while ( n < ballnum)
  { 
   ball[n].x+=ball[n].xvec/TIMESLICE; /* Ball bewegen */
   ball[n].y+=ball[n].yvec/TIMESLICE;

   ball[n].xvec=(ball[n].xv_old+ball[n].xvec)/2;
   ball[n].yvec=(ball[n].yv_old+ball[n].yvec)/2;
 
   ball[n].xv_old=ball[n].xvec;
   ball[n].yv_old=ball[n].yvec;
   n++;
  }
  
  
  
  
  
  
  // Eingaben entgegennehmen
 
  if ( kbhit() )
  {
   switch( getch() )
   {
    case 'x' : if ( player[0].pos > 2 ) player[0].pos--; break;
    case 'v' : if ( player[0].pos < 78 ) player[0].pos++; break;
    case 'c' : if ( player[0].wait == 0 ) player[0].shoot=1, player[0].wait=5; break;
    default : break;
   }

  }


   if ( player[0].shoot==1) split();

  y=0;
  while(y < 25 )
  {
   x=0;
   while ( x < 80 )
   {
    screen[x][y]=' ';
   x++;
   }
   y++;
  }
  
  screen[(int)player[0].pos][14]='O';
   screen[(int)player[0].pos-1][15]='/';
   screen[(int)player[0].pos+1][15]='\\';
   if ( (int)player[0].pos%2==0)screen[(int)player[0].pos][16]='M';
   else screen[(int)player[0].pos][16]='X';
   
   player[0].shoot=0;
   if ( player[0].wait > 0)player[0].wait--;
   
   if ( ballnum==0)printf("\adudededede!"),getch();
   n=0;
   while (n < ballnum)
   {
   	
   	if ( fabs(ball[n].x-player[0].pos) < 2 &&
	     fabs(ball[n].y-15) < 2 )
	     printf("Game Over Man!"), getch();
	     
	n++;
   }
   
   
  n=0;
  while( n < ballnum)
  {
  
    if ( ball[n].type==BIG)
     if ( ball[n].x < 77 && ball[n].x > 3 && ball[n].y > 2 && ball[n].y < 23) 
    {
     screen[(int)ball[n].x-1][(int)ball[n].y-1]=',';screen[(int)ball[n].x][(int)ball[n].y-1]='-';screen[(int)ball[n].x+1][(int)ball[n].y-1]='\\';
     screen[(int)ball[n].x-1][(int)ball[n].y]='|';screen[(int)ball[n].x][(int)ball[n].y]=' ';screen[(int)ball[n].x+1][(int)ball[n].y]='|';
     screen[(int)ball[n].x-1][(int)ball[n].y+1]='`';screen[(int)ball[n].x][(int)ball[n].y+1]='-';screen[(int)ball[n].x+1][(int)ball[n].y+1]='´';
	}
 
   if ( ball[n].type==SMALL)
   if ( ball[n].x < 80 && ball[n].x > 0 && ball[n].y < 25 && ball[n].y > 0 )
   screen[(int)ball[n].x][(int)ball[n].y]='O';
  
  
   n++;
  }

 if ( runs++%1==0)
 {
 
  system("cls\n");
  y=0;
   while(y < 17 )
   {
   	putch('#');
    x=1;
    while ( x < 77 )
    {
     putch(screen[x][y]);
    x++;
    }
    printf("#\n");
    y++;
   }
  printf("###############################################################################");

  n=0; while ( n < 5000000)n++;
} 
/* 
n=0;
while ( n < ballnum)
{
	if ( ball[n].type==BIG)printf("%f ", ball[n].xvec);
   n++;
}
getch();

*/
 }

}
