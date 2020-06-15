#ifndef STATUS_H
#define STATUS_H
#include <iostream>
#include <vector>
using namespace std;

class Status{
    private:
        bool status;
        vector<string> print;
    public:
        Status(){
            status=false;
            vector<string>();
        }

        void setStatus(bool status){
            this->status=status;
        }

        void agregarPrint(string s){
            print.push_back(s);
        }

};

#endif