using namespace std;

#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

#include "Classes.h"

int RefreshRate = 3; // Per second
const int MapYSize = 10;

int CurrentMapId = 0;
int LastMapId = 1;
bool Verbose = true;


CharacterObject Character;

void vPrint(string Instruction, string InputString) {
    static vector<string> PrintData;

    if (Instruction == "Add") {
        PrintData.push_back(InputString);
        cout << "Data add request\n";
    }
}

void DisplayRefresh() {
    static string MapData[MapYSize];

    if (CurrentMapId != LastMapId) {
        cout << "Map update requested";

        LastMapId = CurrentMapId;
    }
    return;



}

int main()
{
    Character.SetSprite("O");

    //while (true) { // driver function
      //  DisplayRefresh;
    //    Sleep(1/RefreshRate);
   // }
}