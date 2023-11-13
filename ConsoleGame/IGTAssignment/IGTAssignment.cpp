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


// Variables //
int RefreshRate = 3; // Per second
bool Verbose = false;


// Fixed variables //
bool GameRunning = true;
int PlayerScore = 0;
bool Error = false;
bool SilentError = false;
string ErrorReason = "";
CharacterObject Character("O");
MapObject MapData;

HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
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
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Color(int Input) {
    SetConsoleTextAttribute(Console, Input); // Shortens
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

void DisplayMap() {
    ClearConsole();
    string Insert = "";
    if (SilentError == true) {
        Insert = "!";
    }
    Color(4);
    cout << Insert << endl;

    for (int i = 0; i < MapYSize; i++) {
        string str = MapData.MapLines[i];
        cout << "   ";
        for (char& c : str)
        {
            Color(7);
            if (c == '#') {
                Color(8);
            }
            cout << c;
            c = '*';
        }
        cout << endl;
    }
    cout << endl;
    Color(7);
    cout << "      Current Level: " << MapData.Id << " | Current Score: " << PlayerScore << endl;
}

int GetKeyboardInput() {
    int Input = _getch();
    return Input;
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

    #ifndef _WIN32
        Color(4);
        cout << "This program is intended for Windows systems only." << endl;
        cout << "Press enter to continue anyway.";
        cin.get();
    #endif

    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(Console, &cursorInfo);
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
        cout << "A fatal error has occured and the program has exited." << endl;
        if (ErrorReason != "") {
            cout << "The program reported an error: " << ErrorReason << endl;
        }
    }
    cin.get();
}