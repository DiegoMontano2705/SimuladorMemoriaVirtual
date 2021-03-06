//
//  RealMemory.h
//  ProyectoFinal
//
//  Created by Rafa Mtz on 13/06/20.
//  Copyright © 2020 Rafa Mtz. All rights reserved.
//

#ifndef RealMemory_h
#define RealMemory_h

#include "Pagina.h"
#include "TablaPaginacion.h"
#include <vector>
#include <utility>

using namespace std;

class RealMemory{
private:
    pair<bool,Pagina> m[2048];
public:

    RealMemory(){};
    
    bool agregar(Pagina pagina, TablaPaginacion &tablaPaginacion){
        for (int i=0; i<2048; i++) {
            if (!m[i].first) {
                m[i].first = true;
                m[i].second = pagina;
                tablaPaginacion.AgregarRealMemory(pagina, i);
                return true;
            }
        }
        return false;
    }
    
    void borrar(Pagina pagina, TablaPaginacion &tablaPaginacion){
        m[tablaPaginacion.getPosicionRealMemory(pagina)].first = false;
    }

};

#endif /* RealMemory_h */