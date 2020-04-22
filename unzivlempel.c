/*
  dekodiert Dateien, die mit lzwpack gepackt wurden

   */


#define BUFSIZE  3000000 // Grî·e des Dateipuffers
#define CLRTAB 65534   // dieser Code bewirkt, da· das Alphabet gelîscht wird

#define EOFile 65535  // der EndOfFile-Code



#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// das Alphabet /////////////////////

struct
{
 unsigned long long num_entries;

 struct
 {
  unsigned len;
  unsigned char *entry;

 } entry[CLRTAB-256]; // die ersten 256 Bytes werden nicht gespeichert, da sie ohne-
                      //  hin vordefiniert sind.

}alphabet;

/// bis hier... ////////////////////////

/*
  find_entry gibt die entsprechende Zeichenkette zu einem Code zurÅck.

  */

typedef struct
        {
         unsigned char *data;
         unsigned int len;
        } STRING;

struct
{
 unsigned int anzahl;	
	
}statistics[65536];

STRING find_entry( unsigned long code )
{
 unsigned long long cmp_ctr;

 STRING INDIZIE;


 if( code<=255 )
 {
  INDIZIE.data=(unsigned char*) malloc(1);
  *INDIZIE.data= (unsigned char)code;
  INDIZIE.len=1;
 }
 else
 if( code <= alphabet.num_entries )
 {
  cmp_ctr= alphabet.entry[code-256].len;
  INDIZIE.len= alphabet.entry[code-256].len;
  INDIZIE.data=(unsigned char*) malloc(INDIZIE.len);

  while( cmp_ctr > 0)
  {
   --cmp_ctr;
   *(INDIZIE.data+cmp_ctr)= *(alphabet.entry[code-256].entry+cmp_ctr);
  }

 }

 return INDIZIE;


}

/*
  append_entry hÑngt einen Eintrag ans Alphabet an.
  */


char append_entry( unsigned char *code, unsigned len)
{
 alphabet.num_entries++;

 alphabet.entry[alphabet.num_entries-256].len=len;
 if( (alphabet.entry[alphabet.num_entries-256].entry=
       ( unsigned char *)malloc( len ))==NULL )
    return 1;

 do
 {
  --len;
  alphabet.entry[alphabet.num_entries-256].entry[len]=code[len];
 } while( len > 0);

 return 0;

}

void destroy_alphabet(void)
{
 while( alphabet.num_entries >255 )
 {
  free( alphabet.entry[alphabet.num_entries-256].entry);
  --alphabet.num_entries;
 }

}



int main (int argc, char *argv[])
{
 /// Alphabet vorinitialisieren //////


 alphabet.num_entries=255;

 /// bis hier... //////

 FILE *ifile;

  if ( argc!= 3) return;

 ifile=fopen( argv[1], "rb");
 if(ifile==NULL )
 {
  return 0;
 }
 FILE *ofile;
 ofile=fopen(argv[2], "wb");

 unsigned long len_input, buf_count;
 signed long int code, C;

 unsigned char to_convert[3];
 STRING decoded, next_entry;
 unsigned char *entry_1st=(unsigned char*)malloc(65537);
 unsigned int len_entry_1st;
 unsigned long length;

 char gone_thru=0;

 struct
 {
  signed int *buffer;
  unsigned int len_frame;
 }inbuf;


 inbuf.buffer=(signed int *)malloc( BUFSIZE*sizeof( signed int) );

 if( inbuf.buffer==NULL|| entry_1st==NULL ); else

  len_input=0;
  while( len_input < BUFSIZE && !feof(ifile) )
  {
   to_convert[0]=fgetc(ifile);
   to_convert[1]=fgetc(ifile);
   // fgets( to_convert, sizeof(unsigned char)*3, ifile );

    inbuf.buffer[len_input]   = to_convert[0];
    inbuf.buffer[len_input]  += (to_convert[1])*256 ;
    
    len_input++;
   

  }

  buf_count=0;


     #define THE_END(x)\
     {\
      destroy_alphabet();\
      fclose(ifile) ; fclose(ofile);\
      free(inbuf.buffer), free(entry_1st) ;\
      return x;\
     }


  while( buf_count < len_input==0 ? 0 : len_input-1 )
  {
   if( inbuf.buffer[buf_count]==CLRTAB ) destroy_alphabet();
   else
   if( inbuf.buffer[buf_count]==EOFile ) //debug!
   {
    THE_END(length)
   }
   else
   {
   	statistics[inbuf.buffer[buf_count]].anzahl++;
   	
   	
    decoded= find_entry( inbuf.buffer[buf_count] );
    //entry_1st= (unsigned char*) malloc( decoded.len);

      
    memcpy( entry_1st, decoded.data, decoded.len );
    free (decoded.data);
    len_entry_1st=decoded.len;

    // entry_1st entspricht nun dem Eintrag an der aktuellen Indexposition
    // len_entry_1st entspricht der LÑnge des Eintrags

    /*
    memcpy( outbuf, entry_1st, sizeof(unsigned char)*len_entry_1st );
    outbuf+=len_entry_1st;
    length+=len_entry_1st;
     */
     
     /*
     if ( len_entry_1st <= 1 ) printf("[I]\n");
     else
	 if ( len_entry_1st <= 2 ) printf("[2]\n");
     else
	 if ( len_entry_1st <= 3 ) printf("[3]\n");
     else
	 if ( len_entry_1st <= 4 ) printf("[4]\n");
     else
	 if ( len_entry_1st <= 5 ) printf("[5]\n");
     else                      printf("[!]\n"); */
     
     int n=0;
     
     n=0;
     
     /* Das Herzstueck 2018 auskommentiert */
     
     // if ( len_entry_1st > 4) OHNE REDUNDANZFILTER
	 while ( n < len_entry_1st ) fputc(entry_1st[n],ofile),/*printf("%c",entry_1st[n]),*/n++;
	 /////////////////////////////////////////////////
	 
	 
	 // DUMPEN 2018
     
     
     
    // Nun wird entry_1st zum Speichern des neuen Indexes gebraucht...

     //realloc( entry_1st, decoded.len+1 );


    if( inbuf.buffer[buf_count+1] <= alphabet.num_entries )
    {
     next_entry= find_entry( inbuf.buffer[buf_count+1] );
     entry_1st[len_entry_1st]= next_entry.data[0] ;
     free (next_entry.data);

     // das erste Zeichen des nÑchsten Eintrags wurde angehÑngt!
    }
    else // K[Omega]K-Fall ...
    if( inbuf.buffer[buf_count+1] == alphabet.num_entries+1 )
    {
     entry_1st[len_entry_1st]= entry_1st[0] ;
    }
    else if( inbuf.buffer[buf_count+1]== CLRTAB ||
             inbuf.buffer[buf_count+1]== EOFile   );
    else
    {
     printf("\nDie Datei wurde nicht gepackt, ist beschaedigt oder wie auch immer!\n");
     THE_END(0)

    }


    if( append_entry( entry_1st, len_entry_1st+1 )==1 )
    {
     printf("\nkein freier Speicher mehr!\n"), fflush(stdout);
     THE_END(0)
    }

    //free (entry_1st);

   }

   buf_count++;
  }
   /*
     Durch die Verwendung des Buffers wÑre das Programm nicht ohne weiteres
      in der Lage, _immer_ in der obigen Schleife das nÑchste Zeichen zu be-
     trachten... hab' das Problem gelîst!
       */
   /*    
  if(!feof(ifile) )
  {
   fseek(ifile,-3, ftell(ifile));
   gone_thru=1;
  } */
 

 THE_END(0)

}
