/*
  i'll call it attribute stack.
  
  use no capitals. All it can display and process are small letters
  
  <fs> <fb> <fm> set font to small, big or medium
  <al> <ar> <am> set alignment to left, middle or right
  <lfilename> the following text will load the described file into the interpreter on a klick,
              don't use a dot or a file ending
  <bfilename> displays a 20x20 pixel pattern
  </f>
  </a>
  </l> closing tags
  
  cursor is moved with i,j,k,l and clicked is with o
  
  
  input text is scanned, attributes will be stacked, screen mask might be marked with a file
  index number for a link.
  That's basically how you can write a hypertext interpreter.
  
  */


#include <stdio.h>

struct
{
 int x,y;
  unsigned int hyperlink, bitmap;
}parsedown;


unsigned char screen[160][90];
unsigned char linkmask[160][90];

struct
{
 unsigned char filename[50];
}linkfiles[50];


unsigned int cursor[2];


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


void displayletter(int c, int x, int y, int sizec)
{
if ( c==' ') return;
 int x2=0, y2=0;
  float size;
 int c2=(tolower(c)-0x61)*6*5;
 
 switch( sizec )
 {
   case 'b': size=2.0; break;
   case 'm' : size=1.5; break;
   case 's' : size=1.0; break;	
 	
 }
 
 
 y2=0;
 while ( y2 < 5*size&& y+y2 < 89)
 {
  x2=0;
  while ( x2 < 6*size&& x+x2 < 159)
  screen[x+x2][y+y2]=font[((int)(x2/size)+(int)(y2/size)*6+c2)],x2++;
  y2++;
  }
  
  
}

void displaylink(int x, int y, int linknum, int sizec)
{
 int x2=0, y2=0;
 float size;
 switch( sizec )
 {
   case 'b': size=2.0; break;
   case 'm' : size=1.5; break;
   case 's' : size=1.0; break;	
 	
 }
 
 y2=0;
 while ( y2 < 5*size&& y+y2 < 89)
 {
  x2=0;
  while ( x2 < 6*size&& x+x2 < 159)
  linkmask[x+x2][y+y2]=linknum,x2++;
  y2++;
  }
  
  
}



struct
{
 int tagtype;
 int font;
 int alignto;
 signed int hyperlink;
  unsigned char hyperstring[50];
  unsigned char bitmapstring[50];
}attribstack[50];


int main(int argc, char *argv[])
{
 FILE *input;
 unsigned char inputfile[255];
 FILE *bitmapfile;
  unsigned char c;
   int layer=0;
   int n=0;
    int x,y, linkcount;
    int lborder;
    int lineadd=0;
    
   int lexstate=0;

if ( argc!=2 ) return;
strcpy(inputfile,argv[1]);

linkreload:
  input=fopen(inputfile,"rb");

 lineadd=0;
 lborder=0;
 lexstate=0;
 layer=0;
 linkcount=0;

parsedown.x=0, parsedown.y=0;
  attribstack[0].tagtype='t',
   attribstack[layer].font='s',
   attribstack[layer].alignto='l',
   attribstack[layer].hyperlink=-1,
    strcpy(attribstack[layer].hyperstring,""),
    strcpy(attribstack[layer].bitmapstring,"");



 #define SAVELAYER \
   attribstack[layer+1].tagtype=attribstack[layer].tagtype,\
   attribstack[layer+1].font=attribstack[layer].font,\
   attribstack[layer+1].alignto=attribstack[layer].alignto,\
   attribstack[layer+1].hyperlink=attribstack[layer].hyperlink,\
    strcpy(attribstack[layer+1].hyperstring,attribstack[layer].hyperstring),\
    strcpy(attribstack[layer+1].bitmapstring,attribstack[layer].bitmapstring),\
   layer++;

  y=0;
  while ( y < 90 )
  {
   x=0;
   while ( x < 160 )
   {
    screen[x][y]=' ';
    linkmask[x][y]=0;
    x++;    
   }
   y++;
  }

 while ( !feof(input) )
 {
  c=fgetc(input);
  if ( feof(input))break;
  if ( lexstate==0 && c=='<' )
  {
   lexstate=1;   
  }
  else if ( lexstate==0 )
  {
     displayletter(c,parsedown.x,parsedown.y,attribstack[layer].font);
     if ( attribstack[layer].hyperlink!=-1 ) displaylink(parsedown.x,parsedown.y,attribstack[layer].hyperlink,attribstack[layer].font);   

    if ( attribstack[layer].alignto=='l') lborder=0;
    if ( attribstack[layer].alignto=='m') lborder=50;
    if ( attribstack[layer].alignto=='r') lborder=100;
    

    if ( attribstack[layer].font=='m' )
    {
    	 (lineadd < 6 ? lineadd=6 : 0 );
    	 (lineadd < 10 ? lineadd=10 : 0 );
    	 (lineadd < 15 ? lineadd=15 : 0 );
    	 
     parsedown.x+=10;
    if ( parsedown.x > 159 ) parsedown.x=lborder, parsedown.y+=lineadd,lineadd=0;
  
    }
      if ( attribstack[layer].font=='b' )
    {
     parsedown.x+=15;
     if ( parsedown.x > 159 ) parsedown.x=lborder, parsedown.y+=lineadd,lineadd=0;
  
    }
      if ( attribstack[layer].font=='s' )
    {
     parsedown.x+=6;
     if ( parsedown.x > 159 ) parsedown.x=lborder, parsedown.y+=lineadd,lineadd=0;
  
    }


  }
  else if ( lexstate==1 && isalpha(c) )
  {
   if ( c=='l' ) lexstate=2;
   else
   if ( c=='b' ) lexstate=3;
   else
   if ( c=='f' ) lexstate=4;
   else
   if ( c=='a' ) lexstate=5;
   else
   if ( c=='r' )
   {
   	
    	 (lineadd < 6 ? lineadd=6 : 0 );
    	 (lineadd < 10 ? lineadd=10 : 0 );
    	 (lineadd < 15 ? lineadd=15 : 0 );
    	 
    parsedown.y+=lineadd,lineadd=0;
    if ( attribstack[layer].alignto=='l') lborder=0;
    if ( attribstack[layer].alignto=='m') lborder=50;
    if ( attribstack[layer].alignto=='r') lborder=100;
    parsedown.x=lborder;
     lexstate=0;
     c=fgetc(input);
    if ( c!='>' ) { fclose(input); return; }

   }
   else { fclose(input); return; }

  }
  else if ( lexstate==1 && c=='/')
  {
   c=fgetc(input);
   
   if ( c=='l' && attribstack[layer].tagtype=='l') layer--;
   else
   if ( c=='f'&& attribstack[layer].tagtype=='f' ) layer--;
   else
   if ( c=='a'&& attribstack[layer].tagtype=='a' ) layer--;
   else { fclose(input); return; }
   c=fgetc(input);
     
   if ( c!='>' )
   {
    fclose(input); return;
   }
   else lexstate=0;
    
  }
  else if ( lexstate==2 )
  {
   SAVELAYER
     attribstack[layer].tagtype='l';

   n=0;
   do
   {
    attribstack[layer].hyperstring[n]=c;
	c=fgetc(input);
    n++;
   } while ( isalpha(c) );
     attribstack[layer].hyperstring[n]='\0';
    
    strcpy(linkfiles[linkcount].filename,attribstack[layer].hyperstring);    
    
	linkcount++;
    attribstack[layer].hyperlink=linkcount;

   if ( c=='>' ) lexstate=0;
   else { fclose(input); return; }    
  }
  else if ( lexstate==3 )
  {
   SAVELAYER
      //attribstack[layer].tagtype='b';
     n=0;
   do
   {
    attribstack[layer].bitmapstring[n]=c;
	c=fgetc(input);
    n++;
   } while ( isalpha(c) );
     attribstack[layer].bitmapstring[n]='\0';
   if ( c=='>' ) lexstate=0;
   else { fclose(input); return; }
   lexstate=0;
   
   
    if ( attribstack[layer].alignto=='l') lborder=0;
    if ( attribstack[layer].alignto=='m') lborder=50;
    if ( attribstack[layer].alignto=='r') lborder=100;
   
   
   bitmapfile=fopen(attribstack[layer].bitmapstring,"rb");
   y=0;
   while ( y < 20&&y+parsedown.y<89)
   {
   	x=lborder;
   	while ( x < 20+lborder&& x+parsedown.x < 159)
   	{
   		unsigned char luma;
   		luma=fgetc(bitmapfile);
   		if ( luma < 64) luma=' ';
   		else if ( luma < 128 ) luma= 176;
   		else if ( luma < 196 ) luma= 177;
   		else luma=219;
   		
   	 screen[parsedown.x+x][parsedown.y+y]=luma;
   	 if ( attribstack[layer].hyperlink!=-1)linkmask[parsedown.x+x][parsedown.y+y]=attribstack[layer].hyperlink;
	 x++;
	}
   	y++;
   }
   fclose(bitmapfile);
   
   parsedown.x+=20+lborder; lineadd=20;
   if ( parsedown.x > 159) parsedown.x=0, parsedown.y+=20,lineadd=0;
    layer--;   
  }
  else if ( lexstate==4 )
  {
   SAVELAYER
     attribstack[layer].tagtype='f';

   //c=fgetc(input);

   if ( c=='s' ) attribstack[layer].font='s';
   else
   if ( c=='b' ) attribstack[layer].font='b';
   else
   if ( c=='m' ) attribstack[layer].font='m';
   else { fclose(input); return; }
     c=fgetc(input);
   if ( c != '>' ) { fclose(input); return; }
   lexstate=0;
  }
  else if ( lexstate==5 )
  {
      SAVELAYER
     attribstack[layer].tagtype='a';

   //c=fgetc(input);

   if ( c=='l' ) attribstack[layer].alignto='l';
   else
   if ( c=='r' ) attribstack[layer].alignto='r';
   else
   if ( c=='m' ) attribstack[layer].alignto='m';
   else { fclose(input); return; }
     c=fgetc(input);
   if ( c != '>' ) { fclose(input); return; }
   lexstate=0;

  }

 }

 fclose(input);
 
 cursor[0]=0, cursor[1]=0; 
 int cmd=0;
 int backed=0;
 
 while ( 1)
 {
   cmd=0;
   if ( kbhit())
   {
   	switch(getch())
    {
     case 'j': if ( cursor[0] > 0) screen[cursor[0]][cursor[1]]=c, cursor[0]--, backed=0;	break;
     case 'l': if ( cursor[0] < 159)screen[cursor[0]][cursor[1]]=c, cursor[0]++, backed=0; break;
     case 'i': if ( cursor[1] > 0) screen[cursor[0]][cursor[1]]=c, cursor[1]--, backed=0; break;
     case 'k': if ( cursor[1] < 90) screen[cursor[0]][cursor[1]]=c, cursor[1]++, backed=0; break;
     case 'o': cmd=1;	
    	
	}
   	
   }
 
  if ( backed==0)c=screen[cursor[0]][cursor[1]],backed=1;
   screen[cursor[0]][cursor[1]]='+';

 system("cls");
   y=0;
   while ( y < 90)
   {
   	x=0;
   	while ( x < 160)
   	{
     if ( cursor[0]==x&&cursor[1]==y&&cmd==1)
     {
     	if ( linkmask[x][y]!=0)
    	{
      strcpy(inputfile,linkfiles[linkmask[x][y]-1].filename);
    	 goto linkreload;	
      }
     }
      x++;
   }
   y++;
  }
  y=0;
  while ( y < 86 )
  {
   x=0;
   while ( x < 159 )
   {
    printf("%c%c%c%c",screen[x][y],screen[x+1][y],screen[x+2][y],screen[x+3][y]);
    
    x+=4;    
   }
   printf("\n");
   y++;
  }
 
 
 }
 
 
}