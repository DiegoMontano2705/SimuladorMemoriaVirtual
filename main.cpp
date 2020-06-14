/*
Instrucciones: El programa deberá simular un manejador de memoria virtual utilizando paginación parecidoal visto en clase, y medir su rendimiento. 
Se utilizarán las estrategias de reemplazo FIFO y LRU, para que comparen el rendimiento de ambas estrategias. 
Asuman que la computadora tiene un solo procesador; una memoria real de 2048 bytes, y que las páginas son de 16 bytes.

Integrantes: Diego Montaño, Jose Gonzalez, Eugenio Gonzalez, Rafael Martinez

*/


#include <iostream>
#include "LecturaArchivo.h"
//#include "Controller.h"

using namespace std;

int main(){

    string sNombreArchivo;
    LecturaArchivo Archivo;
    //Controller C;
    
    // Bienvenida del programa al usuario
    cout << "-------Simulador de Memoria Virtual-------" << endl;
    cout << "Favor de ingresar  nombre de archivo con instrucciones: " ;
    cin >> sNombreArchivo;

    if(!Archivo.VerificarExistenciaArchivo(sNombreArchivo)){
        cout << "ERROR ARCHIVO NO EXISTE" << endl;
        cout << "Favor de ingresar  nombre de archivo con instrucciones : " ;
        cin >> sNombreArchivo;
    }
    Archivo.LeerArchivo(sNombreArchivo);

    //C.setDatos(Archivo.getDatos());
    



    return 0;
}