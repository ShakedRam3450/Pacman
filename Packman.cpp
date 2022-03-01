#include "Packman.h"

Packman::Packman()
{
	ch = SPACE;
	status = NO_CRASH;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Packman::initLocation()
{
	int row, col;
	start.getRowCol(row, col);
	setLocation(row, col);
	setTmpLocation(row, col);
}
void Packman::ifESC() 
{
	char ch = _getch();
	while (ch != ESC)
		ch = _getch();
}
void Packman::isInTunnel(vector<vector<Location>> tunnels, int& tmprow, int& tmpcol, int rows, int cols)
{
	bool in_tunnel = false;

	for (int i = 0; i < tunnels.size(); i++)
		for (int j = 0; j < 2; j++)
			if (compLoc(Location(tmprow, tmpcol), tunnels[i][j]))
				in_tunnel = true;

	if (in_tunnel)
	{
		if (tmprow == 0)
			tmprow = rows - 2;

		else if (tmprow == rows - 1)
			tmprow = 1;

		else if (tmpcol == 0)
			tmpcol = cols - 2;

		else if (tmpcol == cols - 1)
			tmpcol = 1;
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Packman::getStatus() { return status; }
int Packman::getDirectionFromPlayer(char ch)const
{	// this function get char from the players kebord and return the appropriate direction according to the game instructions
	if (ch == 'w' || ch == 'W')
		return UP;
	else if (ch == 'x' || ch == 'X')
		return DOWN;
	else if (ch == 'a' || ch == 'A')
		return LEFT;
	else if (ch == 'd' || ch == 'D')
		return RIGHT;
	else if (ch == 's' || ch == 'S')
		return STAY;
	else
		return NOT_VALID;
}
char Packman::getCh() { return ch; }
vector<int> Packman::getAppearTime() { return appears_time; }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Packman::setCh(char _ch) { ch = _ch; }
void Packman::setStatus(int _status) { status = _status; }
void Packman::setStartLocation(int _row, int _col) { start.setRowCol(_row, _col); }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int Packman::playByUser(Board& board, int mode)
{
	int tmp_row, tmp_col, row, col;

	getLocation(row, col);
	getTmpLocation(tmp_row, tmp_col);
	
	setDirection(getDirectionFromPlayer(ch));
	whereToMove();

	status = checkMove(board);

	if (status != NO_CRASH)
		setTmpLocation(row, col);

	if (mode == SAVE_MODE)
		addStep(getDirectionFromPlayer(ch));

	moveOnBoard(board);

	return status;
}
int Packman::playByFile(Board& board)
{
	int tmp_row, tmp_col, row, col;
	int step;

	step = getSteps()[getStepsCount()];
	addStepsCount();

	getLocation(row, col);
	getTmpLocation(tmp_row, tmp_col);

	setDirection(step);

	status = checkMove(board);

	if (status != NO_CRASH)
		setTmpLocation(row, col);

	moveOnBoard(board);

	return status;
}
int Packman::checkMove(Board& board)
{
	int tmp_row, tmp_col, row, col;
	int board_rows, board_cols;
	int step;

	board.getBoardSize(board_rows, board_cols);

	step = getDirection();
	changeLocation(step);

	getLocation(row, col);
	getTmpLocation(tmp_row, tmp_col);

	// check if entered tunnel 
	isInTunnel(board.getTunnels(), tmp_row, tmp_col, board_rows, board_cols);

	status = NO_CRASH;
	char what_in_board = board.whatInBoard(tmp_row, tmp_col);

	//crash boarded
	if (what_in_board == BOARD)
		status = CRASH_BOARDER;

	if (!insideBoard(board_rows, board_cols, tmp_row, tmp_col))
		status = CRASH_BOARDER;

	//crash ghost 
	else if (what_in_board == GHOST)
		status = STRIKE;

	//crash fruit
	else if (what_in_board >= '5' && what_in_board <= '9')
		status = STRIKE_FRUIT_PACKMAN;

	if (status == NO_CRASH)
		setTmpLocation(tmp_row, tmp_col);

	return status;
}
void Packman::whereToMove()
{
	char tmp_ch;
	if (!_kbhit() && status == CRASH_BOARDER) // in both of these cases packman does not move 
		return;

	if (_kbhit())  // wait for another direction from the player
	{
		tmp_ch = _getch();

		if (valid_ch(tmp_ch))
		{
			if (tmp_ch == ESC)
				ifESC();  // player want to pause, direction does not change

			else
			{
				ch = tmp_ch;  // update direction
				status = NO_CRASH;
			}

			setDirection(getDirectionFromPlayer(ch));
		}
	}
}
void Packman::moveOnBoard(Board& board)
{
	int tmp_row, tmp_col, row, col;
	getLocation(row, col);
	getTmpLocation(tmp_row, tmp_col);

	// mark where packman was on the board 
	board.setBoardCell(row, col, SPACE);

	row = tmp_row; 	// update row and col in new location
	col = tmp_col;
	setLocation(row, col);
	setTmpLocation(tmp_row, tmp_col);

	// update board for object move
	board.setBoardCell(row, col, PACKMAN);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                         save mode
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Packman::addAppear(int appear) { appears_time.push_back(appear); }
void Packman::clearAppears() { appears_time.clear(); }
