#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Board.h"

class AlarmClock;

class Player {
public:
	Player(std::string name) : m_name(name) {}
	std::string name() const { return m_name; }
	virtual bool isInteractive() const { return false; }
	virtual int chooseMove(const Board& b, Side s) const = 0;
	virtual~Player() { }
private:
	std::string m_name;
};

class HumanPlayer : public Player {
public:
	HumanPlayer(std::string name) : Player(name) {}
	bool isInteractive() const { return true; }
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~HumanPlayer() {}
};

class BadPlayer : public Player {
public:
	BadPlayer(std::string name) : Player(name) {}
	bool isInteractive() const { return false; }
	virtual int chooseMove(const Board& b, Side s) const;
	virtual ~BadPlayer() {}
};

class SmartPlayer : public Player {
public:
	SmartPlayer(std::string name) : Player(name) {}
	bool isInteractive() const { return false; }
	virtual int chooseMove(const Board&b, Side s) const;
	virtual ~SmartPlayer() {}
private:
	int findMove(AlarmClock& ac, const Board& b, Side whooseTurn, int& bestHole, int depth) const;
	bool simMove(Board& b, Side s, int hole) const;
	int determineValue(const Board& b, Side whooseTurn, int depth) const;
};
#endif // !PLAYER_H
