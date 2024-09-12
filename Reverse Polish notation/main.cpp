#include <iostream>
#include <stack>
#include <cctype>
#include <cmath>
#include <limits>
#include <string>

using namespace std;

bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

bool isFunction(char c) {
    return (c == 's' or c == 'c');
}

int getOperatorPriority(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/') {
        return 2;
    }
    else if (op == '^') {
        return 3;
    }
    else if (op == 's' || op == 'c') {
        return 4;
    }
    return 0;
}


string GetStringNumber(string expr, int& pos) {
    string strNumber = "";

    for (; pos < expr.length(); pos++) {
        char num = expr[pos];

        if (isdigit(num) || num == '.')
            strNumber += num;
        else {
            pos--;
            break;
        }
    }

    return strNumber;
}

string infixToPostfix(string expression) {
    string postfix = "";
    stack<char> operators;

    for (int i = 0; i < expression.length(); i++) {
        char c = expression[i];

        if (isdigit(c)) {
            postfix += GetStringNumber(expression, i) + " ";
        }
        else if (c == 'x' || c == 'X') {
            postfix += c;
            postfix += " ";
        }
        else if (c == '(') {
            operators.push(c);
        }
        else if (c == 's' || c == 'c') {
            operators.push(c);
        }
        else if (c == 'i' || c == 'n' || c == 't' || c == 'g') {
            continue;
        }
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                if (operators.top() == 's') {
                    postfix += "in";
                }
                else if (operators.top() == 'c') {
                    postfix += "tg";
                }
                postfix += " ";
                operators.pop();
            }

            operators.pop();
        }

        else if (isOperator(c)) {
            while (!operators.empty() && getOperatorPriority(operators.top()) >= getOperatorPriority(c)) {
                postfix += operators.top();
                if (operators.top() == 's') {
                    postfix += "in";
                }
                else if (operators.top() == 'c') {
                    postfix += "tg";
                }
                postfix += " ";
                operators.pop();
            }
            operators.push(c);
        }
    }

    while (!operators.empty()) {
        postfix += operators.top();
        if (operators.top() == 's') {
            postfix += "in";
        }
        else if (operators.top() == 'c') {
            postfix += "tg";
        }
        postfix += " ";
        operators.pop();
    }

    return postfix;
}

double calculate(double a, double b, char op) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    case '^':
        return pow(a, b);
    case 's':
        return sin(a);
    case 'c':
        return 1 / tan(a);
    default:
        return 0;
    }
}

double calculateFunctionValue(double x, string postfix) {
    stack<double> operands;

    for (int i = 0; i < postfix.length(); i++) {
        char c = postfix[i];

        if (isdigit(c)) {
            string num;
            num = GetStringNumber(postfix, i);
            operands.push(stod(num));
        }
        else if (c == 'x' or c == 'X') {
            operands.push(x);
        }
        else if (isFunction(c)) {
            double a = operands.top();
            operands.pop();
            double result = calculate(a, 0, c);
            operands.push(result);
        }
        else if (isOperator(c)) {
            double b = operands.top();
            operands.pop();
            double a = operands.top();
            operands.pop();
            double result = calculate(a, b, c);
            operands.push(result);

        }
    }

    return operands.top();
}

string addZeroForUnaryMinus(string infix) {
    string updatedInfix = "";
    int n = infix.length();

    for (int i = 0; i < n; i++) {
        if (infix[i] == '-' && (i == 0 || infix[i - 1] == '(')) {
            updatedInfix += "0";
        }
        updatedInfix += infix[i];
    }

    return updatedInfix;
}

bool first_check(string expression) {
    int cnt = 0;
    bool flag = true;
    for (int i = 0; i <= expression.length(); i++) {
        if (expression[i] == '(') cnt++;
        else if (expression[i] == ')') cnt--;
        if (cnt < 0) {
            flag = false;
            break;
        }
    }

    return (flag == false || cnt > 0);
}


int main() {
    string expression, postfix;
    double xmin, xmax, h;

    cout << "Введите выражение: ";
    cin >> expression;

    if (first_check(expression)) {
        cout << "Не валидное выражение";
    }
    else {
        postfix = addZeroForUnaryMinus(expression);

        postfix = infixToPostfix(postfix);
        cout << "Постфиксная запись: " << postfix << endl;
        cout << endl;

        cout << "Введите xmin, xmax, h: ";
        cin >> xmin >> xmax >> h;
        cout << endl;

        if (xmin > xmax || xmin != xmax && h == 0) {
            cout << "Ошибка ввода";
        }
        else {
            cout << "x\tf(x)" << endl;
            cout << "--------------" << endl;

            double inf = numeric_limits<double>::infinity();

            double x = xmin;
            while (true) {
                double f;

                f = calculateFunctionValue(x, postfix);
                cout << x << "\t";
                if (isnan(f) || f == inf) {
                    cout << "-" << endl;
                }
                else {
                    cout << f << endl;
                }


                if (fabs(x - xmax) < 0.00001) {
                    break;
                }

                x += h;
                if (x > xmax) {
                    x = xmax;
                    f = calculateFunctionValue(x, postfix);
                    cout << x << "\t";
                    if (isnan(f)) {
                        cout << "-" << endl;
                    }
                    else {
                        cout << f << endl;
                    }
                    break;
                }

            }
        }
    }
    return 0;
}
