#include "Game.h"

Game::Game() 
{
	points_of_time = 0;
	game_mode = NORMAL_MODE;
	points = points_to_earn = 0;
	level = 0;
	end_game = false;
	life = 3;
}

void Game::start(int argc, char* argv[])
{
	initGameMode(argc, argv);
	theGameBoard.setGameMode(game_mode);
	if (game_mode == LOAD_MODE || game_mode == LOAD_SILENT_MODE)
		startFromFile();
	else
		startFromKeybord();
}
void Game::initGameMode(int argc, char* argv[])
{
	bool valid = true;

	if (strcmp("pacman.exe", argv[1]) != 0)
		valid = false;

	if (valid)
	{
		switch (argc)
		{
		case 2:
			game_mode = NORMAL_MODE;
			break;
		case 3:
			if (strcmp("-save", argv[2]) == 0)
				game_mode = SAVE_MODE;
			else if (strcmp("-load", argv[2]) == 0)
				game_mode = LOAD_MODE;
			else
				valid = false;
			break;
		case 4:
			if (strcmp("-silent", argv[3]) != 0)
				valid = false;
			else if (strcmp("-save", argv[2]) == 0)
				game_mode = SAVE_MODE;
			else if (strcmp("-load", argv[2]) == 0)
				game_mode = LOAD_SILENT_MODE;
			break;
		default:
			valid = false;
			break;
		}
	}

	if (!valid)
	{
		cout << "Invalid Usage commandline!" << endl;
		cout << "Usage: [pacman.exe] [-save | -load -silent]" << endl;
		exit(1);
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                  load mode
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Game::startFromFile()
{
	int file_counter = 0, ind = 0, status, end_game_time;
	bool comp;
	life = 3;
	points = points_to_earn = points_of_time = 0;
	vector<Result> cur_game;
	vector<string> file_name_list = file.createFileList();
	vector<string> file_steps_list = file.createFileStepsList();
	
	vector<string> new_file_name_list = findPlayedScreens(file_name_list, file_steps_list);

	if (new_file_name_list.size() == 0)
		menu.printNoFiles(); 
	else
	{
		while (file_counter < new_file_name_list.size())
		{

			bool valid_board;
			init(new_file_name_list[file_counter], valid_board);
			end_game_time = result.findEndGameTime(new_file_name_list[file_counter]);
			if (file_counter == 0  && game_mode != LOAD_SILENT_MODE)
			{
				theGameBoard.printBoard();
				theGameBoard.printBar(life, points);
			}
			steps.readSteps(packman, ghosts, fruit, new_file_name_list[file_counter]);

			int ind = 0, ind_fruit = 0;

			while (!end_game)
			{
				playNewGame(ind, ind_fruit, status, end_game_time);
				if (game_mode == LOAD_SILENT_MODE)
				{
					if (life == 0)
						cur_game.push_back({ points_of_time, GAMEOVER });
					else
					{
						if (points_to_earn == points)
							cur_game.push_back({ points_of_time, WON });
						else
							cur_game.push_back({ points_of_time, STRIKE });
					}		
				}	
				status = NO_CRASH;
				ind++;
			}
			if (game_mode == LOAD_SILENT_MODE)
			{
				comp = result.compareRes(cur_game, new_file_name_list[file_counter]);
				if (!comp)
				{
					menu.printTestResult("TEST FAILED!");
					return;
				}
				cur_game.clear();
			}

			points_of_time = 0;
			file_counter++;
			end_game = false;

			if (EndOfLife(file_counter, file_name_list))
				break;

			system("cls");
		}
		if (game_mode == LOAD_SILENT_MODE)
			menu.printTestResult("SUCCESS");
	}


	
}
void Game::playNewGame(int& ind, int& ind_fruit, int& status, int end_game_time)
{
	int move_counter = 1;

	if (game_mode != LOAD_SILENT_MODE)
	{
		theGameBoard.setColor(WITH_COLOR);
		theGameBoard.printBoard();
		theGameBoard.printBar(life, points);
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//             the player doesn't push the keybord yet
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int packman_moved = packman.getAppearTime()[ind];
	beforeFirstMove(status, packman_moved);
	packman.setStatus(status);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//           packman starts to move
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	afterFirstMove(status, move_counter, ind_fruit, end_game_time);
	packman.setStatus(status);

	checkEndGame();
}
void Game::beforeFirstMove(int& status, int packman_moved)
{
	while (packman_moved > points_of_time)
	{
		for (int i = 0; i < ghosts.size(); i++)
		{
			if (game_mode != LOAD_SILENT_MODE)
				Sleep(SPEED / 5);

			points_of_time++;
			status = ghosts[i].move(theGameBoard, game_mode);
		}

		if (status == STRIKE)
			return;
	}
}
void Game::afterFirstMove(int& status, int& move_counter, int& ind_fruit, int end_game_time)
{
	bool revive_fruit = true;
	int row, col;
	bool is_fruit_alive = false;

	while (!end_game && status != STRIKE)
	{
		if (points_of_time == end_game_time)
			return;

		if (game_mode != LOAD_SILENT_MODE)
			Sleep(SPEED / 10);
		// ~~~~~~~~   FRUIT   ~~~~~~~~~~~~~
		if (fruit.getStepsCount() < fruit.getSteps().size())
		{
			if (is_fruit_alive)
			{
				if (move_counter % 3 == 1)
				{
					status = fruit.move(theGameBoard, game_mode);
					if (status == STRIKE_FRUIT_PACKMAN)
					{
						points += fruit.getValue();
						points_to_earn += fruit.getValue();
						is_fruit_alive = false; //kill fruit
					}
					else if (status == STRIKE_FRUIT_GHOST)
						is_fruit_alive = false; //kill fruit

					if (game_mode != LOAD_SILENT_MODE)
						theGameBoard.printBar(life, points);
				}
				if (fruit.getStepsCount() < fruit.getSteps().size())
					if (fruit.getSteps()[fruit.getStepsCount()] == DISAPPEAR)
						is_fruit_alive = false; //kill fruit

			}
			else if (fruit.getSteps()[fruit.getStepsCount()] == APPEAR && fruit.getAppears()[ind_fruit].time <= points_of_time)
			{
				is_fruit_alive = true;
				fruit.setFruitOnBoardFile(theGameBoard, ind_fruit, points_of_time);
				fruit.addStepsCount();
			}
			else if (fruit.getSteps()[fruit.getStepsCount()] == DISAPPEAR)
			{
				is_fruit_alive = false; //kill fruit
				// remove fruit from screen
				fruit.getLocation(row, col);
				theGameBoard.setBoardCell(row, col, theGameBoard.whatInPacLoc(row, col));
				fruit.addStepsCount();
			}
		}

		// ~~~~~~~~~   GHOSTS  ~~~~~~~~~~~~
		if (move_counter % 2 == 1) // every 2 steps of packman == 1 step of each ghost
		{
			for (int i = 0; i < ghosts.size(); i++)
			{
				if (status == STRIKE)
					break;

				status = ghosts[i].move(theGameBoard, game_mode);

				if (status == STRIKE_FRUIT_GHOST)
					is_fruit_alive = false;; //kill fruit
			}
			if (status == STRIKE)
				return;
		}
		move_counter++;

		// ~~~~~~~~~   PACKMAN   ~~~~~~~~~~~~
		packman.setStatus(status);
		points_of_time++;
		status = packman.move(theGameBoard, game_mode);
		setPoints();

		if (status == STRIKE_FRUIT_PACKMAN)
		{
			points += fruit.getValue();
			points_to_earn += fruit.getValue();
			is_fruit_alive = false;
		}

		if (game_mode != LOAD_SILENT_MODE)
			theGameBoard.printBar(life, points);
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                            save and normal mode
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Game::startFromKeybord()
{
	int file_counter = 0;
	bool valid_board;
	while (!end_game)
	{
		int choise = menu.printMenu();

		if (choise == NEW_GAME)
		{
			menu.askForFile();
			menu.chooseLevel(level);
			menu.setBoardColor();

			life = 3;
			points = points_to_earn = 0;
			file_counter = 0;

			if (menu.getFileNameList().size() == 0)
			{
				menu.printNoFiles();
				choise = EXIT;
			}

			while (file_counter < menu.getFileNameList().size())
			{
				vector<string> file_name_list = menu.getFileNameList();
				init(file_name_list[file_counter], valid_board);

				if (!valid_board)
				{
					menu.invalidBoard();
					break;
				}

				while (!end_game)
					playNewGame(menu.getBoardColor());
					
				writeStepsToFile(file_name_list[file_counter]);
				writeResultFile(file_name_list[file_counter]);

				strike_time.clear();
				file_counter++;
				end_game = false;
				points_of_time = 0;
				menu.printWinLoseMessages(life, file_counter);

				if (EndOfLife(file_counter, file_name_list))
					break; 
				system("cls");			
			}
		}

		if (choise == INSTRUCTIONS)
		{
			menu.printInstructions();
		}

		if (choise == EXIT)
		{
			menu.printByeMessage();
			end_game = true;
		}
	}
}
void Game::playNewGame(int if_color)
{
	int move_counter = 1;
	int status = NO_CRASH;
	char ch;
	int is_fruit_alive;

	srand(time(0));
	theGameBoard.setColor(if_color);
	theGameBoard.printBoard();
	theGameBoard.printBar(life, points);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//             the player doesn't push the keybord yet
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	beforeFirstMove(status);
	packman.setStatus(status);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//             the player push the keybord for the first time
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	if (packman.getStatus() != STRIKE)
	{
		ch = _getch();

		if (game_mode == SAVE_MODE && valid_ch(ch))
			packman.addAppear(points_of_time); // put the time packman start to move

		if (ch == ESC)
			packman.ifESC();		
		
	}

	is_fruit_alive = rand() % FRUIT_PROBABILITY;
	fruit.setFruitOnBoard(theGameBoard, is_fruit_alive, points_of_time, game_mode);
	
	afterFirstMove(status, move_counter, ch, is_fruit_alive);
	packman.setStatus(status);
	
	checkEndGame();
}
void Game::beforeFirstMove(int& status)
{
	while (!_kbhit())
	{
		for (int i = 0; i < ghosts.size() && status != STRIKE; i++)
		{
			Sleep(SPEED/4);
			

			int row, col;
			packman.getLocation(row, col);
			theGameBoard.setPackmanLoction(Location(row, col));
			ghosts[i].setLevel(level);

			points_of_time++;

			status = ghosts[i].move(theGameBoard, game_mode);
		}

		if (status == STRIKE)
			return;	
	}
}
void Game::afterFirstMove(int& status, int& move_counter, char& ch, int& is_fruit_alive)
{
	int fruit_time_to_live = 0;
	srand(time(0));

	while (!end_game && status != STRIKE)
	{
		if (points == 8)
		{
			gotoxy(25, 25);
			cout << "points";
		}
		Sleep(SPEED/2);
		// ~~~~~~~~~~~~~~~~~~~~~~~~   FRUIT   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (is_fruit_alive < FRUIT_LIVES)
		{
			if (move_counter % 3 == 1)
			{
				status = fruit.move(theGameBoard, game_mode);
				theGameBoard.printBar(life, points);
				fruitCrashPacman(status, is_fruit_alive);
				fruitCrashGhost(status, is_fruit_alive);
			}

			if (fruit_time_to_live % FRUIT_KILL_STEPS == 0)
			{
				is_fruit_alive = FRUIT_PROBABILITY; //kill fruit
				if (game_mode == SAVE_MODE)
					fruit.addStep(DISAPPEAR);
			}
		}
		else if (fruit_time_to_live != FRUIT_KILL_FOREVER)
		{
			is_fruit_alive = rand() % FRUIT_PROBABILITY;
			fruit.setFruitOnBoard(theGameBoard, is_fruit_alive, points_of_time, game_mode);
		}
		else if (fruit_time_to_live == FRUIT_KILL_FOREVER)
			if (game_mode == SAVE_MODE)
				fruit.addStep(DISAPPEAR);

		// ~~~~~~~~~~~~~~~~~~~~~~~~~   GHOSTS  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (move_counter % 2 == 1) // every 2 steps of packman == 1 step of each ghost
		{
			for (int i = 0; i < ghosts.size() ; i++)
			{
				if (status == STRIKE)
					break;
				
				int row, col;
				packman.getLocation(row, col);
				theGameBoard.setPackmanLoction(Location(row, col));

				status = ghosts[i].move(theGameBoard, game_mode);

				fruitCrashGhost(status, is_fruit_alive);
					
			}
			if (status == STRIKE)
				return;
		}
		fruit_time_to_live++;
		move_counter++;

		// ~~~~~~~~~~~~~~~~~~~~~~~~~   PACKMAN   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		packman.setCh(ch);
		packman.setStatus(status);
		points_of_time++;
		status = packman.move(theGameBoard, game_mode);
		
		ch = packman.getCh();
		
		setPoints();
		fruitCrashPacman(status, is_fruit_alive);	

	}
	if (status == STRIKE)
	{
		gotoxy(20, 20);
		cout << "shaked";
	}
}
void Game::writeStepsToFile(string file_name_original)
{
	string file_name = steps.changeName(file_name_original, "steps");
	steps.createStepsFile(file_name, packman, ghosts, fruit);
}
void Game::writeResultFile(string file_name_original)
{

	string file_name = result.changeName(file_name_original, "result");
	result.createResultFile(file_name, strike_time, points_of_time, life);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                       
//                              moving thorw files
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Game::makeTunnels()
{
	char what_in_board1, what_in_board2;
	int board_rows, board_cols;
	vector<vector<Location>> tunnels;
	theGameBoard.getBoardSize(board_rows, board_cols);

	for (int col = 0; col < board_cols; col++)  // up-down tunnels
	{
		what_in_board1 = theGameBoard.whatInBoard(0, col);
		what_in_board2 = theGameBoard.whatInBoard(board_rows - 1 , col);

		if (what_in_board1 == ' ' && what_in_board2 == ' ')
		{
			tunnels.push_back({ Location(0, col), Location(board_rows - 1 , col) });
			theGameBoard.initSpot(0, col, '^');
			theGameBoard.initSpot(board_rows - 1 , col, '^');
		}
		
	}

	for (int row = 0; row < board_rows; row++) // left-right tunnels
	{
		what_in_board1 = theGameBoard.whatInBoard(row, 0);
		what_in_board2 = theGameBoard.whatInBoard(row, board_cols - 1);

		if (what_in_board1 == ' ' && what_in_board2 == ' ')
		{
			tunnels.push_back({ Location(row, 0), Location(row, board_cols - 1) });
			theGameBoard.initSpot(row, 0, '^');
			theGameBoard.initSpot(row, board_cols - 1, '^');
		}

	}	
	theGameBoard.setTunnels(tunnels);
}
bool Game::checkValidBoardGame()
{
	bool valid_board, one_packman, one_ampersand;

	one_packman = one_ampersand = false;
	valid_board = theGameBoard.ifBoardIsValid();

	if (!valid_board)
		return false;
	
	int board_rows, board_cols, ghosts_counter = 0;
	theGameBoard.getBoardSize(board_rows, board_cols);

	for (int row = 0; row < board_rows; row++)
	{
		for (int col = 0; col < board_cols; col++)
		{
			char what_in_board = theGameBoard.whatInBoard(row, col);

			if (what_in_board == '@')
			{
				if (!one_packman)
					one_packman = true;
				else
					return false;
			}
			else if (what_in_board == '&')
			{
				if (!one_ampersand)
				{
					one_ampersand = true;

					for (int i = row; i < 3; i++)
						for (int j = col; j < 20; j++)
						{
							char points_area = theGameBoard.whatInBoard(i, j);
							if (points_area != '%' && i != row && j != col)
								return false;
						}
				}
				else
					return false;
			}
			else if (what_in_board == '$')
			{
				ghosts_counter++;
				if (ghosts_counter > 4)
					return false;
			}
			else if (what_in_board != '%' && what_in_board != ' ' && what_in_board != '#')
			{
				return false;
			}
		}
	}

	if (!one_packman || !one_ampersand)
		return false;
	else
		return true;
}
void Game::initBoard()
{
	int board_rows, board_cols;
	theGameBoard.getBoardSize(board_rows, board_cols);

	for (int row = 0; row < board_rows; row++)
		for (int col = 0; col < board_cols; col++)		
			if (theGameBoard.whatInBoard(row, col) == ' ') 
			{
				theGameBoard.initSpot(row, col, BREAD_CRUMBS);
				theGameBoard.setFruitLocCell(row, col);
				points_to_earn++;
			}
	
	for (int row = 0; row < board_rows; row++)
		for (int col = 0; col < board_cols; col++)
		{
			char what_in_board = theGameBoard.whatInBoard(row, col);
			
			if (what_in_board == '#')
			{
				theGameBoard.initSpot(row, col, BOARD);
				theGameBoard.setFruitLocCell(row, col);
			}

			else if (what_in_board == ' ')
			{
				theGameBoard.initSpot(row, col, SPACE);
			}

			else if (what_in_board == '@')
			{
				packman.setStartLocation(row, col);

				packman.setLocation(row, col);
				packman.setTmpLocation(row, col);

				theGameBoard.setPacLocCell(row, col);
				theGameBoard.initSpot(row, col, PACKMAN);
			}

			else if (what_in_board == '$')
			{
				Ghost ghost(Location(row, col)); // create a new ghost
				ghosts.push_back(ghost);
				theGameBoard.initSpot(row, col, GHOST);
				points_to_earn++;
			}

			else if (what_in_board == '%')
			{
				theGameBoard.initSpot(row, col, SPACE);
				theGameBoard.setPacLocCell(row, col);
			}

			else if (what_in_board == '&')
			{
				theGameBoard.updateBarLoc(row, col);
				theGameBoard.initSpot(row, col, SPACE);
			}

			else if (what_in_board == '^')
			{
				theGameBoard.initSpot(row, col, SPACE);
			}
		}
}
void Game::init(string file_name, bool& valid)
{
	int board_rows, board_cols;
	end_game = false;

	// ------------ clear all vectors --------------
	for (int i = 0; i < theGameBoard.getTunnels().size(); i++)
	{
		theGameBoard.getTunnels()[i].clear();
	}
	theGameBoard.getTunnels().clear();
	theGameBoard.startNewBoard();
	theGameBoard.initPacLoc();
	theGameBoard.initFruitLoc();
	fruit.setLocation(0, 0);
	fruit.setTmpLocation(0, 0);
	ghosts.clear();


	// ------------- get the new board fron the next file -----------
	theGameBoard.getBoard(file_name, board_rows, board_cols);
	theGameBoard.setBoardSize(board_rows, board_cols);

	if (!checkValidBoardGame())
	{
		valid = false;
		return;
	}
	
	valid = true;
	makeTunnels();
	initBoard();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                   checks
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Game::setPoints()
{
	int row, col;
	theGameBoard.printBar(life, points);
	packman.getLocation(row, col);

	bool add_point = theGameBoard.checkPacLocCell(row, col);
	if (add_point)
	{
		points++;
		checkWin();
	}
	theGameBoard.setPacLocCell(row, col);
	theGameBoard.printBar(life, points);
}
void Game::checkWin()
{
	if (points == points_to_earn)
		end_game = true;
}
void Game::checkEndGame()
{
	int row, col;

	if (!end_game)
	{
		if (game_mode == SAVE_MODE)
		{
			fruit.addStep(DISAPPEAR);
			strike_time.push_back(points_of_time);
		}
		if (game_mode == LOAD_SILENT_MODE)
		{
				
		}
		life--;
	}

	if (life == 0)
	{
		end_game = true;
	}
		

	if (!end_game)
	{
		// init the board
		packman.getLocation(row, col);
		theGameBoard.setBoardCell(row, col, theGameBoard.whatInPacLoc(row, col));

		for (int i = 0; i < ghosts.size(); i++)
		{
			ghosts[i].getLocation(row, col);
			theGameBoard.setBoardCell(row, col, theGameBoard.whatInPacLoc(row, col));
		}

		fruit.getLocation(row, col);
		theGameBoard.setBoardCell(row, col, theGameBoard.whatInPacLoc(row, col));
	}

	// init the game objects location
	int object_row, object_col;
	packman.initLocation();
	packman.getLocation(object_row, object_col);

	theGameBoard.setBoardCell(object_row, object_col, PACKMAN);

	for (int i = 0; i < ghosts.size(); i++)
	{
		ghosts[i].initLocation();
		ghosts[i].getLocation(object_row, object_col);
		theGameBoard.setBoardCell(object_row, object_col, GHOST);
	}

	system("cls");
}
bool Game::EndOfLife(int& file_counter, vector<string>& file_name_list)
{
	if (life == 0)
	{
		life = 3;
		file_counter = 0;
		points = points_to_earn = 0;
		file_name_list.clear();
		return true;
	}
	else
		return false;
}
void Game::fruitCrashPacman(int status, int& is_fruit_alive)
{
	if (status == STRIKE_FRUIT_PACKMAN)
	{
		points += fruit.getValue();
		points_to_earn += fruit.getValue();

		is_fruit_alive = FRUIT_PROBABILITY; //kill fruit

		if (game_mode == SAVE_MODE)
			fruit.addStep(DISAPPEAR);
		fruit.setFruitOnBoard(theGameBoard, is_fruit_alive, points_of_time, game_mode);
	}
}
void Game::fruitCrashGhost(int status, int& is_fruit_alive)
{
	if (status == STRIKE_FRUIT_GHOST)
	{
		is_fruit_alive = FRUIT_PROBABILITY; //kill fruit

		if (game_mode == SAVE_MODE)
			fruit.addStep(DISAPPEAR);

		fruit.setFruitOnBoard(theGameBoard, is_fruit_alive, points_of_time, game_mode);
	}
}
vector<string> Game::findPlayedScreens(vector<string> file_name_list, vector<string> file_steps_list)
{
	vector<string> played_screens;
	int steps_size = (int)file_steps_list.size();
	int name_list = (int)file_name_list.size();
	int j = 0, i = 0;
	while (i < steps_size )
	{
		if (isSameScreen(file_steps_list[i], file_name_list[j]))
		{
			played_screens.push_back(file_name_list[j]);
			i++; j++;
		}
		else
			j++;
	}
	return played_screens;
	
}
bool Game::isSameScreen(string a, string b)
{
	int i = 0;
	while (a[i] != '.')
	{
		if (a[i] != b[i])
			return false;
		i++;
	}
	return true;
}
