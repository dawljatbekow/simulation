#include "../h/Configuration.h"


typedef struct Kreuzung
{
	int Gabelung;
	int x;
	int y;
}Kreuzung;

typedef struct Koordinaten //Datentyp für Kreuzungen und Kanten
{
	float x; // Kanten bekommen die Koordinaten zwischen den kreuzungen(wie (0,5,0))
	float y;
}Koordinaten;



int hasNorth(int value) {
	if(value==16 || value==144 || value==80 || value== 48 || value==176 || value==112 || value==208 || value==240) return(1);
	else return(0);
}


int hasSouth(int value){
	if(value==32 || value==48 || value==160 || value==96 || value==176 || value==112 || value==224 || value==240)return(1);
	else return(0);
}
int hasWest(int value){
	if(value==64 || value==80 || value==192 || value==96 || value==112 || value==208 || value==240 || value==224) return(1);
	else return(0);
}
int hasEast(int value){
	if(value==128|| value==144 || value==192 || value==160 || value==176 || value==224 || value==208 ||240)return(1);
	else return(0);
}

void xgehen(Koordinaten **snake,Koordinaten **Strecke,Kreuzung *pa,int Richtung,int s, int t){
	int i=0;
	do{ //Kontrolle ob der Roboter schon mal an der Kante gelaufen ist
		if(snake[i]->y==pa->y+Richtung/2){
			i=t;
		}
		else {
			Robot_Move(pa->x+Richtung,pa->y);
			snake[s]->y=pa->y;
			snake[s]->x=pa->x+Richtung/2;
			s++;
			(pa->y)++;
			Strecke[s]->x=pa->x;
			Strecke[s]->y=pa->y;
			i=t;
		}
		i++;

	}while(i<=t);
}

void ygehen(Koordinaten **snake,Koordinaten **Strecke,Kreuzung *pa,int Richtung, int s, int t){
	int i=0;
	do{ //Kontrolle ob der Roboter schon mal an der Kante gelaufen ist
		if(snake[i]->y==pa->y+Richtung/2){
			i=t; //abbruch
		}
		else {
			Robot_Move(pa->x,pa->y+Richtung);
			snake[s]->y=pa->y+Richtung/2;
			snake[s]->x=pa->x;
			s++;
			(pa->y)++;
			Strecke[s]->x=pa->x;
			Strecke[s]->y=pa->y;
			i=t; //abbruch
		}
		i++;

	}while(i<=t);
}

void suchen(Koordinaten snake[],Koordinaten Strecke[],Kreuzung pa){
	int n=0; //Zähler für die Anzahl an gefundenen Gegenständen
	int s=0;// Zähler für die Anzahl an passierten Kreuzungen
	int t=-1;//Zähler für Anzahl unterschiedlicher Kanten
	while(n!=3){
		pa.Gabelung= Robot_GetIntersections();
		t++;
		if(hasNorth(pa.Gabelung)!=0){
			ygehen(&snake,&Strecke,&pa,1,s,t);
		}
		if(hasSouth(pa.Gabelung)!=0){
			ygehen(&snake,&Strecke,&pa,-1,s,t);}
		if(hasEast(pa.Gabelung)!=0){
			xgehen(&snake,&Strecke,&pa,-1,s,t);}
		if(hasWest(pa.Gabelung)!=0){
			xgehen(&snake,&Strecke,&pa,1,s,t);}
		else{
			Robot_Move(Strecke[s-1].x,Strecke[s-1].y);
			pa.x=Strecke[s-1].x;
			pa.y=Strecke[s-1].y;
			s++;
			Strecke[s].x=Strecke[s-2].x;
			Strecke[s].y=Strecke[s-2].y;
			t--;
		}
	}
	while(s!=0){
		int i;
		for(i=0;i<s-1;i++){
			if (snake[s].x==snake[i].x+1 || snake[s].x==snake[i].x-1 || snake[s].y==snake[i].y+1 || snake[s].y==snake[i].y-1){
				s=i; //(*snake)[i]
				break;
			}
			Robot_Move(snake[s].x,snake[s].y);}
	}
return ;
}




int main(void) {
	struct Kreuzung pa; //Aktueller Standort
	struct Koordinaten Strecke[147];//Aufzeichnung von Kreuzungen
	struct Koordinaten snake[147];//Aufzeichnung von Kanten
	pa.x=0;
	pa.y=0;
	Strecke[0].x=0;
	Strecke[0].y=0;
	snake[0].x=0;
	snake[0].y=0;
	suchen(snake,Strecke,pa);

	return EXIT_SUCCESS;
}



