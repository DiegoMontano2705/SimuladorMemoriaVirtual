#ifndef CONTROLADOR_H
#define CONTROLADOR_H
#include <iostream>
using namespace std;

class Controlador{

    private:

    public:
        
        Controlador(){
        }

        bool Instruccion(char opcion){
            switch (opcion)
            {
            case 'P':
                return Pnp(0,0);
                break;
            case 'A':
                return Adpm(0,0);
                break;
            case 'L':
                return Lp(0);
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

        bool Adpm(int d, int p){
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
