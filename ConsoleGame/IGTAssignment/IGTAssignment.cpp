using namespace std;

#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <fstream>
#include <format>
#include <sstream>
#include "Classes.h"
#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <cstdlib>
#include <math.h>

// Variables //
int RefreshRate = 5; // Per second
bool Verbose = false;


// Fixed variables //
bool GameRunning = true;
int PlayerScore = 0;
bool Error = false;
bool SilentError = false;
string ErrorReason = "";
CharacterObject Character("O");
MapObject MapData;
EnemyObject ObjectPool[10];


HANDLE ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
CONSOLE_CURSOR_INFO cursorInfo;


// Functions //
void vPrint(string Input) {
    if (Verbose == true) {
        cout << Input;
    }
}

void ClearConsole() { // Method prevents flickering
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    SetConsoleCursorPosition(ConsoleOut, coord);
}

void Color(char Input) {
    SetConsoleTextAttribute(ConsoleOut, Input); // Shortens
    
}

void LogError(string Input) {
    ErrorReason = Input;
    Error = true;
}

void LoadMap(int TargetMap) {
    stringstream ss;
    ss << "Map" << TargetMap << ".txt";
    ifstream MapFile(ss.str());
    if (MapFile.is_open()) {
        vPrint("Opened file");
        MapData.Id = TargetMap;
        for (int i = 0; i < MapYSize; i++) {
            string ReadLine;
            getline(MapFile, ReadLine);
            MapData.MapLines[i] = ReadLine;
        }
    }
    else {
        LogError("The map file could not be opened. Requested map file: '" + ss.str()+ "'");
    }
}

bool CalculateAttack() {

    return true;
}

void DisplayMap() {
    ClearConsole();
    string Insert = "";
    if (SilentError == true) {
        Insert = "!";
    }
    Color(4);
    cout << Insert << endl;

    int CharX = Character.XPos;
    int CharY = Character.YPos;
    int CurY = MapYSize - 1; // Starts top row

    bool Attack = false;
    string Movement = "";

    if (_kbhit() == 1) {
        char Input = _getch();
        if (Input == ' ') {
            Attack = true;
        }
        if (Input == 'w' || Input == 'a' || Input == 's' || Input == 'd') {
            Movement = Input;
        }
        //cout << Movement << "                                                  " << endl;
    }
    else {
        //cout << "no input                                                  " << endl;
    }

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

 

    for (int i = 0; i < MapYSize; i++) {
        string str = MapData.MapLines[i];
        cout << "   ";
        int CharCount = 0;
        for (char& c : str)
        {
            string Override = "";
            Color(7);
            if (CharCount == 0 or CharCount == 42 or i == 0 or i == MapYSize - 1) {
                Color(8);  
            } else {
                if (CharX == CharCount and CharY == CurY) {
                    Override = Character.GetSprite();
                    if (Attack == false) {
                        Color(Character.Color);
                    }
                    else {
                        if (CalculateAttack() == true) {
                            Override = "*";
                        }
                        Color(4);
                    }
                }
                if (c == '#') {
                    Color(6);
                }
            }
            if (Override != "") {
                cout << Override;
            } else {
                cout << c;
            }
            c = '*';
            CharCount++;
        }
        CurY--;
        cout << endl;
    }

    cout << endl;
    Color(7);
    cout << "      Current Level: " << MapData.Id << " | Current Score: " << PlayerScore << endl << endl;

    for (EnemyObject Enemy : ObjectPool) {
        Color(4);
        if (Enemy.IsActive() == true) {
            if (Enemy.GetHealth() > 0) {
                float HealthBars = 10;
                float HealthComponent = HealthBars / Enemy.GetMaxHealth();

                float HealthBlocks = round(HealthComponent * Enemy.GetHealth());
                float FillerBlocks = HealthBars - HealthBlocks;


                string Bars(HealthBlocks, '#');
                string Fill(FillerBlocks, ' ');

                cout << "         Enemy: [" + Bars + Fill + "] (" + to_string(Enemy.GetHealth()) + "/" + to_string(Enemy.GetMaxHealth()) + ")"  << endl;
            }
        }
    }

 
}



void vPrint(string Instruction, string InputString) {
    static vector<string> PrintData;

    if (Instruction == "Add") {
        PrintData.push_back(InputString);
        cout << "Data add request\n";
    }
}

void Logic() {
    DisplayMap();
}

int main()
{
    // Init

    ObjectPool[1].Init(0,0);
    ObjectPool[1].Damage(70);
    ObjectPool[2].Init(4, 4);
    ObjectPool[2].Damage(20);

    #ifndef _WIN32
        Color(4);
        cout << "This program is intended for Windows systems only." << endl;
        cout << "Press enter to continue anyway.";
        cin.get();
    #endif

    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(ConsoleOut, &cursorInfo);

    Character.Color = 3;
    LoadMap(1);
    
    system("cls");

    // Logic loop
    while (GameRunning == true && Error == false) {
        Logic();
        Sleep(1000/RefreshRate);
    }


    // Ending
    system("cls");
    Color(7);

    if (Error == false) {
        string Insertion = "";
        if (PlayerScore > 50) {
            Insertion = "high ";
        }

        cout << " The game has concluded!" << endl << endl;

        Color(2);
        cout << " You reached level " << MapData.Id << "!" << endl;
        cout << " You reached a " << Insertion << "score of " << PlayerScore << "!";
    }
    else {
        Color(4);
        cout << "An error has occured and the program has exited." << endl;
        if (ErrorReason != "") {
            cout << "The program reported an error: " << ErrorReason << endl;
        }
    }
    cin.get();
}