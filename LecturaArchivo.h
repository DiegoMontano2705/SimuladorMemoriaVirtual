#ifndef LECTURAARCHIVO_H
#define LECTURAARCHIVO_H
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class LecturaArchivo{
    private:
    vector<string> vDatos;
    string sLinea;
    public:
        inline LecturaArchivo(){
        }

        inline vector<string> getDatos(){
            return vDatos;
        }
  
        //Lee datos del archivo de texto y los guarda en un vector
        inline void LeerArchivo(string sArchivo){
            ifstream ifArchivo;
            ifArchivo.open(sArchivo);
            while(!ifArchivo.eof()){
                getline(ifArchivo,sLinea);
                vDatos.push_back(sLinea);
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