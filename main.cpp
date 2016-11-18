#include <iostream>
#include <fstream>
#include <cmath>
#include <random>

using namespace std;

// Hilfskontrukt zur Speicherung und Weiterverarbeitung von Lösungen
struct Solution {
    int* path;
    int anzNodes;
    double length;
};

class Ant {

private:
    int pos;
    double** matrix;
    double** pheroMap;
    int n;
    int ziel;
    double alpha = 0.5;     // Gewichtung von Pheromonen (groesser-gleich 0)
    double beta = 1;        // Gewichtung der Entfernung (groesser-gleich 1)

public:

    Ant(){}

    ~Ant() {}

    // initialisierung nach Kontruktor, da durch dynamisches Array Kontruktoraufruf mit Parametern nicht möglich
    void setup(int start, double** matrix, double** pheroMap, int n, int ziel) {
        Ant::pos = start;
        Ant::matrix = matrix;
        Ant::pheroMap = pheroMap;
        Ant::n = n;
        Ant::ziel = ziel;
    }

    // Ameise läuft solange, bis Weg zum Ziel gefunden. Weg wird gespeichert.
    Solution findTarget() {
        Solution s;
        s.path = new int[1];
        s.anzNodes = 1;
        s.path[0] = pos;
        s.length = 0;

        while (true) {

            double *prob = new double[n];

            double aSum = allowedSum();

            for (int i = 0; i < n; i++) {
                if (pos == i) prob[i] = 0;
                else if (matrix[pos][i] == -1) prob[i] = 0;
                else {
                    prob[i] = (pow(pheroMap[pos][i], alpha) * (pow(1.0 / matrix[pos][i], beta))) / aSum;
                }
            }

            int newNode = rand() % n; // COMPLETELY RANDOM !!!!!!! TODO

            s.length += matrix[pos][newNode];
            pos = newNode;
            s.anzNodes++;
            int *tempPath = new int[s.anzNodes - 1];
            for (int i = 0; i < (s.anzNodes - 1); i++) {
                tempPath[i] = s.path[i];
            }
            delete[] s.path;
            s.path = new int[s.anzNodes];
            for (int i = 0; i < (s.anzNodes - 1); i++) {
                s.path[i] = tempPath[i];
            }
            delete[] tempPath;
            s.path[s.anzNodes - 1] = pos;

            if (pos == ziel) break;

        }

        return s;

    }

    // Hilfsfunktion zur Berechnung der Wegfindungsfunktion
    double allowedSum() {
        double sum = 0;

        for (int i = 0; i < n; i++) {
            if (matrix[pos][i] >= 0) {
                sum += pow(pheroMap[pos][i], alpha)*(pow(1.0/matrix[pos][i], beta));
            }

        }

        return sum;
    }

    void setPheroMap(double** nPheroMap) {
        Ant::pheroMap = nPheroMap;
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
            ants[i].setup(start, matrix, pheroMap, n, ziel);
        }
    }

    // alle Ameisen auf den Weg schicken und bestes Ergebnis des Durchgangs speichern
    void antsRun() {
        Solution* s = new Solution[antnum];
        for (int i = 0; i < antnum; i++) {
            ants[i].setPheroMap(pheroMap);
            s[i] = ants[i].findTarget();
        }

        int minI = 0;
        double minV = s[0].length;

        for (int i = 1; i < antnum; i++) {
            if (s[i].length < minV) {
                minI = i;
                minV = s[i].length;
            }
        }

        bestSolution = s[minI];
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

        // Startzustand der Pheromone herstellen
        pheroMap = new double*[n];
        for (int i = 0; i < n; i++) {
            pheroMap[i] = new double[n];
            for (int j = 0; j < n; j++) {
                pheroMap[i][j] = 0.1;
            }
        }

    }

    ~ACO() {}

    // Hauptschleife
    Solution run() {
        for (int i = 0; i < iter; i++) {
            initAnts();
            antsRun();
            // update PheroMap TODO

        }

        return bestSolution;
    }

};

// Hilfskontrukt zur Übergabe der eingelesenen Matrix
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
    else {  // Parameter korrekt
        Solution s;
        Grid g = readMatrix(argv[1]);
        ACO aco(g.n, g.matrix, start, ende, 10);
        s = aco.run();
        cout << "Beste Loesung:" << endl;
        cout << "Laenge: " << s.length << endl;
        cout << "Weg: ";
        for (int i = 0; i < s.anzNodes; i++) cout << s.path[i] << " ";
        cout << endl;
    }
}