#ifndef __GAME_OBJECT_h
#define __GAME_OBJECT_h

#include "Board.h"

class GameObjects
{
private:
	Location loc;
	int direction;
	vector<int> steps_saver;
	int steps_count = 0;

public:
	GameObjects();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void getLocation(int& _row, int& _col)const; 
	void getTmpLocation(int& _row, int& _col)const; 
	int  getDirection()const;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void setLocation(int row, int col); 
	void setTmpLocation(int tmp_row, int tmp_col); 
	void setDirection(int new_direction);
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool compLoc(Location one, Location two);
	void changeLocation(int direction);
	bool insideBoard(int board_rows, int board_cols, int tmp_row, int tmp_col);
	void addStepsCount();
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//                             save and load mode
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void addStep(int step);
	vector<int> getSteps();
	void clearSteps();
	void clearStepsCounter();
	int getStepsCount();
	int move(Board& board, int mode);
	virtual int playByFile(Board& board) = 0;

	virtual int playByUser(Board& board, int mode) = 0 ;
	virtual int checkMove(Board& board) = 0;
	virtual void moveOnBoard(Board& board) = 0;
};
#endif 
