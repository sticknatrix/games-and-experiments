/*
  for example:
  you type: find a great song like bohemian rhapsody 0
  
  it has read a text file with the following content:
  
  Song classics.
  Bohemian Rhapsody sounds strangely familiar.
  The chord progression in part 1 was based on Without You.
  Without You became one of the most popular ballads ever written
  and was covered by many artists, including Heart.
  Badfinger never had much success, although Without You remains a classic.
  It is a great song, perhaps the greatest song ever written.

  After it has read the text, might match rhapsody with your input.
  Without will give a temporal coincidence with rhapsody.
  Then it could associate song with Without You over another coincidence.
  You typed in song, the string will match and song is evaluated upwards..
  Then there is another temporal coincidence with great.
  Classic will match with Badfinger.
  Now on the top of the entrys, there's the string badfinger and the timestamp
  value of the corresponding text passage which will be printed at the end of
  the program run.
  
  Do you know what i mean? intelligent, heuristic text search with an evolutionary
  algorithm. The search can think.
  
  */



  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  
  int num_elements;
  
FILE *input;
  
  unsigned int timeevent=0;
 long unsigned int ausgelernt_time=10000000;
 
long int globaltime;

unsigned char comparestring[30];
unsigned char stringa[30];
unsigned char stringa_buf[3000][30];


int questionlen=0;

int identiup=0;

struct
{
 unsigned char string[30];
}questions[5];

int remembered=0;


//Anzahl der Eintraege (und die Laenge des zu ueberschreibenden Anhangs)
#define NUM_ELEMENTS (num_elements)
#define RUSHIN (num_elements/50)


struct
{
 unsigned char string[30]; // ein String mit dem zu vergleichenden Wort

} shapeseq[20000]; //aktuelle 'Zeit'

struct
{
 long int timestamp;
   int perish;	
} shapecontext[20000];



struct
{
 
 long int timestamp;
 		
} shapecontext_old; //Timestamp-Wert des Vergleichs-Vorgaengers



float evalseq[20000]; // die Bewertungen (habe das halt jetzt doch nicht alles in einem Struct)

signed int dupetokill,dupeoffset;


int trained=0; // Dateilese-Abbruch

// 'Spielstatus' sichern und laden

void backuptraining(void)
{
  FILE *traindata;
  
  traindata=fopen(".\\training.evo","wb");
  int n=0;
  while ( n < NUM_ELEMENTS)
  {
   fprintf(traindata,"%s %d %f\n",shapeseq[n].string, shapecontext[n].timestamp,evalseq[n]);
  	n++;
  }
  fclose(traindata);
  
  trained=1;
  	
	
}


int frontallappen(void)
{
 signed int num=0;
 int n=0;
 while ( n < NUM_ELEMENTS-RUSHIN) ( rand()%2==1 ? num++ : 0 ), n++;
 num-=(NUM_ELEMENTS-RUSHIN)/2;
 if ( num < 0 ) num=0;
 return num;
}


void restoretraining(void)
{
  FILE *traindata;
  
  traindata=fopen(".\\training.evo","rb");
  int n=0;
  while ( n < NUM_ELEMENTS)
  {
   fscanf(traindata,"%s %d %f\n",&(shapeseq[n].string[0]), &shapecontext[n].timestamp,&evalseq[n]);
  	n++;
  }
  fclose(traindata);
  trained=1;
	
}


static int ausgelernt=0;

// scanf-Ersatz
void scanS(unsigned char *string)
{
  unsigned char zeichen;
  int n=0,n2=0;
  
  //if ( kbhit() ) ausgelernt=1, getch(), printf("\?");
  
  if ( !/*ausgelernt==*/1) {
  
  repeat:
  n=0, n2=0;
  
  while ( n < 29 )
  {
  	repeat3:
     scanf("%c",&zeichen),string[n2]=tolower(zeichen), n++;
 	 if ( zeichen=='!' ) restoretraining();
	 if ( zeichen=='\?'&& ( trained==0) ) { ausgelernt=0; break; }

   if (isalpha(zeichen))n2++; else break;
 }
 	string[n2]='\0';
 	
 	if ( n2 < 2) goto repeat;
    return;
  }
  repeat2:
  n=0, n2=0;
  
  while ( n < 29 )
  {
    fscanf(input,"%c",&zeichen),string[n2]=tolower(zeichen), n++;
   if ( feof(input)) { ausgelernt=1; printf("\?"); fclose(input); backuptraining(); return; }
  if (isalpha(zeichen))n2++; else break;
 }
 	string[n2]='\0';
 	
 	if ( n2 < 2) goto repeat2;
 		
}

   float evaluation;


// fuelle den aktuellen Eintrag zum Vergleichen, damit er mit seinem
// zufaelligen Nachfolger in der Schleife verglichen werden kann

void fillsome(int funcnr)
{
int n=0;
 	if ( trained==0)
	{
 while ( n < 30 )	
  stringa[n]=shapeseq[funcnr].string[n], n++;	
  shapecontext_old.timestamp=shapecontext[funcnr].timestamp;
 }
  if ( trained==1)
  {
   n=0;
   while ( strcmp(shapeseq[funcnr].string,stringa)!=0 && n < NUM_ELEMENTS*2)
     funcnr=rand()%(NUM_ELEMENTS-RUSHIN),n++;	
        shapecontext_old.timestamp=shapecontext[funcnr].timestamp;  	
  }

}

unsigned char stringc[30];


// Fuelle ein Wort in den Ausgabestring

void fillsome_dump(int funcnr)
{
 int n=0;
 while ( n < 30 )
  stringc[n]=shapeseq[funcnr].string[n], n++;	
}


// Vergleiche zwei zufaellige Eintraege und werte einen eventuell rauf


 


int compare_vaguely(int funcnr)
{  


if ( rand()%2==0)
{
	
 if (
        (shapecontext[funcnr].timestamp>=shapecontext_old.timestamp-20&&
      shapecontext[funcnr].timestamp<=shapecontext_old.timestamp+200 && 
	    /*strcmp(shapeseq[funcnr].string, stringa)!=0 &&*/
		 strcmp(shapeseq[funcnr].string, "000")!=0)
		  ) {evalseq[funcnr]+= 1000.0, shapecontext[funcnr].timestamp < ausgelernt_time? remembered=1 : 0;
		
 //if(questionlen!=0)if ( strcmp(stringa,questions[rand()%questionlen].string)==0) printf("Treffer");
 }
  return 1;
}
else
{
if ( /*( fabs(shapecontext[funcnr].timestamp-shapecontext_old.timestamp) > 10 || identiup==1)&&*/ strcmp(shapeseq[funcnr].string,stringa)==0
&& strcmp(shapeseq[funcnr].string, "000")!=0 && shapeseq[funcnr].string[0]!='\0') {
 evalseq[funcnr]+= 1000.0, shapecontext[funcnr].timestamp < ausgelernt_time? remembered=1 : 0;

 if(questionlen!=0)if ( strcmp(stringa,questions[rand()%questionlen].string)==0) printf("Treffer");
  
}
 return 1;
}
// other comparison criterias are thinkable, like puns, phonetic comparison, hamming-distance...
    return 1;
    
}


void combine(void)
{
 
 int opiter=0;
 
  int a=1;
  
  int elema=rand()%(NUM_ELEMENTS-RUSHIN);
  int elemb=rand()%(NUM_ELEMENTS-RUSHIN);
  
  while ( a <= RUSHIN )
  { 
  
  
  if ( rand()%2==0) strcpy(shapeseq[NUM_ELEMENTS-a].string,shapeseq[elema].string);
  else 
                    strcpy(shapeseq[NUM_ELEMENTS-a].string,shapeseq[elemb].string);
  
  if ( rand()%2==0) shapecontext[NUM_ELEMENTS-a].perish=shapecontext[elemb].perish;
  else 
                    shapecontext[NUM_ELEMENTS-a].perish=shapecontext[elema].perish;
  
  
  
  shapecontext[NUM_ELEMENTS-a].timestamp =shapecontext[rand()%2==0 ? elema : elemb].timestamp;
 
   evalseq[NUM_ELEMENTS-a]=1.0;
   
 a++;
}
 
}


void reset(void)
{
 int a=1;
 while ( a <=RUSHIN) evalseq[NUM_ELEMENTS-a]=0.0, a++;
}


void mutate(void)
{
 int opiter=0;
 
  int a=1;
   
   
   while ( a <= RUSHIN )
   {
   	
	   strcpy(shapeseq[NUM_ELEMENTS-a].string,comparestring);
  
   shapecontext[NUM_ELEMENTS-a].timestamp=globaltime;
  
  shapecontext[NUM_ELEMENTS-a].perish=0;
  
    evalseq[NUM_ELEMENTS-a]=1.0;
    
    
  int elemrnd=rand()%(NUM_ELEMENTS);
  
    
   a++;
 }
 

}
 

// Sortiert das Array mit Bubblesort entsprechend seiner Bewertung
   
void sort_eval_cut(void)
{
 int n=0, n2=0,n3=1;
    signed int bufletter;
	
	float buf5;
     
     long int bufseqtimestamp;
     
     
     
     int opiter=0;
     
 while ( n3 < NUM_ELEMENTS)
{
 n2=NUM_ELEMENTS-1;
 while (n2 > n3)
 {
  if ( evalseq[n2] > evalseq[n2-1] ) 
    {
    	n3=0;
    
	 buf5=evalseq[n2-1];
	 evalseq[n2-1]=evalseq[n2];
	 evalseq[n2]=buf5;
	 
	 bufseqtimestamp=shapecontext[n2-1].timestamp;
	shapecontext[n2-1].timestamp=shapecontext[n2].timestamp;
	shapecontext[n2].timestamp=bufseqtimestamp;
	  
	 
	 	
    	opiter=0;
    	while ( opiter < 30)
    	{
    	 
    	 bufletter=shapeseq[n2-1].string[opiter]; 
		 shapeseq[n2-1].string[opiter]=shapeseq[n2].string[opiter];
		 shapeseq[n2].string[opiter]=bufletter;
       opiter++;
	   }
   }
  n2--;
  }
  
  
  n3++;
 }	
 
}



//Ordne ein bestimmtes Element entsprechend seiner Bewertung ein

void insert_cut(int funcnr)
{
 int n=0, n2=0,n3=0;
 
 unsigned char bufstring[30];
 float buftimestamp;
 float funceval;
 int bufperish=0;
 
 funceval=evalseq[funcnr];
 buftimestamp=shapecontext[funcnr].timestamp;
 bufperish=shapecontext[funcnr].perish;
 strcpy(bufstring,shapeseq[funcnr].string);
 
     int opiter=0;
     
 while ( evalseq[n3] >= funceval && n3 < NUM_ELEMENTS ) n3++;
 
 if ( n3 >= NUM_ELEMENTS) return;
 if ( n3 >= funcnr) return;
 n2=funcnr;

 while ( n2 > n3)
 {
  	 evalseq[n2]=evalseq[n2-1];
	 
	shapecontext[n2].timestamp=shapecontext[n2-1].timestamp;
	shapecontext[n2].perish=shapecontext[n2-1].perish;
	  
	  
    	opiter=0;
    	while ( opiter < 30)
    	{
    	 
    	 shapeseq[n2].string[opiter]=shapeseq[n2-1].string[opiter];
	   opiter++;
	   }
    if ( n2-1==dupetokill) dupeoffset++;
	n2--;
 	
 }
 
 
  shapecontext[n3].timestamp=buftimestamp;
  shapecontext[n3].perish=bufperish;
  strcpy(shapeseq[n3].string,bufstring);
  evalseq[n3]=funceval;
  
}




void delete_item(int funcnr)
{
 int n=0, n2=0,n3=0;
 
 unsigned char bufstring[30];
 float buftimestamp;
 float funceval;
 int bufperish=0;
 
 funceval=evalseq[funcnr];
 buftimestamp=shapecontext[funcnr].timestamp;
 bufperish=shapecontext[funcnr].perish;
 strcpy(bufstring,shapeseq[funcnr].string);
 
     int opiter=0;
     
 
 
 n=NUM_ELEMENTS;
 n2=funcnr+1;
 
 while ( n2 < n)
 {
  	 evalseq[n2-1]=evalseq[n2];
	 
	shapecontext[n2-1].timestamp=shapecontext[n2].timestamp;
	shapecontext[n2-1].perish=shapecontext[n2].perish;
	  
	 
	 	
    	opiter=0;
    	while ( opiter < 30)
    	{
    	 
    	 shapeseq[n2-1].string[opiter]=shapeseq[n2].string[opiter];
	   opiter++;
	   }
    n2++;
 	
 }
  
  evalseq[NUM_ELEMENTS-1]=1.0;
  
}





int main(int argc, char *argv[])
{
 int x=0, y=0;
 int funcnr=0;
 
  int ausgelerntset=0;
  long int breakout=10000000;
  
 if ( argc!=3 ) return;
 input=fopen(argv[1],"rb");
 
 num_elements=atoi(argv[2]);
 if ( num_elements > 20000) num_elements=20000;
 
       globaltime=0; // fuer den Timestamp

  shapecontext_old.timestamp=0;
  strcpy(stringa,"000");


   int n2=0;
 
   scanS(comparestring); //das ist der Ersatz fuer Scanf: kann sowohl von der Konsole als
             // auch aus einer Datei lesen (obwohl: waren stdin und stdout nicht auch Dateien, die man
			// danach haette oeffnen koennen?!)
 
   n2=0;
   int n=0;
   
   
      // irgendwie das Array mit den Strings vorinitialisieren
   while (n2 < NUM_ELEMENTS)
   {
   
     n=0; 
       	strcpy(shapeseq[n2].string,"000");
		
		strcpy(stringa_buf[n2%RUSHIN],shapeseq[n2].string);
		
		
		if ( trained==1) break;
		shapecontext[n2].timestamp=0;
		shapecontext[funcnr].perish=0;
       evalseq[n2]=1.0;
   
    n2++;
   }
   
   fillsome(dupetokill=0);
   
   int runs=0;
 while (1 )
 {

  timeevent=0;
   globaltime++;
 	
 	// wenn er sich mal gerade nicht seinem 'Innenleben' zuwendet,
 	// braucht er Input
 
  n2=0;
   if ( runs%1==0)
   while ( n2 < NUM_ELEMENTS)
       {
          
		  if ( trained==0) 
		   evalseq[n2]/=1.0001;
		  else evalseq[n2]/=1.0001;
		  
		  if ( evalseq[n2]<1.0) evalseq[n2]=1.0;
		  if ( evalseq[n2]>10000.0) evalseq[n2]=10000.0;
		     
           n2++; }
   
   // generiere halt ein paar Neue bzw. lese diese
  
   
   // kombiniere zufaellig
   
   //nehme irgendeinen Eintrag zum Vergleich heraus
   
   runs++;
    if ( trained==0)scanS(comparestring);
   mutate();
   
   	 if ( rand()%10<2 )combine();
  
  
  identiup=1;
   funcnr=( rand()%100 < 50 ? NUM_ELEMENTS-1-rand()%RUSHIN  : rand()%(NUM_ELEMENTS) );
   if ( trained==1) funcnr=rand()%(NUM_ELEMENTS-RUSHIN);
   if ( trained==1&&rand()%10==0) funcnr=dupetokill,identiup=1;
  
  
     compare_vaguely(funcnr);
     	
     	dupeoffset=0;
      if ( trained==1) reset();
	   insert_cut(funcnr);
    
    if ( trained==0)
	 dupetokill=( rand()%100 < 50 ? NUM_ELEMENTS-1-rand()%RUSHIN  : rand()%(NUM_ELEMENTS) ), fillsome(dupetokill);
  	 else
  	 if ( rand()%20<18)
	   dupetokill=frontallappen(), fillsome(dupetokill);
	  else
	  {
	   if ( ausgelerntset==0)
	 strcpy(stringa,comparestring);
       else strcpy(stringa,questions[rand()%questionlen].string );
       
       fillsome( dupetokill);
	 }
	     
      if ( trained==1&&ausgelerntset==0)
	  {
	  	n=0;
	   do
	    {
	  	 scanf("%s",questions[n].string),n++;
	    }
		while ( questions[n-1].string[0]!='0'&& n < 5);
		   questionlen=n-1;	
	  	breakout=n*5000+globaltime;
	   } 
 
    if ( trained==1&&ausgelerntset==0) ausgelernt_time=globaltime, ausgelerntset=1;
  
     if ( globaltime==breakout) break;
   
 }
 
  n2=0;
 /*
  float mvalue_timestamp=0;
  while ( n2 < (NUM_ELEMENTS-RUSHIN)/50 ) mvalue_timestamp+=shapecontext[n2].timestamp,n2++;
  mvalue_timestamp/=n2;
  */
   while (n2<questionlen)
   {
   	trained=0;
 input=fopen(argv[1],"rb");
 ausgelernt=0;
 globaltime=0;
 while (ausgelernt==0)
 {
  scanS(comparestring);
   	if ( fabs ( /*mvalue_timestamp*/shapecontext[n2].timestamp-globaltime) < 50 ) printf("%s ",comparestring);
   	
   globaltime++;
  }
  fclose(input);
  n2++;
 }
 

}
