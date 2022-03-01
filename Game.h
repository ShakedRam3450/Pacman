#ifndef __GAME_H
#define __GAME_H

#include "Board.h"
#include "Ghost.h"
#include "Packman.h"
#include "Fruit.h"
#include "menu.h"


class Game 
{
private:
	bool end_game;
	int points, points_to_earn;
	int life, level;

	Menu menu;
	Board theGameBoard;
	Packman packman;
	vector<Ghost> ghosts;
	Fruit fruit;

	// ~~~~~~~~~~~~~~
	vector<int> strike_time;
	int game_mode;
	int points_of_time;
	File steps, result, file;

public:
	Game();

	void start(int argc, char* argv[]);
	void initGameMode(int argc, char* argv[]);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//                                load mode
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void startFromFile();
	void playNewGame(int& ind, int& ind_fruit, int& status, int end_game_time);
	void beforeFirstMove(int& status, int packman_moved);
	void afterFirstMove(int& status, int& move_counter, int& ind_fruit, int end_game_time);
	vector<string> findPlayedScreens(vector<string> file_name_list, vector<string> file_steps_list);
	bool isSameScreen(string a, string b);
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//                            save and normal mode
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void startFromKeybord();
	void playNewGame(int if_color);
	void beforeFirstMove(int& status);
	void afterFirstMove(int& status, int& move_counter, char& ch, int& is_fruit_alive);

	void writeStepsToFile(string file_name_original);
	void writeResultFile(string file_name_original);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//                            moving thorw files
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void init(string file_name, bool& valid);
	void initBoard();
	void makeTunnels();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//                                    checks
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void setPoints();
	void checkWin();
	void checkEndGame();
	bool checkValidBoardGame();
	bool EndOfLife(int& file_counter, vector<string>& file_name_list);
	void fruitCrashPacman(int status, int& is_fruit_alive);
	void fruitCrashGhost(int status, int& is_fruit_alive);
};

#endif 
