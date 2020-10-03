#ifndef GAME_H
#define GAME_H

#include "./Board.h"
class Player;

class Game {
public:
	Game(const Board& b, Player* south, Player* north);
	Game(const Game& other);
	Game& operator=(const Game& other);
	void display() const;
	void status(bool& over, bool& hasWinner, Side& winner) const;
	bool move();
	void play();
	int beans(Side s, int hole) const;
private:
	Board m_Board;		// the Game's Board
	Player* m_south;	// pointer to the south player
	Player* m_north;	// pointer to the north player
	bool southTurn;	// true if it is south's turn
};
#endif // !GAME_H
