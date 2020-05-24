#ifndef LECTURAARCHIVO_H
#define LECTURAARCHIVO_H
#include <iostream>
#include <fstream>
using namespace std;

class LecturaArchivo{
    private:
        string sNombreArchivo;

    public:
        // Constructor
        inline LecturaArchivo(){
        }

        // Getters y Setters

        inline void setNombreArchivo(string sNA){
            sNombreArchivo = sNA;
        }
        inline string getNombreArchivo(){
            return sNombreArchivo;
        }


        /* Método CargarDatos
        *  Leer los datos del archivo recibido y guarda los valores correspondientes
        *  Input: 
        *  Output:
        */ 
        inline void CargarDatos(){

            ifstream ifArchivo(sNombreArchivo);
            while(!ifArchivo.eof()){
                
            }
        }

        /* Método VerificarExistenciaArchivo
        *  Verifica que exista archivo de texto con el nombre dado y regresa un valor booleano
        *  dependiendo del resultado
        *  Input: String sNombreArch
        *  Output: bool (false = No existe archivo, true = Existe el archivo)
        */ 
        inline bool VerificarExistenciaArchivo(string sNombreArch){
            ifstream ifArchivo(sNombreArch);
            if(ifArchivo.fail()){
                return false;
            } else {
                return true;
            }
        }


};


#endif //   LECTURAARCHIVO_H