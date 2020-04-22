#include <stdio.h>
#include <strings.h>
#include <fcntl.h>

#define NUM_WORDS 12000

struct
{
 unsigned char word[20];
 unsigned int len;
 unsigned int taken;

} words[NUM_WORDS];

int main(void)
{
 unsigned int n;
 struct
 {
  unsigned char letter;
  unsigned int x,y;
  unsigned int xprev, yprev;
  }snake[13];
 
 struct
 {
  unsigned char letter;
  unsigned int x,y;
  }letterobj[6];
   
  
  int snakelen=0;
   FILE *input;
   int wc=0;
   input=fopen(".\\words.txt","rb");
    unsigned char cpstring[255];
     int wordnum;
   while ( wc < NUM_WORDS )
  {
   fscanf(input,"%s\n",cpstring);
   strcpy(words[wc].word,cpstring);
   words[wc].len=strlen(cpstring);
   words[wc].taken=0;
   wc++;
  }
   wordnum=0;
   
   fclose(input);
   

   #define LEFT  0
   #define RIGHT 1
   #define UP    2
   #define DOWN  3



 unsigned char direct=RIGHT;
 unsigned int eaten=0;  
  
    snake[snakelen].letter=rand()%26+0x41;
     snake[snakelen].x=rand()%80;
     snake[snakelen].y=rand()%50;
   letterobj[0].letter=' ';
   letterobj[1].letter=' ';
   letterobj[2].letter=' ';
   letterobj[3].letter=' ';
   letterobj[4].letter=' ';
   letterobj[5].letter=' ';


  while (1 )
  {

    int x,y;
    x=0,y=0;
    
    unsigned char screen[90][60];
    y=0;
    while ( y < 50 )
    {
     x=0;
     while ( x < 80 )
     {
      screen[x][y]=' ';
      x++;
     }
     y++;
     }   
   snake[snakelen+1].letter='\0';
   
   
   if ( kbhit() )
   {
   	switch (getch() )
    {
     case 'h' : if ( snakelen > 0 ){
		        if ( snake[snakelen-1].x!= snake[snakelen].x-1  )
	            
				 direct=LEFT;
                } else direct=LEFT;
				break;
     case 'k' : if ( snakelen > 0 ){
	             if ( snake[snakelen-1].x!= snake[snakelen].x+1  ) 
				 direct=RIGHT;
	             } else direct=RIGHT;
                 break;
     case 'u' : if ( snakelen > 0 ){
				if ( snake[snakelen-1].y!= snake[snakelen].y-1 )                                         
				 direct=UP;
	             } else direct=UP;
                 break;
     case 'j' : if ( snakelen > 0 ){
	            if ( snake[snakelen-1].y!= snake[snakelen].y+1  )
                
				 direct=DOWN;
                } else direct=DOWN;
                break;
       }

     }
	 
	  

     screen[snake[snakelen].x][snake[snakelen].y]=snake[snakelen].letter;        
    
     n=snakelen+1;
     
     long int n2;
     n2=0;
     int n3;
     n3=0;    
     int n4=0;
     
     if ( letterobj[0].letter==' '&& letterobj[1].letter==' '&& letterobj[2].letter==' ' && letterobj[3].letter==' ' 
	     && letterobj[4].letter==' '&& letterobj[5].letter==' ')
     {
     	
      while ( n2 < 6 )
      {
       letterobj[n2].letter=rand()%26+0x41;
       if ( rand()%2==0)
	   while ( letterobj[n2].letter!='A'&&letterobj[n2].letter!='E'&&letterobj[n2].letter!='I'&&letterobj[n2].letter!='O'&&letterobj[n2].letter!='U')
       letterobj[n2].letter=rand()%26+0x41;
      
	  
	   letterobj[n2].x=rand()%80, letterobj[n2].y=rand()%50;
         
         n3=n;
       
            n4=0 ;
           while ( n3 > 0 && n4 < n2-1)
        {
         
         if ( letterobj[n2].x==letterobj[n4].x && letterobj[n2].y== letterobj[n4].y )
          {
            letterobj[n2].x=rand()%80, letterobj[n2].y=rand()%50, n4=0,n3=n; continue;
           }
          n4++;
          n3--;
         if( letterobj[n2].x==snake[n3].x && letterobj[n2].y==snake[n3].y )
           {
            letterobj[n2].x=rand()%80, letterobj[n2].y=rand()%50; n4=0; n3=n;continue;
           }
         
         }
         
		 n2++;
        }
       
     }
     
     
       if ( snake[snakelen].x > 79 || snake[snakelen].x <= 0 || snake[snakelen].y > 49 || snake             [snakelen].y <= 0 ) goto gameover;
    
	      if ( direct==LEFT )
     {
      snake[snakelen].xprev=snake[snakelen].x, snake[snakelen].yprev=snake[snakelen].y;
	  snake[snakelen].x--;
     }
     if ( direct==RIGHT )
     {
      snake[snakelen].xprev=snake[snakelen].x, snake[snakelen].yprev=snake[snakelen].y;
      snake[snakelen].x++;
        
     }

     if ( direct==UP )
     {
      snake[snakelen].xprev=snake[snakelen].x, snake[snakelen].yprev=snake[snakelen].y;
	  snake[snakelen].y--;
        
     }

     if ( direct==DOWN )
     {
      snake[snakelen].xprev=snake[snakelen].x, snake[snakelen].yprev=snake[snakelen].y;	
      snake[snakelen].y++;
        
     }
  


    n=snakelen;
     while ( n > 0 )
     {
       if ( 
	   (direct==LEFT&&
	      snake[n].x==snake[snakelen].x-1 && snake[n].y==snake[snakelen].y)||
          (direct==UP&&
	      snake[n].x==snake[snakelen].x && snake[n].y==snake[snakelen].y-1)||
          (direct==DOWN&&
	      snake[n].x==snake[snakelen].x+1 && snake[n].y==snake[snakelen].y+1)||
	      (direct==RIGHT&&
	      snake[n].x==snake[snakelen].x+1 && snake[n].y==snake[snakelen].y)||
            snake[snakelen].x > 79 || snake[snakelen].x <= 0 || snake[snakelen].y > 49 || snake             [snakelen].y <= 0 ) goto gameover;
     
	  
	  snake[n-1].xprev=snake[n-1].x, snake[n-1].yprev=snake[n-1].y;
      snake[n-1].x=snake[n].xprev;
      snake[n-1].y=snake[n].yprev;
     
	  screen[snake[n].x][snake[n].y]=snake[n].letter;        
     
       printf("%d %d\n",snake[n].x, snake[n].y);
     
	  n--;
      
           
     }
    
     

    n2=0;
    while ( n2 < 6 )
    {
      screen[letterobj[n2].x][letterobj[n2].y]=tolower(letterobj[n2].letter);
      n2++;
    }
    
     n2=5;
     while (n2 >= 0)
    {
	if ( snake[snakelen].x==letterobj[n2].x&& snake[snakelen].y==letterobj[n2].y && letterobj[n2].letter!=' ' )
     {
       snakelen++;
       snake[snakelen].x=letterobj[n2].x, snake[snakelen].y=letterobj[n2].y,
        snake[snakelen].letter=letterobj[n2].letter;
          letterobj[n2].letter=' ';
          snake[snakelen+1].letter='\0';
        }    
          n2--;
	  
	  }
	
     n4=0;
     unsigned char snakescribed[255];
     while ( n4 < snakelen+1 ) snakescribed[n4]=tolower(snake[n4].letter), n4++;
     snakescribed[snakelen+1]='\0';
     printf("%s\n",snakescribed);
     
 n4=0;
  while ( n4 < NUM_WORDS )
  { 
    if ( strstr(snakescribed,words[n4].word)!= NULL )
     {
     	n2=(long int)(strstr(snakescribed,words[n4].word));
     	n2-=(long int)snakescribed;
      n3=0;
      while ( n3 < snakelen-words[n4].len-n2+1 )
        {
        snake[n3+n2].x=snake[n3+words[n4].len+n2].x;
         snake[n3+n2].y=snake[n3+words[n4].len+n2].y;
         snake[n3+n2].letter=snake[n3+words[n4].len+n2].letter;
         n3++;
        }                             
        snakelen-=(words[n4].len);
     }   
     n4++;
   }

    system("cls\n");
      y=0;
    while ( y < 50 )
    {
     x=0;
     while ( x < 80 )
     {
      printf("%c%c%c%c%c%c%c%c",screen[x][y],
                                screen[x+1][y],
                                screen[x+2][y],
                                screen[x+3][y],
                                screen[x+4][y],
                                screen[x+5][y],
                                screen[x+6][y],
                                screen[x+7][y]);
      x+=8;
     }
     printf("|\n");
     y++;
     }   
     printf("-----------------------------------------------------------------------------");
      

   }   
   gameover:  

  return (0);


}
