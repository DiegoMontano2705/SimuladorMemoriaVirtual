#ifndef CONTROLADOR_H
#define CONTROLADOR_H
#include <iostream>
#include <vector>
//#include <boost/algorithm/string.hpp>
using namespace std;

class Controlador{

    private:
        vector<string> vDatos;

    public:
        
        Controlador(){
        }

        void setDatos(vector<string> vDatos){
            this->vDatos = vDatos;
        }

        void IniciarSimulador(){
            string sIntruccion;
            for(int i = 0; i < vDatos.size(); i++){
                sIntruccion = vDatos[i];
                Instruccion(sIntruccion[0],i);
            }        
        }

        
        void split(const string& s, char c,
            vector<string>& v) {
            string::size_type i = 0;
            string::size_type j = s.find(c);
            while (j != string::npos) {
                v.push_back(s.substr(i, j-i));
                i = ++j;
                j = s.find(c, j);
                if (j == string::npos)
                    v.push_back(s.substr(i, s.length()));
            }
        }

        bool Instruccion(char opcion,int NumInstruccion){
            vector<string> DigitosSeparados;
            int D,N,P,M;

            switch (opcion)
            {
            case 'P':
                //boost::split(DigitosSeparados,vDatos[NumInstruccion],boost::is_any_of(" "));
                split(vDatos[NumInstruccion],' ',DigitosSeparados);
                N = atoi(DigitosSeparados[1].c_str());
                P = atoi(DigitosSeparados[2].c_str());
                return Pnp(N,P);
                break;
            case 'A':

                //boost::split(DigitosSeparados,vDatos[NumInstruccion],boost::is_any_of(" "));
                split(vDatos[NumInstruccion],' ',DigitosSeparados);
                D = atoi(DigitosSeparados[1].c_str());
                P = atoi(DigitosSeparados[2].c_str());
                M = atoi(DigitosSeparados[3].c_str());
                return Adpm(D,P,M);
            break;
            case 'L':
                //boost::split(DigitosSeparados,vDatos[NumInstruccion],boost::is_any_of(" "));
                split(vDatos[NumInstruccion],' ',DigitosSeparados);
                P = atoi(DigitosSeparados[1].c_str());
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
