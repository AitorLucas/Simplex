// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstdlib> // Para usar system("clear") ou system("cls")

using namespace std;

class Utils {
public:
    static string obterSubscrito(int numero);
    static void limparTela();
};

#endif // UTILS_H