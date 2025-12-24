#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

//Couleur ecran

const unsigned KReset   (0);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);

void couleur (const unsigned & coul) {
    cout << "\033[" << coul << "m";
}

void clearScreen () {
    cout << "\033[H\033[2J";
}


typedef vector<unsigned> line;
typedef vector<line>     mat;

struct maPosition {
    unsigned abs; // colonne
    unsigned ord; // ligne
};

const unsigned KNbCandies  = 4;  // valeurs autorisées [1..KNbCandies]
const unsigned KImpossible = 0;  // case vide
const unsigned KMaxTimes   = 10; // nombre maximal de coups
const size_t   NDefault    = 6;  // taille par défaut


//On initialise la grille

void initGrid (mat & grid, const size_t & matSize) {
    grid = mat(matSize, line(matSize));
    for (unsigned y = 0; y < matSize; ++y) {
        for (unsigned x = 0; x < matSize; ++x) {
            grid[y][x] = 1 + rand() % KNbCandies;
        }
    }
}

//On affiche la grille


void displayGrid (const mat & grid) {
    clearScreen();
    couleur(KReset);

    unsigned N = grid.size();
    for (unsigned y = 0; y < N; ++y) {
        for (unsigned x = 0; x < N; ++x) {
            unsigned v = grid[y][x];
            if (v >= 1 && v <= KNbCandies) {
                cout << v << " ";
            } else {
                cout << ". ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

//On fait les directions//


void makeAMove (mat & grid, const maPosition & pos, const char & direction) {
    unsigned N = grid.size();
    if (pos.abs >= N || pos.ord >= N) return;

    int dx = 0, dy = 0;
    char d = toupper(direction);

    if      (d == 'Q') { dx = -1; dy =  0; }
    else if (d == 'D') { dx =  1; dy =  0; }
    else if (d == 'Z') { dx =  0; dy = -1; }
    else if (d == 'S') { dx =  0; dy =  1; }
    else return;

    int nx = (int)pos.abs + dx;
    int ny = (int)pos.ord + dy;

    if (nx < 0 || ny < 0) return;
    if ((unsigned)nx >= N || (unsigned)ny >= N) return;

    swap(grid[pos.ord][pos.abs], grid[ny][nx]);   
}


int main(){
    size_t matSize = 8;
    mat grid;
    initGrid(grid, matSize);
    displayGrid(grid);

    return 0;
}

// //On détecte les lignes et les colonnes


// bool atLeastThreeInAColumn (const mat & grid, maPosition & pos, unsigned & howMany) {
//     unsigned N = grid.size();
//     for (unsigned x = 0; x < N; ++x) {
//         unsigned y = 0;
//         while (y < N) {
//             unsigned v = grid[y][x];
//             if (v < 1 || v > KNbCandies) { ++y; continue; }
//             unsigned start = y;
//             unsigned end   = y + 1;
//             while (end < N && grid[end][x] == v) ++end;
//             unsigned len = end - start;
//             if (len >= 3) {
//                 pos.abs = x;
//                 pos.ord = start;
//                 howMany = len;
//                 return true;
//             }
//             y = end;
//         }
//     }
//     return false;
// }

// bool atLeastThreeInARow (const mat & grid, maPosition & pos, unsigned & howMany) {
//     unsigned N = grid.size();
//     for (unsigned y = 0; y < N; ++y) {
//         unsigned x = 0;
//         while (x < N) {
//             unsigned v = grid[y][x];
//             if (v < 1 || v > KNbCandies) { ++x; continue; }
//             unsigned start = x;
//             unsigned end   = x + 1;
//             while (end < N && grid[y][end] == v) ++end;
//             unsigned len = end - start;
//             if (len >= 3) {
//                 pos.abs = start;
//                 pos.ord = y;
//                 howMany = len;
//                 return true;
//             }
//             x = end;
//         }
//     }
//     return false;
// }


// //On supprime les lignes et les colonnes//

// void removalInColumn (mat & grid, const maPosition & pos, unsigned howMany) {
//     unsigned N = grid.size();
//     unsigned x = pos.abs;
//     unsigned y0 = pos.ord;
//     if (x >= N || y0 >= N) return;
//     if (y0 + howMany > N) return;

//     for (unsigned k = 0; k < howMany; ++k) {
//         grid[y0 + k][x] = KImpossible;
//     }

//     vector<unsigned> col;
//     for (unsigned y = 0; y < N; ++y) {
//         if (grid[y][x] >= 1 && grid[y][x] <= KNbCandies) col.push_back(grid[y][x]);
//     }

//     int write = N - 1;
//     for (int i = col.size() - 1; i >= 0; --i) {
//         grid[write][x] = col[i];
//         --write;
//     }
//     while (write >= 0) {
//         grid[write][x] = KImpossible;
//         --write;
//     }
// }

// void removalInRow (mat & grid, const maPosition & pos, unsigned howMany) {
//     unsigned N = grid.size();
//     unsigned y = pos.ord;
//     unsigned x0 = pos.abs;
//     if (y >= N || x0 >= N) return;
//     if (x0 + howMany > N) return;

//     for (unsigned k = 0; k < howMany; ++k) {
//         grid[y][x0 + k] = KImpossible;
//     }
//     for (unsigned k = 0; k < howMany; ++k) {
//         maPosition cpos{ x0 + k, y };
//         removalInColumn(grid, cpos, 1);
//     }
// }

// //le main pour afficher le résultat//


// int main() {
//     srand(time(nullptr));

//     mat grid;
//     initGrid(grid, NDefault);

//     unsigned score = 0;

//     for (unsigned turn = 1; turn <= KMaxTimes; ++turn) {
//         displayGrid(grid);

//         cout << "Tour " << turn << "/" << KMaxTimes << "\n";
//         cout << "Deplacements: A(gauche), Z(haut), E(droite), S(bas)\n";
//         cout << "Entrez x y direction (ex: 2 3 A): ";

//         unsigned x, y; char d;
//         if (!(cin >> x >> y >> d)) break;
//         if (x >= grid.size() || y >= grid.size()) continue;

//         maPosition pos{ x, y };
//         makeAMove(grid, pos, d);

//         bool removed = false;
//         while (true) {
//             maPosition fpos{};
//             unsigned howMany = 0;

//             if (atLeastThreeInAColumn(grid, fpos, howMany)) {
//                 unsigned val = grid[fpos.ord][fpos.abs];
//                 removalInColumn(grid, fpos, howMany);
//                 score += howMany * val;
//                 removed = true;
//                 continue;
//             }
//             if (atLeastThreeInARow(grid, fpos, howMany)) {
//                 unsigned val = grid[fpos.ord][fpos.abs];
//                 removalInRow(grid, fpos, howMany);
//                 score += howMany * val;
//                 removed = true;
//                 continue;
//             }
//             break;
//         }

//         displayGrid(grid);
//         if (removed) cout << "Suppression effectuee. Score: " << score << "\n";
//         else cout << "Aucune combinaison.\n";
//     }

//     displayGrid(grid);
//     cout << "Score final: " << score << "\n";
//     cout << "Fin.\n";
//     return 0;
// }


