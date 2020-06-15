#ifndef PAGINA_H
#define PAGINA_H

class Pagina{
    private:
    int id, iNumPagina;
    public:

    Pagina(){}

    Pagina(int id, int iNumPagina){
        this->id=id;
        this->iNumPagina=iNumPagina;
    }

    //Sets and gets
    int get_id(){
        return id;
    }

    int get_numPagina(){
        return iNumPagina;
    }

    void set_id(int id){
        this->id=id;
    }

    void set_numPagina(int iNumPagina){
        this->iNumPagina=iNumPagina;
    }

    


};
#endif