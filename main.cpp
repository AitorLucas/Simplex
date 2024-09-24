#include <iostream>
#include <limits>

using namespace std;

class Simplex {
public:
    Simplex(double** constraintMatrix, double* constraintValues, double* objectiveCoefficients, int numConstraints, int numVariables) 
        : constraintMatrix(constraintMatrix), constraintValues(constraintValues), objectiveCoefficients(objectiveCoefficients), numConstraints(numConstraints), numVariables(numVariables) {
        tableau = new double*[numConstraints + 1];
        for (int i = 0; i <= numConstraints; ++i) {
            tableau[i] = new double[numVariables + numConstraints + 1]();
        }
        initializeTableau();
    }

    ~Simplex() {
        for (int i = 0; i <= numConstraints; ++i) {
            delete[] tableau[i];
        }
        delete[] tableau;
    }

    void solve() {
        while (true) {
            int pivotColumn = findPivotColumn();
            if (pivotColumn == -1) break; // Optimal solution found

            int pivotRow = findPivotRow(pivotColumn);
            if (pivotRow == -1) {
                cout << "Unbounded solution" << endl;
                return;
            }

            performPivot(pivotRow, pivotColumn);
        }
        printSolution();
    }

private:
    double** constraintMatrix;
    double* constraintValues;
    double* objectiveCoefficients;
    int numConstraints, numVariables;
    double** tableau;

    void initializeTableau() {
        for (int i = 0; i < numConstraints; ++i) {
            for (int j = 0; j < numVariables; ++j) {
                tableau[i][j] = constraintMatrix[i][j];
            }
            tableau[i][numVariables + i] = 1;
            tableau[i][numVariables + numConstraints] = constraintValues[i];
        }
        for (int j = 0; j < numVariables; ++j) {
            tableau[numConstraints][j] = -objectiveCoefficients[j];
        }
    }

    int findPivotColumn() {
        int pivotColumn = -1;
        double minValue = 0;
        for (int j = 0; j < numVariables + numConstraints; ++j) {
            if (tableau[numConstraints][j] < minValue) {
                minValue = tableau[numConstraints][j];
                pivotColumn = j;
            }
        }
        return pivotColumn;
    }

    int findPivotRow(int pivotColumn) {
        int pivotRow = -1;
        double minRatio = std::numeric_limits<double>::max();
        for (int i = 0; i < numConstraints; ++i) {
            if (tableau[i][pivotColumn] > 0) {
                double ratio = tableau[i][numVariables + numConstraints] / tableau[i][pivotColumn];
                if (ratio < minRatio) {
                    minRatio = ratio;
                    pivotRow = i;
                }
            }
        }
        return pivotRow;
    }

    void performPivot(int pivotRow, int pivotColumn) {
        double pivotValue = tableau[pivotRow][pivotColumn];
        for (int j = 0; j <= numVariables + numConstraints; ++j) {
            tableau[pivotRow][j] /= pivotValue;
        }
        for (int i = 0; i <= numConstraints; ++i) {
            if (i != pivotRow) {
                double factor = tableau[i][pivotColumn];
                for (int j = 0; j <= numVariables + numConstraints; ++j) {
                    tableau[i][j] -= factor * tableau[pivotRow][j];
                }
            }
        }
    }

    void printSolution() {
        double* solution = new double[numVariables]();
        for (int i = 0; i < numConstraints; ++i) {
            bool isBasic = true;
            int basicVar = -1;
            for (int j = 0; j < numVariables; ++j) {
                if (tableau[i][j] == 1) {
                    if (basicVar == -1) {
                        basicVar = j;
                    } else {
                        isBasic = false;
                        break;
                    }
                } else if (tableau[i][j] != 0) {
                    isBasic = false;
                    break;
                }
            }
            if (isBasic && basicVar != -1) {
                solution[basicVar] = tableau[i][numVariables + numConstraints];
            }
        }
        cout << "Optimal solution found:" << endl;
        for (int j = 0; j < numVariables; ++j) {
            cout << "x" << j + 1 << " = " << solution[j] << endl;
        }
        cout << "Optimal value: " << -tableau[numConstraints][numVariables + numConstraints] << endl;
        delete[] solution;
    }
};

int main() {
    int numConstraints, numVariables;
    cout << "Enter the number of constraints: ";
    cin >> numConstraints;
    cout << "Enter the number of variables: ";
    cin >> numVariables;

    double** constraintMatrix = new double*[numConstraints];
    for (int i = 0; i < numConstraints; ++i) {
        constraintMatrix[i] = new double[numVariables];
    }

    double* constraintValues = new double[numConstraints];
    double* objectiveCoefficients = new double[numVariables];

    cout << "Enter the coefficients of the constraints (A):" << endl;
    for (int i = 0; i < numConstraints; ++i) {
        for (int j = 0; j < numVariables; ++j) {
            cin >> constraintMatrix[i][j];
        }
    }

    cout << "Enter the right-hand side values (b):" << endl;
    for (int i = 0; i < numConstraints; ++i) {
        cin >> constraintValues[i];
    }

    cout << "Enter the coefficients of the objective function (c):" << endl;
    for (int j = 0; j < numVariables; ++j) {
        cin >> objectiveCoefficients[j];
    }

    Simplex simplex(constraintMatrix, constraintValues, objectiveCoefficients, numConstraints, numVariables);
    simplex.solve();

    for (int i = 0; i < numConstraints; ++i) {
        delete[] constraintMatrix[i];
    }
    delete[] constraintMatrix;
    delete[] constraintValues;
    delete[] objectiveCoefficients;

    return 0;
}