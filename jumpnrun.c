unsigned char level[600]=
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1 };

unsigned char block[25]={219,219,'@',219,219,
                        219,219,'@',219,219,
                        219,'@','@','@',219,
                        219,219,'@',219,219,
                        219,219,'@',219,219 }; 


struct
{
  int x, y;
  int jump;
	
} maennle;

int main(void)
{
  int c;
  int scroll_x;
    int block_n;
  unsigned char screen[160][120];
  long int runs=0;

 scroll_x=0;
 
 maennle.x=30, maennle.y=40;
 maennle.jump=0;

 while ( 1 )
 {
 	 maennle.y++;
 
  if ( kbhit() )
  {
   c=getch();
   if ( c=='l' )
   {
   	 if ( (maennle.x+1)/5  < 200-160)
    if (level[(maennle.x+1)/5+(maennle.y/5)*40]==0) maennle.x++; 
   }
   else if ( c=='k' )
   {
    if ( (maennle.x-1)/5 > 0) if (level[(maennle.x-1)/5+(maennle.y/5)*40]==0) maennle.x--; 
   }
   else if ( c=='o' && maennle.jump==0 )
   {
     if ( ((maennle.y+1)/5) < 15) if (level[maennle.x/5+((maennle.y+1)/5)*40]==1) maennle.jump=12;
   	}
	
    
  }
  int x,y;
  
  scroll_x=maennle.x-40 > 0 ? maennle.x-40 : 0;
  if ( scroll_x > 200-160) scroll_x=200-160;
  
  
  if ( maennle.jump > 0 &&level[maennle.x/5+((maennle.y-1)/5)*40]==0) maennle.y-=2, maennle.jump--;
  else if ( maennle.jump > 0) maennle.jump--;
  
  if (level[maennle.x/5+((maennle.y+1)/5)*40]==1 ) maennle.y--;
  
  
  if ( maennle.y/5==15) printf("\a");
  
  x=0, y=0;
  while ( y < 90 )
  {
   x=0;
   while ( x < 160 )
   {
    screen[x][y]=' ';
    x++;
   }
   y++;
  }
 
  
 int block_n_start;
  block_n_start=scroll_x/5;
  int offset;
  int y_map;
  offset=scroll_x%5;
  
  block_n=1;  
  while ( block_n < 32 )
  {
   y_map=0;
   while ( y_map < 15 )
   {
    if ( level[block_n_start+block_n+y_map*40]==1 )
    {
     y=0;
     
     while ( y < 5 )
     {
      x=0;
      while ( x < 5 )
      {
       screen[block_n*5+x-offset][y_map*5+y]=block[x+y*5];
       x++;
      }
      y++;

     }


     }
     y_map++;
    }

    block_n++;
   }  

 screen[maennle.x-scroll_x][maennle.y-1]='o';
   screen[maennle.x-scroll_x][maennle.y]='O';
   screen[maennle.x-1-scroll_x][maennle.y]='/';
   screen[maennle.x+1-scroll_x][maennle.y]='\\';
   screen[maennle.x-scroll_x][maennle.y+1]='M';
   screen[maennle.x-scroll_x][maennle.y+2]='|';
  screen[maennle.x+1-scroll_x][maennle.y+2]='|';


  if ( runs%3==0)
  {
   system("cls\n");   

  x=0, y=0;
  while ( y < 90 )
  {
   x=0;
   while ( x < 159 )
   {
   	if ( x < 4 || x > 155) printf("||||"); else
    printf("%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y]);
    x+=4;
   }
   
   printf("\n");
   y++;
  }
  



 }

 runs++;
 }




}
