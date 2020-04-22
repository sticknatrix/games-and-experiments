/* Some simple routines for Window handling.
   Yet still in 'development'.
   You give it a string with widgets and then it can
   receive input commands from a cursor and the keys.
   get_input() receives widget changes, you have to
   check for a signal variable and then call the
   routine that you associate with the event.
   Always first put in front the widget that has 'sent'
   the signal . Then use readback() to copy the new
   state into a data structure. With update()
   you should be able to set the signal back
   to zero and also produce text label output
   or create new widgets during runtime.
   Movement of windows could be done when the
   window signals '3' if a certain area is clicked,
   which is 'dispatched' every loop run to a routine
   which polls for mouse cursor movement and locks
   it on window coordinates. '4' is release and
   resizing could be done in a similiar way.
   Display() or so to display the entire stacked
   structure.
   It doesn't work with multitasking or queueing.
   If you want to write some crappy DOS apps,
   you might find it useful.
   
   The basic idea was to get the input elements of a certain application
   always in front when you need them. So you display them with a resorted stacked list.
   Also, after the first elements were drawn, the drawn parts of the screen get fixated with a
   mask when going the stack downwards.
   The mouse and keyboard input can be catched through the first elements from the left and
   this gets only overwritten when the stack depth gets deeper.
   It can either be catched just that they don't go to elements outside of the focus or for
   actual input. Elements could change input mode by setting certain values.
   For carets, topmost and modal windows, the application programmer has to do certain things
   by himself. For example, topmost windows would be in front right before the display routine
   call and then sorted back to their old position which is saved in the application.
   It is important that the application always knows how the GUI looks like and therefore it
   hangs until upwardly an update was done so it is still consistent.
   If there was timer interrupt triggerd multitasking and no main loop, this perhaps
   also could fail.
   Input from the user may get lost. However, printer and modem are more annoying.
   It may also be necessary to remove messages for removed windows in a loop when they are
   closed.
   
   To simulate the full multitasking version with C, you have to write a bytecode emulator
   that executes several code sequences at once. Writing a bytecode emulator is quite simple (however,
   a compiler for it is much harder) and you have to simply execute one instruction of each
   through it's calls in the main loop run.
   CLI and STI are important instructions. However, they may not be needed here.

   In that main loop, you could also insert a message queue and a wrapper for the window routines.
   It would be a linked list. When a semaphore in the window structure is 1, it copies the state
   into a message structure, inserts it into the list and sets the semaphore to 0. Then it examines
  the memory of a script. Call it ABI. When a certain byte in the memory is 0, the message structure
   gets copied into a certain area of the memory of the script and the byte is set to 1. This will also include
   other information like some sort of "hwnd" handle. The script would then call some sort of GetMessage() or
   PeekMessage(). The message is then removed from the list.
   The script can then catch that info and process the user command. The other way, inside the memory of the
   script another byte is set and the command for window change is appended somewhere, the message routine in the
   main loop sets the byte to 0 and puts the message with the data needed into the linked list and tries
   to send it to the GUI wrapper when it is ready. The GUI wrapper would then update the window structure with
   some of the routines below.
   Timer-triggered multitasking would require CLI and STI on critical points.   

   The wrapper could poll the timer function and that way, it could send double clicks by bundling two
    clicks together into a new message.
  Watch out for layer=framedepth-1 . Needs to be fixed?
      
 
  */
   
   
   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*
   Ein einfacher Steuersequenzsstring stellt Fenster dar.
   Die Sequenzen werden mit einem Stapel abgearbeitet und zur Laufzeit umgebaut.
   Es gibt eine Update-Semaphore, die kenntlich macht, dass das betreffende Element
   veraendert wurde.
   Die Hervorhebung ergibt sich aus der Reihenfolge im Stapel.
   Ist das erste Element markiert, wird der Rest nicht mehr verarbeitet.
   Die Verarbeitung zielt dann nur noch auf die Beendigung der Sequenz.

   Cursor bewegen mit h,j,k,u und klicken mit l,
   Konsolenfenster fuer Anzeige gross stellen
*/
unsigned char screen[160][120];
unsigned char windStr[1024];

struct
{
  int x,y;
  long int xmax, ymax;
  int blocked;
   unsigned int seqstart, seqend;  
   signed int firstheap;
} framestack[20];
 signed int framedepth=0;
unsigned int pixelbuf[20000][4];
unsigned int pixcount;
 unsigned char screenz[160][120];


/*
  Anwendung muss das Nachvorneholen selber initiieren
  Elemente muessen durch Identifier gezielt angesprochen werden
  Datenfeld muss als Pointer abgeelgt werden
  Eingabekommando muss in Fensterelement gespeichert werden, sodass Anwendung
  die Positionierung des Eingabecursors und das Einfuegen des Textes uebernehmen kann
  evtl. Updatesperre
  Löchsen und Erzeugen von Elementen?
 
    */ 
    
    /*
      Wird ein Fenster neu positioniert, oder wollte das Fenstersystem eine Animation
      beim Minimirern oder auch nur beim Radiobuttonaendern zeigen, muss das Aendern eines Objekts,
	  das auch ausserdem nicht ganz vorne sein muss, durch einen Routinenaufruf des Fenstersystems
      erfolgen, der nicht einfach nur direkt die Werte aendert?
      Diese Routine gibt den Erfolgsstatus an die Anwendung zurueck?
	 */

/* Ich könnte unten nochmal update() aufrufen und so etwas Tipparbeit sparen? 
 */


typedef
struct
{
 unsigned char type;
 unsigned char varname[255];
 unsigned char text[255];
 int x,y;
 int xabs, yabs;
 int xmaxabs;
 long int ymaxabs;
 long int xmax,ymax;
 int command;
 int blocked;
 long int param;
 int visible;
 int visible_abs;
	
} obj_state;


void buildstring(char *string, obj_state *state)
{
	
	unsigned char buf[255];
 if ( state->type=='w')
 {
  strcpy(string,"w");
  strcat(string,":");
  strcat(string,state->varname);
  strcat(string,":");
  strcat(string,state->text);
  strcat(string,":");
  
  itoa(state->xabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
  
  itoa(state->yabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
  
  
  itoa(state->xmaxabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 
  
  itoa(state->ymaxabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 
  itoa(state->blocked,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 
 
  itoa(state->command,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 
 
 	
  }
  else if ( state->type=='b')
  {
  strcpy(string,"b");
  
  strcat(string,":");
  strcat(string,state->varname);
  strcat(string,":");
  strcat(string,state->text);
  strcat(string,":");
  
  itoa(state->xabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
  
  itoa(state->yabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
  
  
  itoa(state->xmaxabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 
  
  itoa(state->ymaxabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 
  itoa(state->blocked,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 
 
  itoa(state->command,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 	
 	
  	
  }
  else if ( state->type=='t')
  {
   strcpy(string,"t");
   
  strcat(string,":");
  strcat(string,state->varname);
  strcat(string,":");
  strcat(string,state->text);
  strcat(string,":");
  
  itoa(state->xabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
  
  itoa(state->yabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
  
  
  itoa(state->xmaxabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 
  
  itoa(state->ymaxabs,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 
  itoa(state->blocked,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 
 
  itoa(state->command,buf,10),
  strcat(string,buf);	
  strcat(string,":");
 	
 
  	
  	
  }	
	
}

void disp_edit_text(obj_state *edfield, signed int z)
{
 long int len;
 len=0;
 
 while (  edfield->x+len < edfield->xmax && *(unsigned char*)(edfield->ymax+len)!='\0' && len < 9)
 ppix(edfield->x+len, edfield->y, *(unsigned char *)(edfield->ymax+len),z), len++;
	while ( len < 10) ppix(edfield->x+len, edfield->y,219,z), len++;
	
	
}

unsigned char get_edit_pos(obj_state *edfield, int x, int y )
{
  int len;
  len=0;
 if ( edfield->y== y && edfield->x <= x )
 {
 	while ( 1)
 	{
         printf("\aEdfield x ist %d, x ist %d, y Edfield y ist %d, y ist %d",edfield->x, x,edfield->y,y ), getch();
 	 if ( len > 9 || edfield->x+len >= edfield->xmax) return 'F' ;
 	 if ( edfield->x+len ==x) return len+0x30;
	  len++;
	 
	  if ( *(unsigned char *)(edfield->ymax+len)=='\0' ) return 'F';
    }
  }	
  else return 'F';
 
}


// andere Routinen koennen readback() und update() aufrufen, spart Tipperei

 
void ppix(unsigned int x, unsigned int y, unsigned char value, unsigned int z)
{
 if ( screenz[x][y]!=255)
 {
  pixelbuf[pixcount][0]=x,
  pixelbuf[pixcount][1]=y,
  pixelbuf[pixcount][2]=value;
  pixelbuf[pixcount][3]=z;
  screenz[x][y]=z;
     
     pixcount++;
}
    
}
void releasescreenblock(unsigned int z)
{
 int x=0, y=0;
 
 while ( y < 120)
 {
  x=0;
  while ( x < 160)
  {
      if ( screenz[x][y]==z) screenz[x][y]=255;
      x++;
  }
  
  y++;
 }    
    
}

unsigned int cursor[2];





void disp_elements(void )
{
 unsigned int type;
 unsigned char text[32];
 unsigned char varname[32];
 unsigned int n2=0;
 unsigned int x,y,xmax,ymax; 
 unsigned int blocked;
 unsigned int updateSemaph;
   unsigned char buf[5];
  unsigned int entry_p, exit_p;
 int n=0;
 entry_p=0;
 
 
 ppix(cursor[0],cursor[1],'X',1);
 releasescreenblock(1);
 
 while ( 1 )
 {
   type=windStr[n];
     
     if ( type=='e')
     {
         framedepth--;
     
        n++;
       exit_p=n;
       framestack[framedepth].seqend=exit_p;
        releasescreenblock(framedepth+1);
        
         if ( framedepth==0) return;
     
     }
     
    if ( type=='w' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
       
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     
         x=framestack[framedepth].x;
         y=framestack[framedepth].y;
         
     framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax < xmax+x ?
                                     framestack[framedepth-1].xmax : xmax+x );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax < ymax+y ?
                                     framestack[framedepth-1].ymax : ymax+y );
                                   
     framestack[framedepth].seqstart=entry_p;   
        
         xmax=framestack[framedepth].xmax;
         ymax=framestack[framedepth].ymax;
             // max irrelevant?
      framedepth++;
             
             printf("%s %d %d %d %d\n",varname,x,y,xmax,ymax), getch();
             
     
      unsigned int drawX, drawY;
       
       drawX=x+1;
       while ( drawX < xmax )
       {
        drawY=y+1;
        while ( drawY < ymax )
        {
         ppix(drawX,drawY,' ',framedepth);
           drawY++;
       
         }
         drawX++;
       }
       
       
       drawX=x;
       while ( drawX < xmax )
       {
            ppix(drawX,y,'#',framedepth);
         
          ppix(drawX,ymax,'#',framedepth), drawX++;
         
       }
         
 
       drawY=y;
       while ( drawY < ymax+1 )
       {
         ppix(x,drawY,'#',framedepth);
         
          ppix(xmax,drawY,'#',framedepth), drawY++;
    
       }
       
        n2=0;
        while(n2 < strlen(text) && n2+x < xmax ) ppix(x+1+n2,y,text[n2],framedepth), n2++;      
       
       
    }
    

  if ( type=='b' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
       
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
         
		 x=framestack[framedepth].x;
         y=framestack[framedepth].y;
         
	 
	 framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax < xmax+x ?
                                     framestack[framedepth-1].xmax : xmax+x );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax < ymax+y ?
                                     framestack[framedepth-1].ymax : ymax+y );
                                   
     framestack[framedepth].seqstart=entry_p;   
        
         xmax=framestack[framedepth].xmax;
         ymax=framestack[framedepth].ymax;
      framedepth++;
     
             // max irrelevant?
             
     
      unsigned int drawX, drawY;
      
       
       drawX=x;
       while ( drawX < xmax )
       {
        drawY=y;
        while ( drawY < ymax )
        {
          ppix(drawX,drawY,'*',framedepth);
           drawY++;
       
         }
         drawX++;
       }
 
             n2=0;
        while(n2 < strlen(text) ) ppix(x+1+n2,y,text[n2],framedepth), n2++;      
       
    }
  

if ( type=='t' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
       
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     
        x=framestack[framedepth].x;
        y=framestack[framedepth].y;
      
	 
	 framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax < xmax+x ?
                                     framestack[framedepth-1].xmax : xmax+x );
    
           /*
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax < y+1 ?
                                     framestack[framedepth-1].ymax  : y+1 );
                         */
     framestack[framedepth].seqstart=entry_p;   
        
      framedepth++;
     
             // max irrelevant?
             
     
      unsigned int drawX, drawY;
      
      obj_state edfield;
      edfield.x=x, edfield.y=y,
      edfield.xmax=xmax+x;
      edfield.ymax=ymax;
      
       
       drawX=x;
       drawY=y; if ( framestack[framedepth-1].ymax > framestack[framedepth-1].y)
        disp_edit_text(&edfield, framedepth);  
    }
  


 
  }

}



void clearScreen()
{
  int x=0, y=0;
  while ( y < 80 )
  {
   x=0;
   while ( x < 160 )
   {
    screen[x][y]=' ';
    screenz[x][y]=0;
    x++;
   }
  y++;
  }
}






int readback(char *identifier, obj_state *status)
{
	

     
 unsigned int type;
 unsigned char text[32];
 unsigned char varname[32];
 unsigned int n2=0;
 unsigned int x,y,xmax,ymax; 
 unsigned int blocked;
 unsigned int updateSemaph;
   unsigned char buf[5];
  unsigned int entry_p, exit_p;
    int weisdown, layer;
    unsigned int xabs, yabs;
      
 int n=0;
 weisdown=0;
 
 signed int framepos;
 unsigned char identifiers[10][255];
 unsigned int ident_len=0;
 n=0;
 
 identifiers[0][0]='\0';
 while ( identifier[n2]!='\0')
 {
  identifiers[ident_len][n]=identifier[n2];
  n2++ ,n++;
  if (identifier[n2]==':' || identifier[n2]=='\0')
  {
  	identifiers[ident_len][n]='\0';
  	ident_len++;
  	if ( identifier[n2]=='\0') break; else n2++;
	 n=0;
  }
 	
 }
 
 
 printf("%s suche ich", identifiers[0]);
 
 n2=0;
 
 framepos=-1;
 
  n=1;
  framestack[0].firstheap=0;
  
 while ( n < 20)
 {
     
     framestack[n].firstheap=-1;
    n++;
 }
 
 unsigned int ignore;
 ignore=0;
 layer=0;
 n=0;
 
 while ( 1 )
 {
   type=windStr[n];
     
     if ( type=='e')
     {
         framedepth--;
     
        n++;
       exit_p=n;
       framestack[framedepth].seqend=exit_p;
       
         
         if ( weisdown==1&& framedepth <= layer
              && layer==ident_len) ignore=1;
         if ( framedepth==0) break;
     
     }
     //else weisdown=0;
     
    if ( type=='w' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
       
       if ( ignore==0)
       {
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax < xmax ?
                                     framestack[framedepth-1].xmax : xmax );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax < ymax ?
                                     framestack[framedepth-1].ymax : ymax );
                                   
     framestack[framedepth].seqstart=entry_p;   
     framestack[framedepth].firstheap=n;
       
       xabs=x, yabs=y;
       
         x=framestack[framedepth].x;
         y=framestack[framedepth].y;
             // max irrelevant?
     framedepth++;
              
     
      unsigned int drawX, drawY;
       
       drawX=x;
       
       
        if ( layer <= ident_len && framedepth-1 <= ident_len&& framedepth-2==layer)
        if ( strcmp(identifiers[layer],varname)==0 )
       {
             //windStr[n-2]='1'; //Semaphore gesetzt
          
          status->type='w';
		  strcpy ( status->text, text), strcpy( status->varname, varname);
		  status->x=x, status->y=y,
		  status->xmax=xmax, status->ymax=ymax,
		  status->xabs=xabs, status->yabs=yabs;
	   	  
		  status->blocked=blocked, status->command=updateSemaph;

          
         // printf(">>%s geblickt<<", varname), getch();
          
           weisdown=1;
           layer=framedepth-1;
           framepos=framedepth-1;
           
       }
       
        }
       //else
       //framedepth--;
          //printf("%s %d %d %d %d %d\n",varname, x,y,xmax,ymax, weisdown), getch();
      
       
    }
    

  if ( type=='b' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      
	  n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
    
    if ( ignore==0)
    {   
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax+x < xmax ?
                                     framestack[framedepth-1].xmax+x : xmax );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax+y < ymax ?
                                     framestack[framedepth-1].ymax+y : ymax );
                                   
     framestack[framedepth].seqstart=entry_p;   
      framestack[framedepth].firstheap=n;
        
		xabs=x, yabs=y;    
        
         x=framestack[framedepth].x;
         y=framestack[framedepth].y;
             // max irrelevant?
                     
     framedepth++;
     
     
      unsigned int drawX, drawY;
      
        if ( layer <= ident_len && framedepth-1 <= ident_len&& framedepth-2==layer)
        if ( strcmp(identifiers[layer],varname)==0 )
       {
              // windStr[n-2]='1'; //Semaphore gesetzt
      	  
      	  status->type='b';
	   	  strcpy ( status->text, text), strcpy( status->varname, varname);
		  status->x=x, status->y=y,
		  status->xmax=xmax, status->ymax=ymax,
		  status->xabs=xabs, status->yabs=yabs;
	   	  
		  status->blocked=blocked, status->command=updateSemaph;
  
	       //printf("Button geklickt"), getch();
           framepos=framedepth-1;
           layer=framedepth-1;
           weisdown=1;
       }
       //else
       //framedepth--;
       
     }
         
           
    }
    
    
  if ( type=='t' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      
	  n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
    
    if ( ignore==0)
    {   
     unsigned long xmaxabs, ymaxabs;
     
      xmaxabs=xmax, ymaxabs=ymax;
      
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax+x < xmax ?
                                     framestack[framedepth-1].xmax+x : xmax );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax+y < ymax ?
                                     framestack[framedepth-1].ymax+y : ymax );
                                   
     framestack[framedepth].seqstart=entry_p;   
      framestack[framedepth].firstheap=n;
        
		xabs=x, yabs=y;    
        
         x=framestack[framedepth].x;
         y=framestack[framedepth].y;
             // max irrelevant?
                     
     framedepth++;
     
     
      unsigned int drawX, drawY;
      
        if ( layer <= ident_len && framedepth-1 <= ident_len&& framedepth-2==layer)
        if ( strcmp(identifiers[layer],varname)==0 )
       {
              // windStr[n-2]='1'; //Semaphore gesetzt
      	  
      	  status->type='t';
	   	  strcpy ( status->text, text), strcpy( status->varname, varname);
		  status->x=x, status->y=y,
		  status->xmax=xmax, status->ymax=ymax,
		  status->xmaxabs=xmaxabs, status->ymaxabs=ymaxabs;
	   	  
		  status->blocked=blocked-0x30, status->command=updateSemaph-0x30;
  
	       //printf("Button geklickt"), getch();
           framepos=framedepth-1;
           layer=framedepth-1;
           weisdown=1;
       }
 
    }
   
   }

  }
     
     
 
      
    // windStr[n-3]='1';
           
   //framestack[framedepth].seqend=n;
   
   
   if ( weisdown==0 ) return 1;
   if ( layer!=ident_len) return 1;
   
   return 0;
	
	
	
}





int putinfront(char *identifier, signed int layers) // wenn -1, voll
{
	 
     
 unsigned int type;
 unsigned char text[32];
 unsigned char varname[32];
 unsigned int n2=0;
 unsigned int x,y,xmax,ymax; 
 unsigned int blocked;
 unsigned int updateSemaph;
   unsigned char buf[5];
  unsigned int entry_p, exit_p;
    int weisdown, layer;
 int n=0;
 weisdown=0;
 
 signed int framepos;
 unsigned char identifiers[10][255];
 unsigned int ident_len=0;
 n=0;
 
 identifiers[0][0]='\0';
 while ( identifier[n2]!='\0')
 {
  identifiers[ident_len][n]=identifier[n2];
  n2++ ,n++;
  if (identifier[n2]==':' || identifier[n2]=='\0')
  {
  	identifiers[ident_len][n]='\0';
  	ident_len++;
  	if ( identifier[n2]=='\0') break; else n2++;
	 n=0;
  }
 	
 }
 
 n2=0;
 
 framepos=-1;
 
  n=1;
  framestack[0].firstheap=0;
  
 while ( n < 20)
 {
     
     framestack[n].firstheap=-1;
    n++;
 }
 
 unsigned int ignore;
 ignore=0;
 layer=0;
 n=0;
 
// printf("Gehe in putinfront()'s Hauptschleife"), getch();
 
 while ( 1 )
 {
   type=windStr[n];
     
     if ( type=='e')
     {
         framedepth--;
     
        n++;
       exit_p=n;
       framestack[framedepth].seqend=exit_p;
       
         
         if ( weisdown==1&& framedepth <= layer
              && layer==ident_len) ignore=1;
         if ( framedepth==0) break;
     
     }
     //else weisdown=0;
     
    if ( type=='w' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
       
       if ( ignore==0)
       {
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax < xmax ?
                                     framestack[framedepth-1].xmax : xmax );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax < ymax ?
                                     framestack[framedepth-1].ymax : ymax );
                                   
     framestack[framedepth].seqstart=entry_p;   
     framestack[framedepth].firstheap=n;
       
         x=framestack[framedepth].x;
         y=framestack[framedepth].y;
             // max irrelevant?
     framedepth++;
              
     
      unsigned int drawX, drawY;
       
       drawX=x;
       
       
        if ( layer <= ident_len && framedepth-1 <= ident_len&& framedepth-2==layer)
        if ( strcmp(identifiers[layer],varname)==0 )
       {
             //windStr[n-2]='1'; //Semaphore gesetzt
          
         // printf(">>%s geklickt<<", varname), getch();
          
           weisdown=1;
           layer=framedepth-1;
           framepos=framedepth-1;
           
       }
       
        }
       //else
       //framedepth--;
        //  printf("%s %d %d %d %d %d\n",varname, x,y,xmax,ymax, weisdown), getch();
      
       
    }
    

  if ( type=='b' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      
	  n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
    
    if ( ignore==0)
    {   
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax+x < xmax ?
                                     framestack[framedepth-1].xmax+x : xmax );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax+y < ymax ?
                                     framestack[framedepth-1].ymax+y : ymax );
                                   
     framestack[framedepth].seqstart=entry_p;   
      framestack[framedepth].firstheap=n;
    
        
         x=framestack[framedepth].x;
         y=framestack[framedepth].y;
             // max irrelevant?
                     
     framedepth++;
     
     
      unsigned int drawX, drawY;
      
        if ( layer <= ident_len && framedepth-1 <= ident_len&& framedepth-2==layer)
        if ( strcmp(identifiers[layer],varname)==0 )
       {
              // windStr[n-2]='1'; //Semaphore gesetzt
      
	  
	      // printf("Button geklickt"), getch();
           framepos=framedepth-1;
           layer=framedepth-1;
           weisdown=1;
       }
       //else
       //framedepth--;
       
     }
         
           
    }
    
    
    
    
    
  if ( type=='t' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      
	  n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
    
    if ( ignore==0)
    {   
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax+x < xmax ?
                                     framestack[framedepth-1].xmax+x : xmax );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax+y < ymax ?
                                     framestack[framedepth-1].ymax+y : ymax );
                                   
     framestack[framedepth].seqstart=entry_p;   
      framestack[framedepth].firstheap=n;
    
        
         x=framestack[framedepth].x;
         y=framestack[framedepth].y;
             // max irrelevant?
                     
     framedepth++;
     
     
      unsigned int drawX, drawY;
      
        if ( layer <= ident_len && framedepth-1 <= ident_len&& framedepth-2==layer)
        if ( strcmp(identifiers[layer],varname)==0 )
       {
              // windStr[n-2]='1'; //Semaphore gesetzt
      
	  
	      // printf("Button geklickt"), getch();
           framepos=framedepth-1;
           layer=framedepth-1;
           weisdown=1;
       }
       //else
       //framedepth--;
       
     }
         
           
    }
    
    
    
    
 
  }
     
     
 
      
    // windStr[n-3]='1';
           
   //framestack[framedepth].seqend=n;
   
   
   if ( weisdown==0 ) return 1;
   if ( layer!=ident_len) return 1;
   
   
   framedepth=framepos;
   unsigned char copied[1024];
   
   
   unsigned int length;
   
   while ( framedepth > 0)
   {
        
       
       if ( framestack[framedepth].seqend-framestack[framedepth].seqstart >= 0 )
   {
           strncpy(copied,windStr+framestack[framedepth].seqstart,framestack[framedepth].seqend-framestack[framedepth].seqstart);
        copied[framestack[framedepth].seqend-framestack[framedepth].seqstart]='\0';
      }
       else copied[0]='\0';
       
       length=strlen(windStr);
       strcpy(windStr+framestack[framedepth].seqstart, windStr+framestack[framedepth].seqend);
       memcpy(windStr+framestack[framedepth-1].firstheap+strlen(copied), windStr+framestack[framedepth-1].firstheap, length);
    memcpy(windStr+framestack[framedepth-1].firstheap, copied, strlen(copied));
       
       
      //  printf("app:%s\ncopied:%s" , windStr,copied ), getch();
       
       
       
       
       framedepth--;
   }
	
  return 0;	
}








#define CHANGE 0
#define INSERT 1
#define DELETE 2

int update(unsigned char *identifier, obj_state *status, int chinode)
{
	unsigned int type;
 unsigned char text[32];
 unsigned char varname[32];
 unsigned int n2=0;
 unsigned long int x,y,xmax,ymax; 
 unsigned int blocked;
 unsigned int updateSemaph;
   unsigned char buf[5];
  unsigned int entry_p, exit_p;
    int weisdown, layer;
    unsigned int xabs, yabs;
      
 int n=0;
 weisdown=0;
 
 signed int framepos;
 unsigned char identifiers[10][255];
 unsigned int ident_len=0;
 n=0;
 
 identifiers[0][0]='\0';
 while ( identifier[n2]!='\0')
 {
  identifiers[ident_len][n]=identifier[n2];
  n2++ ,n++;
  if (identifier[n2]==':' || identifier[n2]=='\0')
  {
  	identifiers[ident_len][n]='\0';
  	ident_len++;
  	if ( identifier[n2]=='\0') break; else n2++;
	 n=0;
  }
 	
 } 
 
 n2=0;
 
 framepos=-1;
 
  n=1;
  framestack[0].firstheap=0;
  
 while ( n < 20)
 {
     
     framestack[n].firstheap=-1;
    n++;
 }
 
 unsigned int ignore;
 ignore=0;
 layer=0;
 n=0;
 
 while ( 1 )
 {
   type=windStr[n];
     
     if ( type=='e')
     {
         framedepth--;
     
        n++;
       exit_p=n;
       framestack[framedepth].seqend=exit_p;
       
         
         if ( weisdown==1&& framedepth <= layer &&
              layer==ident_len ) ignore=1;
         if ( framedepth==0) break;
     
     }
     //else weisdown=0;
     
    if ( type=='w' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
       
       if ( ignore==0)
       {
       	
      unsigned int xmaxabs, ymaxabs;
    	xabs=x, yabs=y;    
        xmaxabs=xmax, ymaxabs=ymax;  
    
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax < xmax ?
                                     framestack[framedepth-1].xmax : xmax );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax < ymax ?
                                     framestack[framedepth-1].ymax : ymax );
                                   
     framestack[framedepth].seqstart=entry_p;   
     framestack[framedepth].firstheap=n;
       
       
         x=framestack[framedepth].x;
         y=framestack[framedepth].y;
             // max irrelevant?
     framedepth++;
              
     
      unsigned int drawX, drawY;
       
       drawX=x;
       
       
        if ( layer <= ident_len && framedepth-1 <= ident_len&& framedepth-2==layer)
        if ( strcmp(identifiers[layer],varname)==0 )
       {
             //windStr[n-2]='1'; //Semaphore gesetzt
         if ( chinode!= CHANGE && chinode!=INSERT)
		 { 
          status->type='w';
		  strcpy ( status->text, text), strcpy( status->varname, varname);
		  status->x=x, status->y=y,
		  status->xmax=xmaxabs, status->ymax=ymaxabs,
		  status->xabs=xabs, status->yabs=yabs;
	   	  
		  status->blocked=blocked, status->command=updateSemaph;
         }
          
         // printf(">>%s geklickt<<", varname), getch();
          
           weisdown=1;
           layer=framedepth-1;
           framepos=framedepth-1;
           
       }
       
        }
       //else
       //framedepth--;
        //  printf("%s %d %d %d %d %d\n",varname, x,y,xmax,ymax, weisdown), getch();
      
       
    }
    

  if ( type=='b' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      
	  n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
    
    if ( ignore==0)
    {   
    
      unsigned int xmaxabs, ymaxabs;
    	xabs=x, yabs=y;    
        xmaxabs=xmax, ymaxabs=ymax;  
    
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax+x < xmax ?
                                     framestack[framedepth-1].xmax+x : xmax );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax+y < ymax ?
                                     framestack[framedepth-1].ymax+y : ymax );
                                   
     framestack[framedepth].seqstart=entry_p;   
      framestack[framedepth].firstheap=n;
        
	  
         x=framestack[framedepth].x;
         y=framestack[framedepth].y;
             // max irrelevant?
                     
     framedepth++;
     
     
      unsigned int drawX, drawY;
      
        if ( layer <= ident_len && framedepth-1 <= ident_len&& framedepth-2==layer)
        if ( strcmp(identifiers[layer],varname)==0 )
       {
              // windStr[n-2]='1'; //Semaphore gesetzt
      	  
      	  if ( chinode!= CHANGE && chinode != INSERT)
      	  {
			
      	  status->type='b';
      	  
		  strcpy ( status->text, text), strcpy( status->varname, varname);
		  status->x=x, status->y=y,
		  status->xmax=xmaxabs, status->ymax=ymaxabs,
		  status->xabs=xabs, status->yabs=yabs;
	   	  
		  status->blocked=blocked, status->command=updateSemaph;
         }
         
	      // printf("Button geklickt"), getch();
           framepos=framedepth-1;
           layer=framedepth-1;
           weisdown=1;
       }
       //else
       //framedepth--;
       
     }
         
           
    }
    
   
   
  if ( type=='t' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      
	  n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
    
    if ( ignore==0)
    {   
    
      unsigned long int xmaxabs, ymaxabs;
    	xabs=x, yabs=y;    
        xmaxabs=xmax, ymaxabs=ymax;  
    
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax+x < xmax ?
                                     framestack[framedepth-1].xmax+x : xmax );
                                     
     framestack[framedepth].ymax=ymax; //( framestack[framedepth-1].ymax+y < ymax ?
                                      //  framestack[framedepth-1].ymax+y : ymax );
                                   
     framestack[framedepth].seqstart=entry_p;   
      framestack[framedepth].firstheap=n;
        
	  
         x=framestack[framedepth].x;
         y=framestack[framedepth].y;
             // max irrelevant?
                     
     framedepth++;
     
     
      unsigned int drawX, drawY;
      
        if ( layer <= ident_len && framedepth-1 <= ident_len&& framedepth-2==layer)
        if ( strcmp(identifiers[layer],varname)==0 )
       {
              // windStr[n-2]='1'; //Semaphore gesetzt
      	  
      	  if ( chinode!= CHANGE && chinode != INSERT)
      	  {
			
      	  status->type='t';
      	  
		  strcpy ( status->text, text), strcpy( status->varname, varname);
		  status->x=x, status->y=y,
		  status->xmax=xmaxabs, status->ymax=ymaxabs,
		  status->xabs=xabs, status->yabs=yabs;
	   	  
		  status->blocked=blocked, status->command=updateSemaph;
         }
         
	      // printf("Button geklickt"), getch();
           framepos=framedepth-1;
           layer=framedepth-1;
           weisdown=1;
       }
       //else
       //framedepth--;
       
     }
         
           
    }
    
   
   
   
   }
    
    
   if ( weisdown==0 ) return 1;
   if ( layer!=ident_len) return 1;
   
   
   framedepth=framepos;
   unsigned char copied[1024];
   
      
      unsigned char converted[255];
      
	  copied[0]='\0';
	 
   if ( chinode==CHANGE|| chinode==INSERT)
   {
	  buildstring(copied, status);
	  
	  
	                    // schauen, ob noch eine Aufwerfung folgt!!
     }
    
       unsigned char windBak[1024];  
     
     if ( chinode==DELETE)
     {
    
       strcpy ( windBak,windStr+framestack[layer].seqend);
   
   
   
       strcpy(windStr+framestack[layer].seqstart, copied);
       strcat(windStr, windBak );
      
     }
     else
	 if ( chinode== INSERT )
	 {
	  strcat(copied,"e"); // das muss nicht sein, da nicht zwingend vorne!
      strcpy ( windBak,windStr+framestack[layer].firstheap);
      strcpy(windStr+framestack[layer].firstheap, copied);
      strcat(windStr, windBak );
      
	 }
     else
	 if ( chinode== CHANGE )
	 {
	  strcpy ( windBak,windStr+framestack[layer].firstheap);
      strcpy(windStr+framestack[layer].seqstart, copied);
      strcat(windStr, windBak );
      
	 }
     
       
       // printf("app:%s\ncopied:%s" , windStr,copied ), getch();
   
	
  return 0;	
    
    
     
      
    // windStr[n-3]='1';
           
   //framestack[framedepth].seqend=n;
   
   
	
}









void get_input(void)
{
    
 unsigned int cmd=0;
 obj_state obj_input;
 unsigned char c;
 
 
 if ( kbhit())
 switch ( (c=getch()) )
 {
  case 'u': if ( cursor[1]> 0) cursor[1]--; break;    
 case 'j': if ( cursor[1]< 80) cursor[1]++; break;    
  case 'h': if ( cursor[0]> 0) cursor[0]--; break;    
 case 'k': if ( cursor[0]< 160) cursor[0]++; break;    
 case 'l': cmd=1;
 default: if ( c >='A' && c <= 'Z') cmd=2; break;
 
 }
 
 if ( cmd==1||cmd==2)
 {
     
     
     
 unsigned int type;
 unsigned char text[32];
 unsigned char varname[32];
 unsigned int n2=0;
 unsigned int x,y,xmax,ymax; 
 unsigned int blocked;
 unsigned int updateSemaph;
   unsigned char buf[5];
  unsigned int entry_p, exit_p;
    int weisdown, layer;
 int n=0;
 weisdown=0;
 
 signed int framepos;
 
 framepos=-1;
 
  n=1;
  framestack[0].firstheap=0;
  
 while ( n < 20)
 {
     
     framestack[n].firstheap=-1;
    n++;
 }
 
 unsigned int ignore;
 ignore=0;
 layer=0;
 n=0;
 
 while ( 1 )
 {
   type=windStr[n];
     
     if ( type=='e')
     {
         framedepth--;
     
        n++;
       exit_p=n;
       framestack[framedepth].seqend=exit_p;
       
         
         if ( weisdown==1&& framedepth < layer) ignore=1;
         if ( framedepth==0) break;
     
     }
     //else weisdown=0;
     
    if ( type=='w' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
       
       if ( ignore==0)
       {
       	 unsigned int xabs, yabs, xmaxabs, ymaxabs;
         xabs=x, yabs=y, xmaxabs=xmax, ymaxabs=ymax;
      
       	
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     
	  x=framestack[framedepth].x;
       y=framestack[framedepth].y;
        
	 framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax < xmax+x ?
                                     framestack[framedepth-1].xmax : xmax+x );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax < ymax+y ?
                                     framestack[framedepth-1].ymax : ymax+y );
                                   
     framestack[framedepth].seqstart=entry_p;   
     framestack[framedepth].firstheap=n;
       
       
         xmax=framestack[framedepth].xmax;
         ymax=framestack[framedepth].ymax;
         
		     // max irrelevant?
     framedepth++;
              
     
      unsigned int drawX, drawY;
       
       drawX=x;
       
       if ( cursor[0] >= x && cursor[0] <= xmax &&
            cursor[1] >= y && cursor[1] <= ymax+1 && blocked=='0' )
       {
             //windStr[n-2]='1'; //Semaphore gesetzt
          
          
            obj_input.type='w';
          if ( cursor[1]!=y)
		    obj_input.command=1;
		    else
			{
			 if ( updateSemaph!='3')obj_input.command=3;
		     else obj_input.command=4;
		    }
		    
		    strcpy(obj_input.varname, varname);
		    strcpy(obj_input.text,text);
		    obj_input.param=0,
            obj_input.xabs=xabs,
            obj_input.yabs=yabs,
            obj_input.xmaxabs=xmaxabs,
            obj_input.ymaxabs=ymaxabs,
            obj_input.blocked=0;
		
		 // printf("\a");
		 // printf(">>%s geklickt<<", varname), getch();
          
           weisdown=1;
           layer=framedepth;
           framepos=framedepth-1;
           
       }
       else if ( cmd==2)
       {
       	 obj_input.type='w',
        
		    strcpy(obj_input.varname, varname);
		    strcpy(obj_input.text,text);
		    obj_input.command=0; // kein Signal
			obj_input.param=0,
            obj_input.xabs=xabs,
            obj_input.yabs=yabs,
            obj_input.xmaxabs=xmaxabs,
            obj_input.ymaxabs=ymaxabs,
            obj_input.blocked=0;  
           
         framepos=framedepth-1;
         layer=framedepth-1;
          weisdown=1;
	   }
       
        }
       //else
       //framedepth--;
        //  printf("%s %d %d %d %d %d\n",varname, x,y,xmax,ymax, weisdown), getch();
      
       
    }
    

  if ( type=='b' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
    
    if ( ignore==0)
    {   
    
        unsigned int xabs, yabs, xmaxabs, ymaxabs;
         xabs=x, yabs=y, xmaxabs=xmax, ymaxabs=ymax;
      
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
     
        x=framestack[framedepth].x;
         y=framestack[framedepth].y;
	 
	 framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax < xmax+x ?
                                     framestack[framedepth-1].xmax : xmax+x );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].ymax < ymax+y ?
                                     framestack[framedepth-1].ymax : ymax+y );
                                   
     framestack[framedepth].seqstart=entry_p;   
      framestack[framedepth].firstheap=n;
    
        
         xmax=framestack[framedepth].xmax;
         ymax=framestack[framedepth].ymax;
		     // max irrelevant?
                     
     framedepth++;
     
     
      unsigned int drawX, drawY;
       if ( cursor[0] >= x && cursor[0] <= x+xmax &&
            cursor[1] >= y && cursor[1] <= y+ymax&& blocked=='0' )
       {
       	
            obj_input.type='b',
            strcpy(obj_input.varname, varname);
		    strcpy(obj_input.text,text);
		    obj_input.command=1,
            obj_input.param=0,
            obj_input.xabs=xabs,
            obj_input.yabs=yabs,
            obj_input.xmaxabs=xmaxabs,
            obj_input.ymaxabs=ymaxabs,
            obj_input.blocked=0;
            
              // windStr[n-2]='1'; //Semaphore gesetzt
           printf("Button geklickt"), getch();
           framepos=framedepth-1;
           layer=framedepth;
           weisdown=1;
       }
       else if ( cmd==2)
       {
       	 strcpy(obj_input.varname, varname);
		    strcpy(obj_input.text,text);
		    obj_input.command=0, // Signal ignorieren
            obj_input.param=0,
            obj_input.xabs=xabs,
            obj_input.yabs=yabs,
            obj_input.xmaxabs=xmaxabs,
            obj_input.ymaxabs=ymaxabs,
            obj_input.blocked=0;
             
           
         framepos=framedepth-1;
         layer=framedepth;
          weisdown=1;
       	
	   }
       //else
       //framedepth--;
       
     }
  
   }
  
  
  
  
  if ( type=='t' )     
    {
     entry_p=n;
     // type:'varname':'text':x:y:xsize:ysize:blocked:update     
      n++;
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) varname[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
       varname[n2]='\0';       
      n2=0;
      while ( windStr[n]!=':' ) text[n2]=windStr[n], n++, n2++;
      if ( windStr[n]==':' )n++; else return;
      text[n2]='\0';

      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      x=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      y=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      xmax=atoi(buf);
      if ( windStr[n]==':' )n++; else return;
      n2=0;
      while ( windStr[n]!=':' ) buf[n2]=windStr[n], n++, n2++;
      buf[n2]='\0';
      ymax=atoi(buf);
      n++;
      
      blocked=windStr[n]; n++, n++;
      updateSemaph=windStr[n]; n++, n++;
    
    if ( ignore==0)
    {   
    
        unsigned long int xabs, yabs, xmaxabs, ymaxabs;
         xabs=x, yabs=y, xmaxabs=xmax, ymaxabs=ymax;
      
     framestack[framedepth].x=framestack[framedepth-1].x+x;
     framestack[framedepth].y=framestack[framedepth-1].y+y;
      
         x=framestack[framedepth].x;
         y=framestack[framedepth].y;
       
	 framestack[framedepth].xmax=  ( framestack[framedepth-1].xmax < xmax+x ?
                                     framestack[framedepth-1].xmax : xmax+x );
                                     
     framestack[framedepth].ymax=( framestack[framedepth-1].y < xmax+1 ?
                                     framestack[framedepth-1].y : xmax+1 );
            
			                       
     framestack[framedepth].seqstart=entry_p;   
      framestack[framedepth].firstheap=n;
    
         xmax=framestack[framedepth].xmax;
         ymax=framestack[framedepth].ymax;
             // max irrelevant?
                     
     framedepth++;
     
     //printf("Binda\a"), getch();
     
     
       obj_state objbuf; 
     
      objbuf.x=x, objbuf.y=y;
      objbuf.xmax=xmax+x, objbuf.ymax=ymax;
     
      unsigned int drawX, drawY;
      
      if (framestack[framedepth-1].ymax > framestack[framedepth-1].y)
      {
	   if ( cmd==1 && 
	        (text[1]=get_edit_pos(&objbuf,cursor[0],cursor[1])) != 'F' )
	   {
       	
            obj_input.type='t',
            strcpy(obj_input.varname, varname);
		    strcpy(obj_input.text,text);
			obj_input.command=cmd,
            obj_input.xabs=xabs,
            obj_input.yabs=yabs,
            obj_input.xmaxabs=xmaxabs,
            obj_input.ymaxabs=ymaxabs,
            obj_input.blocked=0;
            
              // windStr[n-2]='1'; //Semaphore gesetzt
           printf("Editierfeld angeklickt"), getch();
           framepos=framedepth-1;
           layer=framedepth;
           weisdown=1;
       }
       else if ( cmd==2)
       {
       	 
		 obj_input.type='t',
         
		 
		 
		    strcpy(obj_input.varname, varname);
		    strcpy(obj_input.text,text);
			obj_input.command=cmd,
            obj_input.xabs=xabs,
            obj_input.yabs=yabs,
            obj_input.xmaxabs=xmaxabs,
            obj_input.ymaxabs=ymaxabs,
            obj_input.blocked=0;
         
		 
		 
		 
		 obj_input.command=cmd,
         obj_input.text[0]=c;
          
          printf("\a");
           
         framepos=framedepth-1;
         layer=framedepth;
          weisdown=1;
       	
	   }
	   
       }
       //else
       //framedepth--;
       
     }
  
  
   }
           
    
 
  }
     
     if ( weisdown==1)
     {
      unsigned char strconstr[255];	
      unsigned char copied[1024];
       buildstring(strconstr, &obj_input);
     
     
    // printf(">>>>>>%s<<<<", strconstr), getch();
	  	  strncpy(copied,windStr+framestack[layer-1].firstheap,strlen(windStr)-framestack[layer-1].firstheap);
         copied[strlen(windStr)-framestack[layer-1].firstheap]='\0';
        memcpy(windStr+framestack[layer-1].seqstart, strconstr, strlen(strconstr)+1);
        strcat(windStr,copied);
	 	
	 }
    
}
}




void displayScreen(void)
{
  system("cls\n");
  int x=0, y=0;
  unsigned char screenz[160][120];
  
  y=0;
  while ( y < 120)
  {
      x=0;
      while ( x < 160)
      {
        screenz[x][y]=0;    
          x++;
      }
      y++;
  }
   x=0,y=0;
    unsigned int n=0;
   while ( n < pixcount)
   {
       //if (screenz[pixelbuf[pixcount][0]][pixelbuf[pixcount][1]] <= pixelbuf[pixcount][3])
    screen[pixelbuf[n][0]][pixelbuf[n][1]]=pixelbuf[n][2]  ; // ,
    n++;
       //screenz[pixelbuf[pixcount][0]][pixelbuf[pixcount][1]]=pixelbuf[pixcount][3];
 
   }
  while ( y < 80 )
  {
   x=0;
   while ( x < 158 )
   {
    printf("%c%c%c%c%c%c%c%c", screen[x][y], screen[x+1][y], screen[x+2][y], screen[x+3][y],
                                screen[x+4][y], screen[x+5][y], screen[x+6][y], screen[x+7][y]);
    x+=8;
   }
    printf("\n");

  y++;
  }
}





int main(void)
{ 
 
 clock_t delay;
 
   framestack[0].x=0, framestack[0].y=0,
  framestack[0].xmax=100, framestack[0].ymax=80;
  framestack[0].blocked=0;
  framestack[0].seqstart=0, framestack[0].seqend=strlen(windStr);
 
   cursor[0]=0, cursor[1]=0;

  obj_state teststat;
   unsigned char text[255]; 
   unsigned char textptr[30];
   
   strcpy(text,"Werte");
   
    itoa((long int)text,textptr,10);
 
strcpy(windStr,"w:testfenster:Hello:20:20:30:30:0:0:w:xbutton:XBUTTON:1:1:10:10:0:0:eb:testbutton:OK:5:5:4:3:0:0:eew:test2:Bla:25:27:50:50:0:0:" );    
   strcat(windStr,"t:text:Texten:10:10:7:"),
   strcat(windStr,textptr),
   strcat(windStr,":0:0:eee");
 
 //   printf("%s", (unsigned char *)atoi(textptr)), getch();
   //printf("%s", (unsigned char *)(long int)(text)), getch();
   
   printf("%s",windStr), getch();
  /* 
  while (1 )
  {
      
      delay=clock()+100;
    clearScreen();
   framedepth=1;
 
 
 printf(" %d ", strlen(windStr));
*/ 
   framedepth=1;
  
 
   framestack[0].x=0, framestack[0].y=0,
  framestack[0].xmax=100, framestack[0].ymax=80;
  framestack[0].blocked=0;
  framestack[0].seqstart=0, framestack[0].seqend=strlen(windStr);
 
   
   printf("%s",windStr), getch();
 readback("test2:text",&teststat);
 framedepth=1;
  
 
   framestack[0].x=0, framestack[0].y=0,
  framestack[0].xmax=100, framestack[0].ymax=80;
  framestack[0].blocked=0;
  framestack[0].seqstart=0, framestack[0].seqend=strlen(windStr);
 
 //printf("Ueberlebt...");
 teststat.x=1;
 teststat.xabs=5;
 teststat.yabs=5;
 teststat.xmaxabs=10;
 teststat.ymaxabs=10;
 
 
 strcpy(teststat.varname,"INSERTED");
 strcpy(teststat.text,"IMINSERTED");
 teststat.type='w';
 printf("%d", teststat.ymax);
 update("testfenster:xbutton",&teststat,INSERT);
 
 
 framedepth=1;
   framestack[0].x=0, framestack[0].y=0,
  framestack[0].xmax=100, framestack[0].ymax=80;
  framestack[0].blocked=0;
  framestack[0].seqstart=0, framestack[0].seqend=strlen(windStr);
 
 
 disp_elements();
 displayScreen();
 
 printf("\n%s\n", windStr);
 
 /*
 
   framestack[0].x=0, framestack[0].y=0,
  framestack[0].xmax=100, framestack[0].ymax=80;
  framestack[0].blocked=0;
  framestack[0].seqstart=0, framestack[0].seqend=strlen(windStr);
 
 
 pixcount=0;
  printf("vor disp_elements()");
   disp_elements();
   
   
   framedepth=1;
  
 
   framestack[0].x=0, framestack[0].y=0,
  framestack[0].xmax=100, framestack[0].ymax=80;
  framestack[0].blocked=0;
  framestack[0].seqstart=0, framestack[0].seqend=strlen(windStr);
 
 
   readback("test2:text",&teststat);
   framedepth=1;
  
 
   framestack[0].x=0, framestack[0].y=0,
  framestack[0].xmax=100, framestack[0].ymax=80;
  framestack[0].blocked=0;
  framestack[0].seqstart=0, framestack[0].seqend=strlen(windStr);
 
   if ( teststat.command=='1') putinfront("test2:text",-1);
   
   framedepth=1;
  
 
   framestack[0].x=0, framestack[0].y=0,
  framestack[0].xmax=100, framestack[0].ymax=80;
  framestack[0].blocked=0;
  framestack[0].seqstart=0, framestack[0].seqend=strlen(windStr);
 
   printf("vor get_input()");
   get_input();
    displayScreen();
    printf("\n%s",windStr);   
   
    
    //readback("testfenster:testbutton",&teststat);
    
    //printf("Hallo%s", teststat.text );
    
    
    while ( delay > clock());
  }
  */ 

}
