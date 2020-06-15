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
public:
    RealMemory :: RealMemory(){};
    
    bool RealMemory :: insert(Pagina pagina, TablaPaginacion &tablaPaginacion){
        for (int iCont=0; iCont<2048; i++) {
            if (!m[iCont].first) {
                m[iCont].first = true;
                m[iCont].second = pagina;
                tablaPaginacion.AgregarRealMemory(pagina, iCont);
                return true;
            }
        }
        return false;
    }
    
    void RealMemory :: erase(Pagina pagina, TablaPaginacion &tablaPaginacion){
        m[tablaPaginacion.getPosicionRealMemory(pagina)].first = false;
    }
    
    RealMemory();
    bool insert(Pagina pagina, TablaPaginacion &tablaPaginacion);
    void erase(Pagina pagina, TablaPaginacion &tablaPaginacion);
    
private:
    pair<bool,Pagina> m[2048];
};

#endif /* RealMemory_h */