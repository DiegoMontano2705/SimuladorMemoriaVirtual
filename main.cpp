/*
Instrucciones: El programa deberá simular un manejador de memoria virtual utilizando paginación parecidoal visto en clase, y medir su rendimiento. 
Se utilizarán las estrategias de reemplazo FIFO y LRU, para que comparen el rendimiento de ambas estrategias. 
Asuman que la computadora tiene un solo procesador; una memoria real de 2048 bytes, y que las páginas son de 16 bytes.

Integrantes: Diego Montaño, Jose Gonzalez, Eugenio Gonzalez, Rafael Martinez

*/


#include <iostream>
#include "LecturaArchivo.h"
<<<<<<< HEAD
#include "Controlador.h"
=======
//#include "Controller.h"

>>>>>>> de4bb4d3afdfa63da5236e2d343fc13e326b01d5
using namespace std;

int main(){

<<<<<<< HEAD
    /* Variables
    *
    * 
    */
   
   string sNombreArchivo;
   LecturaArchivo Datos;
   Controlador controlador;
    /* 
    * Code 
    */
=======
>>>>>>> de4bb4d3afdfa63da5236e2d343fc13e326b01d5

    string sNombreArchivo;
    LecturaArchivo Datos;
    //Controller C;
  
   cout << " ---Simulador de Manejador de Memoria Virtual--- " << endl;
   cout << endl;
   cout << "Ingresar nombre del archivo con los datos (Incluir extension) : ";
   cin >> sNombreArchivo;
   cout << "Verificando existencia del archivo ..." << endl;

    // Verificación del nombre del archivo
   while(!Datos.VerificarExistenciaArchivo(sNombreArchivo)){
       cout << "!ERROR! Archivo no existe o no fue encontrado" << endl;
       cout << "Ingresar nombre del archivo con los datos (Incluir extension) : ";
       cin >> sNombreArchivo;
       cout << "Verificando existencia del archivo ..." << endl;
   };

    cout << "Archivo encontrado" << endl;  
    Datos.LeerArchivo(sNombreArchivo);

    //C.setDatos(Archivo.getDatos());



    return 0;
}