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

int jouer(mat & grid, const size_t tailleGrille);

const unsigned KNbCandies = 4;
const unsigned KImpossible = 0;

bool combin(const mat &grid, size_t i, size_t j, unsigned val){

    if (j >= 2 && grid[i][j - 1] == val && grid[i][j - 2] == val)
        return true;

    if (i >= 2 && grid[i - 1][j] == val && grid[i - 2][j] == val)
        return true;

    return false;
}

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
            unsigned val;
            do {
                val = rand() % KNbCandies + 1;
            } while (combin(grid, i, j, val));
            grid[i][j] = val;
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
                couleur(0);
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

int menu(){
    int choix = 0;
    cout << "---------------------------" << endl;
    cout << "       Menu Principal      " << endl;
    cout << "---------------------------" << endl;
    cout << "        Mode 1vs1          " << endl;
    cout << "     Voir le leaderboard   " << endl;
    cout << "      Chasse au trésor     " << endl;
    cout << "         Quitter           " << endl;
    cin >> choix;
    return choix;
}


void afficherLeaderboard() {
    unsigned meilleurScore = lireMeilleurScore();
    cout << "==========================" << endl;
    cout << "       LEADERBOARD        " << endl;
    cout << "==========================" << endl;
    cout << "Meilleur score enregistré : " << meilleurScore << endl;
    cout << "==========================" << endl;
    cout << "Appuyez sur une touche pour revenir au menu..." << endl;
    cin.ignore();
    cin.get();
}

int main(){
    srand(time(NULL));
    mat grid;
    const size_t tailleGrille = 10;
    initGrid(grid, tailleGrille);



    while (true) {

        int choix = menu();

        if (choix == 1) {
            jouer(grid, tailleGrille);
        }
        else if (choix == 2) {
            afficherLeaderboard();
        }
        else if (choix == 3) {
            cout << "Au revoir !" << endl;
            return 0;
        }
        else {
            cout << "Choix invalide !" << endl;
        }
    }

}

int jouer(mat & grid, const size_t tailleGrille) {
    const unsigned NbCoupsMax = 20;
    unsigned nbCoups = 0;

    unsigned scoreJ1 = 0;
    unsigned scoreJ2 = 0;
    unsigned joueur = 1;

    maPosition pos;
    char direction;




    while (nbCoups < NbCoupsMax) {
        displayGrid(grid);
        cout << "Joueur " << joueur << " C'est à ton tour " << endl;
        cout << "Score J1 : " << scoreJ1 << " | Score J2 : " << scoreJ2 << endl;
        cout << "Coups restants : " << NbCoupsMax - nbCoups << endl;

        bool coupValide = false;
        while (!coupValide) {
            cout << "Entrez Ligne (0-9), Colonne (0-9) et Direction (Z,S,Q,D) :" << endl;
            cin >> pos.ord >> pos.abs >> direction;

            // Vérification des limites
            if (pos.ord >= tailleGrille || pos.abs >= tailleGrille) {
                cout << "Coordonnées hors grille !" << endl;
                continue;
            }

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

        if (joueur == 1)
            joueur = 2;
        else
            joueur = 1;

    }

    displayGrid(grid);
    cout << "PARTIE TERMINEE !" << endl;
    cout << "Score final J1 : " << scoreJ1 << endl;
    cout << "Score final J2 : " << scoreJ2 << endl;

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
