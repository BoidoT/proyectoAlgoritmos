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

//Estructuras para el arbol
struct equipoGol{ //Almacena la cantidad de goles por equipo
  char nombreEquipo[50];
  int goles;
};

//Ya que puede haber mas de un equipo en cada nodo del arbol se arma una pila
struct equipoGolLista{
  equipoGol dato;
  equipoGolLista *siguiente;
};

struct nodoArbol{
  equipoGolLista *lista;
  nodoArbol *izquierda;
  nodoArbol *derecha;
};

void insertar(registroGol *&lista, int idGol, int fecha, int codEquipo, char *nombreJugador, int idPartido);
bool leerDatosTexto(equipos [],registroGol *&);
int recorrerLista(registroGol *&listaGol);
bool grabarBinario(registroGol *&);
bool leerBinario(registroGol *&);
void generarMatrizGoles(registroGol *&, matriz);
void recorrerMatrizGoles (matriz,equipos[]);
//Funciones para btree
void insertaNodo(nodoArbol *&,equipoGol);
void recorrerInOrder(nodoArbol *);
void equipoGolListaInsertar(equipoGolLista *&,equipoGol);
void dibujarArbol(nodoArbol *);
int dibujarArbolRecursivo(nodoArbol *,bool,int,int,char[20][120]);
/* Funciones auxiliares */
void pausa(const char *);
void pausa();
bool lineaEnBlanco(char [],int);

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

void recorrerMatrizGoles (matriz mPartidos,equipos e[]){
  cout << "-------- RESULTADO RAKING --------" << endl;
  nodoLista* auxNL = NULL;
  int cantGoles,x,y;
  equipoGol eg[32];
  for(x=0; x<32; x++){
    cout << "Equipo " << e[x].nombre << endl;
    cantGoles=0;
    for(y=0; y<7; y++){
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
    cout << "--------------------------------------------------------" <<endl;
    strncpy(eg[x].nombreEquipo,e[x].nombre,50);
    eg[x].goles = cantGoles;
  }
  //CREAR BTREE
  nodoArbol *arbol = NULL;
  for(x=0;x<32;x++){
    insertaNodo(arbol,eg[x]);
  }
  pausa("\n>> Presione una tecla para mostrar el arbol binario completo\n");
  cout << "--- ARBOL BINARIO DE GOLES POR EQUIPO ---" << endl;
  cout << "Nodo: goles(cantidad equipos)\n" << endl;
  dibujarArbol(arbol);
  pausa("\n>> Presione una tecla para recorrer el arbol binario en modo inOrder\n");
  recorrerInOrder(arbol);
}

void equipoGolListaInsertar(equipoGolLista *&l,equipoGol valor){
  //Creo un nodo
  equipoGolLista *nuevo = new equipoGolLista;
  nuevo -> dato = valor;
  nuevo -> siguiente = NULL;
  if(l == NULL){
    l = nuevo;
    return;
  }
  equipoGolLista *aux = l; //Puntero para recorrer
  while(aux -> siguiente != NULL){
    aux = aux -> siguiente;
  }
  aux -> siguiente = nuevo;
}

void insertaNodo(nodoArbol *&arbol,equipoGol valor){
  //Si esta vacio insertar el nodo inicial
  if(arbol == NULL){
    nodoArbol *nuevo = new nodoArbol();
    nuevo -> lista = new equipoGolLista; //Creo una nueva pila con los equipos
    nuevo -> lista = NULL;
    equipoGolListaInsertar(nuevo -> lista,valor);
    nuevo -> izquierda = NULL;
    nuevo -> derecha = NULL;
    arbol = nuevo;
    return;
  }
  //Si no esta vacío determinar a que rama va
  if(valor.goles < arbol -> lista -> dato.goles){ //Rama izquierda
    insertaNodo(arbol -> izquierda,valor);
    return;
  }
  if(valor.goles > arbol -> lista -> dato.goles){ //Rama derecha
    insertaNodo(arbol -> derecha,valor);
    return;
  }
  if(valor.goles == arbol -> lista -> dato.goles){ //Valor duplicado no inservar
    equipoGolListaInsertar(arbol -> lista,valor);
  }
}

//Funcion cabecera para dibujar un arbol binario por consola
void dibujarArbol(nodoArbol *arbol){
  char matrizCaracter[40][120];
  int x;
  if(arbol == NULL){ //Si el arbol esta vacío no hacer nada
    return;
  }
  for(x = 0;x < 20;x++){
    sprintf(matrizCaracter[x], "%119s", " "); //Relleno la matriz de espacios en blanco
  }
  dibujarArbolRecursivo(arbol, false, 0, 0, matrizCaracter); //Llamo a la funcion recursiva
  //Dibujo el resultado del arbol binario
  for(x = 0; x < 20; x++){
    if(lineaEnBlanco(matrizCaracter[x],120)){ //Lineas en blanco cortar
      break;
    }
    printf("%s\n", matrizCaracter[x]);
  }
  cout << "\n" << endl;
}

/*
 * funcion que es llamada recursivamente para dibujar un arbol binario
 * Parametros:
 *  arbol: el arbol binario a dibujar
 *  ramaIzquierda: indica si es una rama izquierda (true) o derecha (false)
 *  ajuste: margen izquierdo acumulado de las iteraciones anteriores
 *  nivelArbol: nivel de profundidad del arbol (0 es la raiz)
 *  matrizCaracter: matriz de char donde se dibuja el arbol
 * Retorna: margen a la izquierda que se debe dejar para la siguiente iteracion
*/
int dibujarArbolRecursivo(nodoArbol *arbol, bool ramaIzquierda, int ajuste, int nivelArbol, char matrizCaracter[20][120]){
  char b[20]; //Buffer de char donde guardar el dato a dibujar
  int ancho = 6; //Ancho de cada nodo - AA(BB)
  int x;

  if(arbol == NULL){ //Si llego al final de una rama retornar 0 margen
    return 0;
  }

  equipoGolLista *aux = arbol -> lista;
  for(x=0;aux != NULL;){
    x++;
    aux = aux -> siguiente;
  }
  sprintf(b, "%02d(%02d)", arbol->lista->dato.goles, x); //Grabo el dato en el buffer
  //Calculo recursivamente el margen de los nodos izquierdos
  int margenIzquierda  = dibujarArbolRecursivo(arbol->izquierda, true,ajuste,nivelArbol + 1, matrizCaracter);
  //Calculo recursivamente el margen de los nodos derechos
  int margenDerecha = dibujarArbolRecursivo(arbol->derecha, false,ajuste + margenIzquierda + ancho, nivelArbol + 1, matrizCaracter);

  //Grabo el dato en la matriz a imprimir por pantalla con los margenes calculados (donde ajuste es el acumulado de iteraciones anteriores)
  for(x = 0;x < ancho;x++){
    matrizCaracter[2 * nivelArbol][ajuste + margenIzquierda + x] = b[x];
  }

  //Si es la raiz no dibujar lineas ni signos +/-
  if(nivelArbol == 0){
    return margenIzquierda + ancho + margenDerecha;
  }

  if(ramaIzquierda){ //Estoy dibujando un nodo izquierdo
    //Dibujo la linea punteada --- hacia la derecha
    for (x = 0; x < ancho + margenDerecha; x++){
      matrizCaracter[2 * nivelArbol - 1][ajuste + margenIzquierda + ancho/2 + x] = '-';
    }
    //Pongo un signo de + debajo del nodo padre
    matrizCaracter[2 * nivelArbol - 1][ajuste + margenIzquierda + ancho/2] = '+';
    //Pongo un signo de + encima del nodo actual
    matrizCaracter[2 * nivelArbol - 1][ajuste + margenIzquierda + ancho + margenDerecha + ancho/2] = '+';
  }else{ //Estoy dibujando un nodo derecho
    //Dibujo la linea punteada --- hacia la izquierda
    for(x = 0;x < margenIzquierda + ancho;x++){
      matrizCaracter[2 * nivelArbol - 1][ajuste - ancho/2 + x] = '-';
    }
    //Pongo un signo de + debajo del nodo padre
    matrizCaracter[2 * nivelArbol - 1][ajuste + margenIzquierda + ancho/2] = '+';
    //Pongo un signo de + encima del nodo actual
     matrizCaracter[2 * nivelArbol - 1][ajuste - ancho/2 - 1] = '+';
  }
  return margenIzquierda + ancho + margenDerecha; //Retorno el margen acumulado para la proxima iteracion
}

//Recorrer el arbol en modo inorder
void recorrerInOrder(nodoArbol *arbol){
  //Si el arbol es null no hacer nada
  if(arbol == NULL){
    return;
  }
  equipoGolLista *aux; //Auxiliar para recorrer lista
  recorrerInOrder(arbol -> izquierda); //Primero la rama izquierda
  cout << "NODO (" << arbol -> lista -> dato.goles << ")" << endl;
  aux = arbol -> lista;
  while(aux != NULL){
    cout << "\tEquipo: " << aux -> dato.nombreEquipo << " Goles: " << aux -> dato.goles << endl; //Luego la raiz
    aux = aux -> siguiente;
  }
  recorrerInOrder(arbol -> derecha); //Por ultimo la rama derecha
}

//AUXILIARES
//Muestra el mensaje en pantalla presione enter para continuar y espera la pulsacion de una tecla
void pausa(){
  pausa("Presione enter para continuar..."); //Mensaje por default
}
void pausa(const char *msg){
  cout << msg << endl;
  getchar();
}
//Retorna false si el string solo tiene espacios en blanco
bool lineaEnBlanco(char str[],int largo){
  for(int x=0;x<largo-10;x++){
    if(str[x] != ' '){
      return false;
    }
  }
  return true;
}