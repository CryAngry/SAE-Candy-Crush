#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// Un chercheur en cryptographie travaillait depuis des années sur un projet classé secret, un programme capable de dissimuler des informations dans des structures apparemment anodines. Avant de partir en mission, il laissa derrière lui un étrange fichier : une version modifiée d’un jeu de bonbons colorés. À première vue, cela ressemblait à un simple divertissement, mais en l’ouvrant, on comprenait vite que ce n’était pas un jeu ordinaire. Le chercheur avait dissimulé son identité dans huit lettres codées en ROT13, chacune libérée uniquement lorsqu’une combinaison rare était réalisée. Comme s’il avait volontairement fragmenté son nom pour le protéger, il avait transformé son ultime message en un puzzle que seuls les plus persévérants pourraient résoudre. Sur la première page du dossier qu’il avait laissé, une phrase manuscrite résumait tout : « Si tu lis ceci, c’est que tu es digne de connaître mon nom. » À mesure que les lettres apparaissent, le joueur se rapproche de la vérité, découvrant peu à peu l’héritage du chercheur disparu. Et lorsque les huit fragments sont enfin réunis, son nom se révèle, comme la clé d’un secret plus vaste encore.

typedef vector<unsigned> line;
typedef vector<line> mat;

struct maPosition { 
    unsigned abs;
    unsigned ord;
};

const unsigned KNbCandies = 4;
const unsigned KImpossible = 0;

// ROT13
string rot13(const string & s) {
    string res = s;
    for (char & c : res) {
        if (c >= 'A' && c <= 'Z')
            c = 'A' + (c - 'A' + 13) % 26;
        else if (c >= 'a' && c <= 'z')
            c = 'a' + (c - 'a' + 13) % 26;
    }
    return res;
}

// Indices = lettres ROT13
vector<string> indices = {
    "Z", // M
    "e", // r
    "P", // C
    "n", // a
    "f", // s
    "n", // a
    "y", // l
    "v"  // i
};

// Suivi des lettres trouvées
vector<bool> indicesTrouves(indices.size(), false);

const unsigned KReset   (0);
const unsigned KNoir    (30);
const unsigned KRouge   (31);
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);




void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}

void clearScreen () {
    cout << "\033[H\033[2J";
}

bool combin(const mat & grid, size_t i, size_t j , unsigned val){
    if (j >= 2 && grid[i][j-1] == val && grid[i][j-2] == val) return true;
    if (i >= 2 && grid[i-1][j] == val && grid[i-2][j] == val) return true;
    return false;
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

    if (direction == 'Z' && pos.ord > 0) newPos.ord--;
    else if (direction == 'S' && pos.ord < grid.size() - 1) newPos.ord++;
    else if (direction == 'Q' && pos.abs > 0) newPos.abs--;
    else if (direction == 'D' && pos.abs < grid.size() - 1) newPos.abs++;
    else return;

    swap(grid[pos.ord][pos.abs], grid[newPos.ord][newPos.abs]);
}

bool atLeastThreeInAColumn (const mat & grid, maPosition & pos, unsigned & howMany) {
    for (size_t j = 0; j < grid.size(); ++j) {
        for (size_t i = 0; i < grid.size(); ++i) {
            if (grid[i][j] != 0) {
                unsigned count = 1;
                while (i + count < grid.size() && grid[i + count][j] == grid[i][j])
                    count++;
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
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid.size(); ++j) {
            if (grid[i][j] != 0) {
                unsigned count = 1;
                while (j + count < grid.size() && grid[i][j + count] == grid[i][j])
                    count++;
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
    size_t col = pos.abs;
    for (size_t i = pos.ord; i < grid.size() - howMany; ++i)
        grid[i][col] = grid[i + howMany][col];
    for (size_t i = grid.size() - howMany; i < grid.size(); ++i)
        grid[i][col] = rand() % KNbCandies + 1;
}

void removalInRow (mat & grid, const maPosition & pos, unsigned howMany) {
    for (unsigned k = 0; k < howMany; ++k) {
        maPosition currentPos = {pos.abs + k, pos.ord};
        removalInColumn(grid, currentPos, 1);
    }
}

int main() {

    srand(time(NULL));
    mat grid;
    const size_t tailleGrille = 8;
    initGrid(grid, tailleGrille);

    const unsigned NbCoupsMax = 20;
    unsigned nbCoups = 0;
    unsigned score = 0;

    maPosition pos;
    char direction;

    while (nbCoups < NbCoupsMax) {
        displayGrid(grid);
        cout << "Score : " << score << endl;
        cout << "Coups restants : " << NbCoupsMax - nbCoups << endl;
        cout << "Entrez Ligne Colonne Direction (Z,Q,S,D) :" << endl;

        cin >> pos.ord >> pos.abs >> direction;

        makeAMove(grid, pos, direction);
        nbCoups++;

        bool suiteTrouvee = true;
        while (suiteTrouvee) {
            suiteTrouvee = false;
            unsigned howMany;
            maPosition detectPos;

            // Lignes
            if (atLeastThreeInARow(grid, detectPos, howMany)) {
                removalInRow(grid, detectPos, howMany);
                score += howMany;

                if (howMany >= 4) {
                    int id = rand() % indices.size();
                    indicesTrouves[id] = true;

                    cout << "\n=== INDICE ===" << endl;
                    cout << "Lettre ROT13 : " << indices[id] << endl;
                    cout << "Décodée : " << rot13(indices[id]) << endl;
                    cout << "=============\n" << endl;
                }

                suiteTrouvee = true;
            }

            // Colonnes
            if (atLeastThreeInAColumn(grid, detectPos, howMany)) {
                removalInColumn(grid, detectPos, howMany);
                score += howMany;

                if (howMany >= 4) {
                    int id = rand() % indices.size();
                    indicesTrouves[id] = true;

                    cout << "\n=== INDICE ===" << endl;
                    cout << "Lettre ROT13 : " << indices[id] << endl;
                    cout << "Décodée : " << rot13(indices[id]) << endl;
                    cout << "=============\n" << endl;
                }

                suiteTrouvee = true;
            }
        }
    }

    // Fin de partie
    displayGrid(grid);
    cout << "PARTIE TERMINEE !" << endl;
    cout << "Votre score final est de : " << score << endl;

    // Vérification des lettres trouvées
    bool tousTrouves = true;
    for (bool b : indicesTrouves)
        if (!b) tousTrouves = false;

    if (tousTrouves) {
        cout << "\n=== CODE FINAL ===" << endl;
        cout << "Mr Casali" << endl;
        cout << "==================\n" << endl;
    } else {
        cout << "\nVous n'avez pas trouvé toutes les lettres..." << endl;
    }

    return 0;
}
