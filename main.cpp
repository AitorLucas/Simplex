#include <iostream>
#include "simplex.h"
#include "utils.h"

using namespace std;

void exibirCabecalho(int numRestricoes, int numVariaveis, double** matrizRestricoes, double* valoresRestricoes, double* coeficientesObjetivo, int contador) {
    cout << "╔═══════════════════╗" << endl;
    cout << "║   Simplex Solver  ║" << endl;
    cout << "╚═══════════════════╝" << endl;

    // Exibir a função objetivo
    cout << "Função objetivo: Max Z = ";
    for (int j = 0; j < numVariaveis; ++j) {
        if (contador == j) {
            // Exibir sublinhado para o coeficiente que está sendo preenchido
            cout << "_x" << Utils::obterSubscrito(j + 1);
        } else if (contador > j) {
            // Exibir coeficiente já preenchido
            cout << coeficientesObjetivo[j] << "x" << Utils::obterSubscrito(j + 1);
        } else {
            // Exibir 0 para coeficientes que ainda não foram preenchidos
            cout << "0x" << Utils::obterSubscrito(j + 1);
        }
        if (j < numVariaveis - 1) {
            cout << " + ";
        }
    }
    cout << endl;

    // Exibir as restrições gradualmente
    int totalCoeficientesObjetivo = numVariaveis;
    for (int i = 0; i < numRestricoes; ++i) {
        // Exibir a restrição apenas se o contador já passou pela função objetivo e pela restrição anterior
        if (contador >= totalCoeficientesObjetivo + i * (numVariaveis + 1)) {
            cout << "Restrição " << i + 1 << ": ";
            for (int j = 0; j < numVariaveis; ++j) {
                int coefIndex = totalCoeficientesObjetivo + i * (numVariaveis + 1) + j;
                if (contador == coefIndex) {
                    // Exibir sublinhado para o coeficiente que está sendo preenchido
                    cout << "_x" << Utils::obterSubscrito(j + 1);
                } else if (contador > coefIndex) {
                    // Exibir coeficiente já preenchido
                    cout << matrizRestricoes[i][j] << "x" << Utils::obterSubscrito(j + 1);
                } else {
                    // Exibir 0 para coeficientes que ainda não foram preenchidos
                    cout << "0x" << Utils::obterSubscrito(j + 1);
                }
                if (j < numVariaveis - 1) {
                    cout << " + ";
                }
            }
            // Exibir o valor do lado direito da restrição
            int valorRestricaoIndex = totalCoeficientesObjetivo + i * (numVariaveis + 1) + numVariaveis;
            if (contador == valorRestricaoIndex) {
                // Exibir sublinhado para o valor do lado direito que está sendo preenchido
                cout << " <= _";
            } else if (contador > valorRestricaoIndex) {
                // Exibir valor já preenchido
                cout << " <= " << valoresRestricoes[i];
            } else {
                // Exibir 0 para valores que ainda não foram preenchidos
                cout << " <= 0";
            }
            cout << endl;
        }
    }
    cout << "=====================" << endl;
}

int main() {
    int numRestricoes, numVariaveis;

    // Solicitar o número de restrições e variáveis
    cout << "Digite o número de restrições: ";
    cin >> numRestricoes;
    cout << "Digite o número de variáveis: ";
    cin >> numVariaveis;

    // Alocar memória para a matriz de restrições e os vetores de valores
    double** matrizRestricoes = new double*[numRestricoes];
    for (int i = 0; i < numRestricoes; ++i) {
        matrizRestricoes[i] = new double[numVariaveis];
    }
    double* valoresRestricoes = new double[numRestricoes];
    double* coeficientesObjetivo = new double[numVariaveis];

    // Inicializar os coeficientes com 0 para evitar lixo de memória
    for (int i = 0; i < numRestricoes; ++i) {
        for (int j = 0; j < numVariaveis; ++j) {
            matrizRestricoes[i][j] = 0;
        }
        valoresRestricoes[i] = 0;
    }
    for (int j = 0; j < numVariaveis; ++j) {
        coeficientesObjetivo[j] = 0;
    }

    int contador = 0;
    while (contador < numVariaveis + numRestricoes * (numVariaveis + 1)) {
        Utils::limparTela();
        exibirCabecalho(numRestricoes, numVariaveis, matrizRestricoes, valoresRestricoes, coeficientesObjetivo, contador);
    
        if (contador < numVariaveis) {
            // Preenchendo a função objetivo
            cout << "Digite o coeficiente de x" << Utils::obterSubscrito(contador + 1) << " para a função objetivo: ";
            cin >> coeficientesObjetivo[contador];
        } else {
            // Preenchendo os coeficientes das restrições e os valores do lado direito
            int restricaoIndex = (contador - numVariaveis) / (numVariaveis + 1);
            int variavelIndex = (contador - numVariaveis) % (numVariaveis + 1);
    
            if (variavelIndex < numVariaveis) {
                // Preenchendo os coeficientes das variáveis nas restrições
                cout << "Digite o coeficiente de x" << Utils::obterSubscrito(variavelIndex + 1) << " para a restrição " << restricaoIndex + 1 << ": ";
                cin >> matrizRestricoes[restricaoIndex][variavelIndex];
            } else {
                // Preenchendo o valor do lado direito da restrição
                cout << "Digite o valor do lado direito da restrição " << restricaoIndex + 1 << ": ";
                cin >> valoresRestricoes[restricaoIndex];
            }
        }
        contador++;
    }
    Utils::limparTela();
    exibirCabecalho(numRestricoes, numVariaveis, matrizRestricoes, valoresRestricoes, coeficientesObjetivo, contador);

    // Resolve
    Simplex simplex(matrizRestricoes, valoresRestricoes, coeficientesObjetivo, numRestricoes, numVariaveis);
    simplex.resolver();

    // Liberar memória
    for (int i = 0; i < numRestricoes; ++i) {
        delete[] matrizRestricoes[i];
    }
    delete[] matrizRestricoes;
    delete[] valoresRestricoes;
    delete[] coeficientesObjetivo;

    return 0;
}