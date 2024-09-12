#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

void show_one_dim(const vector<int>& vec) {
  for (int i = 0; i < vec.size(); i++) {
    cout << vec[i] << " ";
  }
}

void show_two_dim(const vector<vector<int>>& vec) {
    for(const auto& row : vec) {
        for(const auto& element : row) {
            cout << element << " ";
        }
        cout << endl;
    }
}

//Схема Дженнингса

void comp_Djen(vector<int>&a_an, vector<int>&a_d, int &rows, int &cols, int num) {
    string adress;
    if (num == 1) {
        adress = "/Users/ilya/Downloads/Типы и структуры данных 2 курс, 1 семестр/laba2_structs/test6.1.txt";
    }
    else {
        adress = "/Users/ilya/Downloads/Типы и структуры данных 2 курс, 1 семестр/laba2_structs/test6.2.txt";
    }
    ifstream file(adress);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл." << endl;
        return;
    }
    file >> rows >> cols;
    int count_a_an = 0;
    if (rows != cols) {
        cout << "Матрица не симметрична" << endl;
        return;
    }
    int q = 1; //чтобы брать половину матрицы
    for (int i = 0; i < rows; i++) {
        bool check = false;
        for (int j = 0; j < q; j++) {
            int element;
            file >> element;
            if (i == j) {
                a_an.push_back(element);
                a_d.push_back(count_a_an);
                count_a_an++;
            }
            else if (check == true) {
                a_an.push_back(element);
                count_a_an++;
            }
            else if(element != 0) {
                a_an.push_back(element);
                check = true;
                count_a_an++;
            }
        }
        string line;
        getline(file, line);
        q++;
    }
    file.close();
}

void sum_Djen(vector<int>&a_an, vector<int>&a_d, vector<int>&b_an, vector<int>&b_d, vector<int>&c_an, vector<int>&c_d, int &rows_a, int &cols_a, int &rows_b, int &cols_b) {
    if (rows_a != rows_b or cols_a != cols_b) {
        cout << "Невозможно сложить матрицы" << endl;
        return;
    }
    c_an.push_back(a_an[0] + b_an[0]);
    c_d.push_back(0);
    int count_c = 1;
    int k;
    for (int i = 1; i < rows_a; i++) {
        k = a_d[i] - a_d[i - 1] - b_d[i] + b_d[i - 1]; //понять какая строка длиннее
        if (k > 0) {
            int j = 1;
            while (j <= k) {
                c_an.push_back(a_an[a_d[i - 1] + j]);
                count_c++;
                j++;
            }
            while (a_d[i - 1] + j <= a_d[i]) {
                c_an.push_back(a_an[a_d[i - 1] + j] + b_an[b_d[i - 1] + j - k]);
                count_c++;
                j++;
            }
            c_d.push_back(count_c - 1);
        }
        else if (k < 0) {
            k = -1 * k;
            int j = 1;
            while (j <= k) {
                c_an.push_back(b_an[b_d[i - 1] + j]);
                count_c++;
                j++;
            }
            while (b_d[i - 1] + j <= b_d[i]) {
                c_an.push_back(b_an[b_d[i - 1] + j] + a_an[a_d[i - 1] + j - k]);
                count_c++;
                j++;
            }
            c_d.push_back(count_c - 1);
        }
        else {
            int j = a_d[i - 1] + 1; //идем по a_n
            int z = b_d[i - 1] + 1; //идем по b_n
            bool check = false;
            while (j <= a_d[i]) {
                int sum = a_an[j] + b_an[z];
                if (sum != 0) {
                    c_an.push_back(sum);
                    count_c++;
                    check = true;
                }
                else if (check == true) {
                    c_an.push_back(sum);
                    count_c++;
                }
                j++;
                z++;
            }
            if (check == true) {
                c_d.push_back(count_c - 1);
            }
        }
    }
}

void unpack_Djen(vector<int>&a_an, vector<int>&a_d, vector<vector<int>>&a) {
    a[0][0] = a_an[0];
    for (int i = 1; i < a_d.size(); i++) {
        a[i][i] = a_an[a_d[i]];
        if (a_d[i] - a_d[i - 1] > 1) {
            int ind = a_d[i] - 1;
            int k = 1; //помогает понять куда ставить не диагональный элемент
            while (ind != a_d[i - 1]) {
                a[i - k][i] = a_an[ind];
                a[i][i - k] = a_an[ind];
                k++;
                ind--;
            }
        }
    }
}

//Кольцевая схема
void add_element(vector<int>&an, vector<int>&nr, vector<int>&nc, vector<int>&jr, vector<int>&jc, int element, int i, int j, int &count_an) {
    if (element != 0) {
        if (jr[i] == -1) {
            jr[i] = count_an;
            nr.push_back(count_an);
        }
        else {
            int k = jr[i];
            while (nr[k] != jr[i]) {
                k = nr[k];
            }
            nr[k] = count_an;
            nr.push_back(jr[i]);
        }
        if (jc[j] == -1) {
            jc[j] = count_an;
            nc.push_back(count_an);
        }
        else {
            int k = jc[j];
            while (nc[k] != jc[j]) {
                k = nc[k];
            }
            nc[k] = count_an;
            nc.push_back(jc[j]);
        }
        an.push_back(element);
        count_an++;
    }
}

void comp_ring_RM(vector<int>&an, vector<int>&nr, vector<int>&nc, vector<int>&jr, vector<int>&jc, int &rows, int &cols, ifstream &file) {
    int count_an = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int element;
            file >> element;
            add_element(an, nr, nc, jr, jc, element, i, j, count_an);
        }
    }
    file.close();
}

int col_el(vector<int>&an, vector<int>&nc, vector<int>&jc, int coord) {
    for (int i = 0; i < jc.size(); i++) {
        if (jc[i] == coord) {
            return i;
        }
    }
    while (1) {
        coord = nc[coord];
        for (int i = 0; i < jc.size(); i++) {
            if (jc[i] == coord) {
                return i;
            }
        }
    }
}

int row_el(vector<int>&an, vector<int>&nr, vector<int>&jr, int coord) {
    for (int i = 0; i < jr.size(); i++) {
        if (jr[i] == coord) {
            return i;
        }
    }
    while (1) {
        coord = nr[coord];
        for (int i = 0; i < jr.size(); i++) {
            if (jr[i] == coord) {
                return i;
            }
        }
    }
}

void unpack_ring_RM(vector<vector<int>>&a, vector<int>&an, vector<int>&nr, vector<int>&nc, vector<int>&jr, vector<int>&jc) {
    for (int i = 0; i < an.size(); i++) {
        int row = row_el(an, nr, jr, i);
        int col = col_el(an, nc, jc, i);
        a[row][col] = an[i];
    }
}

void sum_ring_RM(vector<int>&a_an, vector<int>&a_nr, vector<int>&a_nc, vector<int>&a_jr, vector<int>&a_jc, int &rows_a, int &cols_a, vector<int>&b_an, vector<int>&b_nr, vector<int>&b_nc, vector<int>&b_jr, vector<int>&b_jc, int &rows_b, int &cols_b, vector<int>&c_an, vector<int>&c_nr, vector<int>&c_nc, vector<int>&c_jr, vector<int>&c_jc) {
    if (rows_a != rows_b or cols_a != cols_b) {
        cout << "Невозможно сложить матрицы" << endl;
        return;
        }
    else if (a_an.size() == 0) {
        c_an = b_an;
        c_nr = b_nr;
        c_nc = b_nc;
        c_jr = b_jr;
        c_jc = b_jc;
        return;
    }
    else if (b_an.size() == 0) {
        c_an = a_an;
        c_nr = a_nr;
        c_nc = a_nc;
        c_jr = a_jr;
        c_jc = a_jc;
        return;
    }
    int count_a = 0;
    int count_b = 0;
    int count_c = 0;
    while (count_a != a_an.size() or count_b != b_an.size()) {
        int col_a = col_el(a_an, a_nc, a_jc, count_a);
        int row_a = row_el(a_an, a_nr, a_jr, count_a);
        int col_b = col_el(b_an, b_nc, b_jc, count_b);
        int row_b = row_el(b_an, b_nr, b_jr, count_b);
        if (row_a > row_b) {
            add_element(c_an, c_nr, c_nc, c_jr, c_jc, b_an[count_b], row_b, col_b, count_c);
            count_b++;
        }
        else if (row_b > row_a) {
            add_element(c_an, c_nr, c_nc, c_jr, c_jc, a_an[count_a], row_a, col_a, count_c);
            count_a++;
        }
        else {
            if (col_a > col_b) {
                add_element(c_an, c_nr, c_nc, c_jr, c_jc, b_an[count_b], row_b, col_b, count_c);
                count_b++;
            }
            else if (col_b > col_a) {
                add_element(c_an, c_nr, c_nc, c_jr, c_jc, a_an[count_a], row_a, col_a, count_c);
                count_a++;
            }
            else {
                int sum = b_an[count_b] + a_an[count_a];
                add_element(c_an, c_nr, c_nc, c_jr, c_jc, sum, row_a, col_a, count_c);
                count_a++;
                count_b++;
            }
        }
    }
    while (count_a != a_an.size()) {
        int col_a = col_el(a_an, a_nc, a_jc, count_a);
        int row_a = row_el(a_an, a_nr, a_jr, count_a);
        add_element(c_an, c_nr, c_nc, c_jr, c_jc, a_an[count_a], row_a, col_a, count_c);
        count_a++;
    }
    while (count_b != b_an.size()) {
        int col_b = col_el(b_an, b_nc, b_jc, count_b);
        int row_b = row_el(b_an, b_nr, b_jr, count_b);
        add_element(c_an, c_nr, c_nc, c_jr, c_jc, b_an[count_b], row_b, col_b, count_c);
        count_b++;
    }
}

void mult_ring_RM(vector<int>&a_an, vector<int>&a_nr, vector<int>&a_nc, vector<int>&a_jr, vector<int>&a_jc, int &rows_a, int &cols_a, vector<int>&b_an, vector<int>&b_nr, vector<int>&b_nc, vector<int>&b_jr, vector<int>&b_jc, int &rows_b, int &cols_b, vector<int>&c_an, vector<int>&c_nr, vector<int>&c_nc, vector<int>&c_jr, vector<int>&c_jc) {
    if (rows_a != cols_b or cols_a != rows_b) {
        cout << "Невозможно перемножить матрицы" << endl;
        return;
    }
    if (a_an.size() == 0 or b_an.size() == 0) {
        return;
    }
    int count_c = 0;
    for (int i = 0; i < rows_a; i++) {
        int now_row = a_jr[i];
        int start_row = now_row;
        if (now_row == -1) {
            continue;
        }
        for (int j = 0; j < cols_b; j++) {
            int now_col = b_jc[j];
            int start_col = now_col;
            now_row = a_jr[i];
            if (now_col == -1) {
                continue;
            }
            int sum = 0;
            while (a_nr[now_row] != start_row and b_nc[now_col] != start_col) {
                int col_a = col_el(a_an, a_nc, a_jc, now_row);
                int row_b = row_el(b_an, b_nr, b_jr, now_col);
                if (col_a > row_b) {
                    now_col = b_nc[now_col];
                }
                else if (row_b > col_a) {
                    now_row = a_nr[now_row];
                }
                else{
//                    cout << col_a << " " << row_b << endl;
//                    cout << now_row << " " << now_col << endl;
                    sum += a_an[now_row] * b_an[now_col];
//                    cout << sum << " " << now_row << " " << now_col << endl;
                    now_col = b_nc[now_col];
                    now_row = a_nr[now_row];
                }
            }
            int col_a = col_el(a_an, a_nc, a_jc, now_row);
            int row_b = row_el(b_an, b_nr, b_jr, now_col);
            if (a_nr[now_row] == start_row) {
                if (col_a == row_b) {
                    sum += a_an[now_row] * b_an[now_col];
                }
                else if (col_a > row_b) {
                    do {
                        now_col = b_nc[now_col];
                        row_b = row_el(b_an, b_nr, b_jr, now_col);
                        if (row_b == col_a) {
                            sum += a_an[now_row] * b_an[now_col];
                            break;
                        }
                    } while (now_col != start_col);
                }
            }
            else if (b_nc[now_col] == start_col) {
                if (col_a == row_b) {
                    sum += a_an[now_row] * b_an[now_col];
                }
                else if (col_a < row_b) {
                    do {
                        now_row = a_nr[now_row];
                        col_a = col_el(a_an, a_nc, a_jc, now_row);
                        if (row_b == col_a) {
                            sum += a_an[now_row] * b_an[now_col];
                            break;
                        }
                    } while (now_row != start_row);
                }
            }
            add_element(c_an, c_nr, c_nc, c_jr, c_jc, sum, i, j, count_c);
        }
    }
}

int main() {
    cout << "Выберите схему, с которой хотите работать:" << endl;
    cout << "1 - Схема Дженнингса, 2 - Кольцевая схема" << endl;
    int k;
    cin >> k;
    if (k == 1) {
        //Работа с первым массивом(сжатие, распаковка, вывод на экран)
        vector<int> a_an;
        vector<int> a_d;
        int rows_a;
        int cols_a;
        comp_Djen(a_an, a_d, rows_a, cols_a, 1);
        vector<vector<int>> a(rows_a, vector<int>(rows_a, 0));
        cout << "a_an: ";
        show_one_dim(a_an);
        cout << endl;
        cout << "a_d: ";
        show_one_dim(a_d);
        cout << endl;
        unpack_Djen(a_an, a_d, a);
        cout << "a: " << endl;
        show_two_dim(a);
        cout << endl;
        
        //Работа со вторым массивом(сжатие, распаковка, вывод на экран)
        vector<int> b_an;
        vector<int> b_d;
        int rows_b;
        int cols_b;
        comp_Djen(b_an, b_d, rows_b, cols_b, 2);
        vector<vector<int>> b(rows_b, vector<int>(rows_b, 0));
        cout << "b_an: ";
        show_one_dim(b_an);
        cout << endl;
        cout << "b_d: ";
        show_one_dim(b_d);
        cout << endl;
        unpack_Djen(b_an, b_d, b);
        cout << "b: " << endl;
        show_two_dim(b);
        cout << endl;
        
        //Поиск суммы
        vector<int> c_an;
        vector<int> c_d;
        sum_Djen(a_an, a_d, b_an, b_d, c_an, c_d, rows_a, cols_a, rows_b, cols_b);
        if (c_an.size() == 0) {
            return 0;
        }
        vector<vector<int>> c(rows_b, vector<int>(rows_b, 0));
        cout << "c_an: ";
        show_one_dim(c_an);
        cout << endl;
        cout << "c_d: ";
        show_one_dim(c_d);
        cout << endl;
        unpack_Djen(c_an, c_d, c);
        cout << "c: " << endl;
        show_two_dim(c);
        cout << endl;
    }
    else {
        //упаковка и вывод первой матрицы
        ifstream file1("/Users/ilya/Downloads/Типы и структуры данных 2 курс, 1 семестр/laba2_structs/test18.1.txt");
        vector<int> a_an;
        vector<int> a_nr;
        vector<int> a_nc;
        int rows_a, cols_a;
        file1 >> rows_a >> cols_a;
        vector<int> a_jr(rows_a, -1);
        vector<int> a_jc(cols_a, -1);
        comp_ring_RM(a_an, a_nr, a_nc, a_jr, a_jc, rows_a, cols_a, file1);
        cout << "a.an: ";
        show_one_dim(a_an);
        cout << endl;
        cout << "a.nr: ";
        show_one_dim(a_nr);
        cout << endl;
        cout << "a.nc: ";
        show_one_dim(a_nc);
        cout << endl;
        cout << "a.jr: ";
        show_one_dim(a_jr);
        cout << endl;
        cout << "a.jc: ";
        show_one_dim(a_jc);
        cout << endl;
        cout << "a:" << endl;
        vector<vector<int>> a(rows_a, vector<int>(cols_a, 0));
        unpack_ring_RM(a, a_an, a_nr, a_nc, a_jr, a_jc);
        show_two_dim(a);
        cout << endl;
        
        // упаковка и вывод второй матрицы
        ifstream file2("/Users/ilya/Downloads/Типы и структуры данных 2 курс, 1 семестр/laba2_structs/test18.2.txt");
        vector<int> b_an;
        vector<int> b_nr;
        vector<int> b_nc;
        int rows_b, cols_b;
        file2 >> rows_b >> cols_b;
        vector<int> b_jr(rows_b, -1);
        vector<int> b_jc(cols_b, -1);
        comp_ring_RM(b_an, b_nr, b_nc, b_jr, b_jc, rows_b, cols_b, file2);
        cout << "b.an: ";
        show_one_dim(b_an);
        cout << endl;
        cout << "b.nr: ";
        show_one_dim(b_nr);
        cout << endl;
        cout << "b.nc: ";
        show_one_dim(b_nc);
        cout << endl;
        cout << "b.jr: ";
        show_one_dim(b_jr);
        cout << endl;
        cout << "b.jc: ";
        show_one_dim(b_jc);
        cout << endl;
        cout << "b:" << endl;
        vector<vector<int>> b(rows_b, vector<int>(cols_b, 0));
        unpack_ring_RM(b, b_an, b_nr, b_nc, b_jr, b_jc);
        show_two_dim(b);
        cout << endl;
        
        //сумма двух матриц
        vector<int> c_an;
        vector<int> c_nr;
        vector<int> c_nc;
        vector<int> c_jr(rows_a, -1);
        vector<int> c_jc(cols_b, -1);
        sum_ring_RM(a_an, a_nr, a_nc, a_jr, a_jc, rows_a, cols_a, b_an, b_nr, b_nc, b_jr, b_jc, rows_b, cols_b, c_an, c_nr, c_nc, c_jr, c_jc);
        if (rows_a == rows_b and cols_a == cols_b) {
            cout << "c" << endl;
            cout << "c.an: ";
            show_one_dim(c_an);
            cout << endl;
            cout << "c.nr: ";
            show_one_dim(c_nr);
            cout << endl;
            cout << "c.nc: ";
            show_one_dim(c_nc);
            cout << endl;
            cout << "c.jr: ";
            show_one_dim(c_jr);
            cout << endl;
            cout << "c.jc: ";
            show_one_dim(c_jc);
            cout << endl;
            cout << "c:" << endl;
            vector<vector<int>> c(rows_a, vector<int>(cols_a, 0));
            unpack_ring_RM(c, c_an, c_nr, c_nc, c_jr, c_jc);
            show_two_dim(c);
            cout << endl;
        }
        
        //произведение двух матриц
        vector<int> d_an;
        vector<int> d_nr;
        vector<int> d_nc;
        vector<int> d_jr(rows_a, -1);
        vector<int> d_jc(cols_b, -1);
        mult_ring_RM(a_an, a_nr, a_nc, a_jr, a_jc, rows_a, cols_a, b_an, b_nr, b_nc, b_jr, b_jc, rows_b, cols_b, d_an, d_nr, d_nc, d_jr, d_jc);
        if (rows_a == cols_b and rows_b == cols_a) {
            cout << "d" << endl;
            cout << "d.an: ";
            show_one_dim(d_an);
            cout << endl;
            cout << "d.nr: ";
            show_one_dim(d_nr);
            cout << endl;
            cout << "d.nc: ";
            show_one_dim(d_nc);
            cout << endl;
            cout << "d.jr: ";
            show_one_dim(d_jr);
            cout << endl;
            cout << "d.jc: ";
            show_one_dim(d_jc);
            cout << endl;
            cout << "d:" << endl;
            vector<vector<int>> d(rows_a, vector<int>(cols_b, 0));
            unpack_ring_RM(d, d_an, d_nr, d_nc, d_jr, d_jc);
            show_two_dim(d);
            cout << endl;
        }
    }
}
