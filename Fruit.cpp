#include "Fruit.h"

Fruit::Fruit() 
{ 
	value = 0; 
}


bool Fruit::isInTunnel(vector<vector<Location>> tunnels, Location loc)
{
	for (int i = 0; i < tunnels.size(); i++)
		for (int j = 0; j < 2; j++)
			if (compLoc(loc, tunnels[i][j]))
				return true;

	return false;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
vector<Appears> Fruit::getAppears() { return appearLoc; }
char Fruit::getValue()const { return value; }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Fruit::setValue(int _val) { value = _val; }
void Fruit::setFruitOnBoard(Board& board, int& is_fruit_alive, int points_of_time, int mode)
{
	int row, col, value;
	char symbol;
	int board_rows, board_cols;
	bool valid_place = false;
	srand(time(0));

	board.getBoardSize(board_rows, board_cols);

	getLocation(row, col);

	if (row != 0 && col != 0)
		board.setBoardCell(row, col, board.whatInPacLoc(row, col));

	if (is_fruit_alive < FRUIT_LIVES)
	{
		while (!valid_place)
		{
			row = rand() % (board_rows - 1) + 1;
			col = rand() % (board_cols - 1) + 1;
			symbol = board.whatInBoard(row, col);
			if (symbol == SPACE || symbol == BREAD_CRUMBS)
				if (board.checkFruitLocCell(row, col))
					valid_place = true;
		}

		value = (rand() % 5) + 5;
		setLocation(row, col);
		setTmpLocation(row, col);

		if (mode == SAVE_MODE)
		{
			addStep(APPEAR);
			appearLoc.push_back({ points_of_time, Location(row, col) , value });
		}

		setValue(value);
		board.initSpot(row, col, value + '0');
	}
}
void Fruit::setFruitOnBoardFile(Board& board, int& ind_fruit, int points_of_time)
{
	int row, col;

	appearLoc[ind_fruit].loc.getRowCol(row, col);
	setLocation(row, col);
	setTmpLocation(row, col);
	setValue(appearLoc[ind_fruit].value);
	ind_fruit++;

	board.initSpot(row, col, value + '0');
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Fruit::playByFile(Board& board)
{
	int stepStatus = CRASH_BOARDER;
	int step;

	step = getSteps()[getStepsCount()];
	while (step == APPEAR || step == DISAPPEAR)
	{
		addStepsCount();
		step = getSteps()[getStepsCount()];
	}
	addStepsCount();

	changeLocation(step);
	stepStatus = checkMove(board);
	
	moveOnBoard(board);
	return stepStatus;
}
int Fruit::playByUser(Board& board, int mode)
{
	int stepStatus = CRASH_BOARDER;
	int step;

	while (stepStatus == CRASH_BOARDER || stepStatus == TUNNEL) // if the fruit hits border or tunnel it needs to change direction
	{
		step = rand() % 4;
		changeLocation(step);
		stepStatus = checkMove(board);
	}
	
	if (mode == SAVE_MODE)
		addStep(step);

	moveOnBoard(board);
	return stepStatus;
}
int Fruit::checkMove(Board& board)
{
	int row, col, tmp_row, tmp_col, status = NO_CRASH;
	int board_rows, board_cols;

	board.getBoardSize(board_rows, board_cols);

	getLocation(row, col);
	getTmpLocation(tmp_row, tmp_col);

	char what_in_board = board.whatInBoard(tmp_row, tmp_col);
	bool inside_the_board = insideBoard(board_rows, board_cols, tmp_row, tmp_col);

	// check if tried to enter tunnel or crash packman
	if (isInTunnel(board.getTunnels(), Location(tmp_row, tmp_col)))
		status = TUNNEL;

	else if (what_in_board == BOARD || !inside_the_board)
		status = CRASH_BOARDER;

	else if (what_in_board == PACKMAN)
		status = STRIKE_FRUIT_PACKMAN;

	else if (what_in_board == GHOST)
		status = STRIKE_FRUIT_GHOST;

	if (status != NO_CRASH)
		setTmpLocation(row, col);

	return status;
}
void Fruit::moveOnBoard(Board& board)
{
	int tmp_row, tmp_col, row, col;
	getLocation(row, col);
	getTmpLocation(tmp_row, tmp_col);

	// update board for fruit move
	board.setBoardCell(row, col, board.whatInPacLoc(row, col));
	
	// update row and col in new location
	row = tmp_row;
	col = tmp_col;
	setLocation(row, col);
	setTmpLocation(row, col);

	// update board for object move
	board.setBoardCell(row, col, value + '0');
}

void Fruit::addAppear(Appears appear) { appearLoc.push_back(appear); }
void Fruit::clearAppears() { appearLoc.clear(); }
