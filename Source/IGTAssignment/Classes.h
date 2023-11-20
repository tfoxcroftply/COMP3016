using namespace std;
#include <string>
#include <iostream>
const int MapYSize = 12;

class InheritPosition {
public:
    int XPos = 0;
    int YPos = 0;
};

class CharacterObject : public InheritPosition {
private:
    string Sprite;

public:
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

class EnemyObject : public InheritPosition {
private:
    int Health = 0;
    int MaxHealth = 0;
    string Type = "regular";

public:
    EnemyObject() {};

    bool Active = false;

    void Init(int InputX, int InputY) {
        Active = true;
        XPos = InputX;
        YPos = InputY;
        Health = 100;
        MaxHealth = 100;
        return;
    }

    void SetType(string ReqType) {
        if (Health == 100 and ReqType != "regular") {
            if (ReqType == "heavy") {
                Type = ReqType;
                MaxHealth = 200;
                Health = 200;
            }
        }
    }

    pair <int, int> GetPos() {
        return make_pair(XPos, YPos);
    }

    string GetType() {
        return Type;
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
            Health = 0;
            Active = false;
            return true;
        }
        return false;
    }
};

class MapObject {
public:
    int Id;
    string MapLines[MapYSize];
    int StartCoords[2];
    int EndCoords[2];
    int StartTime = 0;
};