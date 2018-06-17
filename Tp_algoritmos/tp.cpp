#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <time.h>  

using namespace std;

struct seleccionNacional{
  int numBolillero;
  char nombreEquipo[30];
  char confederacion[20];
  bool asignadoGrupo;
};

struct grupoMundial{
  char identificador;
  seleccionNacional equipos[4]; 
};

bool escribirArchivo(seleccionNacional[]);
seleccionNacional llenarEquipo(int,char[],char[]);
void llenarEquipos(seleccionNacional[]);
void sortearEquipos(grupoMundial[],seleccionNacional[]);

int main(){
  int seleccionUsuario;
  seleccionNacional selNacionales[32];
  grupoMundial gruposSorteo[8];

  llenarEquipos(selNacionales);
  escribirArchivo(selNacionales);
  sortearEquipos(gruposSorteo,selNacionales);

  // while (seleccionUsuario!=2){
  //   cout << "Sorteo Mundial Rusia 2018"<< endl << "  1. Sortear Grupos" << endl << "  2. Salir" << endl;
  //   cin >> seleccionUsuario;
  //   switch (seleccionUsuario){
  //     case 1:
  //       cout << "Estoy sorteando" << endl;
  //       //sortearEquipos(selNacionales);
  //     break;
  //     case 2:
  //        cout << "Saliendo..." << endl;
  //     break;
  //     default:
  //     cout << "ERROR: Ingrese un valor valido" << endl;
  //     break;
  //   }
  // }
  // return 0;
}
/*
struct grupoMundial{
  char identificador;
  seleccionNacional equipos[4]; 
};
*/
void sortearEquipos(grupoMundial grupos[],seleccionNacional selecciones[]){
  srand (time(NULL)); // Inicializo mi función random con una seed del timestamp actual
  char letras[] = "ABCDEFGH";
  for(int x=0;x<8;x++){
    grupos[x].identificador = letras[x];
  }
  selecciones[0].asignadoGrupo = true;
  grupos[0].equipos[0] = selecciones[0]; // Asigno a Rusia al grupo A
  
  int equiposPendientesAsignar = 7;
  int numEquipo;

  //Cabeza de serie -- PASO 1
  for(int x=1;x<=7;x++){
    do{ //Esto se repite hasta que deje de encontrar un equipo ya asignado a grupo
      numEquipo = rand() % 7 + 1;
    }while(selecciones[numEquipo].asignadoGrupo);
    grupos[x].equipos[0] = selecciones[numEquipo];
    selecciones[numEquipo].asignadoGrupo = true;
  }
  
  //Bolillero 2 -- PASO 2
  for(int x=0;x<=7;x++){
    do{ //Esto se repite hasta que deje de encontrar un equipo ya asignado a grupo
      numEquipo = rand() % 8 + 8;
      if(!selecciones[numEquipo].asignadoGrupo){
        //Evaluo si un equipo comnebol y el equipo cabeza de serie es argentina o brasil
        if(strcmp(selecciones[numEquipo].confederacion,"Conmebol") == 0 && 
          (strcmp(grupos[x].equipos[0].nombreEquipo,"Argentina") == 0 ||
          strcmp(grupos[x].equipos[0].nombreEquipo,"Brasil") == 0))
        {
          //En este caso repetidos paso 2 
          for(int y=8;y<=15;y++){
            selecciones[y].asignadoGrupo = false;
            x=0;
          }
          continue; //Vuelvo a sacar un random
        }  
        break; //Rompro el loop y asigno el equipo
      }
    }while(true);
    grupos[x].equipos[1] = selecciones[numEquipo];
    selecciones[numEquipo].asignadoGrupo = true;
  }  

  int repetido = 0;
  //Bolillero 3 -- PASO 3
  for(int x=0;x<=7;x++){
    do{ //Esto se repite hasta que deje de encontrar un equipo ya asignado a grupo
      numEquipo = rand() % 8 + 16;
      if(!selecciones[numEquipo].asignadoGrupo){
        repetido = 0;
        //Evaluo si un equipo comnebol y el equipo cabeza de serie es argentina o brasil
        if(strcmp(grupos[x].equipos[0].confederacion,selecciones[numEquipo].confederacion) == 0){
          repetido++;
        }
        if(strcmp(grupos[x].equipos[1].confederacion,selecciones[numEquipo].confederacion) == 0){
          repetido++;
        }                      
        if((strcmp(selecciones[numEquipo].confederacion,"UEFA") != 0 && repetido > 0) ||
          (strcmp(selecciones[numEquipo].confederacion,"UEFA") == 0 && repetido > 1)){
          //En este caso repetidos paso 2 
          for(int y=16;y<=23;y++){
            selecciones[y].asignadoGrupo = false;
            x=0;
          }
          continue; //Vuelvo a sacar un random
        }       
        break; //Rompro el loop y asigno el equipo
      }
    }while(true);
    grupos[x].equipos[2] = selecciones[numEquipo];
    selecciones[numEquipo].asignadoGrupo = true;
  } 

  //Bolillero 4 -- PASO 4
  for(int x=0;x<=7;x++){
    do{ //Esto se repite hasta que deje de encontrar un equipo ya asignado a grupo
      numEquipo = rand() % 8 + 24;
      if(!selecciones[numEquipo].asignadoGrupo){
        repetido = 0;
        //Evaluo si un equipo comnebol y el equipo cabeza de serie es argentina o brasil
        if(strcmp(grupos[x].equipos[0].confederacion,selecciones[numEquipo].confederacion) == 0){
          repetido++;
        }
        if(strcmp(grupos[x].equipos[1].confederacion,selecciones[numEquipo].confederacion) == 0){
          repetido++;
        }        
        if(strcmp(grupos[x].equipos[2].confederacion,selecciones[numEquipo].confederacion) == 0){
          repetido++;
        }                
        if((strcmp(selecciones[numEquipo].confederacion,"UEFA") != 0 && repetido > 0) ||
          (strcmp(selecciones[numEquipo].confederacion,"UEFA") == 0 && repetido > 1))
        {
          //En este caso repetidos paso 2 
          for(int y=24;y<=31;y++){
            selecciones[y].asignadoGrupo = false;
            x=0;
          }
          continue; //Vuelvo a sacar un random
        }   
        break; //Rompro el loop y asigno el equipo
      }
    }while(true);
    grupos[x].equipos[3] = selecciones[numEquipo];
    selecciones[numEquipo].asignadoGrupo = true;
  }

  for(int z=0;z<8;z++){
    for(int w=0;w<4;w++){
      cout << "Grupo " << grupos[z].identificador << "\tEquipo " << w+1 << " " << grupos[z].equipos[w].nombreEquipo << endl;
    }
  }     
}





void llenarEquipos(seleccionNacional selNacionales[]){
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
}

seleccionNacional llenarEquipo(int bolilla,char nombre[],char confederacion[]){
  seleccionNacional equipo;
  equipo.numBolillero = bolilla;
  strcpy(equipo.nombreEquipo,nombre);
  strcpy(equipo.confederacion,confederacion);
  equipo.asignadoGrupo = false;
  return equipo;
}

bool escribirArchivo(seleccionNacional selNacionales[]){
  FILE *archivo; //Defino el archivo
  archivo = fopen("bolillero.bin","wb"); //Lo abro en modo escritura binaria
  fwrite(&selNacionales, sizeof(struct seleccionNacional), 32, archivo); //Grabo el struct en el archivo
  //sizeof(struct seleccionnacional)  --> mide el tamaño del struct para grabarlo en el archibo
  fclose(archivo);
  return true;
}