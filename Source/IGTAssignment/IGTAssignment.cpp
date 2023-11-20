using namespace std; // Setting the namespace to std, prevents the requirement to use std:: 

#include <iostream> // Defining dependencies for this project
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
#include <mmsystem.h>
#include <chrono>
#pragma comment(lib, "winmm.lib")

// Variables // These are user changeable
int RefreshRate = 10; // Per second
bool Verbose = false;

int GoodTimeLimit = 25; // Extra 5 points if round is completed in this time
int Levels = 8;

pair<int, int> SpawnLocation = make_pair(2, 2); // X,Y locations
pair<int, int> EndLocation = make_pair(40, 10);

// Fixed variables //
bool GameRunning = true; // Indicator for main loop
int PlayerScore = 0; // Score tracker for player
bool Error = false; // Error tracking
bool SilentError = false;
bool TimeoutSound = false; // play tracker
string ErrorReason = ""; 

CharacterObject Character("O"); // Setting character icon
MapObject MapData; // Defining map object
EnemyObject ObjectPool[10]; // Array of multiple enemy objects


HANDLE ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE); // Console operations, helps for console based commands
HANDLE ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
CONSOLE_CURSOR_INFO cursorInfo;


// Functions // Verbose printing, uses a variable to decide whether to print
void vPrint(string Input) {
    if (Verbose == true) {
        cout << Input; // Outputs the input only if verbose is enabled
    }
}

void ClearConsole() { // Method overwrites existing content instead of clearing it, prevents flickering
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    SetConsoleCursorPosition(ConsoleOut, coord);
}

void Color(char Input) { // Function for setting color, shortens it and lessens complexity.
    SetConsoleTextAttribute(ConsoleOut, Input);
}

void LogError(string Input) { // Error logging. When called, it will halt the program and update the reason if provided
    ErrorReason = Input;
    Error = true;
}

double Clamp(double x, double upper, double lower)
{
    return min(upper, max(x, lower));
}

int GetUnix() {
    return chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void LoadMap(int TargetMap) { // Map loading function, loads and stores map upon request
    TimeoutSound = false;
    if (TargetMap > Levels) {
        GameRunning = false;
        return;
    }

    bool AnyActive = false;
    for (EnemyObject &Enemy : ObjectPool) {
        if (Enemy.Active == true) {
            AnyActive = true;
        }
        Enemy.Active = false;
    }

    if (GetUnix() - MapData.StartTime <= GoodTimeLimit && AnyActive == false) {
        PlayerScore += 5;
    }

    stringstream ss;
    ss << "data\\Map" << TargetMap << ".txt"; // Creates the filename for the target file
    ifstream MapFile(ss.str()); // Opens the target file in read mode
    int EnemyCounter = 0;
    if (MapFile.is_open()) { // Checks if target file opened
        vPrint("Opened file"); // Optional printing for testing
        MapData.Id = TargetMap; // Updates map id
        MapData.StartTime = GetUnix();
        for (int i = 0; i < MapYSize; i++) { // Reads the text file and stores it to the map object.
            string ReadLine;
            getline(MapFile, ReadLine);
            
            int Counter = 0;
            for (char& c : ReadLine) {
                
                string DesiredType = "none";
                if (c == 'R') {
                    DesiredType = "regular";
                } else if (c == 'H') {
                    DesiredType = "heavy";
                    
                }

                if (DesiredType != "none") {
                   ObjectPool[EnemyCounter].Init(Counter, MapYSize - 1 - i);
                   ObjectPool[EnemyCounter].SetType(DesiredType);
                   EnemyCounter++;
                   c = ' ';
                }
                Counter++;                
            }

            MapData.MapLines[i] = ReadLine;
        }
    }
    else {
        LogError("The map file could not be opened. Requested map file: '" + ss.str()+ "'"); // When the file is not available, it calls the error function with a reason
    }
} 

void PlayAudio(string InputName) { // Function for playing audio
    string FileName = "data\\" + InputName + ".wav"; // Formatting file name from input
    bool Success = PlaySoundA(FileName.c_str(), NULL, SND_FILENAME | SND_ASYNC); // Playing audio
    if (Success == false) {
        LogError("The sound file '" + InputName + ".wav' was unable to be played.");
    }
}

int GetEnemyByPos(pair<int,int> Location){ // Retrieves an enemy object from the pool based on inputted location
    int CurIndex = 0; // Index tracking
    for (EnemyObject &Enemy : ObjectPool) { // Loop for each object in pool
        if (Enemy.IsActive() == true) { // Checks whether enemy is active
            if (get<0>(Enemy.GetPos()) == get<0>(Location) && get<1>(Enemy.GetPos()) == get<1>(Location)) { // Checks location with enemy
                return CurIndex; // Returns enemy index if success
            }
        }
        CurIndex++; // Index update after loop
    }
    return -1; // Returns -1, signifying error
} 

bool CheckForEnemy(vector<pair<int,int>> EnemyPosPool,pair<int,int> TestPos){ // Checks for enemy presence based on location
    if (GetEnemyByPos(TestPos) != -1) { // Checks to see if return was valid
        return true; // If valid, true would return, signifying enemy is present.
    }
    return false; // Default return, saying no enemy was found
}

bool RequestDamage(pair<int, int> Location) { // Requests damage for an enemy based on location
    vector<pair<int, int>> Locations = { // Generates 4 X,Y coordinates to test around a player
        make_pair(get<0>(Location) + 1,get<1>(Location)),
        make_pair(get<0>(Location) - 1,get<1>(Location)),
        make_pair(get<0>(Location),get<1>(Location) + 1),
        make_pair(get<0>(Location),get<1>(Location) - 1),
    };
    for (pair<int, int> LocationPair : Locations) { // For each point to test, loop
        int SearchIndex = GetEnemyByPos(LocationPair); // Get an enemy object, if it is present in the location tested
        if ( SearchIndex != -1) {
            EnemyObject &Object = ObjectPool[SearchIndex]; // Declare temporary enemy variable
            if (Object.Damage(10) == true) { // Damage it by 10. Returns true if the enemy has been defeated
                PlayAudio("kill"); // Request audio play with the audio name
                PlayerScore += 10; // Add score to player
            }
            return true; // Returns true, saying that damage was successful to the caller
        }
    }
    return false; // Returns false, meaning nothing has happened.
}

void NextLevel() { // Simple function for loading a new level. Moves character to start
    PlayAudio("next");
    LoadMap(MapData.Id + 1);
    Character.MoveX(1);
    Character.MoveY(1);
}

void DisplayMap() { // Display loop for map
    ClearConsole(); // Ask for the display to be refreshed, as it is new

    string Insert = ""; // Simple error tracking icon data
    if (SilentError == true) {
        Insert = "!";
    }
    Color(4);
    cout << Insert << endl; // End of error tracking icon

    int CharX = Character.XPos; // Retrieve position data from character
    int CharY = Character.YPos;
    int CurY = MapYSize - 1; // Starts the index from top row, helps with understanding
    

    bool Attack = false;
    string Movement = ""; // Movement direction tracking variable

    if (_kbhit() == 1) { // Chnecks to see if keyboard key is actively being pressed, getch() sticks to the last character
        char Input = _getch(); // Get the key being pressed
        if (Input == ' ') { // Attack if spacebar has been pressed
            Attack = true;
        }
        if (Input == 'w' || Input == 'a' || Input == 's' || Input == 'd') { // Move if WASD is used
            Movement = Input; // Updates movement variable with key
        }
    }
    else {
        //cout << "no input                                                  " << endl; For development purposes, this is unused
    }

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // Refreshes getch(), it can be temperamental without clearing input buffer

    vector<pair<int,int>> EnemyMap = {}; // Vector generation of X,Y enemy coordinates for plotting purposes. I see it as a dynamic array
    for (EnemyObject &Enemy : ObjectPool) { // Loop every enemy that exists
        if (Enemy.IsActive() == true) { // Enemies always exist, but only select few are active, so this filters them out
            pair <int,int> Pos = Enemy.GetPos(); // Extract the enemies' position
            EnemyMap.push_back(Pos); // Then store it in the created vector above
        }
    }

    for (int i = 0; i < MapYSize; i++) { // Loop for every line stored in map data
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

                    int SearchValue = -1;
                    char Direction = 'n';
                    char DetectedChar = 'z';

                    if (Movement == "d") { // This is a rather inefficient method, but since I am new with C++, I was unable to solve this in any reasonable time
                        DetectedChar = str[CharCount + 1];
                        SearchValue = CharCount + 1;
                        Direction = 'x';
                    }
                    if (Movement == "a") {
                        DetectedChar = str[CharCount - 1];
                        SearchValue = CharCount - 1;
                        Direction = 'x';
                    }
                    if (Movement == "w") {
                        DetectedChar = MapData.MapLines[i - 1][CharCount];
                        SearchValue = CurY + 1;
                        Direction = 'y';
                    }
                    if (Movement == "s") {
                        DetectedChar = MapData.MapLines[i + 1][CharCount];
                        SearchValue = CurY - 1;
                        Direction = 'y';
                    }

                    if (SearchValue != -1 && Direction != 'n') {
                        if (DetectedChar == ' ') {
                            int DesX = CharX;
                            int DesY = CharY;
                            if (Direction == 'y') {
                                DesY = SearchValue;
                            }
                            else if (Direction == 'x') {
                                DesX = SearchValue;
                            }

                            if (CheckForEnemy(EnemyMap, pair<int, int>(DesX, DesY)) == false) {
                                Character.MoveX(DesX);
                                Character.MoveY(DesY);
                            }
                        } else if (DetectedChar == 'X'){
                            NextLevel();
                            return;
                        }
                    }

                    if (Attack == false) {
                        Color(Character.Color);
                    }
                    else {
                        if (RequestDamage(make_pair(CharCount,CurY)) == true) {
                            Override = "*";
                            PlayAudio("hit");
                        }
                        Color(4);
                    }
                }

                if (CheckForEnemy(EnemyMap,pair<int,int>(CharCount,CurY)) == true) {
                    Color(4);
                    Override = "O";
                }

                if (c == '#') {
                    Color(6);
                }
                if (c == 'X') {
                    Color(2);
                }
            }
            if (Override != "") {
                cout << Override;
            } else {
                cout << c;
            }
            //c = '*';
            CharCount++;
        }
        CurY--;
        cout << endl;
    }

    cout << endl;
    Color(7);
    cout << "       Current Level: " << MapData.Id << " | Current Score: " << PlayerScore << endl;
    float TimeMultiplier = float(10) / float(GoodTimeLimit);
    int TimeFrame = round(Clamp(MapData.StartTime + GoodTimeLimit - GetUnix(), GoodTimeLimit, 0));
    int TimeFrameBounded = TimeFrame * TimeMultiplier;

    if (MapData.StartTime + GoodTimeLimit + 1 <= GetUnix()) {
        if (TimeoutSound == false) {
            TimeoutSound = true;
            PlayAudio("timeout");
        }
        Color(4);
    }

    string FillBlocks = string(TimeFrameBounded, '#');
    string SpaceBlocks = string(10 - TimeFrameBounded, ' ');

    cout << "            Time: [" << FillBlocks << SpaceBlocks << "] (" << TimeFrame << ")                " << endl << endl;

    for (EnemyObject &Enemy : ObjectPool) {
        Color(4);
        if (Enemy.IsActive() == true) {
            float HealthBars = 10;
            float HealthComponent = HealthBars / Enemy.GetMaxHealth();

            float HealthBlocks = round(HealthComponent * Enemy.GetHealth());
            float FillerBlocks = HealthBars - HealthBlocks;


            string Bars(HealthBlocks, '#');
            string Fill(FillerBlocks, ' ');

            string InsertionType = "";
            string Type = Enemy.GetType();
            if (Type != "regular") {
                Type[0] = toupper(Type[0]);
                InsertionType = "(" + Type + ")";
            }

            cout << "         Enemy: [" + Bars + Fill + "] (" + to_string(Enemy.GetHealth()) + "/" + to_string(Enemy.GetMaxHealth()) + ") " << InsertionType << "     " << endl;
        }
    }

    vPrint(to_string(MapData.StartTime));

    for (int i = 0; i < 2; i++) {
        cout << string(50, ' ') << endl;
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
    // Init;

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
    Character.MoveX(1);
    Character.MoveY(1);
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