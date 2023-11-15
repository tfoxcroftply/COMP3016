using namespace std;
#include <string>
const int MapYSize = 12;

class CharacterObject {
private:
    string Sprite;

public:
    int XPos = 0;
    int YPos = 0;
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

class MapObject {
public:
    int Id;
    string MapLines[MapYSize];
    int StartCoords[2];
    int EndCoords[2];
};