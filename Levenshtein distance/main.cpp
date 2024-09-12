#include <iostream>
#include <string>
#include <cmath>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
using namespace std;

int minimum(int a, int b, int c) {
    return min(min(a, b), c);
}

void replace(int a[], int b[], int size) {
    for (int i = 0; i < size; i++) {
            a[i] = b[i];
        }
}

string generateRandomString_en(int length) {
    string randomString;
    const string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    const int charsetSize = charset.size();
    
    srand(time(nullptr));

    for (int i = 0; i < length; ++i) {
        randomString += charset[rand() % charsetSize];
    }
    return randomString;
}

string generateRandomString_rus(int length) {
    string randomString;
    const string charset = "абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ1234567890";
    const int charsetSize = charset.size();
    
    srand(time(nullptr));

    for (int i = 0; i < length; ++i) {
        randomString += charset[rand() % charsetSize];
    }
    return randomString;
}

string makeStringsEqualLength(string s1, string s2, int l1, int l2, int k) {
    if (k == 1) { //добавляем пробелы в первую строку
        string spaces(l2 - l1, ' ');
        return s1 + spaces;
    }
    else {
        string spaces(l1 - l2, ' ');
        return s2 + spaces;
    }
}


double getCPUTime() {
#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
    /* Prefer high-res POSIX timers, when available. */
    {
        clockid_t id;
        struct timespec ts;
#if _POSIX_CPUTIME > 0
        /* Clock ids vary by OS.  Query the id, if possible. */
        if (clock_getcpuclockid(0, &id) == -1)
#endif
#if defined(CLOCK_PROCESS_CPUTIME_ID)
            /* Use known clock id for AIX, Linux, or Solaris. */
            id = CLOCK_PROCESS_CPUTIME_ID;
#elif defined(CLOCK_VIRTUAL)
        /* Use known clock id for BSD or HP-UX. */
        id = CLOCK_VIRTUAL;
#else
        id = (clockid_t) - 1;
#endif
        if (id != (clockid_t) - 1 && clock_gettime(id, &ts) != -1)
            return (double) ts.tv_sec + (double) ts.tv_nsec / 1000000000.0;
    }
#endif

#if defined(RUSAGE_SELF)
    {
        struct rusage rusage;
        if (getrusage(RUSAGE_SELF, &rusage) != -1)
            return (double) rusage.ru_utime.tv_sec + (double) rusage.ru_utime.tv_usec / 1000000.0;
    }
#endif

#if defined(_SC_CLK_TCK)
    {
        const double ticks = (double) sysconf(_SC_CLK_TCK);
        struct tms tms;
        if (times(&tms) != (clock_t) - 1)
            return (double) tms.tms_utime / ticks;
    }
#endif

#if defined(CLOCKS_PER_SEC)
    {
        clock_t cl = clock();
        if (cl != (clock_t) - 1)
            return (double) cl / (double) CLOCKS_PER_SEC;
    }
#endif

    
    return -1;        /* Failed. */
}

// Вариант 1

int LevNoRecMatr(string s1, string s2, int l1, int l2) {
    int ans[l1 + 1][l2 + 1];
    for (int i = 0; i <= l1; i ++) {
        for (int j = 0; j <= l2; j ++) {
            if (i == 0 and j == 0) {
                ans[i][j] = 0;
            }
            else if (i == 0) {
                ans[i][j] = j;
            }
            else if (j == 0) {
                ans[i][j] = i;
            }
        }
    }
    for (int i = 1; i <= l1; i ++) {
        for (int j = 1; j <= l2; j ++) {
            int num = 0;
            if (s1[i - 1] != s2[j - 1]) {
                num = 1;
            }
            ans[i][j] = minimum(ans[i - 1][j] + 1, ans[i][j - 1] + 1, ans[i - 1][j - 1] + num);
        }
    }
    return ans[l1][l2];
}

// Вариант 6
int DLevNoRec3str(string s1, string s2, int l1, int l2) {
    s1 = " " + s1;
    s2 = " " + s2;
    int second_row[l2 + 1];
    for (int i = 0; i <= l2; i++) {
        second_row[i] = i;
    }
    int first_row[l2 + 1];
    first_row[0] = 1;
    for (int i = 1; i <= l2; i++) {
        int num = 0;
        if (s1[1] != s2[i]) {
            num = 1;
        }
        first_row[i] = minimum(second_row[i] + 1, first_row[i - 1] + 1, second_row[i - 1] + num);
    }
    int third_row[l2 + 1];
    for (int i = 2; i <= l1; i++) {
        replace(third_row, second_row, l2 + 1);
        replace(second_row, first_row, l2 + 1);
        first_row[0] = i;
        for (int j = 1; j <= l2; j++) {
            int num = 0;
            if (s1[i] != s2[j]) {
                num = 1;
            }
            first_row[j] = minimum(first_row[j - 1] + 1, second_row[j] + 1, second_row[j - 1] + num);
            if (s1[i] == s2[j - 1] and s1[i - 1] == s2[j]) {
                first_row[j] = min(first_row[j], third_row[j - 2] + 1);
            }
        }
    }
    return first_row[l2];
}
//Вариант 3
int LevRecNoCash(string s1, string s2, int l1, int l2) {
    if (l1 == 0 or l2 == 0) {
        return l1 + l2;
    }
    else {
        int num = 0;
        if (s1[l1 - 1] != s2[l2 - 1]) {
            num = 1;
        }
        return minimum(LevRecNoCash(s1, s2, l1 - 1, l2) + 1, LevRecNoCash(s1, s2, l1, l2 - 1) + 1, LevRecNoCash(s1, s2, l1 - 1, l2 - 1) + num);
    }
}

//Основная функция

int main(int argc, const char * argv[]) {
    setlocale(LC_ALL, "");
    string s1, s2;
    cout << "Выберите способ задания строк" << endl;
    cout << "1 - генерация произвольных строк автоматически, 2 - вручную" << endl;
    int z, lang;
    cin >> z;
    cout << "Выберите язык строк" << endl;
    cout << "1 - русский, 2 - английский" << endl;
    cin >> lang;
    if (z == 1) {
        int d, e;
        cout << "Напишите длину первой строки" << endl;
        cin >> d;
        cout << "Напишите длину второй строки" << endl;
        cin >> e;
        if (lang == 1) {
            s1 = generateRandomString_rus(d);
            s2 = generateRandomString_rus(e);
        }
        else {
            s1 = generateRandomString_en(d);
            s2 = generateRandomString_en(e);
        }
    }
    else {
        cout << "Введите две строки" << endl;
        cin >> s1 >> s2;
    }
    double startTime, endTime;
    cout << "Выберите алгоритм:" << endl;
    cout << "1 - Левенштейн матрично, нерекурсивно, 2 - Левенштейн рекурсивно без кэша, 3 - Дамерау-Левенштейн 3 строки" << endl;
    int ans;
    cin >> z;
    int l1, l2;
    l1 = s1.size();
    l2 = s2.size();
    if (lang == 1) {
        l1 /= 2;
        l2 /= 2; //тк русский символ занимает 2 бита
    }
    if (l1 > l2) {
        s1 = makeStringsEqualLength(s1, s2, l1, l2, 1);
    }
    else if (l2 > l1) {
        s2 = makeStringsEqualLength(s1, s2, l1, l2, 2);
    }
    startTime = getCPUTime( );
    if (z == 1) {
        ans = LevNoRecMatr(s1, s2, l1, l2);
    }
    else if (z == 2) {
        ans = LevRecNoCash(s1, s2, l1, l2);
    }
    else {
        ans = DLevNoRec3str(s1, s2, l1, l2);
    }
    endTime = getCPUTime( );
    cout << "Расстояние между строками равно ";
    cout << ans << endl;
    fprintf( stderr, "Использовано процессорного времени  %lf\n", (endTime - startTime));
}
