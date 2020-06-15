//
//  VirtualMemory.h
//  ProyectoFinal
//
//  Created by Rafa Mtz on 13/06/20.
//  Copyright © 2020 Rafa Mtz. All rights reserved.
//

#ifndef VirtualMemory_h
#define VirtualMemory_h

#include "Page.h"
#include <vector>
#include <utility>
#include "TablaPaginacion.h"

using namespace std;

class VirtualMemory{
public:
    VirtualMemory :: VirtualMemory(){}
    
    void VirtualMemory :: insert(Pagina pagina, TablaPaginacion &tablaPaginacion){
        for (int iCont=0; iCont<4096; iCont++) {
            if (!m[iCont].first) {
                m[iCont].first = true;
                m[iCont].second = pagina;
                tablaPaginacion.AgregarVirtualMemory(pagina,iCont);
                break;
            }
        }
    }
    
    void VirtualMemory :: erase(Pagina pagina, TablaPaginacion &tablaPaginacion){
        m[tablaPaginacion.getPosicionVirtualMemory(page)].first = false;
    }
    
    VirtualMemory();
    void insert();
    void erase();
    
private:
    pair<bool,Pagina> m[4096];
};

#endif /* VirtualMemory_h */
