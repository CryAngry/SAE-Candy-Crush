#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

using namespace std;

typedef vector<unsigned> line;
typedef vector<line> mat;

struct maPosition {
    unsigned abs;
    unsigned ord;
};

const unsigned KReset   (0);
const unsigned KNoir    (30);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);

const unsigned KNbCandies = 4;
const unsigned KImpossible = 0;

void clearScreen () {
    cout << "\033[H\033[2J";
}

void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

void initGrid (mat & grid, const size_t & matSize) {
    grid.resize(matSize);
    for (size_t i = 0; i < matSize; ++i) {
        grid[i].resize(matSize);
        for (size_t j = 0; j < matSize; ++j) {
            grid[i][j] = rand() % KNbCandies + 1;
        }
    }
}

void displayGrid (const mat & grid) {
    const size_t matSize = grid.size();
    clearScreen();
    cout << "\033[0m";

    for (size_t i = 0; i < matSize; ++i) {
        for (size_t j = 0; j < matSize; ++j) {
            if (grid[i][j] >= 1 && grid[i][j] <= KNbCandies) {
                couleur(30 + grid[i][j]);
                cout << grid[i][j] << " ";
                couleur(KReset);
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }
}

bool makeAMove (mat & grid, const maPosition & pos, const char & direction){
    maPosition newPos = pos;

    if (direction == 'Z') {
        if (pos.ord > 0) newPos.ord -= 1;
        else return false;
    }
    else if (direction == 'S') {
        if (pos.ord < grid.size() - 1) newPos.ord += 1;
        else return false;
    }
    else if (direction == 'Q') {
        if (pos.abs > 0) newPos.abs -= 1;
        else return false;
    }
    else if (direction == 'D') {
        if (pos.abs < grid.size() - 1) newPos.abs += 1;
        else return false;
    }
    else {
        return false;
    }

    swap(grid[pos.ord][pos.abs], grid[newPos.ord][newPos.abs]);
    return true;
}

bool atLeastThreeInAColumn (const mat & grid, maPosition & pos, unsigned & howMany) {
    const size_t matSize = grid.size();

    for (size_t j = 0; j < matSize; ++j) {
        for (size_t i = 0; i < matSize; ++i) {
            if (grid[i][j] != 0) {
                unsigned count = 1;
                while (i + count < matSize && grid[i + count][j] == grid[i][j]) {
                    count++;
                }
                if (count >= 3) {
                    pos.ord = i;
                    pos.abs = j;
                    howMany = count;
                    return true;
                }
            }
        }
    }
    return false;
}

bool atLeastThreeInARow (const mat & grid, maPosition & pos, unsigned & howMany) {
    const size_t matSize = grid.size();

    for (size_t i = 0; i < matSize; ++i) {
        for (size_t j = 0; j < matSize; ++j) {
            if (grid[i][j] != 0) {
                unsigned count = 1;
                while (j + count < matSize && grid[i][j + count] == grid[i][j]) {
                    count++;
                }
                if (count >= 3) {
                    pos.ord = i;
                    pos.abs = j;
                    howMany = count;
                    return true;
                }
            }
        }
    }
    return false;
}

void removalInColumn (mat & grid, const maPosition & pos, unsigned howMany) {
    size_t taille = grid.size();
    size_t col = pos.abs;

    for (size_t i = pos.ord; i < taille - howMany; ++i) {
        grid[i][col] = grid[i + howMany][col];
    }

    for (size_t i = taille - howMany; i < taille; ++i) {
        grid[i][col] = KImpossible;
    }
}

void removalInRow (mat & grid, const maPosition & pos, unsigned howMany) {
    for (unsigned k = 0; k < howMany; ++k) {
        maPosition currentPos;
        currentPos.ord = pos.ord;
        currentPos.abs = pos.abs + k;
        removalInColumn(grid, currentPos, 1);
    }
}

unsigned lireMeilleurScore(){
    ifstream file("meilleurScore.txt");
    unsigned score = 0;
    if (file) file >> score;
    return score;
}

void enregistrerMeilleurScore(unsigned score){
    ofstream file("meilleurScore.txt");
    file << score;
}

int main() {

    srand(time(NULL));
    mat grid;
    const size_t tailleGrille = 10;
    initGrid(grid, tailleGrille);

    const unsigned NbCoupsMax = 20;
    unsigned nbCoups = 0;

    unsigned scoreJ1 = 0;
    unsigned scoreJ2 = 0;
    unsigned joueur = 1;

    maPosition pos;
    char direction;

    while (nbCoups < NbCoupsMax) {
        displayGrid(grid);
        cout << "Joueur " << joueur << " à toi de jouer !" << endl;
        cout << "Score J1 : " << scoreJ1 << " | Score J2 : " << scoreJ2 << endl;
        cout << "Coups restants : " << NbCoupsMax - nbCoups << endl;

        bool coupValide = false;
        while (!coupValide) {
            cout << "Entrez Ligne (0-9), Colonne (0-9) et Direction (Z,S,Q,D) :" << endl;
            cin >> pos.ord >> pos.abs >> direction;

            if (makeAMove(grid, pos, direction)) {
                coupValide = true;
                nbCoups++;
            } else {
                cout << "Coup impossible !" << endl;
            }
        }

        bool suiteTrouvee = true;
        while (suiteTrouvee) {
            suiteTrouvee = false;
            unsigned howMany;
            maPosition detectPos;

            if (atLeastThreeInARow(grid, detectPos, howMany)) {
                removalInRow(grid, detectPos, howMany);
                if (joueur == 1) scoreJ1 += howMany;
                else scoreJ2 += howMany;
                suiteTrouvee = true;
            }

            if (atLeastThreeInAColumn(grid, detectPos, howMany)) {
                removalInColumn(grid, detectPos, howMany);
                if (joueur == 1) scoreJ1 += howMany;
                else scoreJ2 += howMany;
                suiteTrouvee = true;
            }
        }

        joueur = (joueur == 1 ? 2 : 1);
    }

    displayGrid(grid);
    cout << "PARTIE TERMINEE !" << endl;
    cout << "Score final J1 : " << scoreJ1 << endl;
    cout << "Score final J2 : " << scoreJ2 << endl;

    // meilleur des deux pour le fichier
    unsigned scoreMax = (scoreJ1 > scoreJ2 ? scoreJ1 : scoreJ2);
    unsigned meilleurScore = lireMeilleurScore();
    cout << "Meilleur score actuel : " << meilleurScore << endl;
    if (scoreMax > meilleurScore) {
        enregistrerMeilleurScore(scoreMax);
        cout << "Nouveau meilleur score !" << endl;
    }

    if (scoreJ1 > scoreJ2)
        cout << "Le joueur 1 gagne !" << endl;
    else if (scoreJ2 > scoreJ1)
        cout << "Le joueur 2 gagne !" << endl;
    else
        cout << "Égalité parfaite !" << endl;

    return 0;
}
