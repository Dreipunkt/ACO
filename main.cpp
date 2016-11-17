#include <iostream>
#include <fstream>

using namespace std;

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

void shortestPath (int n, double** adjazenz, int start, int ende) {

}

int main(int argc, char* argv[]) {

    int start = 0;
    int ende = 1;

    if (argc < 2) {
        showHelp();
        return 0;
    }
    else {
            Grid grid = readMatrix(argv[1]);
            shortestPath(grid.n, grid.matrix, start, ende);
    }
}