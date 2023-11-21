using namespace std;
#include <string>
#include <iostream>

const int MapYSize = 12; // Map size tracker, was unable to get this to automatically log

class InheritPosition { // Class which is inherited by multiple classes
public:
    int XPos = 0; // Contains coordinates
    int YPos = 0;
};

class CharacterObject : public InheritPosition { // New class while inheriting the one above
private:
    string Sprite; // Hidden class for string, can only be written to with validaiton from the public function

public:
    int Color = 3; // Character color, with default as 3.

    CharacterObject(string Input) { // Default constructor for characterobject, taking and formatting the input string to set as a sprite
        Input.resize(1); // Formatting
        Sprite = Input;
        return;
    }
    string GetSprite() { // Public function that retrieves and returns the sprite, as nothing else in the program will have access to it
        return Sprite;
    }

    void MoveX(int Input) { // Sets the coordinates. X and Y were originally private, but are now public with inheritance so these are somewhat redundant
        XPos = Input;
    }
    void MoveY(int Input) {
        YPos = Input;
    }
};

class EnemyObject : public InheritPosition { // Enemy object with inherited coordinates class
private:
    int Health = 0; // Default private values
    int MaxHealth = 0;
    string Type = "regular";

public:
    EnemyObject() {}; // Default constructor

    bool Active = false; // Active marker

    void Init(int InputX, int InputY) { // Init function. Constructor is not used as these objects are already created by the pool
        Active = true;
        XPos = InputX;
        YPos = InputY;
        Health = 100;
        MaxHealth = 100;
        return;
    }

    void SetType(string ReqType) { // Type requesting function
        if (Health == 100 and ReqType != "regular") { // Checks whether values are unique, and whether enemy is available for writing
            if (ReqType == "heavy") { // Check the type and append new values
                Type = ReqType;
                MaxHealth = 200;
                Health = 200;
            }
        }
    }

    pair <int, int> GetPos() { // GetPos for coordinates. Also has been made redundant but some lines use the pair type
        return make_pair(XPos, YPos);
    }

    string GetType() { // Returns the encapsulated type
        return Type;
    }

    bool IsActive() { // Returns active, slightly redundant
        return Active;
    }

    int GetHealth() { // Returns encapsulated health marker
        return Health;
    }

    int GetMaxHealth() { // Returns encapsulated value
        return MaxHealth;
    }

    bool Damage(int HitDamage) { // Function which changes the encapsulated variables. Returns true if wiped out
        Health -= HitDamage;
        if (Health <= 0) {
            Health = 0;
            Active = false;
            return true;
        }
        return false;
    }
};

class MapObject { // Simple object for map data. Can be seen as organising a list of variables in a different calss
public:
    int Id;
    string MapLines[MapYSize];
    int StartCoords[2];
    int EndCoords[2];
    int StartTime = 0;
};