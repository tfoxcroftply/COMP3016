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


// Variables //
int RefreshRate = 3; // Per second
const int MapYSize = 16;
int CurrentMap = 0;
string MapLines[MapYSize];

bool Verbose = false;
CharacterObject Character;


// Functions //

void vPrint(string Input) {
    if (Verbose == true) {
        cout << Input;
    }
}

void SetMap(int TargetMap) {
    stringstream ss;
    ss << "Map" << TargetMap << ".txt";
    ifstream MapFile(ss.str());
    if (MapFile.is_open()) {
        vPrint("Opened file");
        CurrentMap = TargetMap;
        for (int i = 0; i < MapYSize; i++) {
            string ReadLine;
            getline(MapFile, ReadLine);
            MapLines[i] = ReadLine;
        }
    }
}

void DisplayMap() {
    for (int i = 0; i < MapYSize; i++) {
        string str = MapLines[i];
        wstring ws(str.begin(), str.end());
        cout << str << endl;
    }
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

int main()
{
    Character.SetSprite("O");
    SetMap(1);
    DisplayMap();

}