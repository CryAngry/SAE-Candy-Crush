#include <iostream> 
#include <fstream>

using namespace std;



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
  cout << "     Chasse aux trésor     " << endl;
  cout << "     Voir le leaderboard   " << endl;
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


int main() {


    while (true) {

        int choix = menu();

        if (choix == 1) {
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

    return 0;
}

