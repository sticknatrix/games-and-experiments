/* E und D zum Bewegen des Schlaegers, rechts ist der Computer, kann man auch bewegen.
  */

#include <stdio.h>
#include <math.h>
#include <time.h>

#define PLAYER -1
#define COM 1
#define OFF 2



 
   #define SCHEITEL 2.5
   #define LADUNG 20
   #define MAXKRAFT 1
   #define LADNULL 100
   #define TRAEGSCHWELLE 0.1
   #define TIMESLICE 10.0
   
   #define MAXDEPTH 5 // war 5
   float MOV;
   #define TIMESLICEB (TIMESLICE/1 /*MOV */)
   #define SCALE 4.0
    


struct
 {
  float x,y;
  float xvec, yvec;
  float touch;
 } ball;      /* der Ball */
 
 int xbuf,ybuf;
 int reinit=PLAYER;
 
 
 struct
 {
  float height;
  int score;
 
 }player[2];  /* Spieler und Computer */


 float heightcmp;
 float distance, power;
 int x_buf, y_buf;
 int x,y;
 int sign;


void simmove(void)
{
	
	
  for(heightcmp=player[0].height-3; heightcmp <= player[0].height+3; heightcmp+=3 )
  {
  xbuf=0, ybuf=0;
  

  power=
   sqrt ( 
    ( ball.x - 20 )  *
    ( ball.x - 20 )+
       ( ball.y - heightcmp) *
    ( ball.y - heightcmp) );
  
    distance=power;

   x_buf=(ball.x - 20);
   y_buf=(ball.y - heightcmp);
 
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;

    if ( fabs(power) < 0.001 ) power=sign*0.001;    

    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;

   if ( fabs ( power) >= LADNULL ) power=0;

          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
      ball.touch=PLAYER;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;
 
  }



   for(heightcmp=player[1].height-3; heightcmp <= player[1].height+3; heightcmp+=3 )
  {
  xbuf=0, ybuf=0;

  power=
   sqrt ( 
    ( ball.x - 60 )  *
    ( ball.x - 60 )+
       ( ball.y - heightcmp) *
    ( ball.y - heightcmp) );
  
    distance=power;

   x_buf=(ball.x - 60);
   y_buf=(ball.y - heightcmp);
 
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;

    if ( fabs(power) < 0.001 ) power=sign*0.001;    

    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;

   if ( fabs ( power) >= LADNULL ) power=0;

          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
      ball.touch=COM;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;

    /*  der Ball besteht aus einem beweglichen Atom und die Schlaeger aus jeweils drei festen,
      die sich in Y-Richtung gemeinsam verschieben lassen */
 
  }



  xbuf=0, ybuf=0;

  power=
   sqrt ( 
    ( 0 )  *
    ( 0 )+
       ( ball.y +3) *
    ( ball.y + 3) );
  
    distance=power;

   x_buf=(0 );
   y_buf=(ball.y +3);
 
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;

    if ( fabs(power) < 0.001 ) power=sign*0.001;    

    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;

   if ( fabs ( power) >= LADNULL ) power=0;

          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
      ball.touch=COM;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;

/* Boden und Decke */
 


  power=
   sqrt ( 
    ( 0 )  *
    ( 0 )+
       ( 22-ball.y) *
    ( 22-ball.y ) );
  
    distance=power;

   x_buf=(0);
   y_buf=(ball.y-22);
 
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;

    if ( fabs(power) < 0.001 ) power=sign*0.001;    

    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;

   if ( fabs ( power) >= LADNULL ) power=0;

          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
      ball.touch=COM;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;

 





  ball.x+=ball.xvec/TIMESLICEB; /* Ball bewegen */
  ball.y+=ball.yvec/TIMESLICEB;

 
}

/* Vorausschauende KI-Simulation */

signed int ki_lookout ( float x, float y, float xvec, float yvec, int play_height, int com_height, int depth, int touch )
{
  signed int buf, comwins, plwins, even,
             pl1, pl2, pl3, comsure;
  if ( depth > MAXDEPTH ) return 0;


    if ( play_height > 15 )play_height=15;
    if ( play_height < 3 ) play_height=3;
    if ( com_height < 3 ) com_height=3;
    if ( com_height > 15 ) com_height=15 ;


 if ( x/1 >= 60/1 ) return PLAYER;
    else
    if ( x/1 <= 20/1 ) return COM;   /* Ball am Gegner durch? Anderer Gegner gewinnt..*/

   int steps;
   steps=MOV;

   ball.x=x, ball.y=y, ball.xvec=xvec, ball.yvec=yvec, player[0].height=play_height, player[1].height=com_height, ball.touch=touch;
   while(steps > 0)simmove(), steps--;
   
   touch=ball.touch, y=ball.y, x=ball.x, xvec=ball.xvec, yvec=ball.yvec, play_height=player[0].height, com_height=player[1].height;
  

  buf=0; comwins=0; plwins=0; even=0; comsure=0;
  pl1=0, pl2=0, pl3=0;

   plwins=0; comwins=0;
   buf=ki_lookout( x+xvec, y+yvec, xvec,yvec, play_height, com_height, depth+1, touch );
   if ( buf==PLAYER ) plwins++;
   if ( buf==COM ) comwins++;
   buf=ki_lookout( x+xvec, y+yvec, xvec,yvec,  play_height-MOV, com_height, depth+1, touch );
   if ( buf==PLAYER ) plwins++;
   if ( buf == COM ) comwins++;
   buf=ki_lookout( x+xvec, y+yvec, xvec,yvec,  play_height+MOV, com_height, depth+1, touch );
   if ( buf==PLAYER ) plwins++;
   if ( buf == COM ) comwins++;
   if ( plwins >=1 ) pl1=1;
   if ( comwins==3 ) comsure=1;

   plwins=0; comwins=0;
   buf=ki_lookout( x+xvec, y+yvec, xvec,yvec,  play_height-MOV, com_height+MOV, depth+1, touch );
   if ( buf==PLAYER ) plwins++;
   if ( buf==COM ) comwins++;
   buf=ki_lookout( x+xvec, y+yvec, xvec,yvec,  play_height, com_height+MOV, depth+1, touch );
   if ( buf==PLAYER ) plwins++;
   if ( buf==COM ) comwins++;
   buf=ki_lookout( x+xvec, y+yvec, xvec,yvec,  play_height+MOV, com_height+MOV, depth+1, touch );
   if ( buf==PLAYER ) plwins++;
   if ( buf==COM ) comwins++;
   if ( plwins>=1) pl2=1;
   if ( comwins==3 ) comsure=1;

   plwins=0; comwins=0;
   buf=ki_lookout( x+xvec, y+yvec, xvec,yvec, play_height, com_height-MOV, depth+1, touch );
   if ( buf==PLAYER ) plwins++;
   if ( buf==COM ) comwins++;
   buf=ki_lookout( x+xvec, y+yvec, xvec,yvec, play_height+MOV, com_height-MOV, depth+1, touch );
   if ( buf==PLAYER ) plwins++;
   if ( buf==COM ) comwins++;
   buf=ki_lookout( x+xvec, y+yvec, xvec,yvec, play_height-MOV, com_height-MOV, depth+1, touch );
   if ( buf==PLAYER ) plwins++;
   if ( buf==COM ) comwins++;
   if ( plwins>= 1 ) pl3=1;
   if ( comwins==3 ) comsure=1;

   /* rekursiver Selbstaufruf zur Simulation der Folgeschritte */

  if ( pl1 == 1 && pl2==1 && pl3== 1 )
  {
   /*  printf("Spieler siegt bei %f %f %d\n", x,y, depth ); getch(); */ /* debug muﬂ zuerst kommen */
    return PLAYER;
  }
  else if (comsure==1 ) return COM;
  return 0;
}


int main(void)
{
 struct
 {
  float x,y;
  float xvec, yvec;
  float touch;
 } ball;      /* der Ball */
 
 int xbuf,ybuf;
 int reinit=PLAYER;
  clock_t delay; 
 
 struct
 {
  float height;
  int score;
 
 }player[2];  /* Spieler und Computer */


 float heightcmp;
 float distance, power;
 int x_buf, y_buf;
 int x,y;
 int sign;

 char screen[80][25];
 
 player[0].score=0, player[1].score=0;

 while ( 1 )
 {
  ball.x=40, ball.y=8+rand()%8;
  if ( reinit==PLAYER ) ball.xvec=5, ball.yvec=0, ball.touch=PLAYER;
   else ball.xvec=-5, ball.yvec=0, ball.touch=COM;
  player[0].height=8, player[1].height=8;
 
 
 while(1)
 {
	delay=clock()+50;
 

  for(heightcmp=player[0].height-3; heightcmp <= player[0].height+3; heightcmp+=3 )
  {
  xbuf=0, ybuf=0;
  

  power=
   sqrt ( 
    ( ball.x - 20 )  *
    ( ball.x - 20 )+
       ( ball.y - heightcmp) *
    ( ball.y - heightcmp) );
  
    distance=power;

   x_buf=(ball.x - 20);
   y_buf=(ball.y - heightcmp);
 
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;

    if ( fabs(power) < 0.001 ) power=sign*0.001;    

    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;

   if ( fabs ( power) >= LADNULL ) power=0;

          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
      ball.touch=PLAYER;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;
 
  }



   for(heightcmp=player[1].height-3; heightcmp <= player[1].height+3; heightcmp+=3 )
  {
  xbuf=0, ybuf=0;

  power=
   sqrt ( 
    ( ball.x - 60 )  *
    ( ball.x - 60 )+
       ( ball.y - heightcmp) *
    ( ball.y - heightcmp) );
  
    distance=power;

   x_buf=(ball.x - 60);
   y_buf=(ball.y - heightcmp);
 
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;

    if ( fabs(power) < 0.001 ) power=sign*0.001;    

    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;

   if ( fabs ( power) >= LADNULL ) power=0;

          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
      ball.touch=COM;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;

    /*  der Ball besteht aus einem beweglichen Atom und die Schlaeger aus jeweils drei festen,
      die sich in Y-Richtung gemeinsam verschieben lassen */
 
  }



  xbuf=0, ybuf=0;

  power=
   sqrt ( 
    ( 0 )  *
    ( 0 )+
       ( ball.y +3) *
    ( ball.y + 3) );
  
    distance=power;

   x_buf=(0 );
   y_buf=(ball.y +3);
 
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;

    if ( fabs(power) < 0.001 ) power=sign*0.001;    

    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;

   if ( fabs ( power) >= LADNULL ) power=0;

          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
      ball.touch=COM;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;

/* Boden und Decke */
 


  power=
   sqrt ( 
    ( 0 )  *
    ( 0 )+
       ( 22-ball.y) *
    ( 22-ball.y ) );
  
    distance=power;

   x_buf=(0);
   y_buf=(ball.y-22);
 
  if ( power-SCHEITEL < 0.0 ) sign= -1.0; else sign= 1.0;
   power=power-SCHEITEL;

    if ( fabs(power) < 0.001 ) power=sign*0.001;    

    power= ( LADUNG / (( power * power)+MAXKRAFT) ) *sign  ;

   if ( fabs ( power) >= LADNULL ) power=0;

          if ( distance != 0 )
          {
          y_buf*=power/distance;
          x_buf*=power/distance;

        } else x_buf=0, y_buf=0;
        
        
        
        if ( (distance=sqrt ( x_buf*x_buf+y_buf*y_buf )) > TRAEGSCHWELLE )
   {
      power= (distance-TRAEGSCHWELLE)/ distance;
      x_buf*=power; y_buf*=power;
      ball.touch=COM;
    }  else x_buf=0, y_buf=0 ;

   
    ball.xvec+=x_buf, ball.yvec+=y_buf;

 





  ball.x+=ball.xvec/TIMESLICE; /* Ball bewegen */
  ball.y+=ball.yvec/TIMESLICE;

 
  if ( kbhit() )
  {
   switch( getch() )
   {
    case 'e' : if ( player[0].height > 3 ) player[0].height--; break;
    case 'd' : if ( player[0].height < 15 ) player[0].height++; break;
    case 'u' : if ( player[1].height > 3 ) player[1].height--; break;
    case 'j' : if ( player[1].height < 15 ) player[1].height++; break;
    default : break;
   } /* Spielereingaben entgegennehmen */

  }

  MOV=1;
  float heightadd=0;
  heightadd=0;
  
  while (MOV < 32)
  {
   y_buf=0;

   if ( x_buf=ki_lookout(ball.x, ball.y, ball.xvec*1.0, ball.yvec*1.0, player[0].height, player[1].height, 1, ball.touch )!=PLAYER )
   { if ( player[1].height > 3 ) y_buf=0; }

    if ( x_buf== COM ) goto skip;
   if ( x_buf=ki_lookout(ball.x, ball.y, ball.xvec*1.0, ball.yvec*1.0, player[0].height, player[1].height-MOV, 1, ball.touch )!=PLAYER )
   { if ( player[1].height > 3 ) y_buf=-1; }

    if ( x_buf==COM ) goto skip;
   if ( x_buf=ki_lookout(ball.x, ball.y, ball.xvec*1, ball.yvec*1, player[0].height, player[1].height+MOV, 1, ball.touch )!=PLAYER )
   { if ( player[1].height < 15 ) y_buf=1; }

   skip:
   
    // hier haette es zu heightadd addiert
   heightadd+=y_buf*(32/MOV);
   
   MOV*=2;
   
   }
 
   if ( heightadd > 0)
  player[1].height++;
    else if ( heightadd < 0)
    player[1].height--;
    
   /* Computerentscheidung */


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

  y=0;
  while ( y < 6 )
  {
   screen[20][(int)player[0].height-3 + y ]='|';
   y++;
  }

  y=0;
  while ( y < 6 )
  {
   screen[60][(int)player[1].height-3 + y ]='|';
   y++;
  }


  
 if ( ball.x < 80 && ball.x > 0 && ball.y < 25 && ball.y > 0 )
 screen[(int)ball.x][(int)ball.y]='O';

 system("cls\n");
 y=0;
  while(y < 18 )
  {
   x=0;
   while ( x < 77 )
   {
    putch(screen[x][y]);
   x++;
   }
   printf("\n");
   y++;
  }

  printf("-------------------------------------------------------------------------------");
  printf("\nSpieler 1 : %d      Spieler 2 : %d", player[0].score, player[1].score );

  /* Bildschirm im Textmodus darstellen */

  if ( ball.x < 0 || ball.x > 80 )
  {
   if ( ball.x > 60 ) player[0].score++, reinit=PLAYER;
   else if ( ball.x < 20 ) player[1].score++, reinit=COM;
   
   break;
  }

  while ( delay > clock());
 }

 }


}
