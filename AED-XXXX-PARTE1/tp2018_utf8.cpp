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

struct seleccionesAgrupadas{
  char nombreEquipo[30];
  char confederacion[20];
  char grupo;
};

/* PROTOTIPOS */
bool escribirArchivo(const char[],seleccionNacional[],int);
bool escribirArchivo(char[],seleccionNacional[],int);
bool llenarEquipos(seleccionNacional[]);
seleccionNacional llenarEquipo(int,const char[],const char[]);
bool sortearEquipos(grupoMundial[],seleccionNacional[]);
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

  if(!llenarEquipos(selNacionales)){ //Defino los equipos
    return 1;
  }
  if(!sortearEquipos(gruposSorteo,selNacionales)){ //Sorteo los grupos
    return 1;
  }
  if(!leerGrupos(selAgrupadas)){ //Leo los archivos generados
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
    return opcion;
  }while(true);
}

/* Genera los 32 equipos
  Parametros:
    selNacionales -> array de structs seleccionNacional a generar con los 32 equipos del mundial
  Retorna -> true si la operacin fue correcta, false si hubo un error al generar el archivo binario con los equipos
*/
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
  }
  cout << "32 equipos generados, archivo selecciones.bin generado" << endl;
  return true;
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

/* Asigna los 32 equipos a los 8 grupos usando las reglas dadas
  Parametros:
    grupos -> array de struct grupoMundial donde se asignara 4 equipos a cada uno
    selecciones -> array de struct seleccionNacional con los 32 equipos a sortear
  Retorna -> true si la operacin fue correcta, false si hubo un error al generar los archivos binarios de los grupos
*/
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

/* Lee los archivos binarios con los grupos (grupo_A.bin, grupo_B.bin...) y los mete en un array de seleccionesAgrupadas
 * Parametros:
 *  selAgrupadas -> Array de structs seleccionesAgrupadas a llenar
 * Retorna -> true si se pudo leer todos los archivos, false en caso contrario
*/
bool leerGrupos(seleccionesAgrupadas selAgrupadas[]){
  char letras[] = "ABCDEFGH";
  int x,y;
  char nombreArchivo[8];
  strcpy(nombreArchivo,"grupo_x"); //La x sera sustituida por la letra del grupo

  seleccionNacional selNacional[4]; //Array temporal de seleccionNacional para guardar la lectura de archivos
  for(x=0;x<8;x++){
    nombreArchivo[6] = letras[x];
    if(!leerArchivo(nombreArchivo,selNacional,4)){ //Leo el archivo y le paso el struct temporal de 4 equipos
      cout << "Hubo un error al procesar el archivo binario de uno del grupo " << letras[x];
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
  int x,y,z;
  bool cambio; //Indica que se hizo un cambio
  seleccionesAgrupadas tmp; //Temporal para hacer el intercambio
  switch(modo){
    case 1: //letra grupo
      do{ //Se repite hasta que no haya mas cambios
        cambio = false; //se resetea bandera
        for(x=0;x<31;x++){
          for(y=x;y<32;y++){
            if(selAgrupadas[y].grupo < selAgrupadas[x].grupo){ //Comparo registros
              tmp = selAgrupadas[y];
              selAgrupadas[y] = selAgrupadas[x];
              selAgrupadas[x] = tmp;
              cambio = true; //Si hubo un cambio se marca la bandera para hacer un ciclo mas
            }
          }
        }
      }while(cambio);
    break;
    case 2: //nombre equipo
      do{
        cambio = false;
        for(x=0;x<31;x++){
          for(y=x;y<32;y++){
            if(strcmp(selAgrupadas[y].nombreEquipo,selAgrupadas[x].nombreEquipo) < 0){
              tmp = selAgrupadas[y];
              selAgrupadas[y] = selAgrupadas[x];
              selAgrupadas[x] = tmp;
              cambio = true;
            }
          }
        }
      }while(cambio);
    break;
    case 3: //confederacion
      do{
        cambio = false;
        for(x=0;x<31;x++){
          for(y=x;y<32;y++){
            if(strcmp(selAgrupadas[y].confederacion,selAgrupadas[x].confederacion) < 0){
              tmp = selAgrupadas[y];
              selAgrupadas[y] = selAgrupadas[x];
              selAgrupadas[x] = tmp;
              cambio = true;
            }
          }
        }
      }while(cambio);
    break;
    case 4: //grupo y confederacion
      do{
        cambio = false;
        for(x=0;x<31;x++){
          for(y=x;y<32;y++){
            if(selAgrupadas[y].grupo < selAgrupadas[x].grupo){ //Primero comparo el grupo
              tmp = selAgrupadas[y];
              selAgrupadas[y] = selAgrupadas[x];
              selAgrupadas[x] = tmp;
              cambio = true;
            }
            if(selAgrupadas[y].grupo == selAgrupadas[x].grupo){ //Si los grupos son iguales comparo la confederacion
              if(strcmp(selAgrupadas[y].confederacion,selAgrupadas[x].confederacion) < 0){
                tmp = selAgrupadas[y];
                selAgrupadas[y] = selAgrupadas[x];
                selAgrupadas[x] = tmp;
                cambio = true;
              }
            }
          }
        }
      }while(cambio);
    break;
    case 5: //grupo, confederacion y nombre
      do{
        cambio = false;
        for(x=0;x<31;x++){
          for(y=x;y<32;y++){
            if(selAgrupadas[y].grupo < selAgrupadas[x].grupo){ //Primero comparo el grupo
              tmp = selAgrupadas[y];
              selAgrupadas[y] = selAgrupadas[x];
              selAgrupadas[x] = tmp;
              cambio = true;
            }
            if(selAgrupadas[y].grupo == selAgrupadas[x].grupo){ //Si los grupos son iguales comparo la confederacion
              if(strcmp(selAgrupadas[y].confederacion,selAgrupadas[x].confederacion) < 0){
                tmp = selAgrupadas[y];
                selAgrupadas[y] = selAgrupadas[x];
                selAgrupadas[x] = tmp;
                cambio = true;
              }
              if(strcmp(selAgrupadas[y].confederacion,selAgrupadas[x].confederacion) == 0){ //Si el grupo y confederacion son iguales comparo los nombres de equipo
                if(strcmp(selAgrupadas[y].nombreEquipo,selAgrupadas[x].nombreEquipo) < 0){
                  tmp = selAgrupadas[y];
                  selAgrupadas[y] = selAgrupadas[x];
                  selAgrupadas[x] = tmp;
                  cambio = true;
                }
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
  cin.clear();
  cin.ignore(INT_MAX, '\n');
  cin.get();
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

/* Lee un archivo binario
 * Parametros:
 *  nombreArchivo -> Nombre del archivo que se escribira
 *  lecturaEquipos -> Array de structs seleccionNacional donde se volcaran los datos
 *  cant -> Cantidad de registros a leer
 * Retorna -> true si la lectura fue exitosa, false en caso contrario
 */
bool leerArchivo(char nombreArchivo[],seleccionNacional lecturaEquipos[],int cant){
  char nombreArchivoExt[15]; //Nombre de archivo mas extension
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
  return true;
}
