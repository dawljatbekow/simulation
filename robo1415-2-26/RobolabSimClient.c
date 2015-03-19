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

struct Kreuzung pa; //Aktueller Standort
struct Koordinaten Strecke[147];//Aufzeichnung von Kreuzungen
struct Koordinaten snake[147];//Aufzeichnung von Kanten
int s;
int t;
int n;

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
	if(value==128|| value==144 || value==192 || value==160 || value==176 || value==224 || value==208 ||value==240)return(1);
	else return(0);
}

int Kontrolle(float Richtungx,float Richtungy){ // Kontrolle ob der Roboter schon mal auf der Kante gelaufen ist
	int i;
	for(i=0;i<=t;i++){
		if(snake[i].y==pa.y+Richtungy/2.0 && snake[i].x==pa.x+Richtungx/2.0){
			return 0;
			break;
		}
	}
return 1;
}

void gehen(float Richtungx, float Richtungy){ //funktion für das gehen auf neuen Wegen, Richtung ist entweder |1| oder NUll,
	//wobei einer der Richtung durch das fehlen diagonaler Linien immer NUll ist
	if(Robot_Move(pa.x+Richtungx,pa.y+Richtungy)==0x02)n++; //Robot_Move wird auch in einer if abfrage ausgeführt
	s++; //Erhöhung des Zählers für Strecke
	t++; //Erhöhung des Zählers für snake
	snake[t].y=pa.y+Richtungy/2.0; //Speicherung der gegangenen Kante für die Überprüfung der schon gegangenen Kanten
	snake[t].x=pa.x+Richtungx/2.0;
	pa.y=pa.y+Richtungy; //Änderung des aktuellen Standorts
	pa.x=pa.x+Richtungx;
	Strecke[s].x=pa.x; //Speicherung des Punktes für den Rückweg
	Strecke[s].y=pa.y;
}


void suchen(){
	n=0; //Zähler für die Anzahl an gefundenen Gegenständen
	s=0;// Zähler für die Anzahl an passierten Kreuzungen
	t=-1;//Zähler für Anzahl unterschiedlicher Kanten
	while(n!=3){
		pa.Gabelung= Robot_GetIntersections(); //Einlesen der Hexadizimalen Zahl, die den Typ der Kreuzung bestimmt
		if(hasNorth(pa.Gabelung)!=0 && Kontrolle(0,1)==1) gehen(0,1); //kontrolliert ob er nach norden gehen kann und ob er da schon war
		else if(hasSouth(pa.Gabelung)!=0 && Kontrolle(0,-1)==1) gehen(0,-1); //Süden
		else if(hasEast(pa.Gabelung)!=0 && Kontrolle(1,0)==1) gehen(1,0); //Osten
		else if(hasWest(pa.Gabelung)!=0 && Kontrolle(-1,0)==1) gehen(-1,0); //Westen
		else{
			//Zurückgehen aus einer Sackgasse
			if (Robot_Move(Strecke[s-1].x,Strecke[s-1].y)==0x02)n++;
			pa.x=Strecke[s-1].x;
			pa.y=Strecke[s-1].y;
			Strecke[s].x=Strecke[s-1].x; //Löschen der doppelten Kreuzung
			Strecke[s].y=Strecke[s-1].y;
			s--;
		}
	}

return ;
}

void rueckkehr(void){ //
	int i;
	while(s!=0){//Solange er nicht wieder am start ist(Strecke[s=0] ist natürlich der Startpunkt
		for(i=0;i<s-1;i++){
			if (Strecke[s].x+1==Strecke[i].x && Strecke[s].y==Strecke[i].y && (hasEast(Robot_GetIntersections())==1)) {//Kontrolle ob ein Punkt in Östlicher Richtung
				//schon befahren wurde und ob eine Verbindung zu ihm Existiert
				Robot_Move(Strecke[i].x,Strecke[i].y);
				break;}
			else if (Strecke[s].x-1==Strecke[i].x && Strecke[s].y==Strecke[i].y && (hasWest(Robot_GetIntersections())==1)) {//Analog oben
				Robot_Move(Strecke[i].x,Strecke[i].y);
				break;}
			else if (Strecke[s].y+1==Strecke[i].y && Strecke[s].x==Strecke[i].x && (hasNorth(Robot_GetIntersections())==1)) {
				Robot_Move(Strecke[i].x,Strecke[i].y);
				break;}
			else if (Strecke[s].y-1==Strecke[i].y && Strecke[s].x==Strecke[i].x  && (hasSouth(Robot_GetIntersections())==1)) {
				Robot_Move(Strecke[i].x,Strecke[i].y);
				break;}
			}
		if(i==s-1){ //Wenn Die oben genannten Fälle nicht eingetreten sind, dann läuft er auf seinem letzten Weg zurück
		s--;
		Robot_Move(Strecke[s].x,Strecke[s].y);}
		else s=i;
}
return ;
}

int main(void){
	pa.x=0;
	pa.y=0;
	Strecke[0].x=0;
	Strecke[0].y=0;
	snake[0].x=0;
	snake[0].y=0;
	if (hasNorth(Robot_GetIntersections())==1){ snake[0].y=0.5;}
	else if (hasSouth(Robot_GetIntersections())==1){ snake[0].y=-0.5;}
	else if (hasEast(Robot_GetIntersections())==1){ snake[0].x=0.5;}
	else if (hasWest(Robot_GetIntersections())==1){ snake[0].y=-0.5;}
	suchen();
	rueckkehr();

	return EXIT_SUCCESS;
}



