#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

typedef vector <unsigned> line;
typedef vector <line> mat;

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

bool combin(const mat & grid, size_t i, size_t j , unsigned val){
    
    if (j >= 2 && grid[i][j-1] == val && grid[i][j-2] == val) return true;

    if (i >= 2 && grid[i-1][j] == val && grid[i-2][j] == val) return true;

    return false;
}



void initGrid (mat & grid, const size_t & matSize) {
    grid.resize (matSize);
    for (size_t i = 0; i < matSize; ++i) {
        grid[i].resize (matSize);
        for (size_t j = 0; j < matSize; ++j) {
            unsigned val;
            do{
                val = rand() % KNbCandies + 1;
            } while(combin(grid, i, j , val));
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
                couleur(KReset);
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }
}

void makeAMove (mat & grid, const maPosition & pos, const char & direction){
    maPosition newPos = pos;

    if (direction == 'Z') {
        if (pos.ord > 0) newPos.ord -= 1;
    }
    else if (direction == 'S') {
        if (pos.ord < grid.size() - 1) newPos.ord += 1;
    }
    else if (direction == 'Q') {
        if (pos.abs > 0) newPos.abs -= 1;
    }
    else if (direction == 'D') {
        if (pos.abs < grid.size() - 1) newPos.abs += 1;
    }
    else {
        return; 
    }
    swap (grid[pos.ord][pos.abs], grid[newPos.ord][newPos.abs]);
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
                
                while (j + count < matSize && grid[i][j+count] == grid[i][j]) {
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

int main() {

    srand(time(NULL));
    mat grid;
    const size_t tailleGrille = 8;
    initGrid(grid, tailleGrille);

    const unsigned NbCoupsMax = 20; // Nombre de coups max
    unsigned nbCoups = 0;
    unsigned score = 0;

    maPosition pos;
    char direction;

    //Tant que coupmax non atteints
    while (nbCoups < NbCoupsMax) {
        displayGrid(grid); //affiche la grille et menu 
        cout << "Score : " << score << endl;
        cout << "Coups restants : " << NbCoupsMax - nbCoups << endl;
        cout << "Entrez Ligne (0-9), Colonne (0-9) et Direction (Z,Q,S,D) :" << endl;

        cin >> pos.ord >> pos.abs >> direction;//saisit position et direction

        makeAMove(grid, pos, direction); 
        nbCoups++;

        bool suiteTrouvee = true;
        while(suiteTrouvee){
            suiteTrouvee = false; //on part du principe qu'il y a plus rien
            unsigned howMany;
            maPosition detectPos;

            if (atLeastThreeInARow(grid, detectPos, howMany)) {
                removalInRow(grid, detectPos, howMany);
                score += howMany;
                suiteTrouvee = true; //apres modif, faut la var
            }

            if (atLeastThreeInAColumn(grid, detectPos, howMany)) {
                removalInColumn(grid, detectPos, howMany);
                score += howMany;
                suiteTrouvee = true; 
            }

        }

    }

    //fin de la partie
    displayGrid(grid);
    cout << "PARTIE TERMINEE !" << endl;
    cout << "Votre score final est de : " << score << endl;

    return 0;

}
