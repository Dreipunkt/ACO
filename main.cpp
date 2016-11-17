#include <iostream>
#include <fstream>
#include <cmath>
#include <random>

using namespace std;

struct Solution {
    int* path;
    double length;
};

class Ant {

private:
    int pos;
    double** matrix;
    double** pheroMap;
    int n;
    double alpha = 0.5;     // Gewichtung von Pheromonen (groesser-gleich 0)
    double beta = 1;        // Gewichtung der Entfernung (groesser-gleich 1)

public:

    Ant(){}

    ~Ant() {}

    void setup(int start, double** matrix, double** pheroMap, int n) {
        Ant::pos = start;
        Ant::matrix = matrix;
        Ant::pheroMap = pheroMap;
        Ant::n = n;
    }

    Solution findTarget() {
        int anzNodes = 1;
        Solution s;
        s.path = new int[anzNodes];
        s.path[anzNodes - 1] = pos;
        s.length = 0;

        double* prob = new double[n];

        double aSum = allowedSum();

        for (int i = 0; i < n; i++) {
            if (pos == i) prob[i] = 0;
            else if (matrix[pos][i] == -1) prob[i] = 0;
            else {
                prob[i] = (pow(pheroMap[pos][i], alpha)*(pow(1.0/matrix[pos][i], beta)))/aSum;
            }
        }

        cout << prob[2];
    }

    double allowedSum() {
        double sum = 0;

        for (int i = 0; i < n; i++) {
            if (matrix[pos][i] >= 0) {
                sum += pow(pheroMap[pos][i], alpha)*(pow(1.0/matrix[pos][i], beta));
            }

        }

        return sum;
    }

};

class ACO {

private:

    int n;
    double** matrix;
    double** pheroMap;
    int start;
    int ziel;
    int iter;
    int antnum;
    Solution bestSolution;
    Ant* ants;

    void initAnts() {
        ACO::ants = new Ant[antnum];
        for (int i = 0; i < antnum; i++) {
            ants[i].setup(start, matrix, pheroMap, n);
        }
    }

    void antsRun() {
        Solution* s = new Solution[antnum];
        for (int i = 0; i < antnum; i++) {
            s[i] = ants[i].findTarget();
        }
    }

public:

    // anzahl der knoten, matrix, startknoten, zielknoten, anzahl der iterationen, anzahl der ameisen
    ACO(int n, double** matrix, int start, int ziel, int iter, int antnum = 0) {
        ACO::n = n;
        ACO::matrix = matrix;
        ACO::start = start;
        ACO::ziel = ziel;
        ACO::iter = iter;
        if (antnum < 1) {
            ACO::antnum = n;
        }
        else {
            ACO::antnum = antnum;
        }

        pheroMap = new double*[n];
        for (int i = 0; i < n; i++) {
            pheroMap[i] = new double[n];
            for (int j = 0; j < n; j++) {
                pheroMap[i][j] = 0.1;
            }
        }
    }

    ~ACO() {}

    Solution run() {
        for (int i = 0; i < iter; i++) {
            initAnts();
            antsRun();
        }

        return bestSolution;
    }

};

struct Grid {
    int n;
    double** matrix;
};

void showHelp() {
    cout << "USAGE:" << endl;
}

Grid readMatrix(char* filename) {
    ifstream file(filename);

    int n;
    file >> n;

    double** matrix = new double*[n];

    for (int i = 0; i < n; i++) {
        matrix[i] = new double[n];
        for (int j = 0; j < n; j++) {
            file >> matrix[i][j];
        }
    }

    Grid grid;
    grid.n = n;
    grid.matrix = matrix;
    return grid;
}

int main(int argc, char* argv[]) {

    int start = 0;
    int ende = 1;

    if (argc < 2) {
        showHelp();
        return 0;
    }
    else {
        Grid g = readMatrix(argv[1]);
        ACO aco(g.n, g.matrix, start, ende, 10);
        aco.run();
    }
}