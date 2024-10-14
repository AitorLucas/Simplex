// simplex.h
#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <iostream>
#include <limits> // Para usar std::numeric_limits

using namespace std;

class Simplex {
public:
    Simplex(double** matrizRestricoes, double* valoresRestricoes, double* coeficientesObjetivo, int numRestricoes, int numVariaveis);
    ~Simplex();
    void resolver();

private:
    double** matrizRestricoes;
    double* valoresRestricoes;
    double* coeficientesObjetivo;
    int numRestricoes, numVariaveis;
    double** tableau;
    int contagemIteracoes;

    void inicializarTableau();
    int encontrarColunaPivo();
    int encontrarLinhaPivo(int colunaPivo);
    void realizarPivo(int linhaPivo, int colunaPivo);
    void imprimirTableau();
    void imprimirSolucao();
};

#endif // SIMPLEX_H