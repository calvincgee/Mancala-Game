#include "../Header Files/Player.h"
#include <iostream>

using namespace std;

//==========================================================================
// AlarmClock ac(numMilliseconds);  // Set an alarm clock that will time out
//                                  // after the indicated number of ms
// if (ac.timedOut())  // Will be false until the alarm clock times out; after
//                     // that, always returns true.
//==========================================================================

#include <chrono>
#include <future>
#include <atomic>

class AlarmClock
{
public:
	AlarmClock(int ms)
	{
		m_timedOut = false;
		m_isRunning = true;
		m_alarmClockFuture = std::async([=]() {
			for (int k = 0; k < ms && m_isRunning; k++)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (m_isRunning)
				m_timedOut = true;
		});
	}

	~AlarmClock()
	{
		m_isRunning = false;
		m_alarmClockFuture.get();
	}

	bool timedOut() const
	{
		return m_timedOut;
	}

	AlarmClock(const AlarmClock&) = delete;
	AlarmClock& operator=(const AlarmClock&) = delete;
private:
	std::atomic<bool> m_isRunning;
	std::atomic<bool> m_timedOut;
	std::future<void> m_alarmClockFuture;
};

// Return the move that the HumanPlayer chooses
// Returns -1 and prompts again if input is invalid
int HumanPlayer::chooseMove(const Board& b, Side s) const{
	if (b.beansInPlay(s) == 0) {	// check if there is a move to be made
		return -1;
	}
	int input;
	cout << "What is your move?" << endl;
	cin >> input;
	while (input <= 0 || input > b.holes() || b.beans(s, input) == 0) {	// checks if the input is valid and if there is a bean in that hole
		cout << "Invalid input." << endl;
		cin >> input;
	}
	return input;
}

// Returns the move that the BadPlayer chooses
// Returns the left most available hole, regardless of size
int BadPlayer::chooseMove(const Board& b, Side s) const{
	if (b.beansInPlay(s) == 0) {	// checks if there is a move to make
		return -1;
	}
	else {
		int answer = 1;
		while (answer <= b.holes()) {
			if (b.beans(s, answer) != 0) {
				break;
			}
			answer++;
		}
		return answer;
	}
}

// Returns the move that the SmartPlayer chooses
int SmartPlayer::chooseMove(const Board&b, Side s) const {
	int myMove;
	AlarmClock ac(4900);
	findMove(ac, b, s, myMove, 0);
	return myMove;
}

// Returns the value of the best move to make given a board state and side
// If it returns 1000, that means that that move corresponds to a win, if it reurns -1000 that means that that move corresponds to a loss
// Any in between corresponds to the difference between the two pots
// Changes bestHole to the best choice
// Does not search more than 5 layers
// AlarmClock set to 4.9 seconds, telling the program to wrap up
int SmartPlayer::findMove(AlarmClock& ac, const Board& b, Side whooseTurn, int& bestHole, int depth) const{
	Board temp(b);
	Side mySide;
	Side notMySide;
	int bestVal;
	// if ac has timed out, return the value so far
	if (ac.timedOut()) {
		bestHole = -1;
		return determineValue(temp, whooseTurn, depth);
	}
	// if the depth is even, we will be trying to maximize the value, thus start with lowest value possible
	if (depth % 2 == 0) {
		bestVal = -1000;
	}
	// if the depth is odd, we will be trying to minimize the value, thus start with the highest value possible
	else {
		bestVal = 1000;
	}
	// set the sides
	if (whooseTurn == SOUTH) {
		mySide = SOUTH;
		notMySide = NORTH;
	}
	else {
		mySide = NORTH;
		notMySide = SOUTH;
	}
	// if there is no move to be made, find out which side has won and return the corresponding value, or if a tie occurred
	if (temp.beansInPlay(mySide) == 0) {
		for (int i = 1; i <= temp.holes(); i++) {
			temp.moveToPot(notMySide, i, notMySide);
		}
		bestHole = -1;
		if (determineValue(temp, whooseTurn, depth) < 0) {
			return -1000;
		}
		else if (determineValue(temp, whooseTurn, depth) > 0) {
			return 1000;
		}
		else {
			return determineValue(temp, whooseTurn, depth);
		}
	}
	else if (temp.beansInPlay(notMySide) == 0) {
		for (int i = 1; i <= temp.holes(); i++) {
			temp.moveToPot(mySide, i, mySide);
		}
		bestHole = -1;
		if (determineValue(temp, whooseTurn, depth) < 0) {
			return -1000;
		}
		else if (determineValue(temp, whooseTurn, depth) > 0) {
			return 1000;
		}
		else {
			return determineValue(temp, whooseTurn, depth);
		}
	}
	// does not search depth of 6, returns the value at that point
	if (depth == 6) {
		bestHole = -1;
		return determineValue(temp, whooseTurn, depth);
	}
	for (int i = 1; i <= temp.holes(); i++) {		// loop through each hole
		Board backup(temp);	// create a Board that is a copy of the current board state
		bool again = false;
		int tempVal;
		int tempHole = bestHole;	// remember the current bestHole
		if (temp.beans(mySide, i) != 0) {	// check if i is a valid move
			again = simMove(temp, mySide, i);	// simulate the move with the copied board
			// if the turn results in another move, recursively find the next move
			// NOTE: the input Board is updated to include the simulated move
			// NOTE: depth and side stay the same because the turn is not over
			if (again) {
				tempVal = findMove(ac, temp, mySide, bestHole, depth);
			}
			// recursively find the next move
			// NOTE: the input Board is updated to include the simulated move
			// NOTE: side is switched and depth is incremented because it is now time to simulate the other player's move
			else {
				tempVal = findMove(ac, temp, notMySide, bestHole, depth + 1);
			}
			temp = backup;		// "undo" the moves made
			// if the depth is even, we will be trying to maximize the value
			if (depth % 2 == 0) {
				if (tempVal >= bestVal) {
					bestVal = tempVal;
					bestHole = i;		// set bestHole to the move that produced the desired value
				}
				else {
					bestHole = tempHole;		// reset bestHole
				}
			}
			// if the depth is odd, we will be trying to minimize the value
			else {
				if (tempVal <= bestVal) {
					bestVal = tempVal;
					bestHole = i;		// set bestHole to the move that produced the desired value
				}
				else {
					bestHole = tempHole;		// reset bestHole
				}
			}
		}
	}
	return bestVal;
}

// Carry out the move on the given board, side, and hole
// Returm true if the player get's another turn, return false otherwise
bool SmartPlayer::simMove(Board& b, Side s, int hole) const{
	int move = hole;
	Side endSide;
	int endHole;
	Side mySide;
	Side notMySide;
	// setup sides
	if (s == SOUTH) {
		mySide = SOUTH;
		notMySide = NORTH;
	}
	else {
		mySide = NORTH;
		notMySide = SOUTH;
	}
	// check if there is a move to make
	if (b.beansInPlay(mySide) == 0) {
		for (int i = 1; i <= b.holes(); i++) {
			b.moveToPot(notMySide, i, notMySide);
		}
		return false;
	}
	b.sow(mySide, move, endSide, endHole);		// carry out the move on the board
	if (endSide == mySide && endHole == 0) {		// check if the player gets another move
		return true;
	}
	if (endSide == mySide) {
		if (b.beans(mySide, endHole) == 1 && b.beans(notMySide, endHole) != 0) {		// check if it ended on the player's side in an empty hole, opposite some beans
			// add the beans on both sides to the player's pot
			b.moveToPot(notMySide, endHole, mySide);
			b.moveToPot(mySide, endHole, mySide);
		}
	}
	// check if the game is over after the move
	if (b.beansInPlay(mySide) == 0) {
		for (int i = 1; i <= b.holes(); i++) {
			b.moveToPot(notMySide, i, notMySide);
		}
	}
	else if (b.beansInPlay(notMySide) == 0) {
		for (int i = 1; i <= b.holes(); i++) {
			b.moveToPot(mySide, i, mySide);
		}
	}
	return false;
}

// Returns the difference in pots given a board state
// Always returns difference in terms of the original side
int SmartPlayer::determineValue(const Board& b, Side whooseTurn, int depth) const{
	// depending on depth and input side, determine the original side and output the difference in terms of that side
	if (depth % 2 == 0) {
		if (whooseTurn == SOUTH) {
			return b.beans(SOUTH, 0) - b.beans(NORTH, 0);
		}
		else {
			return b.beans(NORTH, 0) - b.beans(SOUTH, 0);
		}
	}
	else {
		if (whooseTurn == SOUTH) {
			return b.beans(NORTH, 0) - b.beans(SOUTH, 0);
		}
		else {
			return b.beans(SOUTH, 0) - b.beans(NORTH, 0);
		}
	}
}