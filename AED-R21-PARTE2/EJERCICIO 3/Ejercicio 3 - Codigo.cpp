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

struct equiposGoles{
  equipos datos;
  int goles;
};

struct nodoEquiposGoles{
  equiposGoles datos;
  nodoEquiposGoles *siguiente;
};

struct nodoArbol{
  nodoEquiposGoles *lista;
  nodoArbol *izquierda;
  nodoArbol *derecha;
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


void insertar(nodoRegistroGol *&lista, registroGol d);
bool leerBinario(nodoRegistroGol *&, equiposGoles []);
void contarGoles(nodoRegistroGol *&, equiposGoles []);
//Funciones para btree
void insertaNodo(nodoArbol *&,equiposGoles);
void recorrerInOrder(nodoArbol *);
void nodoEquipoGolInsertar(nodoEquiposGoles *&,equiposGoles);
void dibujarArbol(nodoArbol *);
int dibujarArbolRecursivo(nodoArbol *,bool,int,int,char[20][120]);
/* Funciones auxiliares */
void pausa(const char *);
void pausa();
bool lineaEnBlanco(char [],int);
void formatoFecha(int,char []);

int main(){
  #ifdef _WIN32
    system ("color 0F" ); //Cambio el color de la consola
    SetConsoleOutputCP(CP_UTF8); //Cambia el code page a UTF-8 para evitar problemas con carácteres no ingleses (solo windows)
  #endif
  //Variables
	nodoRegistroGol* listaGol = NULL; //Lista dinamica que carga el Archivo
  equiposGoles vEquipos[32];
  if(!leerBinario(listaGol,vEquipos)){
    pausa(">> Presione enter para salir del programa");
    return 1;
  }
  contarGoles(listaGol ,vEquipos);
  //CREAR BTREE
  nodoArbol *arbol = NULL;
  for(int x=0;x<32;x++){
    insertaNodo(arbol,vEquipos[x]);
  }
  cout << "--- ARBOL BINARIO DE GOLES POR EQUIPO ---" << endl;
  cout << "Nodo: goles(cantidad equipos)\n" << endl;
  dibujarArbol(arbol);
  pausa("\n>> Presione enter para recorrer el arbol binario en modo inOrder\n");
  recorrerInOrder(arbol);
  pausa("\n>> Precione enter para salir del programa\n");
  return 0;
}

//Lee el Archivo toda la lista, en memoria
bool leerBinario(nodoRegistroGol *&listaGol,equiposGoles e[]){
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
    e[x].datos.id = auxE.id;
    strncpy(e[x].datos.nombre,auxE.nombre,30);
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

void contarGoles(nodoRegistroGol*& listaGol,equiposGoles eq[]){
  int x;
  for(x=0;x<32;x++){
    eq[x].goles = 0;
  }
  while(listaGol != NULL){
    for(int x=0;x<32;x++){
      if(eq[x].datos.id == listaGol -> datos.codEquipo){
        eq[x].goles++;
      }
    }
    listaGol = listaGol -> siguiente;
  }
}

void insertaNodo(nodoArbol *&arbol,equiposGoles valor){
  //Si esta vacio insertar el nodo inicial
  if(arbol == NULL){
    nodoArbol *nuevo = new nodoArbol;
    nuevo -> lista = new nodoEquiposGoles; //Creo una nueva pila con los equipos
    nuevo -> lista = NULL;
    nodoEquipoGolInsertar(nuevo -> lista,valor);
    nuevo -> izquierda = NULL;
    nuevo -> derecha = NULL;
    arbol = nuevo;
    return;
  }
  //Si no esta vacío determinar a que rama va
  if(valor.goles < arbol -> lista -> datos.goles){ //Rama izquierda
    insertaNodo(arbol -> izquierda,valor);
    return;
  }
  if(valor.goles > arbol -> lista -> datos.goles){ //Rama derecha
    insertaNodo(arbol -> derecha,valor);
    return;
  }
  if(valor.goles == arbol -> lista -> datos.goles){ //Valor duplicado no inservar
    nodoEquipoGolInsertar(arbol -> lista,valor);
  }
}

void nodoEquipoGolInsertar(nodoEquiposGoles *&l,equiposGoles valor){
  //Creo un nodo
  nodoEquiposGoles *nuevo = new nodoEquiposGoles;
  nuevo -> datos = valor;
  nuevo -> siguiente = NULL;
  if(l == NULL){
    l = nuevo;
    return;
  }
  nodoEquiposGoles *aux = l; //Puntero para recorrer
  while(aux -> siguiente != NULL){
    aux = aux -> siguiente;
  }
  aux -> siguiente = nuevo;
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

  nodoEquiposGoles *aux = arbol -> lista;
  for(x=0;aux != NULL;){
    x++;
    aux = aux -> siguiente;
  }
  sprintf(b, "%02d(%02d)", arbol->lista->datos.goles, x); //Grabo el dato en el buffer
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
  nodoEquiposGoles *aux; //Auxiliar para recorrer lista
  recorrerInOrder(arbol -> izquierda); //Primero la rama izquierda
  cout << "NODO (" << arbol -> lista -> datos.goles << ")" << endl;
  aux = arbol -> lista;
  while(aux != NULL){
    cout << "\tEquipo: " << aux -> datos.datos.nombre << " Goles: " << aux -> datos.goles << endl; //Luego la raiz
    aux = aux -> siguiente;
  }
  recorrerInOrder(arbol -> derecha); //Por ultimo la rama derecha
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
//Retorna false si el string solo tiene espacios en blanco
bool lineaEnBlanco(char str[],int largo){
  for(int x=0;x<largo-10;x++){
    if(str[x] != ' '){
      return false;
    }
  }
  return true;
}