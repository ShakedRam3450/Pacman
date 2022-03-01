#ifndef __PACKMAN_H
#define __PACKMAN_H

#include "GameObjects.h"

class Board;
class GameObjects;

class Packman : public GameObjects 
{
private:
	vector<int> appears_time;
	Location start;
	int status;
	char ch;

public:
	Packman();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void initLocation();

	void isInTunnel(vector<vector<Location>> tunnels, int& tmprow, int& tmpcol, int rows, int cols);
	void ifESC();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int getDirectionFromPlayer(char ch)const;
	int getStatus();
	char getCh();
	vector<int> getAppearTime();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void setCh(char _ch);
	void setStartLocation(int _row, int _col);
	void setStatus(int _status);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual int playByUser(Board& board, int mode)override;
	virtual int playByFile(Board& board)override;

	virtual int checkMove(Board& board)override;
	virtual void moveOnBoard(Board& board)override;
	void whereToMove();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//                         save mode
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void addAppear(int appear);
	void clearAppears();

};
#endif