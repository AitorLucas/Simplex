// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstdlib> // Para usar system("clear") ou system("cls")
#include <iomanip> // Para formatar a saída
#include <cmath>   // Para std::round

using namespace std;

class Utils {
public:
    static string obterSubscrito(int numero);
    static void limparTela();
    static double arredondar(double valor, int casasDecimais);
};

#endif // UTILS_H