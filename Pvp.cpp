#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

using namespace std;

typedef vector<unsigned> line;
typedef vector<line> mat;

struct maPosition
{
    unsigned abs;
    unsigned ord;
};

const unsigned KReset(0);
const unsigned KNbCandies = 4;

void clearScreen()
{
    cout << "\033[H\033[2J";
}

void couleur(const unsigned &coul)
{
    cout << "\033[" << coul << "m";
}

bool combin(const mat &grid, size_t i, size_t j, unsigned val)
{
    if (j >= 2 && grid[i][j - 1] == val && grid[i][j - 2] == val)
        return true;
    if (i >= 2 && grid[i - 1][j] == val && grid[i - 2][j] == val)
        return true;
    return false;
}

void initGrid(mat &grid, const size_t &matSize)
{
    grid.resize(matSize);
    for (size_t i = 0; i < matSize; ++i)
    {
        grid[i].resize(matSize);
        for (size_t j = 0; j < matSize; ++j)
        {
            unsigned val;
            do
            {
                val = rand() % KNbCandies + 1;
            } while (combin(grid, i, j, val));
            grid[i][j] = val;
        }
    }
}

void displayGrid(const mat &grid, unsigned scoreJ1, unsigned scoreJ2, unsigned joueur)
{
    const size_t matSize = grid.size();
    clearScreen();
    cout << "\033[0m";

    cout << "=== MODE PvP ===" << endl;
    cout << "Score J1 : " << scoreJ1 << " | Score J2 : " << scoreJ2 << endl;
    cout << "C'est au JOUEUR " << joueur << " de jouer !" << endl
         << endl;

    for (size_t i = 0; i < matSize; ++i)
    {
        for (size_t j = 0; j < matSize; ++j)
        {
            if (grid[i][j] >= 1 && grid[i][j] <= KNbCandies)
            {
                couleur(30 + grid[i][j]);
                cout << grid[i][j] << " ";
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

void makeAMove(mat &grid, const maPosition &pos, const char &direction)
{
    maPosition newPos = pos;

    if (direction == 'Z')
    {
        if (pos.ord > 0)
            newPos.ord -= 1;
    }
    else if (direction == 'S')
    {
        if (pos.ord < grid.size() - 1)
            newPos.ord += 1;
    }
    else if (direction == 'Q')
    {
        if (pos.abs > 0)
            newPos.abs -= 1;
    }
    else if (direction == 'D')
    {
        if (pos.abs < grid.size() - 1)
            newPos.abs += 1;
    }
    else
    {
        return;
    }
    swap(grid[pos.ord][pos.abs], grid[newPos.ord][newPos.abs]);
}

bool atLeastThreeInAColumn(const mat &grid, maPosition &pos, unsigned &howMany)
{
    const size_t matSize = grid.size();

    for (size_t j = 0; j < matSize; ++j)
    {
        for (size_t i = 0; i < matSize; ++i)
        {
            if (grid[i][j] != 0)
            {
                unsigned count = 1;
                while (i + count < matSize && grid[i + count][j] == grid[i][j])
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

bool atLeastThreeInARow(const mat &grid, maPosition &pos, unsigned &howMany)
{
    const size_t matSize = grid.size();

    for (size_t i = 0; i < matSize; ++i)
    {
        for (size_t j = 0; j < matSize; ++j)
        {
            if (grid[i][j] != 0)
            {
                unsigned count = 1;
                while (j + count < matSize && grid[i][j + count] == grid[i][j])
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

void removalInColumn(mat &grid, const maPosition &pos, unsigned howMany)
{
    size_t taille = grid.size();
    size_t col = pos.abs;

    for (size_t i = pos.ord; i < taille - howMany; ++i)
    {
        grid[i][col] = grid[i + howMany][col];
    }

    for (size_t i = taille - howMany; i < taille; ++i)
    {
        grid[i][col] = rand() % KNbCandies + 1;
    }
}

void removalInRow(mat &grid, const maPosition &pos, unsigned howMany)
{
    for (unsigned k = 0; k < howMany; ++k)
    {
        maPosition currentPos;
        currentPos.ord = pos.ord;
        currentPos.abs = pos.abs + k;

        removalInColumn(grid, currentPos, 1);
    }
}

unsigned lireMeilleurScore()
{
    ifstream file("meilleurScore.txt");
    unsigned score = 0;
    if (file)
        file >> score;
    return score;
}

void enregistrerMeilleurScore(unsigned score)
{
    ofstream file("meilleurScore.txt");
    file << score;
}

int main()
{
    srand(time(NULL));
    mat grid;
    const size_t tailleGrille = 8;
    initGrid(grid, tailleGrille);

    const unsigned NbCoupsMax = 6;
    unsigned nbCoups = 0;

    unsigned scoreJ1 = 0;
    unsigned scoreJ2 = 0;
    unsigned joueurActuel = 1;

    maPosition pos;
    char direction;

    while (nbCoups < NbCoupsMax)
    {
        displayGrid(grid, scoreJ1, scoreJ2, joueurActuel);
        cout << "Coups restants : " << NbCoupsMax - nbCoups << endl;
        cout << "J" << joueurActuel << " -> Entrez Ligne Col Dir (ex: 4 5 Z) :";

        cin >> pos.ord >> pos.abs >> direction;

        if (pos.ord >= tailleGrille || pos.abs >= tailleGrille ||
            (direction != 'Z' && direction != 'S' && direction != 'Q' && direction != 'D'))
        {
            cout << "Mauvaise saisie ! Reessayez." << endl;
            continue;
        }

        makeAMove(grid, pos, direction);

        maPosition testPos;
        unsigned testCount;
        bool coupValide = false;

        if (atLeastThreeInARow(grid, testPos, testCount) || atLeastThreeInAColumn(grid, testPos, testCount))
        {
            coupValide = true;
        }

        if (coupValide)
        {
            bool suiteTrouvee = true;
            while (suiteTrouvee)
            {
                suiteTrouvee = false;
                unsigned howMany;
                maPosition detectPos;

                if (atLeastThreeInARow(grid, detectPos, howMany))
                {
                    removalInRow(grid, detectPos, howMany);
                    if (joueurActuel == 1)
                        scoreJ1 += howMany;
                    else
                        scoreJ2 += howMany;
                    suiteTrouvee = true;
                }

                if (atLeastThreeInAColumn(grid, detectPos, howMany))
                {
                    removalInColumn(grid, detectPos, howMany);
                    if (joueurActuel == 1)
                        scoreJ1 += howMany;
                    else
                        scoreJ2 += howMany;
                    suiteTrouvee = true;
                }

                if (howMany > 3)
                {
                    cout << "Wow !" << endl;
                }
            }
        }
        else
        {
            makeAMove(grid, pos, direction);
            cout << "Mauvais coup !" << endl;
        }

        nbCoups++;
        joueurActuel = (joueurActuel == 1) ? 2 : 1;
    }

    displayGrid(grid, scoreJ1, scoreJ2, joueurActuel);
    cout << "PARTIE TERMINEE !" << endl;
    cout << "Score Final J1 : " << scoreJ1 << endl;
    cout << "Score Final J2 : " << scoreJ2 << endl;

    if (scoreJ1 > scoreJ2)
        cout << "VICTOIRE JOUEUR 1 !" << endl;
    else if (scoreJ2 > scoreJ1)
        cout << "VICTOIRE JOUEUR 2 !" << endl;
    else
        cout << "EGALITE !" << endl;

    unsigned scoreMax = (scoreJ1 > scoreJ2 ? scoreJ1 : scoreJ2);
    unsigned meilleurScore = lireMeilleurScore();
    if (scoreMax > meilleurScore)
    {
        enregistrerMeilleurScore(scoreMax);
        cout << "NOUVEAU RECORD ENREGISTRE : " << scoreMax << endl;
    }

    return 0;
}