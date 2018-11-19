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
struct registroGol{
  int idGol;
  int codEquipo;
  int fecha;
  char nombreJugador[50];
  int idPartido;
  registroGol* siguiente;
};




void insertar(registroGol *&lista, int idGol, int fecha, int codEquipo, char *nombreJugador, int idPartido);
bool leerDatosTexto(equipos [],registroGol *&);
bool grabarBinario(registroGol *&);
/* Funciones auxiliares */
void pausa(const char *);
void pausa();


int main(){
  #ifdef _WIN32
    system ("color 0F" ); //Cambio el color de la consola
    SetConsoleOutputCP(CP_UTF8); //Cambia el code page a UTF-8 para evitar problemas con carácteres no ingleses (solo windows)
  #endif
  //Variables
	registroGol* listaGol; //Lista dinamica que carga el Archivo
  equipos e[32]; //Array de equipos
	listaGol = NULL;
  if(!leerDatosTexto(e,listaGol)){
    return 0;
  }
  grabarBinario(listaGol);
  pausa("\n>> Precione enter para salir del programa\n");
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

//Graba en el Archivo binario toda la lista
bool grabarBinario(registroGol *&listaGol){
  pausa("\n>> Presione enter para generar el archivo binario\n");
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
      cout <<"ERROR: No se pudo generar el archivo datos.bin" << endl;
      return false;
    }
    pNodo = pNodo->siguiente;
  }
  fclose(f);
  cout <<"Se ha generado el archivo datos.bin de forma exitosa" << endl;
  return true;
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
