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
#include <time.h>
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

/* PROTOTIPOS */
bool escribirArchivo(const char[],seleccionNacional[],int);
bool escribirArchivo(char[],seleccionNacional[],int);
bool sortearEquipos(grupoMundial[],seleccionNacional[]);
bool leerArchivo(char[],seleccionNacional[],int);


int main(){

  //setlocale(LC_CTYPE, "es-ES");
  #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); //Cambia el code page a UTF-8 para evitar problemas con carácteres no ingleses (solo windows)
  #endif
  //Defino arrays de las estructuras que usaremos
  seleccionNacional selNacionales[32];
  grupoMundial gruposSorteo[8];
  char rutaArchivoSelecciones [27] = "..\\EJERCICIO 1\\selecciones";

  if(!leerArchivo(rutaArchivoSelecciones, selNacionales, 32)){
    return 1;
  }
  if(!sortearEquipos(gruposSorteo,selNacionales)){ //Sorteo los grupos
    return 1;
  }
  system("pause");
}


bool leerArchivo(char nombreArchivo[],seleccionNacional lecturaEquipos[],int cant){
  char nombreArchivoExt[30]; //Nombre de archivo mas extension
  FILE *archivo; //Puntero al archivo
  strcpy(nombreArchivoExt,nombreArchivo);
  strcat(nombreArchivoExt,".bin");
  archivo = fopen(nombreArchivoExt,"rb"); //Lo abro en modo lectura binaria
  if(archivo == NULL){
    cout << "Error al abrir el archivo "<< nombreArchivoExt << " en modo lectura" << endl;
    return false;
  }
  fread(lecturaEquipos, sizeof(seleccionNacional), cant, archivo);
  if(ferror(archivo)){ //Compruebo que se hayan podido grabar los datos (ferror() == 0)
    cout << "Error al leer el archivo "<< nombreArchivoExt << endl;
    return false;
  }
  fclose(archivo);
  cout << "Archivo leído exitosamente " << nombreArchivoExt << endl;
  return true;
}

bool sortearEquipos(grupoMundial grupos[],seleccionNacional selecciones[]){
  srand (time(NULL)); // Inicializo mi funcin random con una seed del timestamp actual
  char letras[] = "ABCDEFGH";
  char nombreGrupo[9]; //Aca se va a almacenar el nombre del archivo a generar
  int x,y; //Auxiliares
  int mismaConfederacion; //Variable contadora para evaluar equipos de la misma confederacion en un grupo
  int numEquipo; //Equipo sorteado

  for(int x=0;x<8;x++){
    grupos[x].identificador = letras[x];
  }

  grupos[0].equipos[0] = selecciones[0]; // Asigno a Rusia al grupo A
  selecciones[0].asignadoGrupo = true; //Lo marco como asignado


  //Cabeza de serie -- PASO 1
  for(x=1;x<=7;x++){
    do{ //Este loop ejecutara hasta encontrar un equipo que no haya sido asignado a un grupo
      numEquipo = rand() % 7 + 1; //Saco un numero aleatorio
    }while(selecciones[numEquipo].asignadoGrupo);
    grupos[x].equipos[0] = selecciones[numEquipo]; //Asigno el equipo sorteado al grupo
    selecciones[numEquipo].asignadoGrupo = true; //Lo marco como asignado a un grupo
  }

  //Bolillero 2 -- PASO 2
  for(x=0;x<=7;x++){
    do{
      numEquipo = rand() % 8 + 8; //Saco un numero aleatorio
      if(!selecciones[numEquipo].asignadoGrupo){
        //Evaluo si el equipo sorteado es un equipo comnebol y el equipo cabeza de serie es argentina o brasil
        if(strcmp(selecciones[numEquipo].confederacion,"Conmebol") == 0 &&
          (strcmp(grupos[x].equipos[0].nombreEquipo,"Argentina") == 0 ||
          strcmp(grupos[x].equipos[0].nombreEquipo,"Brasil") == 0))
        {
          //En caso de que se cumpla la condicion anterior, deshacemos el paso 2 y volvemos a sortear el bolillero
          for(int y=8;y<=15;y++){
            selecciones[y].asignadoGrupo = false; //Desmarco la asignacion
            x=0;
          }
          continue; //Vuelvo a sacar un random
        }
        break; //Rompro el loop y asigno el equipo
      }
    }while(true);
    grupos[x].equipos[1] = selecciones[numEquipo];; //Asigno el equipo sorteado al grupo
    selecciones[numEquipo].asignadoGrupo = true;
  }

  //Bolillero 3 -- PASO 3
  for(x=0;x<=7;x++){
    do{ //Esto se repite hasta que deje de encontrar un equipo ya asignado a grupo
      numEquipo = rand() % 8 + 16; //Saco un numero aleatorio
      if(!selecciones[numEquipo].asignadoGrupo){
        mismaConfederacion = 0; //Indica cuantos equipos de la misma confederacion al sorteado hay en el grupo
        //Si hay un equipo de la misma confederacion al sorteado en la posicion 1 del grupo (0 en el array) y lo marco en un contador
        if(strcmp(grupos[x].equipos[0].confederacion,selecciones[numEquipo].confederacion) == 0){
          mismaConfederacion++;
        }
        //Si hay un equipo de la misma confederacion al sorteado en la posicion 2 del grupo (1 en el array) y lo marco en un contador
        if(strcmp(grupos[x].equipos[1].confederacion,selecciones[numEquipo].confederacion) == 0){
          mismaConfederacion++;
        }
        //Si hay mas de un equipo de la misma confederacion en el grupo (2 en caso UEFA), desasigno los equipos sorteados y repito el paso 3
        if((strcmp(selecciones[numEquipo].confederacion,"UEFA") != 0 && mismaConfederacion > 0) ||
          (strcmp(selecciones[numEquipo].confederacion,"UEFA") == 0 && mismaConfederacion > 1)){
          for(y=16;y<=23;y++){
            selecciones[y].asignadoGrupo = false; //Desmarco la asignacion
            x=0;
          }
          continue; //Vuelvo a sacar un random
        }
        break; //Rompro el loop y asigno el equipo
      }
    }while(true);
    grupos[x].equipos[2] = selecciones[numEquipo]; //Asigno el equipo sorteado al grupo
    selecciones[numEquipo].asignadoGrupo = true; //Lo marco como asignado a un grupo
  }

  //Bolillero 4 -- PASO 4
  for(x=0;x<=7;x++){
    do{ //Esto se repite hasta que deje de encontrar un equipo ya asignado a grupo
      numEquipo = rand() % 8 + 24; //Saco un numero aleatorio
      if(!selecciones[numEquipo].asignadoGrupo){
        mismaConfederacion = 0;
        //Si hay un equipo de la misma confederacion al sorteado en la posicion 1 del grupo (0 en el array) y lo marco en un contador
        if(strcmp(grupos[x].equipos[0].confederacion,selecciones[numEquipo].confederacion) == 0){
          mismaConfederacion++;
        }
        //Si hay un equipo de la misma confederacion al sorteado en la posicion 2 del grupo (1 en el array) y lo marco en un contador
        if(strcmp(grupos[x].equipos[1].confederacion,selecciones[numEquipo].confederacion) == 0){
          mismaConfederacion++;
        }
        //Si hay un equipo de la misma confederacion al sorteado en la posicion 3 del grupo (2 en el array) y lo marco en un contador
        if(strcmp(grupos[x].equipos[2].confederacion,selecciones[numEquipo].confederacion) == 0){
          mismaConfederacion++;
        }
        //Si hay mas de un equipo de la misma confederacion en el grupo (2 en caso UEFA), desasigno los equipos sorteados y repito el paso 4
        if((strcmp(selecciones[numEquipo].confederacion,"UEFA") != 0 && mismaConfederacion > 0) ||
          (strcmp(selecciones[numEquipo].confederacion,"UEFA") == 0 && mismaConfederacion > 1))
        {
          for(y=24;y<=31;y++){
            selecciones[y].asignadoGrupo = false; //Desmarco la asignacion
            x=0;
          }
          continue; //Vuelvo a sacar un random
        }
        break; //Rompro el loop y asigno el equipo
      }
    }while(true);
    grupos[x].equipos[3] = selecciones[numEquipo]; //Asigno el equipo sorteado al grupo
    selecciones[numEquipo].asignadoGrupo = true; //Lo marco como asignado a un grupo
  }

  //Recorro los grupos y lo grabo en archivos binarios
  for(x=0;x<8;x++){
    strcpy(nombreGrupo,"grupo_");
    strncat(nombreGrupo,&letras[x],1);
    if(!escribirArchivo(nombreGrupo,grupos[x].equipos,4)){
      return false;
    }
  }
  cout << "Grupos sorteados, 8 archivos binarios generados grupo_A.bin, grupo_B.bin, ..." << endl;
  return true;
}

/* Escribe un archivo binario con los datos proporcionados
 * Parametros:
 *  nombreArchivo -> Nombre del archivo que se escribira
 *  selNacionales -> Array de structs seleccionNacional a grabar
 *  cant -> Cantidad de registros del array proporcionado a grabar
 * Retorna -> true si la escritura fue exitosa, false en caso contrario
 */

/* Sobrecarga de funcion para que acepte el parametro nombreArchivo como un string constante (definido con comillas dobles) sin tirar warning al compilar */

bool escribirArchivo(const char nombreArchivo[],seleccionNacional selNacionales[],int cant){
  char _nombreArchivo[15];
  strcpy(_nombreArchivo,nombreArchivo);
  return escribirArchivo(_nombreArchivo,selNacionales,cant);
}

bool escribirArchivo(char nombreArchivo[],seleccionNacional selNacionales[],int cant){
  char nombreArchivoExt[15]; //Nombre de archivo mas extension
  FILE *archivo; //Puntero al archivo
  strcpy(nombreArchivoExt,nombreArchivo);
  strcat(nombreArchivoExt,".bin");
  archivo = fopen(nombreArchivoExt,"wb"); //Lo abro en modo escritura binaria (Sobreescribe)
  if(archivo == NULL){ //Si el puntero al archivo es null es que no se pudo abrir
    cout << "Error al abrir el archivo " << nombreArchivoExt << " en modo escritura" << endl;
    return false;
  }
  fwrite(selNacionales, sizeof(struct seleccionNacional), cant, archivo); //Grabo los struct en el archivo
  if(ferror(archivo)){ //Compruebo que se hayan podido grabar los datos (ferror() == 0)
    cout << "Error al escribir en el archivo "<< nombreArchivoExt << endl;
    return false;
  }
  fclose(archivo);
  return true;
}