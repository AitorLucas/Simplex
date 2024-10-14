// utils.cpp
#include "utils.h"

string Utils::obterSubscrito(int numero) {
    string subscrito = "";
    string numerosSubscritos[] = {"₀", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉"};
    while (numero > 0) {
        int digito = numero % 10;
        subscrito = numerosSubscritos[digito] + subscrito;
        numero /= 10;
    }
    return subscrito;
}

void Utils::limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

