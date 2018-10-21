
#include <cstdlib>
#include <iostream>
#include <string.h>
using namespace std;


#define CANTIDADDATOS 20

// para prueba de estructura
struct tRegArbol{
    int cantidadGoles;
    char equipo[60];
}; 

// nodo
template <typename TS> 
struct pNodo {
   TS *data;
   pNodo<TS> *pIzq ; 
   pNodo<TS> *pDer  ; 
   int value;
} ;

template <typename TS> 
struct TDAArbol{   

  pNodo<TS> *raiz;   // es global a la estructura, entonces se utiliza en los métodos asociados al TDA Pilas

  bool vacio(){return raiz == NULL ;} ;

  void eliminarArbol(TS *d)
  {
    if(d!=NULL){
        destroy_tree(d->pIzq);
        destroy_tree(d->right);
        delete d;
    }
  }
 void insertar(pNodo<TS> *nodo, TS *d, int value)
 {
    cout<<"Insertar(): Entrando en la funcion."<<endl;
    cout<<"Insertar(): Nodo Actual: "<<nodo<<endl;
    cout<<"Insertar(): Dato a insertar: "<<d<<endl;
    cout<<"Insertar(): Valor de Raiz: "<<raiz<<endl;
    if(raiz == NULL)
    {
      cout<<"Insertar(): El nodo donde insertar es null"<<endl;
      raiz=new pNodo<TS>;
      raiz->value=value;
      raiz->data = d;
      raiz->pIzq=NULL;    
      raiz->pDer=NULL;
    }
    else{

    
    if(value < nodo->value)
    {
        cout<<"Insertar(): value es menor"<<endl;
    if(nodo->pIzq!=NULL)
     insertar(nodo->pIzq, d, value);
    else
    {
      nodo->pIzq=new pNodo<TS>;
      nodo->pIzq->value=value;
      nodo->pIzq->data = d;
      nodo->pIzq->pIzq=NULL;    
      nodo->pIzq->pDer=NULL;   
    }  
  }
  else if(value>=nodo->value)
  {
      cout<<"Insertar(): value es mayor"<<endl;
    if(nodo->pDer!=NULL)
      insertar(nodo->pDer, d, value);
    else
    {
      nodo->pDer=new pNodo<TS>;
      nodo->pDer->value=value;
      nodo->data = d;
      nodo->pDer->pIzq=NULL;  
      nodo->pDer->pDer=NULL; 
    }
  }
    }
  cout<<"Insertar(): Fin de la funcion"<<endl;
 }

 void insertar_dato(char *equipo, int cantidadGoles)
 {
     TS *dato = new TS();
     strcpy(dato->equipo,  equipo);
     dato->cantidadGoles = cantidadGoles;
     cout<<"insertar_dato(): Insertando dato en Nodo("<<equipo<<", "<<cantidadGoles<<")"<<endl;
    insertar (raiz, dato, cantidadGoles);
    //delete dato; Causa leaks?
 }
    // constructor, se ejecuta cada vez que se crea una variable
    TDAArbol() {raiz = NULL;}; 

} ;

void cargarDatos (TDAArbol <tRegArbol> &arbol);

int main(void)
{
    tRegArbol pDato;
    TDAArbol <tRegArbol> MiArbol;
    cargarDatos(MiArbol);
    return EXIT_SUCCESS;
}

void cargarDatos (TDAArbol <tRegArbol> &arbol)
{ 
    tRegArbol pDato;
    //string nombre[10] = {"Pepe","Juan","Pedro", "Maria", "Josefa", "Martina", "Loki", "Bus", "Gato", "red"};
    string ciudad[10] = {"BUE","NQN","UAQ", "MDQ", "FTE", "RGA", "COR", "REL", "BRC", "JUJ"};
    cout<<"Cargando Datos"<<endl;
    char ciu [60];
    int goles;
    for (int i=0; i<CANTIDADDATOS;i++)
    {
        strcpy (ciu,ciudad[rand()%10].c_str());
        goles = rand()%5;
        cout<<"Equi: "<<ciu<<" Goles: "<<goles<<endl;
        arbol.insertar_dato (ciu, goles);

    }
}

