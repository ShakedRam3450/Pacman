#include "Game.h"

//make cursor invisible
void set_cursor(bool visible) {
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = visible;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

int main(int argc, char* argv[])
{
	Game theGame;
	set_cursor(false);
	theGame.start(argc,argv); 
	return 0;
}
