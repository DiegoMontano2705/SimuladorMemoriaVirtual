//
//  RealMemory.h
//  ProyectoFinal
//
//  Created by Rafa Mtz on 13/06/20.
//  Copyright © 2020 Rafa Mtz. All rights reserved.
//

#ifndef RealMemory_h
#define RealMemory_h

using namespace std;

class RealMemory{
public:
    void insert();
    void check();
    void erase();
    
private:
    int memoria[2048];
};



#endif /* RealMemory_h */
