#include <stdio.h>
#include <string.h>


#define GROESSFELDX 32 // war 5 s unten war 10
#define GROESSFELDY 32
#define NUM_WORDS  12000   // war 152


#define SEQLEN 200
#define NUM_ELEMENTS 200


long int runs=0;

unsigned char screen[GROESSFELDX][GROESSFELDY];
unsigned char screeno[GROESSFELDX][GROESSFELDY];

int wordnum;

struct
{
 unsigned char word[35];
 unsigned int len;
 unsigned int taken;
 
} words[NUM_WORDS];

float best_score;
int fieldnotfull;

void backupfield(float score)
{
 int x=0, y=0;
 
 
 
 while ( y < GROESSFELDY)
 {
  x=0;
  while ( x < GROESSFELDX) 	
  {
  	//if ( screen[x][y]==' ') return ;
  	x++;
   }	
  y++;	
 }
 
 fieldnotfull=0;
 
 y=0;
 while ( y < GROESSFELDY)
 {
  x=0;
  while ( x < GROESSFELDX) 	
  {
   screeno[x][y]=screen[x][y];	
  	x++;
   }	
  y++;	
 }
 best_score=score;


system("cls\n");
 y=0;
   while ( y < GROESSFELDY+2)
   {
   	x=0;
   	while ( x < GROESSFELDX+2)
   	{
   	
   	 if ( y== GROESSFELDY+1 || y==0) printf("-");
   	 else
	 if ( x==0 || x == GROESSFELDX+1) printf("|"); else
	 
	 
	 printf("%c",screeno[x-1][y-1]);
   	 x++;	
	}
   	printf("\n");
   	y++;
   }
   
   printf("%f\n",best_score);


}



struct
{
 signed int word[SEQLEN];
 int horv[SEQLEN];
 int x[SEQLEN], y[SEQLEN];
 
 float eval;	
	
} evokombi[NUM_ELEMENTS];





int crossword(int evopatt )
{
 int n;
 unsigned char wordbuf[50];
 

   float score2=0;
  int y=0, x=0;
   int horv=0;
int wordnum;
wordnum=0;

signed int wnum=-1;

evokombi[evopatt].eval=0;

while ( wnum < SEQLEN)
{
     repeat:
    wnum++;  
    if ( wnum>=SEQLEN) break;
      n=0;
      score2=0;

while ( evokombi[evopatt].word[wnum]==-1 && wnum < SEQLEN ) wnum++;
 if ( wnum>=SEQLEN ) break;

wordnum=evokombi[evopatt].word[wnum];
 	
     	
  if ( evokombi[evopatt].horv[wnum]==0) horv='h'; else horv='v';
  
  x=evokombi[evopatt].x[wnum], y=evokombi[evopatt].y[wnum];
  

  
     	score2=0;
      
	
	   if ( horv=='h' )
       {
        if ( x+words[wordnum].len >= GROESSFELDX ) { x++; goto repeat; }
   
        n=0;
        while ( n < words[wordnum].len && x+n < GROESSFELDX )
        {  
         if ( screen[x+n][y]!= words[wordnum].word[n] && screen[x+n][y]!=' ' )
         { x++; score2=0; goto repeat; }
          if ( screen[x+n][y]==words[wordnum].word[n] ) score2+=100;
          if ( screen[x+n][y]==' ') score2++;
          
		 n++;
        }
        if ( n!= words[wordnum].len) { score2=0; goto repeat; }

        n=0;
		while ( n < words[wordnum].len && n+x < GROESSFELDX)
        {  
         wordbuf[n]=screen[x+n][y];
         
		 screen[x+n][y]= words[wordnum].word[n];
                  
         n++;
        }
        
        int old_wordnum;
        int wbuflen;
        wbuflen=words[wordnum].len;
        old_wordnum=wordnum;
		//wordnum++; 
        words[wordnum].taken=1;
        
        
        evokombi[evopatt].eval+=score2;
        
        score2=0;
        
		 {
		  goto repeat;
		 }
           

       }


	   if ( horv=='v' )
       {
        if ( y+words[wordnum].len >= GROESSFELDY ) { x++; goto repeat; }
   
        
        n=0;
        while ( n < words[wordnum].len && y+n < GROESSFELDY )
        {  
         if ( screen[x][y+n]!= words[wordnum].word[n] && screen[x][y+n]!=' ' )
         { x++; score2=0; goto repeat; }
         
         if ( screen[x][y+n]==words[wordnum].word[n] ) score2+=100;
         if ( screen[x][y+n]==' ') score2++;
         
		 n++;
        }
        if ( n!= words[wordnum].len) { score2=0; goto repeat; }


        n=0;
		while ( n < words[wordnum].len && n+y < GROESSFELDY)
        {  
         wordbuf[n]=screen[x][y+n];
         
		 screen[x][y+n]= words[wordnum].word[n];
                  
         n++;
        }	
		
		
	    evokombi[evopatt].eval+=score2;
       	
       	score2=0;
		
		 {
			goto repeat;
         	
		 }

     }
  
 
  }

 if ( evokombi[evopatt].eval >= best_score || fieldnotfull==1 ) backupfield(evokombi[evopatt].eval);
     
  return 1;
  
  

}







void combine(void)
{
 
 int opiter=0;
 
 int subit;
 
 subit=1;
 
 while ( subit < 50)
 {
 opiter=0;
 

   
 while ( opiter < SEQLEN)
 {
   int takewhich;
   takewhich=rand()%(NUM_ELEMENTS-1);

    evokombi[NUM_ELEMENTS-subit].eval=0;
	evokombi[NUM_ELEMENTS-subit].horv[opiter]=evokombi[takewhich].horv[opiter];
	evokombi[NUM_ELEMENTS-subit].word[opiter]=evokombi[takewhich].word[opiter];
	evokombi[NUM_ELEMENTS-subit].x[opiter]=evokombi[takewhich].x[opiter];
	evokombi[NUM_ELEMENTS-subit].y[opiter]=evokombi[takewhich].y[opiter];   
   
  opiter++;
 }
 
 subit++;
 
 }
 
}

void mutate(void)
{
	
	int subit=1;
	
	
	while ( subit < 50)
 {
 int opiter;
 opiter=0;
 
   
 while ( opiter < SEQLEN)
 {
  
    evokombi[NUM_ELEMENTS-subit].eval=0;
	evokombi[NUM_ELEMENTS-subit].horv[opiter]=rand()%2;
	evokombi[NUM_ELEMENTS-subit].word[opiter]=rand()%NUM_WORDS;
	evokombi[NUM_ELEMENTS-subit].x[opiter]=rand()%GROESSFELDX;
	evokombi[NUM_ELEMENTS-subit].y[opiter]=rand()%GROESSFELDY;
   
     
  opiter++;
 }
 
 subit++;  
}
 
}


void sort_eval_cut(void)
{
 int n=0, n2=0,n3=1;
    signed int bufx, bufy, bufword, bufhorv;
	float buf5;
     
     
     int opiter=0;
     
 while ( n3 < NUM_ELEMENTS)
{
 n2=NUM_ELEMENTS-1;
 while (n2 > n3)
 {
  if (  evokombi[n2].eval > evokombi[n2-1].eval ) 
    {
    	n3=0;
    
	 buf5=evokombi[n2-1].eval;
	 evokombi[n2-1].eval=evokombi[n2].eval;
	 evokombi[n2].eval=buf5;
	 	
    	opiter=0;
    	while ( opiter < SEQLEN)
    	{
    	 
    	 bufx=evokombi[n2-1].x[opiter],
    	 bufy=evokombi[n2-1].y[opiter],
    	 bufword=evokombi[n2-1].word[opiter],
    	 bufhorv=evokombi[n2-1].horv[opiter],
    	 
		 evokombi[n2-1].x[opiter]=evokombi[n2].x[opiter],
		 evokombi[n2-1].y[opiter]=evokombi[n2].y[opiter],
		 evokombi[n2-1].word[opiter]=evokombi[n2].word[opiter],
		 evokombi[n2-1].horv[opiter]=evokombi[n2].horv[opiter],
		 
		 evokombi[n2].x[opiter]=bufx;
		 evokombi[n2].y[opiter]=bufy;
		 evokombi[n2].word[opiter]=bufword;
		 evokombi[n2].horv[opiter]=bufhorv;
		 
       opiter++;
	   }
   }
  n2--;
  }
  
  
  n3++;
 }	
 
}








void erasethedupes(void)
{
  int count=0;
  int n=0, n2=0;
  
  while ( count < NUM_ELEMENTS)
  {
  
  n=0;
  while ( n < SEQLEN)
  {
  	n2=n+1;
  	while ( n2 < SEQLEN)
   	{
     if ( evokombi[count].word[n]== evokombi[count].word[n2] ) evokombi[count].word[n2]=-1; // evokombi[count].eval=0;
   	 n2++;
    }
    n++;
   }
  
   count++; 
  }
  	
  	
}



void initkombis(void)
{

	int subit=1;
	
	
	while ( subit <= NUM_ELEMENTS)
 {
 int opiter;
 opiter=0;
 
   
 while ( opiter < SEQLEN)
 {
  
    evokombi[NUM_ELEMENTS-subit].eval=0;
	evokombi[NUM_ELEMENTS-subit].horv[opiter]=rand()%2;
	evokombi[NUM_ELEMENTS-subit].word[opiter]=rand()%NUM_WORDS;
	evokombi[NUM_ELEMENTS-subit].x[opiter]=rand()%GROESSFELDX;
	evokombi[NUM_ELEMENTS-subit].y[opiter]=rand()%GROESSFELDY;
   
     
  opiter++;
 }
 
 subit++;  
}
   	
}







int main(void)
{
 FILE *input;
   int wc;
  unsigned char cpstring[35];

   wc=0;
   
   input=fopen(".\\words.txt","rb");
   while ( wc < NUM_WORDS )
  {
   fscanf(input,"%s\n",cpstring);
   strcpy(words[wc].word,cpstring);
   //printf("%s\n",words[wc].word);
   
   words[wc].len=strlen(cpstring);
   
   words[wc].taken=0;
   wc++;
  }
 
 /*
  int xc=0;
  while ( xc < NUM_WORDS)
  printf("%s %d\n",words[xc].word, words[xc].len), xc++;
  */
  
   wordnum=0;
   //getch();
   
   fclose(input);
   



   
   int x=0,y=0;
   while ( y < GROESSFELDY )
   {
   	x=0;
   	while ( x < GROESSFELDX)
   	{
   	 screen[x][y]=' ';
   	 screeno[x][y]=' ';
		x++;	
	}
   	
   	y++;
   }


   fieldnotfull=1;



initkombis();


   best_score=0;
   

while (1 )
{
   
      int x=0,y=0;
      x=0, y=0;
   while ( y < GROESSFELDY )
   {
   	x=0;
   	while ( x < GROESSFELDX)
   	{
   	 screen[x][y]=' ';
		x++;	
	}
   	
   	y++;
   }

   combine(), erasethedupes(), sort_eval_cut() ;
   
   if ( rand()%2==0 )mutate(), erasethedupes(), sort_eval_cut();

   
   int repeatit=0;
   
   repeatit=0;
   
   while ( repeatit < 20)
   {
   	
   	
      x=0, y=0;
   while ( y < GROESSFELDY )
   {
   	x=0;
   	while ( x < GROESSFELDX)
   	{
   	 screen[x][y]=' ';
		x++;	
	}
   	
   	y++;
   }
    erasethedupes();
    crossword(rand()%NUM_ELEMENTS);
    erasethedupes();
     sort_eval_cut();  
    erasethedupes();
	repeatit++;
   }
   
   if ( kbhit() ) break;
  /* 
   system("cls\n");
   y=0;
   while ( y < GROESSFELDY+2)
   {
   	x=0;
   	while ( x < GROESSFELDX+2)
   	{
   	
   	 if ( y== GROESSFELDY+1 || y==0) printf("-");
   	 else
	 if ( x==0 || x == GROESSFELDX+1) printf("|"); else
	 
	 
	 printf("%c",screeno[x-1][y-1]);
   	 x++;	
	}
   	printf("\n");
   	y++;
   }
   */

}
   
   
    
}
