#include "../Header Files/Game.h"
#include "../Header Files/Board.h"
#include "../Header Files/Player.h"
#include <iostream>
#include <time.h>

using namespace std;

// Game constructor
Game::Game(const Board& b, Player* south, Player* north): m_Board(b) {
	m_south = south;
	m_north = north;
	srand (time(NULL));
	int flip = rand() % 2;
	if (flip == 0) {
		southTurn = true;
	} else {
		southTurn = false;
	}
}

// Copy constructor
Game::Game(const Game& other) : m_Board(other.m_Board) {
	this->m_north = other.m_north;
	this->m_south = other.m_south;
	this->southTurn = other.southTurn;
}

// Assignment operator
Game& Game::operator=(const Game& other) {
	if (this == &other) {
		return *this;
	}
	this->m_Board = other.m_Board;
	this->m_north = other.m_north;
	this->m_south = other.m_south;
	this->southTurn = other.southTurn;
	return *this;
}

// Displays the current board state
void Game::display() const {
	cout << "\t\t" + m_north->name() << endl;
	cout << "\t\t";
	for (int i = 1; i <= m_Board.holes(); i++) {
		cout << i << " ";
	}
	cout << endl;
	cout << "\t\t";
	for (int i = 1; i <= m_Board.holes(); i++) {
		cout << "--";
	}
	cout << endl;
	cout << "\t\t";
	for (int i = 1; i <= m_Board.holes(); i++) {
		cout << m_Board.beans(NORTH, i) << " ";
	}
	cout << endl;
	cout << m_north->name() + "'s pot " << m_Board.beans(NORTH, 0) << "\t\t" << m_Board.beans(SOUTH, 0) << " " + m_south->name() + "'s pot" << endl;
	cout << "\t\t";
	for (int i = 1; i <= m_Board.holes(); i++) {
		cout << m_Board.beans(SOUTH, i) << " ";
	}
	cout << endl;
	cout << "\t\t";
	for (int i = 1; i <= m_Board.holes(); i++) {
		cout << "--";
	}
	cout << endl;
	cout << "\t\t";
	for (int i = 1; i <= m_Board.holes(); i++) {
		cout << i << " ";
	}
	cout << endl;
	cout << "\t\t" + m_south->name() << endl << endl;
}

// Checks the game's board state
// If the game is over, over = true
// If the game has a winner, hasWinner = true and winner is set to the winning side
void Game::status(bool& over, bool& hasWinner, Side& winner) const {
	if (m_Board.beansInPlay(NORTH) != 0 && m_Board.beansInPlay(SOUTH) != 0) {		// check if the game is over (no more beans in play)
		over = false;
	}
	else {
		over = true;
		if (m_Board.beans(NORTH, 0) == m_Board.beans(SOUTH, 0)) {	// check if there is a tie
			hasWinner = false;
		}
		else {	// find the winner and set winner
			hasWinner = true;
			if (m_Board.beans(NORTH, 0) > m_Board.beans(SOUTH, 0)) {
				winner = NORTH;
			}
			else {
				winner = SOUTH;
			}
		}
	}
}

// Conduct a Game move
// Return false if the game is over, true otherwise
bool Game::move() {
	int move;
	Side endSide;
	int endHole;
	bool notEndTurn = true;
	Player* temp;
	Side mySide;
	Side notMySide;
	// set temp to the player whose turn it is 
	if (southTurn) {
		temp = m_south;
		mySide = SOUTH;
		notMySide = NORTH;
	}
	else {
		temp = m_north;
		mySide = NORTH;
		notMySide = SOUTH;
	}
	while (notEndTurn) {
		if (m_Board.beansInPlay(mySide) == 0) {	// check if there is a move to make
			for (int i = 1; i <= m_Board.holes(); i++) {
				m_Board.moveToPot(notMySide, i, notMySide);
			}
			return false;
		}
		if (temp->isInteractive()) {	// if the player is interactive, display the board
			this->display();
		}
		move = temp->chooseMove(m_Board, mySide);		// have the player choose a move
		if (move == -1) {
			return false;
		}
		m_Board.sow(mySide, move, endSide, endHole);		// sow the move that the player chose
		if (endSide == mySide && endHole == 0) {		// if it ended in the player's pot, take another move
			continue;
		}
		if (endSide == mySide) {
			if (m_Board.beans(mySide, endHole) == 1 && m_Board.beans(notMySide, endHole) != 0) {	// check if it ended on the player's side in an empty hole, opposite some beans
				// add the beans on both sides to the player's pot
				m_Board.moveToPot(notMySide, endHole, mySide);
				m_Board.moveToPot(mySide, endHole, mySide);
			}
		}
		// check if the game is over after the move
		if (m_Board.beansInPlay(mySide) == 0) {
			for (int i = 1; i <= m_Board.holes(); i++) {
				m_Board.moveToPot(notMySide, i, notMySide);
			}
			return false;
		} else if (m_Board.beansInPlay(notMySide) == 0) {
			for (int i = 1; i <= m_Board.holes(); i++) {
				m_Board.moveToPot(mySide, i, mySide);
			}
			return false;
		}
		notEndTurn = false;
	}
	southTurn = !southTurn;		// switch turns
	return true;
}

// Play the game out until the game is over
void Game::play() {
	bool isOver = false;
	bool hasWinner = false;
	Side winner;
	this->status(isOver, hasWinner, winner);		// check the status
	while (!isOver) {
		// display who's turn it is and the board if they are interactive
		if (southTurn) {
			cout << "It is " << m_south->name() << "'s turn." << endl;
			if (!m_south->isInteractive()) {
				this->display();
			}
		}
		else {
			cout << "It is " << m_north->name() << "'s turn." << endl;
			if (!m_north->isInteractive()) {
				this->display();
			}
		}
		this->move();	// make a move
		if (!m_north->isInteractive() && !m_south->isInteractive()) {	// if both player's are not interactive, wait for an enter
			cout << "Press enter." << endl;
			string s;
			getline(cin, s);
		}
		this->status(isOver, hasWinner, winner);	// check the status
	}
	// display the outcome of the game
	if (!hasWinner) {
		cout << "The game ended in a tie." << endl;
	}
	else {
		if (winner == SOUTH) {
			cout << "The winner was " + m_south->name() << endl;
		}
		else {
			cout << "The winner was " + m_north->name() << endl;
		}
	}
}

// Return the number of beans in a hole
int Game::beans(Side s, int hole) const {
	return m_Board.beans(s, hole);
}