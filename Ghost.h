#ifndef __GHOST_H
#define __GHOST_H

#include "GameObjects.h"

class Game;

class Ghost :public GameObjects 
{
private:
	Location start;
	int steps_counter_race, steps_counter_rand;
	int level;

public:
	Ghost(Location _loc);

	void initLocation();
	bool isInTunnel(vector<vector<Location>> _tunnels, Location loc);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	vector<float> findMinimumDistance(float x1, float x2, float x3, float x4);
	vector<int> findPackmanDirecion(int packman_row, int packman_col);
	float distance(int x1, int y1, int x2, int y2);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void setLevel(int _level);
	void setRaceStepsCounter(int _counter);
	void setRandStepsCounter(int _counter);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int getRaceStepsCounter();
	int getRandStepsCounter();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	virtual int playByUser(Board& board, int mode)override;
	virtual int playByFile(Board& board)override;

	virtual int checkMove(Board& board)override;
	virtual void moveOnBoard(Board& board)override;
};

#endif
