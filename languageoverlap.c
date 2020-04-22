/*
  Just paste some ASCII text together and this tool will calculate the similarity overlap in word use.
  You might just want to automatically determine the language a text was written in.
  Or maybe you want to identify someone who is posting or chatting and thinks he is anonymous.
  Or maybe you just wanna draw conclusions about someone's personality automatically.
  Maybe find 'like'-minded individuals using a web-spider.
  Maybe you only want to know which writer at most writes the way you talk.
  */



//#define COMPILE_PROFILE_GENERATOR

#ifdef COMPILE_PROFILE_GENERATOR


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct
{
 unsigned char wort[255];
 float frequenz;
}woerter[250000];


int main(int argc, char *argv[])
{
 FILE *input, *output;
 unsigned char c;
 unsigned long int n;
 unsigned long int wc;
 unsigned char cmpstring[255];
 float num_tokens=0;
 
 if ( argc!=2) return 1;
 
 input=fopen(argv[1],"rb");
 if ( input==NULL) {
 	printf("Datei nicht gefunden!");
 	return 1;
 }
 	
 	wc=0;
 	while (1)
 	{
	 
	    n=0;
	    while (!isalpha(c=fgetc(input))&& !feof(input))
	    {   
	    
		}
	    
	    do
	    {
    	  if ( feof(input)) { fclose(input);
 	  
    	   printf("%d\n", wc);
       	   goto writelist;
	      }	
	  
	      cmpstring[n]=tolower(c);
	      n++;
	      if ( n==254) break;
		 }
		 while (isalpha(c=fgetc(input))&& !feof(input)&&n < 254);
    	
 	    cmpstring[n]='\0';
 	    
 	     if ( n > 2 )
 	     { 
 	     n=0;
 	     while ( n < wc)
 	     {
 	      if ( strcmp(cmpstring,woerter[n].wort)==0)
		   {
		   	 woerter[n].frequenz++;
		   	 num_tokens++;
		   	 break;
 	       }
			n++;
		  }
		  if ( n==wc)
		  {
		  	strcpy(woerter[n].wort,cmpstring);
		   	  num_tokens++;
			woerter[n].frequenz=1;
				 
		    wc++;
		  }
		  if ( wc==250000) { printf("Mindestens eine viertel Million Woerter!"); return 2; }
 	    }
    	}
 	   
		writelist:
		 
 	    if ( wc > 0)
 	    {
		 
 	      output=fopen(".\\profil.txt","wb");
         
        while (wc > 0)
 	    {
		  wc--;
		   fprintf(output,"%s %f\n", woerter[wc].wort,woerter[wc].frequenz/num_tokens );		
		 }
 	    
 	    fclose(output);
		 
	 }
	 
	 return 0;
	
}

#else ifndef COMPILE_PROFILE_GENERATOR

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef
struct
{
 unsigned char wort[255];
 float frequenz;
}woerter_ptr;

woerter_ptr * woerter;

typedef
struct
{
 unsigned char wort[255];
 float frequenz;	
} woerter_prob_ptr;

woerter_prob_ptr *woerter_prob;



int main(int argc, char *argv[])
{
 FILE *input, *output;
 unsigned char c;
 signed int wc_found;
 signed long int n;
 signed long int wc;
 unsigned char cmpstring[255];
 float num_tokens=0;
 signed int prof_wc;
  float uebereinstimmung=0.0;
  int hit=0;
  float numsum;	
   float prof_wc_found_bak;
    float restprob=1.0, restwort=1.0;	   
 
 
 woerter_prob= malloc(sizeof (woerter_prob_ptr)*250000);
 woerter= malloc(sizeof (woerter_ptr)*250000);
 
 if ( woerter_prob==NULL || woerter==NULL) { printf("Hat nicht geklappt"); return 1; }
 
 
 if ( argc!=3) return 1;
 
 input=fopen(argv[1],"rb");
 if ( input==NULL) {
 	printf("Datei nicht gefunden!");
 	return 1;
 }
 	
 	n=0;
 	wc=0;
 	while (!feof(input))
 	{
	 fscanf(input, "%s %f\n",woerter[n].wort, &woerter[n].frequenz), n++;
      //woerter[n-1].found=0;
     	} //


 fclose(input);
 
 
 prof_wc=n-1;

 num_tokens=0;

 input=fopen(argv[2],"rb");
 if ( input==NULL) {
 	printf("Datei nicht gefunden!");
 	return 1;
 }
 	
 	n=0;
 	while (!feof(input))
 	{
	 fscanf(input, "%s %f\n",woerter_prob[n].wort, &woerter_prob[n].frequenz), n++;
   
    	} //


     fclose(input);

     wc=n-1;
	 num_tokens=n;
     
 	   	  
 	   	  
 	   	  restprob=1.0, restwort=1.0;
 	   	  
		 wc_found=wc;	   
 	    while ( wc > -1)
 	    {
		 
		 hit=0;
 	     n=0;
		  n=prof_wc; 
        
		while (n > -1)
 	    {
 	    	
		  if ( strcmp(woerter[n].wort, woerter_prob[wc].wort)==0 ) uebereinstimmung+=fabs(woerter[n].frequenz-woerter_prob[wc].frequenz),hit=1,
		  restwort-=woerter[n].frequenz, restprob-=woerter_prob[wc].frequenz;
		  n--;
		 }
 	    
 	    
 	   	 wc--;
 	   }
	   
	    uebereinstimmung=uebereinstimmung+restprob+restwort;
	    
	    
	    
	   printf("\nUebereinstimmung: %f\n", uebereinstimmung );
	  
	 return 0;
	
}
#endif
