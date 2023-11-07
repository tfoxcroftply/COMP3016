bool KeyboardLogicRunning = false;
char CurrentKey = NULL;

void KeyboardLogic() {
	if (KeyboardLogicRunning == false) {
		KeyboardLogicRunning = true;

		while (KeyboardLogicRunning == true) {
			
			char Input = _getch();
			CurrentKey = Input;

			Sleep(1 / 30);
		}
	}
}

void SetKeyboardState(bool State) {
	if (State == true) {
		KeyboardLogic();
	}
	else {
		KeyboardLogicRunning = false;
	}
}
