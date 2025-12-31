#include <iostream>
#include <cstdlib>
#include <limits>

using namespace std;

void clearScreen()
{
    cout << "\033[H\033[2J";
}

int main()
{
    while (true)
    {
        clearScreen();
        int choix = 0;

        cout << "=============================" << endl;
        cout << "   CANDY CRUSH - LAUNCHER    " << endl;
        cout << "=============================" << endl;
        cout << " 1. Jouer au Mode Classique" << endl;
        cout << " 2. Jouer au Mode PvP" << endl;
        cout << " 3. Jouer au Mode Chasse au Tresor" << endl;
        cout << " 4. Quitter" << endl;
        cout << "=============================" << endl;
        cout << " Votre choix : ";

        cin >> choix;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choix == 1)
        {
            system("main.exe");

            cout << "Appuyez sur Entree pour revenir au menu..." << endl;
            cin.ignore(1000, '\n'); //Ignore tant que c'est pas Entrée
            cin.get();// Attends un touche pour revenir
        }
        else if (choix == 2)
        {
            system("Pvp.exe");

            cout << "Appuyez sur Entree pour revenir au menu..." << endl;
            cin.ignore(1000, '\n');
            cin.get();
        }
        else if (choix == 3)
        {
            system("Chasseautrésor.exe");

            cout << "Appuyez sur Entree pour revenir au menu..." << endl;
            cin.ignore(1000, '\n'); 
            cin.get();
        }
        else if (choix == 4)
        {
            break;
        }
    }
    return 0;
}