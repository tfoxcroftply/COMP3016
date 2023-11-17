using namespace std;
#include <string>
const int MapYSize = 12;

class CharacterObject {
private:
    string Sprite;


public:
    int XPos = 1;
    int YPos = 2;
    int Color = 3;

    CharacterObject(string Input) {
        Input.resize(1);
        Sprite = Input;
        return;
    }
    string GetSprite() {
        return Sprite;
    }

    void MoveX(int Input) {
        XPos = Input;
    }
    void MoveY(int Input) {
        YPos = Input;
    }
};

class EnemyObject {
private:
    bool Active = false;
    int Health = 0;
    int XPos = 0;
    int YPos = 0;
    int MaxHealth = 0;

public:
    EnemyObject() {};

    void Init(int InputX, int InputY) {
        Active = true;
        XPos = InputX;
        YPos = InputY;
        Health = 100;
        MaxHealth = 100;
        return;
    }

    void SetType(string Type) {
        if (Health == 100 and Type != "regular") {
            if (Type == "Heavy") {
                MaxHealth = 200;
                Health = 200;
            }
        }
    }

    pair <int, int> GetPos() {
        return make_pair(XPos, YPos);
    }

    bool IsActive() {
        return Active;
    }

    int GetHealth() {
        return Health;
    }
    int GetMaxHealth() {
        return MaxHealth;
    }

    bool Damage(int HitDamage) {
        Health -= HitDamage;
        if (Health <= 0) {
            delete this;
            return true;
        }
    }
};

class MapObject {
public:
    int Id;
    string MapLines[MapYSize];
    int StartCoords[2];
    int EndCoords[2];
};