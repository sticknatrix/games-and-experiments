#include <stdio.h>

int main(void)
{
 unsigned char bild[64000];


 int y=0,x=0;
 signed int len;

 float xdelt, ydelt;
 signed int xintdelt, yintdelt;
 FILE *decoded;


 y=0;
 while ( y < 200 )
 {
  x=0;
  while ( x < 320 ) bild[x+y*320]=255, x++;
  y++;
 }

while (1)
{
 scanf("%d %d %d %d %d\n", &x, &y, &xintdelt, &yintdelt, &len);
 xdelt=(float)xintdelt/100, ydelt=(float)yintdelt/100;
 
 if ( len==0 ) break;

 len--;
  while (len >= 0 )
  {
  if ( ! (x+xdelt*len > 320 || y+ydelt*len > 200 ||
           x+xdelt*len < 0 || y+ydelt*len < 0 ) )
   {
    bild[(int)(x+xdelt*len)+(int)(y+ydelt*len)*320]=0;
   }
    len--;

  }

 
}


 decoded=fopen(".\\decoded.raw","wb");
 fwrite(bild, sizeof(unsigned char), 64000, decoded);
 fclose(decoded);


}
