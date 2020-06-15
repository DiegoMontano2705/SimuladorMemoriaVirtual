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
using namespace std;

class Controlador{

    private:
        vector<string> vDatos;
        RealMemory RM;
        VirtualMemory VM;
        TablaPaginacion TP;
        map<int, Proceso> historiaProceso;
        int totalSwaps;
        double currentTime;
        double d_load_page_memory_initially = 1.0, d_swap = 1.0, d_access_page_in_real_memory = 0.1,
        d_free_page_real_memory = 0.1, d_free_page_secondary_memory = 0.1;
        //Queue rq

    public:
        
        Controlador(){
        }
        Controlador(Queue *rq) {
            this->RM = RealMemory();
            this->VM = VirtualMemory();
            this->TP = TablaPaginacion();
            this->historiaProceso = map<int, Proceso>();
            this->currentTime = 0.0;
            this->totalSwaps = 0;
            //this->rq = rq;
            
        }

        void setDatos(vector<string> vDatos){
            this->vDatos = vDatos;
        }


        Status reporteSalida(){
            Status status;
            status.agregarPrint("Reporte de Salida");
            status.agregarPrint("Turn around de procesos:");

            double turnaroundPromedio = 0;

            for(auto it = historiaProceso.begin(); it != historiaProceso.end(); it++) {
                Proceso &p = it->second;
                if (!p.checkFinalizado()) borrarProceso(it->first);
                status.agregarPrint("El turn around del proceso " + to_string(it->first) + " es " + to_string(p.getTurnAround()) + " segundos");
                turnaroundPromedio += p.getTurnAround();
            }
            turnaroundPromedio /= (double)historiaProceso.size();

            for(auto it = historiaProceso.begin(); it != historiaProceso.end(); it++) {
                Proceso &p = it->second;
                status.agregarPrint("Los page faults del proceso " + to_string(it->first) + " son " + to_string(p.getFallos()));
            }
            status.agregarPrint("Tiempo Turnaround Promedio: " + to_string(turnaroundPromedio));

            status.agregarPrint("Total de swap in y out: " + to_string(totalSwaps));

            ResetData();
            return status;
        }


        Status borrarProceso(int pId) {
            Status status;
            status.agregarPrint("Liberar los marcos de pagina ocupados por el proceso " + to_string(pId));
            vector<int> realMemoryFrames, secondaryMemoryFrames;
            Proceso pcs = getProceso(pId);
            for (int i = 0; i < pcs.getPaginas(); i++) {
                Pagina currentPage(pId, i);
                if (TP.WhereAmI(currentPage)) {
                    realMemoryFrames.push_back(TP.getPosicionRealMemory(currentPage));
                    RM.borrar(currentPage, TP);
                    currentTime += d_free_page_real_memory;
                } else {
                    secondaryMemoryFrames.push_back(TP.getPosicionVirtualMemory(currentPage));
                    VM.borrar(currentPage, TP);
                    currentTime += d_free_page_secondary_memory;
                }
                rq->erase(currentPage);
            }

            if(realMemoryFrames.size() > 0){
                string realMemoryFramesString = "";
                realMemoryFramesString += "Se libreraron los marcos de pagina de memoria real: [";

                for(int frame : realMemoryFrames) realMemoryFramesString += to_string(frame) + ", ";

                realMemoryFramesString.pop_back(); realMemoryFramesString.pop_back(); /

                realMemoryFramesString += "]";
                status.agregarPrint(realMemoryFramesString);
            }

            if(secondaryMemoryFrames.size() > 0){
                string secondaryMemoryFramesString = "";
                secondaryMemoryFramesString += "Se libreraron los marcos de pagina del area de swapping: [";

                for(int frame : secondaryMemoryFrames) secondaryMemoryFramesString += to_string(frame) + ", ";

                secondaryMemoryFramesString.pop_back(); secondaryMemoryFramesString.pop_back(); /

                secondaryMemoryFramesString += "]";

                status.agregarPrint(secondaryMemoryFramesString);
        }

        this->TP.eliminarProceso(pId);
        endProceso(pId);
        return status;
    }

        void ResetData(){
            RM = RealMemory();
            VM = VirtualMemory();
            TP = TablaPaginacion();
            historiaProceso = map<int, Proceso>();
            currentTime = 0.0;
            totalSwaps = 0;
            //rq->reset();
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
                cout << "Check" << endl;
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

        bool Instruccion(char opcion,int NumInstruccion){
            vector<string> DigitosSeparados;
            vector<int> params;
            int D,N,P,M;

            switch (opcion)
            {
            case 'P':
                params.clear();
                params = leerInstruccion(vDatos[NumInstruccion],2);
                N = params[0];
                P = params[1];
                return Pnp(N,P);
                break;
            case 'A':
                params.clear();
                params = leerInstruccion(vDatos[NumInstruccion],3);
                D = params[0];
                P = params[1];
                M = params[2];
                return Adpm(D,P,M);
            break;
            case 'L':
                params.clear();
                params = leerInstruccion(vDatos[NumInstruccion],1);
                P = params[0];
                return Lp(P);
                break;
            case 'C':
                C();
                break;
            case 'F':
                return F();
                break;
            case 'E':
                E();
            default:
                break;
            }
        }

        bool Pnp(int n, int p){
            /*Cargar un proceso:
                solicitud "n" bytes para cargar un proceso a la memoria.
                "p" es un número entero arbitrario que indica el id del proceso.
            */
            return true;
        }

        bool Adpm(int d, int p, int m){
            /*Solicitud para accesar la dirección virtual:
            "d" del proceso "p"
            "d" desde 0 a dir max virtual de proceso.
            */
           return true;
        }

        bool Lp(int p){
            /*Libera las páginas del proceso "p":
            */
            return true;
        }

        void C(){
            //Linea de comentarios
        }

        bool F(){
            /*turnaround time / turnaround promedio / fallos de página / # total de operaciones swap.

            */
            return true;
        }

        void E(){
            //Termina el programa
        }

};
#endif //Controlador.h
