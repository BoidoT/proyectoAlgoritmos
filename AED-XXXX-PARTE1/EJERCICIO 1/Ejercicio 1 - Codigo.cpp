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


/* PROTOTIPOS */
bool escribirArchivo(const char[],seleccionNacional[],int);
bool escribirArchivo(char[],seleccionNacional[],int);
bool llenarEquipos(seleccionNacional[]);
seleccionNacional llenarEquipo(int,const char[],const char[]);


int main(){
  int opcionMenu;
  //setlocale(LC_CTYPE, "es-ES");
  #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); //Cambia el code page a UTF-8 para evitar problemas con carácteres no ingleses (solo windows)
  #endif
  //Defino arrays de las estructuras que usaremos
  seleccionNacional selNacionales[32];

  if(!llenarEquipos(selNacionales)){ //Defino los equipos
    return 1;
  }
  system("pause");
  return 0;
}

bool llenarEquipos(seleccionNacional selNacionales[]){
  selNacionales[0] = llenarEquipo(1, "Rusia", "UEFA");
  selNacionales[1] = llenarEquipo(1, "Alemania", "UEFA");
  selNacionales[2] = llenarEquipo(1, "Brasil", "Conmebol");
  selNacionales[3] = llenarEquipo(1, "Portugal", "UEFA");
  selNacionales[4] = llenarEquipo(1, "Argentina", "Conmebol");
  selNacionales[5] = llenarEquipo(1, "Bélgica", "UEFA");
  selNacionales[6] = llenarEquipo(1, "Polonia", "UEFA");
  selNacionales[7] = llenarEquipo(1, "Francia", "UEFA");

  selNacionales[8] = llenarEquipo(2, "España", "UEFA");
  selNacionales[9] = llenarEquipo(2, "Perú", "Conmebol");
  selNacionales[10] = llenarEquipo(2, "Suiza", "UEFA");
  selNacionales[11] = llenarEquipo(2, "Inglaterra", "UEFA");
  selNacionales[12] = llenarEquipo(2, "Colombia", "Conmebol");
  selNacionales[13] = llenarEquipo(2, "México", "Concacaf");
  selNacionales[14] = llenarEquipo(2, "Uruguay", "Conmebol");
  selNacionales[15] = llenarEquipo(2, "Croacia", "UEFA");

  selNacionales[16] = llenarEquipo(3, "Dinamarca", "UEFA");
  selNacionales[17] = llenarEquipo(3, "Islandia", "UEFA");
  selNacionales[18] = llenarEquipo(3, "Costa Rica", "Concacaf");
  selNacionales[19] = llenarEquipo(3, "Suecia", "UEFA");
  selNacionales[20] = llenarEquipo(3, "Túnez", "CAF");
  selNacionales[21] = llenarEquipo(3, "Egipto", "CAF");
  selNacionales[22] = llenarEquipo(3, "Senegal", "CAF");
  selNacionales[23] = llenarEquipo(3, "Irán", "AFC");

  selNacionales[24] = llenarEquipo(4, "Serbia", "UEFA");
  selNacionales[25] = llenarEquipo(4, "Nigeria", "AFC");
  selNacionales[26] = llenarEquipo(4, "Australia", "AFC");
  selNacionales[27] = llenarEquipo(4, "Japón", "AFC");
  selNacionales[28] = llenarEquipo(4, "Marruecos", "CAF");
  selNacionales[29] = llenarEquipo(4, "Panamá", "Concacaf");
  selNacionales[30] = llenarEquipo(4, "Corea del Sur", "AFC");
  selNacionales[31] = llenarEquipo(4, "Arabia Saudita", "AFC");
  if(!escribirArchivo("selecciones",selNacionales,32)){
    cout << "Hubo un error al generar el archivo binario con las selecciones" << endl;
    return false;
  }else{
    cout << "32 equipos generados, archivo selecciones.bin generado" << endl;
    return true;
  }
}

/* Crea un registro del tipo seleccionNacional y lo retorna
 * Parametros:
 *  bolillero -> numero de bolillero
 *  nombre -> nombre del equipo
 *  confederacion -> confederacion del equipo
 * Retorna -> un struct seleccionNacional
*/
seleccionNacional llenarEquipo(int bolillero,const char nombre[],const char confederacion[]){
  seleccionNacional equipo;
  equipo.numBolillero = bolillero;
  strcpy(equipo.nombreEquipo,nombre);
  strcpy(equipo.confederacion,confederacion);
  equipo.asignadoGrupo = false;
  return equipo;
}

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