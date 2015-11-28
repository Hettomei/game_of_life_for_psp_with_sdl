/* 
 * Jeux de la vie PSP
 * par Timothée
 * le 31.08.2009 06:00:08
 */

#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"

#define printf	pspDebugScreenPrintf
#define CELLX 48//480 //nbr de cellule horizontal
#define CELLY 27//272 //nbr de cellule verticale

SDL_Surface *ecran, *pix, *reset;
SDL_Rect position;
SceCtrlData pad;
int TAILLE = 10; //taille cellule carré de TAILLE pixel

struct cellule{
	char a[CELLX+2][CELLY+2];  //affiché a lécran
	char b[CELLX+2][CELLY+2]; //tampon
};
 struct cellule cell; 
  
void init();
void init2();
void bouge();
void calcul_cote();
void calcul_centre();
void maj_case();

void dessine_mort(int xx, int yy);
void dessine_vivante(int xx, int yy);

int main(int argc, char *argv[]){
	
	int fin=1;
	srand(time(NULL));

	init();
	
 	while(fin==1){
    	sceCtrlReadBufferPositive(&pad, 1); 

		if (pad.Buttons != 0){
			
			if (pad.Buttons & PSP_CTRL_CROSS){
				bouge();
			} 
			
			if (pad.Buttons & PSP_CTRL_CIRCLE){
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				sceCtrlReadBufferPositive(&pad, 1); 
				bouge();
			} 
			
			if (pad.Buttons & PSP_CTRL_SQUARE){
				init2();
			} 
			
		/*	if (pad.Buttons & PSP_CTRL_UP){   		   		
 			}
			if (pad.Buttons & PSP_CTRL_DOWN){
   		   	}
 
			if (pad.Buttons & PSP_CTRL_LEFT){
			} 
			if (pad.Buttons & PSP_CTRL_RIGHT){
			}      */
			if (pad.Buttons & PSP_CTRL_START){
			fin=0; //quitt
			}
		/*	if (pad.Buttons & PSP_CTRL_SELECT){
			}
			if (pad.Buttons & PSP_CTRL_LTRIGGER){
			}
			if (pad.Buttons & PSP_CTRL_RTRIGGER){
			}*/
		}
	}

	/* Clean up the SDL library */
	SDL_FreeSurface(pix);
	SDL_Quit();
	sceKernelExitGame();
	return(0);
}

void init(){
	int j;
	int i;
	pspDebugScreenInit();
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	
	SDL_Init(SDL_INIT_VIDEO);
	ecran = SDL_SetVideoMode(480, 272, 32, SDL_FULLSCREEN);
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); //couleur AP blanc
	pix=SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE, TAILLE, 32, 0, 0, 0, 0); // Allocation de la surface /largeur / hauteur /nb_de_bit
	reset=SDL_CreateRGBSurface(SDL_HWSURFACE, 480, 272, 32, 0, 0, 0, 0);

	//nombreMystere = (rand() % (MAX - MIN + 1)) + MIN;
		for (j=0;j<=CELLY;j++){
			for (i=0;i<=CELLX;i++){
				if((rand() % 2)==1){
					dessine_vivante(i,j);
				}
			}
		}	
/*	dessine_vivante(5,5);
	dessine_vivante(6,6);
	dessine_vivante(4,7);
	dessine_vivante(5,7);
	dessine_vivante(6,7); */
	
	SDL_Flip(ecran); //obligatoire;
}
/*
 * Comment je m'y prend ?
 * Je parcour toutes les cases
 * quand je suis sur une vivante, j''ajoute 1 à toute celle qui son autour
 * Puis je refait le tour, et les case 
 * 0,1 -> morte
 * 2 -> meme état qu'avant
 * 3-> vie
 * 4 5 6 7 8 -> meurt 
 */
void bouge(){

	calcul_cote(); 	/*décompisition en coin, coté, centre pour éviter des if à répétition*/
	calcul_centre();
	maj_case();
	
	
	SDL_Flip(ecran);
}
void calcul_cote(){
	
	int i;
		
	/////// LES 4 COINS
	if (cell.a[0][0] == 1){ //haut gauche
		cell.b[1][0]++;
		cell.b[1][1]++;
		cell.b[0][1]++;
	}
	if (cell.a[CELLX][0] == 1){ //haut droite
		cell.b[CELLX-1][0]++;
		cell.b[CELLX-1][1]++;
		cell.b[CELLX][1]++;
	}
	if (cell.a[0][CELLY] == 1){ //bas gauche
		cell.b[0][CELLY-1]++;
		cell.b[1][CELLY-1]++;
		cell.b[1][CELLY]++;
	}
	if (cell.a[CELLX][CELLY] == 1){ //bas droite
		cell.b[CELLX][CELLY-1]++;
		cell.b[CELLX-1][CELLY-1]++;
		cell.b[CELLX-1][CELLY]++;
	}
	
	////Coté haut
	for (i=1;i<CELLX;i++){
		if(cell.a[i][0] == 1){
			cell.b[i-1][0]++;
			cell.b[i+1][0]++;
			cell.b[i-1][1]++;
			cell.b[i][1]++;
			cell.b[i+1][1]++;
		}
		//bas
		if(cell.a[i][CELLY] == 1){
			cell.b[i-1][CELLY]++;
			cell.b[i+1][CELLY]++;
			cell.b[i-1][CELLY-1]++;
			cell.b[i][CELLY-1]++;
			cell.b[i+1][CELLY-1]++;
		}
	}
	
	////Coté gauche
	for (i=1;i<CELLY;i++){
		if(cell.a[0][i] == 1){
			cell.b[0][i-1]++;
			cell.b[1][i-1]++;
			cell.b[1][i]++;
			cell.b[0][i+1]++;
			cell.b[1][i+1]++;
		}
		//droite
		if(cell.a[CELLX][i] == 1){
			cell.b[CELLX][i-1]++;
			cell.b[CELLX-1][i-1]++;
			cell.b[CELLX-1][i]++;
			cell.b[CELLX-1][i+1]++;
			cell.b[CELLX][i+1]++;
		}
	}
}
void calcul_centre(){
	int i;
	int j;
	//Tout ce qu'il y a au centre
	for (j=1;j<CELLY;j++){
		for (i=1;i<CELLX;i++){
			if(cell.a[i][j] == 1){
				cell.b[i-1][j-1]++;
				cell.b[i][j-1]++;
				cell.b[i+1][j-1]++;
				cell.b[i-1][j]++;
				cell.b[i+1][j]++;
				cell.b[i-1][j+1]++;
				cell.b[i][j+1]++;
				cell.b[i+1][j+1]++;
			}
		}
	}	
}
void maj_case(){
	int i;
	int j;
//On met à jour cell.a[X][Y]
	for (j=0;j<=CELLY;j++){
		for (i=0;i<=CELLX;i++){
			if(cell.b[i][j]==0){
				if (cell.a[i][j]==1){
					dessine_mort(i,j);
				}
				continue;
			} //pour éviter de perdre du temps
			
			if(cell.b[i][j]==2){ //reste ds mm état
				cell.b[i][j]=0;
				continue;
			}
			
			if(cell.b[i][j]==1){ //0 ou 1 voisine -> morte
				if (cell.a[i][j]==1){
					dessine_mort(i,j);
				}
				cell.b[i][j]=0;
				continue;
			}
			
			if(cell.b[i][j]==3){ //3 voisine -->vie ou reste vivante
				if (cell.a[i][j]==0){
					dessine_vivante(i,j);
				}
				cell.b[i][j]=0;
				continue;
			}
			
			if(cell.b[i][j]>3){ //Plus de 3 voisine -> mort
				if (cell.a[i][j]==1){
					dessine_mort(i,j);
				}
				cell.b[i][j]=0;
				continue;
			}
		}
	}
}
void dessine_mort(xx,yy){
	cell.a[xx][yy]=0;
	position.x = xx*TAILLE;
	position.y = yy*TAILLE;
	SDL_FillRect(pix, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); //on remplit de blanc (pour effacer car AP blanc)
	SDL_BlitSurface(pix, NULL, ecran, &position); //on colle care blanc;
}
void dessine_vivante(xx,yy){
	cell.a[xx][yy]=1;
	position.x = xx*TAILLE;
	position.y = yy*TAILLE;
	SDL_FillRect(pix, NULL, SDL_MapRGB(ecran->format, 0, 0, 0)); //on remplit de noir (pour effacer car AP blanc)
	SDL_BlitSurface(pix, NULL, ecran, &position); //on colle care noir;
}
void init2(){
	int i;
	int j;
	position.x = 0;
	position.y = 0;
	SDL_FillRect(reset, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
	SDL_BlitSurface(reset, NULL, ecran, &position);
	
	for (j=0;j<=CELLY;j++){
		for (i=0;i<=CELLX;i++){
			if((rand() % 2)==1){
				cell.a[i][j]=0;
				cell.b[i][j]=0;
			}
		}
	}
	
	for (j=0;j<=CELLY;j++){
		for (i=0;i<=CELLX;i++){
			if((rand() % 2)==1){
				dessine_vivante(i,j);
			}
		}
	}

	SDL_Flip(ecran);
}
