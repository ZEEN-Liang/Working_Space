#include <iostream>
#include "CNF.cpp"
using namespace std;

int main(){
    int l,c;
    cin>>l>>c;
    CNF *cnf;
    LITS *lits;
    lits=litsinit(l);
    cnf=cnfinit(l);
    
    
    while(c--){
        cout<<"here\n";
        addClause(cnf,lits);
    }
    
    return 0;
}