using namespace std;
#include <string>

class CharacterObject {
private:
    string Sprite;

    int XPos;
    int YPos;
public:
    void SetSprite(string Input) {
        Input.resize(1);
        Sprite = Input;
        return;
    }
    string GetSprite() {
        return Sprite;
    }
};