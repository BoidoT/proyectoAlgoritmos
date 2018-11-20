#include <iostream>
#include <string.h>
#ifdef _WIN32 //Solo incluir si se compila en windows
  #include <Windows.h> //Libreria para setear la consola en utf8 en windows
  #define DS "\\" //Separador de directorios
#else
  #define DS "/" //Separador de directorios
#endif

using namespace std;

//Equipos
struct equipos{
  int id;
  char nombre[50];
};

//Estructuras para el ranking de jugadores por fecha y global
struct rankJugador{
  char nombreJugador[50];
  char equipo[50];
  int goles;
};
struct nodoRankJugador{
  rankJugador datos;
  nodoRankJugador *siguiente;
};
struct rankFecha{
  int fecha;
  nodoRankJugador *nodoRankJugadores;
};
struct nodoRankFechas{
  rankFecha datos;
  nodoRankFechas *siguiente;
};

//Matrix de goleadores por fecha
struct jugadorFecha{
  char nombreJugador[50];
  int fecha;
  int goles;
};

struct nodoJugadorFecha{
  jugadorFecha datos;
  nodoJugadorFecha *siguiente;
};

typedef nodoJugadorFecha *matriz[32][7];

struct registroGol{
  int idGol;
  int codEquipo;
  int fecha;
  char nombreJugador[50];
  int idPartido;
};

struct nodoRegistroGol{
  registroGol datos;
  nodoRegistroGol* siguiente;
};

struct equipoGol{ //Almacena la cantidad de goles por equipo
  char nombreEquipo[50];
  int goles;
};

void insertar(nodoRegistroGol *&lista, registroGol d);
bool leerBinario(nodoRegistroGol *&, equipos []);
void generarMatrizGoles(nodoRegistroGol *&, matriz);
void recorrerMatrizGoles (matriz,equipos[]);
void insertarJugadorEnRank(nodoRankFechas *&, nodoJugadorFecha *,char []);
void insertarJugadorEnRankGlobal(nodoRankFechas *&, nodoJugadorFecha *,char []);
void ordenarRankGlobal(nodoRankFechas *&);
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
	nodoRegistroGol* listaGol = NULL; //Lista dinamica que carga el Archivo
  matriz mPartidos;
  equipos vEquipos[32];
  if(!leerBinario(listaGol,vEquipos)){
    pausa(">> Presione enter para salir del programa");
    return 1;
  }
  generarMatrizGoles(listaGol , mPartidos);
  recorrerMatrizGoles(mPartidos,vEquipos);
  pausa(">> Presione enter para salir del programa");
  return 0;
}

//Lee el Archivo toda la lista, en memoria
bool leerBinario(nodoRegistroGol *&listaGol,equipos e[]){
  registroGol auxRG;
  equipos auxE;
  FILE* f;
  int registros;
  f = fopen(".." DS "EJERCICIO 1" DS "goles.bin", "rb");
  if (f == NULL){ //Error al leer el archivo
    cout << "ERROR: No se pudo leer el archivo [goles.bin]" << endl;
    return false;
  }
  do{
    registros = fread(&auxRG,sizeof(registroGol),1,f);
    if(registros == 0){ //Cuando llego al final del archivo salir
      break;
    }
    insertar(listaGol, auxRG);
    if(ferror(f)){ //Error de lectura
      cout << "ERROR: No se pudo leer el archivo [goles.bin]" << endl;
      return false;
    }
  }while(true);
  fclose(f);
  f = fopen(".." DS "EJERCICIO 1" DS "equipos.bin", "rb");
  if (f == NULL){ //Error al leer el archivo
    cout << "ERROR: No se pudo leer el archivo [equipos.bin]" << endl;
    return false;
  }
  for(int x=0;x<32;x++){
    registros = fread(&auxE,sizeof(equipos),1,f);
    if(registros == 0){ //Cuando llego al final del archivo salir
      break;
    }
    e[x].id = auxE.id;
    strncpy(e[x].nombre,auxE.nombre,30);
    if(ferror(f)){ //Error de lectura
      cout << "ERROR: No se pudo leer el archivo [equipos.bin]" << endl;
      return false;
    }
  }
  fclose(f);
  return true;
}

//Inserta un registro en la lista de goles al final de la misma
void insertar(nodoRegistroGol *&listaGol, registroGol d){
  //Si la lista no esta vacia, declaro un nuevo Nodo
  nodoRegistroGol* nuevoNodo = new nodoRegistroGol;
  nuevoNodo -> datos = d;
  nuevoNodo -> siguiente = NULL;

  //Si la lista esta vacia asignarla al nodo nuevo
  if(listaGol == NULL){
    listaGol = nuevoNodo;
    return;
  }

  nodoRegistroGol *aux = listaGol; //Aux lo voy a usar para recorrer la lista
  //Recorro hasta encontrar donde insertar el nodo (o llegar al final de la lista)
  while(aux -> siguiente != NULL){
    aux = aux -> siguiente;
  }
  //Inserto el nodo
  aux -> siguiente = nuevoNodo;
  return;
}

void generarMatrizGoles(nodoRegistroGol*& listaGol, matriz mPartidos){
  int partido,x,y;
  bool crearNodo; //Bandera para indicar si se debe crear un nuevo nodo de gol en el partido actual (si el jugador coincide)
  nodoJugadorFecha* nuevoNodo = NULL,*auxNL;
  registroGol* auxRG = NULL;
  //Las filas son equipos, las columnas son cada partido que jugo dicho equipo (solo se cuentan partidos con goles)
  for(x=0 ; x<32; x++){
    for(y=0 ; y<7 ; y++){
      mPartidos[x][y] = NULL ; //Cada celda es una lista enlazada de goleadores
    }
  }

  for(partido=0;listaGol != NULL;){ //
    //Si la primera columna (partido) esta vacia (NULL) insertar un nuevo nodo
    if(mPartidos[listaGol -> datos.codEquipo -1][partido] == NULL){
      nuevoNodo = new nodoJugadorFecha;
      nuevoNodo -> datos.fecha = listaGol -> datos.fecha;
      nuevoNodo -> datos.goles = 1;
      strcpy(nuevoNodo -> datos.nombreJugador, listaGol-> datos.nombreJugador);
      nuevoNodo -> siguiente = NULL;
      mPartidos[listaGol -> datos.codEquipo -1][partido] = nuevoNodo; //Inserto el primer nodo de la lista enlazada de la celda
      listaGol = listaGol -> siguiente; //Avanzo al siguiente registro de la lista de goles
      partido=0; //Reseteo el contador de partidos
      continue; //Repetir el LOOP
    }

    //Si el partido existente en la columna no coincide con el leido avanzar a la siguiente columna,
    //esto se repetira hasta encontrar una columna que coincida con el partido a insertar o una columna libre
    if(mPartidos[listaGol -> datos.codEquipo -1][partido] -> datos.fecha != listaGol -> datos.fecha){
      partido++; //Aumento en 1 el contador de columnas (partido)
      continue; //Repetir el LOOP
    }

    //Si llego aca es que encontro un segundo registro del mismo partido,
    //hay que determinar si el gol es del mismo jugador o hay que agregar un nodo
    auxNL = mPartidos[listaGol -> datos.codEquipo -1][partido]; //Puntero auxiliar para recorrer la lista enlazada
    crearNodo = true; //Si esta bandera queda en true, se añade un nodo
    while(auxNL != NULL){ //Recorro la lista
      if(strcmp(auxNL -> datos.nombreJugador,listaGol -> datos.nombreJugador) == 0){ //Si el jugador es el mismo sumar los goles
        auxNL -> datos.goles++; //Sumo goles
        crearNodo = false; //Marco la bandera en false para evitar que se cree un nodo
        break;
      }
      auxNL = auxNL -> siguiente;
    }
    //Si crear nodo esta en true es que no hay goles de este jugador registrados en este partido
    if(crearNodo){
      nuevoNodo = new nodoJugadorFecha;
      nuevoNodo -> datos.fecha = listaGol -> datos.fecha;
      nuevoNodo -> datos.goles=1;
      strcpy(nuevoNodo -> datos.nombreJugador, listaGol-> datos.nombreJugador);
      nuevoNodo -> siguiente=NULL;
      //Busco el ultimo nodo para enlazar el nodo creado
      auxNL = mPartidos[listaGol -> datos.codEquipo -1][partido];
      while(auxNL -> siguiente != NULL){
        auxNL = auxNL -> siguiente;
      }
      auxNL -> siguiente = nuevoNodo; //Insertar el nuevo nodo al a continuacion del ultimo nodo
    }
    listaGol = listaGol -> siguiente; //Avanzo al siguiente registro
    partido=0; //Reseteo el contador de columnas (partidos)
  }
}

void insertarJugadorEnRank(nodoRankFechas *&cl, nodoJugadorFecha *nl,char equipo[]){
  //Si es mi primer elemento de mi lista, completo la informacion
  if(cl==NULL || cl -> datos.fecha > nl -> datos.fecha){
    //Creo nodo
    nodoRankFechas *nuevo = new nodoRankFechas;
    nuevo -> datos.fecha = nl -> datos.fecha;
    nuevo -> datos.nodoRankJugadores = new nodoRankJugador;
    strcpy(nuevo->datos.nodoRankJugadores -> datos.nombreJugador, nl -> datos.nombreJugador);
    nuevo -> datos.nodoRankJugadores -> datos.goles = nl -> datos.goles;
    strcpy(nuevo -> datos.nodoRankJugadores -> datos.equipo,equipo);
    nuevo -> datos.nodoRankJugadores -> siguiente = NULL;
    nuevo -> siguiente = cl;
    cl = nuevo;
    return;
  }

  if(cl -> datos.fecha == nl -> datos.fecha){
    //nUEVO NODO
    nodoRankJugador *nuevoJ = new nodoRankJugador;
    strcpy(nuevoJ -> datos.nombreJugador,nl -> datos.nombreJugador);
    nuevoJ -> datos.goles = nl -> datos.goles;
    strcpy(nuevoJ -> datos.equipo,equipo);
    nuevoJ -> siguiente = NULL;

    nodoRankJugador *lrjAux = cl -> datos.nodoRankJugadores;
    if(cl->datos.nodoRankJugadores->datos.goles<nl->datos.goles){
        nuevoJ -> siguiente = cl->datos.nodoRankJugadores;
        cl->datos.nodoRankJugadores = nuevoJ;
        return;
    }
    while(lrjAux -> siguiente != NULL){
      if(lrjAux ->siguiente->datos.goles<nl->datos.goles){
        nuevoJ -> siguiente = lrjAux ->siguiente;
        lrjAux->siguiente = nuevoJ;
        return;
      }
      lrjAux = lrjAux -> siguiente;
    }
    lrjAux -> siguiente = nuevoJ; //Asigno nuevo nodo Jugador
    return;
  }

  nodoRankFechas *lrfAux = cl;
  while(lrfAux -> siguiente != NULL){ // Si ya existe la fecha en mi lista
    if(lrfAux -> siguiente -> datos.fecha > nl -> datos.fecha){
      //Creo nodo
      nodoRankFechas *nuevo = new nodoRankFechas;
      nuevo -> datos.fecha = nl -> datos.fecha;
      nuevo -> datos.nodoRankJugadores = new nodoRankJugador;
      strcpy(nuevo->datos.nodoRankJugadores -> datos.nombreJugador, nl -> datos.nombreJugador);
      nuevo -> datos.nodoRankJugadores -> datos.goles = nl -> datos.goles;
      strcpy(nuevo->datos.nodoRankJugadores -> datos.equipo,equipo);
      nuevo -> datos.nodoRankJugadores -> siguiente = NULL;
      nuevo -> siguiente = lrfAux -> siguiente;
      lrfAux -> siguiente = nuevo;
      return;
    }
    if(lrfAux -> siguiente -> datos.fecha == nl -> datos.fecha){
      //Nuevo nodo
      nodoRankJugador *nuevoJ = new nodoRankJugador;
      strcpy(nuevoJ -> datos.nombreJugador,nl -> datos.nombreJugador);
      nuevoJ -> datos.goles = nl -> datos.goles;
      strcpy(nuevoJ -> datos.equipo,equipo);
      nuevoJ -> siguiente = NULL;

      //Comparo los goles con el primer jugador
      //lrfAux -> siguiente -> datos.nodoRankJugadores -> datos.goles << endl;
      nodoRankJugador *lrjAux = lrfAux -> siguiente -> datos.nodoRankJugadores;
      if(lrfAux->siguiente->datos.nodoRankJugadores->datos.goles<=nl->datos.goles){
        nuevoJ -> siguiente = lrfAux->siguiente->datos.nodoRankJugadores;
        lrfAux->siguiente->datos.nodoRankJugadores = nuevoJ;
        return;
      }
      while(lrjAux -> siguiente != NULL){
        if(lrjAux ->siguiente->datos.goles<=nl->datos.goles){
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
  nodoRankFechas *nuevo = new nodoRankFechas;
  nuevo -> datos.fecha = nl -> datos.fecha;
  nuevo -> datos.nodoRankJugadores = new nodoRankJugador;
  strcpy(nuevo->datos.nodoRankJugadores -> datos.nombreJugador, nl -> datos.nombreJugador);
  nuevo -> datos.nodoRankJugadores -> datos.goles = nl -> datos.goles;
  strcpy(nuevo -> datos.nodoRankJugadores -> datos.equipo,equipo);
  nuevo -> datos.nodoRankJugadores -> siguiente = NULL;
  nuevo -> siguiente = lrfAux -> siguiente;
  lrfAux -> siguiente = nuevo;
  return;
}

void insertarJugadorEnRankGlobal(nodoRankFechas *&cl, nodoJugadorFecha *nl,char equipo[]){
  if(cl == NULL){
    nodoRankFechas *nuevo = new nodoRankFechas;
    nuevo -> datos.nodoRankJugadores = new nodoRankJugador;
    strcpy(nuevo -> datos.nodoRankJugadores -> datos.nombreJugador,nl -> datos.nombreJugador);
    nuevo -> datos.nodoRankJugadores -> datos.goles = nl -> datos.goles;
    strcat(nuevo -> datos.nodoRankJugadores -> datos.equipo,equipo);
    nuevo -> siguiente = NULL;
    cl = nuevo;
    return;
  }

  nodoRankFechas *aux = cl;
  while(aux -> siguiente != NULL){
    if(strcmp(aux -> siguiente -> datos.nodoRankJugadores -> datos.nombreJugador,nl -> datos.nombreJugador) == 0){
      aux -> datos.nodoRankJugadores -> datos.goles += nl -> datos.goles;
      return;
    }
    aux = aux -> siguiente;
  }
  nodoRankFechas *nuevo = new nodoRankFechas;
  nuevo -> datos.nodoRankJugadores = new nodoRankJugador;
  strcpy(nuevo -> datos.nodoRankJugadores -> datos.nombreJugador,nl -> datos.nombreJugador);
  nuevo -> datos.nodoRankJugadores -> datos.goles = nl -> datos.goles;
  strcat(nuevo -> datos.nodoRankJugadores -> datos.equipo,equipo);
  nuevo -> siguiente = NULL;
  aux -> siguiente = nuevo;
}

void ordenarRankGlobal(nodoRankFechas *&cl){
  //Si la lista esta vacia o tiene un solo elemento salir
  if(cl == NULL || cl -> siguiente == NULL){
    return;
  }
  nodoRankFechas *aux = cl, *auxSig;
  rankFecha temp; //Temporal para intercambiar punteros
  //Se ordena la lista de jugadores por goles
  while(aux -> siguiente != NULL){ //Recorro la lista hasta el penultimo nodo
    auxSig = aux -> siguiente; //Auxiliar para recorrer el nodo n+1
    while(auxSig != NULL){ //Recorro el nodo n+1 hasta llegar al final de la lista
      if(aux->datos.nodoRankJugadores -> datos.goles < auxSig->datos.nodoRankJugadores -> datos.goles){ //Comparo n con n+1, si se cumple la condicion los intercambio
        temp = auxSig -> datos;
        auxSig->datos = aux->datos;
        aux->datos = temp;
      }
      auxSig = auxSig -> siguiente;
    }
    aux = aux -> siguiente;
    auxSig = aux -> siguiente;
  }
}

void recorrerMatrizGoles (matriz mPartidos,equipos e[]){
  cout << "-------- RESULTADO RANKING PAISES --------" << endl;
  nodoJugadorFecha* auxNL = NULL;
  nodoRankFechas* cabezaRanking = NULL; // Lista de Ranking de Goleadores de una Fecha
  nodoRankFechas* cabezaRankingGlobal = NULL;
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
      formatoFecha(mPartidos[x][y] -> datos.fecha,fechaDmy);
      cout << "\tPartido " << y+1 << " (" << fechaDmy << ")" << endl; // Encontre un partido con goles para un equipo
      while(auxNL != NULL){ // Recorro mi lista de goles para ese partido
        cantGoles += auxNL->datos.goles;
        cout << "\t\t" << auxNL -> datos.nombreJugador << " - Goles: " << auxNL-> datos.goles << endl; //Muestro los goles de un jugador para una fecha determinada
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
    formatoFecha(cabezaRanking -> datos.fecha,fechaDmy);
    cout << "Fecha: " << fechaDmy << endl;
    while(cabezaRanking -> datos.nodoRankJugadores != NULL){
      cout << "\t" << cabezaRanking -> datos.nodoRankJugadores -> datos.nombreJugador;
      cout << " - Goles: " << cabezaRanking -> datos.nodoRankJugadores -> datos.goles;
      cout << " - Equipo: " << cabezaRanking -> datos.nodoRankJugadores -> datos.equipo << endl;
      cabezaRanking -> datos.nodoRankJugadores = cabezaRanking -> datos.nodoRankJugadores -> siguiente;
    }
    cabezaRanking = cabezaRanking -> siguiente;
  }
  pausa("\n>> Presione enter para mostrar el ranking general de jugadores\n");
  cout << "-------- RESULTADO RANKING JUGADORES GENERAL --------" << endl;
  ordenarRankGlobal(cabezaRankingGlobal);
  while(cabezaRankingGlobal != NULL){
    cout << "\t" << cabezaRankingGlobal -> datos.nodoRankJugadores -> datos.nombreJugador;
    cout << " - Goles: " << cabezaRankingGlobal -> datos.nodoRankJugadores -> datos.goles;
    cout << " - Equipo: " << cabezaRankingGlobal -> datos.nodoRankJugadores -> datos.equipo << endl;
    cabezaRankingGlobal -> datos.nodoRankJugadores = cabezaRankingGlobal -> datos.nodoRankJugadores -> siguiente;
    cabezaRankingGlobal = cabezaRankingGlobal -> siguiente;
  }
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