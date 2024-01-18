#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <ostream>
#include <fstream>
#include "GameState.h"
#include "Graph.h"
#include "Queue.h"


bool yes = false;

// Selects first empty spot on board (For easy computer)
Vec validMove(GameState game){
    for(int i = 0; i<game.size; i++){
        for(int j = 0; j<game.size; j++){
            if(game.grid[i][j] == -1){
                return Vec(i,j);
            }
        }
    }
    return Vec(0,0);
}

//HardDifficulty AI
Vec findBestMove(GameState game){
    Graph<GameState> stateSpace;
    Vertex<GameState>* start = new Vertex<GameState>(game);

    stateSpace.addVertex(start);

    Queue<Vertex<GameState>*> q;
    q.enqueue(start);

    while (!q.isEmpty())
    {
        Vertex<GameState>* curr = q.dequeue();
        if (!start->data.done)
        {
            for(int i = 0; i < game.size; i++)
            {
                for (int j = 0; i < game.size; j++)
                {
                    if(start->data.grid[i][j] == -1)
                    {
                        GameState next = start->data;
                        next.play(i, j);
                        Vertex<GameState>* successor = new Vertex<GameState>(next);
                        stateSpace.addVertex(successor);
                        stateSpace.addDirectedEdge(start, successor);
                        if(!successor->data.done)
                        {
                            q.enqueue(successor);
                        }
                    }
                }
            }
        }
    }
    return Vec(0,0);

    //for every successor of start, call getReward(successor, player);
}

// User Selects Size;
int chooseSize(){
    int option;
    system("clear");
    while(option != 1 && option != 2) {
        std::cout << "=========================" << std:: endl;
        std::cout << "       SELECT SIZE       " << std::endl;
        std::cout << "=========================" << std:: endl;
        std::cout << std::endl;
        std::cout << "1. 3x3" << std::endl;
        std::cout << "2. 4x4" << std::endl;
        std::cout << std::endl;
        std::cout << "SELECT AN OPTION: ";
        std::cin>>option;
    }
    
    switch(option) {
        case(1): return 3;
        case(2): return 4;
    }

    //arbitrary return
    return 3;
}

//reads the file like "player1"space"player2"
int readStats(std::string player){ 
    int x, o;
    std::ifstream MyReadFile("stats.txt");

    while (MyReadFile >> x >> o) {
        // std::cout << x << " " << o << std::endl;
    }

    if (player == "x") {
        return x;
    } else {
        return o;
    }
    
}

// Completely Clears Stats File ~ used for Updating {NOT FOR RESETING STATS}
void clearStatsFile() {
  std::ofstream outFile("stats.txt", std::ofstream::trunc);
  outFile << "";
}

//Saving function score 1, score 2 ('X' or 'O' as arguemnts)
void updateStatistics(char winner) {
  // Save individual stats to override later
  int xWins = readStats("x");
  int oWins = readStats("o");
  
  // Clear file to override later
  clearStatsFile();

  // Increment Wins for winner according to input of 'winner'
  if(winner == 'X'){
    xWins += 1;
  }
  else if(winner == 'O') {
    oWins += 1;
  }

    // Override Statistics in stats.txt with updated statistics
    std::ofstream outFile("stats.txt", std::ios::app);
    outFile << xWins << " " << oWins;
}

// Displays Statistics
void showStatistics(int& xWins, int& oWins){
    system("clear");

    int x = 0;
    int o = 0;

    //Finish code for x & y implementation

    std::cout << "=========================" << std:: endl;
    std::cout << "     STATISTICS PAGE     " << std::endl;
    std::cout << "=========================" << std:: endl;

    std::cout << std::endl;
    std::cout << "Player 1 has WON " << xWins << " times" << std::endl;
    std::cout << "Player 2 has WON " << oWins << " times" << std::endl;
    std::cout << std::endl;
    std::cout <<"------------------------------"<<std::endl << std::endl;
    if (yes)
    {
        std::cout << "The statistics have been reset." << std::endl;
    }
    std::cout << "Press [ENTER] to go back to menu: ";
    std::cin.ignore();
    std::string empty;
    getline(std::cin,empty);
}

// Resets Statistics (You can add this to menu if you want)
void resetStatistics() {
    yes = true;
    clearStatsFile();
    std::ofstream outFile("stats.txt", std::ios::app);
    outFile << "0 0";
    int xWins = 0; 
    int oWins = 0;
    showStatistics(xWins, oWins);
    yes = false;
}

// Function to return reward value of current state
int getReward(Vertex<GameState>* start, int player){
    if (start->neighbors.size() == 0){
        int currPlayer = start->data.currentTurn;
        if (start->data.hasWon(player)){
            return 100;
        }
        else if (start->data.hasWon(!player)){
            return -100;
        }
        else{
            return 50;
        }
    }
    else{
        int reward = getReward(start->neighbors[0]->location, player);
        for (int i = 1; i < start->neighbors.size(); i++){
            int curr = getReward(start->neighbors[i]->location, player);
            if (start->data.currentTurn == player){
                if (curr > reward){
                    reward = curr;
                }
            }
            else{
                if (curr < reward){
                    reward = curr;
                }
            }
        }
        return reward;
    }
}

// Finds best move for Hard AI 3x3
Vec findBestMove3(GameState game) {
    int depth = 0;
    Graph<GameState> stateSpace;
    Vertex<GameState>* start = new Vertex<GameState>(game);

    stateSpace.addVertex(start);

    Queue<Vertex<GameState>*> frontier;
    frontier.enqueue(start);

    int bestValue = -1000000000;
    int x = -1, y = -1;

    while (!frontier.isEmpty()) {
        Vertex<GameState>* curr = frontier.dequeue();
        if (!curr->data.done) {
            for (int i = 0; i < game.size; i++) {
                for (int j = 0; j < game.size; j++) {
                    if (curr->data.grid[i][j] == -1) {
                        GameState next = curr->data;
                        next.play(i, j);
                        Vertex<GameState>* successor = new Vertex<GameState>(next);
                        stateSpace.addVertex(successor);
                        stateSpace.addDirectedEdge(curr, successor);
                        int value = getReward(successor, 0); 
                        if (!successor->data.done) {
                            frontier.enqueue(successor);
                        }
                    }
                }
            }
        }
    }
    bestValue = -1000000000; 
    int player = game.currentTurn;
    for(int i = 0; i < start->neighbors.size(); i++) {
        
        int value = getReward(start->neighbors[i]->location, player);
        if(value > bestValue) {
            bestValue = value;
            x = start->neighbors[i]->location->data.lastMove.x;
            y = start->neighbors[i]->location->data.lastMove.y;
        }            
    }
    return Vec(x, y);
}

// Finds best move for Hard AI 4x4
Vec findBestMove4(GameState game) {
    Graph<GameState> stateSpace;
    Vertex<GameState>* start = new Vertex<GameState>(game);

    stateSpace.addVertex(start);

    Queue<Vertex<GameState>*> frontier;
    frontier.enqueue(start);

    int bestValue = -1000000000;
    int x = -1, y = -1;
    int depth = 0;


    while (!frontier.isEmpty()) {
        Vertex<GameState>* curr = frontier.dequeue();
        if (!curr->data.done) {
            for (int i = 0; i < game.size; i++) {
                for (int j = 0; j < game.size; j++) {
                    if (curr->data.grid[i][j] == -1) {
                        GameState next = curr->data;
                        next.play(i, j);
                        Vertex<GameState>* successor = new Vertex<GameState>(next);
                        stateSpace.addVertex(successor);
                        stateSpace.addDirectedEdge(curr, successor);
                        int value = getReward(successor, 0); 
                        if (depth<=3016) {
                            frontier.enqueue(successor);
                            depth++;
                        }
                    }
                }
            }
        }
    }
    bestValue = -1000000000; 
    int player = game.currentTurn;
    for(int i = 0; i < start->neighbors.size(); i++) {
        
        int value = getReward(start->neighbors[i]->location, player);
        if(value > bestValue) {
            bestValue = value;
            x = start->neighbors[i]->location->data.lastMove.x;
            y = start->neighbors[i]->location->data.lastMove.y;
            if(bestValue==100){ 
                break;
            }
        }            
    }
    return Vec(x, y);
}

// Program to run HardMode 3x3 (DONE)
void hardMode3() {
    GameState game(3);

    while (!game.done) {
        system("clear");
        std::cout << game << std::endl;

        int x, y;

        if (game.currentTurn) {
            std::cout << "Computer Chooses" << std::endl;
            Vec move = findBestMove3(game);
            x = move.x;
            y = move.y;
        } else{
            std::cout << std::endl;
            std::cout << "Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
            std::string input;
            int pos1, pos2;
            std::cin >> input;

            if(input.length() == 2 && 
            toupper(input[0]) >= 'A' && toupper(input[0]) <= 'C' &&
            input[1] >= '1' && input[1] <= '3') { 
                    y = int(toupper(input[0])) - 65;
                    x = int(input[1]) - 49;
            }
        }
        game.play(x,y);
    }

    // Print Message When Game is Over
    std::cout << std::endl;
    std::cout << game << std::endl;
    std::cout << std::endl;

    if(game.hasWon(0)){
        std::cout << "Player X has won"<< std::endl;
    } else if (game.hasWon(1)){
        std::cout << "Player O has won" << std::endl;
    }
    else {
        std::cout << "It's a tie" << std::endl;
    }

    std::cout << "Press [ENTER] to go to menu: ";

    std::cin.ignore();
    std::string empty;
    getline(std::cin, empty);
}

// Program to run HardMode 4x4 (WIP)
void hardMode4() {
    GameState game(4);

    while (!game.done) {
        system("clear");
        std::cout << game << std::endl;

        int x, y;

        if (game.currentTurn) {
            Vec move = findBestMove4(game);
            x = move.x;
            y = move.y;
        } else{
            std::cout << std::endl;
            std::cout << "Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
            std::string input;
            int pos1, pos2;
            std::cin >> input;

            if(input.length() == 2 && 
            toupper(input[0]) >= 'A' && toupper(input[0]) <= 'D' &&
            input[1] >= '1' && input[1] <= '4') { 
                    y = int(toupper(input[0])) - 65;
                    x = int(input[1]) - 49;
            }
        }
        game.play(x,y);
    }

    // Print Message When Game is Over
    std::cout << std::endl;
    std::cout << game << std::endl;
    std::cout << std::endl;

    if(game.hasWon(0)){
        std::cout << "Player X has won"<< std::endl;
    } else if (game.hasWon(1)){
        std::cout << "Player O has won" << std::endl;
    }
    else {
        std::cout << "It's a tie" << std::endl;
    }

    std::cout << "Press [ENTER] to go to menu: ";

    std::cin.ignore();
    std::string empty;
    getline(std::cin, empty);
}

// Function for Player vs Player (3x3)
void humanMode3(int& xWins, int& oWins){
    GameState game(3);
    while(!game.done){
        system("clear");
        int currentPLayer = game.getCurrentTurn()+1; 
        std::cout<<game<<std::endl;
        std::cout<<"Player " << currentPLayer <<": ";
        std::string input;
        int pos1, pos2;
        std::cin >> input;

        if(input.length() == 2 && 
        toupper(input[0]) >= 'A' && toupper(input[0]) <= 'C' &&
        input[1] >= '1' && input[1] <= '3') {
              
                pos2 = int(toupper(input[0])) - 65;
                pos1 = int(input[1]) - 49;
        }
        game.play(pos1,pos2);
    }

    // Print Message When Game is Over
    system("clear");
    std::cout << game << std::endl;
    std::cout << std::endl;

    if(game.hasWon(0)){
        std::cout << "Player X has won"<< std::endl;
        updateStatistics('X');
        
    } else if (game.hasWon(1)){
        std::cout << "Player O has won" << std::endl;
        updateStatistics('O');
    }
    else {
        std::cout << "It's a tie" << std::endl;
    }

    std::cout << "Press [ENTER] to go to menu: ";

    std::cin.ignore();
    std::string empty;
    getline(std::cin, empty);
}

// Function for Player vs Player (4x4)
void humanMode4(int& xWins, int& oWins){
    GameState game(4);
    while(!game.done){
        system("clear");
        int currentPLayer = game.getCurrentTurn()+1; 
        std::cout<<game<<std::endl;
        std::cout<<"Player " << currentPLayer <<": ";
        std::string input;
        int pos1, pos2;
        std::cin >> input;

        if(input.length() == 2 && 
        toupper(input[0]) >= 'A' && toupper(input[0]) <= 'D' &&
        input[1] >= '1' && input[1] <= '4') {
              
                pos2 = int(toupper(input[0])) - 65;
                pos1 = int(input[1]) - 49;
        }
        game.play(pos1,pos2);
    }

    // Print Message When Game is Over
    system("clear");
    std::cout << game << std::endl;
    std::cout << std::endl;

    if(game.hasWon(0)){
        std::cout << "Player X has won"<< std::endl;
        updateStatistics('X');
        
    } else if (game.hasWon(1)){
        std::cout << "Player O has won" << std::endl;
        updateStatistics('O');
    }
    else {
        std::cout << "It's a tie" << std::endl;
    }

    std::cout << "Press [ENTER] to go to menu: ";

    std::cin.ignore();
    std::string empty;
    getline(std::cin, empty);
}

// Function for running Player vs Player (And choosing size)
void playHumanMode(int& xWins, int& oWins) {
    int size = chooseSize();
    switch(size) {
        case(3):
            humanMode3(xWins, oWins);
            break;

        case(4):
            humanMode4(xWins, oWins);
            break;
    }
}

// Function for computer easy mode (3x3)
void easyMode3(){
    GameState game(3);
    while(!game.done){
        system("clear");
        std::cout << game << std::endl;
        int x, y;
        if(game.currentTurn){
            Vec move = validMove(game);
            x = move.x;
            y = move.y;
        } else{
            std::cout << std::endl;
            std::cout << "Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
            std::string input;
            int pos1, pos2;
            std::cin >> input;

            if(input.length() == 2 && 
            toupper(input[0]) >= 'A' && toupper(input[0]) <= 'C' &&
            input[1] >= '1' && input[1] <= '3') { 
                    y = int(toupper(input[0])) - 65;
                    x = int(input[1]) - 49;
            }
        }
        game.play(x,y);
    }

    system("clear");
    std::cout << game << std::endl;
    std::cout << std::endl;
    if(game.hasWon(0)){
        std::cout << "Player X has won"<< std::endl;
        updateStatistics('X');
    } else if (game.hasWon(1)){
        std::cout << "Player O has won" << std::endl;
        updateStatistics('O');
    }
    else {
        std::cout << "It's a tie" << std::endl;
    }
    std::cout << "Press [ENTER] to go to menu: ";
    std::cin.ignore();
    std::string empty;
    getline(std::cin,empty);

}

// Function for computer easy mode (4x4)
void easyMode4(){
    GameState game(4);
    while(!game.done){
        system("clear");
        std::cout << game << std::endl;
        int x, y;
        if(game.currentTurn){
            Vec move = validMove(game);
            x = move.x;
            y = move.y;
        } else{
            std::cout << std::endl;
            std::cout << "Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
            std::string input;
            int pos1, pos2;
            std::cin >> input;

            if(input.length() == 2 && 
            toupper(input[0]) >= 'A' && toupper(input[0]) <= 'D' &&
            input[1] >= '1' && input[1] <= '4') { 
                    y = int(toupper(input[0])) - 65;
                    x = int(input[1]) - 49;
            }
        }
        game.play(x,y);
    }

    system("clear");
    std::cout << game << std::endl;
    std::cout << std::endl;
    if(game.hasWon(0)){
        std::cout << "Player X has won"<< std::endl;
        updateStatistics('X');
    } else if (game.hasWon(1)){
        std::cout << "Player O has won" << std::endl;
        updateStatistics('O');
    }
    else {
        std::cout << "It's a tie" << std::endl;
    }
    std::cout << "Press [ENTER] to go to menu: ";
    std::cin.ignore();
    std::string empty;
    getline(std::cin,empty);

}

// Prints Menu Options for User
void menu(){
    system("clear");
    std::cout << "============================" << std:: endl;
    std::cout << "   WELCOME TO TIC-TAC-TOE   " << std::endl;
    std::cout << "============================" << std:: endl;

    std::cout<<std::endl;
    std::cout << "1. Play against Human" << std::endl;
    std::cout << "2. Play against Computer"  << std::endl;
    std::cout << "3. How to Play" << std::endl;
    std::cout << "4. Display Statistics" << std::endl;
    std::cout << "5. Reset Statistics" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "SELECT AN OPTION: ";
}

// Function to choose Difficulty for player vs computer (both difficulty)
void chooseDifficulty(){
    // Completed
    system("clear");
    std::cout << "=========================" << std:: endl;
    std::cout << "    SELECT DIFFICULTY     " << std::endl;
    std::cout << "=========================" << std:: endl;
    std::cout << std::endl;
    std::cout << "1. Easy Mode" << std::endl;
    std::cout << "2. Hard Mode" << std::endl;
    std::cout << std::endl;
    std::cout << "SELECT AN OPTION: ";

    int option;
    std::cin>>option;

    // User Selects Difficulty // size;
    int size = chooseSize();
    switch(option){
        case 1:
            switch(size) {
                case(3):
                    easyMode3();
                    break;
                case(4):
                    easyMode4();
                    break;
            }
            break;
        case 2:
            // hardMode3();
            switch(size) {
                case(3): 
                    hardMode3();
                    break;
                case(4): 
                    hardMode4();
                    break;
            }
            break;
    }
}

// Function to print rules of Tic-Tac-Toe
void printRules() {
    std::cout << "=========================" << std:: endl;
    std::cout << "       HOW TO PLAY       " << std::endl;
    std::cout << "=========================" << std:: endl << std::endl;
    std::cout << "[*] Player 1 and Player 2 play the game is played on a 3x3 grid." << std::endl;
    std::cout << "[*] Player 1 goes first and places an X in any empty square." << std::endl;
    std::cout << "[*] Player 2 then places an O in any empty square." << std::endl;
    std::cout << "[*] Players take turns placing their marks in empty squares until one player gets 3 of their marks in a row, either horizontally, vertically, or diagonally." << std::endl;
    std::cout << "[*] If all 9 squares are filled and no player has 3 in a row, the game is a tie." << std::endl << std::endl;
    std::cout << "Example: If player 1 inputs A1:" << std::endl;
    std::cout << "    A   B   C  " << std::endl;
    std::cout << "   --- --- --- " << std::endl;
    std::cout << "1 | X |   |   |" << std::endl;
    std::cout << "   --- --- --- " << std::endl;
    std::cout << "2 |   |   |   |" << std::endl;
    std::cout << "   --- --- --- " << std::endl;
    std::cout << "3 |   |   |   |" << std::endl;
    std::cout << "   --- --- --- " << std::endl;

    std::cout << "Press [ENTER] to go back to the main menu: " << std::endl;
    
    std::cin.ignore();
    std::string empty;
    getline(std::cin,empty);
}
#endif