//
//  VirtualMemory.h
//  ProyectoFinal
//
//  Created by Rafa Mtz on 13/06/20.
//  Copyright © 2020 Rafa Mtz. All rights reserved.
//

#ifndef VirtualMemory_h
#define VirtualMemory_h

#include "Pagina.h"
#include <vector>
#include <utility>
#include "TablaPaginacion.h"

using namespace std;

class VirtualMemory{
private:
    pair<bool,Pagina> m[4096];
    
public:

    VirtualMemory(){}
    
    void agregar(Pagina pagina, TablaPaginacion &tablaPaginacion){
        for (int i=0; i<4096; i++) {
            if (!m[i].first) {
                m[i].first = true;
                m[i].second = pagina;
                tablaPaginacion.AgregarVirtualMemory(pagina,i);
                break;
            }
        }
    }
    
    void borrar(Pagina pagina, TablaPaginacion &tablaPaginacion){
        m[tablaPaginacion.getPosicionVirtualMemory(pagina)].first = false;
    }
    
};

#endif /* VirtualMemory_h */
