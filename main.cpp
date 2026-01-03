#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>
//author: Hssini Ayoub,Nguyen Vinh Tan Thomas
//date:05/01/2026
using namespace std;

// ============================================================================
//  TYPES ET CONSTANTES
// ============================================================================

typedef vector<unsigned> line;
typedef vector<line> mat;

struct maPosition
{
    unsigned abs;
    unsigned ord;
};

const unsigned KNbCandies = 4;
const unsigned KImpossible = 0;

// Couleurs ANSI
const unsigned KReset   = 0;
const unsigned KNoir    = 30;
const unsigned KRouge   = 31;
const unsigned KVert    = 32;
const unsigned KJaune   = 33;
const unsigned KBleu    = 34;
const unsigned KMagenta = 35;
const unsigned KCyan    = 36;


// ============================================================================
//  ROT13 + INDICES
// ============================================================================

string rot13(const string & s)
{
    string res = s;

    for (char & c : res)
    {
        if (c >= 'A' && c <= 'Z')
        {
            c = 'A' + (c - 'A' + 13) % 26;
        }
        else if (c >= 'a' && c <= 'z')
        {
            c = 'a' + (c - 'a' + 13) % 26;
        }
    }

    return res;
}

// Lettres ROT13
vector<string> indices =
    {
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


// ============================================================================
//  OUTILS D'AFFICHAGE
// ============================================================================

void couleur(const unsigned & coul)
{
    cout << "\033[" << coul << "m";
}

void clearScreen()
{
    cout << "\033[H\033[2J";
}

unsigned lireMeilleurScore(){
    ifstream file("meilleurScore.txt");
    unsigned score = 0;
    if (file) file >> score;
    return score;
}

void enregistrerMeilleurScore(unsigned score){
    ofstream file ("meilleurScore.txt");
    file << score;
}


void afficherHistoireTresor()
{
    clearScreen();

    cout << "============================================================================" << endl;
    cout << "  HISTOIRE DU MODE CHASSE AU TRESOR (ROT13)" << endl;
    cout << "============================================================================" << endl;
    cout << endl;

    cout << "Un chercheur en cryptographie travaillait depuis des annees sur un projet" << endl;
    cout << "classe secret, un programme capable de dissimuler des informations dans des" << endl;
    cout << "structures apparemment illisibles. Avant de partir en mission, il laissa" << endl;
    cout << "derriere lui un etrange fichier : une version modifiee d’un jeu de bonbons" << endl;
    cout << "colores." << endl;
    cout << endl;

    cout << "A premiere vue, cela ressemblait a un simple divertissement, mais en" << endl;
    cout << "l’ouvrant, on comprenait vite que ce n’etait pas un jeu ordinaire. Le" << endl;
    cout << "chercheur avait dissimule son identite dans huit lettres codees en ROT13," << endl;
    cout << "chacune liberee uniquement lorsqu’une combinaison rare etait realisee." << endl;
    cout << endl;

    cout << "Comme s’il avait volontairement fragmente son nom pour le proteger, il avait" << endl;
    cout << "transforme son ultime message en un puzzle que seuls les plus perseverants" << endl;
    cout << "pourraient resoudre." << endl;
    cout << endl;

    cout << "Sur la premiere page du dossier qu’il avait laisse, une phrase manuscrite" << endl;
    cout << "resumait tout :" << endl;
    cout << "    « Si tu lis ceci, c’est que tu es digne de connaitre mon nom. »" << endl;
    cout << endl;

    cout << "A mesure que les lettres apparaissent, le joueur se rapproche de la verite," << endl;
    cout << "decouvrant peu a peu le chercheur disparu. Et lorsque les huit fragments" << endl;
    cout << "sont enfin reunis, son nom se revele, comme la cle d’un secret plus vaste" << endl;
    cout << "encore." << endl;
    cout << endl;

    cout << "============================================================================" << endl;
    cout << "Appuyez sur Entree pour commencer la chasse au tresor..." << endl;
    cout << "============================================================================" << endl;

    cin.ignore();
    cin.get();
}


// ============================================================================
//  FONCTIONS UTILITAIRES
// ============================================================================

bool combin(const mat & grid, size_t i, size_t j, unsigned val)
{
    if (j >= 2 && grid[i][j - 1] == val && grid[i][j - 2] == val)
    {
        return true;
    }

    if (i >= 2 && grid[i - 1][j] == val && grid[i - 2][j] == val)
    {
        return true;
    }

    return false;
}




// ============================================================================
//  INITIALISATION DE LA GRILLE (chiffres, ASCII,Chasse au trésor)
// ============================================================================

void initGrid(mat & grid, const size_t & matSize, bool asciiMode, bool treasureMode)
{
    grid.resize(matSize);

    for (size_t i = 0; i < matSize; ++i)
    {
        grid[i].resize(matSize);

        for (size_t j = 0; j < matSize; ++j)
        {
            if (asciiMode)
            {
                // Lettres A, B, C, D
                unsigned lettre;
                do
                {
                    lettre = 'A' + (rand() % KNbCandies);
                }
                while (combin(grid, i, j, lettre));
                grid[i][j] = lettre ;
            }
            else
            {
                // Mode classique ou trésor → chiffres
                unsigned val;

                do
                {
                    val = rand() % KNbCandies + 1;
                }
                while (combin(grid, i, j, val));

                grid[i][j] = val;
            }
        }
    }
}


// ============================================================================
//  AFFICHAGE DE LA GRILLE (couleurs chiffres + lettres)
// ============================================================================

void displayGrid(const mat & grid)
{
    clearScreen();
    cout << "\033[0m";

    for (size_t i = 0; i < grid.size(); ++i)
    {
        for (size_t j = 0; j < grid[i].size(); ++j)
        {
            unsigned val = grid[i][j];

            // Lettres ASCII
            if (val >= 'A' && val <= 'Z')
            {
                if (val == 'A')
                {
                    couleur(KRouge);
                }
                else if (val == 'B')
                {
                    couleur(KVert);
                }
                else if (val == 'C')
                {
                    couleur(KJaune);
                }
                else if (val == 'D')
                {
                    couleur(KBleu);
                }

                cout << char(val) << " ";
                couleur(KReset);
            }
            // Chiffres classiques
            else if (val >= 1 && val <= KNbCandies)
            {
                couleur(30 + val);
                cout << val << " ";
                couleur(KReset);
            }
            else
            {
                cout << "  ";
            }
        }

        cout << endl;
    }
}


// ============================================================================
//  DEPLACEMENT D'UN BONBON
// ============================================================================

bool makeAMove(mat & grid, const maPosition & pos, const char & direction)
{
    maPosition newPos = pos;

    if (direction == 'Z')
    {
        if (pos.ord > 0)
        {
            newPos.ord -= 1;
        }
        else
        {
            return false;
        }
    }
    else if (direction == 'S')
    {
        if (pos.ord < grid.size() - 1)
        {
            newPos.ord += 1;
        }
        else
        {
            return false;
        }
    }
    else if (direction == 'Q')
    {
        if (pos.abs > 0)
        {
            newPos.abs -= 1;
        }
        else
        {
            return false;
        }
    }
    else if (direction == 'D')
    {
        if (pos.abs < grid.size() - 1)
        {
            newPos.abs += 1;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    swap(grid[pos.ord][pos.abs], grid[newPos.ord][newPos.abs]);
    return true;
}


// ============================================================================
//  DETECTION DES COMBOS (LIGNES + COLONNES)
// ============================================================================

bool atLeastThreeInAColumn(const mat & grid, maPosition & pos, unsigned & howMany)
{
    for (size_t j = 0; j < grid.size(); ++j)
    {
        for (size_t i = 0; i < grid.size(); ++i)
        {
            if (grid[i][j] != 0)
            {
                unsigned count = 1;

                while (i + count < grid.size() && grid[i + count][j] == grid[i][j])
                {
                    count++;
                }

                if (count >= 3)
                {
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

bool atLeastThreeInARow(const mat & grid, maPosition & pos, unsigned & howMany)
{
    for (size_t i = 0; i < grid.size(); ++i)
    {
        for (size_t j = 0; j < grid.size(); ++j)
        {
            if (grid[i][j] != 0)
            {
                unsigned count = 1;

                while (j + count < grid.size() && grid[i][j + count] == grid[i][j])
                {
                    count++;
                }

                if (count >= 3)
                {
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


// ============================================================================
//  SUPPRESSION DES COMBINAISONS
// ============================================================================

void removalInColumn(mat & grid, const maPosition & pos, unsigned howMany, bool asciiMode)
{
    size_t col = pos.abs;

    for (size_t i = pos.ord; i < grid.size() - howMany; ++i)
    {
        grid[i][col] = grid[i + howMany][col];
    }

    for (size_t i = grid.size() - howMany; i < grid.size(); ++i)
    {
        if (asciiMode)
        {
            grid[i][col] = 'A' + (rand() % KNbCandies);
        }
        else
        {
            grid[i][col] = rand() % KNbCandies + 1;
        }
    }
}

void removalInRow(mat & grid, const maPosition & pos, unsigned howMany, bool asciiMode)
{
    for (unsigned k = 0; k < howMany; ++k)
    {
        maPosition currentPos;
        currentPos.ord = pos.ord;
        currentPos.abs = pos.abs + k;

        removalInColumn(grid, currentPos, 1, asciiMode);
    }
}


// ============================================================================
//  FONCTION PRINCIPAL : jouerPartie()
// ============================================================================
int jouerPartie(mat & grid, const size_t tailleGrille, bool pvp, bool asciiMode, bool treasureMode, unsigned nbCoupsMax,unsigned ScoreObjectif)
{
    unsigned nbCoups = 0;

    unsigned scoreSolo = 0;
    unsigned scoreJ1 = 0;
    unsigned scoreJ2 = 0;
    unsigned joueur = 1;

    maPosition pos;
    char direction;

    while (nbCoups < nbCoupsMax)
    {
        displayGrid(grid);

        if (pvp)
        {
            cout << "Joueur " << joueur << ", à toi !" << endl;
            cout << "Score J1 : " << scoreJ1 << " | Score J2 : " << scoreJ2 << endl;
        }
        else
        {
            cout << "Score : " << scoreSolo << endl;
        }

        cout << "Coups restants : " << nbCoupsMax - nbCoups << endl;
        cout << "Entrez Ligne (0-7), Colonne (0-7) et Direction (Z,Q,S,D) ou M pour Menu :" << endl;

        string input;
        cin >> input;

        if (input == "M" || input == "m")
        {
            return 0;
        }

        pos.ord = stoi(input);
        cin >> pos.abs >> direction;

        if (pos.ord >= tailleGrille || pos.abs >= tailleGrille ||
            (direction != 'Z' && direction != 'S' && direction != 'Q' && direction != 'D'))
        {
            cout << "Mauvaise saisie ! Reessayez." << endl;
            continue;
        }

        makeAMove(grid, pos, direction);
        ++nbCoups;

        bool suiteTrouvee = true;

        while (suiteTrouvee)
        {
            suiteTrouvee = false;

            unsigned howMany;
            maPosition detectPos;

            // LIGNES
            if (atLeastThreeInARow(grid, detectPos, howMany))
            {
                // ta version avec asciiMode dans removalInRow
                removalInRow(grid, detectPos, howMany, asciiMode);

                // SCORE
                if (asciiMode)
                {
                    if (pvp)
                    {
                        if (joueur == 1)
                        {
                            scoreJ1 += howMany * grid[detectPos.ord][detectPos.abs];
                        }
                        else
                        {
                            scoreJ2 += howMany * grid[detectPos.ord][detectPos.abs];
                        }
                    }
                    else
                    {
                        scoreSolo += howMany * grid[detectPos.ord][detectPos.abs];
                    }
                }
                else
                {
                    if (pvp)
                    {
                        if (joueur == 1)
                        {
                            scoreJ1 += howMany;
                        }
                        else
                        {
                            scoreJ2 += howMany;
                        }
                    }
                    else
                    {
                        scoreSolo += howMany;
                    }
                }

                // MODE TRESOR
                if (treasureMode && howMany >= 4)
                {
                    int id = rand() % indices.size();
                    indicesTrouves[id] = true;

                    cout << "\n=== INDICE ===" << endl;
                    cout << "Lettre ROT13 : " << indices[id] << endl;
                    cout << "Décodée : " << rot13(indices[id]) << endl;
                    cout << "=============\n" << endl;
                }

                suiteTrouvee = true;
            }

            // COLONNES
            if (atLeastThreeInAColumn(grid, detectPos, howMany))
            {
                // ta version avec asciiMode dans removalInColumn
                removalInColumn(grid, detectPos, howMany, asciiMode);

                // SCORE
                if (asciiMode)
                {
                    if (pvp)
                    {
                        if (joueur == 1)
                        {
                            scoreJ1 += howMany * grid[detectPos.ord][detectPos.abs];
                        }
                        else
                        {
                            scoreJ2 += howMany * grid[detectPos.ord][detectPos.abs];
                        }
                    }
                    else
                    {
                        scoreSolo += howMany * grid[detectPos.ord][detectPos.abs];
                    }
                }
                else
                {
                    if (pvp)
                    {
                        if (joueur == 1)
                        {
                            scoreJ1 += howMany;
                        }
                        else
                        {
                            scoreJ2 += howMany;
                        }
                    }
                    else
                    {
                        scoreSolo += howMany;
                    }
                }

                // MODE TRESOR
                if (treasureMode && howMany >= 4)
                {
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

        if (!pvp && scoreSolo >= ScoreObjectif) {
            displayGrid(grid);
            couleur(KVert);
            cout << "\n==========================================" << endl;
            cout << "  BRAVO ! Objectif de " << ScoreObjectif<< " points atteint !" << endl;
            cout << "==========================================\n" << endl;
            couleur(KReset);
            return 1;
        }

        if (pvp)
        {
            if (joueur == 1)
            {
                joueur = 2;
            }
            else
            {
                joueur = 1;
            }
        }
    }

    // FIN DE PARTIE
    displayGrid(grid);
    cout << "PARTIE TERMINEE !" << endl;

    if (pvp)
    {
        cout << "Score final J1 : " << scoreJ1 << endl;
        cout << "Score final J2 : " << scoreJ2 << endl;
    }
    else
    {
        cout << "Score final : " << scoreSolo << endl;
    }

    if (treasureMode)
    {
        bool tousTrouves = true;

        for (bool b : indicesTrouves)
        {
            if (!b)
            {
                tousTrouves = false;
            }
        }

        if (tousTrouves)
        {
            cout << "\n=== CODE FINAL ===" << endl;
            cout << "Mr Casali" << endl;
            cout << "==================\n" << endl;
        }
        else
        {
            cout << "\nVous n'avez pas trouvé toutes les lettres..." << endl;
        }
    }

    return 0;
}


// ============================================================================
//  MODES DE JEU
// ============================================================================

int modeClassique(mat & grid, const size_t tailleGrille)
{
    initGrid(grid, tailleGrille, false, false);
    jouerPartie(grid, tailleGrille, false, false, false, 10, 20);
    return 0;
}

int mode1vs1(mat & grid, const size_t tailleGrille)
{
    initGrid(grid, tailleGrille, false, false);
    jouerPartie(grid, tailleGrille, true, false, false,10, 100000000);
    return 0;
}

int modeASCII(mat & grid, const size_t tailleGrille)
{
    initGrid(grid, tailleGrille, true, false);
    jouerPartie(grid, tailleGrille, false, true, false,10, 1000);
    return 0;
}

int modeASCIIPvP(mat & grid, const size_t tailleGrille)
{
    initGrid(grid, tailleGrille, true, false);
    jouerPartie(grid, tailleGrille, true, true, false,10, 10000000);
    return 0;
}

int modeTreasure(mat & grid, const size_t tailleGrille)
{
    afficherHistoireTresor();
    initGrid(grid, tailleGrille, false, true);
    jouerPartie(grid, tailleGrille, false, false, true, 20, 100000000);
    return 0;
}


// ============================================================================
//  MENU
// ============================================================================

int menu()
{
    int choix = 0;

    cout << "=============================" << endl;
    cout << "       CANDY CRUSH ++        " << endl;
    cout << "=============================" << endl;
    cout << " 1. Mode Classique" << endl;
    cout << " 2. Mode PvP" << endl;
    cout << " 3. Mode ASCII Classique" << endl;
    cout << " 4. Mode ASCII PvP" << endl;
    cout << " 5. Mode Chasse au Tresor" << endl;
    cout << " 6. Quitter" << endl;
    cout << "=============================" << endl;
    cout << " Votre choix : ";

    cin >> choix;
    return choix;
}


// ============================================================================
//  MAIN
// ============================================================================

int main()
{
    srand(time(NULL));

    mat grid;
    const size_t tailleGrille = 8;

    while (true)
    {
        int choix = menu();

        if (choix == 1)
        {
            modeClassique(grid, tailleGrille);
        }
        else if (choix == 2)
        {
            mode1vs1(grid, tailleGrille);
        }
        else if (choix == 3)
        {
            modeASCII(grid, tailleGrille);
        }
        else if (choix == 4)
        {
            modeASCIIPvP(grid, tailleGrille);
        }
        else if (choix == 5)
        {
            modeTreasure(grid, tailleGrille);
        }
        else if (choix == 6)
        {
            cout << "Au revoir !" << endl;
            return 0;
        }
        else
        {
            cout << "Choix invalide !" << endl;
        }

        cout << "Appuyez sur Entree pour revenir au menu..." << endl;
        cin.ignore();
        cin.get();
    }

    return 0;
}
