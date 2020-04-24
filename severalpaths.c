#include <stdlib.h>


#define UNPASSABLE       1
#define PASSABLE         0
#define ALREADY_PASSED   2
#define GOAL             3
#define MARK             4

#define FX_SIZE 7
#define FY_SIZE 7

unsigned int field[FX_SIZE][FY_SIZE];

int ptoken=0;

typedef
struct
{
 int x,y;
 unsigned   rec_count;
 signed int  prevID;
 signed int ID;
 unsigned   inactivated;

 void *prev, *next; /* Ende mit NULL terminiert */

}
rec_control;


rec_control *iteration_buf;
rec_control *Start;

 signed int ID_upcount;
 unsigned ID_subtract;

int check_path_overcross(int x, int y, int newID)
{
 int older_ID;

 older_ID=newID;
 
 
 
 for(;;)
 { 
  //printf("Id hat den Wert %d an den Koordinaten %d %d...\n", iteration_buf->prevID,
  //iteration_buf->x, iteration_buf->y );
  
  if (older_ID != -1 )
  {
   iteration_buf=Start;
   while(iteration_buf->ID!= older_ID )
                   iteration_buf=iteration_buf->next;
  }
  older_ID=iteration_buf->prevID;  

  if(iteration_buf->x== x && iteration_buf->y==y) return 1;
  
    if ( older_ID==-1) break;

  
 }

 return 0;
}



void insert_pos(int x, int y, int rec_count, int ID )
{
 rec_control *getlast, *old ;



 if ( check_path_overcross(x,y, ID )==1) return; // am 20.11.2017 neu
 

/*
 if( field[x][y]==ALREADY_PASSED )return;
 else */ 
 if (field[x][y]== GOAL ) return; //field[x][y]=ALREADY_PASSED;


/* auskommentiert am 20.11.2017*/

 //printf("Haenge Position %d %d in die Kette...\n",x,y);

 
  getlast=Start;


 while( getlast->next != NULL )
 {
  getlast=getlast->next;

 }

 old=getlast;

 getlast->next=malloc(sizeof ( rec_control ));
 if ( getlast->next==NULL) printf("Speicherallokationsfehler!"), getch();
 getlast=getlast->next;

 getlast->prev=old;
 getlast->next=NULL;
 getlast->x=x;
 getlast->y=y;
 getlast->rec_count=rec_count+1;
 getlast->inactivated=0;
 getlast->ID=ID_upcount;
 getlast->prevID=ID;

 ID_upcount++;


 /* Kettedumpen debug */
   /*
   getlast=Start;
   while(getlast!= NULL )
   {
    printf("Pointer in der Kette hat die Koordinaten %d %d und die Aktivierung %d...\n",
            getlast->x, getlast->y, getlast->inactivated );

     getlast=getlast->next;
   }
   */

 /* bis hier */

}

int fill( rec_control *pos)
{
 //printf("Bin am Anfang der Fuellfunktion mit den Koordinaten %d %d...\n",pos->x, pos->y ); /*debug */
 while ( pos->inactivated==1 && pos->next!= NULL ) pos=pos->next;
  if ( pos->inactivated==1 ) return 0;
 pos->inactivated=1; // auskommentiert am 20.11.2017
 ID_subtract=0;

 //printf("Bin am Anfang der Fuellfunktion mit den Koordinaten %d %d...\n",pos->x, pos->y ); /*debug */


  // den Feld-Nullcheck am 20.11.2017 raus
 if ( pos->x-1 >= 0 ) if (field[pos->x-1][pos->y]!=1 )
 {

  insert_pos(pos->x-1, pos->y, pos->rec_count, pos->ID);

 }
 if ( pos->x-1 >= 0 )
 {
  if ( field[pos->x-1][pos->y]==3 ) {iteration_buf=pos; return 1; }
 }

 if ( pos->x+1 < FX_SIZE ) if ( field[pos->x+1][pos->y]!=1 )
 {

  insert_pos(pos->x+1, pos->y, pos->rec_count, pos->ID);

 }

 if (pos->x+1 < FX_SIZE )
 {
  if ( field[pos->x+1][pos->y]==3 ){iteration_buf=pos;  return 1; }
 }


 if ( pos->y-1 >= 0 ) if ( field[pos->x][pos->y-1]!=1 )
 {

  insert_pos(pos->x, pos->y-1, pos->rec_count, pos->ID );

 }
 if (pos->y-1 >= 0 )
 {
  if ( field[pos->x][pos->y-1]==3 ){iteration_buf=pos;  return 1; }
 }


 if ( pos->y+1 < FY_SIZE ) if( field[pos->x][pos->y+1]!=1 )
 {

  insert_pos(pos->x, pos->y+1, pos->rec_count, pos->ID);

 }
 if (pos->y+1 < FY_SIZE )
 {
  if( field[pos->x][pos->y+1]==3 ){iteration_buf=pos;  return 1; }
 }



 return 0;

}

void backsort()
{

 rec_control *buf;

 do
 {
 iteration_buf=Start;

 if ( iteration_buf->next != NULL )
 {

   while(  iteration_buf->rec_count <=
          ((rec_control *)(iteration_buf->next ))->rec_count )
   {
     //printf("in der Suchschleife ... debug ");
     if( iteration_buf->next != NULL ) iteration_buf=iteration_buf->next;
     else break;
     if ( iteration_buf->next == NULL ) break;

   }
 } else break;

  if ( iteration_buf->next != NULL )
  {

   //printf("Habe einen Treffer gelandet %d %d, vertausche...\n",
      //      iteration_buf->rec_count, ((rec_control *)(iteration_buf->next))->rec_count); /*debug*/

    if ( iteration_buf != Start )
    ((rec_control *) (iteration_buf->prev ))->next=iteration_buf->next;

     printf("vorm if debug\n");

    if(

       (rec_control *) (  (rec_control *) (iteration_buf->next ))
       ->next

       != NULL )
    {
       ( (rec_control *) (  (rec_control *) (iteration_buf->next ))
       ->next ) -> prev=iteration_buf;
    }

     //printf("hinterm if debug...\n");

    ((rec_control *) (iteration_buf->next ))->prev=iteration_buf->prev;
    iteration_buf->prev=iteration_buf->next;

     //printf("eins weiter...\n");

    buf=
    ((rec_control *) (iteration_buf->next ))->next;

    ((rec_control *) (iteration_buf->next ))->next=iteration_buf;
    iteration_buf->next=buf;

 

    //printf("bin durch...\n");
  }

 }while ( iteration_buf->next != NULL );


 iteration_buf=Start;

 //printf("Am Ende der Sortierfunktion...\n");
}



void mark_path()
{
 int older_ID;
  
 for(;;)
 {
  //printf("Id hat den Wert %d an den Koordinaten %d %d...\n", iteration_buf->prevID,
  //iteration_buf->x, iteration_buf->y );

 // if ( field[iteration_buf->x][iteration_buf->y]!=GOAL)
  field[iteration_buf->x][iteration_buf->y]=ptoken%26+0x41 ; //20.11.2017
  
  older_ID=iteration_buf->prevID;

  if (older_ID != -1 )
  {
   iteration_buf=Start;
   while(iteration_buf->ID!= older_ID )
                   iteration_buf=iteration_buf->next;
 
  }
  else break;

 }
 ptoken=(ptoken+1);


}

void find_path(unsigned x, unsigned y) /* entspricht der Kontrollfunktion */
{

  int xc=0,yc=0;

 Start=malloc(sizeof(rec_control));

 Start->prev=NULL;
 Start->next=NULL;
 Start->x=x;
 Start->y=y;
 Start->rec_count=0;
 Start->inactivated=0; // am 20.11.2017
 Start->prevID=-1;
 Start->ID=-1;
 Start->next=NULL;
 iteration_buf=Start;

 while ( ptoken < 'Z'  ) { /* printf("Rufe die Ruecksortier-\n"
                                            "funktion auf...\n"); */ /* debug*/
											
											if ( fill(iteration_buf) != 0 )
											{
											
												
											
											
											mark_path();
											
											
												
											system("cls\n");
 yc=0;
 do
 {
  xc=0;
  do
  {
  	if ( field[xc][yc] >=0x41 && field[xc][yc] < 0x41+26) putch(field[xc][yc]);
  	else
   if (field[xc][yc]==MARK )putch('p');
   else if ( field[xc][yc]==UNPASSABLE ) putch('x');
   else if ( field[xc][yc]==PASSABLE )putch(' ');
   else if ( field[xc][yc]== ALREADY_PASSED ) putch('b');
   else if ( field[xc][yc]==GOAL ) putch('g');


   xc++;
  }
  while(xc<FX_SIZE);

  printf("\n");

  yc++;
 }
 while( yc<FY_SIZE );

  getch();
											
											
										   }
											
											
											backsort();
										
  }


}


int main(void)
{
 int x,y;
 char c;

 ID_upcount=0;y=0;
 ID_subtract=0;

 printf("Das Feld hat die Groesse %d mal %d .\n"
        "Leerzeichen fuer Durchgangsweg, x fuer Block und g fuer Ziel...\n"
         ,FX_SIZE, FY_SIZE );
 do
 {
  x=0;
  do
  {
  /*
  	c=' '; // vorgegeben am 20.11.2017
      if ( x==9 && y==11) c='g'; // vorgegeben am 20.11.2017
   */
   do{ c=getch();} while (c!=' ' && c != 'x' && c != 'g' );
   putch(c);
   if(c==' ') field[x][y]=PASSABLE;
        else  if (c=='g' ) field[x][y]=GOAL;
        else  field[x][y]=UNPASSABLE;

   x++;
  }
  while(x<FX_SIZE);

  printf("\n");

  y++;
 }
 while( y<FY_SIZE );

 printf("\nBitte Startkoordinaten X und Y eingeben:\n");
 scanf("%d %d",&x,&y);


 find_path(x,y);

 system("cls\n");
 y=0;
 do
 {
  x=0;
  do
  {
   if (field[x][y]==MARK )putch('p');
   else if ( field[x][y]==UNPASSABLE ) putch('x');
   else if ( field[x][y]==PASSABLE || field[x][y]== ALREADY_PASSED ) putch(' ');
   else if ( field[x][y]==GOAL ) putch('g');


   x++;
  }
  while(x<FX_SIZE);

  printf("\n");

  y++;
 }
 while( y<FY_SIZE );

  getch();
 return (0);
}
