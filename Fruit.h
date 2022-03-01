#ifndef __FRUIT_H
#define __FRUIT_H

#include "GameObjects.h"

class Board;

typedef struct appears
{
	int time;
	Location loc;
	int value;

}Appears;

class Fruit :public GameObjects
{
private:
	int value;
	vector<Appears> appearLoc;

public:
	Fruit();

	bool isInTunnel(vector<vector<Location>> tunnels, Location loc);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	vector<Appears> getAppears();
	char getValue()const;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void setValue(int _val);
	void setFruitOnBoard(Board& board, int& is_fruit_alive, int screen_counter, int mode);
	void setFruitOnBoardFile(Board& board,int& ind_fruit, int points_of_time);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual int playByUser(Board& board, int mode)override;
	virtual int playByFile(Board& board)override;
	virtual int checkMove(Board& board)override;
	virtual void moveOnBoard(Board& board)override;
	
	void addAppear(Appears apear);
	void clearAppears();

};
#endif
