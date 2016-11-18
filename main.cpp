#include <iostream>
#include <fstream>
#include <cmath>
#include <random>

#define DEBUG_MODE false // Flag zum Anzeigen von Debugging Ausgaben (Normalzustand: false)

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

    // wählt anhand der Wahrscheinlichkeits-Map einen Knoten aus
    int getNextNode(double* prob) {
        int bCount = 0;

        for (int i = 0; i < n; i++) {
            if(DEBUG_MODE) cout << "PROB " << prob[i] << endl; //D
            if (prob[i] > 0) bCount++;
        }



        int* bound = new int[bCount];
        int* corr = new int[bCount];

        bCount = 0;

        for (int i = 0; i < n; i++) {
            if (prob[i] > 0) {
                bound[bCount++] = (int) (prob[i] * 10000);     // Faktor regelt Genauigkeit der Verteilung (MAXIMAL 30000)
                corr[bCount - 1] = i;
                if(DEBUG_MODE) cout << "BOUND " << bound[bCount - 1] << endl; //D
                if(DEBUG_MODE) cout << "CORR " << corr[bCount - 1] << endl; //D
            }
        }

        for (int i = 0; i < bCount; i++) {
            if (i > 0) bound[i] = bound[i] + bound[i-1];
            if(DEBUG_MODE) cout << "BOUND_ADD " << bound[i] << endl; //D
        }

        if(DEBUG_MODE) cout << "RAND_MAX " << (bound[bCount - 1] + 1) << endl; //D
        int rando = rand() % (bound[bCount - 1] + 1);

        if(DEBUG_MODE) cout << "RAND " << rando << endl; //D


        for (int i = 0; i < bCount; i++) {
            if (rando <= bound[i]) return corr[i];
        }

        return 0;   // eigentlich irrelevant, außer bei Fehlern
    }

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

            // erstellen der Wahrscheinlichkeits-Map
            for (int i = 0; i < n; i++) {
                if (matrix[pos][i] == -1) prob[i] = 0;
                else {
                    prob[i] = (pow(pheroMap[pos][i], alpha) * (pow(1.0 / matrix[pos][i], beta))) / aSum;
                }
            }

            // auswählen des nächsten Knotens auf Basis der Wahrscheinlichkeiten
            int newNode = getNextNode(prob);

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

            if(DEBUG_MODE) cout << "POS " << pos << endl; //D
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

    // Logik zum Updaten der Pheromon-Map
    void pheroUpdate(Solution* s) {
        double decay = 0.1; // Decay-Faktor der Spuren zwischen 0 und 1 (groesser --> schnelleres Verschwinden alter Spuren)

        // decay
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                pheroMap[i][j] *= (1.0 - decay);
            }
        }

        double q = 1.0;     // Qualitätsfaktor (sollte möglichst nahe an optimaler Lösung liegen)

        // hinzufügen der neuen Pheromone
        for (int i = 0; i < antnum; i++) {
            for (int j = 0; j < s[i].anzNodes - 1; j++) {
                pheroMap[s[i].path[j]][s[i].path[j + 1]] += (q / s[i].length);
            }
        }
    }

    // alle Ameisen auf den Weg schicken und bestes Ergebnis des Durchgangs retunieren
    Solution antsRun() {

        Solution* s = new Solution[antnum];
        for (int i = 0; i < antnum; i++) {

            ants[i].setPheroMap(pheroMap);

            s[i] = ants[i].findTarget();
        }



        pheroUpdate(s);     // update der Pheromon-Map

        int minI = 0;
        double minV = s[0].length;

        for (int i = 1; i < antnum; i++) {
            if (s[i].length < minV) {
                minI = i;
                minV = s[i].length;
            }
        }

        return s[minI];
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
                pheroMap[i][j] = 1.0;
            }
        }

    }

    ~ACO() {}

    // Hauptschleife, retuniert beste Lösung aller Durchgänge
    Solution run() {
        for (int i = 0; i < iter; i++) {
            Solution s;
            initAnts();
            s = antsRun();  // update der Pheromon-Map erfolgt auch hier
            if (i == 0) bestSolution = s;
            else if (s.length < bestSolution.length) bestSolution = s;
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
    cout << "Usage: ACO.exe Path_to_datafile Startnode Endnode Number_of_iterations" << endl;
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
    int iterNum = 1;

    if (argc < 5) {
        showHelp();
        return 0;
    }
    else {  // Parameter korrekt
        Solution s;
        Grid g = readMatrix(argv[1]);
        start = atoi(argv[2]);
        ende = atoi(argv[3]);
        iterNum = atoi(argv[4]);
        ACO aco(g.n, g.matrix, start, ende, iterNum);
        s = aco.run();

        // Ausgabe
        cout << "Beste Loesung:" << endl;
        cout << "Laenge: " << s.length << endl;
        cout << "Weg: ";
        for (int i = 0; i < s.anzNodes; i++) cout << s.path[i] << " ";
        cout << endl;
    }
}