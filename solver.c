#include <stdio.h>
#include <string.h>

typedef
struct
{
  int size;
  char name[255];
} per;

per person[10], person_zw;



typedef
struct
{
 unsigned char relation;
 int person_1, person_2;
  int ignore;
} rel;

rel relation[10], relation_zw;

typedef
struct
{
  int size;
} perpro;

 perpro person_prob[10], person_prob_zw;

typedef 
struct
{
  int ignore;
} relpro;
relpro relation_prob[10], relation_prob_zw;

long int score, score_old, longcount;


struct
{
  unsigned char text[255];
  unsigned char type;
} token[255];

 int n2;

void remove_tokens( int position, int amount_removed )
{

 while( position+ amount_removed <= n2 )
 {
  token[position].type=token[position+amount_removed].type;
  strcpy( token[position].text, token[position+amount_removed].text );

  position++;
 }

 n2-=amount_removed;

}


int main(void)
{
  unsigned char expression[1024];
   int limitl, limitr, limitl2, limitr2, reduction_type;
  signed int n, n3, state; 
  int rel_count=0; 

  gets(expression ); 
  
  n=0,n2=0;
  state=0;



    if ( expression[n]==' ')state=0;
    if ( isalpha ( expression[n] ) ) state=1;
    if ( expression[n]=='>' || expression[n]=='<'|| expression[n]== '=' ) state=2;
    if ( expression[n]==',' ) state=3;
    if ( expression[n]==';' ) state=4;

    while ( n < strlen(expression) )
  {
   
   if ( state==0 )
   {
    while ( expression[n]==' ' && n < strlen(expression) ) n++;
    if ( n >= strlen(expression) ) break;
     
    if ( expression[n]==' ' ) state=0;
    else if ( isalpha ( expression[n] ) ) state=1;
    else if ( expression[n]=='>' || expression[n]=='<'|| expression[n]== '=' ) state=2;
    else if ( expression[n]==',' ) state=3;
    else if ( expression[n]==';' ) state=4;
    else return;
   }

   if ( state==1 )
   {
    n3=0;
    while ( isalpha(expression[n]) && n < strlen(expression))
    token[n2].text[n3]=expression[n], n++,n3++;
    token[n2].text[n3]='\0';
    token[n2].type='v';
    n2++;
    
    if ( n >= strlen(expression)) break;
    
    if ( expression[n]==' ' ) state=0;
    else if ( isalpha ( expression[n]  ) ) state=1;
    else if ( expression[n]=='>' || expression[n]=='<'|| expression[n]== '=' ) state=2;
    else if ( expression[n]==',' ) state=3;
    else if ( expression[n]==';' ) state=4;
    else return;
    }
    

   if ( state==2 )
   {
    n3=0;
    token[n2].text[n3]=expression[n], n++,n3++;
    token[n2].text[n3]='\0';
    token[n2].type='o';
    n2++;
    
    if ( n >= strlen(expression)) break;
    
    if ( expression[n]==' ' ) state=0;
    else if ( isalpha ( expression[n] ) ) state=1;
    else if ( expression[n]=='>' || expression[n]=='<'|| expression[n]== '=' ) state=2;
    else if ( expression[n]==',' ) state=3;
    else if ( expression[n]==';' ) state=4;
    else return;
    }


   if ( state==3 )
   {
    n3=0;
    token[n2].text[n3]=expression[n], n++,n3++;
    token[n2].text[n3]='\0';
    token[n2].type='s';
    n2++;
    
    if ( n >= strlen(expression)) break;
    
    if ( expression[n]==' ' ) state=0;
    else if ( isalpha ( expression[n] ) ) state=1;
    else if ( expression[n]=='>' || expression[n]=='<'|| expression[n]== '=' ) state=2;
    else if ( expression[n]==',' ) state=3;
    else if ( expression[n]==';' ) state=4;
    else return;
    }    
     
    
   if ( state==4 )
   {
    n3=0;
    token[n2].text[n3]=expression[n], n++, n3++;
    token[n2].text[n3]='\0';
    token[n2].type='a';
    n2++;
    
    break;
    }    

   }
  
   rel_count=0;
    n=0,n3=0;
    reduction_type=' ';
while (reduction_type!='a')
{
	reduction_type='f';
 limitl=0, limitr=n2;
 
 /*
 while ( limitl < n2)printf(" %c ",token[limitl].type), limitl++;
 limitl=0, printf("\n");
 */

 limitl2=limitl, limitr2=limitr-1;
 
  while ( limitr2-limitl2 >= 2)
  {
   if ( token[limitr2].type=='v' && token[limitr2-1].type=='o' && token[limitr2-2].type=='v' )
   {
   	limitl=limitr2-2, limitr=limitr2;
   	reduction_type='o';
  	
   } limitr2--; }
   


 limitl2=limitl, limitr2=limitr-1;
 
  while ( limitr2-limitl2 >= 2)
  {
   if ( token[limitr2].type=='r' && token[limitr2-1].type=='s' && token[limitr2-2].type == 'r')
   {
   	limitl=limitr2-2, limitr=limitr2;
   	 reduction_type='s';
  	
   } limitr2--; }


 if ( limitr == 2)
  {
  
   if ( token[limitl+1].type=='a'&& token[limitl].type=='r') reduction_type='a';
 } else
  if ( limitr==1)
  {
   if ( token[limitl].type=='a' )reduction_type='a';
  }
 


	
	
	if (reduction_type=='o')
	{
		
		n3=n > 0 ? n-1 : n;
		while ( strcmp(person[n3].name, token[limitl].text)!= 0 && n3 > -1)n3--;
     if ( n3==-1 )n++,n3=n-1,
     strcpy(person[n3].name, token[limitl].text);
     
     
	 relation[rel_count].person_1=n3;
     
		n3=n > 0 ? n-1 : n;
		while ( strcmp(person[n3].name, token[limitr].text)!= 0 && n3 > -1)n3--;
     if ( n3==-1 )n++, n3=n-1,
     strcpy(person[n3].name, token[limitr].text);
     
     relation[rel_count].person_2=n3;
     
     relation[rel_count].relation=token[limitr-1].text[0];
     rel_count++;
     
     remove_tokens(limitl, limitr-limitl);
     token[limitl].type='r';
	  
	}
	
	if ( reduction_type=='s' )  remove_tokens(limitl+1, limitr-limitl),token[limitl].type='r';           
						               	   
	if ( reduction_type=='f') return;
	
}


/*
n2=0;
while ( n2 < n)
printf("Es gibt den Namen %s\n", person[n2].name), n2++;

n2=0;
while (n2 < rel_count)
printf ("Es gibt die Relation %d %c %d\n",relation[n2].person_1, relation[n2].relation, relation[n2].person_2),n2++;
*/

n3=0;
score_old=0;
longcount=0;
while ( n3 < 10000000   )
{
 
 n2=0;
 while ( n2 < rel_count)
 relation[n2].ignore=rand()%1,n2++;
 
 n2=0;
 while ( n2 < n)person[n2].size=rand()%n,n2++, longcount++;
 
 n2=0;
 score=0;
 while ( n2 < rel_count)
 {
 	
 	if ( relation[n2].ignore==0)
 	{
	 //score++;
   	if ( relation[n2].relation=='=' && person[relation[n2].person_1].size == person[relation[n2].person_2].size)score++;
 	if ( relation[n2].relation=='<' && person[relation[n2].person_1].size < person[relation[n2].person_2].size)score++;
 	if ( relation[n2].relation=='>' && person[relation[n2].person_1].size > person[relation[n2].person_2].size)score++;	
   n2++;
  }
  
  	

   }
   
   
  	if ( score >= score_old)
  	{
  		score_old=score;
  		
  		n2=0;
         while ( n2 < rel_count)
         relation_prob[n2].ignore=relation[n2].ignore,n2++;
 
 
        n2=0;
         while ( n2 < n)
         person_prob[n2].size=person[n2].size,n2++;
 
  		if ( score==rel_count) break;
	  }
   
 n3++;
 }

 // printf("%d %d\n",score_old, rel_count);
 n2=0;
 
 while ( n2 < n)
 {
 	n3=n-1;
  while ( n3 > n2) {
  	if ( person_prob[n3].size > person_prob[n3-1].size )
  	{
 		
      /*
 		memcpy(&person_zw,&person[n3-1], sizeof(person[0]));
 		memcpy(&relation_zw,&relation[n3-1], sizeof(relation[0]));
 	    memcpy(&relation_prob_zw, &relation_prob[n3-1], sizeof(relation_prob[0]));
	    memcpy(&person_prob_zw, &person_prob[n3-1], sizeof(person_prob[0]));
	 
	    memcpy(&person[n3-1],&person[n3], sizeof(person[0]));
 		memcpy(&relation[n3-1],&relation[n3], sizeof(relation[0]));
 	    memcpy(&relation_prob[n3-1], &relation_prob[n3], sizeof(relation_prob[0]));
	    memcpy(&person_prob[n3-1], &person_prob[n3], sizeof(person_prob[0]));
	 
	    memcpy(&person[n3],&person_zw, sizeof(person[0]));
 		memcpy(&relation[n3],&relation_zw, sizeof(relation[0]));
 	    memcpy(&relation_prob[n3], &relation_prob_zw, sizeof(relation_prob[0]));
	    memcpy(&person_prob[n3], &person_prob_zw, sizeof(person_prob[0]));
	 
	  */
	  
	  person_zw=person[n3-1], relation_zw=relation[n3-1], relation_prob_zw=relation_prob[n3-1], person_prob_zw=person_prob[n3-1];
	  person[n3-1]=person[n3], relation[n3-1]=relation[n3], relation_prob[n3-1]=relation_prob[n3], person_prob[n3-1]=person_prob[n3];
	  person[n3]=person_zw, relation[n3]=relation_zw, relation_prob[n3]=relation_prob_zw, person_prob[n3]=person_prob_zw;
	 	
      }
 		n3--;

	 }
   
     n2++;
   }



n2=0;
if ( score != rel_count) printf("Die Aussage ist logisch widerspruechlich.\n");
while ( n2 < n)
printf("%d. %s\n", n2+1, person[n2].name), n2++;
getch();
/*
n2=0;
while (n2 < rel_count)
printf ("Es gibt die Relation %d %c %d\n",relation[n2].person_1, relation[n2].relation, relation[n2].person_2),n2++;
*/
  		

}
