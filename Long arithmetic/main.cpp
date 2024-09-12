#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

const int MAX_DIGITS = 10;

class QuaternaryNumber {
private:
    int digits[MAX_DIGITS];
    bool isNegative;

public:
    bool isValid = true;
    QuaternaryNumber() {
        for (int i = 0; i < MAX_DIGITS; ++i) {
            digits[i] = 0;
        }
        isNegative = false;
    }

    QuaternaryNumber(int value) {
        isNegative = value < 0;
        value = abs(value);

        for (int i = 0; i < MAX_DIGITS; ++i) {
            digits[i] = value % 4;
            value /= 4;
        }
        if (value != 0) {
            cout << "Переполнение" << endl;
            isValid = false;
        }
    }

    // Операторы сравнения по модулю
    bool operator==(const QuaternaryNumber& other) const {
        return isEqual(other);
    }

    bool operator!=(const QuaternaryNumber& other) const {
        return !isEqual(other);
    }

    bool operator<<=(const QuaternaryNumber& other) const {
        return isLessThan_abs(other); //сравнение по модулю
    }

    bool operator>>=(const QuaternaryNumber& other) const {
        return isGreaterThan_abs(other); //сравнение по модулю
    }
    
    bool operator<(const QuaternaryNumber& other) const {
        return isLessThan(other);
    }

    bool operator>(const QuaternaryNumber& other) const {
        return isGreaterThan(other);
    }

    bool operator<=(const QuaternaryNumber& other) const {
        return !isGreaterThan(other);
    }

    bool operator>=(const QuaternaryNumber& other) const {
        return !isLessThan(other);
    }

    // Оператор присваивания
    QuaternaryNumber& operator=(const QuaternaryNumber& other) {
        if (this != &other) {
            for (int i = 0; i < MAX_DIGITS; ++i) {
                digits[i] = other.digits[i];
            }
            isNegative = other.isNegative;
        }
        return *this;
    }

    // Операции сложения и вычитания
   QuaternaryNumber operator+(const QuaternaryNumber& other) const {
    QuaternaryNumber result;
    int carry = 0;

    // Определяем знак результата
    if (isNegative && !other.isNegative) {
        if (*this >>= other) {
            result.isNegative = true;
        } else {
            result.isNegative = false;
        }
    } else if (!isNegative && other.isNegative) {
        if (*this >>= other) {
            result.isNegative = false;
        } else {
            result.isNegative = true;
        }
    } else if (isNegative && other.isNegative) {
        result.isNegative = true;
    } else {
        result.isNegative = false;
    }
    bool isZero = true;
    // Складываем числа по модулю
    for (int i = 0; i < MAX_DIGITS; ++i) {
        int sum;

        if (isNegative && !other.isNegative) {
            if (*this >>= other) {
                sum = digits[i] - other.digits[i] - carry;
            }
            else {
            sum = other.digits[i] - digits[i] - carry;
            }
        }
        else if (!isNegative && other.isNegative) {
            if (*this >>= other) {
                sum = digits[i] - other.digits[i] - carry;
            }
            else {
            sum = other.digits[i] - digits[i] - carry;
            }
        }
        else if (isNegative && other.isNegative) {
            sum = other.digits[i] + digits[i] + carry;
        }
        else {
            sum = digits[i] + other.digits[i] + carry;
        }

        if (sum < 0) {
            sum += 4;
            carry = 1;
        }
        else if (sum > 3) {
            carry = 1;
        }
        else {
            carry = 0;
        }
        if (sum % 4 != 0) {
            isZero = false;
        }
        result.digits[i] = sum % 4;
    }
    if (carry > 0) {
        for (int i = MAX_DIGITS - 1; i >= 0; --i) {
            if (result.digits[i] != 0) {
                result.digits[i - 1] += carry;
                carry = 0;
                break;
            }
        }
    }
    if (isZero) {
        result.isNegative = false;
    }
    return result;
}


    QuaternaryNumber operator-(const QuaternaryNumber& other) const {
     QuaternaryNumber result;
     int carry = 0;

     // Определяем знак результата
     if (isNegative && other.isNegative) {
         if (*this >>= other) {
             result.isNegative = true;
         } else {
             result.isNegative = false;
         }
     } else if (!isNegative && !other.isNegative) {
         if (*this >>= other) {
             result.isNegative = false;
         } else {
             result.isNegative = true;
         }
     } else if (isNegative && !other.isNegative) {
         result.isNegative = true;
     } else {
         result.isNegative = false;
     }
     bool isZero = true;
     // Складываем числа по модулю
     for (int i = 0; i < MAX_DIGITS; ++i) {
         int sum;

         if (isNegative && other.isNegative) {
             if (*this >>= other) {
                 sum = digits[i] - other.digits[i] - carry;
             }
             else {
             sum = other.digits[i] - digits[i] - carry;
             }
         }
         else if (!isNegative && !other.isNegative) {
             if (*this >>= other) {
                 sum = digits[i] - other.digits[i] - carry;
             }
             else {
             sum = other.digits[i] - digits[i] - carry;
             }
         }
         else if (isNegative && !other.isNegative) {
             sum = other.digits[i] + digits[i] + carry;
         }
         else {
             sum = digits[i] + other.digits[i] + carry;
         }

         if (sum < 0) {
             sum += 4;
             carry = 1;
         }
         else if (sum > 3) {
             carry = 1;
         }
         else {
             carry = 0;
         }
         if (sum % 4 != 0) {
             isZero = false;
         }
         result.digits[i] = sum % 4;
     }
     if (carry > 0) {
         for (int i = MAX_DIGITS - 1; i >= 0; --i) {
             if (result.digits[i] != 0) {
                 result.digits[i - 1] += carry;
                 carry = 0;
                 break;
             }
         }
     }
    if (isZero) {
        result.isNegative = false;
    }
     return result;
 }


    // Другие операции
    QuaternaryNumber operator<<(int shift) const {
        QuaternaryNumber result = *this;
        for (int i = 0; i < shift; ++i) {
            for (int j = MAX_DIGITS - 1; j > 0; --j) {
                result.digits[j] = result.digits[j - 1];
            }
            result.digits[0] = 0;
        }
        return result;
    }

    QuaternaryNumber operator>>(int shift) const {
        QuaternaryNumber result = *this;
        for (int i = 0; i < shift; ++i) {
            for (int j = 0; j < MAX_DIGITS - 1; ++j) {
                result.digits[j] = result.digits[j + 1];
            }
            result.digits[MAX_DIGITS - 1] = 0;
        }
        return result;
    }

    QuaternaryNumber operator%(const QuaternaryNumber& divisor) const {
        QuaternaryNumber dividend = *this;
        if (dividend.isNegative == divisor.isNegative) {
            while (dividend >= divisor) {
                dividend = dividend - divisor;
            }
        }
        else if (dividend.isNegative) {
            while (dividend.isNegative) {
                dividend = dividend + divisor;
            }
        }
        else {
            while (!dividend.isNegative) {
                dividend = dividend + divisor;
            }
        }
        return dividend;
    }

    // Вспомогательные функции
    bool isEqual(const QuaternaryNumber& other) const {
        if (isNegative != other.isNegative) {
            return false;
        }
        for (int i = 0; i < MAX_DIGITS; ++i) {
            if (digits[i] != other.digits[i]) {
                return false;
            }
        }
        return true;
    }
    
    bool isEqual_abs(const QuaternaryNumber& other) const {
            for (int i = 0; i < MAX_DIGITS; ++i) {
                if (digits[i] != other.digits[i]) {
                    return false;
                }
            }
            return true;
        }

    bool isGreaterThan_abs(const QuaternaryNumber& other) const {
            for (int i = MAX_DIGITS - 1; i >= 0; --i) {
                if (digits[i] > other.digits[i]) {
                    return true;
                } else if (digits[i] < other.digits[i]) {
                    return false;
                }
            }
            return false;
        }
    
    bool isGreaterThan(const QuaternaryNumber& other) const {
        if (isNegative and !other.isNegative) {
            return false;
        }
        else if (!isNegative and other.isNegative) {
            return true;
        }
        else if (!isNegative and !other.isNegative) {
            for (int i = MAX_DIGITS - 1; i >= 0; --i) {
                if (digits[i] > other.digits[i]) {
                    return true;
                } else if (digits[i] < other.digits[i]) {
                    return false;
                }
            }
            return false;
        }
        else {
            for (int i = MAX_DIGITS - 1; i >= 0; --i) {
                if (digits[i] > other.digits[i]) {
                    return false;
                } else if (digits[i] < other.digits[i]) {
                    return true;
                }
            }
            return true;
        }
    }
    
    bool isLessThan_abs(const QuaternaryNumber& other) const {
        return !isEqual_abs(other) && !isGreaterThan_abs(other);
    }
    
    bool isLessThan(const QuaternaryNumber& other) const {
        return !isEqual(other) && !isGreaterThan(other);
    }

    // Вывод числа
    void print() const {
        if (isNegative) {
            cout << '-';
        }

        for (int i = MAX_DIGITS - 1; i >= 0; --i) {
            cout << digits[i];
        }

        cout << endl;
    }
};

int main() {
    int x, y;
    cout << "Введите первое число" << endl;
    cin >> x;
    cout << "Введите второе число" << endl;
    cin >> y;
    QuaternaryNumber a(x);
    QuaternaryNumber b(y);
    if (a.isValid and b.isValid){
        QuaternaryNumber sum = a + b;
        QuaternaryNumber diff = a - b;
        QuaternaryNumber leftShift = a << 2;
        QuaternaryNumber rightShift = a >> 1;
        QuaternaryNumber modulo = a % b;
        
        // Проверка операторов сравнения
        bool isEqual = (a == b);
        bool isNotEqual = (a != b);
        bool isLessThan = (a < b);
        bool isGreaterThan = (a > b);
        bool isLessOrEqual = (a <= b);
        bool isGreaterOrEqual = (a >= b);
        
        // Проверка оператора присваивания
        QuaternaryNumber assigned = a;
        
        cout << "a: ";
        a.print();
        
        cout << "b: ";
        b.print();
        
        cout << "a + b: ";
        sum.print();
        
        cout << "a - b: ";
        diff.print();
        
        cout << "a << 2: ";
        leftShift.print();
        
        cout << "a >> 1: ";
        rightShift.print();
        
        cout << "a % b: ";
        modulo.print();
        
        cout << "a == b: " << boolalpha << isEqual << endl;
        cout << "a != b: " << boolalpha << isNotEqual << endl;
        cout << "a < b: " << boolalpha << isLessThan << endl;
        cout << "a > b: " << boolalpha << isGreaterThan << endl;
        cout << "a <= b: " << boolalpha << isLessOrEqual << endl;
        cout << "a >= b: " << boolalpha << isGreaterOrEqual << endl;
        
        cout << "Assigned: ";
        assigned.print();
    }
    return 0;
}
