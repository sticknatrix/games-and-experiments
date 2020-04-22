/*
  Dieses Programm kodiert eine Eingabe mit einem LZ78/LZW-Ñhnlichen Verfah-
   ren

   */


#define BUFSIZE  8000 // Grî·e des Dateipuffers
#define CLRTAB 65534   // hat das Alphabet diese Grî·e erreicht, wird es ge-
                      // lîscht und neu aufgebaut. Gleichzeitig stellt dieser
                       // Wert das entsprechende Steuerzeichen fÅr den De-
                      // coder dar!

#define EOFile 65535  // der EndOfFile-Code



#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// das Alphabet /////////////////////

struct
{
 unsigned long long num_entries;
 unsigned len_longest_entry;

 struct
 {
  unsigned len;
  unsigned char *entry;

 } entry[CLRTAB-256]; // die ersten 256 Bytes werden nicht gespeichert, da sie ohne-
                      //  hin vordefiniert sind.

}alphabet;

/// bis hier... ////////////////////////

/*
  find_entry sucht das Alphabeth nach einer angegebenen Zeichenkette ab.

  */

signed long int find_entry( unsigned char *code, unsigned len)
{
 unsigned long long entry_c, cmp_ctr;
 char equal_flag;


 if( len==0) return -2;
 if( len==1)
 {
  return (signed long int)*code;
 }


 entry_c=256;

 while( entry_c <= alphabet.num_entries )
 {

  if( alphabet.entry[entry_c-256].len == len )
  {
   equal_flag=1;
   cmp_ctr=0;

   while( cmp_ctr < len)
   {

    if( *(alphabet.entry[entry_c-256].entry+cmp_ctr) != *(code+cmp_ctr) )
    {
     equal_flag=0; break;
    }

    ++cmp_ctr;
   }

   if( equal_flag==1)
   {
    return entry_c;// logische Indexnummer zurÅckgeben
   }

  }

  entry_c++;
 }

 return -1;
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
    { printf("RAM alle!"); return 1; }

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
 alphabet.len_longest_entry=1;

}

/*
  put_code schreibt eine Sequenz von 12Bit-Codes in die angegebene Datei

  */

FILE *ofile;


int put_code( char *file, signed long code )
{
 
 static unsigned file_opened=0;
 static unsigned char buf_1, buf_2, buf_3, count=0;


 if( file_opened==0)
 {
  ofile=fopen( file, "wb" );
  if( ofile==NULL )
  {
   fclose(ofile);
   return 1;
  }
  file_opened=1;

 }

 if( code== -1 )
 {
   buf_1=0xFF;// das Ende der Datei markieren
   buf_2=0xFF;

  fputc(buf_1, ofile);
   fputc(buf_2, ofile);

   fclose(ofile), file_opened=0;
  }


  buf_1=(unsigned char)  (code%256);
  buf_2=(unsigned char) ((code/256))%256;

  fputc(buf_1, ofile);
  fputc(buf_2, ofile);
 
 count++;
 count&=1;
}



int main(int argc, char *argv[])
{
 /// Alphabet vorinitialisieren //////

 alphabet.len_longest_entry=1;
 alphabet.num_entries=255;

 /// bis hier... //////

 FILE *ifile;

 if( argc!=3)
 {
  printf("Syntax: %s [Eingabedatei] [Ausgabedatei]\n",argv[0] ), fflush(stdout);
  return 1;
 }
 ifile=fopen(argv[1], "rb");
 if(ifile==NULL )
 {
  printf("Konnte das angegebene File nicht finden!\n");
  return 1;
 }


 unsigned long len_input, buf_count;
 signed long int code, C;


 struct
 {
  unsigned char *buffer;
  unsigned int len_frame;
 }inbuf;

  inbuf.buffer=(unsigned char *)malloc(BUFSIZE);

 do
 {
  printf("."), fflush(stdout);
  //inbuf.buffer=(unsigned char *)malloc( BUFSIZE);

  len_input=0;
  while( len_input < BUFSIZE && !feof(ifile) )
  {
   inbuf.buffer[len_input]=fgetc(ifile); /* Shiften erhoeht Redundanz 2018 */
   ++len_input;
  }


  if( feof(ifile) ) --len_input;


  buf_count=0;

  while( buf_count < len_input)
  {
   // inbuf.len_frame=1; // so geht's deutlich schneller->
   code= *(inbuf.buffer+buf_count), inbuf.len_frame=2;

   while( (C=find_entry( inbuf.buffer+buf_count, inbuf.len_frame)) >=0 &&
           alphabet.len_longest_entry>= inbuf.len_frame &&
            inbuf.len_frame+buf_count < len_input )
   {
    code=(C), ++inbuf.len_frame;
   }

   put_code( argv[2], code );

   
   
   if( append_entry( inbuf.buffer+buf_count, inbuf.len_frame)==1 )
   {
    printf("\nkein freier Speicher mehr!\n"), fflush(stdout);
    //free( inbuf.buffer);
    return 1;
   }

   if( inbuf.len_frame > alphabet.len_longest_entry) ++alphabet.len_longest_entry;

   buf_count+=(inbuf.len_frame-1 ==0 ? 1 : inbuf.len_frame-1 );


   if( alphabet.num_entries >= CLRTAB )
   {
    destroy_alphabet();
    put_code( argv[2], CLRTAB );
   }

  }
  //free(inbuf.buffer) ;
 }while( !feof(ifile) );

 put_code( argv[2], -1 ); //Ausgabedatei schlie·en, Operationen abschlie·en
 fclose(ifile);
 destroy_alphabet();
 return 0;

}
