#include <bits/c++config.h>

#include <iostream>
#include <system_error>
#include "GameState.h"
#include "Graph.h"
#include "Vertex.h"
#include "helper.h"
#include <Queue.h>

using namespace std;

int main(){
    GameState game;

    int option;
    int xWins = readStats("x");
    int oWins = readStats("o");

    // Print Menu Options
    menu();

    while(cin >> option){
        system("clear");
        switch(option){
            case 1:
            // User selects humanMode
                playHumanMode(xWins, oWins);
                break;
            case 2:
            // User selects between easyMode or hardMode
                chooseDifficulty();
                break;
            case 3:
            // Prints Rules of Tic-Tac-Toe
                system("clear");
                printRules();
                break;
            case 4:
            // Displays statistics page
                xWins = readStats("x"); 
                oWins = readStats("o");
                showStatistics(xWins, oWins);
                break;
            case 5:
            // Clears Statistics
                system("clear");
                resetStatistics();
                break;
            case 6:
            // Exits program
                system("clear");
                return 0;
        }
        menu();
    }
    return 0;
}