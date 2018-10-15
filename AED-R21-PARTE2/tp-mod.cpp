#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define CANTIDADGOLES 150
#ifdef _WIN32 //Solo incluir si se compila en windows
  #include <Windows.h> //Libreria para setear la consola en utf8 en windows
#endif

using namespace std;

//Equipos
struct equipos{
  int id;
  char nombre[50];
};

//Para guardar en la matriz
struct nodoLista{
  char nombreJugador[50];
  int fecha;
  int goles;
  nodoLista *siguiente;  
};

typedef nodoLista *matriz[32][7];

struct registroGol{
  int idGol;
  int codEquipo;
  int fecha;
  char nombreJugador[50];
  int idPartido;
  registroGol* siguiente;
};


registroGol* insertar(registroGol *&lista, int idGol, int fecha, int codEquipo, char *nombreJugador, int idPartido);
bool leerDatosTexto(equipos [],registroGol *&);
int recorrerLista(registroGol *&listaGol);
bool grabarBinario(registroGol *&);
bool leerBinario(registroGol *&);
void generarMatrizGoles(registroGol *&, matriz);
void recorrerMatrizGoles (matriz,equipos[]);

int main(){
  #ifdef _WIN32
    system ("color 0F" ); //Cambio el color de la consola
    SetConsoleOutputCP(CP_UTF8); //Cambia el code page a UTF-8 para evitar problemas con carácteres no ingleses (solo windows)
  #endif
  //Variables
	registroGol* listaGol; //Lista dinamica que carga el Archivo
  registroGol* listaGolArchivo;
  equipos e[32]; //Array de equipos
	listaGol = NULL;
  listaGolArchivo = NULL;
  matriz mPartidos;
  leerDatosTexto(e,listaGol);
  grabarBinario(listaGol);
  leerBinario(listaGolArchivo);
  generarMatrizGoles(listaGolArchivo , mPartidos);
  recorrerMatrizGoles(mPartidos,e);
  return 0;
}

//Funcion auxiliar que lee los archivos separados por comas: equipos.csv y goles.csv para armar las listas
bool leerDatosTexto(equipos e[],registroGol *&golPila){
  FILE *f;
  char linea[60]; //Aca se lee cada linea
  char temp[60]; //Usado para convertir char[] -> int
  char *coma[3],*eof; //Puntero para separar los campos de cada linea
  int x;
  //Campos goles
  int idGol;
  int fecha;
  int idEquipo;
  int idPartido;
  char nombreJugador[50];

  //Leo archivo de texto de los equipos
  f = fopen("equipos.csv", "r");
  if(f == NULL) {
    cout << "ERROR: No se pudo leer el archivo [equipos.csv]" << endl;
    return false;
  }
  //Leo el archivo de texto
  for(x = 0;x < 32 && fgets (linea, 60, f) != NULL;x++){
    if(linea[0] == '#'){ //Ignoro encabezado
      x--;
      continue;
    }
    if (ferror(f)){ //Error al leer los datos
      return false;
    }       
    coma[0] = strchr(linea,','); //Puntero a la posicion donde esta la coma para cortar el id de equipo
    eof = strchr(linea,'\r'); //Puntero a la posicion donde esta el salto de linea (CRLF)
    if(eof == NULL){
      eof = strchr(linea,'\n'); //Puntero a la posicion donde esta el salto de linea (LF)
    }
    //Extraigo ID de equipo
    temp[0] = '\0';
    strncat(temp,linea,coma[0] - &linea[0]);
    e[x].id = atoi(temp);
    //Extraigo Nombre de equipo
    temp[0] = '\0';
    strncat(temp,coma[0]+1,eof - coma[0]-1); 
    strncpy(e[x].nombre,temp,50); 
  }
  fclose(f);

  //Leo archivo de texto de los goles
  f = fopen("goles.csv", "r");
  if(f == NULL) {
    cout << "ERROR: No se pudo leer el archivo [goles.csv]" << endl;
    return false;
  }
  //Leo el archivo de texto
  for(x = 0;fgets (linea, 60, f) != NULL;x++){
    if(linea[0] == '#'){ //Ignoro encabezado
      x--;
      continue;
    }
    if (ferror(f)){ //Error al leer los datos
      return false;
    }    
    coma[0] = strchr(linea,','); //Puntero a la posicion donde esta la primera coma para cortar la fecha del partido
    coma[1] = strchr(coma[0]+1,','); //Puntero a la posicion donde esta la segunda coma para cortar el jugador
    coma[2] = strchr(coma[1]+1,','); //Puntero a la posicion donde esta la segunda coma para cortar el idpartido
    eof = strchr(linea,'\r'); //Puntero a la posicion donde esta el salto de linea (CRLF)
    if(eof == NULL){
      eof = strchr(linea,'\n'); //Puntero a la posicion donde esta el salto de linea (LF)
    }
    //Fecha gol
    temp[0] = '\0';
    strncat(temp,linea,coma[0] - &linea[0]);
    fecha = atoi(temp);
    //Jugador
    nombreJugador[0] = '\0';
    strncat(nombreJugador,coma[0]+1,coma[1] - coma[0]-1);
    //Id equipo
    temp[0] = '\0';    
    strncat(temp,coma[1]+1,coma[2] - coma[1]-1); 
    idEquipo = atoi(temp);     
    //ID partido
    temp[0] = '\0';    
    strncat(temp,coma[2]+1,eof - coma[2]-1); 
    idPartido = atoi(temp);   
    insertar(golPila,x,fecha,idEquipo,&nombreJugador[0],idPartido);
  }
  fclose(f);
  return true;
}

//Inserta un registro en la lista de goles ordenado por equipo -> fecha
registroGol* insertar(registroGol *&listaGol, int idGol, int fecha, int codEquipo, char *nombreJugador, int idPartido){
  registroGol *aux;
  //Si la lista no esta vacia, declaro un nuevo Nodo
  registroGol* nuevoNodo = new registroGol();
  nuevoNodo -> idGol = idGol;
  nuevoNodo -> codEquipo = codEquipo;
  strcpy(nuevoNodo -> nombreJugador,nombreJugador); 
  nuevoNodo -> fecha = fecha;  
  nuevoNodo -> idPartido = idPartido;  
  nuevoNodo -> siguiente = NULL;

  //Si la lista esta vacia
  if(listaGol == NULL){ 
    //¿Por que devuelvo esto?
    listaGol = nuevoNodo;
    return nuevoNodo;
  }

  //Lista con un solo elemento
  if(listaGol -> codEquipo > codEquipo || (listaGol -> codEquipo == codEquipo && listaGol -> fecha > fecha)){
    nuevoNodo -> siguiente = listaGol;
    listaGol = nuevoNodo;
    return nuevoNodo;
  }

  //Aux lo voy a usar para recorrer la lista hasta cierta condicion o llegar al final de la lista
  aux = listaGol;
  //Caso General, recorro hasta encontrar donde insertar el nodo
  while(aux -> siguiente != NULL 
  && (aux -> siguiente -> codEquipo < codEquipo 
  || (aux -> siguiente -> codEquipo == codEquipo && aux -> siguiente -> fecha <= fecha))){
    aux = aux -> siguiente;
  }

  //Inserto el nodo
  nuevoNodo -> siguiente = aux -> siguiente;
  aux -> siguiente = nuevoNodo;
  return nuevoNodo;
}

//Graba en el Archivo binario toda la lista
bool grabarBinario(registroGol *&listaGol){
  registroGol *pNodo;
  pNodo = listaGol;
  FILE* f;
  f = fopen("datos.bin", "wb");
  if (f == NULL){ //Error al abrir el archivo en modo escritura
    return false;
  }
  while (pNodo != NULL){ //Recorro la lista y voy escribiendo el archivo binario
    fwrite(pNodo,sizeof(registroGol),1,f);
    if (ferror(f)){ //Error al escribir los datos
      return false;
    }
    pNodo = pNodo->siguiente;
  }
  fclose(f);
  return true;
}

//Lee el Archivo toda la lista, en memoria
bool leerBinario(registroGol *&listaGol){
  registroGol aux;
  FILE* f = fopen("datos.bin", "rb");
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
  int x,y;
  bool crearNodo; //Bandera para indicar si se debe crear un nuevo nodo de gol en el partido actual (si el jugador coincide)
  nodoLista* nuevoNodo = NULL,*auxNL;
  registroGol* auxRG = NULL;
  for(x=0 ; x<32; x++){
    for(y=0 ; y<7 ; y++){
      mPartidos[x][y] = NULL ;
    }
  }
  x=0;
  while ( listaGol != NULL ){
    //Si la primera columna esta vacia (NULL) insertar un nuevo nodo
    if(mPartidos[listaGol -> codEquipo -1][x] == NULL){
      nuevoNodo = new nodoLista();
      strcpy(nuevoNodo -> nombreJugador, listaGol-> nombreJugador);
      nuevoNodo -> fecha = listaGol -> fecha; 
      nuevoNodo -> goles = 1;
      nuevoNodo -> siguiente = NULL;
      mPartidos[listaGol -> codEquipo -1][x] = nuevoNodo; //Y lo inserto en la columna
      listaGol = listaGol -> siguiente; //Avanzo al siguiente registro
      x=0; //Reseteo el contador de columnas
      continue; //Repetir el LOOP
    }
    //Si el partido existentente en la columna no coincide con el leido avanzar a la siguiente columna
    if(mPartidos[listaGol -> codEquipo -1][x] -> fecha != listaGol -> fecha){
      x++; //Aumento en 1 el contador de columnas para buscar la siguiente
      continue; //Repetir el LOOP
    }
    //Si llego aca es que encontro un segundo registro del mismo partido, hay que determinar si el gol es del mismo jugador o hay que agregar un nodo
    auxNL = mPartidos[listaGol -> codEquipo -1][x]; //Hago una copia del principio de la lista 
    crearNodo = true; //Si esta bandera queda en true, se suma gol y no se añade nodo
    while(auxNL -> siguiente != NULL){ //Recorro la lista
      if(strcmp(auxNL -> nombreJugador,listaGol -> nombreJugador) == 0){ //Si el jugador es el mismo sumar los goles
        auxNL -> goles++;
        crearNodo = false; //Marco la bandera en false para evitar que se cree un nodo
        break;
      }
      auxNL = auxNL -> siguiente;
    }
    if(crearNodo){ // encontro un segundo registro del mismo partido y el jugador es distinto, agregar nuevo nodo
      //Si no encontre el jugador en la lista de goles del partido insertar un nuevo nodo
      nuevoNodo = new nodoLista();
      strcpy(nuevoNodo -> nombreJugador, listaGol-> nombreJugador);
      nuevoNodo -> fecha = listaGol -> fecha; 
      nuevoNodo -> goles=1;
      nuevoNodo -> siguiente=NULL;
      auxNL -> siguiente = nuevoNodo; //Insertar el nuevo nodo al final de la lista
    }
    listaGol = listaGol -> siguiente; //Avanzo al siguiente registro
    x=0; //Reseteo el contador de columnas
  }
}

void recorrerMatrizGoles (matriz mPartidos,equipos e[]){
  nodoLista* auxNL = NULL;
  int cantGoles,z;
  for(int x=0; x<32; x++){
    cout << "Equipo " << e[x].nombre << endl;
    cantGoles=0;
    for(int y=0; y<7; y++){
      auxNL = mPartidos[x][y];
      if(auxNL == NULL){
        continue;
      }
      cout << "\tPartido " << y+1 << " (" << mPartidos[x][y] -> fecha << ")" << endl;
      while(auxNL != NULL){
        cantGoles += auxNL->goles;
        cout << "\t\t" << auxNL -> nombreJugador << " - Goles: " << auxNL-> goles << endl;
        auxNL = auxNL -> siguiente;
      }
    }
    cout << "\tTotal goles : " << cantGoles << "\n\n" << endl;
  }
}