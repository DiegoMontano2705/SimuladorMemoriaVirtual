//
//  VirtualMemory.h
//  ProyectoFinal
//
//  Created by Rafa Mtz on 13/06/20.
//  Copyright © 2020 Rafa Mtz. All rights reserved.
//

#ifndef VirtualMemory_h
#define VirtualMemory_h

using namespace std;

class VirtualMemory{
public:
    void insert();
    void erase();
    
private:
    int memoria[4096];
}

#endif /* VirtualMemory_h */
