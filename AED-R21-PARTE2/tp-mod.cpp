#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define CANTIDADGOLES 150
#ifdef _WIN32 //Solo incluir si se compila en windows
  #include <Windows.h> //Libreria para setear la consola en utf8 en windows
#endif

using namespace std;

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


registroGol* generarGoles(registroGol* &golPila, int cantidadGoles);
registroGol* insertar(registroGol*& lista, int idGol, int fecha, int codEquipo, char *nombreJugador, int idPartido);
int recorrerLista(registroGol*& listaGol);
bool grabarListaEnArchivo(registroGol*&, char*);
bool leerArchivo (registroGol*& listaGol, char* nombreArchivo);
void generarMatrizGoles(registroGol*& listaGol, matriz mPartidos);
void recorrerMatrizGoles (matriz mPartidos);

int main(){
  #ifdef _WIN32
    system ("color 0F" ); //Cambio el color de la consola
    SetConsoleOutputCP(CP_UTF8); //Cambia el code page a UTF-8 para evitar problemas con carácteres no ingleses (solo windows)
  #endif
  //Variables
	registroGol* listaGol; //Lista dinamica que carga el Archivo
  registroGol* listaGolArchivo;
	listaGol = NULL;
  listaGolArchivo = NULL;
   matriz mPartidos;
  generarGoles(listaGol, CANTIDADGOLES);
  grabarListaEnArchivo(listaGol, (char *)"lista.bin");
  leerArchivo(listaGolArchivo, (char *)"lista.bin");
  generarMatrizGoles(listaGolArchivo , mPartidos);
  recorrerMatrizGoles ( mPartidos );
  return 0;
}

void jugadorAleatorio(char jugador[]){
  char jugadoresStr[] = "Christian Stuani,Edinson Cavani,Maximiliano Gómez,Luis Suárez,Jonathan Urretaviscaya,Giorgian de Arrascaeta,Fakhreddine Ben Youssef,Bassem Srarfi,Naim Sliti,Saber Khalifa,Wahbi Khazri,Ahmed Khalil,Ghaylane Chaâleli,Seifeddine Khaoui,Breel Embolo,Haris Seferovic,Mario Gavranovic,Josip Drmic,Marcus Berg,John Guidetti,Isaac Kiese Thelin,Ola Toivonen,Aleksandar Mitrovic,Aleksandar Prijovic,Luka Jovic,Nemanja Radonjic,Keita Baldé,Mame Biram Diouf,Moussa Konate,Sadio Mané,Mbaye Niang,Diafra Sakho,Moussa Sow,Ismaila Sarr,Artem Dzyuba,Aleksey Miranchuk,Fedor Smolov,André Silva,Cristiano Ronaldo,Gelson Martins,Gonçalo Guedes,Ricardo Quaresma,Dawid Kownacki,Robert Lewandowski,Arkadiusz Milik,Lukasz Teodorczyk,Jefferson Farfán,Paolo Guerrero,Raúl Ruidíaz,André Carrillo,Edison Flores,Ismael Díaz,Blas Pérez,Luis Tejada,Gabriel Torres,Abdiel Arroyo,Ahmed Musa,Kelechi Iheanacho,Victor Moses,Odion Ighalo,Alex Iwobi,Nwankwo Simeon,Abdullahi Shehu,Tyronne Ebuehi,Elderson Echiejile,Brian Idowu,Chidozie Awaziem,William Ekong,Leon Balogun,Kenneth Omeruo,Marco Fabián,Raúl Jiménez,Oribe Peralta,Javier Hernández,Carlos Vela,Hirving Lozano,Khalid Boutaib,Aziz Bouhaddouz,Ayoub El Kaabi,Youssef En Nesyri,Shinji Okazaki,Yuya Osako,Yoshinori Muto,Alfred Finnbogason,Bjorn Bergmann Sigurdarson,Jon Dadi Bodvarsson,Alireza Jahanbakhsh,Ashkan Dejagah,Mehdi Taremi,Reza Ghoochannejhad,Saman Ghoddos,Sardar Azmoun,Karim Ansarifard,Jamie Vardy,Marcus Rashford,Raheem Sterling,Danny Welbeck,Harry Kane,Ousmane Dembélé,Olivier Giroud,Antoine Griezmann,Kylian Mbappé,Thomas Lemar,Nabil Fekir,Florian Thauvin,Iago Aspas,Rodrigo,Diego Costa,Lucas Vázquez,Marwan Mohsen,Mohamed Salah,Mahmoud Kahraba,Ramadan Sobhy,Mahmoud Shikabala,Amr Warda,Pione Sisto,Martin Braithwaite,Andreas Cornelius,Viktor Fischer,Yussuf Poulsen,Nicolai Jorgensen,Kasper Dolberg,Mario Mandzukic,Ivan Perisic,Nikola Kalinic,Andrej Kramarić,Marko Pjaca,Ante Rebic,Johan Venegas,Joel Campbell,Marco Ureña,Kim Shin-wook,Son Heung-min,Hwang Hee-chan,Carlos Bacca,Miguel Ángel Borja,Radamel Falcao García,Luis Fernando Muriel,José Heriberto Izquierdo,Neymar,Gabriel Jesus,Firmino,Douglas Costa,Taison,Michy Batshuayi,Romelu Lukaku,Eden Hazard,Dries Mertens,Adnan Januzaj,Tim Cahill,Tomi Juric,Mathew Leckie,Daniel Arzani,Andrew Nabbout,Robbie Kruse,Dimi Petratos,Jaime MacLaren,Leo Messi,Paulo Dybala,Sergio Agüero,Gonzalo Higuaín,Fahad Al-Muwallad,Mohammad Al-Sahlawi,Muhannad Assiri,Mario Gómez,Timo Werner,Marco Reus,";  
  char jugadores[167][50];
  char temp[50],chr;
  temp[0] = '\0';
  jugador[0] = '\0';
  //Aca llenamos un array de 167 jugadores para sortearlos
  for(int x=0,z=0;x<strlen(jugadoresStr);x++){
    chr = jugadoresStr[x];
    if(chr == ','){ //Delimitador
      strcpy(jugadores[z],temp);
      temp[0] = '\0';
      z++;
    }else{
      strncat(temp,&chr,1);
    }
  }
  strcpy(jugador,jugadores[rand() % 167]);
}

registroGol* generarGoles(registroGol* &listaGol, int cantidadGoles){ //TODO: Yo lo haria VOID, la puta madre Andrea
	registroGol* q=NULL;


  srand (time(NULL));
  cout<<"Cantidad Goles: "<<cantidadGoles<<endl;
  for(int x=0;x<cantidadGoles;x++){
    int codigoEquipo = rand() % 32+1;
    int idDePartido = rand() % 64+1;
    int fechaPartido = (idDePartido < 62 ? idDePartido/2 + 20180701 : 20180731 );
    char jugador[50];
    jugadorAleatorio(jugador);
	  registroGol* p=new registroGol();
	  insertar(listaGol,x+1,fechaPartido,codigoEquipo,&jugador[0],idDePartido);
    
	}
}
//Implementacion:
//pLista siempre va a ser el principio de la lista
//pNodo se usa para iterar
registroGol* insertar(registroGol *&listaGol, int idGol, int fecha, int codEquipo, char *nombreJugador, int idPartido){
  registroGol *pNodo;
  //Si la lista no esta vacia, declaro un nuevo Nodo
  registroGol* aux = new registroGol();
  aux -> idGol = idGol;
  aux -> codEquipo = codEquipo;
  strcpy(aux -> nombreJugador,nombreJugador); 
  aux -> fecha = fecha;  
  aux -> idPartido = idPartido;  
  aux -> siguiente = NULL;

  //Si la lista esta vacia
  if(listaGol == NULL){ 
    //¿Por que devuelvo esto?
    listaGol = aux;
    return aux;
  }

//Caso particular: Lista de un solo elemento. 
  if(listaGol -> codEquipo > codEquipo || (listaGol -> codEquipo == codEquipo && listaGol -> fecha > fecha)){
    aux -> siguiente = listaGol;
    listaGol = aux;
    return aux;
  }

  //Declaro pNodo que lo voy a usar para recorrer la lista hasta cierta condicion
  pNodo = listaGol;

  //Caso General, recorro.
  while(pNodo -> siguiente != NULL 
  && (pNodo -> siguiente -> codEquipo < codEquipo 
  || (pNodo -> siguiente -> codEquipo == codEquipo && pNodo -> siguiente -> fecha <= fecha))){
    pNodo = pNodo -> siguiente;
  }

  //Encontre donde tengo que insertarlo
  aux -> siguiente = pNodo -> siguiente;
  pNodo -> siguiente  = aux;
  return aux;
}

//Graba en el Archivo toda la lista
bool grabarListaEnArchivo (registroGol *&listaGol, char* nombreArchivo){
  registroGol *pNodo;
  pNodo = listaGol;
  FILE* f;
  f = fopen(nombreArchivo, "wb");

  //Caso extremo
  if (f == NULL){
    return false;
  }

  while (pNodo != NULL)
  {
    fwrite(pNodo,sizeof(registroGol),1,f);
    if (ferror(f))
    { 
      return false;
    }
    pNodo = pNodo->siguiente;
  }

  fclose(f);
  return true;
}

//Lee el Archivo toda la lista, en memoria
bool leerArchivo (registroGol *&listaGol, char* nombreArchivo){
  registroGol pNodo;
  //pNodo = listaGol;
  FILE* f;
  f = fopen(nombreArchivo, "rb");

  //Caso extremo
  if (f == NULL){
    return false;
  }
  int registros;
  do{
    registros = fread(&pNodo,sizeof(registroGol),1,f);
    if(registros == 0){
      break;
    }
    insertar(listaGol, pNodo.idGol, pNodo.fecha, pNodo.codEquipo, pNodo.nombreJugador, pNodo.idPartido);
    if(ferror(f)){ 
      return false;
    }
  }while(1);

  fclose(f);
  return true;
}

int recorrerLista(registroGol*& listaGol){
  registroGol *pNodo = listaGol; 
  int cant = 0;
  while(pNodo != NULL){
    cout << "IDGOL: " << pNodo -> idGol << " EQUIPO: " << pNodo -> codEquipo << " FECHA: " << pNodo -> fecha << " JUGADOR: " << pNodo -> nombreJugador << " IDPARTIDO: " << pNodo -> idPartido << endl;
    pNodo = pNodo -> siguiente;
    cant++;
  }  
  return cant;
}

//DEBUG
int recorrer(nodoLista*& nl){
  nodoLista *pNodo = nl; 
  int cant = 0;
  if(nl == NULL){
    cout << "Me pasaste un NULL" << endl;
  }
  while(pNodo != NULL){
    cout << "Fecha: " << pNodo -> fecha << " Nomjugador: " << pNodo -> nombreJugador << " Goles: " << pNodo -> goles << endl;
    pNodo = pNodo -> siguiente;
    cant++;
  }  
  return cant;
}

void generarMatrizGoles(registroGol*& listaGol, matriz mPartidos){
  int x;
  nodoLista* auxNL = NULL;
  registroGol* auxRG = NULL;
  for(int x=0 ; x<32; x++){
    for(int y=0 ; y<7 ; y++){
      mPartidos[x][y] = NULL ;
    }
  }

  bool nuevoNodo;
  while ( listaGol != NULL ){
    cout << "IDGOL: " << listaGol -> idGol << " EQUIPO: " << listaGol -> codEquipo << " FECHA: " << listaGol -> fecha << " JUGADOR: " << listaGol -> nombreJugador << " IDPARTIDO: " << listaGol -> idPartido << endl;
    //Si la primera columna esta vacia (NULL) insertar ahi
    if(mPartidos[listaGol -> codEquipo -1][x] == NULL){
      auxNL = new nodoLista();
      strcpy(auxNL->nombreJugador, listaGol-> nombreJugador);
      auxNL->fecha = listaGol -> fecha; 
      auxNL->goles=1;
      auxNL->siguiente=NULL;
      mPartidos[listaGol -> codEquipo -1][x] = auxNL; //Y lo inserto en la columna
      listaGol = listaGol -> siguiente; //Avanzo al siguiente registro
      x=0; //Reseteo el contador de columnas
      cout << ">>> Lista vacia inserto nodo " << x << endl; 
      continue; //Repetir el LOOP
    }

    //Si el partido existentente en la columna no coincide con el leido avanzar a la siguiente columna
    if(mPartidos[listaGol -> codEquipo -1][x] -> fecha != listaGol -> fecha){
      x++; //Aumento en 1 el contador de columnas para buscar la siguiente
      cout << ">>> Avanzo a la columna " << x << endl; 
      continue; //Repetir el LOOP
    }

    //Si llego aca es que encontro un segundo registro del mismo partido, hay que determinar si el gol es del mismo jugador o hay que agregar un nodo
    auxNL = mPartidos[listaGol -> codEquipo -1][x];
    nuevoNodo = true; //Si esta bandera queda en true, se suma gol y no se añade nodo
    while(auxNL != NULL){ //Recorro la lista
      if(strcmp(auxNL -> nombreJugador,listaGol -> nombreJugador) == 0){ //Si el jugador es el mismo sumar los goles
        cout << "SUME GOL " << listaGol -> nombreJugador << endl;  
        nuevoNodo = false;            
        auxNL -> goles++;
        break;
      }
      auxNL = auxNL -> siguiente;
    }
    if(nuevoNodo){ // encontro un segundo registro del mismo partido y el jugador es distinto, agregar nuevo nodo
      //Si no encontre el jugador en la lista de goles del partido insertar un nuevo nodo
      auxNL = new nodoLista();
      strcpy(auxNL->nombreJugador, listaGol-> nombreJugador);
      auxNL->fecha = listaGol -> fecha; 
      auxNL->goles=1;
      auxNL->siguiente=NULL;
      cout << "MISMO PARTIDO - FECHA: " << auxNL -> fecha << " JUG: " << auxNL -> nombreJugador << endl;
    }
    //Si llego aca es que encontro un segundo registro del mismo partido y el jugador es distinto, agregar nuevo nodo
    listaGol = listaGol -> siguiente; //Avanzo al siguiente registro
    x=0; //Reseteo el contador de columnas
  }
  cout << "Termine " << endl;
}




void recorrerMatrizGoles (matriz mPartidos){
  nodoLista* auxNL = NULL;
  int cantGoles,z;
  for(int x=0; x<32; x++){
    cout << "Equipo " << x+1 << endl;
    cantGoles=0;
    for(int y=0; y<7; y++){
      auxNL = mPartidos[x][y];
      if(auxNL == NULL){
        continue;
      }
      cout << "\tPartido " << y+1 << " " << mPartidos[x][y] -> fecha << endl;
      z=1;
      while(auxNL != NULL){
        cantGoles += auxNL->goles;
        cout << "\t\tGol " << z << " > " << auxNL -> fecha << auxNL -> nombreJugador << endl << endl;
        auxNL = auxNL -> siguiente;
      }
    }
  //  cout << "Cantidad de goles del Equipo " << x+1 <<": "<< cantGoles << endl;
  }
}