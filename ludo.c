/*
 I believe it is almost the end product.
 You might debug it on your own if you please.
 */

#include <stdio.h>
#include <stdlib.h>

#define NUM_PLAYERS 2
#define PL1 0
#define PL2 1


 unsigned char strecke[37];
 unsigned char goal1[4];
 unsigned char goal2[4];

 int winner, steps;


struct
{
 signed int pos;
 signed int goal;
 signed int home; 

} huetchen[NUM_PLAYERS][4];





 unsigned char vstrecke[37];
 unsigned char vgoal1[4];
 unsigned char vgoal2[4];

 signed int vwinner, vsteps;


struct
{
 signed int pos;
 signed int goal;
 signed int home; 

} vhuetchen[NUM_PLAYERS][4];



int evaluate_board(void)
{

  signed int evalret;
  
  int n=0;    
  while ( n < 4 ) 
  {
   if ( vgoal1[n]!=0 ) evalret--;
   if ( vgoal2[n]!=0 ) evalret++;
   if ( vhuetchen[PL1][n].home!= 0) evalret--;
   if ( vhuetchen[PL2][n].home!= 0) evalret++;
   n++;
 }
 
 if ( evalret > 0 ) return evalret; else return 0;
	
}


int vgame_finished(void)
{
 int n=0;
 while ( n < NUM_PLAYERS )
 {
  if ( vhuetchen[n][0].goal!=-1&&
       vhuetchen[n][1].goal!=-1&&
       vhuetchen[n][2].goal!=-1&&
       vhuetchen[n][3].goal!=-1) return n;
    n++;
 }
 return -1;
}


int vkickopponent(int player, int stein)
{
  if ( player==PL1 )
  {
   if ( (vstrecke[huetchen[PL1][stein].pos%28]&16)!=0 )  vhuetchen[PL2][0].home=1, vstrecke[huetchen[PL1][stein].pos%28]^=16,vhuetchen[PL2][0].pos=-1;
   if ( (vstrecke[huetchen[PL1][stein].pos%28]&32)!=0 )  vhuetchen[PL2][1].home=1, vstrecke[huetchen[PL1][stein].pos%28]^=32,vhuetchen[PL2][1].pos=-1;
   if ( (vstrecke[huetchen[PL1][stein].pos%28]&64)!=0 )  vhuetchen[PL2][2].home=1, vstrecke[huetchen[PL1][stein].pos%28]^=64,vhuetchen[PL2][2].pos=-1;
   if ( (vstrecke[huetchen[PL1][stein].pos%28]&128)!=0 )  vhuetchen[PL2][3].home=1, vstrecke[huetchen[PL1][stein].pos%28]^=128,vhuetchen[PL2][3].pos=-1;

   }
   else
  if ( player==PL2 )
  {
  	
  	
   if ( (vstrecke[huetchen[PL2][stein].pos%28]&1)!=0 )  vhuetchen[PL1][0].home=1, vstrecke[huetchen[PL2][stein].pos%28]^=1,vhuetchen[PL1][0].pos=-1;
   if ( (vstrecke[huetchen[PL2][stein].pos%28]&2)!=0 )  vhuetchen[PL1][1].home=1, vstrecke[huetchen[PL2][stein].pos%28]^=2,vhuetchen[PL1][1].pos=-1;
   if ( (vstrecke[huetchen[PL2][stein].pos%28]&4)!=0 )  vhuetchen[PL1][2].home=1, vstrecke[huetchen[PL2][stein].pos%28]^=4,vhuetchen[PL1][2].pos=-1;
   if ( (vstrecke[huetchen[PL2][stein].pos%28]&8)!=0 )  vhuetchen[PL1][3].home=1, vstrecke[huetchen[PL2][stein].pos%28]^=8,vhuetchen[PL1][3].pos=-1;

   }
}


int vcheckamove(int player, int stein, int steps)
{

 if ( player==PL1 )
 {
 if ( steps==6 )
 {
  if ( vhuetchen[player][stein].home!=0 )
  {
    if ( (vstrecke[0]&0xf) != 0 ) return 1;
  	
  }  

 }else
   if ( vhuetchen[player][stein].home==1 ) return 1;
   
 
 if ( vhuetchen[player][stein].pos!= -1 && vhuetchen[player][stein].pos+steps <= 27)
  {
    if ( (vstrecke[huetchen[player][stein].pos+steps]&0xf) !=0 ) return 1;
  }
  else
  if ( vhuetchen[player][stein].pos!= -1 && vhuetchen[player][stein].pos+steps > 27 && vhuetchen[player][stein].pos+steps<=31)
  {
    if ( (vgoal1[(huetchen[player][stein].pos+steps-1)-27]&0xf) !=0 ) return 1;
  }
  else
  if ( vhuetchen[player][stein].goal!= -1 && vhuetchen[player][stein].goal+steps <= 3 )
  {
    if ( (vgoal1[(huetchen[player][stein].goal+steps)]&0xf) !=0 ) return 1;
  } 
  
  if ( vhuetchen[player][stein].goal!= -1 && vhuetchen[player][stein].goal+steps >= 3 ) return 1;
  if ( vhuetchen[player][stein].pos!= -1 && vhuetchen[player][stein].pos+steps>31 )return 1;
  
  
 }


 if ( player==PL2 )
 {
 if ( steps==6 )
 {
  if ( vhuetchen[player][stein].home!=0 )
  {
    if ( player==PL2)
    if ( (vstrecke[9]&0xf0) != 0 ) return 1;
  }
 }else
   if ( vhuetchen[player][stein].home==1 ) return 1;

  if ( vhuetchen[player][stein].pos!= -1 && vhuetchen[player][stein].pos+steps <= 36)
  {
    if ( (vstrecke[huetchen[player][stein].pos+steps]&0xf0) !=0 ) return 1;
  }
  else
  if ( vhuetchen[player][stein].pos!= -1 && vhuetchen[player][stein].pos+steps > 36&&vhuetchen[player][stein].pos+steps<=40)
  {
    if ( (vgoal2[(huetchen[player][stein].pos+steps-1)-36]&0xf) !=0 ) return 1;
  }
  else
  if ( vhuetchen[player][stein].goal!= -1 && vhuetchen[player][stein].goal+steps <= 3 )
  {
    if ( (goal2[(huetchen[player][stein].goal+steps)]&0xf) !=0 ) return 1;
  }
  
  if ( vhuetchen[player][stein].goal!= -1 && vhuetchen[player][stein].goal+steps >= 3 ) return 1;
  if ( vhuetchen[player][stein].pos!= -1 &&vhuetchen[player][stein].pos+steps>40) return 1;
   
 }


  return 0; 
}


int vdoamove(int player, int stein, int steps)
{
	
	if ( vcheckamove(player,stein,steps)==1 ) return 2;


 if ( vhuetchen[player][stein].home!= 0 )
 {
   vhuetchen[player][stein].home=0;
   if ( player==PL1&&stein==0)vhuetchen[player][stein].pos=0, vstrecke[0]^=1;
   if ( player==PL1&&stein==1)vhuetchen[player][stein].pos=0, vstrecke[0]^=2;
   if ( player==PL1&&stein==2)vhuetchen[player][stein].pos=0, vstrecke[0]^=4;
   if ( player==PL1&&stein==3)vhuetchen[player][stein].pos=0, vstrecke[0]^=8;
   if ( player==PL2&&stein==0)vhuetchen[player][stein].pos=9, vstrecke[9]^=16;
   if ( player==PL2&&stein==1)vhuetchen[player][stein].pos=9, vstrecke[9]^=32;
   if ( player==PL2&&stein==2)vhuetchen[player][stein].pos=9, vstrecke[9]^=64;
   if ( player==PL2&&stein==3)vhuetchen[player][stein].pos=9, vstrecke[9]^=128;
   vkickopponent(player,stein);
   
 } 
 else
 if ( vhuetchen[player][stein].pos!= -1 )
 {
  while ( steps> 0 )
  {
  	
  	  if ( vcheckamove(PL1,stein,steps)==0 && player==PL1 && vhuetchen[player][stein].pos == 27&& steps>0)
   {
   	if ( stein==0) vstrecke[huetchen[player][stein].pos%28]^=1;  
    if ( stein==1) vstrecke[huetchen[player][stein].pos%28]^=2;  
    if ( stein==2) vstrecke[huetchen[player][stein].pos%28]^=4;  
    if ( stein==3) vstrecke[huetchen[player][stein].pos%28]^=8;
    
        vhuetchen[player][stein].goal=steps-1, steps=0, vhuetchen[player][stein].pos=-1;
    if ( stein==0) vgoal1[huetchen[player][stein].goal]=1;  
    if ( stein==1) vgoal1[huetchen[player][stein].goal]=2;  
    if ( stein==2) vgoal1[huetchen[player][stein].goal]=4;  
    if ( stein==3) vgoal1[huetchen[player][stein].goal]=8;
        
    break;
   }
   else
   if ( vcheckamove(PL2,stein,steps)==0 && player==PL2 && vhuetchen[player][stein].pos ==36&& steps>0)
   {
   	if ( stein==0) vstrecke[huetchen[player][stein].pos%28]^=16;  
    if ( stein==1) vstrecke[huetchen[player][stein].pos%28]^=32;  
    if ( stein==2) vstrecke[huetchen[player][stein].pos%28]^=64;  
    if ( stein==3) vstrecke[huetchen[player][stein].pos%28]^=128;
    
   
	    vhuetchen[player][stein].goal=steps-1, steps=0, vhuetchen[player][stein].pos=-1;
    if ( stein==0) vgoal2[huetchen[player][stein].goal]=1;  
    if ( stein==1) vgoal2[huetchen[player][stein].goal]=2;  
    if ( stein==2) vgoal2[huetchen[player][stein].goal]=4;  
    if ( stein==3) vgoal2[huetchen[player][stein].goal]=8;
    
    
    break;
   }
   
      vkickopponent(player,stein);
   	
    if ( player==PL1 &&vhuetchen[player][stein].pos>-1)
	{
	//strecke[ huetchen[player][stein].pos]=0;
     if ( stein==0 && player==PL1 ) vstrecke[ huetchen[player][stein].pos%28]^=1;
     if ( stein==1 && player==PL1 ) vstrecke[ huetchen[player][stein].pos%28]^=2;
     if ( stein==2 && player==PL1 ) vstrecke[ huetchen[player][stein].pos%28]^=4;
     if ( stein==3 && player==PL1 ) vstrecke[ huetchen[player][stein].pos%28]^=8;
    
	
    }
	if ( player==PL2 &&vhuetchen[player][stein].pos >-1 )
	{
	 //strecke[ huetchen[player][stein].pos]=0; 	
     if ( stein==0 && player==PL2 ) vstrecke[ huetchen[player][stein].pos%28]^=16;
     if ( stein==1 && player==PL2 ) vstrecke[ huetchen[player][stein].pos%28]^=32;
     if ( stein==2 && player==PL2 ) vstrecke[ huetchen[player][stein].pos%28]^=64;
     if ( stein==3 && player==PL2 ) vstrecke[ huetchen[player][stein].pos%28]^=128;
    }
  
    if ( vhuetchen[player][stein].pos!=-1)vhuetchen[player][stein].pos++, steps--;

    if ( stein==0 && player==PL1 ) vstrecke[ vhuetchen[player][stein].pos%28]^=1;
    if ( stein==1 && player==PL1 ) vstrecke[ vhuetchen[player][stein].pos%28]^=2;
    if ( stein==2 && player==PL1 ) vstrecke[ vhuetchen[player][stein].pos%28]^=4;
    if ( stein==3 && player==PL1 ) vstrecke[ vhuetchen[player][stein].pos%28]^=8;

    if ( stein==0 && player==PL2 ) vstrecke[ vhuetchen[player][stein].pos%28]^=16;
    if ( stein==1 && player==PL2 ) vstrecke[ vhuetchen[player][stein].pos%28]^=32;
    if ( stein==2 && player==PL2 ) vstrecke[ vhuetchen[player][stein].pos%28]^=64;
    if ( stein==3 && player==PL2 ) vstrecke[ vhuetchen[player][stein].pos%28]^=128;
 
   vkickopponent(player,stein);
   
   } 


 }
 else
 if ( vhuetchen[player][stein].goal!= -1 )
 {
   if ( player==PL1)
   {
   	vgoal1[huetchen[player][stein].goal]=0;
      vhuetchen[player][stein].goal+=steps;
    if ( stein==0) vgoal1[vhuetchen[player][stein].goal]=1;  
    if ( stein==1) vgoal1[vhuetchen[player][stein].goal]=2;  
    if ( stein==2) vgoal1[vhuetchen[player][stein].goal]=4;  
    if ( stein==3) vgoal1[vhuetchen[player][stein].goal]=8;
   }
   else
   if ( player==PL2)
   {
   	vgoal2[vhuetchen[player][stein].goal]=0;
     vhuetchen[player][stein].goal+=steps;
    if ( stein==0) vgoal2[vhuetchen[player][stein].goal]=1;  
    if ( stein==1) vgoal2[vhuetchen[player][stein].goal]=2;  
    if ( stein==2) vgoal2[vhuetchen[player][stein].goal]=4;  
    if ( stein==3) vgoal2[vhuetchen[player][stein].goal]=8;
   }
   

 }


}





int vmain(int move, int rec_depth, int player)
{
	signed int retcode, retcode_n[4]={0,0,0,0}, retcode_a=0,buf, nomovepossible;
	int n;
	
	if ( rec_depth > 4) return 0;
	
 unsigned char bstrecke[37];
 unsigned char bgoal1[4];
 unsigned char bgoal2[4];

 int bwinner, bsteps;


struct
{
 signed int pos;
 signed int goal;
 signed int home; 

} bhuetchen[NUM_PLAYERS][4];




	n=0;
  while ( n < 37 ) bstrecke[n]=vstrecke[n],n++;
  n=0;
  while ( n < 4 ) bgoal1[n]=vgoal1[n], bgoal2[n]=vgoal2[n],
                  bhuetchen[PL1][n].home=vhuetchen[PL1][n].home,
                  bhuetchen[PL2][n].home=vhuetchen[PL2][n].home, 
                  bhuetchen[PL1][n].goal=vhuetchen[PL1][n].goal, 
                  bhuetchen[PL2][n].goal=vhuetchen[PL2][n].goal, 
                  bhuetchen[PL1][n].pos=vhuetchen[PL1][n].pos,
                  bhuetchen[PL2][n].pos=vhuetchen[PL2][n].pos,n++;
                  bwinner=vwinner, bsteps=vsteps;


   
   int count=0;
   while ( count < 4)
   {
	if ( player==PL1)
	{
     if ( rec_depth>0 )vsteps=rand()%6+1; else vsteps=steps;
     //printf("Fuer Dich wurde eine %d geworfen\n",vsteps);
     nomovepossible=vdoamove(0,count,vsteps);
    vwinner=vgame_finished();
     if ( vwinner==PL1 ) return 0;
     else if ( vwinner==PL2) return 1;
    
    retcode_a= (buf=evaluate_board() > retcode_a ? buf : retcode_a );

   }
   else
   if ( player==PL2)
   {
    if ( rec_depth>0 )vsteps=rand()%6+1; else vsteps=steps;
     //printf("Fuer Computer wurde eine %d geworfen\n",vsteps);
     nomovepossible=vdoamove(1,count,vsteps);
    
    vwinner=vgame_finished();
     if ( vwinner==PL1 ) return 0;
     else if ( vwinner==PL2) return 1;
    
    retcode_a= (buf=evaluate_board() > retcode_a ? buf : retcode_a );

   }

    
	n=0;
  while ( n < 37 ) vstrecke[n]=bstrecke[n],n++;
  n=0;
  while ( n < 4 ) vgoal1[n]=bgoal1[n], vgoal2[n]=bgoal2[n],
                  vhuetchen[PL1][n].home=bhuetchen[PL1][n].home,
                  vhuetchen[PL2][n].home=bhuetchen[PL2][n].home, 
                  vhuetchen[PL1][n].goal=bhuetchen[PL1][n].goal, 
                  vhuetchen[PL2][n].goal=bhuetchen[PL2][n].goal, 
                  vhuetchen[PL1][n].pos=bhuetchen[PL1][n].pos,
				  vhuetchen[PL2][n].pos=bhuetchen[PL2][n].pos,
				  n++;
				  vwinner=bwinner, vsteps=bsteps;

      if ( nomovepossible!=2)
	 retcode_n[count]+=vmain(count,rec_depth+1,player^1);
     
	 count++;
	}

if ( player==PL2)
{
	count=0;retcode=0;
	
 while ( count < 4){
 if (retcode_n[count] > retcode ) retcode=retcode_n[count] ;
   count++; }
	
}


if ( player==PL1)
{
	count=0;retcode=999999;
	
 while ( count < 4){
 if (retcode_n[count] < retcode ) retcode=retcode_n[count];
 count++; }
	
}


return retcode+retcode_a;


}






int backtracksum(void)
{

 int n=0;
  int n2=0;
  
  while ( n < 37 ) vstrecke[n]=strecke[n],n++;
  n=0;
  while ( n < 4 ) vgoal1[n]=goal1[n], vgoal2[n]=goal2[n],
                  vhuetchen[PL1][n].home=huetchen[PL1][n].home,
                  vhuetchen[PL2][n].home=huetchen[PL2][n].home, 
                  vhuetchen[PL2][n].goal=huetchen[PL2][n].goal, 
                  vhuetchen[PL1][n].goal=huetchen[PL1][n].goal, 
				  vhuetchen[PL1][n].pos=huetchen[PL1][n].pos,
				  vhuetchen[PL2][n].pos=huetchen[PL2][n].pos,
				  n++;
   
   signed int evaluation[4];
   
   n=0; while ( n < 4) evaluation[n]=0, n++;
   
  n2=0;
  while ( n2 < 4)
  { 
   n=0;
   while ( n < 4)
   evaluation[n]+=vmain(n,0,1),n++;
   n2++;
  }
  
  int retcode=0;
  int retvalue=0;
  n=0;
  while ( n < 4)
  {
  	if ( retvalue < evaluation[n]) retcode=n, retvalue=evaluation[n];
  	n++;
  }

  return retcode;
}



int game_finished(void)
{
 int n=0;
 while ( n < NUM_PLAYERS )
 {
  if ( huetchen[n][0].goal!=-1&&
       huetchen[n][1].goal!=-1&&
       huetchen[n][2].goal!=-1&&
       huetchen[n][3].goal!=-1) return n;
    n++;
 }
 return -1;
}


int kickopponent(int player, int stein)
{
  if ( player==PL1 )
  {
   if ( (strecke[huetchen[PL1][stein].pos%28]&16)!=0 )  huetchen[PL2][0].home=1, strecke[huetchen[PL1][stein].pos%28]^=16,huetchen[PL2][0].pos=-1,printf("kick!"),getch();
   if ( (strecke[huetchen[PL1][stein].pos%28]&32)!=0 )  huetchen[PL2][1].home=1, strecke[huetchen[PL1][stein].pos%28]^=32,huetchen[PL2][1].pos=-1,printf("kick!"),getch();
   if ( (strecke[huetchen[PL1][stein].pos%28]&64)!=0 )  huetchen[PL2][2].home=1, strecke[huetchen[PL1][stein].pos%28]^=64,huetchen[PL2][2].pos=-1,printf("kick!"),getch();
   if ( (strecke[huetchen[PL1][stein].pos%28]&128)!=0 )  huetchen[PL2][3].home=1, strecke[huetchen[PL1][stein].pos%28]^=128,huetchen[PL2][3].pos=-1,printf("kick!"),getch();

   }
   else
  if ( player==PL2 )
  {
  	
  	
   if ( (strecke[huetchen[PL2][stein].pos%28]&1)!=0 )  huetchen[PL1][0].home=1, strecke[huetchen[PL2][stein].pos%28]^=1,huetchen[PL1][0].pos=-1,printf("kick!"),getch();
   if ( (strecke[huetchen[PL2][stein].pos%28]&2)!=0 )  huetchen[PL1][1].home=1, strecke[huetchen[PL2][stein].pos%28]^=2,huetchen[PL1][1].pos=-1,printf("kick!"),getch();
   if ( (strecke[huetchen[PL2][stein].pos%28]&4)!=0 )  huetchen[PL1][2].home=1, strecke[huetchen[PL2][stein].pos%28]^=4,huetchen[PL1][2].pos=-1,printf("kick!"),getch();
   if ( (strecke[huetchen[PL2][stein].pos%28]&8)!=0 )  huetchen[PL1][3].home=1, strecke[huetchen[PL2][stein].pos%28]^=8,huetchen[PL1][3].pos=-1,printf("kick!"),getch();

   }
}


int checkamove(int player, int stein, int steps)
{

 if ( player==PL1 )
 {
 if ( steps==6 )
 {
  if ( huetchen[player][stein].home!=0 )
  {
    if ( (strecke[0]&0xf) != 0 ) return 1;
  	
  }  

 }else
   if ( huetchen[player][stein].home==1 ) return 1;
   
 
 if ( huetchen[player][stein].pos!= -1 && huetchen[player][stein].pos+steps <= 27)
  {
    if ( (strecke[huetchen[player][stein].pos+steps]&0xf) !=0 ) return 1;
  }
  else
  if ( huetchen[player][stein].pos!= -1 && huetchen[player][stein].pos+steps > 27 && huetchen[player][stein].pos+steps<=31)
  {
    if ( (goal1[(huetchen[player][stein].pos+steps-1)-27]&0xf) !=0 ) return 1;
  }
  else
  if ( huetchen[player][stein].goal!= -1 && huetchen[player][stein].goal+steps <= 3 )
  {
    if ( (goal1[(huetchen[player][stein].goal+steps)]&0xf) !=0 ) return 1;
  } 
  
  if ( huetchen[player][stein].goal!= -1 && huetchen[player][stein].goal+steps >= 3 ) return 1;
  if ( huetchen[player][stein].pos!= -1 && huetchen[player][stein].pos+steps>31 )return 1;
  
  
 }


 if ( player==PL2 )
 {
 if ( steps==6 )
 {
  if ( huetchen[player][stein].home!=0 )
  {
    if ( player==PL2)
    if ( (strecke[9]&0xf0) != 0 ) return 1;
  }
 }else
   if ( huetchen[player][stein].home==1 ) return 1;

  if ( huetchen[player][stein].pos!= -1 && huetchen[player][stein].pos+steps <= 36)
  {
    if ( (strecke[huetchen[player][stein].pos+steps]&0xf0) !=0 ) return 1;
  }
  else
  if ( huetchen[player][stein].pos!= -1 && huetchen[player][stein].pos+steps > 36&&huetchen[player][stein].pos+steps<=40)
  {
    if ( (goal2[(huetchen[player][stein].pos+steps-1)-36]&0xf) !=0 ) return 1;
  }
  else
  if ( huetchen[player][stein].goal!= -1 && huetchen[player][stein].goal+steps <= 3 )
  {
    if ( (goal2[(huetchen[player][stein].goal+steps)]&0xf) !=0 ) return 1;
  }
  
  if ( huetchen[player][stein].goal!= -1 && huetchen[player][stein].goal+steps >= 3 ) return 1;
  if ( huetchen[player][stein].pos!= -1 &&huetchen[player][stein].pos+steps>40) return 1;
   
 }


  return 0; 
}


int doamove(int player, int stein, int steps)
{

 if ( checkamove(player,0,steps)==1 &&  checkamove(player,1,steps)==1 &&
      checkamove(player,2,steps)==1 && checkamove(player,3,steps)==1 ) { printf("Kein Zug moeglich! Taste...\n"); getch();return -1; }

if ( player==PL1)
{
 printf("Huetchen auswaehlen:\n");
 do { do {stein=getch(); rand();}while ( stein > '4' || stein < '1'); stein-=0x31; } while ( checkamove(player,stein,steps)==1 );

}
else
if ( player==PL2)
{
	while ( checkamove(player,stein,steps)==1 ) { stein=rand()%4; }
	
}

 if ( huetchen[player][stein].home!= 0 )
 {
   huetchen[player][stein].home=0;
   if ( player==PL1&&stein==0)huetchen[player][stein].pos=0, strecke[0]^=1;
   if ( player==PL1&&stein==1)huetchen[player][stein].pos=0, strecke[0]^=2;
   if ( player==PL1&&stein==2)huetchen[player][stein].pos=0, strecke[0]^=4;
   if ( player==PL1&&stein==3)huetchen[player][stein].pos=0, strecke[0]^=8;
   if ( player==PL2&&stein==0)huetchen[player][stein].pos=9, strecke[9]^=16;
   if ( player==PL2&&stein==1)huetchen[player][stein].pos=9, strecke[9]^=32;
   if ( player==PL2&&stein==2)huetchen[player][stein].pos=9, strecke[9]^=64;
   if ( player==PL2&&stein==3)huetchen[player][stein].pos=9, strecke[9]^=128;
   kickopponent(player,stein);
   
 } 
 else
 if ( huetchen[player][stein].pos!= -1 )
 {
  while ( steps> 0 )
  {
  	
  	  if ( checkamove(PL1,stein,steps)==0 && player==PL1 && huetchen[player][stein].pos == 27&& steps>0)
   {
   	if ( stein==0) strecke[huetchen[player][stein].pos%28]^=1;  
    if ( stein==1) strecke[huetchen[player][stein].pos%28]^=2;  
    if ( stein==2) strecke[huetchen[player][stein].pos%28]^=4;  
    if ( stein==3) strecke[huetchen[player][stein].pos%28]^=8;
    
        huetchen[player][stein].goal=steps-1, steps=0, huetchen[player][stein].pos=-1;
    if ( stein==0) goal1[huetchen[player][stein].goal]=1;  
    if ( stein==1) goal1[huetchen[player][stein].goal]=2;  
    if ( stein==2) goal1[huetchen[player][stein].goal]=4;  
    if ( stein==3) goal1[huetchen[player][stein].goal]=8;
        
    break;
   }
   else
   if ( checkamove(PL2,stein,steps)==0 && player==PL2 && huetchen[player][stein].pos ==36&& steps>0)
   {
   	if ( stein==0) strecke[huetchen[player][stein].pos%28]^=16;  
    if ( stein==1) strecke[huetchen[player][stein].pos%28]^=32;  
    if ( stein==2) strecke[huetchen[player][stein].pos%28]^=64;  
    if ( stein==3) strecke[huetchen[player][stein].pos%28]^=128;
    
   
	    huetchen[player][stein].goal=steps-1, steps=0, huetchen[player][stein].pos=-1;
    if ( stein==0) goal2[huetchen[player][stein].goal]=1;  
    if ( stein==1) goal2[huetchen[player][stein].goal]=2;  
    if ( stein==2) goal2[huetchen[player][stein].goal]=4;  
    if ( stein==3) goal2[huetchen[player][stein].goal]=8;
    
    
    break;
   }
   
      kickopponent(player,stein);
   	
    if ( player==PL1 &&huetchen[player][stein].pos>-1)
	{
	//strecke[ huetchen[player][stein].pos]=0;
     if ( stein==0 && player==PL1 ) strecke[ huetchen[player][stein].pos%28]^=1;
     if ( stein==1 && player==PL1 ) strecke[ huetchen[player][stein].pos%28]^=2;
     if ( stein==2 && player==PL1 ) strecke[ huetchen[player][stein].pos%28]^=4;
     if ( stein==3 && player==PL1 ) strecke[ huetchen[player][stein].pos%28]^=8;
    
	
    }
	if ( player==PL2 &&huetchen[player][stein].pos >-1 )
	{
	 //strecke[ huetchen[player][stein].pos]=0; 	
     if ( stein==0 && player==PL2 ) strecke[ huetchen[player][stein].pos%28]^=16;
     if ( stein==1 && player==PL2 ) strecke[ huetchen[player][stein].pos%28]^=32;
     if ( stein==2 && player==PL2 ) strecke[ huetchen[player][stein].pos%28]^=64;
     if ( stein==3 && player==PL2 ) strecke[ huetchen[player][stein].pos%28]^=128;
    }
  
    if ( huetchen[player][stein].pos!=-1)huetchen[player][stein].pos++, steps--;

    if ( stein==0 && player==PL1 ) strecke[ huetchen[player][stein].pos%28]^=1;
    if ( stein==1 && player==PL1 ) strecke[ huetchen[player][stein].pos%28]^=2;
    if ( stein==2 && player==PL1 ) strecke[ huetchen[player][stein].pos%28]^=4;
    if ( stein==3 && player==PL1 ) strecke[ huetchen[player][stein].pos%28]^=8;

    if ( stein==0 && player==PL2 ) strecke[ huetchen[player][stein].pos%28]^=16;
    if ( stein==1 && player==PL2 ) strecke[ huetchen[player][stein].pos%28]^=32;
    if ( stein==2 && player==PL2 ) strecke[ huetchen[player][stein].pos%28]^=64;
    if ( stein==3 && player==PL2 ) strecke[ huetchen[player][stein].pos%28]^=128;
 
   kickopponent(player,stein);
   
   } 


 }
 else
 if ( huetchen[player][stein].goal!= -1 )
 {
   if ( player==PL1)
   {
   	goal1[huetchen[player][stein].goal]=0;
      huetchen[player][stein].goal+=steps;
    if ( stein==0) goal1[huetchen[player][stein].goal]=1;  
    if ( stein==1) goal1[huetchen[player][stein].goal]=2;  
    if ( stein==2) goal1[huetchen[player][stein].goal]=4;  
    if ( stein==3) goal1[huetchen[player][stein].goal]=8;
   }
   else
   if ( player==PL2)
   {
   	goal2[huetchen[player][stein].goal]=0;
     huetchen[player][stein].goal+=steps;
    if ( stein==0) goal2[huetchen[player][stein].goal]=1;  
    if ( stein==1) goal2[huetchen[player][stein].goal]=2;  
    if ( stein==2) goal2[huetchen[player][stein].goal]=4;  
    if ( stein==3) goal2[huetchen[player][stein].goal]=8;
   }
   

 }


}





int main(void)
{
 int n=0;
  while ( n < 37 ) strecke[n]=0,n++;
  n=0;
  while ( n < 4 ) goal1[n]=0, goal2[n]=0,n++;


  huetchen[PL1][0].home=1,  huetchen[PL1][1].home=1,  huetchen[PL1][2].home=1,  huetchen[PL1][3].home=1;
 huetchen[PL2][0].home=1,  huetchen[PL2][1].home=1,  huetchen[PL2][2].home=1,  huetchen[PL2][3].home=1;
   
   huetchen[PL1][0].pos=-1, huetchen[PL1][1].pos=-1,huetchen[PL1][2].pos=-1, huetchen[PL1][3].pos=-1;    
   huetchen[PL1][0].goal=-1, huetchen[PL1][1].goal=-1,huetchen[PL1][2].goal=-1, huetchen[PL1][3].goal=-1;    
   
   huetchen[PL2][0].pos=-1, huetchen[PL2][1].pos=-1,huetchen[PL2][2].pos=-1, huetchen[PL2][3].pos=-1;    
   huetchen[PL2][0].goal=-1, huetchen[PL2][1].goal=-1,huetchen[PL2][2].goal=-1, huetchen[PL2][3].goal=-1;    


 unsigned char board[20][20];

  	unsigned char boardbak[20][20]={
	          {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	          {' ','X','X','X','X',' ',' ',' ',' ',' ',' ','X','X','X','X',' ',' ',' ',' ',' '},
	          {' ','X',' ',' ','X',' ',' ','X','X','X',' ','X',' ',' ','X',' ',' ',' ',' ',' '},
	          {' ','X',' ',' ','X',' ',' ','X',' ','X',' ','X',' ',' ','X',' ',' ',' ',' ',' '},
	          {' ','X',' ',' ','X',' ',' ','X',' ','X',' ','X',' ',' ','X',' ',' ',' ',' ',' '},
	          {' ','X','X','X','X',' ',' ','X',' ','X',' ','X','X','X','X',' ',' ',' ',' ',' '},
	          {' ',' ','X',' ',' ',' ',' ','X',' ','X',' ',' ',' ','X',' ',' ',' ',' ',' ',' '},
	          {' ','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X',' '},
	          {' ','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X',' '},
	          {' ','X',' ','X','X','X','X','X','X','X','X','X','X','X','X','X','X',' ','X',' '},
	          {' ','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','X',' '},
	          {' ','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X','X',' '},
	          {' ',' ','X',' ','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	          {' ',' ','X',' ','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	          {' ',' ','X',' ','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	          {' ',' ','X',' ','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	          {' ',' ','X','X','X',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	          {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	          {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
	          {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}};

int x=0, y=0;
int nb;
	
  while ( (winner=game_finished())==-1 )
  {
	
	y=0;
	while ( y < 20)
	{
	 x=0;
	 while ( x < 20) board[x][y]=boardbak[y][x],x++;		
	 y++;	
	}
  	
	          
	
	#define DISPLAY\
  	system("cls\n");\
  	y=0;\
  	while ( y < 20)\
  	{\
  	  x=0;\
	  while ( x < 20)\
	  {\
	    if ( board[x][y]=='X') printf("%c",177);\
	    else printf("%c",board[x][y]);\
	    \
	    x++;\
		  }\
		  printf("\n");\
  		y++;\
	  }\
	  
	  n=0;
    while ( n < 16)
    {
	 nb=(int)(n*1.0);
    
	 if ( strecke[n]==1) board[2+nb][8]='1',board[2+nb][7]=176;
	 else
	 if ( strecke[n]==2) board[2+nb][8]='2',board[2+nb][7]=176;
	 else
	 if ( strecke[n]==4) board[2+nb][8]='3',board[2+nb][7]=176;
	 else
	 if ( strecke[n]==8) board[2+nb][8]='4',board[2+nb][7]=176;
	 else
	 if ( strecke[n]==16) board[2+nb][8]='1',board[2+nb][7]=219;
	 else
	 if ( strecke[n]==32) board[2+nb][8]='2',board[2+nb][7]=219;
	 else
	 if ( strecke[n]==64) board[2+nb][8]='3',board[2+nb][7]=219;
	 else
	 if ( strecke[n]==128) board[2+nb][8]='4',board[2+nb][7]=219;
	 n++;
	 	
	}
	
	while ( n < 27)
	{
	 nb=(int)(n*1.0);
	 
	 if ( strecke[n]==1) board[19+16-nb][10]='1',board[19+16-nb][11]=176;
	 else
	 if ( strecke[n]==2) board[19+16-nb][10]='2',board[19+16-nb][11]=176;
	 else
	 if ( strecke[n]==4) board[19+16-nb][10]='3',board[19+16-nb][11]=176;
	 else
	 if ( strecke[n]==8) board[19+16-nb][10]='4',board[19+16-nb][11]=176;
	 else
	 if ( strecke[n]==16) board[19+16-nb][10]='1',board[19+16-nb][11]=219;
	 else
	 if ( strecke[n]==32) board[19+16-nb][10]='2',board[19+16-nb][11]=219;
	 else
	 if ( strecke[n]==64) board[19+16-nb][10]='3',board[19+16-nb][11]=219;
	 else
	 if ( strecke[n]==128) board[19+16-nb][10]='4',board[19+16-nb][11]=219;
	 n++;	
	}
	
	
	
     while ( n < 37)
    {
	 nb=(int)((n%27)*1.0);
    
	 if ( strecke[n]==1) board[2+nb][8]='1',board[2+nb][7]=176;
	 else
	 if ( strecke[n]==2) board[2+nb][8]='2',board[2+nb][7]=176;
	 else
	 if ( strecke[n]==4) board[2+nb][8]='3',board[2+nb][7]=176;
	 else
	 if ( strecke[n]==8) board[2+nb][8]='4',board[2+nb][7]=176;
	 else
	 if ( strecke[n]==16) board[2+nb][8]='1',board[2+nb][7]=219;
	 else
	 if ( strecke[n]==32) board[2+nb][8]='2',board[2+nb][7]=219;
	 else
	 if ( strecke[n]==64) board[2+nb][8]='3',board[2+nb][7]=219;
	 else
	 if ( strecke[n]==128) board[2+nb][8]='4',board[2+nb][7]=219;
	 n++;
	 	
	}
	
	
	if ( huetchen[PL1][0].home==1) board[2][2]='1';
	if ( huetchen[PL1][1].home==1) board[2][3]='2';
	if ( huetchen[PL1][2].home==1) board[3][2]='3';
	if ( huetchen[PL1][3].home==1) board[3][3]='4';
	
	
	
	if ( huetchen[PL2][0].home==1) board[12][2]='1';
	if ( huetchen[PL2][1].home==1) board[12][3]='2';
	if ( huetchen[PL2][2].home==1) board[13][2]='3';
	if ( huetchen[PL2][3].home==1) board[13][3]='4';
	
	if ( goal1[0]!=0) board[3][12]=176;
	if ( goal1[1]!=0) board[3][13]=176;
	if ( goal1[2]!=0) board[3][14]=176;
	if ( goal1[3]!=0) board[3][15]=176;
	
	
	if ( goal2[0]!=0) board[8][6]=219;
	if ( goal2[1]!=0) board[8][5]=219;
	if ( goal2[2]!=0) board[8][4]=219;
	if ( goal2[3]!=0) board[8][3]=219;
	
  
  DISPLAY	
  steps=rand()%6+1;
    printf("Fuer Dich wurde eine %d geworfen\n",steps);
    doamove(0,0,steps);
    
    steps=rand()%6+1;  
    
	printf("Computer hat eine %d geworfen\n",steps);
    int bufv;
    bufv=backtracksum();
    
	doamove(1,bufv,steps);
    
    
  }
  DISPLAY
  if ( winner==0) printf("Du hast gewonnen."); else printf("Computer hat gewonnen.");


}
