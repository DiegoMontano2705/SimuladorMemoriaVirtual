#ifndef TABLAPAGINACION_H
#define TABLAPAGINACION_H
#include "Pagina.h"
#include <unordered_map>
using namespace std;


class TablaPaginacion{

    private:
        //True= realMemory, False= VirtualMemory
        unordered_map<int, unordered_map<int, pair<bool, int>>> tabla;
    public:

    TablaPaginacion(){
        // Mapa en el que se almacena id, en que memoria está y la posicion 
        tabla = unordered_map<int, unordered_map<int, pair<bool, int>>>();
    }

    void AgregarRealMemory(Pagina pagina, int posicion){
        tabla[pagina.get_id()][pagina.get_numPagina()] = {true, posicion};
    }

    int getPosicionRealMemory(Pagina pagina){
        return tabla[pagina.get_id()][pagina.get_numPagina()].second;
    }

    void AgregarVirtualMemory(Pagina pagina, int posicion){
        tabla[pagina.get_id()][pagina.get_numPagina()]={false,posicion};
    }

    int getPosicionVirtualMemory(Pagina pagina){
        return tabla[pagina.get_id()][pagina.get_numPagina()].second;
    }

    bool WhereAmI(Pagina pagina){
        //True: real memory / false: VirtualMemory
        return tabla[pagina.get_id()][pagina.get_numPagina()].first;
    }

    bool eliminarProceso(int id){
        return tabla.erase(id);
    }


};

#endif