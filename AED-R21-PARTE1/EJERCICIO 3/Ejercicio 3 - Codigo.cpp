/* TP ANUAL ALGORITMOS
  Integrantes:
    Alejandro	López	1676969	K1091
    Tomas	Boido	1677548	K1091
    Andrea	Zumárraga	1677640	K1022
    Julián	Sepúlveda	1682696	K1091
    Nicolás	García	1682880	K1091
    Christian	Cortez	1668699	K1091
    Diego	Simon	1682258	K1091
*/
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32 //Solo incluir si se compila en windows
  #include <Windows.h> //Libreria para setear la consola en utf8 en windows
#endif
#include <limits.h>

using namespace std;

/* DEFINICION DE STRUCTS */
struct seleccionNacional{
  int numBolillero;
  char nombreEquipo[30];
  char confederacion[20];
  bool asignadoGrupo; //Bandera para indicar que el equipo fue asignado a un grupo
};

struct grupoMundial{
  char identificador; //Letra del grupo
  seleccionNacional equipos[4];
};

struct seleccionesAgrupadas{
  char nombreEquipo[30];
  char confederacion[20];
  char grupo;
};

/* PROTOTIPOS */
seleccionNacional llenarEquipo(int,const char[],const char[]);
bool leerArchivo(char[],seleccionNacional[],int);
bool leerGrupos(seleccionesAgrupadas[]);
void ordenarEquipos(seleccionesAgrupadas[],int);
int menu();

int main(){
  int opcionMenu;
  //setlocale(LC_CTYPE, "es-ES");
  #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); //Cambia el code page a UTF-8 para evitar problemas con carácteres no ingleses (solo windows)
  #endif
  //Defino arrays de las estructuras que usaremos
  seleccionNacional selNacionales[32];
  grupoMundial gruposSorteo[8];
  seleccionesAgrupadas selAgrupadas[32];

  if(!leerGrupos(selAgrupadas)){ //Leo los archivos generados
    cout << "Presione una tecla para salir..." << endl;
    getchar();
    return 1;
  }
  while(true){
    opcionMenu = menu();
    if(!opcionMenu){ // 0 (false) = salir
      return 0; //Termino la ejecucion correctamente
    }
    ordenarEquipos(selAgrupadas,opcionMenu); //Pregunto al usuario como quiero ordenar y muestro el resultado
  }
}

/* Menu de opciones - Retorna opcion seleccionada */
int menu(){
  int opcion = -1;
  cout << "Cómo desea ordenar los equipos?" << endl;
  cout << "\t1: Letra de Grupo." << endl;
  cout << "\t2: Nombre de equipo." << endl;
  cout << "\t3: Confederación." << endl;
  cout << "\t4: Número de Grupo y confederación." << endl;
  cout << "\t5: Número de Grupo, confederación y nombre de equipo." << endl;
  cout << "\t0: Salir." << endl;
  do{
    cin >> opcion;
    if(!cin || opcion < 0 || opcion > 5){
      cout << "Opcion no válida" << endl;
      cin.clear();
      cin.ignore(INT_MAX, '\n');
      opcion = -1;
      continue;
    }
    #ifdef _WIN32
      system("cls"); //Limpio la consola   (windows)
    #else
      system("clear"); //Limpio la consola  (unix)
    #endif
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    return opcion;
  }while(true);
}


/* Lee los archivos binarios con los grupos (grupo_A.bin, grupo_B.bin...) y los mete en un array de seleccionesAgrupadas
 * Parametros:
 *  selAgrupadas -> Array de structs seleccionesAgrupadas a llenar
 * Retorna -> true si se pudo leer todos los archivos, false en caso contrario
*/
bool leerGrupos(seleccionesAgrupadas selAgrupadas[]){
  char letras[] = "ABCDEFGH";
  int x,y;
  char nombreArchivo[]="..\\EJERCICIO 2\\grupo_x.bin";

  seleccionNacional selNacional[4]; //Array temporal de seleccionNacional para guardar la lectura de archivos
  for(x=0;x<8;x++){
    nombreArchivo[strlen(nombreArchivo)-5] = letras[x];
    if(!leerArchivo(nombreArchivo,selNacional,4)){ //Leo el archivo y le paso el struct temporal de 4 equipos
      cout << "Hubo un error al procesar el archivo binario del grupo " << letras[x] << endl;
      return false;
    }
    for(y=0;y<4;y++){ //Agrego los resultados al array de seleccionesAgrupadas
      strcpy(selAgrupadas[(x*4)+y].nombreEquipo,selNacional[y].nombreEquipo);
      strcpy(selAgrupadas[(x*4)+y].confederacion,selNacional[y].confederacion);
      selAgrupadas[(x*4)+y].grupo = letras[x];
    }
  }
  return true;
}

/* Ordena los equipos
 * Parametros:
 *  selAgrupadas -> array de structs seleccionesAgrupadas con los 32 equipos
 *  modo -> tipo de ordenamiento: 1=letra grupo, 2=nombre equipo, 3=confederacion, 4=grupo y confederacion, 5=grupo, confederacion y nombre
*/
void ordenarEquipos(seleccionesAgrupadas selAgrupadas[],int modo){
  char letras[] = "ABCDEFGH";
  int x;
  bool cambio; //Indica que se hizo un cambio
  seleccionesAgrupadas tmp; //Temporal para hacer el intercambio
  switch(modo){
    case 1: //letra grupo
      do{ //Se repite hasta que no haya mas cambios
        cambio = false; //se resetea bandera
        for(x=0;x<31;x++){
          if(selAgrupadas[x+1].grupo < selAgrupadas[x].grupo){ //Comparo registros
            tmp = selAgrupadas[x];
            selAgrupadas[x] = selAgrupadas[x+1];
            selAgrupadas[x+1] = tmp;
            cambio = true; //Si hubo un cambio se marca la bandera para hacer un ciclo mas
          }
        }
      }while(cambio);
    break;
    case 2: //nombre equipo
      do{
        cambio = false;
        for(x=0;x<31;x++){
          if(strcmp(selAgrupadas[x+1].nombreEquipo,selAgrupadas[x].nombreEquipo) < 0){
            tmp = selAgrupadas[x];
            selAgrupadas[x] = selAgrupadas[x+1];
            selAgrupadas[x+1] = tmp;
            cambio = true;
          }
        }
      }while(cambio);
    break;
    case 3: //confederacion
      do{
        cambio = false;
        for(x=0;x<31;x++){
          if(strcmp(selAgrupadas[x+1].confederacion,selAgrupadas[x].confederacion) < 0){
            tmp = selAgrupadas[x];
            selAgrupadas[x] = selAgrupadas[x+1];
            selAgrupadas[x+1] = tmp;
            cambio = true;
          }
        }
      }while(cambio);
    break;
    case 4: //grupo y confederacion
      do{
        cambio = false;
        for(x=0;x<31;x++){
          if(selAgrupadas[x+1].grupo < selAgrupadas[x].grupo){ //Primero comparo el grupo
            tmp = selAgrupadas[x];
            selAgrupadas[x] = selAgrupadas[x+1];
            selAgrupadas[x+1] = tmp;
            cambio = true;
          }
          if(selAgrupadas[x+1].grupo == selAgrupadas[x].grupo){ //Si los grupos son iguales comparo la confederacion
            if(strcmp(selAgrupadas[x+1].confederacion,selAgrupadas[x].confederacion) < 0){
              tmp = selAgrupadas[x+1];
              selAgrupadas[x+1] = selAgrupadas[x];
              selAgrupadas[x] = tmp;
              cambio = true;
            }
          }
        }
      }while(cambio);
    break;
    case 5: //grupo, confederacion y nombre
      do{
        cambio = false;
        for(x=0;x<31;x++){
          if(selAgrupadas[x+1].grupo < selAgrupadas[x].grupo){ //Primero comparo el grupo
            tmp = selAgrupadas[x+1];
            selAgrupadas[x+1] = selAgrupadas[x];
            selAgrupadas[x] = tmp;
            cambio = true;
          }
          if(selAgrupadas[x+1].grupo == selAgrupadas[x].grupo){ //Si los grupos son iguales comparo la confederacion
            if(strcmp(selAgrupadas[x+1].confederacion,selAgrupadas[x].confederacion) < 0){
              tmp = selAgrupadas[x+1];
              selAgrupadas[x+1] = selAgrupadas[x];
              selAgrupadas[x] = tmp;
              cambio = true;
            }
            if(strcmp(selAgrupadas[x+1].confederacion,selAgrupadas[x].confederacion) == 0){ //Si el grupo y confederacion son iguales comparo los nombres de equipo
              if(strcmp(selAgrupadas[x+1].nombreEquipo,selAgrupadas[x].nombreEquipo) < 0){
                tmp = selAgrupadas[x+1];
                selAgrupadas[x+1] = selAgrupadas[x];
                selAgrupadas[x] = tmp;
                cambio = true;
              }
            }
          }
        }
      }while(cambio);
    break;
  }
  //Muestro en pantalla el resultado
  for(x=0;x<32;x++){
    cout << x+1 << " Grupo: " << selAgrupadas[x].grupo << " " << selAgrupadas[x].nombreEquipo << " - " << selAgrupadas[x].confederacion << endl;
  }
  cout << "Precione una tecla para volver al menú" << endl;
  getchar();
}

bool leerArchivo(char nombreArchivo[],seleccionNacional lecturaEquipos[],int cant){
  FILE *archivo; //Puntero al archivo
  archivo = fopen(nombreArchivo,"rb"); //Lo abro en modo lectura binaria
  if(archivo == NULL){
    cout << "Error al abrir el archivo "<< nombreArchivo << " en modo lectura" << endl;
    return false;
  }
  fread(lecturaEquipos, sizeof(seleccionNacional), cant, archivo);
  if(ferror(archivo)){ //Compruebo que se hayan podido grabar los datos (ferror() == 0)
    cout << "Error al leer el archivo "<< nombreArchivo << endl;
    return false;
  }
  fclose(archivo);
  return true;
}