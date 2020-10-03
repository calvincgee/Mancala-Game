#ifndef BOARD_H
#define BOARD_H

#include "./Side.h"
 
class Board {
public:
	Board(int nHoles, int nInitialBeansPerHole);
	Board(const Board& old);
	~Board();
	Board& operator=(const Board& other);
	int holes() const { return m_holes; }
	int beans(Side s, int hole) const;
	int beansInPlay(Side s) const;
	int totalBeans() const { return m_totBeans; }
	bool sow(Side s, int hole, Side& endSide, int& endHole);
	bool moveToPot(Side s, int hole, Side potOwner);
	bool setBeans(Side s, int hole, int beans);
private:
	int m_holes;	// number of holes
	int m_totBeans;	// number of total beans
	int* nSide;		// array for the north side of the board
	int* sSide;		// array for the south side of the board
	int nPot;		// number of beans in the north pot
	int sPot;		// number of beans in the south pot
};
#endif // !BOARD_H
