#include "simplex.h"
#include "utils.h"

Simplex::Simplex(double** matrizRestricoes, double* valoresRestricoes, double* coeficientesObjetivo, int numRestricoes, int numVariaveis) 
    : matrizRestricoes(matrizRestricoes), valoresRestricoes(valoresRestricoes), coeficientesObjetivo(coeficientesObjetivo), numRestricoes(numRestricoes), numVariaveis(numVariaveis), contagemIteracoes(0) {
    tableau = new double*[numRestricoes + 1];
    for (int i = 0; i <= numRestricoes; ++i) {
        tableau[i] = new double[numVariaveis + numRestricoes + 1]();
    }
    inicializarTableau();
}

Simplex::~Simplex() {
    for (int i = 0; i <= numRestricoes; ++i) {
        delete[] tableau[i];
    }
    delete[] tableau;
}

void Simplex::resolver() {
    while (true) {
        contagemIteracoes++;
        int colunaPivo = encontrarColunaPivo();
        if (colunaPivo == -1) break; // Solução ótima encontrada
        int linhaPivo = encontrarLinhaPivo(colunaPivo);
        if (linhaPivo == -1) {
            cout << "Solução ilimitada" << endl;
            return;
        }
        realizarPivo(linhaPivo, colunaPivo);
        imprimirTableau();
    }
    imprimirSolucao();
}

void Simplex::inicializarTableau() {
    for (int i = 0; i < numRestricoes; ++i) {
        for (int j = 0; j < numVariaveis; ++j) {
            tableau[i][j] = matrizRestricoes[i][j]; // Coeficientes das variáveis de decisão
        }
        tableau[i][numVariaveis + i] = 1; // Variáveis de folga (1 na coluna da variável de folga)
        tableau[i][numVariaveis + numRestricoes] = valoresRestricoes[i]; // Lado direito (LD)
    }
    // Inicializa os coeficientes da função objetivo
    for (int j = 0; j < numVariaveis; ++j) {
        tableau[numRestricoes][j] = -coeficientesObjetivo[j]; // Maximização (negativo)
    }

    // Imprime a configuração inicial do tableau (primeira iteração)
    imprimirTableau();
}

int Simplex::encontrarColunaPivo() {
    int colunaPivo = -1;
    double valorMinimo = 0;
    for (int j = 0; j < numVariaveis + numRestricoes; ++j) {
        if (tableau[numRestricoes][j] < valorMinimo) {
            valorMinimo = tableau[numRestricoes][j];
            colunaPivo = j;
        }
    }
    return colunaPivo;
}

int Simplex::encontrarLinhaPivo(int colunaPivo) {
    int linhaPivo = -1;
    double menorRazao = std::numeric_limits<double>::max();
    for (int i = 0; i < numRestricoes; ++i) {
        if (tableau[i][colunaPivo] > 0) {
            // A razão θ é calculada como o valor do lado direito (β) dividido pelo coeficiente da coluna pivô
            double razao = tableau[i][numVariaveis + numRestricoes] / tableau[i][colunaPivo];
            if (razao < menorRazao) {
                menorRazao = razao;
                linhaPivo = i;
            }
        }
    }
    return linhaPivo;
}

void Simplex::realizarPivo(int linhaPivo, int colunaPivo) {
    double valorPivo = tableau[linhaPivo][colunaPivo];
    for (int j = 0; j <= numVariaveis + numRestricoes; ++j) {
        tableau[linhaPivo][j] /= valorPivo;
    }
    for (int i = 0; i <= numRestricoes; ++i) {
        if (i != linhaPivo) {
            double fator = tableau[i][colunaPivo];
            for (int j = 0; j <= numVariaveis + numRestricoes; ++j) {
                tableau[i][j] -= fator * tableau[linhaPivo][j];
            }
        }
    }
}

void Simplex::imprimirTableau() {
    const int casasDecimais = 2;

    cout << "Iteração " << contagemIteracoes << ":\n";

    // Imprimir cabeçalho das variáveis
    cout << "Base\t";
    for (int j = 0; j < numVariaveis; ++j) {
        cout << "x" << Utils::obterSubscrito(j + 1) << "\t";  // Variáveis de decisão
    }
    for (int j = 0; j < numRestricoes; ++j) {
        cout << "s" << Utils::obterSubscrito(j + 1) << "\t";  // Variáveis de folga
    }
    cout << "β\t";  // Beta
    cout << endl;

    // Imprimir as linhas do tableau
    for (int i = 0; i < numRestricoes; ++i) {
        // Identificar a variável base
        bool ehBase = false;
        int varBase = -1;
        for (int j = 0; j < numVariaveis; ++j) {
            if (tableau[i][j] == 1) {
                varBase = j;
                ehBase = true;
                break;
            }
        }
        if (ehBase) {
            cout << "x" << Utils::obterSubscrito(varBase + 1) << "\t";  // Variável base
        } else {
            cout << "s" << Utils::obterSubscrito(i + 1) << "\t";  // Variável de folga
        }

        // Imprimir os coeficientes da linha
        for (int j = 0; j <= numVariaveis + numRestricoes; ++j) {
            cout << Utils::arredondar(tableau[i][j], casasDecimais) << "\t";
        }

        cout << endl;
    }

    cout << endl;
    cout << "Cj-Zj\t";
    for (int j = 0; j <= numVariaveis + numRestricoes -1; ++j) {
        double zj = -Utils::arredondar(tableau[numRestricoes][j], casasDecimais);
        // if (j == numVariaveis + numRestricoes) {
        //     zj = -zj; // Beta
        // }
        if (zj == -0) {
            zj = 0;
        }
        cout << zj << "\t";
    }

    cout << endl;
    cout << "Zj(β): " << Utils::arredondar(tableau[numRestricoes][numVariaveis + numRestricoes], casasDecimais);
    cout << endl << endl;
}

void Simplex::imprimirSolucao() {
    double* solucao = new double[numVariaveis]();
    for (int j = 0; j < numVariaveis; ++j) {
        for (int i = 0; i < numRestricoes; ++i) {
            if (tableau[i][j] == 1) {
                bool ehBase = true;
                for (int k = 0; k < numRestricoes; ++k) {
                    if (k != i && tableau[k][j] != 0) {
                        ehBase = false;
                        break;
                    }
                }
                if (ehBase) {
                    solucao[j] = tableau[i][numVariaveis + numRestricoes];
                    break;
                }
            }
        }
    }
    cout << "Solução ótima encontrada:" << endl;
    for (int j = 0; j < numVariaveis; ++j) {
        cout << "x" << Utils::obterSubscrito(j + 1) << " = " << solucao[j] << endl;
    }
    cout << "Valor ótimo: " << tableau[numRestricoes][numVariaveis + numRestricoes] << endl;
    delete[] solucao;
}


