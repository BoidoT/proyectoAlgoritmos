#include <iostream>
#include <stdlib.h>
#include <time.h>
#define CANTIDADGOLES 10

using namespace std;

struct registroGol{
  int idGol;
  int codEquipo;
  int fecha;
  string nombreJugador;
  int idPartido;
  registroGol* siguiente;
};

void push(registroGol*& pila, int idGol, int fecha, int codEquipo, string nombreJugador, int idPartido);
int recorrerPila(registroGol* &pila);
registroGol* generarGoles(registroGol* &golPila, int cantidadGoles);


int main(){
	
	registroGol* frente;
	frente = NULL;
  
   generarGoles(frente, CANTIDADGOLES);
  recorrerPila(frente); 
  return 0;
	
	
	
}

registroGol* generarGoles(registroGol* &golPila, int cantidadGoles){
	registroGol* q=NULL;
	string jugadores[10] = {"TONI KROOS","CRISTIANO RONALDO","LIONEL MESSI","ANTOINE GRIEZMANN","EDEN HAZARD","DAVID SILVA","NEYMAR JR.","HARRY KANE","LUKA MODRIC","EDINSON CAVANI"};
  srand (time(NULL));
  for(int x=0;x<cantidadGoles;x++){
    int codigoEquipo = rand() % 32+1;
    int fechaPartido = rand() % 28 + 20180701;
    int idJugador = rand() % 9;
    int idDePartido = rand() % 64+1;

	registroGol* p=new registroGol();
	push(golPila,x+1,fechaPartido,codigoEquipo,jugadores[idJugador],idDePartido);
	}
}



  
int recorrerPila(registroGol* &pila){

	registroGol* p=pila;
	registroGol* q=NULL;
	int cant=0;
	
	while(p!=NULL){
		cout << "ID GOL: " << p -> idGol << " Cod. Equipo: " << p -> codEquipo << " Fecha: " << p -> fecha << " Jugador: " << p -> nombreJugador << " Id Partido: " << p -> idPartido << endl;
		q=p;
		p=p->siguiente;
		cant++;
	}
  return cant ;	
}



void push(registroGol*& pila, int idGol, int fecha, int codEquipo, string nombreJugador, int idPartido){
    registroGol* q = new registroGol();
    q -> idGol = idGol;
    q -> codEquipo = codEquipo;
    q -> fecha = fecha;
    q -> nombreJugador = nombreJugador;
    q -> idPartido = idPartido;
    q -> siguiente = pila;
    pila = q;
    return;
}
