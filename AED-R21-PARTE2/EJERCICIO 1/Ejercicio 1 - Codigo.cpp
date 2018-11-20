#include <iostream>
#include <string.h>
#ifdef _WIN32 //Solo incluir si se compila en windows
  #include <Windows.h> //Libreria para setear la consola en utf8 en windows
#endif

using namespace std;

//Equipos
struct equipos{
  int id;
  char nombre[50];
};


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

void insertarOrdenado(nodoRegistroGol *&lista, registroGol d);
bool leerDatosTexto(equipos [],nodoRegistroGol *&);
bool grabarBinarios(nodoRegistroGol *,equipos []); //Genera los binarios con los equipos y los goles
/* Funciones auxiliares */
void pausa(const char *);
void pausa();

int main(){
  #ifdef _WIN32
    system ("color 0F" ); //Cambio el color de la consola
    SetConsoleOutputCP(CP_UTF8); //Cambia el code page a UTF-8 para evitar problemas con carácteres no ingleses (solo windows)
  #endif
	nodoRegistroGol *listaGol = NULL; //Lista dinamica que carga el Archivo
  equipos vEquipos[32]; //Array de equipos
  if(!leerDatosTexto(vEquipos,listaGol)){ //Leo los archivos de texto
    pausa(">> Presione enter para salir del programa");
    return 1;
  }
  if(!grabarBinarios(listaGol,vEquipos)){ //Grabo los binarios
    pausa(">> Presione enter para salir del programa");
    return 1;
  }
  pausa(">> Presione enter para salir del programa");
  return 0;
}

//Funcion auxiliar que lee los archivos separados por comas: equipos.csv y goles.csv para armar las listas
bool leerDatosTexto(equipos e[],nodoRegistroGol *&listaGol){
  FILE *f;
  char linea[60]; //Aca se lee cada linea
  char temp[60]; //Usado para convertir char[] -> int
  char *coma[3],*eof; //Puntero para separar los campos de cada linea
  int x;
  registroGol datos;

  for(x=0;x<32;x++){
    e[x].id = 0;
  }

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
      cout << "ERROR: No se pudo leer el archivo [equipos.csv]" << endl;
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
      cout << "ERROR: No se pudo leer el archivo [goles.csv]" << endl;
      return false;
    }
    coma[0] = strchr(linea,','); //Puntero a la posicion donde esta la primera coma para cortar la fecha del partido
    coma[1] = strchr(coma[0]+1,','); //Puntero a la posicion donde esta la segunda coma para cortar el jugador
    coma[2] = strchr(coma[1]+1,','); //Puntero a la posicion donde esta la segunda coma para cortar el idpartido
    eof = strchr(linea,'\r'); //Puntero a la posicion donde esta el salto de linea (CRLF)
    if(eof == NULL){
      eof = strchr(linea,'\n'); //Puntero a la posicion donde esta el salto de linea (LF)
    }
    //IdGol
    datos.idGol = x;
    //Fecha gol
    temp[0] = '\0';
    strncat(temp,linea,coma[0] - &linea[0]);
    datos.fecha = atoi(temp);
    //Jugador
    datos.nombreJugador[0] = '\0';
    strncat(datos.nombreJugador,coma[0]+1,coma[1] - coma[0]-1);
    //Id equipo
    temp[0] = '\0';
    strncat(temp,coma[1]+1,coma[2] - coma[1]-1);
    datos.codEquipo = atoi(temp);
    //ID partido
    temp[0] = '\0';
    strncat(temp,coma[2]+1,eof - coma[2]-1);
    datos.idPartido = atoi(temp);
    insertarOrdenado(listaGol,datos);
  }
  fclose(f);
  return true;
}

//Inserta un registro en la lista de goles ordenado por equipo -> fecha
void insertarOrdenado(nodoRegistroGol *&listaGol, registroGol d){
  //Si la lista no esta vacia, declaro un nuevo Nodo
  nodoRegistroGol* nuevoNodo = new nodoRegistroGol;
  nuevoNodo -> datos = d;
  nuevoNodo -> siguiente = NULL;

  //Si la lista esta vacia asignarla al nodo nuevo
  if(listaGol == NULL){
    listaGol = nuevoNodo;
    return;
  }

  //Comparo con el primer elemento de la lista
  if(listaGol -> datos.codEquipo > d.codEquipo || (listaGol -> datos.codEquipo == d.codEquipo && listaGol -> datos.fecha > d.fecha)){
    nuevoNodo -> siguiente = listaGol;
    listaGol = nuevoNodo;
    return;
  }

  nodoRegistroGol *aux = listaGol; //Aux lo voy a usar para recorrer la lista
  //Recorro hasta encontrar donde insertar el nodo (o llegar al final de la lista)
  while(aux -> siguiente != NULL
  && (aux -> siguiente -> datos.codEquipo < d.codEquipo
  || (aux -> siguiente -> datos.codEquipo == d.codEquipo && aux -> siguiente -> datos.fecha <= d.fecha))){
    aux = aux -> siguiente;
  }

  //Inserto el nodo
  nuevoNodo -> siguiente = aux -> siguiente;
  aux -> siguiente = nuevoNodo;
  return;
}

//Graba en el Archivo binario toda la lista, tambien genera un binario auxiliar con los equipos
bool grabarBinarios(nodoRegistroGol *listaGol, equipos e[]){
  pausa("\n>> Presione enter para generar el archivo binario\n");
  nodoRegistroGol *aux = listaGol;
  registroGol auxS;
  FILE* f;
  f = fopen("goles.bin", "wb");
  if (f == NULL){ //Error al abrir el archivo en modo escritura
    cout << "ERROR: No se pudo generar el archivo [goles.bin]" << endl;
    return false;
  }
  while (aux != NULL){ //Recorro la lista y voy escribiendo el archivo binario
    auxS = aux -> datos;
    fwrite(&auxS,sizeof(registroGol),1,f);
    if (ferror(f)){ //Error al escribir los datos
      cout << "ERROR: No se pudo generar el archivo [goles.bin]" << endl;
      return false;
    }
    aux = aux -> siguiente;
  }
  fclose(f);
  f = fopen("equipos.bin", "wb");
  if (f == NULL){ //Error al abrir el archivo en modo escritura
    cout << "ERROR: No se pudo generar el archivo [equipos.bin]" << endl;
    return false;
  }
  for(int x=0;x<32 && e[x].id != 0;x++){ //Recorro la lista y voy escribiendo el archivo binario
    fwrite(&e[x],sizeof(equipos),1,f);
    if (ferror(f)){ //Error al escribir los datos
      cout << "ERROR: No se pudo generar el archivo [equipos.bin]" << endl;
      return false;
    }
  }
  fclose(f);
  cout << "Se han generado los archivos [equipos.bin] y [goles.bin] de forma exitosa\n" << endl;
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