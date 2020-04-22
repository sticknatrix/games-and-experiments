/*
Added the path search algorithm from the Miracle C distribution.
I hope that's it.
  
  */
  


#include <stdio.h>
#include <time.h>
#include <fcntl.h>

#define NUM_SCRIPTS 7  // Number of individual scripts, they're all running simultaneously, give them a semaphore waiting loop to pause them
#define NUM_OBJECTS 6 // Number of game objects, including actors
#define GLOBAL_STATES 16 // Number of global state values


signed int data[1024*NUM_SCRIPTS+GLOBAL_STATES+NUM_OBJECTS*9]; // The shared working memory for the VM scripts


unsigned int cursor[2]; // Cursor X and Y coordinate


#define NUM_SENTENCES 3 // Strings to be displayed when something is being 'said', -1 means none
#define NUM_ROOMS 1 // Number of individual rooms


unsigned char objnames[NUM_OBJECTS][32]; // Identifier strings for objects
unsigned char sentences[NUM_SENTENCES][255]; // Sentences data array


#define OBJ_NUM(n) (1024*NUM_SCRIPTS+GLOBAL_STATES+n*9)

#define X_SCROLL     1024*NUM_SCRIPTS  // X room scrolling
#define Y_SCROLL     1024*NUM_SCRIPTS+1 // still not implemented
#define TIMERTICK    1024*NUM_SCRIPTS+2 // When the timer function delivers a new value, it is set to 1, a certain script has to set it to 0 and deliver it
#define TOUCHING     1024*NUM_SCRIPTS+3
#define ROOM         1024*NUM_SCRIPTS+4 // Inventory is 1, empty room is 0
#define ITEM_A       1024*NUM_SCRIPTS+5 // First item ID to be combined in a sentence
#define ITEM_B       1024*NUM_SCRIPTS+6 // second one
#define VERB         1024*NUM_SCRIPTS+7 // Verb to be used in a sentence
#define ACTION_CODE  1024*NUM_SCRIPTS+8 // User command state resulting from the sequence of the clicked stuff
#define SAY_SENTENCE 1024*NUM_SCRIPTS+9 // Script selection of sentence to be said
#define PATHSTARTOBJ 1024*NUM_SCRIPTS+10 // Object from which the path search starts


#define NOACTION 0
#define LOOK 1
#define PICK 2
#define COMBINE 3
#define PREP_LOOK    4
#define PREP_PICK    5
#define PREP_COMBI   6
#define PREP_COMBI_B 7
#define WALKTOXY     8



unsigned char sprite[400*7]; // Sprite patterns, raw 20x20 tiles with 8 bit color depth, linear greyscale, above 240 somewhere means transparency
unsigned char screen[160][90]; // Screen Buffer
signed int screenz[160][50]; // Z depth of each coordinate on the screen buffer

unsigned char *roompatt; // arrays often crash, but today we have OS automatic dynamic memory allocation
unsigned char *roomz; // The Z depth for room coordinates itself, however, sprites may be put in front of this


signed int walkmask[32][2][10][NUM_ROOMS]; // X, Y and Z coordinates of walkable coordinates of a room, still needs to be upscaled by a correcting routine


// just for the path search test
unsigned char walklayer1[320]={0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
                               0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
							   0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
							   0,0,0,1,0,1,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
							   0,0,0,1,0,1,0,0,1,0,0,1,0,0,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
							   0,0,0,1,0,1,0,0,1,0,0,1,0,0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
							   0,0,0,1,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							   0,0,0,1,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							   0,0,0,1,0,1,0,0,1,1,1,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							   0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};



// initialize the walk mask
void initwalklayer(void)
{
	int x=0,z=0;
	
 while ( z < 10)
 {
  x=0;
  while ( x < 32)
  {
  	walkmask[x][1][z][0]=walklayer1[x+z*32];
  	walkmask[x][0][z][0]=1;
    if (walklayer1[x+z*32]==1) roompatt[x*10+(z/4+30)*320]=0; 	 
  	
	  x++;
  	
	 }	
	z++;
 	
	 }	
	
}


 signed int passedbox[32][2][10]; // so the path search knows where it was and where it can spread



// That algorithm assigns each coordinate the shortest cross-formed way from the starting coordinate, afterwards you can trace the way back from the goal
// by seeking by-one-decremented neighbour points

signed int shortest;

int path_recurse(int x, int y, int z, int xg, int yg, int zg, int depth)
{
 
 
   //printf("%d %d %d\n",x,y,z);
   if ( depth > 1999) return;
   if ( x==xg && y == yg && z == zg){ if ( depth-1 < shortest )shortest=depth-1; }
   
 
       
      passedbox[x][y][z]=depth-1;
	     
    signed int buf;
      
      if ( x > 0 )if (passedbox[x-1][y][z]>depth||passedbox[x-1][y][z]==-3) path_recurse(x-1,y,z,xg,yg,zg,depth+1);
      if ( x < 31) if (passedbox[x+1][y][z]>depth||passedbox[x+1][y][z]==-3)path_recurse(x+1,y,z,xg,yg,zg,depth+1);
      if ( y > 0 ) if (passedbox[x][y-1][z]>depth||passedbox[x][y-1][z]==-3) path_recurse(x,y-1,z,xg,yg,zg,depth+1);
      if ( y < 1 ) if (passedbox[x][y+1][z]>depth||passedbox[x][y+1][z]==-3) path_recurse(x,y+1,z,xg,yg,zg,depth+1);
      if ( z > 0 ) if (passedbox[x][y][z-1]>depth||passedbox[x][y][z-1]==-3) path_recurse(x,y,z-1,xg,yg,zg,depth+1);
      if ( z < 9 ) if (passedbox[x][y][z+1]>depth||passedbox[x][y][z+1]==-3) path_recurse(x,y,z+1,xg,yg,zg,depth+1);
      
	  //printf("%d\n",depth);
	  return shortest;
 	
	
}



void markback(int x, int y, int z, int len)
{
 if ( len ==0 ) passedbox[x][y][z]=-4;
	
	if ( x > 0 ) if ( passedbox[x-1][y][z]==len-1)markback(x-1,y,z,len-1);
	if ( x < 31 ) if ( passedbox[x+1][y][z]==len-1)markback(x+1,y,z,len-1);
	if ( y > 0 ) if ( passedbox[x][y-1][z]==len-1)markback(x,y-1,z,len-1);
	if ( y < 1 ) if ( passedbox[x][y+1][z]==len-1)markback(x,y+1,z,len-1);
	if ( z < 9 ) if ( passedbox[x][y][z+1]==len-1)markback(x,y,z+1,len-1);
	if ( z > 0 ) if (passedbox[x][y][z-1]==len-1)markback(x,y,z-1,len-1);
	
}


// The thing is, there are just two Y heights, what you would think is Y is actually taken from the Z mask of the clicked coordinate.
// It still cannot select sprites, however, you can assume that they were transferred with there Z value on the Z mask there before
// Overlapping of several sprites shall not happen anyway
// Max could walk through Sam. There was no necessity to implement details like walking around him. Just imagine he did with a time leap.
// Consider the problems that could cause also.

void find_path(int obj, int x_, int y_)
{

 signed int x=data[OBJ_NUM(obj)+0]/10,y=data[OBJ_NUM(obj)+1]/20, z=data[OBJ_NUM(obj)+8]/10;
  x_/=10, y_/=30; 
 

 signed int z_=screenz[x_*10-data[X_SCROLL]][y_*20]/26;
 // pay attention: if sprite was moved in front, coordinates behind cannot be accessed anymore
 

//data[OBJ_NUM(obj)+1]=3;
 //y=1;  // Z-Maskenfehlenpfusch
 y_=1;
 //printf("Suche den Pfad von %d %d %d nach %d %d %d\n",x,y,z, x_,y_,z_);
 //getch();
 
 unsigned int c1,c2,c3;
 
 c3=0;
 while ( c3 < 10)
 {
  c1=0;
  while ( c1 < 32)
  {
   c2=0;
   while ( c2 < 2)
   {
    	passedbox[c1][c2][c3]=walkmask[c1][c2][c3][data[ROOM]-2]== 0 ? -3 : -2 ;
   	    c2++;
	   }	
    	c1++;
   }
   c3++;
  }
  
 
 
 //passedbox[x_][y_][z_]=-3;
 
 shortest=2000;
 
 path_recurse(x,y,z,x_,y_,z_,0);
 if ( shortest!=2000)markback(x_,y_,z_,shortest);
 data[OBJ_NUM(obj)+7]=1;
 
 data[OBJ_NUM(obj)+4]=x*10,  data[OBJ_NUM(obj)+5]=y*20,  data[OBJ_NUM(obj)+6]=z*10;
 
 
 // There's a semaphore to be marked when a path was found.
 // I'm still not sure how to calculate the runtime of the path search, but probably it cannot exceed 1999
 if ( shortest!= 2000)
 {
  	//printf("Pfad gefunden.");
 	if ( x-1 >= 0) if ( passedbox[x-1][y][z] ==-4 ) shortest= passedbox[x-1][y][z], data[OBJ_NUM(obj)+4]=x*10-10,  data[OBJ_NUM(obj)+5]=y*20,  data[OBJ_NUM(obj)+6]=z*10;
	  if ( x+1 < 32) if ( passedbox[x+1][y][z] ==-4 ) shortest= passedbox[x+1][y][z], data[OBJ_NUM(obj)+4]=x*10+10,  data[OBJ_NUM(obj)+5]=y*20,  data[OBJ_NUM(obj)+6]=z*10;
 	if ( y-1 >= 0) if ( passedbox[x][y-1][z]  ==-4 ) shortest= passedbox[x][y-1][z], data[OBJ_NUM(obj)+4]=x*10,  data[OBJ_NUM(obj)+5]=y*20-20,  data[OBJ_NUM(obj)+6]=z*10;
	if ( y+1 < 2) if ( passedbox[x][y+1][z]  ==-4 )  shortest= passedbox[x][y+1][z], data[OBJ_NUM(obj)+4]=x*10,  data[OBJ_NUM(obj)+5]=y*20+20,  data[OBJ_NUM(obj)+6]=z*10;
	if ( z-1 >= 0) if ( passedbox[x][y][z-1]  ==-4 ) shortest= passedbox[x][y][z-1], data[OBJ_NUM(obj)+4]=x*10,  data[OBJ_NUM(obj)+5]=y*20,  data[OBJ_NUM(obj)+6]=z*10-10;
	if ( z+1 < 10) if ( passedbox[x][y][z+1]  ==-4 )  shortest= passedbox[x][y][z+1], data[OBJ_NUM(obj)+4]=x*10,  data[OBJ_NUM(obj)+5]=y*20,  data[OBJ_NUM(obj)+6]=z*10+10;
	
 }
 else data[OBJ_NUM(obj)+7]=3; // We found no path
 //printf("Durchgelaufen");
 /*
 	printf("%d\n",passedbox[x-1][y][z] );
 	printf("%d\n",passedbox[x+1][y][z] );
 	printf("%d\n",passedbox[x][y+1][z] );
    printf("%d\n",passedbox[x][y-1][z] );
    printf("%d\n",passedbox[x][y][z+1] );
    printf("%d\n",passedbox[x][y][z-1] );
 */
 //printf("%d\n", shortest);
 //printf("Resultat: %d %d %d",data[OBJ_NUM(obj)+4],data[OBJ_NUM(obj)+5],data[OBJ_NUM(obj)+6]);
 //getch();
 
}







// Draw the level and afterwards draw the sprites
void drawlevel(void)
{
 int x,y;
 int room=data[ROOM]-2; // remember: 0 and 1 are special rooms

 y=0;
 while ( y < 50)
 {
  x=0;
  while ( x < 160)
  {
  	 screenz[x][y]=roomz[x+data[X_SCROLL] +y*320 +room*320*50];
  	  x++;
	  }	
 	y++;
 }
 

 y=0;	
 while ( y < 50)
 {
   x=0;
   while ( x < 160)
   {
     int c;
	 c=	roompatt[x+data[X_SCROLL]+y*320+room*320*50];
	 
	 if ( c < 64) c=' '; // ASCII can be just like greyscale, it's very ugly having only 4 greyscale values, however, it's still only a test
	 else
	 if ( c < 96) c=176;
	 else
	 if ( c < 128) c= 177;
	 else
	 if ( c < 196) c= '@';
	 else c=219;
    screen[x][y]=c ;
 	 x++;
	 }
   y++;	 
   }
   
   int n;
   n=0;
   while ( n < NUM_OBJECTS)
   {
   	if ( data[OBJ_NUM(n)+3]==room+2 || (data[OBJ_NUM(n)+3]==1 ) ) // first determine if it's in the current room or in the inventory below, subtract the scrolling value...
	    put_sprite( data[OBJ_NUM(n)+3] != 1 ? data[OBJ_NUM(n)+0]-data[X_SCROLL] : data[OBJ_NUM(n)+0],data[OBJ_NUM(n)+1],data[OBJ_NUM(n)+2],-96, data[OBJ_NUM(n)+8] ); 
	  	
	n++;
   }
	
}


// Sometimes we need to know whether a certain object is at a certain mouse cursor position.
// The Y and Z confusion may still be causing trouble... however, we're always clicking objects and not room pattern parts,
// what can be selected always needs to be placed, use a completely empty and transparent pattern for all of them

signed int getobjfromcoord( int x, int y )
{
 int n=0;
 
 while ( n < NUM_OBJECTS)
 {
 	if ( data[OBJ_NUM(n)+3]!=1)
 	{
   if ( data[OBJ_NUM(n)+3]==data[ROOM] &&
         fabs (  x -  ( data[OBJ_NUM(n)+0]-data[X_SCROLL]+10 )   ) < 15 &&
		  fabs ( y -  ( data[OBJ_NUM(n)+1]-data[Y_SCROLL]+10 )   ) < 15  ) return n;
     }
    else
   {
   	if ( data[OBJ_NUM(n)+3]==1 &&
          fabs ( x -  ( data[OBJ_NUM(n)+0] +10 )  ) < 10 &&
		  fabs ( y -  ( data[OBJ_NUM(n)+1] +10 ) ) < 10  ) return n;
   
   	
	} 
 
  n++;	
 }

 return -1;	
}


// Is a _certain_ object there?
signed int getobjNfromcoord( int x, int y, int n )
{
 
 	if ( data[OBJ_NUM(n)+3]!=1)
 	{
   if ( data[OBJ_NUM(n)+3]==data[ROOM] &&
         fabs (  x -  ( data[OBJ_NUM(n)+0]-data[X_SCROLL]+10 )   ) < 15 &&
		  fabs ( y -  ( data[OBJ_NUM(n)+1]-data[Y_SCROLL]+10 )   ) < 55  ) return n;
     }  	
     

 return -1;	
}



// draw the sprite and change the Z mask, there's still the possibility for brightness scaling

void put_sprite(int xstart,int ystart, int n, int luma, int z)
{
  int x=0;
  int y=0;
  signed int scroll;   
   scroll=0;
   
   if ( ystart < 50)ystart+=z/4; // Here you can see that i don't know what i'm doing. From a certain aspect, Z could about look like an increased Y
 
  // printf("Setze es an Position %d %d",xstart-scroll, ystart);
 
 //printf("%d\n",z);
 
  while ( y < 20 )
  {
   x=0;
   while ( x < 20 )
  {
  	if ( screenz[x+xstart][y+ystart]/50<= z || ystart > 50)
   if ( xstart+x-scroll > 0 && xstart+x-scroll < 160 &&
        ystart+y > 0 && ystart+y < 90 )
   {
    if ( sprite[n*400+x+y*20]<220 ) // eigentlich 255 der Transparenzwert
    {
     if ( sprite[n*400+x+y*20]+luma< 64 ) 
      screen[xstart+x-scroll][y+ystart]=176;
     else
     if ( sprite[n*400+x+y*20]+luma< 128 ) 
      screen[xstart+x-scroll][y+ystart]=177;
     else
     if ( sprite[n*400+x+y*20]+luma< 196 ) 
      screen[xstart+x-scroll][y+ystart]='@';
     else
	//if ( sprite[n*400+x+y*20]+luma< 255 ) 
      screen[xstart+x-scroll][y+ystart]=219;
      
       screenz[x][y]=z;
   
    }
    
    }
    x++;
   }
   y++;

  }

}




/*
  0 von Beginn: X-Koordinate eines Sprites
  1 von Beginn: Y-Koordinate eines Sprites
  2 : Sprite-Phase
  3: Raum ( 0 kein, 1 Inventory )
  4: Pfadsuche X
  5: Pfadsuche Y
  6: Pfadsuche Z
  7: Pfadsuche hat Daten 2 Aufruf 1 Abholen
  8: Z-Daten
   der Rest ist frei
*/

// A data object needs x, y and z coordinates, for getobjfromcoord() we still ignored z, however, with two lines of code it should be about ok
// You can assign each data object a sprite tile number, you also assign it a room. The path search will enter it's next coordinates somewhere there,
// but you can catch the next coordinates only when the value at offset 7 is 1. You have to call the path search again for every animation step.
// But this is only logical because some things in the room might change while walking
// Maybe implement the possibility to find an object with a script over, again, a call semaphore, and then you could take it's coordinates from the result
// Comparing direct coordinates with the touch instruction could also have advantages?



// each script has it's own instruction memory ( not to confuse with the random-access 'data segment' thing )
// Each instruction can have two operands
// and when a script is locked, the VM will continue executing it until it is unlocked and therefore also the core will hang
struct
{
 signed int instruction[1024][3];
 signed int instr_ptr;
 unsigned int locked;
} script[NUM_SCRIPTS];



#define ADD 0 // some basic mathematical operations, would be practical to extend the VM instruction set, however, multiplication and division are additions
#define SUB 1 // and subtraction in a loop, did you know that?
#define MOV 2 // move value from address of second operand to the address of the first operand
#define IFCMP 3 // when values on the addresses of both operands are equal, don't skip the next instruction, otherwise, add 2 to instr_ptr
#define JMP 4 // Jump to address of operand 1
#define LOCK 5 // Don't underestimate the possibility to prevent interruption of a certain script. Without this possibility, correct execution is never guaranteed
#define UNLOCK 6 // it will also lock the main loop, but our scripts shouldn't be a threat to the 'operating system' anyway
#define TOUCH 7 // when object of number of operand 1 is close enough to the object with the number of operand 2, set a certain value in script memory to 1,
             // otherwise zero

// Virtual machine sounds very sophisticated, however, in it's basic form, it's cheap

void VM_Step(void)
{
 int n=0;
 while ( n < NUM_SCRIPTS )
 {
   if ( script[n].instruction[script[n].instr_ptr][0]==ADD )
   {
    data[script[n].instruction[script[n].instr_ptr][1]]+=
      data[script[n].instruction[script[n].instr_ptr][2]];
      
    script[n].instr_ptr++;     
   }
   else
   if ( script[n].instruction[script[n].instr_ptr][0]==SUB )
   {
   data[script[n].instruction[script[n].instr_ptr][1]]-=
      data[script[n].instruction[script[n].instr_ptr][2]];
     
    script[n].instr_ptr++;     
    
   }
   else
   if ( script[n].instruction[script[n].instr_ptr][0]==MOV )
   {
     data[script[n].instruction[script[n].instr_ptr][1]]=
      data[script[n].instruction[script[n].instr_ptr][2]];
     script[n].instr_ptr++;     
 
   }
   else
   if ( script[n].instruction[script[n].instr_ptr][0]==IFCMP )
   {
     if (    data[script[n].instruction[script[n].instr_ptr][1]]==
      data[script[n].instruction[script[n].instr_ptr][2]]);
     else script[n].instr_ptr++;
    
   
    script[n].instr_ptr++;     
       
   }
   else
   if ( script[n].instruction[script[n].instr_ptr][0]==JMP )
   {
    script[n].instr_ptr=script[n].instruction[script[n].instr_ptr][1];
   }
   else
   if ( script[n].instruction[script[n].instr_ptr][0]==LOCK )
   {
    //int n2;
    //n2=0;
    //while ( n2< NUM_SCRIPTS ){
        //   if (!( n2==n) )
		   
		   script[n].locked=1; //n2++; } 
    script[n].instr_ptr++;     
   }
   else
   if ( script[n].instruction[script[n].instr_ptr][0]==UNLOCK )
   {
    script[n].locked=0;
    script[n].instr_ptr++;     
    
   }
   else if ( script[n].instruction[script[n].instr_ptr][0]==TOUCH)
   {
   	long int obj1, obj2;
   	obj1=script[n].instruction[script[n].instr_ptr][1];
   	obj2=script[n].instruction[script[n].instr_ptr][2];
   	
     if ( fabs(data[1024*NUM_SCRIPTS+GLOBAL_STATES+obj1*9+0]-
   	           data[1024*NUM_SCRIPTS+GLOBAL_STATES+obj2*9+0] ) < 30 &&
   	      fabs(data[1024*NUM_SCRIPTS+GLOBAL_STATES+obj1*9+1]-
   	           data[1024*NUM_SCRIPTS+GLOBAL_STATES+obj2*9+1] ) < 30 &&
				fabs(data[1024*NUM_SCRIPTS+GLOBAL_STATES+obj1*9+8]-
   	           data[1024*NUM_SCRIPTS+GLOBAL_STATES+obj2*9+8] ) < 30 &&
   	           
   	           data[1024+NUM_SCRIPTS+GLOBAL_STATES+obj1*9+3]==
   	           data[1024+NUM_SCRIPTS+GLOBAL_STATES+obj2*9+3]
				  )
   	  data[TOUCHING]=1;
   	  else data[TOUCHING]=0;
   	           
   	script[n].instr_ptr++;     
               
   	
   	
   }

  if ( script[n].locked==0)n++; else continue;
 }

}




int main(void)
{
 clock_t timertick;
 int pathgoalx, pathgoaly;


 // Load graphic patterns and stuff, the preprocessor is very useful to keep things overviewable
 
  FILE *pattfile;
  
  pattfile=fopen(".\\sprites","rb");

  fread(sprite,20*20*6,sizeof(unsigned char),pattfile);
  fclose(pattfile);
  
  
  pattfile=fopen(".\\rooms","rb");
  
  roompatt=malloc(320*50*NUM_ROOMS);
  roomz=malloc(320*50*NUM_ROOMS);
  
  fread(roompatt,320*50*NUM_ROOMS, sizeof(unsigned char), pattfile);
  fread(roomz,320*50*NUM_ROOMS, sizeof(unsigned char), pattfile);
  
  fclose(pattfile);
 
  initwalklayer();
  
 
 // One directive, and you already have an assembler.
 // The first value is not exactly the 'code segment', but the script number
  
  #define VMCODE(n,adress,opcode,op1,op2) script[n].instruction[adress][0]=(opcode),\
                                          script[n].instruction[adress][1]=(op1),\
										  script[n].instruction[adress][2]=(op2),
										


// Here our assmbler coding experiments begin.
// What riddle could we take?
// We start with basic needs, hand to mouth...

/*
	VMCODE(0,0,ADD,X_SCROLL,100)
	VMCODE(0,1,JMP,0,0) 
  
  
  				
	VMCODE(1,0,IFCMP,TIMERTICK,101)
	VMCODE(1,1,MOV,X_SCROLL,0)
	VMCODE(1,2,JMP,0,0) 
   
  
  	VMCODE(1,0,LOCK,0,0)
	VMCODE(1,1,IFCMP,TIMERTICK,101)
	VMCODE(1,2,JMP,5,0) 
    VMCODE(1,3,UNLOCK,0,0)
  	VMCODE(1,4,JMP,0,0) 
	VMCODE(1,5,ADD,2,101)
	VMCODE(1,6,MOV,TIMERTICK,102)
	VMCODE(1,7,UNLOCK,0,0)
    VMCODE(1,8,IFCMP,2,103)
    VMCODE(1,9,MOV,2,0)
	VMCODE(1,10,JMP,0,0)  	
	  				
	VMCODE(2,0,JMP,0,0) 
  
  */
  
  VMCODE(0,0,UNLOCK,0,0)
  VMCODE(0,1,LOCK,0,0)
  VMCODE(0,2,IFCMP,ACTION_CODE,101)
  VMCODE(0,3,JMP,5,0)
  VMCODE(0,4,JMP,0,0)
  VMCODE(0,5,IFCMP,ITEM_A,105)
  VMCODE(0,6,JMP,8,0)
  VMCODE(0,7,JMP,0,0)
  VMCODE(0,8,MOV,SAY_SENTENCE,102)
  VMCODE(0,9,MOV,OBJ_NUM(1)+3,102)
  VMCODE(0,10,MOV,OBJ_NUM(2)+3,105)
  VMCODE(0,11,MOV,ACTION_CODE,NOACTION)
  VMCODE(0,12,UNLOCK,0,0)
  VMCODE(0,13,JMP,0,0)
  
  //Pfadsuche
  
  VMCODE(1,0,UNLOCK,0,0)
  VMCODE(1,1,UNLOCK,0,0)
  VMCODE(1,2,LOCK,0,0)
  VMCODE(1,3,IFCMP,ACTION_CODE,104)
  VMCODE(1,4,JMP,6,0)
  VMCODE(1,5,JMP,0,0)
  VMCODE(1,6,MOV,ACTION_CODE,0)
  VMCODE(1,7,MOV,OBJ_NUM(0)+7,106)
  VMCODE(1,8,JMP,0,0)
 
 
  VMCODE(2,0,UNLOCK,0,0)
  VMCODE(2,1,LOCK,0,0)
  VMCODE(2,2,IFCMP,OBJ_NUM(0)+7,105)
  VMCODE(2,3,JMP,5,0)
  VMCODE(2,4,JMP,0,0)
  VMCODE(2,5,MOV,OBJ_NUM(0),OBJ_NUM(0)+4)
  VMCODE(2,6,MOV,OBJ_NUM(0)+1,OBJ_NUM(0)+5)
  VMCODE(2,7,MOV,OBJ_NUM(0)+8,OBJ_NUM(0)+6)
  VMCODE(2,8,MOV,OBJ_NUM(0)+7,102)
  //VMCODE(2,9,MOV,X_SCROLL,OBJ_NUM(0))
  VMCODE(2,9,LOCK,0,0)
  
  VMCODE(2,10,MOV,OBJ_NUM(0)+7,102)
  VMCODE(2,11,JMP,0,0)
  
  
  
  VMCODE(3,0,IFCMP,OBJ_NUM(0),107)
  VMCODE(3,1,JMP,3,0)
  VMCODE(3,2,JMP,0,0)
  VMCODE(3,3,MOV,X_SCROLL,108 )
  VMCODE(3,4,JMP,0,0)
  
  VMCODE(4,0,IFCMP,OBJ_NUM(0),108)
  VMCODE(4,1,JMP,3,0)
  VMCODE(4,2,JMP,0,0)
  VMCODE(4,3,MOV,X_SCROLL,102)
  VMCODE(4,4,JMP,0,0)
  
  
  
  
  // Benutze Zigarettenautomat mit Cash Card
  
  
  VMCODE(5,0,UNLOCK,0,0)
  VMCODE(5,1,LOCK,0,0)
  VMCODE(5,2,IFCMP,ACTION_CODE,109)
  VMCODE(5,3,JMP,5,0)
  VMCODE(5,4,JMP,0,0)
  VMCODE(5,5,IFCMP,ITEM_A,110)
  VMCODE(5,6,JMP,8,0)
  VMCODE(5,7,JMP,0,0)
  
  VMCODE(5,8,IFCMP,ITEM_B,111)
  VMCODE(5,9,JMP,11,0)
  VMCODE(5,10,JMP,0,0)
  VMCODE(5,11,TOUCH,0,5)
  VMCODE(5,12,IFCMP,TOUCHING,105)
  VMCODE(5,13,JMP,15,0)
  VMCODE(5,14,JMP,19,0)
  VMCODE(5,15,MOV,OBJ_NUM(4)+3,105)
  VMCODE(5,16,MOV,ACTION_CODE,NOACTION)
  VMCODE(5,17,UNLOCK,0,0)
  VMCODE(5,18,JMP,0,0)
  VMCODE(5,19,MOV,SAY_SENTENCE,105)
  VMCODE(5,20,JMP,0,0)
 
 
 // Benutze Feuerzeug mit Plaquestar Light
 
  VMCODE(6,0,UNLOCK,0,0)
  VMCODE(6,1,LOCK,0,0)
  VMCODE(6,2,IFCMP,ACTION_CODE,109)
  VMCODE(6,3,JMP,5,0)
  VMCODE(6,4,JMP,0,0)
  VMCODE(6,5,IFCMP,ITEM_A,112)
  VMCODE(6,6,JMP,8,0)
  VMCODE(6,7,JMP,0,0)
  
  VMCODE(6,8,IFCMP,ITEM_B,113)
  VMCODE(6,9,JMP,11,0)
  VMCODE(6,10,JMP,0,0)
  VMCODE(6,11,MOV,SAY_SENTENCE,110)
  VMCODE(6,12,JMP,0,0)
  
  
  
  // Variablen
  
  // In the data part, you have to put some values at certain addresses because the byte-code also doesn't support direct values and so on...
  // It would probably also make sense to extend the VM with registers and a stack option, however, theoretically full functionality is provided i guess
  
  data[101]=LOOK;
  data[102]=0;
  data[103]=10;
  data[104]=WALKTOXY;
  data[105]=1;
  data[106]=2;
  data[107]=140;
  data[108]=130;
  data[109]=COMBINE;
  data[110]=2;
  data[111]=5;
  data[112]=3;
  data[113]=4;
  
  
  
  data[X_SCROLL]=0;
  data[Y_SCROLL]=0;
  data[TIMERTICK]=0;
 
  data[PATHSTARTOBJ]=0; 
   data[ROOM]=2;
   data[OBJ_NUM(0)+3]=2;
   data[OBJ_NUM(0)+2]=0;
   data[OBJ_NUM(0)+0]=0;
   data[OBJ_NUM(0)+1]=20;
   data[OBJ_NUM(0)+8]=50; // war 9
   
   
   
   data[OBJ_NUM(1)+3]=1;
   data[OBJ_NUM(1)+2]=1;
   data[OBJ_NUM(1)+0]=120;
   data[OBJ_NUM(1)+1]=52;
   data[OBJ_NUM(1)+8]=0;
   
   
   data[OBJ_NUM(2)+3]=0;
   data[OBJ_NUM(2)+2]=2;
   data[OBJ_NUM(2)+0]=120;
   data[OBJ_NUM(2)+1]=52;
   data[OBJ_NUM(2)+8]=0;

   data[OBJ_NUM(3)+3]=1;
   data[OBJ_NUM(3)+2]=3;
   data[OBJ_NUM(3)+0]=95;
   data[OBJ_NUM(3)+1]=52;
   data[OBJ_NUM(3)+8]=0;
  
  
  
   data[OBJ_NUM(4)+3]=0;
   data[OBJ_NUM(4)+2]=4;
   data[OBJ_NUM(4)+0]=70;
   data[OBJ_NUM(4)+1]=52;
   data[OBJ_NUM(4)+8]=0;
  
  
   data[OBJ_NUM(5)+3]=2;
   data[OBJ_NUM(5)+2]=5;
   data[OBJ_NUM(5)+0]=230;
   data[OBJ_NUM(5)+1]=15;
   data[OBJ_NUM(5)+8]=5;
   
   
   
   data[SAY_SENTENCE]=-1;
   data[ACTION_CODE]=NOACTION;
    strcpy(objnames[0],"Billy"); // each object needs a text string
    strcpy(objnames[1],"Wallet");
   strcpy(objnames[2],"Cash Card");
   strcpy(objnames[3],"Cigarette Lighter");
   strcpy(objnames[4],"Plaquestar Light"); // no similarity to real products intended
   strcpy(objnames[5],"Cigarette Machine");
   
   strcpy(sentences[0],"There's my cash card with a special chip.'");
   strcpy(sentences[1],"I first have to walk to the vending machine.");
   strcpy(sentences[2],"Now I have cancer."); // we're not gonna propagate anything. No irony intended.
   

   cursor[0]=80, cursor[1]=40;  
  timertick=(long int)clock()/10000;
  while (1 )
  {
   VM_Step();
   if ( clock()/10000 != timertick ) data [TIMERTICK]=1, timertick=clock()/10000; // When the timer value goes a second higher or changes significantly,
   // we still will have enough time to transfer the notification into the script memory
   
   //else data[TIMERTICK]=0;
   int x=0, y=0;
    while ( y < 90)
    {
     x=0;
	 while ( x< 160)
	 {
	  screen[x][y]=' ';	// erase the screen
      x++;
	 }
      y++;
    }	
    
    unsigned char cmd;
    cmd=0;
    if ( kbhit()) // Keyboard press trigger
    {
    	
    	
     switch(getch())
     {
      case 'h' : if ( cursor[0] > 3 ) cursor[0]-=3; break;	
      case 'k' :  if ( cursor[0] < 156 ) cursor[0]+=3;  break;	
      case 'u' : if ( cursor[1] > 3 ) cursor[1]-=3;  break;	
      case 'j' : if ( cursor[1] < 84 ) cursor[1]+=3;  break;	
      case 'l' : cmd=1;  break;	
     	
	 }
	}
    
    // We can only move the cursor with four keys and 'click' with another
    
    
    drawlevel(); // It is necessary to also provide the Z information to some other program parts, so this has to be done before
    
	
	
	 screen[cursor[0]][cursor[1]]='*';
	
	 screen[20][70]='L',screen[21][70]='o',screen[22][70]='o',screen[23][70]='k', // Nice interface design
	 screen[40][70]='P',screen[41][70]='i',screen[42][70]='c',screen[43][70]='k',
	 screen[60][70]='U',screen[61][70]='s',screen[62][70]='e';
	 
	 
	 // Build the command instructions together when there was a 'click'
	 // It may look like a sentence, but internally it are only 3 IDs
	 if ( cmd==1 )
	 {
	 	
	 	 signed int itema;
	      
	 	
	   if ( fabs ( cursor[0]-22 ) < 5 && fabs (cursor[1]-70 ) < 5 ) data[ACTION_CODE]=PREP_LOOK;	
	 	else
	      if ( fabs ( cursor[0]-42 ) < 5 && fabs (cursor[1]-70 ) < 5 ) data[ACTION_CODE]=PREP_PICK;	
	 	else
	   if ( fabs ( cursor[0]-61 ) < 5 && fabs (cursor[1]-70 ) < 5 )data[ACTION_CODE]=PREP_COMBI;	
	 	else
	 	{
	 	  if ( data[ACTION_CODE]==PREP_LOOK)	
	      {
	      	 if ( (itema=getobjfromcoord(cursor[0],cursor[1]))!= -1)
	      	   {	      	
		         data[ACTION_CODE]=LOOK;
				 data[ITEM_A]=itema;	  
			   }		
	 	
	       }
		  else
		  if ( data[ACTION_CODE]==PREP_PICK)	
	      {
	      	 if ( (itema=getobjfromcoord(cursor[0],cursor[1]))!= -1)
	      	   {	      	
		         data[ACTION_CODE]=PICK;
				 data[ITEM_A]=itema;	  
			   }		
	 	
	       }
	       else
	       if ( data[ACTION_CODE]==PREP_COMBI)
	       {
	       	 if ( (itema=getobjfromcoord(cursor[0],cursor[1]))!= -1)
	      	   {	      	
		         data[ACTION_CODE]=PREP_COMBI_B;
				 data[ITEM_A]=itema;	  
			   }		
	 	
	       	
		   }
	     else
	       if ( data[ACTION_CODE]==PREP_COMBI_B)
	       {
	       	 signed int itemb;
	       	 if ( (itemb=getobjfromcoord(cursor[0],cursor[1]))!= -1)
	      	   {	      	
		         data[ACTION_CODE]=COMBINE;
				 data[ITEM_B]=itemb;	  
			   }		
	 	
	       	
		   }
		   else if ( cursor[1] < 50) data[ACTION_CODE]=WALKTOXY, pathgoalx=cursor[0], pathgoaly=cursor[1]; // buffer the cursor target coordinates
		 
		 
		 	
		 }
	 	
	 	
	 }
	 
	
	system("cls\n");
	y=0;
	while ( y < 85)
    {
     x=0;
	 while ( x< 160)
	 {
	  printf("%c%c%c%c%c%c%c%c",screen[x][y], screen[x+1][y], screen[x+2][y], screen[x+3][y],screen[x+4][y],screen[x+5][y],screen[x+6][y],screen[x+7][y]);	
      //printf("%c%c%c%c",screen[x][y], screen[x+1][y], screen[x+2][y], screen[x+3][y]);	
      
	  x+=8;
	 }
	 printf("\n");
      y++;
    }
    
    if ( data[OBJ_NUM(data[PATHSTARTOBJ])+7]==2) // When a script has requested it, run the path search
	  if ( getobjNfromcoord(pathgoalx,pathgoaly,data[PATHSTARTOBJ])!=data[PATHSTARTOBJ] )
	find_path(data[PATHSTARTOBJ],pathgoalx+data[X_SCROLL],pathgoaly),data[ACTION_CODE]=WALKTOXY; else {
	 data[OBJ_NUM(data[PATHSTARTOBJ])+7]=0,data[ACTION_CODE]=NOACTION; }
	
	//printf("%d %d", data[OBJ_NUM(0)+4],data[OBJ_NUM(0)+5]);
    
    // Dump the command sentence
    if ( data[ACTION_CODE]==PREP_LOOK) printf("Look at ");
	 if ( data[ACTION_CODE]==PREP_PICK) printf("Pick Up");
	 if ( data[ACTION_CODE]==PREP_COMBI) printf("Use"); 
	  if ( data[ACTION_CODE]==LOOK) printf("Look at "), printf("%s",objnames[data[ITEM_A]]);
	 if ( data[ACTION_CODE]==PICK) printf("Pick Up "), printf("%s",objnames[data[ITEM_A]]);
	 if ( data[ACTION_CODE]==PREP_COMBI_B) printf("Use "), printf("%s",objnames[data[ITEM_A]]);
	 if ( data[ACTION_CODE]==COMBINE) printf("Use "), printf("%s",objnames[data[ITEM_A]]), printf(" with "), printf("%s", objnames[data[ITEM_B]]);
	 if ( data[SAY_SENTENCE]!=-1) printf("\n%s",sentences[data[SAY_SENTENCE]]);
	 
    
	}
  


}
