#ifndef QUEUE_H
#define QUEUE_H
#include <vector>
using namespace std;

class Queue{
private:
    vector<Pagina> queuePagina;
public:
    Queue(){
        queuePagina=vector<Pagina>();
    }
    Pagina front(int id){
        Pagina res;
        for(int i=0; i<queuePagina.size();i++){
            res=queuePagina[i];
            break;
        }
        queuePagina.erase(queuePagina.begin()+1);
        return res;
    }

    void erase(Pagina pagina){
        for(int i=queuePagina.size()-1; i>=0; i--){
            if(queuePagina.at(i).get_id()==pagina.get_id()){
                queuePagina.erase(queuePagina.begin()+1);
            }
        }
    }

    void agregar(Pagina pagina){
        queuePagina.push_back(pagina);
    }

    void actualiza(Pagina pagina) {
        int borra;
        for(int i=0; i<queuePagina.size(); i++){
            if(queuePagina[i].get_id() == pagina.get_id() && queuePagina[i].get_numPagina() == pagina.get_numPagina()){
                borra = i;
                break;
            }
        }
        queuePagina.erase(queuePagina.begin()+borra);
       queuePagina.push_back(pagina);
    }

    void clear(){
        queuePagina=vector<Pagina>();
    }


};

#endif