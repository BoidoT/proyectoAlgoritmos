#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32 //Solo incluir si se compila en windows
  #include <Windows.h> //Libreria para setear la consola en utf8 en windows
#endif

using namespace std;

//Equipos
struct equipos{
  int id;
  char nombre[50];
};

// ESTRUCTURA PARA RANKING DE GOLEADORES
struct rankJugador{
  char nombreJugador[50];
  char equipo[50];
  int goles;
};
struct lstRankJugador{
  rankJugador jugador;
  lstRankJugador *siguiente;
};
struct rankFecha{
  int fecha;
  lstRankJugador *lstRankJugadores;
};
struct lstRankFechas{
  rankFecha fecha;
  lstRankFechas *siguiente;
};

// Para guardar en la matriz
struct nodoLista{
  char nombreJugador[50];
  int fecha;
  int goles;
  nodoLista *siguiente;
};

typedef nodoLista *matriz[32][7];

struct equipoGol{ //Almacena la cantidad de goles por equipo
  char nombreEquipo[50];
  int goles;
};

struct registroGol{
  int idGol;
  int codEquipo;
  int fecha;
  char nombreJugador[50];
  int idPartido;
  registroGol* siguiente;
};

// Prototipo de funciones
void insertar(registroGol *&lista, int idGol, int fecha, int codEquipo, char *nombreJugador, int idPartido);
bool leerBinario(registroGol *&);
void generarMatrizGoles(registroGol *&, matriz);
void recorrerMatrizGoles (matriz,equipos[]);
void insertarJugadorEnRank(lstRankFechas *&, nodoLista *,char []);
void insertarJugadorEnRankGlobal(lstRankFechas *&, nodoLista *,char []);
void ordenarRankGlobal(lstRankFechas *&);
/* Funciones auxiliares */
void pausa(const char *);
void pausa();
void formatoFecha(int,char []);

int main(){
  #ifdef _WIN32
    system ("color 0F" ); //Cambio el color de la consola
    SetConsoleOutputCP(CP_UTF8); //Cambia el code page a UTF-8 para evitar problemas con carácteres no ingleses (solo windows)
  #endif
  //Variables
  registroGol* listaGolArchivo;
  equipos e[32]; //Array de equipos
  listaGolArchivo = NULL;
  matriz mPartidos;
  leerBinario(listaGolArchivo);
  generarMatrizGoles(listaGolArchivo , mPartidos);
  recorrerMatrizGoles(mPartidos,e);
  return 0;
}


//Inserta un registro en la lista de goles ordenado por equipo -> fecha
void insertar(registroGol *&listaGol, int idGol, int fecha, int codEquipo, char *nombreJugador, int idPartido){
  //Si la lista no esta vacia, declaro un nuevo Nodo
  registroGol* nuevoNodo = new registroGol();
  nuevoNodo -> idGol = idGol;
  nuevoNodo -> codEquipo = codEquipo;
  strcpy(nuevoNodo -> nombreJugador,nombreJugador);
  nuevoNodo -> fecha = fecha;
  nuevoNodo -> idPartido = idPartido;
  nuevoNodo -> siguiente = NULL;

  //Si la lista esta vacia asignarla al nodo nuevo
  if(listaGol == NULL){
    listaGol = nuevoNodo;
    return;
  }

  //Comparo con el primer elemento de la lista
  if(listaGol -> codEquipo > codEquipo || (listaGol -> codEquipo == codEquipo && listaGol -> fecha > fecha)){
    nuevoNodo -> siguiente = listaGol;
    listaGol = nuevoNodo;
    return;
  }

  registroGol *aux = listaGol; //Aux lo voy a usar para recorrer la lista
  //Recorro hasta encontrar donde insertar el nodo (o llegar al final de la lista)
  while(aux -> siguiente != NULL
  && (aux -> siguiente -> codEquipo < codEquipo
  || (aux -> siguiente -> codEquipo == codEquipo && aux -> siguiente -> fecha <= fecha))){
    aux = aux -> siguiente;
  }

  //Inserto el nodo
  nuevoNodo -> siguiente = aux -> siguiente;
  aux -> siguiente = nuevoNodo;
  return;
}


//Lee el Archivo toda la lista, en memoria
bool leerBinario(registroGol *&listaGol){
  registroGol aux;
  FILE* f = fopen("..\\EJERCICIO 1\\datos.bin", "rb");
  if (f == NULL){ //Error al leer el archivo
    return false;
  }
  int registros;
  do{
    registros = fread(&aux,sizeof(registroGol),1,f);
    if(registros == 0){ //Cuando llego al final del archivo salir
      break;
    }
    insertar(listaGol, aux.idGol, aux.fecha, aux.codEquipo, aux.nombreJugador, aux.idPartido);
    if(ferror(f)){ //Error de lectura
      return false;
    }
  }while(true);
  fclose(f);
  return true;
}

void generarMatrizGoles(registroGol*& listaGol, matriz mPartidos){
  pausa("\n>> Presione enter para mostrar el listado de fechas y goles\n");
  int partido,x,y;
  bool crearNodo; //Bandera para indicar si se debe crear un nuevo nodo de gol en el partido actual (si el jugador coincide)
  nodoLista* nuevoNodo = NULL,*auxNL;
  registroGol* auxRG = NULL;
  //Las filas son equipos, las columnas son cada partido que jugo dicho equipo (solo se cuentan partidos con goles)
  for(x=0 ; x<32; x++){
    for(y=0 ; y<7 ; y++){
      mPartidos[x][y] = NULL ; //Cada celda es una lista enlazada de goleadores
    }
  }

  for(partido=0;listaGol != NULL;){ //
    //Si la primera columna (partido) esta vacia (NULL) insertar un nuevo nodo
    if(mPartidos[listaGol -> codEquipo -1][partido] == NULL){
      nuevoNodo = new nodoLista();
      nuevoNodo -> fecha = listaGol -> fecha;
      nuevoNodo -> goles = 1;
      strcpy(nuevoNodo -> nombreJugador, listaGol-> nombreJugador);
      nuevoNodo -> siguiente = NULL;
      mPartidos[listaGol -> codEquipo -1][partido] = nuevoNodo; //Inserto el primer nodo de la lista enlazada de la celda
      listaGol = listaGol -> siguiente; //Avanzo al siguiente registro de la lista de goles
      partido=0; //Reseteo el contador de partidos
      continue; //Repetir el LOOP
    }

    //Si el partido existente en la columna no coincide con el leido avanzar a la siguiente columna,
    //esto se repetira hasta encontrar una columna que coincida con el partido a insertar o una columna libre
    if(mPartidos[listaGol -> codEquipo -1][partido] -> fecha != listaGol -> fecha){
      partido++; //Aumento en 1 el contador de columnas (partido)
      continue; //Repetir el LOOP
    }

    //Si llego aca es que encontro un segundo registro del mismo partido,
    //hay que determinar si el gol es del mismo jugador o hay que agregar un nodo
    auxNL = mPartidos[listaGol -> codEquipo -1][partido]; //Puntero auxiliar para recorrer la lista enlazada
    crearNodo = true; //Si esta bandera queda en true, se añade un nodo
    while(auxNL != NULL){ //Recorro la lista
      if(strcmp(auxNL -> nombreJugador,listaGol -> nombreJugador) == 0){ //Si el jugador es el mismo sumar los goles
        auxNL -> goles++; //Sumo goles
        crearNodo = false; //Marco la bandera en false para evitar que se cree un nodo
        break;
      }
      auxNL = auxNL -> siguiente;
    }
    //Si crear nodo esta en true es que no hay goles de este jugador registrados en este partido
    if(crearNodo){
      nuevoNodo = new nodoLista();
      nuevoNodo -> fecha = listaGol -> fecha;
      nuevoNodo -> goles=1;
      strcpy(nuevoNodo -> nombreJugador, listaGol-> nombreJugador);
      nuevoNodo -> siguiente=NULL;
      //Busco el ultimo nodo para enlazar el nodo creado
      auxNL = mPartidos[listaGol -> codEquipo -1][partido];
      while(auxNL -> siguiente != NULL){
        auxNL = auxNL -> siguiente;
      }
      auxNL -> siguiente = nuevoNodo; //Insertar el nuevo nodo al a continuacion del ultimo nodo
    }
    listaGol = listaGol -> siguiente; //Avanzo al siguiente registro
    partido=0; //Reseteo el contador de columnas (partidos)
  }
}

void insertarJugadorEnRank(lstRankFechas *&cl, nodoLista *nl,char equipo[]){
  //Si es mi primer elemento de mi lista, completo la informacion
  if(cl==NULL || cl -> fecha.fecha > nl -> fecha){
    //Creo nodo
    lstRankFechas *nuevo = new lstRankFechas;
    nuevo -> fecha.fecha = nl -> fecha;
    nuevo -> fecha.lstRankJugadores = new lstRankJugador;
    strcpy(nuevo->fecha.lstRankJugadores -> jugador.nombreJugador, nl -> nombreJugador);
    nuevo -> fecha.lstRankJugadores -> jugador.goles = nl -> goles;
    strcpy(nuevo -> fecha.lstRankJugadores -> jugador.equipo,equipo);
    nuevo -> fecha.lstRankJugadores -> siguiente = NULL;
    nuevo -> siguiente = cl;
    cl = nuevo;
    return;
  }

  if(cl -> fecha.fecha == nl -> fecha){
    //nUEVO NODO
    lstRankJugador *nuevoJ = new lstRankJugador;
    strcpy(nuevoJ -> jugador.nombreJugador,nl -> nombreJugador);
    nuevoJ -> jugador.goles = nl -> goles;
    strcpy(nuevoJ -> jugador.equipo,equipo);
    nuevoJ -> siguiente = NULL;

    lstRankJugador *lrjAux = cl -> fecha.lstRankJugadores;
    if(cl->fecha.lstRankJugadores->jugador.goles<nl->goles){
        nuevoJ -> siguiente = cl->fecha.lstRankJugadores;
        cl->fecha.lstRankJugadores = nuevoJ;
        return;
    }
    while(lrjAux -> siguiente != NULL){
      if(lrjAux ->siguiente->jugador.goles<nl->goles){
        nuevoJ -> siguiente = lrjAux ->siguiente;
        lrjAux->siguiente = nuevoJ;
        return;
      }
      lrjAux = lrjAux -> siguiente;    
    }
    lrjAux -> siguiente = nuevoJ; //Asigno nuevo nodo Jugador
    return;
  }
  
  lstRankFechas *lrfAux = cl;
  while(lrfAux -> siguiente != NULL){ // Si ya existe la fecha en mi lista
    if(lrfAux -> siguiente -> fecha.fecha > nl -> fecha){
      //Creo nodo
      lstRankFechas *nuevo = new lstRankFechas;
      nuevo -> fecha.fecha = nl -> fecha;
      nuevo -> fecha.lstRankJugadores = new lstRankJugador;
      strcpy(nuevo->fecha.lstRankJugadores -> jugador.nombreJugador, nl -> nombreJugador);
      nuevo -> fecha.lstRankJugadores -> jugador.goles = nl -> goles;
      strcpy(nuevo->fecha.lstRankJugadores -> jugador.equipo,equipo);
      nuevo -> fecha.lstRankJugadores -> siguiente = NULL;
      nuevo -> siguiente = lrfAux -> siguiente;
      lrfAux -> siguiente = nuevo;
      return;
    }
    if(lrfAux -> siguiente -> fecha.fecha == nl -> fecha){
      //Nuevo nodo
      lstRankJugador *nuevoJ = new lstRankJugador;
      strcpy(nuevoJ -> jugador.nombreJugador,nl -> nombreJugador);
      nuevoJ -> jugador.goles = nl -> goles;
      strcpy(nuevoJ -> jugador.equipo,equipo);
      nuevoJ -> siguiente = NULL;
      
      //Comparo los goles con el primer jugador
      //lrfAux -> siguiente -> fecha.lstRankJugadores -> jugador.goles << endl;
      lstRankJugador *lrjAux = lrfAux -> siguiente -> fecha.lstRankJugadores;
      if(lrfAux->siguiente->fecha.lstRankJugadores->jugador.goles<=nl->goles){
        nuevoJ -> siguiente = lrfAux->siguiente->fecha.lstRankJugadores;
        lrfAux->siguiente->fecha.lstRankJugadores = nuevoJ;
        return;
      }
      while(lrjAux -> siguiente != NULL){
        if(lrjAux ->siguiente->jugador.goles<=nl->goles){
          nuevoJ -> siguiente = lrjAux ->siguiente;
          lrjAux->siguiente = nuevoJ;
          return;
        }
        lrjAux = lrjAux -> siguiente;    
      }
      lrjAux -> siguiente = nuevoJ; //Asigno nuevo jugador
      return;
    }
    lrfAux = lrfAux -> siguiente;
  }

  //Creo nodo
  lstRankFechas *nuevo = new lstRankFechas;
  nuevo -> fecha.fecha = nl -> fecha;
  nuevo -> fecha.lstRankJugadores = new lstRankJugador;
  strcpy(nuevo->fecha.lstRankJugadores -> jugador.nombreJugador, nl -> nombreJugador);
  nuevo -> fecha.lstRankJugadores -> jugador.goles = nl -> goles;
  strcpy(nuevo -> fecha.lstRankJugadores -> jugador.equipo,equipo);  
  nuevo -> fecha.lstRankJugadores -> siguiente = NULL;
  nuevo -> siguiente = lrfAux -> siguiente;
  lrfAux -> siguiente = nuevo;
  return;
}

void insertarJugadorEnRankGlobal(lstRankFechas *&cl, nodoLista *nl,char equipo[]){
  if(cl == NULL){
    lstRankFechas *nuevo = new lstRankFechas;
    nuevo -> fecha.lstRankJugadores = new lstRankJugador;
    strcpy(nuevo -> fecha.lstRankJugadores -> jugador.nombreJugador,nl -> nombreJugador);
    nuevo -> fecha.lstRankJugadores -> jugador.goles = nl -> goles;
    strcat(nuevo -> fecha.lstRankJugadores -> jugador.equipo,equipo);    
    nuevo -> siguiente = NULL;      
    cl = nuevo;
    return;
  }

  lstRankFechas *aux = cl;
  while(aux -> siguiente != NULL){
    if(strcmp(aux -> siguiente -> fecha.lstRankJugadores -> jugador.nombreJugador,nl -> nombreJugador) == 0){
      aux -> fecha.lstRankJugadores -> jugador.goles += nl -> goles;
      return;
    }
    aux = aux -> siguiente;
  }
  lstRankFechas *nuevo = new lstRankFechas;
  nuevo -> fecha.lstRankJugadores = new lstRankJugador;
  strcpy(nuevo -> fecha.lstRankJugadores -> jugador.nombreJugador,nl -> nombreJugador);
  nuevo -> fecha.lstRankJugadores -> jugador.goles = nl -> goles;
  strcat(nuevo -> fecha.lstRankJugadores -> jugador.equipo,equipo);    
  nuevo -> siguiente = NULL;    
  aux -> siguiente = nuevo;
}

void ordenarRankGlobal(lstRankFechas *&cl){
  bool cambio ;
  //Si la lista esta vacia o tiene un solo elemento salir
  if(cl == NULL || cl -> siguiente == NULL){ 
    return;
  }
  lstRankFechas *aux = cl, *auxSig;
  rankFecha temp;
  while(aux -> siguiente != NULL){
    auxSig = aux -> siguiente;
    while(auxSig != NULL){
      if(aux->fecha.lstRankJugadores -> jugador.goles < auxSig->fecha.lstRankJugadores -> jugador.goles){
        temp = auxSig -> fecha;
        auxSig->fecha = aux->fecha;
        aux->fecha = temp;          
      }
      auxSig = auxSig -> siguiente;                    
    }    
    aux = aux -> siguiente;
    auxSig = aux -> siguiente;
  }
}

void recorrerMatrizGoles (matriz mPartidos,equipos e[]){
  cout << "-------- RESULTADO RANKING PAISES --------" << endl;
  nodoLista* auxNL = NULL;
  lstRankFechas* cabezaRanking = NULL; // Lista de Ranking de Goleadores de una Fecha
  lstRankFechas* cabezaRankingGlobal = NULL;
  int cantGoles,x,y;
  equipoGol eg[32];
  char fechaDmy[11];
  for(x=0; x<32; x++){ // Cada iteracion corresponde a un equipo
    cout << "Equipo " << e[x].nombre << endl;
    cantGoles=0; // Inicializo el contador de goles totales para un Equipo
    for(y=0; y<7; y++){ // Recorro los 7 posibles partidos del equipo en el mundial
      auxNL = mPartidos[x][y]; //Asigno el puntero nodo auxiliar a una posicion de mi matriz estatica
      if(auxNL == NULL){ // Si no encuentro un partido registrado en esta posicion de la matriz, avanzo al siguiente.
        continue;
      }
      formatoFecha(mPartidos[x][y] -> fecha,fechaDmy);
      cout << "\tPartido " << y+1 << " (" << fechaDmy << ")" << endl; // Encontre un partido con goles para un equipo
      while(auxNL != NULL){ // Recorro mi lista de goles para ese partido
        cantGoles += auxNL->goles;
        cout << "\t\t" << auxNL -> nombreJugador << " - Goles: " << auxNL-> goles << endl; //Muestro los goles de un jugador para una fecha determinada
        insertarJugadorEnRank(cabezaRanking, auxNL,e[x].nombre);
        insertarJugadorEnRankGlobal(cabezaRankingGlobal, auxNL,e[x].nombre);
        auxNL = auxNL -> siguiente;
      }
    }
    cout << "\tTotal goles : " << cantGoles << "\n\n" << endl; // Una vez que recorri los 7 partidos de un equipo imprimo el total de goles del equipo
    cout << "--------------------------------------------------------" <<endl; // Divisor de Equipo
    strncpy(eg[x].nombreEquipo,e[x].nombre,50); // Asigno el nombre del equipo y su cantidad de goles a un vector auxiliar.
    eg[x].goles = cantGoles;
  }
  pausa("\n>> Presione enter para mostrar el ranking de jugadores por fecha\n");
  cout << "-------- RESULTADO RANKING JUGADORES POR FECHA --------" << endl;
  while(cabezaRanking != NULL){
    formatoFecha(cabezaRanking -> fecha.fecha,fechaDmy);
    cout << "Fecha: " << fechaDmy << endl;    
    while(cabezaRanking -> fecha.lstRankJugadores != NULL){
      cout << "\t" << cabezaRanking -> fecha.lstRankJugadores -> jugador.nombreJugador;
      cout << " - Goles: " << cabezaRanking -> fecha.lstRankJugadores -> jugador.goles;
      cout << " - Equipo: " << cabezaRanking -> fecha.lstRankJugadores -> jugador.equipo << endl;
      cabezaRanking -> fecha.lstRankJugadores = cabezaRanking -> fecha.lstRankJugadores -> siguiente; 
    }
    cabezaRanking = cabezaRanking -> siguiente;
  }
  pausa("\n>> Presione enter para mostrar el ranking general de jugadores\n");
  cout << "-------- RESULTADO RANKING JUGADORES GENERAL --------" << endl;
  ordenarRankGlobal(cabezaRankingGlobal);
  while(cabezaRankingGlobal != NULL){
    cout << "\t" << cabezaRankingGlobal -> fecha.lstRankJugadores -> jugador.nombreJugador;
    cout << " - Goles: " << cabezaRankingGlobal -> fecha.lstRankJugadores -> jugador.goles;
    cout << " - Equipo: " << cabezaRankingGlobal -> fecha.lstRankJugadores -> jugador.equipo << endl;
    cabezaRankingGlobal -> fecha.lstRankJugadores = cabezaRankingGlobal -> fecha.lstRankJugadores -> siguiente; 
    cabezaRankingGlobal = cabezaRankingGlobal -> siguiente;
  }  
 return;
}



//AUXILIARES
//Muestra el mensaje en pantalla presione enter para continuar y espera la pulsacion de enter
void pausa(){
  pausa("Presione enter para continuar..."); //Mensaje por default
}
void pausa(const char *msg){
  cout << msg << endl;
  getchar();
}

void formatoFecha(int ymd,char fecha[]){
  char buff[10],*ptr,delim = '/';
  itoa(ymd,&buff[0],10);
  ptr = &buff[0];
  fecha[0] = '\0';
  strncat(fecha,ptr+6,2);
  strncat(fecha,&delim,1);
  strncat(fecha,ptr+4,2);
  strncat(fecha,&delim,1);  
  strncat(fecha,ptr,4);
}