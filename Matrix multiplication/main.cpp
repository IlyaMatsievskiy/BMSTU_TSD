#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

// Функция для сложения матриц
vector<vector<int>> matrixAddition(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> result(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = A[i][j] + B[i][j];
        }
    }

    return result;
}

// Функция для вычитания матриц
vector<vector<int>> matrixSubtraction(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> result(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }

    return result;
}

// Функция для умножения матриц методом Штрассена
vector<vector<int>> strassenMatrixMultiply(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    int rowsA = A.size();
    int colsA = A[0].size();
    int rowsB = B.size();
    int colsB = B[0].size();
    int newSize = n;
    
    if (n <= 2) {
        vector<vector<int>> result(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    result[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        return result;
    }

    int newSizeCols = max(n, rowsB);
    int newSizeRows = max(colsA, colsB);
    if (n % 2 != 0) {
        newSize++;
    }

    vector<vector<int>> newA(newSizeRows, vector<int>(newSizeCols, 0));
    vector<vector<int>> newB(newSizeRows, vector<int>(newSizeCols, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            newA[i][j] = A[i][j];
            newB[i][j] = B[i][j];
        }
    }

    vector<vector<int>> A11(newSize / 2, vector<int>(newSize / 2, 0));
    vector<vector<int>> A12(newSize / 2, vector<int>(newSize / 2, 0));
    vector<vector<int>> A21(newSize / 2, vector<int>(newSize / 2, 0));
    vector<vector<int>> A22(newSize / 2, vector<int>(newSize / 2, 0));
    vector<vector<int>> B11(newSize / 2, vector<int>(newSize / 2, 0));
    vector<vector<int>> B12(newSize / 2, vector<int>(newSize / 2, 0));
    vector<vector<int>> B21(newSize / 2, vector<int>(newSize / 2, 0));
    vector<vector<int>> B22(newSize / 2, vector<int>(newSize / 2, 0));
    for (int i = 0; i < newSize / 2; i++) {
        for (int j = 0; j < newSize / 2; j++) {
            A11[i][j] = newA[i][j];
            A12[i][j] = newA[i][j + newSize / 2];
            A21[i][j] = newA[i + newSize / 2][j];
            A22[i][j] = newA[i + newSize / 2][j + newSize / 2];

            B11[i][j] = newB[i][j];
            B12[i][j] = newB[i][j + newSize / 2];
            B21[i][j] = newB[i + newSize / 2][j];
            B22[i][j] = newB[i + newSize / 2][j + newSize / 2];
        }
    }

    vector<vector<int>> M1 = strassenMatrixMultiply(matrixAddition(A11, A22), matrixAddition(B11, B22));
    vector<vector<int>> M2 = strassenMatrixMultiply(matrixAddition(A21, A22), B11);
    vector<vector<int>> M3 = strassenMatrixMultiply(A11, matrixSubtraction(B12, B22));
    vector<vector<int>> M4 = strassenMatrixMultiply(A22, matrixSubtraction(B21, B11));
    vector<vector<int>> M5 = strassenMatrixMultiply(matrixAddition(A11, A12), B22);
    vector<vector<int>> M6 = strassenMatrixMultiply(matrixSubtraction(A21, A11), matrixAddition(B11, B12));
    vector<vector<int>> M7 = strassenMatrixMultiply(matrixSubtraction(A12, A22), matrixAddition(B21, B22));

    vector<vector<int>> C11 = matrixAddition(matrixSubtraction(matrixAddition(M1, M4), M5), M7);
    vector<vector<int>> C12 = matrixAddition(M3, M5);
    vector<vector<int>> C21 = matrixAddition(M2, M4);
    vector<vector<int>> C22 = matrixAddition(matrixAddition(matrixSubtraction(M1, M2), M3), M6);

    vector<vector<int>> result(newSize, vector<int>(newSize));
    for (int i = 0; i < newSize / 2; i++) {
        for (int j = 0; j < newSize / 2; j++) {
            result[i][j] = C11[i][j];
            result[i][j + newSize / 2] = C12[i][j];
            result[i + newSize / 2][j] = C21[i][j];
            result[i + newSize / 2][j + newSize / 2] = C22[i][j];
        }
    }

    // Матрица без лишних нулей
    int k = min(rowsA, rowsB);
    int t = min(colsA, colsB);
    vector<vector<int>> finalResult(k, vector<int>(t));
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < t; j++) {
            finalResult[i][j] = result[i][j];
        }
    }

    return finalResult;
}

// Функция для умножения матриц классическим способом
vector<vector<int>> classicMatrixMultiply(const vector<vector<int>>& matA, const vector<vector<int>>& matB) {

    if (matA[0].size() != matB.size()) {
        cout << "Невозможно умножить матрицы: неподходящие размеры" << endl;
        return {};
    }

    vector<vector<int>> result(matA.size(), vector<int>(matB[0].size(), 0));

    for (int i = 0; i < matA.size(); i++) {
        for (int j = 0; j < matB[0].size(); j++) {
            for (int k = 0; k < matA[0].size(); k++) {
                result[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }

    return result;
}

// Функция для умножения матриц с использованием алгоритма Винограда
vector<vector<int>> matrixMultiplyVinograd(const vector<vector<int>>& matA, const vector<vector<int>>& matB) {
    int rowsA = matA.size();
    int colsA = matA[0].size();
    int rowsB = matB.size();
    int colsB = matB[0].size();

    if (colsA != rowsB) {
        cout << "Невозможно умножить матрицы: неподходящие размеры" << endl;
        return {};
    }

    vector<vector<int>> result(rowsA, vector<int>(colsB, 0));

    // Предварительные вычисления для оптимизации
    vector<int> rowFactor(rowsA, 0);
    vector<int> colFactor(colsB, 0);
    for (int i = 0; i < rowsA; i++) {
        for (int k = 0; k < colsA - 1; k += 2) {
            rowFactor[i] += matA[i][k] * matA[i][k + 1];
        }
    }
    for (int j = 0; j < colsB; j++) {
        for (int k = 0; k < rowsB - 1; k += 2) {
            colFactor[j] += matB[k][j] * matB[k + 1][j];
        }
    }

    // Основной цикл умножения с использованием предварительных вычислений
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            result[i][j] = -rowFactor[i] - colFactor[j];
            for (int k = 0; k < colsA - 1; k += 2) {
                result[i][j] += (matA[i][k] + matB[k + 1][j]) * (matA[i][k + 1] + matB[k][j]);
            }
            if (colsA % 2 != 0) {
                result[i][j] += matA[i][colsA - 1] * matB[rowsB - 1][j];
            }
        }
    }

    return result;
}

// Функция для вывода матрицы
void printMatrix(const vector<vector<int>>& mat) {
    for (const auto& row : mat) {
        for (int val : row) {
            if (val == 0) {
                continue;
            }
            cout << val << " ";
        }
        cout << endl;
    }
}

int main(){
    setlocale(0, "");
    cout << "Каким методом Вы хотите перемножить матрицы?" << endl;
    cout << "Классический метод - 1" << endl;
    cout << "Метод Винограда - 2" << endl;
    cout << "Метод Штрассена - 3" << endl;
    int choose = 0;
    double medtime = 0;
    cin >> choose;
    
    vector<vector<int>> matrixA = {{1, 2, -3}, {4, 5, 6}};
    vector<vector<int>> matrixB = {{7}, {8}, {9}};
    if (choose == 1) {

    

        cout << "Матрица A:" << endl;
        printMatrix(matrixA);
        cout << endl;

        cout << "Матрица B:" << endl;
        printMatrix(matrixB);
        cout << endl;

        clock_t start_time = clock();
        vector<vector<int>> result = classicMatrixMultiply(matrixA, matrixB);
        clock_t end_time = clock();
        double cpu_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;
        medtime += cpu_time;
        cout << "Результат умножения классическим методом:" << endl;
        printMatrix(result);
        cout << "Процессорное время: " << cpu_time << endl;
        cout << endl;
    }
    else if (choose == 2) {


        cout << "Матрица A:" << endl;
        printMatrix(matrixA);
        cout << endl;

        cout << "Матрица B:" << endl;
        printMatrix(matrixB);
        cout << endl;

        clock_t start_time = clock();
        vector<vector<int>> result = matrixMultiplyVinograd(matrixA, matrixB);
        clock_t end_time = clock();
        double cpu_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;
        medtime += cpu_time;
        cout << "Результат умножения с помощью алгоритма Винограда:" << endl;
        printMatrix(result);
        cout << "Процессорное время: " << cpu_time << endl;
        cout << endl;
    }
    else if (choose == 3) {
        cout << "Матрица A:" << endl;
        printMatrix(matrixA);
        cout << endl;
        cout << "Матрица B:" << endl;
        printMatrix(matrixB);
        cout << endl;

        clock_t start_time = clock();
        vector<vector<int>> result = strassenMatrixMultiply(matrixA, matrixB);
        clock_t end_time = clock();
        double cpu_time = (double(end_time - start_time)) / CLOCKS_PER_SEC;
        medtime += cpu_time;
        cout << "Результат умножения с помощью алгоритма Штрассена:" << endl;
        printMatrix(result);
        cout << "Процессорное время: " << cpu_time << endl;
        cout << endl;
    }
    else {
        cout << "Неверный выбор" << endl;
    }
    return 0;
}
