#ifndef CONTROLADOR_H
#define CONTROLADOR_H
#include <iostream>
#include <vector>
#include <map>
#include "RealMemory.h"
#include "VirtualMemory.h"
#include "Status.h"
#include "Proceso.h"
#include "TablaPaginacion.h"
#include "Queue.h" 
#include <math.h>
using namespace std;

class Controlador{

    private:
        vector<string> vDatos;
        // Variables FIFO
        RealMemory RM_FIFO;
        VirtualMemory VM_FIFO;
        TablaPaginacion TP_FIFO;
        map<int, Proceso> historiaProceso_FIFO;
        int totalSwaps_FIFO;
        double currentTime_FIFO;
        Queue fifo;
        // Variables LRU
        RealMemory RM_LRU;
        VirtualMemory VM_LRU;
        TablaPaginacion TP_LRU;
        map<int, Proceso> historiaProceso_LRU;
        int totalSwaps_LRU;
        double currentTime_LRU;
        Queue lru;
        double d_load_page_memory_initially = 1.0, d_swap = 1.0, d_access_page_in_real_memory = 0.1,
        d_free_page_real_memory = 0.1, d_free_page_secondary_memory = 0.1;
        int page_size = 16;

    public:
        
        Controlador() {
            this->RM_FIFO = RealMemory();
            this->VM_FIFO = VirtualMemory();
            this->TP_FIFO = TablaPaginacion();
            this->historiaProceso_FIFO = map<int, Proceso>();
            this->currentTime_FIFO = 0.0;
            this->totalSwaps_FIFO = 0;
            this->fifo = Queue();
            this->RM_LRU = RealMemory();
            this->VM_LRU = VirtualMemory();
            this->TP_LRU = TablaPaginacion();
            this->historiaProceso_LRU = map<int, Proceso>();
            this->currentTime_LRU = 0.0;
            this->totalSwaps_LRU = 0;
            this->lru = Queue();
            
        }

        void setDatos(vector<string> vDatos){
            this->vDatos = vDatos;
        }


        void ResetData(){
            RM_FIFO = RealMemory();
            VM_FIFO = VirtualMemory();
            TP_FIFO = TablaPaginacion();
            historiaProceso_FIFO = map<int, Proceso>();
            currentTime_FIFO = 0.0;
            totalSwaps_FIFO = 0;
            fifo.clear();
            RM_LRU = RealMemory();
            VM_LRU = VirtualMemory();
            TP_LRU = TablaPaginacion();
            historiaProceso_LRU = map<int, Proceso>();
            currentTime_LRU = 0.0;
            totalSwaps_LRU = 0;
            lru.clear();
        }

    

        void IniciarSimulador(){
            string sIntruccion;
            for(int i = 0; i < vDatos.size(); i++){
                sIntruccion = vDatos[i];
                Instruccion(sIntruccion[0],i);
            }        
        }

        vector<int> leerInstruccion(string s, int params){
            vector<int> parametros;
            string subs;
            string temp;
            int iPos = 0;
            int ePos = 0;
            
            subs = s.substr(1);
            
            for(int i = 0; i<params; i++){
                // Primer digito
                iPos = subs.find_first_not_of(' ');
                // Quita espacios
                subs = subs.substr(iPos);
                // Final del numero
                ePos = subs.find_first_of(' ');
                // Guardar substring
                //cout << "Check" << endl;
                temp = subs.substr(0,ePos);
                // Meter a parametros
                parametros.push_back(stoi(temp));
                // Quita numero
                if(ePos != -1){
                    subs = subs.substr(ePos);
                }
            }
            
            return parametros;
        }

        Pagina swap(int p, bool isFifo) {
    Pagina swapPag;
    if(isFifo){
        // remove page from real memory that doesn't belong to the current process
        swapPag = fifo.front(p);
        // add such page to secondary memory
        RM_FIFO.borrar(swapPag, TP_FIFO);
        VM_FIFO.agregar(swapPag, TP_FIFO);
        // add to current time the time it takes to do a swap
        currentTime_FIFO += d_swap;
        
        // add the swap to the total swaps
        totalSwaps_FIFO++;
    } else {
        // remove page from real memory that doesn't belong to the current process
        swapPag = lru.front(p);
        // add such page to secondary memory
        RM_FIFO.borrar(swapPag,TP_LRU);
        VM_FIFO.agregar(swapPag,TP_LRU);
        // add to current time the time it takes to do a swap
        currentTime_LRU += d_swap;
        
        // add the swap to the total swaps
        totalSwaps_LRU++;
    }
    return swapPag;
}

pair<int, pair<bool, Pagina>> addToRealMemory(Pagina &page, bool isFifo) {
    pair<int, pair<bool, Pagina>> result = {-1, {false, Pagina()}};
    if(isFifo){
        bool realMemoryInsertionResult = RM_FIFO.agregar(Pagina(), TP_FIFO);
        if(!realMemoryInsertionResult){// there's not enough space in real memory
            // we should swap a page to secondary memory
            Pagina swapedPage = swap(page.get_id(),true);
            // we insert page again now that there's enough space
            RM_FIFO.agregar(page, TP_FIFO);
            // since a swap occurred we have to add this to the result
            result.second = {true, swapedPage};
        }
        // setup the real memory frame where the page ended up
        result.first = TP_FIFO.getPosicionRealMemory(page);
        // we finally add it to the queue
        fifo.agregar(page);
        // last but not least we return the result
    }else{
        bool realMemoryInsertionResult = RM_LRU.agregar(Pagina(), TP_LRU);
        if(!realMemoryInsertionResult){// there's not enough space in real memory
            // we should swap a page to secondary memory
            Pagina swapedPage = swap(page.get_id(),false);
            // we insert page again now that there's enough space
            RM_LRU.agregar(page, TP_LRU);
            // since a swap occurred we have to add this to the result
            result.second = {true, swapedPage};
        }
        // setup the real memory frame where the page ended up
        result.first = TP_LRU.getPosicionRealMemory(page);
        // we finally add it to the queue
        lru.agregar(page);
        // last but not least we return the result
    }
    return result;
}

void createProcess(int pId, int bytes, int totalPages, double currentTime, bool isFifo){
    if(isFifo){
        historiaProceso_FIFO[pId] = Proceso(bytes, totalPages, currentTime);
    } else {
        historiaProceso_LRU[pId] = Proceso(bytes, totalPages, currentTime);
    }
}

void eraseProcess(int p, bool isFifo) {

    if (isFifo) {
        // if the proccess doesn't even exist then there's an error
        if(historiaProceso_FIFO.count(p) ||historiaProceso_FIFO[p].checkFinalizado()) {
            vector<int> realMemoryFrames, secondaryMemoryFrames;
            Proceso pcs = historiaProceso_FIFO[p];
            for (int i = 0; i < pcs.getPaginas(); i++) {
                Pagina currentPage(p, i);
                if (TP_FIFO.WhereAmI(currentPage)) {
                    realMemoryFrames.push_back(TP_FIFO.getPosicionRealMemory(currentPage));
                    RM_FIFO.borrar(currentPage, TP_FIFO);
                    //add to the current time the time it takes to remove a page from real memory
                    currentTime_FIFO += d_free_page_real_memory;
                } else {
                    secondaryMemoryFrames.push_back(TP_FIFO.getPosicionVirtualMemory(currentPage));
                    VM_FIFO.borrar(currentPage, TP_FIFO);
                    //add to the current time the time it takes to remove a page from secondary memory
                    currentTime_FIFO += d_free_page_secondary_memory;
                }
                fifo.erase(currentPage);
            }

            TP_FIFO.eliminarProceso(p);
            historiaProceso_FIFO[p].terminarProceso(currentTime_FIFO);
        }
    } else {
        // if the proccess doesn't even exist then there's an error
        if(historiaProceso_LRU.count(p) || historiaProceso_LRU[p].checkFinalizado()) {
            vector<int> realMemoryFrames, secondaryMemoryFrames;
            Proceso pcs = historiaProceso_LRU[p];
            for (int i = 0; i < pcs.getPaginas(); i++) {
                Pagina currentPage(p, i);
                if (TP_LRU.WhereAmI(currentPage)) {
                    realMemoryFrames.push_back(TP_LRU.getPosicionRealMemory(currentPage));
                    RM_LRU.borrar(currentPage, TP_LRU);
                    //add to the current time the time it takes to remove a page from real memory
                    currentTime_LRU += d_free_page_real_memory;
                } else {
                    secondaryMemoryFrames.push_back(TP_LRU.getPosicionVirtualMemory(currentPage));
                    VM_LRU.borrar(currentPage, TP_LRU);
                    //add to the current time the time it takes to remove a page from secondary memory
                    currentTime_LRU += d_free_page_secondary_memory;
                }
                lru.erase(currentPage);
            }

            TP_LRU.eliminarProceso(p);
            historiaProceso_LRU[p].terminarProceso(currentTime_LRU);
        }
    }

}

        void Instruccion(char opcion,int NumInstruccion){
            int D,N,P,M;
    
    vector<int> params;
    
    
    switch (opcion){
        case 'P':
            params.clear();
            
            params = leerInstruccion(vDatos[NumInstruccion],2);
            N = params[0];
            P = params[1];
            Pnp(N,P,true);
            Pnp(N,P,false);
            break;
        case 'A':
            params.clear();
            
            params = leerInstruccion(vDatos[NumInstruccion],3);
            D = params[0];
            P = params[1];
            M = params[2];
            Adpm(D,P,M,true);
            Adpm(D,P,M,false);
            break;
        case 'L':
            params.clear();
            
            params = leerInstruccion(vDatos[NumInstruccion],1);
            P = params[0];
            Lp(P,true);
            Lp(P,false);
            break;
        case 'C':
            C(vDatos[NumInstruccion]);
            break;
        case 'F':
            F();
            // Reset the data structures so that new inputs are not affected by previous ones.
            ResetData();
            break;
        default:
            break;
    }
        }

        void Pnp(int n, int p, bool isFifo){
    /*Cargar un proceso:
     solicitud "n" bytes para cargar un proceso a la memoria.
     "p" es un número entero arbitrario que indica el id del proceso.*/
    if(isFifo){
        // Checar si ya existe --> Error
        if(!historiaProceso_FIFO.count(p)){
            int ipaginas = ceil(n/16.0);
            
            // Checar si el tamaño es más grande del que podemos almacenar
            if(!((ipaginas > 128) || ipaginas <= 0)){
                vector<int> realMemoryFrames; // stores where in real memory the pages of the process ended up.
                vector<Pagina> swapedPages; // stores the pages that had to be swapped in order to add this process properly.
                // create simple process
                createProcess(p, n, ipaginas, currentTime_FIFO, true);
                // begin transactions, start adding it to real memory
                for(int i = 0; i < ipaginas; i++){
                    // add time of adding initial page to memory
                    currentTime_FIFO += d_load_page_memory_initially;
                    // create page
                    Pagina currPage(p, n);
                    // add page to real memory and save the frame where it was added to output it later
                    pair<int, pair<bool, Pagina>> result = addToRealMemory(currPage, true);
                    realMemoryFrames.push_back(result.first);
                    
                    if(result.second.first) // if there was a fault then we should add the page swaped for further output
                        swapedPages.push_back(result.second.second);
                }
            }
            
        }
    } else {
        // Checar si ya existe --> Error
        if(!historiaProceso_LRU.count(p)){
            int ipaginas = ceil(n/16.0);
            
            // Checar si el tamaño es más grande del que podemos almacenar
            if(!((ipaginas > 128) || ipaginas <= 0)){
                vector<int> realMemoryFrames; // stores where in real memory the pages of the process ended up.
                vector<Pagina> swapedPages; // stores the pages that had to be swapped in order to add this process properly.
                // create simple process
                createProcess(p, n, ipaginas, currentTime_LRU, false);
                // begin transactions, start adding it to real memory
                for(int i = 0; i < ipaginas; i++){
                    // add time of adding initial page to memory
                    currentTime_LRU += d_load_page_memory_initially;
                    // create page
                    Pagina currPage(p, n);
                    // add page to real memory and save the frame where it was added to output it later
                    pair<int, pair<bool, Pagina>> result = addToRealMemory(currPage, false);
                    realMemoryFrames.push_back(result.first);
                    
                    if(result.second.first) // if there was a fault then we should add the page swaped for further output
                        swapedPages.push_back(result.second.second);
                }
            }
            
        }
    }
    
}

void Adpm(int d, int p, int m, bool isFifo){
    /*Solicitud para accesar la dirección virtual:
     "d" del proceso "p"
     "d" desde 0 a dir max virtual de proceso.*/
    
    if (isFifo) {
        if(!historiaProceso_FIFO.count(p)){

            // check if the requested virtual direction really exists, if not then we generate an error
            if(d > historiaProceso_FIFO[p].getBytes() || d < 0){
                
                // create page
                    int pageNumber = d / page_size;

                    Pagina page(p, pageNumber);
                    // check if page is in secondary memory
                if(!TP_FIFO.WhereAmI(page)) { // if it isn't then we have to move it to real memory
                        // first a page fault occured so we notify our process about that
                        Proceso &currProcess = historiaProceso_FIFO[p];
                    currProcess.agregarFallo();

                        
                        // we remove it form secondary memory
                        VM_FIFO.borrar(page, TP_FIFO);

                        // now we add it to real memory
                        pair<int, pair<bool, Pagina>> result = addToRealMemory(page, true);

                        // add to current time the time it takes to swap a page from secondary to real memory
                        currentTime_FIFO += d_swap;

                        // add the swap to the total swaps
                        totalSwaps_FIFO++;

                    // add to current time the time it takes to check a page in real memory
                    currentTime_FIFO += d_access_page_in_real_memory;
                
                }
                
            }
        }
    } else {
        if(!historiaProceso_LRU.count(p)){

            // check if the requested virtual direction really exists, if not then we generate an error
            if(d > historiaProceso_LRU[p].getBytes() || d < 0){
                
                // create page
                    int pageNumber = d / page_size;

                    Pagina page(p, pageNumber);
                    // check if page is in secondary memory
                if(!TP_LRU.WhereAmI(page)) { // if it isn't then we have to move it to real memory
                        // first a page fault occured so we notify our process about that
                        Proceso &currProcess = historiaProceso_LRU[p];
                    currProcess.agregarFallo();

                        
                        // we remove it form secondary memory
                        VM_LRU.borrar(page, TP_LRU);

                        // now we add it to real memory
                        pair<int, pair<bool, Pagina>> result = addToRealMemory(page, false);

                        // add to current time the time it takes to swap a page from secondary to real memory
                        currentTime_LRU+= d_swap;

                        // add the swap to the total swaps
                        totalSwaps_LRU++;

                    // add to current time the time it takes to check a page in real memory
                    currentTime_LRU += d_access_page_in_real_memory;

                    lru.actualiza(page);
                
                }
                
            }
        }
    }
    
}

void Lp(int p, bool isFifo){
    
    if (isFifo) {
        /*Libera las páginas del proceso "p":*/
        if(historiaProceso_FIFO.count(p) || historiaProceso_FIFO[p].checkFinalizado()){
            

            vector<int> realMemoryFrames, secondaryMemoryFrames;
            Proceso pcs = historiaProceso_FIFO[p];
            for (int i = 0; i < pcs.getPaginas(); i++) {
                Pagina currentPage(p, i);
                if (TP_FIFO.WhereAmI(currentPage)) {
                    realMemoryFrames.push_back(TP_FIFO.getPosicionRealMemory(currentPage));
                    RM_FIFO.borrar(currentPage, TP_FIFO);
                    //add to the current time the time it takes to remove a page from real memory
                    currentTime_FIFO += d_free_page_real_memory;
                } else {
                    secondaryMemoryFrames.push_back(TP_FIFO.getPosicionVirtualMemory(currentPage));
                    VM_FIFO.borrar(currentPage,TP_FIFO);
                    //add to the current time the time it takes to remove a page from secondary memory
                    currentTime_FIFO += d_free_page_secondary_memory;
                }
                fifo.erase(currentPage);
            }


            TP_FIFO.eliminarProceso(p);
            historiaProceso_FIFO[p].terminarProceso(currentTime_FIFO);
        }
        
    } else {
       /*Libera las páginas del proceso "p":*/
        if(historiaProceso_LRU.count(p) || historiaProceso_LRU[p].checkFinalizado()){
            

            vector<int> realMemoryFrames, secondaryMemoryFrames;
            Proceso pcs = historiaProceso_LRU[p];
            for (int i = 0; i < pcs.getPaginas(); i++) {
                Pagina currentPage(p, i);
                if (TP_LRU.WhereAmI(currentPage)) {
                    realMemoryFrames.push_back(TP_LRU.getPosicionRealMemory(currentPage));
                    RM_LRU.borrar(currentPage,TP_LRU);
                    //add to the current time the time it takes to remove a page from real memory
                    currentTime_LRU += d_free_page_real_memory;
                } else {
                    secondaryMemoryFrames.push_back(TP_LRU.getPosicionVirtualMemory(currentPage));
                    VM_LRU.borrar(currentPage,TP_LRU);
                    //add to the current time the time it takes to remove a page from secondary memory
                    currentTime_LRU += d_free_page_secondary_memory;
                }
                lru.erase(currentPage);
            }


            TP_LRU.eliminarProceso(p);
            historiaProceso_LRU[p].terminarProceso(currentTime_LRU);
        }
    }
}

void C(string s){
    //Linea de comentarios
    s = s.substr(1);
    int ipos = s.find_first_not_of(' ');
    if(ipos < 0){
        s = " ";
    } else{
        s = s.substr(ipos);
    }
    
    cout << s << endl;
    
}

void F(){
    
        /*turnaround time / turnaround promedio / fallos de página / # total de operaciones swap.*/
         
         //Status status;
         //status.addStringResult("Reporte de Salida"); // Initialize the string that will be used for output.
         //status.addStringResult("Turn around de procesos:");
         cout << "Turn around de procesos con FIFO:" << endl;
         // Initialize average turn around value
         double averageTurnAround_FIFO = 0;

         // Calculate average turn around while adding to the ouput the individual turn around of each process
         for(auto it = historiaProceso_FIFO.begin(); it != historiaProceso_FIFO.end(); it++) {
             Proceso &p = it->second;
             if (!p.checkFinalizado()){
                 eraseProcess(it->first, true);
             }
             cout << "El turn around del proceso " << to_string(it->first) << " fue " << to_string(p.getTurnAround()) <<
                       " segundos" << endl;
             averageTurnAround_FIFO += p.getTurnAround();
         }
         averageTurnAround_FIFO/=(double)historiaProceso_FIFO.size();

         // Add to output the page faults per process
         for(auto it = historiaProceso_FIFO.begin(); it != historiaProceso_FIFO.end(); it++) {
             Proceso &p = it->second;
             cout << "Los cantidad de fallos de página del proceso " << to_string(it->first) << " fue " << to_string(p.getFallos()) << endl;
         }

         // Display the average turnaround time.
        cout << "Average Turnaround Time: " << to_string(averageTurnAround_FIFO) << endl;

         // Display the number of swap in swap out operations.
         cout << "Number of Swap In Swap Out Operations: " << to_string(totalSwaps_FIFO) << "\n\n" <<endl;

   
        /*turnaround time / turnaround promedio / fallos de página / # total de operaciones swap.*/
         
         //Status status;
         //status.addStringResult("Reporte de Salida"); // Initialize the string that will be used for output.
         //status.addStringResult("Turn around de procesos:");
         cout << "Turn around de procesos con LRU:" << endl;
         // Initialize average turn around value
         double averageTurnAround_LRU = 0;

         // Calculate average turn around while adding to the ouput the individual turn around of each process
         for(auto it = historiaProceso_LRU.begin(); it != historiaProceso_LRU.end(); it++) {
             Proceso &p = it->second;
             if (!p.checkFinalizado()){
                 eraseProcess(it->first, true);
             }
             cout << "El turn around del proceso " << to_string(it->first) << " fue " << to_string(p.getTurnAround()) <<
                       " segundos" << endl;
             averageTurnAround_LRU += p.getTurnAround();
         }
         averageTurnAround_LRU/=(double)historiaProceso_LRU.size();

         // Add to output the page faults per process
         for(auto it = historiaProceso_LRU.begin(); it != historiaProceso_LRU.end(); it++) {
             Proceso &p = it->second;
             cout << "Los cantidad de fallos de página del proceso " << to_string(it->first) << " fue " << to_string(p.getFallos()) << endl;
         }

         // Display the average turnaround time.
        cout << "Average Turnaround Time: " << to_string(averageTurnAround_LRU) << endl;

         // Display the number of swap in swap out operations.
         cout << "Number of Swap In Swap Out Operations: " << to_string(totalSwaps_LRU) << "\n\n" <<endl;
    
    
}

};
#endif //Controlador.h
