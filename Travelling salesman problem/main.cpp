#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

using namespace std;

const double INF = numeric_limits<double>::infinity();

class Ant {
public:
    vector<int> tour;
    vector<bool> visited;
    double tourLength;

    Ant(int numRivers) : tourLength(0) {
        tour.resize(numRivers);
        visited.resize(numRivers, false);
        for (int i = 0; i < numRivers; ++i) {
            tour[i] = i;
        }
        for (int i = tour.size() - 1; i > 0; --i) {
            int j = rand() % (i + 1);
            swap(tour[i], tour[j]);
        }
    }

    void reset() {
        fill(visited.begin(), visited.end(), false);
        tourLength = 0;
    }
};

class Graph {
public:
    vector<vector<double>> timeMatrix;

    Graph(int numRivers) {
        timeMatrix.resize(numRivers, vector<double>(numRivers));
        for (int i = 0; i < numRivers; ++i) {
            for (int j = 0; j < numRivers; ++j) {
                if (i != j) {
                    // Задаем случайные значения времени перемещения
                    timeMatrix[i][j] = rand() % 20 + 1; // Для примера, случайные значения от 1 до 20
                }
            }
        }
    }
};

class GraphClass {
public:
    vector<Graph> graphs;

    GraphClass(int numGraphs, int numRivers) {
        graphs.resize(numGraphs, Graph(numRivers));
    }
};

class ACO {
public:
    ACO(int numRivers, double alpha, double beta, int numAnts, const vector<vector<double>>& timeMatrix)
        : numRivers(numRivers), alpha(alpha), beta(beta), numAnts(numAnts), timeMatrix(timeMatrix),
          pheromones(numRivers, vector<double>(numRivers, 1.0 / numRivers)),
          visibility(numRivers, vector<double>(numRivers, 1.0 / numRivers)), ants(numAnts), bestTourLength(INF) {}

    void runACO(int maxIterations);

    double getDeviation(const vector<int>& tour);
    double getMaxDeviation();

private:
    int numRivers;
    double alpha;
    double beta;
    int numAnts;
    const vector<vector<double>>& timeMatrix;
    vector<vector<double>> pheromones;
    vector<vector<double>> visibility;
    vector<vector<int>> ants;
    vector<int> bestTour;
    double bestTourLength;

    void initializeAnts();
    void antTourConstruction();
    void updatePheromones();
    void updateBestTour();
    double calculateTourLength(const vector<int>& tour);
    int selectNextRiver(int ant, const vector<bool>& visited, int currentRiver);
};

void ACO::initializeAnts() {
    for (auto& ant : ants) {
        ant.resize(numRivers);
        for (int i = 0; i < numRivers; ++i) {
            ant[i] = i;
        }
        random_shuffle(ant.begin() + 1, ant.end());
    }
}

void ACO::antTourConstruction() {
    for (auto& ant : ants) {
        vector<bool> visited(numRivers, false);
        ant[0] = rand() % numRivers;
        visited[ant[0]] = true;

        for (size_t j = 1; j < ant.size(); ++j) {
            ant[j] = selectNextRiver(ant[j], visited, ant[j - 1]);
            visited[ant[j]] = true;
        }
    }
}

int ACO::selectNextRiver(int ant, const vector<bool>& visited, int currentRiver) {
    double totalProbability = 0.0;

    for (int i = 0; i < numRivers; ++i) {
        if (!visited[i]) {
            totalProbability += pow(pheromones[currentRiver][i], alpha) * pow(visibility[currentRiver][i], beta);
        }
    }

    double randomValue = static_cast<double>(rand()) / RAND_MAX;
    double cumulativeProbability = 0.0;

    for (int i = 0; i < numRivers; ++i) {
        if (!visited[i]) {
            double probability = pow(pheromones[currentRiver][i], alpha) * pow(visibility[currentRiver][i], beta) / totalProbability;

            cumulativeProbability += probability;

            if (randomValue <= cumulativeProbability) {
                return i;
            }
        }
    }

    // Если все реки посещены, возвращаем текущую реку
    return currentRiver;
}

void ACO::runACO(int maxIterations) {
    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        initializeAnts();
        antTourConstruction();
        updatePheromones();
        updateBestTour();
    }

    cout << "Лучший маршрут: ";
    for (int river : bestTour) {
        cout << river << " ";
    }
    cout << endl << "Длина маршрута: " << bestTourLength << endl;

    double maxDeviation = getMaxDeviation();
    cout << "Максимальное отклонение: " << maxDeviation << endl;
}

double ACO::calculateTourLength(const vector<int>& tour) {
    double length = 0.0;

    for (int i = 0; i < numRivers - 1; ++i) {
        length += timeMatrix[tour[i]][tour[i + 1]];
    }

    length += timeMatrix[tour.back()][tour.front()]; // Возвращение в начальный город

    return length;
}

double ACO::getDeviation(const vector<int>& tour) {
    double tourLength = calculateTourLength(tour);
    return abs(tourLength - bestTourLength);
}

double ACO::getMaxDeviation() {
    double maxDeviation = 0.0;

    for (auto& ant : ants) {
        double deviation = getDeviation(ant);
        maxDeviation = max(maxDeviation, deviation);
    }

    return maxDeviation;
}

void ACO::updatePheromones() {
    constexpr double evaporationRate = 0.5;

    for (int i = 0; i < numRivers; ++i) {
        for (int j = 0; j < numRivers; ++j) {
            pheromones[i][j] *= (1.0 - evaporationRate);

            for (int k = 0; k < numAnts; ++k) {
                double pheromoneChange = 0.0;

                if (find(ants[k].begin(), ants[k].end(), i) != ants[k].end() &&
                    find(ants[k].begin(), ants[k].end(), j) != ants[k].end()) {
                    pheromoneChange = 1.0 / calculateTourLength(ants[k]);
                }

                pheromones[i][j] += pheromoneChange;
                pheromones[j][i] = pheromones[i][j]; // симметрично
            }
        }
    }
}

void ACO::updateBestTour() {
    for (auto& ant : ants) {
        double tourLength = calculateTourLength(ant);
        if (tourLength < bestTourLength) {
            bestTourLength = tourLength;
            bestTour = ant;
        }
    }
}


class BruteForceRiver {
public:
    BruteForceRiver(const vector<vector<double>>& timeMatrix)
        : timeMatrix(timeMatrix), numRivers(timeMatrix.size()) {}

    void solve() {
        vector<int> rivers(numRivers);
        for (int i = 0; i < numRivers; ++i) {
            rivers[i] = i;
        }

        double minTime = INF;
        vector<int> bestTour;

        do {
            double currentTime = calculateTourTime(rivers);
            if (currentTime < minTime) {
                minTime = currentTime;
                bestTour = rivers;
            }
        } while (next_permutation(rivers.begin() + 1, rivers.end()));

        cout << "Лучший маршрут: ";
        for (int river : bestTour) {
            cout << river << " ";
        }
        cout << endl << "Суммарное время: " << minTime << endl;
    }

private:
    const vector<vector<double>>& timeMatrix;
    const int numRivers;

    double calculateTourTime(const vector<int>& tour) const {
        double time = 0.0;
        for (int i = 0; i < numRivers - 1; ++i) {
            time += timeMatrix[tour[i]][tour[i + 1]];
        }
        time += timeMatrix[tour.back()][tour.front()]; // Return to the starting river
        return time;
    }
};

int main() {
    cout << "Какой алгоритм использовать?" << endl;
    cout << "1 -  полного перебора, 2 - муравьиный алгоритм, 3 - муравьиный алгоритм с графом из случайных значений" << endl;
    int n;
    cin >> n;
    if (n == 2) {
        srand(static_cast<unsigned>(time(0)));
    
        // Граф
        vector<vector<double>> timeMatrix = {
            // {0, 2, 5, 10},
            // {1, 10, 6, 4},
            // {15, 7, 7, 8},
            // {6, 32, 0, 4}
            {100, 88, 78, 101},
            {90, 79, 69, 56},
            {115, 70, 105, 88},
            {64, 67, 22, 99}
        };
    
        // Параметры муравьиного алгоритма
        int numRivers = timeMatrix.size();
        double alpha = 1.0;
        double beta = 2.0;
        int numAnts = 20;
        int maxIterations = 100;
    
        // Создание объекта ACO и запуск алгоритма
        ACO aco(numRivers, alpha, beta, numAnts, timeMatrix);
        aco.runACO(maxIterations);
    }
    else if (n == 3) {
        srand(static_cast<unsigned>(time(0)));

        // Пример: создание класса данных с тремя графами
        GraphClass graphClass(3, 10);

        // Параметры: количество рек, alpha, beta, количество муравьев
        ACO aco(graphClass.graphs[0].timeMatrix.size(), 1.0, 2.0, 20, graphClass.graphs[0].timeMatrix);
        aco.runACO(10); // Произведем 10 запусков для каждого графа
    }
    else {
        // Пример: матрица времени перемещения между реками
    vector<vector<double>> timeMatrix = {
        // {0, 2, 9, 10},
        // {1, 0, 6, 4},
        // {15, 7, 0, 8},
        // {6, 3, 12, 0}
        {100, 88, 78, 101},
        {90, 79, 69, 56},
        {115, 70, 105, 88},
        {64, 67, 22, 99}
    };

    BruteForceRiver bruteForceRiver(timeMatrix);
    bruteForceRiver.solve();
    }
    return 0;
}


//#include <iostream>
//#include <vector>
//#include <limits>
//#include <cstdlib>
//#include <ctime>
//#include <algorithm>
//#include <cmath>
//
//using namespace std;
//
//const double INF = numeric_limits<double>::infinity();
//
//class Ant {
//public:
//    vector<int> tour;
//    vector<bool> visited;
//    double tourLength;
//
//    Ant(int numRivers) : tourLength(0) {
//        tour.resize(numRivers);
//        visited.resize(numRivers, false);
//        for (int i = 0; i < numRivers; ++i) {
//            tour[i] = i;
//        }
//        for (int i = tour.size() - 1; i > 0; --i) {
//            int j = rand() % (i + 1);
//            swap(tour[i], tour[j]);
//        }
//    }
//
//    void reset() {
//        fill(visited.begin(), visited.end(), false);
//        tourLength = 0;
//    }
//};
//
//class Graph {
//public:
//    vector<vector<double>> timeMatrix;
//
//    Graph(int numRivers) {
//        timeMatrix.resize(numRivers, vector<double>(numRivers));
//        for (int i = 0; i < numRivers; ++i) {
//            for (int j = 0; j < numRivers; ++j) {
//                if (i != j) {
//                    // Задаем случайные значения времени перемещения
//                    timeMatrix[i][j] = rand() % 20 + 1; // Случайные значения от 1 до 20
//                }
//            }
//        }
//    }
//};
//
//class GraphClass {
//public:
//    vector<Graph> graphs;
//
//    GraphClass(int numGraphs, int numRivers) {
//        graphs.resize(numGraphs, Graph(numRivers));
//    }
//};
//
//class ACO {
//public:
//    ACO(int numRivers, double alpha, double beta, int numAnts, const vector<vector<double>>& timeMatrix)
//        : numRivers(numRivers), alpha(alpha), beta(beta), numAnts(numAnts), timeMatrix(timeMatrix),
//          pheromones(numRivers, vector<double>(numRivers, 1.0 / numRivers)),
//          visibility(numRivers, vector<double>(numRivers, 1.0 / numRivers)), ants(numAnts), bestTourLength(INF) {}
//
//    void runACO(int maxIterations);
//
//    double getDeviation(const vector<int>& tour);
//    double getMaxDeviation();
//
//private:
//    int numRivers;
//    double alpha;
//    double beta;
//    int numAnts;
//    const vector<vector<double>>& timeMatrix;
//    vector<vector<double>> pheromones;
//    vector<vector<double>> visibility;
//    vector<vector<int>> ants;
//    vector<int> bestTour;
//    double bestTourLength;
//
//    void initializeAnts();
//    void antTourConstruction();
//    void updatePheromones();
//    void updateBestTour();
//    double calculateTourLength(const vector<int>& tour);
//    int selectNextRiver(int ant, const vector<bool>& visited, int currentRiver);
//};
//
//void ACO::initializeAnts() {
//    for (auto& ant : ants) {
//        ant.resize(numRivers);
//        for (int i = 0; i < numRivers; ++i) {
//            ant[i] = i;
//        }
//        random_shuffle(ant.begin() + 1, ant.end());
//    }
//}
//
//void ACO::antTourConstruction() {
//    for (auto& ant : ants) {
//        vector<bool> visited(numRivers, false);
//        ant[0] = rand() % numRivers;
//        visited[ant[0]] = true;
//
//        for (size_t j = 1; j < ant.size(); ++j) {
//            ant[j] = selectNextRiver(ant[j], visited, ant[j - 1]);
//            visited[ant[j]] = true;
//        }
//    }
//}
//
//int ACO::selectNextRiver(int ant, const vector<bool>& visited, int currentRiver) {
//    double totalProbability = 0.0;
//
//    for (int i = 0; i < numRivers; ++i) {
//        if (!visited[i]) {
//            totalProbability += pow(pheromones[currentRiver][i], alpha) * pow(visibility[currentRiver][i], beta);
//        }
//    }
//
//    double randomValue = static_cast<double>(rand()) / RAND_MAX;
//    double cumulativeProbability = 0.0;
//
//    for (int i = 0; i < numRivers; ++i) {
//        if (!visited[i]) {
//            double probability = pow(pheromones[currentRiver][i], alpha) * pow(visibility[currentRiver][i], beta) / totalProbability;
//
//            cumulativeProbability += probability;
//
//            if (randomValue <= cumulativeProbability) {
//                return i;
//            }
//        }
//    }
//
//    // Если все реки посещены, возвращаем текущую реку
//    return currentRiver;
//}
//
//void ACO::runACO(int maxIterations) {
//    for (int iteration = 0; iteration < maxIterations; ++iteration) {
//        initializeAnts();
//        antTourConstruction();
//        updatePheromones();
//        updateBestTour();
//    }
//
//    cout << "Лучший маршрут: ";
//    for (int river : bestTour) {
//        cout << river << " ";
//    }
//    cout << endl << "Длина маршрута: " << bestTourLength << endl;
//}
//
//double ACO::calculateTourLength(const vector<int>& tour) {
//    double length = 0.0;
//
//    for (int i = 0; i < numRivers - 1; ++i) {
//        length += timeMatrix[tour[i]][tour[i + 1]];
//    }
//
//    length += timeMatrix[tour.back()][tour.front()]; // Возвращение в начальную реку
//
//    return length;
//}
//
//void ACO::updatePheromones() {
//    constexpr double evaporationRate = 0.5;
//
//    for (int i = 0; i < numRivers; ++i) {
//        for (int j = 0; j < numRivers; ++j) {
//            pheromones[i][j] *= (1.0 - evaporationRate);
//
//            for (int k = 0; k < numAnts; ++k) {
//                double pheromoneChange = 0.0;
//
//                if (find(ants[k].begin(), ants[k].end(), i) != ants[k].end() &&
//                    find(ants[k].begin(), ants[k].end(), j) != ants[k].end()) {
//                    pheromoneChange = 1.0 / calculateTourLength(ants[k]);
//                }
//
//                pheromones[i][j] += pheromoneChange;
//                pheromones[j][i] = pheromones[i][j];
//            }
//        }
//    }
//}
//
//void ACO::updateBestTour() {
//    for (auto& ant : ants) {
//        double tourLength = calculateTourLength(ant);
//        if (tourLength < bestTourLength) {
//            bestTourLength = tourLength;
//            bestTour = ant;
//        }
//    }
//}
//
//
//class BruteForceRiver {
//public:
//    BruteForceRiver(const vector<vector<double>>& timeMatrix)
//        : timeMatrix(timeMatrix), numRivers(timeMatrix.size()) {}
//
//    void solve() {
//        vector<int> rivers(numRivers);
//        for (int i = 0; i < numRivers; ++i) {
//            rivers[i] = i;
//        }
//
//        double minTime = INF;
//        vector<int> bestTour;
//
//        do {
//            double currentTime = calculateTourTime(rivers);
//            if (currentTime < minTime) {
//                minTime = currentTime;
//                bestTour = rivers;
//            }
//        } while (next_permutation(rivers.begin() + 1, rivers.end()));
//
//        cout << "Лучший маршрут: ";
//        for (int river : bestTour) {
//            cout << river << " ";
//        }
//        cout << endl << "Суммарное время: " << minTime << endl;
//    }
//
//private:
//    const vector<vector<double>>& timeMatrix;
//    const int numRivers;
//
//    double calculateTourTime(const vector<int>& tour) const {
//        double time = 0.0;
//        for (int i = 0; i < numRivers - 1; ++i) {
//            time += timeMatrix[tour[i]][tour[i + 1]];
//        }
//        time += timeMatrix[tour.back()][tour.front()];
//        return time;
//    }
//};
//
//int main() {
//    cout << "Какой алгоритм использовать?" << endl;
//    cout << "1 -  полного перебора, 2 - муравьиный алгоритм, 3 - муравьиный алгоритм с графом из случайных значений" << endl;
//    int n;
//    cin >> n;
//    if (n == 2) {
//        srand(static_cast<unsigned>(time(0)));
//    
//        // Граф
//        vector<vector<double>> timeMatrix = {
//        {100, 88, 78, 101},
//        {90, 79, 69, 56},
//        {115, 70, 105, 88},
//        {64, 67, 22, 99}
//    };
//    
//        int numRivers = timeMatrix.size();
//        double alpha = 1.0;
//        double beta = 2.0;
//        int numAnts = 20;
//        int maxIterations = 100;
//    
//        ACO aco(numRivers, alpha, beta, numAnts, timeMatrix);
//        aco.runACO(maxIterations);
//    }
//    else if (n == 3) {
//        srand(static_cast<unsigned>(time(0)));
//
//        GraphClass graphClass(3, 10);
//
//        ACO aco(graphClass.graphs[0].timeMatrix.size(), 1.0, 2.0, 20, graphClass.graphs[0].timeMatrix);
//        aco.runACO(10);
//    }
//    else {
//    vector<vector<double>> timeMatrix = {
//        // {0, 2, 9, 10},
//        // {1, 0, 6, 4},
//        // {15, 7, 0, 8},
//        // {6, 3, 12, 0}
//        {100, 88, 78, 101},
//        {90, 79, 69, 56},
//        {115, 70, 105, 88},
//        {64, 67, 22, 99}
//    };
//
//    BruteForceRiver bruteForceRiver(timeMatrix);
//    bruteForceRiver.solve();
//    }
//    return 0;
//}
