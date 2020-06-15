#ifndef CONTROLADOR_H
#define CONTROLADOR_H
#include <iostream>
#include <vector>
#include <map>
#include "RealMemory.h"
#include "VirtualMemory.h"
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
        double tiempo_FIFO;
        Queue fifo;
        // Variables LRU
        RealMemory RM_LRU;
        VirtualMemory VM_LRU;
        TablaPaginacion TP_LRU;
        map<int, Proceso> historiaProceso_LRU;
        int totalSwaps_LRU;
        double tiempo_LRU;
        Queue lru;
        double CargaPagina = 1.0, Swap = 1.0, dAccesoPaginaRM = 0.1,
        dDisponiblePaginaRM = 0.1, dDisponiblePaginaVM = 0.1;
        int tamanoPagina = 16;

    public:
        
        Controlador() {
            this->RM_FIFO = RealMemory();
            this->VM_FIFO = VirtualMemory();
            this->TP_FIFO = TablaPaginacion();
            this->historiaProceso_FIFO = map<int, Proceso>();
            this->tiempo_FIFO = 0.0;
            this->totalSwaps_FIFO = 0;
            this->fifo = Queue();
            this->RM_LRU = RealMemory();
            this->VM_LRU = VirtualMemory();
            this->TP_LRU = TablaPaginacion();
            this->historiaProceso_LRU = map<int, Proceso>();
            this->tiempo_LRU = 0.0;
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
            tiempo_FIFO = 0.0;
            totalSwaps_FIFO = 0;
            fifo.clear();
            RM_LRU = RealMemory();
            VM_LRU = VirtualMemory();
            TP_LRU = TablaPaginacion();
            historiaProceso_LRU = map<int, Proceso>();
            tiempo_LRU = 0.0;
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
                swapPag = fifo.front(p);
                RM_FIFO.borrar(swapPag, TP_FIFO);
                VM_FIFO.agregar(swapPag, TP_FIFO);
                tiempo_FIFO += Swap;
                
                totalSwaps_FIFO++;
            } else {
                swapPag = lru.front(p);
                RM_FIFO.borrar(swapPag,TP_LRU);
                VM_FIFO.agregar(swapPag,TP_LRU);
                tiempo_LRU += Swap;
                
                totalSwaps_LRU++;
            }
            return swapPag;
        }

        pair<int, pair<bool, Pagina>> AgregarRealMemory(Pagina &page, bool isFifo) {
            pair<int, pair<bool, Pagina>> resultado = {-1, {false, Pagina()}};
            if(isFifo){
                bool RMResultadoAgrado = RM_FIFO.agregar(Pagina(), TP_FIFO);
                if(!RMResultadoAgrado){
                    Pagina swapedPage = swap(page.get_id(),true);
                    RM_FIFO.agregar(page, TP_FIFO);
                    resultado.second = {true, swapedPage};
                }
                resultado.first = TP_FIFO.getPosicionRealMemory(page);
                fifo.agregar(page);
            }else{
                bool realMemoryInsertionResult = RM_LRU.agregar(Pagina(), TP_LRU);
                if(!realMemoryInsertionResult){
                    Pagina swapedPage = swap(page.get_id(),false);
                    RM_LRU.agregar(page, TP_LRU);
                    resultado.second = {true, swapedPage};
                }
                resultado.first = TP_LRU.getPosicionRealMemory(page);
                lru.agregar(page);
            }
            return resultado;
        }

        void crearProceso(int pId, int bytes, int totalPaginas, double currentTime, bool isFifo){
            if(isFifo){
                historiaProceso_FIFO[pId] = Proceso(bytes, totalPaginas, currentTime);
            } else {
                historiaProceso_LRU[pId] = Proceso(bytes, totalPaginas, currentTime);
            }
        }

        void BorrarProceso(int p, bool isFifo) {
            if (isFifo) {
                if(historiaProceso_FIFO.count(p) ||historiaProceso_FIFO[p].checkFinalizado()) {
                    vector<int> RMFrame, VMFrame;
                    Proceso pcs = historiaProceso_FIFO[p];

                    for (int i = 0; i < pcs.getPaginas(); i++) {
                        Pagina currentPage(p, i);
                        if (TP_FIFO.WhereAmI(currentPage)) {
                            RMFrame.push_back(TP_FIFO.getPosicionRealMemory(currentPage));
                            RM_FIFO.borrar(currentPage, TP_FIFO);
                            tiempo_FIFO += dDisponiblePaginaRM;
                        } else {
                            VMFrame.push_back(TP_FIFO.getPosicionVirtualMemory(currentPage));
                            VM_FIFO.borrar(currentPage, TP_FIFO);
                            tiempo_FIFO += dDisponiblePaginaVM;
                        }
                        fifo.erase(currentPage);
                    }
                    TP_FIFO.eliminarProceso(p);
                    historiaProceso_FIFO[p].terminarProceso(tiempo_FIFO);
                }
            } else {
                if(historiaProceso_LRU.count(p) || historiaProceso_LRU[p].checkFinalizado()) {
                    vector<int> RMFrame, VMFrame;
                    Proceso pcs = historiaProceso_LRU[p];

                    for (int i = 0; i < pcs.getPaginas(); i++) {
                        Pagina currentPage(p, i);
                        if (TP_LRU.WhereAmI(currentPage)) {
                            RMFrame.push_back(TP_LRU.getPosicionRealMemory(currentPage));
                            RM_LRU.borrar(currentPage, TP_LRU);
                            tiempo_LRU += dDisponiblePaginaRM;
                        } else {
                            VMFrame.push_back(TP_LRU.getPosicionVirtualMemory(currentPage));
                            VM_LRU.borrar(currentPage, TP_LRU);
                            tiempo_LRU += dDisponiblePaginaVM;
                        }
                        lru.erase(currentPage);
                    }

                    TP_LRU.eliminarProceso(p);
                    historiaProceso_LRU[p].terminarProceso(tiempo_LRU);
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
                    int iPaginas = ceil(n/16.0);
                    
                    if(!((iPaginas > 128) || iPaginas <= 0)){
                        vector<int> RMFrame; 
                        vector<Pagina> swapedPages; 
                        crearProceso(p, n, iPaginas, tiempo_FIFO, true);
                        for(int i = 0; i < iPaginas; i++){
                            tiempo_FIFO += CargaPagina;
                            Pagina currPage(p, n);
                            pair<int, pair<bool, Pagina>> result = AgregarRealMemory(currPage, true);
                            RMFrame.push_back(result.first);
                            
                            if(result.second.first) 
                                swapedPages.push_back(result.second.second);
                        }
                    }
                    
                }
            } else {
                // Checar si ya existe --> Error
                if(!historiaProceso_LRU.count(p)){
                    int iPaginas = ceil(n/16.0);
                    
                    // Checar si el tamaño es más grande del que podemos almacenar
                    if(!((iPaginas > 128) || iPaginas <= 0)){
                        vector<int> RMFrame; 
                        vector<Pagina> swapedPages;
                        crearProceso(p, n, iPaginas, tiempo_LRU, false);
                        for(int i = 0; i < iPaginas; i++){
                            tiempo_LRU += CargaPagina;
                            // create page
                            Pagina currPage(p, n);
                            pair<int, pair<bool, Pagina>> result = AgregarRealMemory(currPage, false);
                            RMFrame.push_back(result.first);
                            if(result.second.first) 
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
                if(historiaProceso_FIFO.count(p)){

                    if(d < historiaProceso_FIFO[p].getBytes() || d > 0){
                        
                            int paginaNum = d / tamanoPagina;

                            Pagina pag(p, paginaNum);
                        if(!TP_FIFO.WhereAmI(pag)) { 
                                Proceso &currProcess = historiaProceso_FIFO[p];
                                currProcess.agregarFallo();
                                
                                VM_FIFO.borrar(pag, TP_FIFO);

                                pair<int, pair<bool, Pagina>> result = AgregarRealMemory(pag, true);
                                tiempo_FIFO += Swap;
                                totalSwaps_FIFO++;

                            tiempo_FIFO += dAccesoPaginaRM;
                        
                        }
                        
                    }
                }
            } else {
                if(historiaProceso_LRU.count(p)){

                    if(d < historiaProceso_LRU[p].getBytes() || d > 0){
                        
                            int paginaNum = d / tamanoPagina;

                            Pagina pag(p, paginaNum);
                            if(!TP_LRU.WhereAmI(pag)) { 
                                Proceso &currProcess = historiaProceso_LRU[p];
                                currProcess.agregarFallo();
                                    
                                    VM_LRU.borrar(pag, TP_LRU);

                                    pair<int, pair<bool, Pagina>> resultado = AgregarRealMemory(pag, false);

                                    tiempo_LRU+= Swap;

                                    totalSwaps_LRU++;

                                tiempo_LRU += dAccesoPaginaRM;
                                lru.actualiza(pag);
                                }
                            
                        }
                    }
                }   
        }

        void Lp(int p, bool isFifo){
            
            if (isFifo) {
                /*Libera las páginas del proceso "p":*/
                if(historiaProceso_FIFO.count(p) || historiaProceso_FIFO[p].checkFinalizado()){
                    
                    vector<int> RMFrame, VMFrame;
                    Proceso pcs = historiaProceso_FIFO[p];
                    for (int i = 0; i < pcs.getPaginas(); i++) {
                        Pagina PaginaActual(p, i);
                        if (TP_FIFO.WhereAmI(PaginaActual)) {
                            RMFrame.push_back(TP_FIFO.getPosicionRealMemory(PaginaActual));
                            RM_FIFO.borrar(PaginaActual, TP_FIFO);
                            tiempo_FIFO += dDisponiblePaginaRM;
                        } else {
                            VMFrame.push_back(TP_FIFO.getPosicionVirtualMemory(PaginaActual));
                            VM_FIFO.borrar(PaginaActual,TP_FIFO);
                            tiempo_FIFO += dDisponiblePaginaVM;
                        }
                        fifo.erase(PaginaActual);
                    }
                    TP_FIFO.eliminarProceso(p);
                    historiaProceso_FIFO[p].terminarProceso(tiempo_FIFO);
                }
            } else {
            /*Libera las páginas del proceso "p":*/
                if(historiaProceso_LRU.count(p) || historiaProceso_LRU[p].checkFinalizado()){
                    
                    vector<int> RMFrame, VMFrame;
                    Proceso pcs = historiaProceso_LRU[p];
                    for (int i = 0; i < pcs.getPaginas(); i++) {
                        Pagina PaginaActual(p, i);
                        if (TP_LRU.WhereAmI(PaginaActual)) {
                            RMFrame.push_back(TP_LRU.getPosicionRealMemory(PaginaActual));
                            RM_LRU.borrar(PaginaActual,TP_LRU);
                            tiempo_LRU += dDisponiblePaginaRM;
                        } else {
                            VMFrame.push_back(TP_LRU.getPosicionVirtualMemory(PaginaActual));
                            VM_LRU.borrar(PaginaActual,TP_LRU);
                            tiempo_LRU += dDisponiblePaginaVM;
                        }
                        lru.erase(PaginaActual);
                    }


                    TP_LRU.eliminarProceso(p);
                    historiaProceso_LRU[p].terminarProceso(tiempo_LRU);
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
         
         cout << "Turn around de procesos con FIFO:" << endl;
         double PromedioTurnAround_FIFO = 0;
         for(auto it = historiaProceso_FIFO.begin(); it != historiaProceso_FIFO.end(); it++) {
             Proceso &p = it->second;
             if (!p.checkFinalizado()){
                 BorrarProceso(it->first, true);
             }
             cout << "El turn around del proceso " << to_string(it->first) << " fue " << to_string(p.getTurnAround()) << " segundos" << endl;
             PromedioTurnAround_FIFO += p.getTurnAround();
         }
         PromedioTurnAround_FIFO /= (double)historiaProceso_FIFO.size();

         for(auto it = historiaProceso_FIFO.begin(); it != historiaProceso_FIFO.end(); it++) {
             Proceso &p = it->second;
             cout << "Los cantidad de fallos de página del proceso " << to_string(it->first) << " fue " << to_string(p.getFallos()) << endl;
         }

        cout << "Promedio Turnaround Time: " << to_string(PromedioTurnAround_FIFO) << endl;

        cout << "Cantidad de operaciones Swap In Swap Out: " << to_string(totalSwaps_FIFO) << "\n\n" <<endl;

   
        /*turnaround time / turnaround promedio / fallos de página / # total de operaciones swap.*/
         
         cout << "Turn around de procesos con LRU:" << endl;
         // Initialize average turn around value
         double PromedioTurnAround_LRU = 0;

         for(auto it = historiaProceso_LRU.begin(); it != historiaProceso_LRU.end(); it++) {
             Proceso &p = it->second;
             if (!p.checkFinalizado()){
                 BorrarProceso(it->first, true);
             }
             cout << "El turn around del proceso " << to_string(it->first) << " fue " << to_string(p.getTurnAround()) << " segundos" << endl;
             PromedioTurnAround_LRU += p.getTurnAround();
         }
         PromedioTurnAround_LRU /= (double)historiaProceso_LRU.size();

         for(auto it = historiaProceso_LRU.begin(); it != historiaProceso_LRU.end(); it++) {
             Proceso &p = it->second;
             cout << "Los cantidad de fallos de página del proceso " << to_string(it->first) << " fue " << to_string(p.getFallos()) << endl;
         }

        cout << "Promedio Turnaround Time: " << to_string(PromedioTurnAround_LRU) << endl;

         cout << "Cantidad de operaciones Swap In Swap Ou: " << to_string(totalSwaps_LRU) << "\n\n" <<endl;
    
}

};
#endif //Controlador.h
