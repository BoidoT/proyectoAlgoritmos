#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define CANTIDADGOLES 150

using namespace std;



//Para guardar en la matriz
struct nodoLista{
  char nombreJugador[30];
  int fecha;
  int goles;
  nodoLista *siguiente;  
};

typedef nodoLista matriz[32][7];

struct registroGol{
  int idGol;
  int codEquipo;
  int fecha;
  char nombreJugador[30];
  int idPartido;
  registroGol* anterior;
  registroGol* siguiente;
};

registroGol* generarGoles(registroGol* &golPila, int cantidadGoles);
registroGol* insertar(registroGol*& lista, int idGol, int fecha, int codEquipo, char *nombreJugador, int idPartido);
int recorrerLista(registroGol*& listaGol);
bool grabarListaEnArchivo(registroGol*&, char*);
bool leerArchivo (registroGol *&listaGol, char* nombreArchivo);

int main(){
  //Variables
	registroGol* listaGol;
  registroGol* listaGolArchivo;
	listaGol = NULL;
  listaGolArchivo = NULL;
  generarGoles(listaGol, CANTIDADGOLES);
  cout<< "Goles Generados"<<endl;
  //int cant = recorrerLista(listaGol);
  //cout<< "Lista Recorrida"<<endl;
  //cout << cant << " Registros leidos " << endl;
  grabarListaEnArchivo(listaGol, (char *)"lista.bin");
  leerArchivo(listaGolArchivo, (char *)"lista.bin");
  int cant = recorrerLista(listaGolArchivo);
  cout << cant << " Registros leidos " << endl;
  return 0;
}

registroGol* generarGoles(registroGol* &listaGol, int cantidadGoles){
	registroGol* q=NULL;
	char jugadoresStr[] = "TONI KROOS,CRISTIANO RONALDO,LIONEL MESSI,ANTOINE GRIEZMANN,EDEN HAZARD,DAVID SILVA,NEYMAR JR.,HARRY KANE,LUKA MODRIC,EDINSON CAVANI,";
  char jugadores[10][30];
  char temp[30],chr;
  temp[0] = '\0';
  //Aca llenamos un array de 10 jugadores para sortearlos
  for(int x=0,z=0;x<strlen(jugadoresStr);x++){
    chr = jugadoresStr[x];
    if(chr == ','){ //Delimitador
      strcpy(jugadores[z],temp);
      temp[0] = '\0';
      z++;
    }else{
      strncat(temp,&chr,1);
    }
  }
  
  srand (time(NULL));
  cout<<"Cantidad Goles: "<<cantidadGoles<<endl;
  for(int x=0;x<cantidadGoles;x++){
    int codigoEquipo = rand() % 32+1;
    int fechaPartido = rand() % 28 + 20180701;
    int idJugador = rand() % 9;
    int idDePartido = rand() % 64+1;
	  registroGol* p=new registroGol();
	  insertar(listaGol,x+1,fechaPartido,codigoEquipo,&jugadores[idJugador][0],idDePartido);
    
	}
}
//Implementacion:
//pLista siempre va a ser el principio de la lista
//pNodo se usa para iterar
registroGol* insertar(registroGol *&listaGol, int idGol, int fecha, int codEquipo, char *nombreJugador, int idPartido){
  registroGol *pNodo;
  //Si la lista no esta vacia, declaro un nuevo Nodo
  registroGol* aux = new registroGol();
  aux -> idGol = idGol;
  aux -> codEquipo = codEquipo;
  strcpy(aux -> nombreJugador,nombreJugador); 
  aux -> fecha = fecha;  
  aux -> idPartido = idPartido;  
  aux -> siguiente = NULL;
  aux -> anterior = NULL;

  //Si la lista esta vacia
  if(listaGol == NULL){ 
    //¿Por que devuelvo esto?
    listaGol = aux;
    return aux;
  }

//Caso particular: Lista de un solo elemento. 
  if(listaGol -> codEquipo > codEquipo || (listaGol -> codEquipo == codEquipo && listaGol -> fecha > fecha)){
    aux -> siguiente = listaGol;
    listaGol = aux;
    return aux;
  }

  //Declaro pNodo que lo voy a usar para recorrer la lista hasta cierta condicion
  pNodo = listaGol;

  //Caso General, recorro.
  while(pNodo -> siguiente != NULL 
  && (pNodo -> siguiente -> codEquipo <= codEquipo 
  || (pNodo -> siguiente -> codEquipo == codEquipo && pNodo -> siguiente -> fecha <= fecha))){
    pNodo = pNodo -> siguiente;
  }

  //Encontre donde tengo que insertarlo
  aux -> siguiente = pNodo -> siguiente;
  pNodo -> siguiente  = aux;
  return aux;
}

//Graba en el Archivo toda la lista
bool grabarListaEnArchivo (registroGol *&listaGol, char* nombreArchivo){
  registroGol *pNodo;
  pNodo = listaGol;
  FILE* f;
  f = fopen(nombreArchivo, "wb");

  //Caso extremo
  if (f == NULL){
    return false;
  }

  while (pNodo != NULL)
  {
    fwrite(pNodo,sizeof(registroGol),1,f);
    if (ferror(f))
    { 
      return false;
    }
    pNodo = pNodo->siguiente;
  }

  fclose(f);
  return true;
}

//Lee el Archivo toda la lista, en memoria
bool leerArchivo (registroGol *&listaGol, char* nombreArchivo){
  registroGol pNodo;
  //pNodo = listaGol;
  FILE* f;
  f = fopen(nombreArchivo, "rb");

  //Caso extremo
  if (f == NULL){
    return false;
  }
  int registros;
  do{
    registros = fread(&pNodo,sizeof(registroGol),1,f);
    insertar(listaGol, pNodo.idGol, pNodo.fecha, pNodo.codEquipo, pNodo.nombreJugador, pNodo.idPartido);
    if(ferror(f)){ 
      return false;
    }
  }while(registros > 0);

  fclose(f);
  return true;
}

int recorrerLista(registroGol*& listaGol){
  registroGol *pNodo = listaGol; 
  int cant = 0;
  while(pNodo -> siguiente != NULL){
    cout << pNodo -> idGol << " " << pNodo -> codEquipo << " " << pNodo -> fecha << " " << pNodo -> nombreJugador << " " << pNodo -> idPartido << endl;
    pNodo = pNodo -> siguiente;
    cant++;
  }  
  return cant;
}
