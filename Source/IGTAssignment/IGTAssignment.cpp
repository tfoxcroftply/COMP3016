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
#pragma comment(lib, "winmm.lib") // Including lib for sound functionality

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

CharacterObject Character("O"); // Setting character icon, it will get validated by the object
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

double Clamp(double x, double upper, double lower) // Input number, upper range and lower range
{
    return min(upper, max(x, lower)); // Function for clamping an input number between a range
}

int GetUnix() {
    return chrono::system_clock::to_time_t(std::chrono::system_clock::now()); // returns the seconds since 1/1/1970, used for tracking relative time distances
}

void LoadMap(int TargetMap) { // Map loading function, loads and stores map upon request
    TimeoutSound = false; // Marker for checking whether the timeout sound has been played before
    if (TargetMap > Levels) { // Check to see if the game has reached the target number of levels
        GameRunning = false; // Marks the game to disable itself
        return; // Returns the loadmap function earlier as any more processing will not need to be done.
    }

    bool AnyActive = false; // Marker for whether any enemies were found existing from the old map
    for (EnemyObject &Enemy : ObjectPool) { // Collects and loops over the enemies in the object pool
        if (Enemy.Active == true) { // Checks if the enemy is marked as active
            AnyActive = true; // Logs that an enemy has been found
        }
        Enemy.Active = false; // Disables the enemy, it is the start of a new map
    }

    if (GetUnix() - MapData.StartTime <= GoodTimeLimit && AnyActive == false) { // Check to see if all enemies have been defeated, and whether the level was completed in time
        PlayerScore += 5; // If so, add an additional 5 points onto the player score
    }

    stringstream ss; // String stream for adding multiple strings together
    ss << "data\\Map" << TargetMap << ".txt"; // Creates the filename for the target file
    ifstream MapFile(ss.str()); // Opens the target file in read mode, after converting the stringstream to a single string
    int EnemyCounter = 0; // Simple tracker for finding how many enemies have been found
    if (MapFile.is_open()) { // Checks if target file opened
        vPrint("Opened file"); // Optional printing for testing
        MapData.Id = TargetMap; // Updates map id
        MapData.StartTime = GetUnix(); // Get the timestamp for future comparison
        for (int i = 0; i < MapYSize; i++) { // Reads the text file and stores it to the map object.
            string ReadLine;
            getline(MapFile, ReadLine); // Retrieves a single line from the map file, automatically reads consecutively
            
            int Counter = 0; // Counter for the below loop, as it does not use an int
            for (char& c : ReadLine) { // Cycle through each character in the string, checking for any markers
                
                string DesiredType = "none"; // Marker for enemy types, defined as "none" meaning nothing has been found
                if (c == 'R') { // If a marker of "R" has been found, set its type
                    DesiredType = "regular";
                } else if (c == 'H') { // Same for heavy type
                    DesiredType = "heavy";
                    
                }

                if (DesiredType != "none") { // Checks to see if the desiredtype has actually changed, menaing something has been found
                   ObjectPool[EnemyCounter].Init(Counter, MapYSize - 1 - i); // Initialise the enemy using the Init() function, passing the X,Y coordinates
                   ObjectPool[EnemyCounter].SetType(DesiredType); // Set the enemy type using the found type
                   EnemyCounter++; // Counter for enemy index tracking, to prevent overwriting a single object
                   c = ' '; // Removes the character from the line, otherwise the enemy marker will remain on the map when dead
                }
                Counter++; // Increase index counter now that loop has completed
            }

            MapData.MapLines[i] = ReadLine; // Appends the formatted line to the map data
        }
    }
    else {
        LogError("The map file could not be opened. Requested map file: '" + ss.str()+ "'"); // When the file is not available, it calls the error function with a reason
    }
} 

void PlayAudio(string InputName) { // Function for playing audio
    string FileName = "data\\" + InputName + ".wav"; // Formatting file name from input
    bool Success = PlaySoundA(FileName.c_str(), NULL, SND_FILENAME | SND_ASYNC); // Playing audio, using the formatted file name
    if (Success == false) { // Determines whether the sound failed based on its return
        LogError("The sound file '" + InputName + ".wav' was unable to be played."); // Log the error
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
        if ( SearchIndex != -1) { // Checks if enemy was actually found
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
    PlayAudio("next"); // Request audio file of "next.wav"
    LoadMap(MapData.Id + 1); // Loadmap with increased level
    Character.MoveX(1); // Set character starting positions
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
        string str = MapData.MapLines[i]; // Shortens the map data into a variable
        cout << "   "; // Spacer to offset the map to the right
        int CharCount = 0; // Index tracker for characters
        for (char& c : str) // Loop for every character in the selected line
        {
            string Override = ""; // Override variable, used for intercepting and plotting custom character
            Color(7); // Default color of white is set here
            if (CharCount == 0 or CharCount == 42 or i == 0 or i == MapYSize - 1) { // Checks to see whether the character position is a part of the map border
                Color(8); // If it is, colour it grey
            } else {
                if (CharX == CharCount and CharY == CurY) { // If the indexes match the character coordinates, this is the character location
                    Override = Character.GetSprite(); // Sets the override as the sprite symbol

                    int SearchValue = -1; // Position to search by, -1 means nothing has been found
                    char Direction = 'n'; // Direction tracker, n meaning nothing has been set
                    char DetectedChar = 'z'; // Character tracker, z meaning nothing has been found

                    if (Movement == "d") { // This is a rather inefficient method, but since I am new with C++, I was unable to solve this in any reasonable time
                        DetectedChar = str[CharCount + 1]; // Log the detected character
                        SearchValue = CharCount + 1; // Get the coordinates of the new position to search ...
                        Direction = 'x'; // along this axis
                    }
                    if (Movement == "a") { // Repeat above for every movement input
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

                    if (SearchValue != -1 && Direction != 'n') { // If values have been changed, something must have been found, so continue below
                        if (DetectedChar == ' ') { // Checks to see whether new character is a moveable space
                            int DesX = CharX; // Generate the new coordinates
                            int DesY = CharY;
                            if (Direction == 'y') { // Append search value against the desired axis
                                DesY = SearchValue;
                            }
                            else if (Direction == 'x') {
                                DesX = SearchValue;
                            }

                            if (CheckForEnemy(EnemyMap, pair<int, int>(DesX, DesY)) == false) { // Check for enemy presence, they won't appear on the map file so this has to be done manually
                                Character.MoveX(DesX); // If all is clear, move the character using the object functions
                                Character.MoveY(DesY);
                            }
                        } else if (DetectedChar == 'X'){ // The X symbol means the end of the map, so if this is detected ..
                            NextLevel(); // .. the new level will load
                            return; // and return early as nothing else needs to be done
                        }
                    }

                    if (Attack == false) { // Checks whether attacking is requested
                        Color(Character.Color); // By default, be the characters default colour
                    }
                    else { // Otherwise, attempt an attack
                        if (RequestDamage(make_pair(CharCount,CurY)) == true) { // Request damage, and if the character has been hit successfully, it will return true
                            Override = "*"; // Leading to the hit marker and sound playing
                            PlayAudio("hit");
                        }
                        Color(4); // Regardless of an enemy being hit, set the colour to red
                    }
                }

                if (CheckForEnemy(EnemyMap,pair<int,int>(CharCount,CurY)) == true) { // If no enemy has been found, then continue below.
                    Color(4);
                    Override = "O"; // This area has probably been made redundant, but as it works, I won't change it
                }

                if (c == '#') { // Setting colours for both walls, and the exit point
                    Color(6);
                }
                if (c == 'X') {
                    Color(2);
                }
            }
            if (Override != "") { // Checks whether a new character has been requested
                cout << Override; // If so, use that
            } else {
                cout << c; // Otherwise, use what the map has provided
            }
            //c = '*';
            CharCount++; // Update the index counter
        }
        CurY--; // And update the map Y tracker, which is inverted
        cout << endl; // Add a space for other data
    }

    cout << endl; // And another space here
    Color(7); // Reset colours to white, just in case it has been left on a different color
    cout << "       Current Level: " << MapData.Id << " | Current Score: " << PlayerScore << endl; // Display current map name, and player score
    float TimeMultiplier = float(10) / float(GoodTimeLimit); // Calculations to fit a range of data into a progress bar
    int TimeFrame = round(Clamp(MapData.StartTime + GoodTimeLimit - GetUnix(), GoodTimeLimit, 0)); // Clamping between 0 and the time limit, before rounding it
    int TimeFrameBounded = TimeFrame * TimeMultiplier; // Then applying the multiplier to make sure it scales properly

    if (MapData.StartTime + GoodTimeLimit + 1 <= GetUnix()) { // Checks to see if time has been reached
        if (TimeoutSound == false) { // If it has, and the sound hasn't been played yet ..
            TimeoutSound = true; // Play the sound and update the variable to signify such, to prevent from replays
            PlayAudio("timeout");
        }
        Color(4); // Mark the timer as red, to show that it has a problem
    }

    string FillBlocks = string(TimeFrameBounded, '#'); // Make the strings for the progress bar, using the calculations from above
    string SpaceBlocks = string(10 - TimeFrameBounded, ' ');

    cout << "            Time: [" << FillBlocks << SpaceBlocks << "] (" << TimeFrame << ")                " << endl << endl; // Use the newly created strings in a progress bar

    for (EnemyObject &Enemy : ObjectPool) { // For every enemy in the pool, loop over them
        Color(4); // Set to red by default
        if (Enemy.IsActive() == true) { // Checks to see if the enemy is marked active
            float HealthBars = 10; // Requested health bars to fit in the progress bar

            float HealthComponent = HealthBars / Enemy.GetMaxHealth(); // More calculations, similar to the timer progress bar
            float HealthBlocks = round(HealthComponent * Enemy.GetHealth());
            float FillerBlocks = HealthBars - HealthBlocks;


            string Bars(HealthBlocks, '#'); // Similar string creation
            string Fill(FillerBlocks, ' ');

            string InsertionType = ""; // Temporary insertion string
            string Type = Enemy.GetType(); // Get enemy type, for optional displaying
            if (Type != "regular") {  // If enemy is not regular, therefore a different type
                Type[0] = toupper(Type[0]); // Format the string and insert it in brackets
                InsertionType = "(" + Type + ")";
            }

            cout << "         Enemy: [" + Bars + Fill + "] (" + to_string(Enemy.GetHealth()) + "/" + to_string(Enemy.GetMaxHealth()) + ") " << InsertionType << "     " << endl; // Health bar, with insertion of any generated strings
        }
    }

    vPrint(to_string(MapData.StartTime)); // Optional verbose printing

    for (int i = 0; i < 2; i++) { // Loop a few times
        cout << string(50, ' ') << endl; // Make some spaces, to ensure that nothing else can go in its place. Must be done with my streamlined method of clearing, as it overwrites
    }
}



void vPrint(string Instruction, string InputString) { // Verbose printing function, optional, currently unused
    static vector<string> PrintData;

    if (Instruction == "Add") {
        PrintData.push_back(InputString);
        cout << "Data add request\n";
    }
}

void Logic() { // Function for displaying map logic, this is looped over. It is somewhat redundant and had more things planned, so it does nothing but redirect
    DisplayMap();
}

int main() // Main function of the program that is automatically run
{
    // Init;

    #ifndef _WIN32 // Checks whether the program is running on Windows, if not, it continues below
        Color(4);
        cout << "This program is intended for Windows systems only." << endl; // Display a warning to the user
        cout << "Press enter to continue anyway.";
        cin.get(); // Waits for user input
    #endif

    cursorInfo.dwSize = 100; // Setting parameters for hiding the input cursor
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(ConsoleOut, &cursorInfo);

    Character.Color = 3; // Character object initialisation
    Character.MoveX(1);
    Character.MoveY(1);

    cout << "Press any key to play"; // Display messages
    cin.get(); // Wait for the user to interact

    LoadMap(1); // Load the first map to begin the game.
    system("cls"); // Original clearing method to prevent bugs

    // Logic loop
    while (GameRunning == true && Error == false) { // While the game is marked to run, and no errors have been found
        Logic(); // Constantly request the logic loop
        Sleep(1000/RefreshRate); // And wait long enough to meet the desired refresh rate per second
    }


    // Ending
    system("cls"); // Clear absolutely everything
    Color(7); // Set to white

    if (Error == false) { // If an error has not been found, and the game is marked as complete, the user has finished the game
        string Insertion = ""; // Temporary insertion string
        if (PlayerScore > 50) { // If the player score is over 50
            Insertion = "high "; // Make the insertion string to add "high" before score below
        }

        cout << " The game has concluded!" << endl << endl; // Message displaying

        Color(2); // Set to green
        cout << " You reached level " << MapData.Id << "!" << endl; // Give player details
        cout << " You reached a " << Insertion << "score of " << PlayerScore << "!"; 
    }
    else { // An error has occured
        Color(4); // Set the colour to red
        cout << "An error has occured and the program has exited." << endl; // Warn the player
        if (ErrorReason != "") { // If there has been a reason logged
            cout << "The program reported an error: " << ErrorReason << endl; // Then output it below
        }
    }
    cin.get(); // Wait for user input
}