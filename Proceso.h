#ifndef PROCESO_H
#define PROCESO_H
using namespace std;

class Proceso{
private:
    int bytes;
    int paginas;
    double tInicio;
    double tFinal;
    int fallosDePagina;
    bool finalizado;
public:
    Proceso() {}
    //Constructor
    Proceso(int pbytes, int ppaginas, double tiempoInicial) {
        bytes = pbytes;
        paginas = ppaginas;
        tInicio = tiempoInicial;
        tFinal = 0;
        fallosDePagina = 0;
        finalizado = false;
    }

    // Obtener el tamaño del proceso
    int getBytes() {
        return bytes;
    }
    // Obtener la cantidad de páginas del proceso
    int getPaginas() {
        return paginas;
    }
    // Obtener los fallos de página
    int getFallos() {
        return fallosDePagina;
    }
    // Obtener el turn around
    double getTurnAround() {
        return (tFinal - tInicio);
    }
    // Registra un fallo de páginas
    void agregarFallo(){
        fallosDePagina++;
    }
    // Checar si el proceso ha finalizado
    bool checkFinalizado() {
        return finalizado;
    }
    // Termina un proceso
    void terminarProceso(double tiempo) {
        finalizado = true;
        tFinal = tiempo;
    }
};

#endif

