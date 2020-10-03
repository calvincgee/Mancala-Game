#include "../Header Files/Board.h"
#include "../Header Files/Side.h"
#include <iostream>

using namespace std;

// Board Constructor
Board::Board(int nHoles, int nInitialBeansPerHole) {
	m_holes = nHoles;
	m_totBeans = m_holes * nInitialBeansPerHole * 2;
	nPot = 0;
	sPot = 0;
	nSide = new int[nHoles];
	sSide = new int[nHoles];
	for (int i = 0; i < m_holes; i++) {
		nSide[i] = nInitialBeansPerHole;
		sSide[i] = nInitialBeansPerHole;
	}
}

// Board copy constructor
Board::Board(const Board& old) {
	this->m_holes = old.m_holes;
	this->m_totBeans = old.m_totBeans;
	this->nPot = old.nPot;
	this->sPot = old.sPot;
	nSide = new int[m_holes];
	sSide = new int[m_holes];
	for (int i = 0; i < m_holes; i++) {
		this->nSide[i] = old.nSide[i];
		this->sSide[i] = old.sSide[i];
	}
}

// Board destructor
Board::~Board() {
	delete[] nSide;
	delete[] sSide;
}

// Board assignment operator
Board& Board::operator=(const Board& other) {
	if (&other == this) {	// check for aliasing
		return *this;
	}
	delete[] nSide;
	delete[] sSide;
	this->m_holes = other.m_holes;
	this->m_totBeans = other.m_totBeans;
	this->nPot = other.nPot;
	this->sPot = other.sPot;
	nSide = new int[m_holes];
	sSide = new int[m_holes];
	for (int i = 0; i < m_holes; i++) {
		this->nSide[i] = other.nSide[i];
		this->sSide[i] = other.sSide[i];
	}
	return *this;
}

// Returns the number of beans in a hole on a given side
// To access the pots, hole = 0
// Returns -1 if the hole is invalid
int Board::beans(Side s, int hole) const {
	if (hole < 0 || hole > m_holes) {	// checks if the hole is valid
		return -1;
	}
	if (hole == 0) {	// checks if the hole is a pot
		if (s == SOUTH) {
			return sPot;
		}
		else {
			return nPot;
		}
	}
	if (s == SOUTH) {
		return sSide[hole - 1];
	}
	else {
		return nSide[m_holes - hole];
	}
}

// Returns the beans in play on a given side
int Board::beansInPlay(Side s) const {
	int count = 0;
	for (int i = 0; i < m_holes; i++) {
		if (s == SOUTH) {
			count += sSide[i];
		}
		else {
			count += nSide[i];
		}
	}
	return count;
}

// Sows the given hole, putting one beach in each of the following holes, including the pot if it is on the correct side
// Set endSide to the side that sow ended on
// Set endHole to the hole that sow ended on
// Returns false if the hole is invalid or the hole is empty
// Otherwise returns true
bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
	if (hole <= 0 || hole > m_holes) {	// checks if the hole is valid
		return false;
	}
	if (s == SOUTH) {
		if (sSide[hole - 1] == 0) {	// checks if the hole is empty
			return false;
		}
		else {
			int index = hole - 1;
			bool south = true;
			int temp = sSide[index];
			sSide[index] = 0;		// set the hole to 0
			while (temp != 0) {		// continues to loop while there is still beans to give out
				if (index == -1) {	// check if a bean was just deposited into south's pot
					south = !south;
				}
				index++;
				if (index == m_holes) {		// check if the index is at the end of the array
					if (south) {		// if it is on the right side, add a bean to the pot
						sPot++;
						temp--;
						index = -1;
						continue;
					}
					index = 0;		// if it is not on the right side, set index to 0 and switch sides
					south = !south;
				}
				if (south) {
					sSide[index]++;
					temp--;
				}
				else {
					nSide[index]++;
					temp--;
				}
			}
			//		set the endSide and endHole
			if (south) {
				endSide = SOUTH;
				endHole = index + 1;
			}
			else {
				endSide = NORTH;
				endHole = m_holes - index;
			}
			return true;
		}
	}
	//		repeat the same steps for the north side
	else {
		if (nSide[m_holes - hole] == 0) {
			return false;
		}
		else {
			int index = m_holes - hole;
			bool north = true;
			int temp = nSide[index];
			nSide[index] = 0;
			while (temp != 0) {
				if (index == -1) {
					north = !north;
				}
				index++;
				if (index == m_holes) {
					if (north) {
						nPot++;
						temp--;
						index = -1;
						continue;
					}
					index = 0;
					north = !north;
				}
				if (north) {
					nSide[index]++;
					temp--;
				}
				else {
					sSide[index]++;
					temp--;
				}
			}
			if (north) {
				endSide = NORTH;
				if (index == -1) {
					index = m_holes;
				}
				endHole = m_holes - index;
			}
			else {
				endSide = SOUTH;
				endHole = index + 1;
			}
			return true;
		}
	}
}

// Move the beans in a given hole to a specified pot
// Return false if the hole is invalid, otherwise return true
bool Board::moveToPot(Side s, int hole, Side potOwner) {
	if (hole <= 0 || hole > m_holes) {	// check if the hole is valid
		return false;
	}
	int beans;
	//	find the number of beans in the hole and set the hole to 0
	if (s == SOUTH) {
		beans = sSide[hole - 1];
		sSide[hole - 1] = 0;
	}
	else {
		beans = nSide[m_holes - hole];
		nSide[m_holes - hole] = 0;
	}
	// add the beans to the correct pot
	if (potOwner == SOUTH) {
		sPot += beans;
	}
	else {
		nPot += beans;
	}
	return true;
}

// Set the number of beans in any given hole
// Change totBeans accordingly
// Return false if the hole or beans in invalid, true otherwise
bool Board::setBeans(Side s, int hole, int beans) {
	if (hole < 0 || hole > m_holes || beans < 0) {		// check if the hole and beans are valid
		return false;
	}
	int difference;
	// calculate the difference between what was in the hole/pot and beans, then add beans to the hole/pot
	if (hole == 0) {
		if (s == SOUTH) {
			difference = beans - sPot;
			sPot = beans;
		}
		else {
			difference = beans - nPot;
			nPot = beans;
		}
	}
	else {
		if (s == SOUTH) {
			difference = beans - sSide[hole - 1];
			sSide[hole - 1] = beans;
		}
		else {
			difference = beans - nSide[m_holes - hole];
			nSide[m_holes - hole] = beans;
		}
	}
	m_totBeans += difference;	// change totBeans by the difference
	return true;
}