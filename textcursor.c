#include <stdio.h>
#include <time.h>


unsigned char font[780]={

                         0,0,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,177,177,177,177,
                         0,177,0,0,0,177,
                         0,177,0,0,0,177,


                         0,177,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,177,177,177,0,

                         0,0,177,177,177,177,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,
                         0,0,177,177,177,177,

                         0,177,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,0,0,0,177,
                         0,177,0,0,0,177,
                         177,177,177,177,177,0,


                         177,177,177,177,177,177,
                         0,177,0,0,0,0,
                         0,177,177,177,177,0,
                         0,177,0,0,0,0,
                         177,177,177,177,177,177,

                         0,177,177,177,177,0,
                         0,177,0,0,0,0,
                         0,177,177,177,177,0,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,


                         0,0,177,177,177,0,
                         177,177,0,0,0,0,
                         177,0,0,177,177,177,
                         177,0,0,0,177,0,
                         0,177,177,177,177,0,

                         0,177,0,0,177,0,
                         0,177,0,0,177,0,
                         0,177,177,177,177,0,
                         0,177,0,0,177,0,
                         0,177,0,0,177,0,

                         0,0,177,0,0,0,
                         0,0,177,0,0,0,
                         0,0,177,0,0,0,
                         0,0,177,0,0,0,
                         0,0,177,0,0,0,


                         0,177,177,177,177,0,
                         0,0,0,0,0,177,
                         0,177,177,177,0,177,
                         0,0,177,0,0,177,
                         0,0,177,177,177,0,

                         0,177,0,0,177,0,
                         0,177,0,177,0,0,
                         0,177,177,0,0,0,
                         0,177,0,177,0,0,
                         0,177,0,0,177,0,


                         0,177,0,0,0,0,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,
                         0,177,177,177,177,0,

                         0,177,177,0,177,177,
                         177,0,0,177,0,177,
                         177,0,0,177,0,177,
                         177,0,0,0,0,177,
                         177,0,0,0,0,177,

                         177,177,0,0,0,177,
                         177,0,177,0,0,177,
                         177,0,177,0,0,177,
                         177,0,0,177,0,177,
                         177,0,0,0,177,177,


                         0,177,177,177,177,0,
                         177,0,0,0,0,177,
                         177,0,0,0,0,177,
                         177,0,0,0,0,177,
                         0,177,177,177,177,0,

                         0,177,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,177,177,177,0,
                         0,177,0,0,0,0,
                         0,177,0,0,0,0,


                         0,177,177,177,177,0,
                         177,0,0,0,0,177,
                         177,0,0,177,0,177,
                         177,0,0,0,177,0,
                         0,177,177,177,0,177,

                         0,177,177,177,177,0,
                         0,177,0,0,0,177,
                         0,177,177,177,177,0,
                         0,177,0,177,0,0,
                         0,177,0,0,177,177,

                         0,0,177,177,177,177,
                         0,177,0,0,0,0,
                         0,0,177,177,177,0,
                         0,0,0,0,0,177,
                         0,177,177,177,177,0,


                         177,177,177,177,177,177,
                         0,0,0,177,0,0,
                         0,0,0,177,0,0,
                         0,0,0,177,0,0,
                         0,0,0,177,0,0,

                         0,177,0,0,0,177,
                         0,177,0,0,0,177,
                         0,177,0,0,0,177,
                         0,177,0,0,0,177,
                         0,0,177,177,177,0,


                         0,177,0,0,0,177,
                         0,177,0,0,177,177,
                         0,177,0,0,177,0,
                         0,0,177,0,177,0,
                         0,0,0,177,0,0,

                         177,0,0,0,0,177,
                         177,0,0,0,0,177,
                         177,0,177,177,0,177,
                         177,0,177,177,0,177,
                         0,177,0,0,177,177,

                         177,0,0,0,177,0,
                         0,177,0,177,0,0,
                         0,0,177,0,0,0,
                         0,177,0,177,0,0,
                         177,0,0,0,177,0,


                         0,177,0,0,0,177,
                         0,0,177,0,0,177,
                         0,0,0,177,177,0,
                         0,0,0,177,0,0,
                         0,177,177,0,0,0,

                         0,177,177,177,177,177,
                         0,0,0,0,0,177,
                         0,0,0,177,177,0,
                         0,0,177,0,0,0,
                         0,177,177,177,177,177  };


unsigned char screen[160][120];

struct
{
 int x_scrl, y_scrl;
 unsigned char data[1750]; 
 unsigned cursorpos;
}textbox;

struct
{
 int x, y;
  int c;
 int clicksig;
 int typesig;
}input;


void display_text(void)
{
 int x=0, y=0;
 int xf,yf; 
 int n=0;

#define XSTART 15
#define YSTART 5
#define XEND 60
#define YEND 55

#define XBOXSIZE 30
#define YBOXSIZE 90


x=XSTART;
while ( x < XEND) screen[x][YSTART]=219,
                  screen[x][YEND]=219,
                    x++;

y=YSTART;
while ( y < YEND+1) screen[XSTART][y]=219,
                  screen[XEND][y]=219,
                    y++;


y=1;
x=1;

 while ( textbox.data[n]!='\0')
 {
 	if (textbox.data[n]==0xa|| textbox.data[n]==0xd ) { n++, y+=6, x=1; continue; }
	 if (x+6 > XBOXSIZE) { y+=6,x=1; }
    if ( textbox.data[n]==' ') { x+=6; n++; continue; }
    
  yf=0;
 
  while ( yf < 5)
  {
   xf=0;
   while (xf < 6)
   {
   	
    if ( 
	     -textbox.x_scrl+XSTART+x+xf >= XSTART &&
         -textbox.y_scrl+yf+y+YSTART >= YSTART &&
	
	     -textbox.x_scrl+x+xf < XEND-XSTART &&
         -textbox.y_scrl+yf+y< YEND-YSTART
		 
		 && y+yf-textbox.y_scrl < YBOXSIZE )
    screen[XSTART-textbox.x_scrl+x+xf][-textbox.y_scrl+yf+y+YSTART]=font[(textbox.data[n]-0x61)*30+xf+yf*6];
	
	xf++;
   }
   if ( n==textbox.cursorpos)
   screen[XSTART-textbox.x_scrl+x+0][-textbox.y_scrl+yf+y+YSTART]=219;
	
   yf++;
  }

  n++;
  x+=6;
 }
 
}


int get_textposition(int goalx, int goaly)
{
 
 int x=0, y=0;
 int xf,yf; 
 int n=0;

#define XSTART 15
#define YSTART 5
#define XEND 60
#define YEND 55

#define XBOXSIZE 30
#define YBOXSIZE 90

y=1;
x=1;

 while ( textbox.data[n]!='\0')
 {
 	if (textbox.data[n]==0xa|| textbox.data[n]==0xd ) { n++, y+=6, x=1; continue; }
	 if (x+6 > XBOXSIZE) { y+=6,x=1; }
    //if ( textbox.data[n]==' ') { x+=6; n++; continue; }
    
  yf=0;
 
  while ( yf < 5)
  {
   xf=0;
   while (xf < 6)
   {
   	
    if ( 
	     -textbox.x_scrl+XSTART+x+xf >= XSTART &&
         -textbox.y_scrl+yf+y+YSTART >= YSTART &&
	
	     -textbox.x_scrl+x+xf < XEND-XSTART &&
         -textbox.y_scrl+yf+y< YEND-YSTART
		 
		 && y+yf-textbox.y_scrl < YBOXSIZE )
    if ( goalx==XSTART-textbox.x_scrl+x+xf && goaly==-textbox.y_scrl+yf+y+YSTART) return n;
    
	xf++;
   }
   yf++;
  }

  n++;
  x+=6;
 }
 

 return -1;

}

void place_cursor(int pos)
{
 textbox.cursorpos=pos;

}



void mausklick(void)
{
	int c;
	if ( kbhit())
	{
	 c=getch();
	 
	 if ( c=='8' && input.y > 0 ) input.y--;
	 if ( c=='2' && input.y < 89 ) input.y++;
	 if ( c=='4' && input.x > 0 ) input.x--;
	 if ( c=='6' && input.x < 159 ) input.x++;
	
	 if ( c=='5' && input.x < 159 ) input.clicksig=1;
	 
	 if ( c >='a'&& c<='z') input.c=c, input.typesig=1;
	 
	 if ( c =='0') input.c=c, input.typesig=1;
	 
	 if ( c==' ') input.c=c, input.typesig=1;
	 
		
	}
	screen[input.x][input.y]='+';

}

void insert(unsigned int pos, unsigned int c)
{
	unsigned char buf[1750];
	
	if ( c=='0')
	{
	 // if ( textbox.cursorpos > 0)textbox.cursorpos--;
     strcpy(buf,textbox.data+pos+1);
     strcpy(textbox.data+pos,buf);
	}
	else
	{
	strcpy(buf, textbox.data+pos);
	if ( (c >= 'a' && c <= 'z')||c=='0xa'|| c==0xd|| c==' ')
     textbox.data[pos]=c;	
	strcpy(textbox.data+pos+1,buf);
	textbox.cursorpos++;
   }
}



int main(void)
{
 clock_t delay;
 int x=0,y=0;
 int runs=0;

  int posbuf=0;
 

  input.clicksig=0, input.typesig=0, input.x=0, input.y=0;
  
  textbox.x_scrl=0, textbox.y_scrl=0;
  textbox.cursorpos=3;
  strcpy(textbox.data,"quick brown fox jumps over the lazy dog");


while (1 )
{
   delay=clock()+100;
	y=0;
  while ( y < 90)
  {
   x=0;
   while ( x < 160)
   {
   	screen[x][y]=' ';
   	x++;
   }
  	
  	printf("\n");
  	y++;
  }

  display_text();
  mausklick();
 
   if ( input.clicksig==1)
   {
    input.clicksig=0;
	posbuf=get_textposition(input.x, input.y );
	
	if ( posbuf!=-1) place_cursor(posbuf);
   }
   
   if ( input.typesig==1)
   {
   	input.typesig=0;
   	insert(textbox.cursorpos,input.c);
   }
   
   system("cls\n");
   if ( runs%1==0)
 {
 y=0;
  while ( y < 70)
  {
   x=0;
   while ( x < 160)
   {
   	 /*if ( screen[x][y]!=0 && screen[x][y]!=' ')*/printf("%c%c%c%c",screen[x][y] ,screen[x+1][y],screen[x+2][y],screen[x+3][y] );
   	 /*else printf(" ") */;
   	  x+=4;
  
   }
  	
  	printf("\n");
  	y++;
  }

 }
  runs++;
  while ( delay > clock());
 }

}
